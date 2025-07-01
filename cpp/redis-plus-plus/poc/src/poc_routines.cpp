#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "poc_routines.h"

// Macro for delay configuration (in nanoseconds)
#define DELAY 10

// Helper function to log messages with thread ID in the format [OPERATION-ID] MESSAGE
void log_message(const std::string& operation, uint8_t thread_id, const std::string& message) {
    std::cout << "[" << operation << "-" << static_cast<int>(thread_id) << "] " << message << std::endl;
}

// Function to attempt acquiring a lock using SETNX
bool acquire_lock(sw::redis::Redis& redis, const std::string& lock_key, uint8_t thread_id) {
    try {
        // SETNX returns 1 if the key was set (lock acquired), 0 if it already exists (lock held by another)
        long long result = redis.setnx(lock_key, "1");
        if (result == 1) {
            log_message("LOCK", thread_id, "Lock acquired for key " + lock_key);
            return true;
        } else {
            log_message("LOCK", thread_id, "Lock acquisition failed for key " + lock_key);
            return false;
        }
    } catch (const std::exception& e) {
        log_message("ERROR", thread_id, "Exception while acquiring lock: " + std::string(e.what()));
        return false;
    }
}

// Function to release a lock by deleting the key
void release_lock(sw::redis::Redis& redis, const std::string& lock_key, uint8_t thread_id) {
    try {
        redis.del(lock_key);
        log_message("LOCK", thread_id, "Lock released for key " + lock_key);
    } catch (const std::exception& e) {
        log_message("ERROR", thread_id, "Exception while releasing lock: " + std::string(e.what()));
    }
}

// Function to check if a lock exists without acquiring it
bool lock_exists(sw::redis::Redis& redis, const std::string& lock_key, uint8_t thread_id) {
    try {
        bool exists = redis.exists(lock_key);
        if (exists) {
            log_message("LOCK_CHECK", thread_id, "Lock exists for key " + lock_key);
        }
        return exists;
    } catch (const std::exception& e) {
        log_message("ERROR", thread_id, "Exception while checking lock: " + std::string(e.what()));
        return false;
    }
}

// Read operation thread function
void poc_read(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key, const std::string& lock_key) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    log_message("READ", thread_id, "Connected to Redis");

    // Fixed delay for deterministic behavior
    const std::chrono::nanoseconds delay(DELAY);

    // Simulate multiple read attempts
    for (int attempt = 0; attempt < 5; ++attempt) {
        // Check if lock exists before reading
        while (lock_exists(redis, lock_key, thread_id)) {
            log_message("READ", thread_id, "Waiting for lock to be released before reading...");
            std::this_thread::sleep_for(delay);
        }

        // Read the value
        try {
            std::optional<std::string> value = redis.get(shared_key);
            if (value) {
                log_message("READ", thread_id, "Read value: " + *value);
            } else {
                log_message("READ", thread_id, "Key not found: " + shared_key);
            }
        } catch (const std::exception& e) {
            log_message("ERROR", thread_id, "Exception while reading: " + std::string(e.what()));
        }

        // Wait before next read attempt
        std::this_thread::sleep_for(delay);
    }
}

// Write operation thread function
void poc_write(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key, const std::string& lock_key) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    log_message("WRITE", thread_id, "Connected to Redis");

    // Fixed delay for deterministic behavior
    const std::chrono::nanoseconds delay(DELAY);

    // Simulate multiple write attempts
    for (int attempt = 0; attempt < 3; ++attempt) {
        // Attempt to acquire lock
        while (!acquire_lock(redis, lock_key, thread_id)) {
            log_message("WRITE", thread_id, "Waiting to acquire lock for writing...");
            std::this_thread::sleep_for(delay);
        }

        // Write a new value
        try {
            std::string new_value = "Written by Thread " + std::to_string(static_cast<int>(thread_id)) + " (Attempt " + std::to_string(attempt + 1) + ")";
            redis.set(shared_key, new_value);
            log_message("WRITE", thread_id, "Wrote value: " + new_value);
        } catch (const std::exception& e) {
            log_message("ERROR", thread_id, "Exception while writing: " + std::string(e.what()));
        }

        // Release the lock
        release_lock(redis, lock_key, thread_id);

        // Wait before next write attempt
        std::this_thread::sleep_for(delay);
    }
}

// Update operation thread function
void poc_update(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key, const std::string& lock_key) {
    // Create a unique Redis connection for this thread
    sw::redis::Redis redis(connection_string);
    log_message("UPDATE", thread_id, "Connected to Redis");

    // Fixed delay for deterministic behavior
    const std::chrono::nanoseconds delay(DELAY);

    // Simulate multiple update attempts
    for (int attempt = 0; attempt < 3; ++attempt) {
        // Attempt to acquire lock
        while (!acquire_lock(redis, lock_key, thread_id)) {
            log_message("UPDATE", thread_id, "Waiting to acquire lock for updating...");
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
            log_message("UPDATE", thread_id, "Updated value to: " + new_value);
        } catch (const std::exception& e) {
            log_message("ERROR", thread_id, "Exception while updating: " + std::string(e.what()));
        }

        // Release the lock
        release_lock(redis, lock_key, thread_id);

        // Wait before next update attempt
        std::this_thread::sleep_for(delay);
    }
}
