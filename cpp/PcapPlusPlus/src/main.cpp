#include <iostream>
#include <string>
#include "PacketUtils.h"

int main(int argc, char* argv[]) {
    std::cout << "PcapPlusPlus Packet Parser POC" << std::endl;
    std::cout << "=============================" << std::endl;
    
    if (argc > 1) {
        // Parse packets from file
        std::string filename = argv[1];
        int maxPackets = (argc > 2) ? std::stoi(argv[2]) : -1;
        
        std::cout << "Parsing packets from file: " << filename << std::endl;
        if (maxPackets > 0) {
            std::cout << "Maximum packets to process: " << maxPackets << std::endl;
        }
        
        PacketUtils::parseFromFile(filename, maxPackets);
    } else {
        // Create and analyze sample packets
        PacketUtils::createSamplePackets();
        
        std::cout << "\nUsage: " << argv[0] << " <pcap_file> [max_packets]" << std::endl;
        std::cout << "  pcap_file   - Path to a PCAP or PCAP-NG file to analyze" << std::endl;
        std::cout << "  max_packets - Optional limit on number of packets to process" << std::endl;
    }
    
    return 0;
}