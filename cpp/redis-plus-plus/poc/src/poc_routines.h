#ifndef POC_ROUTINES_H
#define POC_ROUTINES_H

#include <string>
#include <cstdint>
#include "RedisLock.h"

// Function prototypes for thread operations
void poc_read(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key);
void poc_write(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key);
void poc_update(uint8_t thread_id, const std::string& connection_string, const std::string& shared_key);

#endif // POC_ROUTINES_H
