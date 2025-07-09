#ifndef REDIS_EXTENDED_CHANNEL_MANAGER_H
#define REDIS_EXTENDED_CHANNEL_MANAGER_H

#include <string>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <sw/redis++/redis++.h>
#include "Logging.h"

namespace redis_extended {

class RedisChannelManager {
public:
    // Constructor
    RedisChannelManager(sw::redis::Redis& redis, uint8_t threadId = 0, 
                        logging::ILogger* logger = nullptr);

    // Destructor
    ~RedisChannelManager();

    // Publish a JSON message to a specified channel
    bool publish(const std::string& channel, const std::string& jsonMessage);

    // Subscribe to a channel and process incoming messages with a callback
    // The callback should handle JSON messages
    void subscribe(const std::string& channel, 
                   std::function<void(const std::string&)> callback);

    // Unsubscribe from a channel
    void unsubscribe(const std::string& channel);

    // Stop all subscription activities
    void stopSubscriptions();

private:
    sw::redis::Redis& redis_; // Reference to Redis connection
    uint8_t threadId_;        // Thread ID for logging purposes
    logging::ILogger* logger_; // Logger instance for tracking operations
    std::atomic<bool> running_; // Flag to control subscription loops
    std::unique_ptr<std::thread> subscriptionThread_; // Thread for handling subscriptions

    // Helper function to log messages using the provided logger
    void log(const std::string& level, const std::string& message) const;
};

} // namespace redis_extended

#endif // REDIS_EXTENDED_CHANNEL_MANAGER_H
