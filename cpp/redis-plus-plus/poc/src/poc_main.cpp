#include <iostream>
#include <thread>
#include <vector>
#include <cstdint>
#include "poc_routines.h"

// Macro for delay configuration (in nanoseconds)
#define DELAY 10

int main() {
    // Get Redis connection parameters from environment variables or use defaults
    const char* redis_host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    const char* redis_port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    
    // Construct the connection string for threads to use
    std::string connection_string = "tcp://" + std::string(redis_host) + ":" + std::string(redis_port);
    
    std::cout << "Main thread acting as manager. Threads will connect to Redis at " << connection_string << std::endl;

    // Shared key to be accessed by multiple threads
    const std::string shared_key = "shared_data";
    const std::string lock_key = "lock_shared_data";

    // Number of threads for each operation type
    const int num_readers = 3;
    const int num_writers = 2;
    const int num_updaters = 2;
    const int total_threads = num_readers + num_writers + num_updaters;

    // Vector to hold thread objects
    std::vector<std::thread> threads;

    // Counter for unique thread IDs
    uint8_t thread_id = 0;

    // Create reader threads, each with its own Redis connection
    for (int i = 0; i < num_readers; ++i) {
        std::thread reader_thread(poc_read, thread_id++, connection_string, shared_key, lock_key);
        threads.push_back(std::move(reader_thread));
        std::cout << "Started reader thread with ID: " << static_cast<int>(thread_id - 1) << std::endl;
    }

    // Create writer threads, each with its own Redis connection
    for (int i = 0; i < num_writers; ++i) {
        std::thread writer_thread(poc_write, thread_id++, connection_string, shared_key, lock_key);
        threads.push_back(std::move(writer_thread));
        std::cout << "Started writer thread with ID: " << static_cast<int>(thread_id - 1) << std::endl;
    }

    // Create updater threads, each with its own Redis connection
    for (int i = 0; i < num_updaters; ++i) {
        std::thread updater_thread(poc_update, thread_id++, connection_string, shared_key, lock_key);
        threads.push_back(std::move(updater_thread));
        std::cout << "Started updater thread with ID: " << static_cast<int>(thread_id - 1) << std::endl;
    }

    // Wait for all threads to complete
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "All threads completed. Proof of concept simulation finished." << std::endl;

    return 0;
}
