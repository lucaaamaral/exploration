#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "poc_routines.h"
#include "Logging.h"

// Macro for delay configuration (in nanoseconds)
#define DELAY 10

// Read operation thread function
void poc_read(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    RedisLock lock(redis, shared_key, thread_id);
    LOG_MSG("READ", thread_id, "Connected to Redis");

    // Fixed delay for deterministic behavior
    const std::chrono::nanoseconds delay(DELAY);

    // Simulate multiple read attempts
    for (int attempt = 0; attempt < 5; ++attempt) {
        // Check if lock exists before reading
        while (lock.isLocked()) {
            LOG_MSG("READ", thread_id, "Waiting for lock to be released before reading...");
            std::this_thread::sleep_for(delay);
        }

        // Read the value
        try {
            std::optional<std::string> value = redis.get(shared_key);
            if (value) {
                LOG_MSG("READ", thread_id, "Read value: " + *value);
            } else {
                LOG_MSG("READ", thread_id, "Key not found: " + shared_key);
            }
        } catch (const std::exception& e) {
            LOG_MSG("ERROR", thread_id, "Exception while reading: " + std::string(e.what()));
        }

        // Wait before next read attempt
        std::this_thread::sleep_for(delay);
    }
}

// Write operation thread function
void poc_write(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    RedisLock lock(redis, shared_key, thread_id);
    LOG_MSG("WRITE", thread_id, "Connected to Redis");

    // Fixed delay for deterministic behavior
    const std::chrono::nanoseconds delay(DELAY);

    // Simulate multiple write attempts
    for (int attempt = 0; attempt < 3; ++attempt) {
        // Attempt to acquire lock
        while (!lock.tryLock()) {
            LOG_MSG("WRITE", thread_id, "Waiting to acquire lock for writing...");
            std::this_thread::sleep_for(delay);
        }

        // Write a new value
        try {
            std::string new_value = "Written by Thread " + std::to_string(static_cast<int>(thread_id)) + " (Attempt " + std::to_string(attempt + 1) + ")";
            redis.set(shared_key, new_value);
            LOG_MSG("WRITE", thread_id, "Wrote value: " + new_value);
        } catch (const std::exception& e) {
            LOG_MSG("ERROR", thread_id, "Exception while writing: " + std::string(e.what()));
        }

        // Release the lock
        lock.unlock();

        // Wait before next write attempt
        std::this_thread::sleep_for(delay);
    }
}

// Update operation thread function
void poc_update(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    RedisLock lock(redis, shared_key, thread_id);
    LOG_MSG("UPDATE", thread_id, "Connected to Redis");

    // Fixed delay for deterministic behavior
    const std::chrono::nanoseconds delay(DELAY);

    // Simulate multiple update attempts
    for (int attempt = 0; attempt < 3; ++attempt) {
        // Attempt to acquire lock
        while (!lock.tryLock()) {
            LOG_MSG("UPDATE", thread_id, "Waiting to acquire lock for updating...");
            std::this_thread::sleep_for(delay);
        }

        // Read current value, modify it, and write back
        try {
            std::optional<std::string> current_value = redis.get(shared_key);
            std::string new_value;
            if (current_value) {
                new_value = *current_value + " Updated by Thread " + std::to_string(static_cast<int>(thread_id)) + " (Attempt " + std::to_string(attempt + 1) + ")";
            } else {
                new_value = "Updated by Thread " + std::to_string(static_cast<int>(thread_id)) + " (Attempt " + std::to_string(attempt + 1) + ")";
            }
            redis.set(shared_key, new_value);
            LOG_MSG("UPDATE", thread_id, "Updated value to: " + new_value);
        } catch (const std::exception& e) {
            LOG_MSG("ERROR", thread_id, "Exception while updating: " + std::string(e.what()));
        }

        // Release the lock
        lock.unlock();

        // Wait before next update attempt
        std::this_thread::sleep_for(delay);
    }
}
