#include <iostream>
#include <thread>
#include <vector>
#include <cstdint>
#include "pubsub_routines.h"
#include "Logging.h"

// Macro for logging
#define LOG_MSG(level, thread_id, message) \
    std::cout << "[" << level << "] Thread " << static_cast<int>(thread_id) << ": " << message << std::endl

// Macro for delay configuration (in milliseconds)
#define DELAY 10

int main() {
    // Get Redis connection parameters from environment variables or use defaults
    const char* redis_host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    const char* redis_port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    
    // Configure Redis connection options with timeout
    sw::redis::ConnectionOptions opts;
    opts.host = redis_host;
    opts.port = std::stoi(redis_port);
    opts.socket_timeout = std::chrono::seconds(5); // Set timeout to 5 seconds
    
    redis_extended::logging::DefaultLogger logger;
    LOG_MSG("INFO", 0, "Threads will connect to Redis at " + opts.host + 
        ":" + std::to_string(opts.port) + " with timeout " + 
        std::to_string(opts.socket_timeout.count()) + "ms");

    // Shared channel for pub/sub operations
    const std::string shared_channel = "data:stream";

    // Number of threads for each operation type
    const int num_publishers = 3;
    const int num_subscribers = 2;
    const int total_threads = num_publishers + num_subscribers;

    // Vector to hold thread objects
    std::vector<std::thread> threads;

    // Counter for unique thread IDs
    uint8_t thread_id = 0;

    // Create publisher threads, each with its own Redis connection
    for (int i = 0; i < num_publishers; ++i) {
        std::thread pub_thread(publisher_thread, thread_id++, opts, shared_channel);
        threads.push_back(std::move(pub_thread));
        LOG_MSG("INFO", 0, "Started publisher thread with ID: " + std::to_string(static_cast<int>(thread_id - 1)));
    }

    // Create subscriber threads, each with its own Redis connection
    for (int i = 0; i < num_subscribers; ++i) {
        std::thread sub_thread(subscriber_thread, thread_id++, opts, shared_channel);
        threads.push_back(std::move(sub_thread));
        LOG_MSG("INFO", 0, "Started subscriber thread with ID: " + std::to_string(static_cast<int>(thread_id - 1)));
    }

    // Wait for all threads to complete
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    LOG_MSG("INFO", 0, "All threads completed. Pub/Sub proof of concept simulation finished.");

    return 0;
}
