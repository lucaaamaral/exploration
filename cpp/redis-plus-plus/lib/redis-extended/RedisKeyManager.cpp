#include "RedisKeyManager.h"

namespace redis_extended {

RedisKeyManager::RedisKeyManager(sw::redis::Redis& redis, const std::string& key, uint8_t threadId, 
                                 logging::ILogger* logger)
    : redis_(redis), key_(key), lockKey_("lock:" + key), threadId_(threadId), logger_(logger) {
    log("INFO", "Initialized RedisKeyManager for key: " + key_);
}

RedisKeyManager::~RedisKeyManager() {
    // Ensure the lock is released if held
    if (isLocked()) {
        unlock();
    }
    log("INFO", "Destroyed RedisKeyManager for key: " + key_);
}

bool RedisKeyManager::tryLock() {
    try {
        // Attempt to acquire the lock using SETNX
        bool acquired = redis_.setnx(lockKey_, "locked");
        if (acquired) {
            log("INFO", "Lock acquired for key: " + key_);
        } else {
            log("WARNING", "Failed to acquire lock for key: " + key_);
        }
        return acquired;
    } catch (const std::exception& e) {
        log("ERROR", "Error trying to lock key " + key_ + ": " + e.what());
        return false;
    }
}

void RedisKeyManager::unlock() {
    try {
        redis_.del(lockKey_);
        log("INFO", "Lock released for key: " + key_);
    } catch (const std::exception& e) {
        log("ERROR", "Error releasing lock for key " + key_ + ": " + e.what());
    }
}

bool RedisKeyManager::isLocked() {
    try {
        return redis_.exists(lockKey_);
    } catch (const std::exception& e) {
        log("ERROR", "Error checking lock status for key " + key_ + ": " + e.what());
        return false;
    }
}

bool RedisKeyManager::write(const std::string& value) {
    if (!tryLock()) {
        log("WARNING", "Cannot write to key " + key_ + ": Lock not acquired");
        return false;
    }
    try {
        redis_.set(key_, value);
        log("INFO", "Successfully wrote value to key: " + key_);
        unlock();
        return true;
    } catch (const std::exception& e) {
        log("ERROR", "Error writing to key " + key_ + ": " + e.what());
        unlock();
        return false;
    }
}

bool RedisKeyManager::update(const std::string& newValue) {
    if (!tryLock()) {
        log("WARNING", "Cannot update key " + key_ + ": Lock not acquired");
        return false;
    }
    try {
        if (redis_.exists(key_)) {
            redis_.set(key_, newValue);
            log("INFO", "Successfully updated value for key: " + key_);
            unlock();
            return true;
        } else {
            log("WARNING", "Key " + key_ + " does not exist for update");
            unlock();
            return false;
        }
    } catch (const std::exception& e) {
        log("ERROR", "Error updating key " + key_ + ": " + e.what());
        unlock();
        return false;
    }
}

std::string RedisKeyManager::read() {
    try {
        auto value = redis_.get(key_);
        if (value) {
            log("INFO", "Successfully read value from key: " + key_);
            return *value;
        } else {
            log("WARNING", "Key " + key_ + " does not exist for read");
            return "";
        }
    } catch (const std::exception& e) {
        log("ERROR", "Error reading from key " + key_ + ": " + e.what());
        return "";
    }
}

void RedisKeyManager::log(const std::string& level, const std::string& message) const {
    if (logger_) {
        logger_->log(level, message);
    }
}

} // namespace redis_extended
