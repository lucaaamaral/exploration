#include "RedisChannelManager.h"

namespace redis_extended {

RedisChannelManager::RedisChannelManager(sw::redis::Redis& redis, uint8_t threadId, 
                                         logging::ILogger* logger)
    : redis_(redis), threadId_(threadId), logger_(logger), running_(false) {
    this->log("INFO", "Initialized RedisChannelManager for thread ID: " + std::to_string(static_cast<int>(this->threadId_)));
}

RedisChannelManager::~RedisChannelManager() {
    this->stopSubscriptions();
    this->log("INFO", "Destroyed RedisChannelManager for thread ID: " + std::to_string(static_cast<int>(this->threadId_)));
}

bool RedisChannelManager::publish(const std::string& channel, const std::string& jsonMessage) {
    try {
        this->redis_.publish(channel, jsonMessage);
        this->log("INFO", "Published message to channel: " + channel);
        return true;
    } catch (const std::exception& e) {
        this->log("ERROR", "Error publishing to channel " + channel + ": " + e.what());
        return false;
    }
}

void RedisChannelManager::subscribe(const std::string& channel, 
                                    std::function<void(const std::string&)> callback) {
    if (!this->running_) {
        this->running_ = true;
        this->subscriptionThread_ = std::make_unique<std::thread>([this, channel, callback]() {
            try {
                sw::redis::Subscriber subscriber = this->redis_.subscriber();
                subscriber.on_message([&callback](std::string chan, std::string msg) {
                    callback(msg);
                });
                subscriber.subscribe(channel);
                this->log("INFO", "Subscribed to channel: " + channel);
                
                while (this->running_) {
                    try {
                        subscriber.consume();
                    } catch (const sw::redis::TimeoutError& te) {
                        this->log("INFO", "Timeout waiting for messages on channel " + channel + ": " + te.what());
                        continue; // Continue the loop to keep subscription active
                    }
                }
            } catch (const std::exception& e) {
                this->log("ERROR", "Error in subscription to channel " + channel + ": " + e.what());
            }
        });
    } else {
        this->log("WARNING", "Subscription already running for thread ID: " + std::to_string(static_cast<int>(this->threadId_)));
    }
}

void RedisChannelManager::unsubscribe(const std::string& channel) {
    try {
        sw::redis::Subscriber subscriber = this->redis_.subscriber();
        subscriber.unsubscribe(channel);
        this->log("INFO", "Unsubscribed from channel: " + channel);
    } catch (const std::exception& e) {
        this->log("ERROR", "Error unsubscribing from channel " + channel + ": " + e.what());
    }
}

void RedisChannelManager::stopSubscriptions() {
    this->running_ = false;
    if (this->subscriptionThread_ && this->subscriptionThread_->joinable()) {
        this->subscriptionThread_->join();
        this->subscriptionThread_.reset();
        this->log("INFO", "Stopped all subscription activities for thread ID: " + std::to_string(static_cast<int>(this->threadId_)));
    }
}

void RedisChannelManager::log(const std::string& level, const std::string& message) const {
    if (this->logger_) {
        this->logger_->log(level, message);
    }
}

} // namespace redis_extended
