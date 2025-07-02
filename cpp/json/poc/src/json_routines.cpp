#include <iostream>
#include <fstream>
#include "json_routines.h"

void handleJsonConfiguration() {
    // Simulate reading a configuration file
    std::ifstream config_file("config.json");
    if (!config_file.is_open()) {
        std::cerr << "Could not open config.json. Using default configuration." << std::endl;
        
        // Default configuration
        json config;
        config["app_name"] = "JSON POC App";
        config["version"] = "1.0.0";
        config["settings"]["debug"] = true;
        config["settings"]["log_level"] = "info";
        config["users"][0]["id"] = 1;
        config["users"][0]["name"] = "Alice";
        config["users"][1]["id"] = 2;
        config["users"][1]["name"] = "Bob";

        // Save the default configuration to a file
        std::ofstream out_file("config.json");
        if (out_file.is_open()) {
            out_file << config.dump(4);
            out_file.close();
            std::cout << "Default configuration saved to config.json" << std::endl;
        }
    } else {
        // Parse the configuration file
        json config;
        config_file >> config;
        config_file.close();
        std::cout << "Configuration loaded from config.json" << std::endl;

        // Display configuration
        std::cout << "App Name: " << config["app_name"].get<std::string>() << std::endl;
        std::cout << "Version: " << config["version"].get<std::string>() << std::endl;
        std::cout << "Debug: " << (config["settings"]["debug"].get<bool>() ? "true" : "false") << std::endl;
        std::cout << "Log Level: " << config["settings"]["log_level"].get<std::string>() << std::endl;
        
        std::cout << "Users:" << std::endl;
        for (const auto& user : config["users"]) {
            std::cout << "  - ID: " << user["id"].get<int>() << ", Name: " << user["name"].get<std::string>() << std::endl;
        }
    }

    // Demonstrate updating configuration
    json updated_config;
    updated_config["app_name"] = "Updated JSON POC App";
    updated_config["version"] = "1.1.0";
    updated_config["settings"]["debug"] = false;
    updated_config["settings"]["log_level"] = "error";
    updated_config["users"][0]["id"] = 1;
    updated_config["users"][0]["name"] = "Alice Updated";
    updated_config["users"][1]["id"] = 2;
    updated_config["users"][1]["name"] = "Bob Updated";

    // Save updated configuration
    std::ofstream updated_file("updated_config.json");
    if (updated_file.is_open()) {
        updated_file << updated_config.dump(4);
        updated_file.close();
        std::cout << "Updated configuration saved to updated_config.json" << std::endl;
    } else {
        std::cerr << "Could not save updated configuration." << std::endl;
    }
}
