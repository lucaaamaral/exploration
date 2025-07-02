#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>

// Macro for formatted logging
#define LOG_MSG(operation, thread_id, message) \
    std::cout << "[" << operation << "-" << static_cast<int>(thread_id) << "] " << message << std::endl

#endif // LOGGING_H
