#ifndef OLLAMA_HPP
#define OLLAMA_HPP

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <numeric>
#include <memory>

// Include the nlohmann/json library
#include "./nlohmann/json.hpp"

// Include the cpp-httplib library
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "./cpp-httplib/httplib.h"

namespace ollama {
    using json = nlohmann::json;

    // Global configuration options
    inline bool use_exceptions = true;
    inline bool log_requests = false;
    inline bool log_replies = false;

    // Exception classes
    class exception : public std::exception {
    public:
        exception(const std::string& message) : message(message) {}
        const char* what() const noexcept override { return message.c_str(); }
    private:
        std::string message;
    };

    class invalid_json_exception : public exception {
    public:
        invalid_json_exception(const std::string& message) : exception(message) {}
    };

    // Message types
    enum class message_type { generate, chat };

    // Messages class for chat API
    class messages {
    private:
        std::vector<json> messages_array;
        
    public:
        messages() = default;
        
        void add_message(const std::string& role, const std::string& content) {
            json message;
            message["role"] = role;
            message["content"] = content;
            messages_array.push_back(message);
        }
        
        void add_system(const std::string& content) {
            add_message("system", content);
        }
        
        void add_user(const std::string& content) {
            add_message("user", content);
        }
        
        void add_assistant(const std::string& content) {
            add_message("assistant", content);
        }
        
        const std::vector<json>& get_messages() const {
            return messages_array;
        }
    };

    // Request class
    class request : public json {
    public:
        // Constructor for generate API
        request(const std::string& model, const std::string& prompt, 
                const json& options = nullptr, bool stream = false,
                const std::vector<std::string>& images = std::vector<std::string>()) {
            (*this)["model"] = model;
            (*this)["prompt"] = prompt;
            (*this)["stream"] = stream;
            
            if (!images.empty()) {
                (*this)["images"] = images;
            }
            
            if (options != nullptr) {
                for (auto& item : options.items()) {
                    (*this)[item.key()] = item.value();
                }
            }
        }
        
        // Constructor for chat API
        request(const std::string& model, const messages& msgs, 
                const json& options = nullptr, bool stream = false,
                const std::string& format = "json", 
                const std::string& keep_alive = "5m") {
            (*this)["model"] = model;
            (*this)["messages"] = json(msgs.get_messages());
            (*this)["stream"] = stream;
            (*this)["format"] = format;
            (*this)["keep_alive"] = keep_alive;
            
            if (options != nullptr) {
                for (auto& item : options.items()) {
                    (*this)[item.key()] = item.value();
                }
            }
        }
    };

    // Response class
    class response {
    public:
        response() {
            valid = false;
        }
        
        response(const std::string& json_str, message_type type = message_type::generate) {
            this->error_string = "";
            this->type = type;
            
            try {
                this->json_data = json::parse(json_str);
                this->valid = true;
            } catch(...) {
                if (ollama::use_exceptions) 
                    throw ollama::invalid_json_exception("Unable to parse JSON string:" + json_str);
                valid = false;
            }
        }
        
        const std::string& as_json_string() const {
            return simple_string;
        }
        
        std::string as_simple_string() const {
            if (!valid) return "";
            
            if (type == message_type::generate) {
                if (json_data.contains("response"))
                    return json_data["response"].get<std::string>();
            } else if (type == message_type::chat) {
                if (json_data.contains("message") && 
                    json_data["message"].contains("content"))
                    return json_data["message"]["content"].get<std::string>();
            }
            return "";
        }
        
        const json& as_json() const {
            return json_data;
        }
        
        bool has_error() const {
            return json_data.contains("error") && !json_data["error"].get<std::string>().empty();
        }
        
        std::string get_error() const {
            if (has_error())
                return json_data["error"].get<std::string>();
            return "";
        }
        
        operator std::string() const { 
            return this->as_simple_string(); 
        }

    private:
        std::string simple_string;
        std::string error_string;
        json json_data;        
        message_type type;
        bool valid;
    };
}

class Ollama {
    using json = nlohmann::json;

public:
    Ollama(const std::string& url) {
        this->server_url = url;
        this->cli = new httplib::Client(url);
        this->setReadTimeout(120);
    }

    Ollama() : Ollama("http://localhost:11434") {}
    ~Ollama() { delete this->cli; }

    void setReadTimeout(int seconds) {
        this->cli->set_read_timeout(seconds, 0);
    }

    bool is_running() {
        auto res = cli->Get("/");
        if (res && res->body == "Ollama is running") return true;
        return false;
    }

    std::vector<std::string> list_running_models() {
        std::vector<std::string> models;
        json json_response = running_model_json();
        
        for (auto& model : json_response["models"]) {
            models.push_back(model["name"]);
        }
        return models;
    }

    json running_model_json() {
        json models;
        if (auto res = cli->Get("/api/ps")) {
            if (ollama::log_replies) std::cout << res->body << std::endl;
            models = json::parse(res->body);
        } else { 
            if (ollama::use_exceptions) 
                throw ollama::exception("No response returned from server when querying running models: " + 
                                      std::to_string(static_cast<int>(res.error())));
        }
        return models;
    }

    std::vector<std::string> list_models() {
        std::vector<std::string> models;
        json json_response = list_model_json();
        
        for (auto& model : json_response["models"]) {
            models.push_back(model["name"]);
        }
        return models;
    }

    json list_model_json() {
        json models;
        if (auto res = cli->Get("/api/tags")) {
            if (ollama::log_replies) std::cout << res->body << std::endl;
            models = json::parse(res->body);
        } else { 
            if (ollama::use_exceptions) 
                throw ollama::exception("No response returned from server when querying model list: " + 
                                      std::to_string(static_cast<int>(res.error())));
        }
        return models;
    }

    bool load_model(const std::string& model) {
        json request;
        request["model"] = model;
        std::string request_string = request.dump();
        if (ollama::log_requests) std::cout << request_string << std::endl;

        // Send a blank request with the model name to instruct ollama to load the model into memory.
        if (auto res = this->cli->Post("/api/generate", request_string, "application/json")) {
            if (ollama::log_replies) std::cout << res->body << std::endl;
            json response = json::parse(res->body);
            return response["done"];        
        } else { 
            if (ollama::use_exceptions) 
                throw ollama::exception("No response returned from server when loading model: " + 
                                      std::to_string(static_cast<int>(res.error())));
        }
        return false;
    }

    ollama::response chat(const std::string& model, const ollama::messages& messages, json options=nullptr) {
        ollama::response response;

        // Create request object
        json request;
        request["model"] = model;
        request["messages"] = json(messages.get_messages());
        request["stream"] = false;
        
        if (options != nullptr) {
            for (auto& item : options.items()) {
                request[item.key()] = item.value();
            }
        }
        
        std::string request_string = request.dump();
        if (ollama::log_requests) std::cout << request_string << std::endl;      

        if (auto res = this->cli->Post("/api/chat", request_string, "application/json")) {
            if (ollama::log_replies) std::cout << res->body << std::endl;

            response = ollama::response(res->body, ollama::message_type::chat);
            if (response.has_error()) { 
                if (ollama::use_exceptions) 
                    throw ollama::exception("Ollama response returned error: " + response.get_error());
            }
        } else {
            if (ollama::use_exceptions) 
                throw ollama::exception("No response returned from server " + this->server_url + 
                                      ". Error: " + std::to_string(static_cast<int>(res.error())));
        }

        return response;
    }

    // Include other methods as needed

private:
    std::string server_url;
    httplib::Client* cli;
};

#endif // OLLAMA_HPP 