cmake_minimum_required(VERSION 3.10)
project(RedisPubSubPoC LANGUAGES CXX)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Threads REQUIRED)

# Include directories# Include directories for redis-plus-plus
include_directories(${CMAKE_SOURCE_DIR}/../lib/redis-plus-plus/src)
include_directories(${CMAKE_SOURCE_DIR}/../lib/redis-extended)


# Source files for the PoC
set(SOURCES
    src/main.cpp
    src/pubsub_routines.cpp
)

# Create executable
add_executable(pubsub_poc ${SOURCES})

# Link libraries
target_link_libraries(pubsub_poc
    redis-extended
    redis++
    hiredis
    Threads::Threads
)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable warnings and optimizations
target_compile_options(pubsub_poc PRIVATE -Wall -Wextra -O2)
