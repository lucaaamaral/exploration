#include "RedisLock.h"
#include <iostream>

RedisLock::RedisLock(sw::redis::Redis& redis, const std::string& sharedKey, uint8_t threadId)
    : redis_(redis), lockKey_("lock:" + sharedKey), threadId_(threadId) {
}

bool RedisLock::tryLock() {
    try {
        // SETNX returns 1 if the key was set (lock acquired), 0 if it already exists (lock held by another)
        long long result = redis_.setnx(lockKey_, "1");
        if (result == 1) {
            logMessage("LOCK", "Lock acquired for key " + lockKey_);
            return true;
        } else {
            logMessage("LOCK", "Lock acquisition failed for key " + lockKey_);
            return false;
        }
    } catch (const std::exception& e) {
        logMessage("ERROR", "Exception while acquiring lock: " + std::string(e.what()));
        return false;
    }
}

void RedisLock::unlock() {
    try {
        redis_.del(lockKey_);
        logMessage("LOCK", "Lock released for key " + lockKey_);
    } catch (const std::exception& e) {
        logMessage("ERROR", "Exception while releasing lock: " + std::string(e.what()));
    }
}

bool RedisLock::isLocked() {
    try {
        bool exists = redis_.exists(lockKey_);
        if (exists) {
            logMessage("LOCK_CHECK", "Lock exists for key " + lockKey_);
        }
        return exists;
    } catch (const std::exception& e) {
        logMessage("ERROR", "Exception while checking lock: " + std::string(e.what()));
        return false;
    }
}

void RedisLock::logMessage(const std::string& operation, const std::string& message) const {
    LOG_MSG(operation, threadId_, message);
}
