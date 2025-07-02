#include <sw/redis++/redis++.h>
#include <iostream>
#include <cstdlib>

int main() {
    // Get Redis connection parameters from environment variables or use defaults
    const char* redis_host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    const char* redis_port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    
    // Construct the connection string
    std::string connection_string = "tcp://" + std::string(redis_host) + ":" + std::string(redis_port);
    
    // Create a Redis connection
    auto redis = sw::redis::Redis(connection_string);

    // Test basic SET and GET operations
    redis.set("test_key", "Hello, Redis!");
    auto val = redis.get("test_key");
    if (val) {
        std::cout << "Value retrieved: " << *val << std::endl;
    } else {
        std::cout << "Key not found." << std::endl;
    }
    val = redis.get("missing_key");
    if (val) {
        std::cout << "Value retrieved: " << *val << std::endl;
    } else {
        std::cout << "Key not found." << std::endl;
    }
    

    // Test INCR operation
    redis.set("counter", "0");
    long long counter = redis.incr("counter");
    std::cout << "Counter after INCR: " << counter << std::endl;

    std::cout << "Successfully connected and performed basic operations with redis-plus-plus." << std::endl;

    return 0;
}
