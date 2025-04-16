# TermSage

A C++ project for interfacing with Ollama API to work with machine learning models.

## Prerequisites

- C++20 compatible compiler
- CMake 3.16 or higher
- OpenSSL
- CURL
- Ollama installed and running on your system

## Setup

1. Clone the repository
```bash
git clone <repository-url>
cd TermSage
```

2. Run the setup script to install dependencies
```bash
chmod +x setup.sh
./setup.sh
```

3. Create a build directory and build the project
```bash
mkdir -p build && cd build
cmake ..
make
```

4. Run the application
```bash
./bin/TermSage
```

## Features

- Connect to local Ollama API
- List available models
- List running models
- More features coming soon!

## Dependencies

- nlohmann/json: JSON for Modern C++
- cpp-httplib: A C++ HTTP/HTTPS client/server library
- OpenSSL: For secure communications
- CURL: For HTTP requests

## Troubleshooting

1. Ensure Ollama is installed and running:
```bash
ollama serve
```

2. If you encounter build errors, check that all dependencies were properly installed:
```bash
./setup.sh
```

## License

[MIT License](LICENSE)
