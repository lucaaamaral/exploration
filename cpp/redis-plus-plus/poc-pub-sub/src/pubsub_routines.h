#ifndef PUBSUB_ROUTINES_H
#define PUBSUB_ROUTINES_H

#include <string>
#include <cstdint>

// Function prototypes for thread operations
void publisher_thread(uint8_t thread_id, const std::string& connection_string, const std::string& channel);
void subscriber_thread(uint8_t thread_id, const std::string& connection_string, const std::string& channel);

#endif // PUBSUB_ROUTINES_H
