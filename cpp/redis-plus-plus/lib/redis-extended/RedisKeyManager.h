#ifndef REDIS_EXTENDED_KEY_MANAGER_H
#define REDIS_EXTENDED_KEY_MANAGER_H

#include <string>
#include <sw/redis++/redis++.h>
#include "Logging.h"

namespace redis_extended {

class RedisKeyManager {
public:
    // Constructor
    RedisKeyManager(sw::redis::Redis& redis, const std::string& key, uint8_t threadId = 0, 
                    logging::ILogger* logger = nullptr);

    // Destructor
    ~RedisKeyManager();

    // Locking methods
    bool tryLock();
    void unlock();
    bool isLocked();

    // Data manipulation methods
    bool write(const std::string& value);
    bool update(const std::string& newValue);
    std::string read();

private:
    sw::redis::Redis& redis_; // Reference to Redis connection
    std::string key_;         // Key managed by this instance
    std::string lockKey_;     // Key used for locking
    uint8_t threadId_;        // Thread ID for logging purposes
    logging::ILogger* logger_; // Logger instance for tracking operations

    // Helper function to log messages using the provided logger
    void log(const std::string& level, const std::string& message) const;
};

} // namespace redis_extended

#endif // REDIS_EXTENDED_KEY_MANAGER_H
