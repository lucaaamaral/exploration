#ifndef REDIS_LOCK_H
#define REDIS_LOCK_H

#include <string>
#include <cstdint>
#include <sw/redis++/redis++.h>
#include "Logging.h"

class RedisLock {
public:
    // Constructor
    RedisLock(sw::redis::Redis& redis, const std::string& sharedKey, uint8_t threadId);

    // Attempt to acquire the lock using SETNX
    bool tryLock();

    // Release the lock by deleting the key
    void unlock();

    // Check if the lock exists without acquiring it
    bool isLocked();

private:
    sw::redis::Redis& redis_; // Reference to Redis connection
    std::string lockKey_;     // Key used for locking
    uint8_t threadId_;        // Thread ID for logging purposes

    // Helper function to log messages using the LOG_MSG macro
    void logMessage(const std::string& operation, const std::string& message) const;
};

#endif // REDIS_LOCK_H
