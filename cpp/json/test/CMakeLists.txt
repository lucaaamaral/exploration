cmake_minimum_required(VERSION 3.10)
project(json_test LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

# Add the nlohmann/json library as a subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/../lib/json nlohmann_json)

# Add the source files
add_executable(basic_example src/basic_example.cpp)

# Link libraries
target_link_libraries(basic_example
    nlohmann_json::nlohmann_json
)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

