# TermSage C++ Project Documentation

This document describes the implementation details, fixes applied, and guidance for using the TermSage C++ project on macOS.

## Project Overview

TermSage is a C++ application that interfaces with the Ollama API to provide chat functionality with locally-hosted AI models. The project uses a simplified HTTP client implementation to communicate with the Ollama server running on localhost.

## Dependencies

The project has the following dependencies:

1. **C++20 compatible compiler** - Apple Clang on macOS
2. **CMake** - For building the project (minimum version 3.16)
3. **CURL** - For HTTP request handling
4. **nlohmann/json** - For JSON parsing and manipulation
5. **Ollama** - Must be installed and running on the system

## Key Components

### 1. Custom HTTP Client

One of the main challenges in this project was implementing a simple HTTP client that doesn't rely on OpenSSL, as it was causing compatibility issues with the macOS compiler. We created a custom lightweight HTTP client implementation in `cpp-httplib-no-ssl.h` that:

- Uses only standard C++ libraries and POSIX sockets
- Implements basic HTTP GET and POST requests
- Handles HTTP response parsing
- Doesn't require OpenSSL or any other TLS/SSL libraries

### 2. Ollama API Wrapper

The `ollama_fixed.hpp` file provides a clean C++ wrapper around the Ollama API. It offers:

- Connection to the Ollama server
- Listing available and running models
- Chat functionality with context/history support
- Error handling with optional exceptions

### 3. CLI Chat Application

The `main.cpp` implements a simple command-line interface for chatting with Ollama models, including:

- Listing available models
- Model selection by number or name
- Interactive chat with history
- Error reporting

## Fixes Applied

### 1. OpenSSL Dependency Issues

The original implementation relied on the cpp-httplib library, which by default requires OpenSSL. This caused issues on macOS:

```
cannot open source file "openssl/err.h" (dependency of "ExternalDependencies/ollama_fixed.hpp")
```

**Solution:** We created a custom HTTP client implementation (`cpp-httplib-no-ssl.h`) that doesn't require OpenSSL, focusing only on the functionality needed for the Ollama API.

### 2. API Compatibility

The original error handling in the Ollama wrapper was incompatible with our simplified HTTP client:

```
class "std::__1::shared_ptr<httplib::Response>" has no member "error"
```

**Solution:** We updated the error handling logic to check response status codes instead of using the error member.

### 3. C++ Standard Library Issues

There were compatibility issues with certain GCC-specific implementations:

```
no member named '__cxx11' in namespace 'std'
```

**Solution:** We rewrote the problematic parts to use standard C++ features compatible with the Apple Clang compiler.

## Building on macOS

1. **Install dependencies:**
   ```bash
   brew install cmake nlohmann-json curl
   ```

2. **Ensure Ollama is installed and running:**
   ```bash
   # Install Ollama if not already installed
   brew install ollama
   
   # Start the Ollama server
   ollama serve
   ```

3. **Build the project:**
   ```bash
   mkdir -p build
   cd build
   cmake ..
   make
   ```

4. **Run the application:**
   ```bash
   ./TermSage
   ```

## macOS Compiler Considerations

The Apple Clang compiler has some differences from GCC/standard Clang:

1. **Headers and include paths:** macOS uses a different organization for system headers. In our CMakeLists.txt, we added specific include paths for macOS:
   ```cmake
   include_directories(
       ${CMAKE_SOURCE_DIR}/src
       ${CURL_INCLUDE_DIRS}
       /opt/homebrew/opt/openssl/include
   )
   ```

2. **C++ Standard Library implementation:** Apple's implementation of the C++ standard library has differences from GNU's libstdc++. We avoided using implementation-specific features and stuck to standard C++20 features.

3. **Compiler Flags:** We added macOS-specific compiler flags:
   ```cmake
   if(APPLE)
       target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
   endif()
   ```

## Usage

After building the application, you can use it to chat with any model available in your Ollama installation:

1. The application will list available models
2. Select a model by number or name
3. Start chatting - type your messages and press Enter
4. Type 'exit' to end the chat session

## Troubleshooting

1. **Linter errors about OpenSSL:** 
   - These are IDE-specific linter warnings and don't affect compilation
   - The program will still build and run correctly
   
2. **Connection errors:**
   - Ensure Ollama is running with `ollama serve`
   - Check that you have at least one model pulled with `ollama list`

3. **Model not found:**
   - Pull a model with `ollama pull modelname`
   - Common models: `ollama pull llama3`, `ollama pull gemma:7b`

## Future Improvements

1. Add streaming response support
2. Implement model parameter customization
3. Add multi-model conversations
4. Create a proper UI instead of CLI 