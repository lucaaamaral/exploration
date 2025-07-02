#ifndef REDIS_EXTENDED_LOGGING_H
#define REDIS_EXTENDED_LOGGING_H

#include <string>

namespace redis_extended {
namespace logging {

// Abstract logging interface for modular logging system
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& level, const std::string& message) = 0;
};

// Default logger implementation for basic console output
class DefaultLogger : public ILogger {
public:
    void log(const std::string& level, const std::string& message) override;
};

} // namespace logging
} // namespace redis_extended

#endif // REDIS_EXTENDED_LOGGING_H
