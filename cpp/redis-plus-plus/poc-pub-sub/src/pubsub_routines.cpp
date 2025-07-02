#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "pubsub_routines.h"
#include "RedisChannelManager.h"
#include "Logging.h"

// Macro for logging
#define LOG_MSG(level, thread_id, message) \
    std::cout << "[" << level << "] Thread " << static_cast<int>(thread_id) << ": " << message << std::endl

// Macro for delay configuration (in milliseconds)
#define DELAY 100

// Publisher thread function
void publisher_thread(uint8_t thread_id, const std::string& connection_string, const std::string& channel) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    redis_extended::RedisChannelManager channel_manager(redis, thread_id, new redis_extended::logging::DefaultLogger());
    LOG_MSG("INFO", thread_id, "PUBLISHER: Connected to Redis");

    // Fixed delay for deterministic behavior
    const std::chrono::milliseconds delay(DELAY);

    // Simulate publishing multiple JSON messages
    for (int attempt = 0; attempt < 10; ++attempt) {
        // Create a JSON message
        nlohmann::json message;
        message["thread_id"] = thread_id;
        message["attempt"] = attempt + 1;
        message["timestamp"] = std::chrono::system_clock::now().time_since_epoch().count();
        message["data"] = "Sample data payload from publisher thread " + std::to_string(thread_id);

        // Convert JSON to string
        std::string json_str = message.dump();

        // Publish the message
        if (channel_manager.publish(channel, json_str)) {
            LOG_MSG("INFO", thread_id, "PUBLISHER: Published JSON message to " + channel + ": " + json_str);
        } else {
            LOG_MSG("ERROR", thread_id, "PUBLISHER: Failed to publish JSON message to " + channel);
        }

        // Wait before next publish attempt
        std::this_thread::sleep_for(delay);
    }
}

// Subscriber thread function
void subscriber_thread(uint8_t thread_id, const std::string& connection_string, const std::string& channel) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    redis_extended::RedisChannelManager channel_manager(redis, thread_id, new redis_extended::logging::DefaultLogger());
    LOG_MSG("INFO", thread_id, "SUBSCRIBER: Connected to Redis");

    // Define callback for processing incoming messages
    auto callback = [thread_id](const std::string& msg) {
        try {
            // Parse JSON message
            auto json_msg = nlohmann::json::parse(msg);
            std::string log_msg = "SUBSCRIBER: Received JSON message: " + msg;
            LOG_MSG("INFO", thread_id, log_msg);
            std::cout << "Thread " << static_cast<int>(thread_id) << " received: " << msg << std::endl;
        } catch (const std::exception& e) {
            LOG_MSG("ERROR", thread_id, "SUBSCRIBER: Failed to parse JSON message: " + std::string(e.what()));
        }
    };

    // Subscribe to the channel
    channel_manager.subscribe(channel, callback);

    // Keep the thread alive for a while to receive messages
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop subscriptions
    channel_manager.stopSubscriptions();
    LOG_MSG("INFO", thread_id, "SUBSCRIBER: Stopped subscription to " + channel);
}
