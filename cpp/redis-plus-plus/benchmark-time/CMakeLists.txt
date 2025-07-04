# CMakeLists.txt for benchmark-time suite in redis-plus-plus
cmake_minimum_required(VERSION 3.10)

# Define project name for benchmark-time
project(redis_plus_plus_benchmark_time LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Threads REQUIRED)

# Include directories for redis-plus-plus
include_directories(${CMAKE_SOURCE_DIR}/../lib/redis-plus-plus/src)

# Define include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Define source files
set(SOURCES
    src/main.cpp
    src/Statistics.cpp
    src/CsvExporter.cpp
    src/PublishBenchmark.cpp
    src/SubscribeBenchmark.cpp
)
# Note: Header-only implementations for new benchmarks do not need separate source files

# Create executable for benchmark-time
add_executable(benchmark_time ${SOURCES})

# Define project name as a compile-time macro
target_compile_definitions(benchmark_time PRIVATE PROJECT_NAME="${PROJECT_NAME}")

# Link libraries
target_link_libraries(benchmark_time
    redis++
    hiredis
    Threads::Threads
)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install the executable
install(TARGETS benchmark_time DESTINATION bin)

# Custom target to run the benchmark
add_custom_target(run_benchmark_time
    COMMAND ${CMAKE_BINARY_DIR}/bin/benchmark_time
    DEPENDS benchmark_time
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running benchmark-time for redis-plus-plus"
)
