#!/bin/bash

echo "Setting up dependencies for TermSage..."

# Check if Homebrew is installed
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# Install required packages
echo "Installing dependencies..."
brew install nlohmann-json openssl cpp-httplib || {
    echo "Failed to install dependencies with Homebrew."
    exit 1
}

# If cpp-httplib is not available via Homebrew, install it from source
if ! brew list cpp-httplib &> /dev/null; then
    echo "cpp-httplib not found in Homebrew. Installing from source..."
    mkdir -p src/ExternalDependencies/cpp-httplib
    curl -L -o httplib.h.download https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
    mv httplib.h.download src/ExternalDependencies/cpp-httplib/httplib.h
fi

# Create a json_lib directory for nlohmann/json header
mkdir -p src/ExternalDependencies/nlohmann
curl -L -o json.hpp.download https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
mv json.hpp.download src/ExternalDependencies/nlohmann/json.hpp

echo "Setup complete!" 