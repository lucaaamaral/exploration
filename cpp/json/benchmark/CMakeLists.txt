cmake_minimum_required(VERSION 3.10)
project(json_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

# Add the nlohmann/json library as a subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/../lib/json nlohmann_json)

# Find Google Benchmark
find_package(benchmark REQUIRED)

# Add the source files
add_executable(json_benchmark
    src/parse_benchmark.cpp
    src/serialize_benchmark.cpp
    src/access_benchmark.cpp
)

# Link against Google Benchmark
target_link_libraries(json_benchmark 
    benchmark::benchmark
    nlohmann_json::nlohmann_json
)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
