#include <iostream>
#include <sw/redis++/redis++.h>
#include "RedisLock.h"
#include "Logging.h"

void createRedisKey(sw::redis::Redis& redis, const std::string& key, const std::string& value, uint8_t threadId) {
    RedisLock lock(redis, key, threadId);
    if (lock.tryLock()) {
        try {
            redis.set(key, value);
            LOG_MSG("REDIS_OP", threadId, "Created key " + key + " with value " + value);
            std::cout << "Created key: " << key << " with value: " << value << std::endl;
        } catch (const std::exception& e) {
            LOG_MSG("ERROR", threadId, "Failed to create key " + key + ": " + e.what());
            std::cerr << "Failed to create key: " << key << " - Error: " << e.what() << std::endl;
        }
        lock.unlock();
    } else {
        LOG_MSG("REDIS_OP", threadId, "Failed to acquire lock for creating key " + key);
        std::cerr << "Failed to acquire lock for creating key: " << key << std::endl;
    }
}

void updateRedisKey(sw::redis::Redis& redis, const std::string& key, const std::string& newValue, uint8_t threadId) {
    RedisLock lock(redis, key, threadId);
    if (lock.tryLock()) {
        try {
            if (redis.exists(key)) {
                redis.set(key, newValue);
                LOG_MSG("REDIS_OP", threadId, "Updated key " + key + " to new value " + newValue);
                std::cout << "Updated key: " << key << " to new value: " << newValue << std::endl;
            } else {
                LOG_MSG("REDIS_OP", threadId, "Key " + key + " does not exist for update");
                std::cerr << "Key: " << key << " does not exist for update" << std::endl;
            }
        } catch (const std::exception& e) {
            LOG_MSG("ERROR", threadId, "Failed to update key " + key + ": " + e.what());
            std::cerr << "Failed to update key: " << key << " - Error: " << e.what() << std::endl;
        }
        lock.unlock();
    } else {
        LOG_MSG("REDIS_OP", threadId, "Failed to acquire lock for updating key " + key);
        std::cerr << "Failed to acquire lock for updating key: " << key << std::endl;
    }
}

void deleteRedisKey(sw::redis::Redis& redis, const std::string& key, uint8_t threadId) {
    RedisLock lock(redis, key, threadId);
    if (lock.tryLock()) {
        try {
            if (redis.exists(key)) {
                redis.del(key);
                LOG_MSG("REDIS_OP", threadId, "Deleted key " + key);
                std::cout << "Deleted key: " << key << std::endl;
            } else {
                LOG_MSG("REDIS_OP", threadId, "Key " + key + " does not exist for deletion");
                std::cerr << "Key: " << key << " does not exist for deletion" << std::endl;
            }
        } catch (const std::exception& e) {
            LOG_MSG("ERROR", threadId, "Failed to delete key " + key + ": " + e.what());
            std::cerr << "Failed to delete key: " << key << " - Error: " << e.what() << std::endl;
        }
        lock.unlock();
    } else {
        LOG_MSG("REDIS_OP", threadId, "Failed to acquire lock for deleting key " + key);
        std::cerr << "Failed to acquire lock for deleting key: " << key << std::endl;
    }
}

void demonstrateRedisOperations(sw::redis::Redis& redis, uint8_t threadId) {
    std::string testKey = "test:key";
    std::string initialValue = "initial_value";
    std::string updatedValue = "updated_value";

    LOG_MSG("REDIS_DEMO", threadId, "Starting Redis operations demonstration");
    std::cout << "Starting Redis operations demonstration" << std::endl;

    createRedisKey(redis, testKey, initialValue, threadId);
    updateRedisKey(redis, testKey, updatedValue, threadId);
    deleteRedisKey(redis, testKey, threadId);

    LOG_MSG("REDIS_DEMO", threadId, "Completed Redis operations demonstration");
    std::cout << "Completed Redis operations demonstration" << std::endl;
}
