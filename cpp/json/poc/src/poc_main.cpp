#include <iostream>
#include <sw/redis++/redis++.h>
#include "json_routines.h"
#include <RedisKeyManager.h>
#include <Logging.h>

int main() {
    const char* redis_host = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    const char* redis_port = std::getenv("REDIS_PORT") ? std::getenv("REDIS_PORT") : "6379";
    sw::redis::ConnectionOptions connection_options;
    connection_options.host = redis_host;
    connection_options.port = std::stoi(redis_port);
    try {
        sw::redis::Redis redis(connection_options);
        
        // Demonstrate RedisKeyManager usage with JSON formatted strings
        redis_extended::logging::DefaultLogger logger;
        redis_extended::RedisKeyManager keyManager(redis, "demo_json_key", 0, &logger);
        std::cout << "Demonstrating RedisKeyManager with JSON data:" << std::endl;
        
        // Create and serialize initial JSON data
        nlohmann::json initialJson;
        initialJson["app_name"] = "JSON POC App";
        initialJson["version"] = "1.0.0";
        initialJson["settings"]["debug"] = true;
        initialJson["settings"]["log_level"] = "info";
        initialJson["users"][0]["id"] = 1;
        initialJson["users"][0]["name"] = "Alice";
        initialJson["users"][1]["id"] = 2;
        initialJson["users"][1]["name"] = "Bob";
        std::string initialJsonStr = initialJson.dump();
        if (keyManager.write(initialJsonStr)) {
            std::cout << "Wrote initial JSON value to key." << std::endl;
        } else {
            std::cout << "Failed to write initial JSON value to key." << std::endl;
        }
        
        // Read and deserialize JSON data
        std::string value = keyManager.read();
        try {
            nlohmann::json readJson = nlohmann::json::parse(value);
            std::cout << "Read JSON value: " << readJson.dump(2) << std::endl;
        } catch (const nlohmann::json::exception& e) {
            std::cout << "Failed to parse JSON from read value: " << value << std::endl;
        }
        
        // Update JSON data
        nlohmann::json updatedJson = initialJson;
        updatedJson["settings"]["theme"] = "dark";
        std::string updatedJsonStr = updatedJson.dump();
        if (keyManager.update(updatedJsonStr)) {
            std::cout << "Updated JSON value for key." << std::endl;
        } else {
            std::cout << "Failed to update JSON value for key." << std::endl;
        }
        
        // Read updated JSON data
        value = keyManager.read();
        try {
            nlohmann::json updatedReadJson = nlohmann::json::parse(value);
            std::cout << "Read updated JSON value: " << updatedReadJson.dump(2) << std::endl;
        } catch (const nlohmann::json::exception& e) {
            std::cout << "Failed to parse JSON from updated read value: " << value << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to run Redis operations demo with JSON: " << e.what() << std::endl;
    }

    std::cout << "JSON POC Application Completed" << std::endl;
    return 0;
}
