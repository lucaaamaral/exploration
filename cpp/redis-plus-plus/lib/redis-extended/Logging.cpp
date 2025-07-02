#include "Logging.h"
#include <iostream>

namespace redis_extended {
namespace logging {

void DefaultLogger::log(const std::string& level, const std::string& message) {
    std::cout << "[" << level << "] " << message << std::endl;
}

} // namespace logging
} // namespace redis_extended
