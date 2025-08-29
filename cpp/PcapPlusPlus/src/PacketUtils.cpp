#include "PacketUtils.h"

#include <iostream>
#include <memory>

// Note: PcapFileDevice.h is part of Pcap++ which depends on libpcap for file I/O operations.
// We are using it here to ease demonstration, but it wouldn't be used in a scenario where
// file I/O is not needed.
// This demonstrates packet parsing/creation functionality.
#include <PcapFileDevice.h>
#include <Packet.h>
#include <EthLayer.h>
#include <IPv4Layer.h>
#include <TcpLayer.h>
#include <PayloadLayer.h>
#include <MacAddress.h>
#include <IpAddress.h>
#include <SystemUtils.h>

#include "PacketAnalyzer.h"

namespace PacketUtils {

bool parseFromFile(const std::string& filename, int maxPackets) {
    pcpp::PcapFileReaderDevice reader(filename);
    
    if (!reader.open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }

    std::cout << "Successfully opened file: " << filename << std::endl;
    
    PacketAnalyzer analyzer;
    pcpp::RawPacket rawPacket;
    int packetCount = 0;

    while (reader.getNextPacket(rawPacket)) {
        pcpp::Packet packet(&rawPacket);
        analyzer.analyzePacket(packet);
        
        packetCount++;
        if (maxPackets > 0 && packetCount >= maxPackets) {
            std::cout << "\nReached maximum packet limit (" << maxPackets << ")" << std::endl;
            break;
        }
    }

    reader.close();
    analyzer.printStatistics();
    return true;
}

void createSamplePackets() {
    std::cout << "\n=== Creating Sample Packets ===" << std::endl;
    
    // Create a simple TCP packet
    pcpp::Packet tcpPacket(100);
    
    // Add Ethernet layer
    pcpp::EthLayer ethLayer(pcpp::MacAddress("aa:bb:cc:dd:ee:ff"), pcpp::MacAddress("11:22:33:44:55:66"));
    tcpPacket.addLayer(&ethLayer);
    
    // Add IPv4 layer
    pcpp::IPv4Layer ipv4Layer(pcpp::IPv4Address("192.168.1.100"), pcpp::IPv4Address("10.0.0.1"));
    ipv4Layer.getIPv4Header()->protocol = pcpp::PACKETPP_IPPROTO_TCP;
    tcpPacket.addLayer(&ipv4Layer);
    
    // Add TCP layer
    pcpp::TcpLayer tcpLayer(12345, 80);
    tcpLayer.getTcpHeader()->sequenceNumber = pcpp::hostToNet32(1000);
    tcpLayer.getTcpHeader()->ackNumber = pcpp::hostToNet32(2000);
    tcpLayer.getTcpHeader()->synFlag = 1;
    tcpPacket.addLayer(&tcpLayer);
    
    // Add payload
    std::string payload = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
    pcpp::PayloadLayer payloadLayer(reinterpret_cast<const uint8_t*>(payload.c_str()), payload.length());
    tcpPacket.addLayer(&payloadLayer);
    
    // Compute checksums
    tcpPacket.computeCalculateFields();
    
    std::cout << "Created sample TCP packet with " << tcpPacket.getRawPacket()->getRawDataLen() << " bytes" << std::endl;
    
    PacketAnalyzer analyzer;
    analyzer.analyzePacket(tcpPacket);
}

}