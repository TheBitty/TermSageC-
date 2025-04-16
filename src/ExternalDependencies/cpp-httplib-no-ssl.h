//
// Modified cpp-httplib without OpenSSL dependencies
// Based on the original httplib.h by Yuji Hirose
// Modified for use in TermSage project
//

#ifndef CPP_HTTPLIB_NO_SSL_H
#define CPP_HTTPLIB_NO_SSL_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <functional>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace httplib {

// Error codes
enum class Error {
  Success = 0,
  Unknown,
  Connection,
  Timeout,
  Canceled,
  Read,
  Write,
  ExceedRedirectCount,
  Compression,
  ConnectionClosed
};

inline const char* to_string(Error error) {
  switch (error) {
  case Error::Success: return "Success";
  case Error::Connection: return "Connection";
  case Error::Timeout: return "Timeout";
  case Error::Canceled: return "Canceled";
  case Error::Read: return "Read";
  case Error::Write: return "Write";
  case Error::ExceedRedirectCount: return "ExceedRedirectCount";
  case Error::Compression: return "Compression";
  case Error::ConnectionClosed: return "ConnectionClosed";
  case Error::Unknown: return "Unknown";
  default: return "Invalid";
  }
}

// HTTP status codes
enum class StatusCode {
  OK_200 = 200,
  Created_201 = 201,
  BadRequest_400 = 400,
  Unauthorized_401 = 401,
  Forbidden_403 = 403,
  NotFound_404 = 404,
  InternalServerError_500 = 500
};

// Request headers
using Headers = std::multimap<std::string, std::string>;

// Request/Response body
using Body = std::string;

// Request class
class Request {
public:
  std::string method;
  std::string path;
  Headers headers;
  Body body;
};

// Response class
class Response {
public:
  int status = -1;
  Headers headers;
  Body body;
};

// Client class - Simplified version with just what we need for the Ollama API
class Client {
public:
  Client(const std::string& host, int port = -1) : host_(host), port_(port) {
    if (port_ == -1) {
      // Check if host includes port (e.g., "localhost:11434")
      auto pos = host_.find(':');
      if (pos != std::string::npos) {
        try {
          port_ = std::stoi(host_.substr(pos + 1));
          host_ = host_.substr(0, pos);
        } catch (const std::exception&) {
          // If port parsing fails, use default
          port_ = 80;
        }
      } else {
        port_ = 80; // Default HTTP port
      }
    }
  }

  virtual ~Client() {}

  void set_read_timeout(time_t sec, time_t usec = 0) {
    read_timeout_sec_ = sec;
    read_timeout_usec_ = usec;
  }

  // Simple GET request
  std::shared_ptr<Response> Get(const std::string& path) {
    return send_request("GET", path, "", "");
  }

  // Simple POST request with JSON body
  std::shared_ptr<Response> Post(const std::string& path, const std::string& body, const std::string& content_type) {
    return send_request("POST", path, body, content_type);
  }

private:
  std::string host_;
  int port_;
  time_t read_timeout_sec_ = 300;
  time_t read_timeout_usec_ = 0;

  // Simplified request sending function
  std::shared_ptr<Response> send_request(const std::string& method, const std::string& path, 
                                         const std::string& body, const std::string& content_type) {
    auto res = std::make_shared<Response>();
    int sock = -1;

    // Resolve hostname
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* result;
    if (getaddrinfo(host_.c_str(), std::to_string(port_).c_str(), &hints, &result) != 0) {
      return res; // Failed to resolve hostname
    }

    // Create socket
    for (auto rp = result; rp != nullptr; rp = rp->ai_next) {
      sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (sock == -1) continue;

      if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) break;

      close(sock);
      sock = -1;
    }

    freeaddrinfo(result);

    if (sock == -1) {
      return res; // Failed to connect
    }

    // Set socket timeout
    struct timeval tv;
    tv.tv_sec = read_timeout_sec_;
    tv.tv_usec = read_timeout_usec_;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // Prepare request
    std::stringstream request_stream;
    request_stream << method << " " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << host_ << "\r\n";
    request_stream << "Connection: close\r\n";

    if (!content_type.empty() && !body.empty()) {
      request_stream << "Content-Type: " << content_type << "\r\n";
      request_stream << "Content-Length: " << body.size() << "\r\n";
    }

    request_stream << "\r\n";
    
    if (!body.empty()) {
      request_stream << body;
    }

    std::string request_str = request_stream.str();

    // Send request
    if (::send(sock, request_str.c_str(), request_str.size(), 0) == -1) {
      close(sock);
      return res; // Failed to send request
    }

    // Read response
    char buffer[4096];
    std::string response_str;
    ssize_t bytes_read;

    while ((bytes_read = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
      buffer[bytes_read] = '\0';
      response_str += buffer;
    }

    close(sock);

    // Parse response
    if (response_str.empty()) {
      return res; // Empty response
    }

    // Find header/body separator
    auto header_end = response_str.find("\r\n\r\n");
    if (header_end == std::string::npos) {
      return res; // Invalid response
    }

    // Parse status line
    auto header = response_str.substr(0, header_end);
    auto status_line_end = header.find("\r\n");
    auto status_line = header.substr(0, status_line_end);

    // "HTTP/1.1 200 OK"
    if (status_line.size() < 12) {
      return res; // Invalid status line
    }

    auto status_start = status_line.find(' ');
    auto status_end = status_line.find(' ', status_start + 1);
    if (status_start == std::string::npos || status_end == std::string::npos) {
      return res; // Invalid status line
    }

    auto status_str = status_line.substr(status_start + 1, status_end - status_start - 1);
    res->status = std::stoi(status_str);

    // Parse headers
    auto header_lines = header.substr(status_line_end + 2); // Skip status line
    std::istringstream header_stream(header_lines);
    std::string line;
    while (std::getline(header_stream, line)) {
      if (line.empty() || line == "\r") continue;
      
      // Remove \r if present
      if (line.back() == '\r') {
        line.pop_back();
      }

      auto colon_pos = line.find(':');
      if (colon_pos != std::string::npos) {
        auto key = line.substr(0, colon_pos);
        auto value = line.substr(colon_pos + 1);
        
        // Trim whitespace
        value.erase(0, value.find_first_not_of(' '));
        value.erase(value.find_last_not_of(' ') + 1);
        
        res->headers.emplace(key, value);
      }
    }

    // Extract body
    res->body = response_str.substr(header_end + 4);

    return res;
  }
};

} // namespace httplib

#endif // CPP_HTTPLIB_NO_SSL_H 