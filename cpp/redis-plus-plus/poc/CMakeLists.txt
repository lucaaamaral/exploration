cmake_minimum_required(VERSION 3.10)

project(redis_plus_plus_poc LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Threads REQUIRED)

# Include directories for redis-plus-plus
include_directories(${CMAKE_SOURCE_DIR}/lib/redis-plus-plus/src)

# Add executable for proof of concept
add_executable(poc_main
    src/poc_main.cpp
    src/poc_routines.cpp
)

# Link libraries
target_link_libraries(poc_main
    hiredis
    redis++
    Threads::Threads
)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set output directory for the executable
set_target_properties(poc_main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
