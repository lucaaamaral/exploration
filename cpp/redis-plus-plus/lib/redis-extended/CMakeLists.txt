cmake_minimum_required(VERSION 3.10)
project(redis-extended LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for redis-plus-plus
include_directories(${CMAKE_SOURCE_DIR}/lib/redis-plus-plus/src)

# Source files
set(SOURCES
    Logging.cpp
    RedisKeyManager.cpp
)

# Create static library
add_library(redis-extended STATIC ${SOURCES})

# Link against redis-plus-plus
target_link_libraries(redis-extended PUBLIC 
    hiredis 
    redis++
)

# Install library
install(TARGETS redis-extended
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Install headers
install(FILES Logging.h RedisKeyManager.h
        DESTINATION include/redis-extended)
