cmake_minimum_required(VERSION 3.10)

project(benchmark_lib LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the library
add_library(benchmark STATIC
    src/Statistics.cpp
    src/CsvExporter.cpp
    src/BenchmarkConfig.cpp
    src/BenchmarkRunner.cpp
)

# Specify include directories for the library
target_include_directories(benchmark PUBLIC include)

# Installation rules (optional, for installing the library)
install(TARGETS benchmark
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

# Optional: Enable warnings and optimizations
if (MSVC)
    target_compile_options(benchmark PRIVATE /W4)
else()
    target_compile_options(benchmark PRIVATE -Wall -Wextra -Wpedantic)
endif()
