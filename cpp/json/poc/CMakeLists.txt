cmake_minimum_required(VERSION 3.10)
project(json_poc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

# Add the nlohmann/json library as a subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/../lib/json nlohmann_json)
# Include directories for redis-plus-plus
include_directories(${CMAKE_SOURCE_DIR}/../lib/redis-plus-plus/src)
# Include directories for redis-extended
include_directories(${CMAKE_SOURCE_DIR}/../lib/redis-extended)

# Add the source files
add_executable(json_poc 
    src/poc_main.cpp 
    src/json_routines.cpp
)

# Link against libraries
target_link_libraries(json_poc 
    nlohmann_json::nlohmann_json
    hiredis
    redis++
    redis-extended
)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
