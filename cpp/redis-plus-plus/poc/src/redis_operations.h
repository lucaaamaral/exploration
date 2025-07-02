#ifndef REDIS_OPERATIONS_H
#define REDIS_OPERATIONS_H

#include <string>
#include <sw/redis++/redis++.h>

void createRedisKey(sw::redis::Redis& redis, const std::string& key, const std::string& value, uint8_t threadId);
void updateRedisKey(sw::redis::Redis& redis, const std::string& key, const std::string& newValue, uint8_t threadId);
void deleteRedisKey(sw::redis::Redis& redis, const std::string& key, uint8_t threadId);
void demonstrateRedisOperations(sw::redis::Redis& redis, uint8_t threadId);

#endif // REDIS_OPERATIONS_H
