cmake_minimum_required(VERSION 3.10)

project(RedisCppExploration LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Threads REQUIRED)

# Include directories for redis-cpp
include_directories(${CMAKE_SOURCE_DIR}/lib/redis-cpp/include)

# Add executable for basic example
add_executable(basic_example src/basic_example.cpp)

# Link libraries
target_link_libraries(basic_example
    Threads::Threads
    redis-cpp
)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Custom target to run the example
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basic_example
    DEPENDS basic_example
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running basic example..."
)
