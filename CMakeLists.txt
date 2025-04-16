cmake_minimum_required(VERSION 3.26)
project(TermSage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

# Add source files
set(SOURCES
        src/main.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CURL::libcurl
        OpenSSL::SSL
        OpenSSL::Crypto
)

# If on Unix-like systems, link against pthread
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()