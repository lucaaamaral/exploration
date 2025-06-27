cmake_minimum_required(VERSION 3.10)

project(RedisPlusPlusBenchmark LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Threads REQUIRED)
find_package(benchmark REQUIRED)

# Include directories for redis-plus-plus
include_directories(${CMAKE_SOURCE_DIR}/../lib/redis-plus-plus/src)

# Add benchmark executable
add_executable(redis_plus_plus_benchmark
    src/set_benchmark.cpp
    src/get_benchmark.cpp
    src/del_benchmark.cpp
    src/incr_benchmark.cpp
    src/exists_benchmark.cpp
    src/expire_benchmark.cpp
    src/ttl_benchmark.cpp
    src/decr_benchmark.cpp
    src/hset_benchmark.cpp
    src/hget_benchmark.cpp
    src/hmset_benchmark.cpp
    src/hmget_benchmark.cpp
    src/lpush_benchmark.cpp
    src/lpop_benchmark.cpp
    src/rpush_benchmark.cpp
    src/rpop_benchmark.cpp
    src/sadd_benchmark.cpp
    src/smembers_benchmark.cpp
    src/scard_benchmark.cpp
    src/sismember_benchmark.cpp
    # Add more benchmark files for other operations as needed
)

# Link libraries
target_link_libraries(redis_plus_plus_benchmark
    Threads::Threads
    benchmark::benchmark
    benchmark::benchmark_main
    hiredis
    redis++
)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Custom target to run the benchmarks
add_custom_target(run_benchmarks
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/redis_plus_plus_benchmark
    DEPENDS redis_plus_plus_benchmark
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running redis-plus-plus benchmarks..."
)
