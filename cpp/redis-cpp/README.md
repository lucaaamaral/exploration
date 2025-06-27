# Redis-Cpp Exploration

This directory contains exploration code for the [redis-cpp](https://github.com/tdv/redis-cpp) library, a C++17 client for Redis. The purpose of this exploration is to understand the library's API, usability, and setup process as part of a broader comparison with another Redis C++ library.

## Overview

Redis-cpp is a lightweight C++ client for Redis, designed to provide a simple and efficient interface for interacting with Redis servers. It focuses on core Redis functionality with a straightforward API. This exploration code provides a basic example to connect to a Redis server and perform simple operations.

## Setup and Build Instructions

To build and run the exploration code, you can either set up locally or use Docker for a containerized environment. Follow the appropriate steps below:

### Local Setup

1. **Ensure Prerequisites**:
   - A Redis server must be running locally on the default port (6379) or configured accordingly in the code.
   - Install necessary build tools like CMake (version 3.10 or higher) and a C++17-compatible compiler (e.g., GCC or Clang).

2. **Build the Library (if not already built)**:
   - If the redis-cpp library is not pre-built, navigate to `lib/redis-cpp` and follow the library's build instructions to compile it. Typically, this involves:
     ```bash
     mkdir build && cd build
     cmake ..
     make && make install
     ```
   - Ensure the library is installed or linked correctly for use in this project.

3. **Build the Exploration Code**:
   - From the `cpp/redis-cpp` directory, create a build directory and run CMake:
     ```bash
     mkdir build && cd build
     cmake ..
     make
     ```
   - This will build the `basic_example` executable in the `bin` directory within the build folder.

4. **Run the Example**:
   - Ensure your Redis server is running, then execute the example:
     ```bash
     ./bin/basic_example
     ```
   - Alternatively, use the custom CMake target:
     ```bash
     make run
     ```

### Docker Setup

1. **Ensure Docker is Installed**:
   - Make sure Docker is installed on your system and the Docker daemon is running.

2. **Start the Redis Server Container**:
   - Navigate to the `tools/redis` directory and start the Redis server container using the Makefile:
     ```bash
     cd tools/redis
     make start
     ```
   - This will start a Redis server on the default port (6379) within a Docker network using the official Redis image.

3. **Run Tests Using Makefile**:
   - Return to the `cpp/redis-cpp` directory and use the Makefile to run tests:
     ```bash
     cd ../../cpp/redis-cpp
     make test
     ```
   - This will build the Docker image for the exploration code and run the container, linking it to the Redis server network to execute the basic example.

4. **Build the Exploration Code Image (Optional)**:
   - If you prefer to build the image manually:
     ```bash
     docker build -t redis-cpp-exploration .
     ```

5. **Run the Exploration Code Container (Optional)**:
   - If you prefer to run the container manually:
     ```bash
     docker run --network redis-network redis-cpp-exploration
     ```
   - This will execute the basic example, connecting to the Redis server running in the separate container.

## Basic Example Code

The file `src/basic_example.cpp` contains a simple program demonstrating basic interaction with a Redis server using redis-cpp:

- **Connection**: Establishes a connection to a Redis server at `127.0.0.1:6379` using a stream.
- **SET and GET Operations**: Sets a key-value pair (`test_key` with value `Hello, Redis!`) and retrieves it, printing the result.
- **INCR Operation**: Sets a counter to 0 and increments it, displaying the new value.
- **Error Handling**: Wraps operations in a try-catch block to handle potential exceptions.

This example serves as a starting point to understand the library's syntax and basic functionality.

## Drawbacks of redis-cpp Implementation

Based on the exploration and challenges encountered in the `basic_example.cpp`, the following drawbacks of the redis-cpp library have been identified:
- **Complex Error Handling**: The library requires explicit exception handling for null or unexpected response types, which can complicate simple operations and lead to runtime errors if not managed properly.
- **Command Formatting Sensitivity**: Commands must be formatted with separate arguments, which can be error-prone compared to more intuitive string-based command inputs.
- **Limited Type Safety**: Certain type-checking methods are inaccessible or not straightforward, making it harder to safely handle responses without risking exceptions.
- **Documentation and Usability**: The need for specific workarounds in the example code suggests that the library may lack comprehensive documentation or user-friendly abstractions for common Redis operations.

## Troubleshooting

- **Include Path Errors in VSCode**: If you encounter errors like `cannot open source file "redis-cpp/execute.h"`, ensure the library is built and installed correctly. Update the VSCode C/C++ extension include path settings if necessary by adding the path to the library headers (e.g., `/path/to/exploration/cpp/redis-cpp/lib/redis-cpp/include`).
- **Linking Errors**: Ensure the redis-cpp library is installed and linked correctly in the CMakeLists.txt. You may need to adjust library paths based on your system configuration.
- **Redis Server Not Running**: Verify that a Redis server is active and accessible at the specified address and port.

## Next Steps

This exploration code will be expanded to include more comprehensive examples and performance benchmarking metrics.
