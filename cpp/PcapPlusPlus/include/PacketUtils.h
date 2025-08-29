#pragma once

#include <string>

namespace PacketUtils {
    bool parseFromFile(const std::string& filename, int maxPackets = -1);
    void createSamplePackets();
}