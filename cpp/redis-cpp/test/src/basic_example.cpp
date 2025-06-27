#include <redis-cpp/execute.h>
#include <redis-cpp/stream.h>
#include <iostream>
#include <cstdlib>

int main() {
    // Get Redis connection parameters from environment variables or use defaults
    const char* redis_host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    const char* redis_port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    
    // Create a Redis connection stream
    std::shared_ptr<std::iostream> stream = rediscpp::make_stream(redis_host, redis_port);
    if (!stream) {
        std::cerr << "Failed to create Redis connection stream." << std::endl;
        return -1;
    }

    // Test basic ping operation
    rediscpp::value response = rediscpp::execute(*stream, "ping");
    std::cout << response.as<std::string>() << std::endl;

    // Test basic SET operation
    std::string const key = "test_key";
    rediscpp::execute(*stream, "set", key, "Hello, Redis!");

    // Test GET operation
    response = rediscpp::execute(*stream, "get", key);
    try {
        std::cout << "Value retrieved for key '" << key << "': " << response.as<std::string>() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to retrieve value for key '" << key << "': " << e.what() << std::endl;
    }

    // Test GET operation for a non existing key
    response = rediscpp::execute(*stream, "get", "missing_key");
    if (response.empty())
        std::cout << "Empty response." << std::endl;
    else if (response.is_bulk_string()){
        std::cout << "bulk string response." << std::endl;
        // There is no proper way to test the type or print the error message, only expect for error
        try {
            std::cout << "Unexpected value retrieved for 'missing_key': " << response.as_bulk_string() << std::endl;
        } catch (const std::exception& e) {
            // error: You can't cast Null to a type.
            std::cout << "Expected behavior: No value found for 'missing_key', error: " << e.what() << std::endl;
        }
    }
    
    // Test INCR operation
    rediscpp::execute(*stream, "set", "counter", "0");
    rediscpp::value counter = rediscpp::execute(*stream, "INCR", "counter");
    std::cout << "Counter after INCR: " << counter.as<int>() << std::endl;
    
    std::cout << "Successfully connected and performed basic operations with redis-cpp." << std::endl;

    return 0;
}
