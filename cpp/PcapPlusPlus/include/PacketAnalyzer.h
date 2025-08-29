#pragma once

#include <string>
#include <cstdint>
#include <Packet.h>
#include <Layer.h>
#include <EthLayer.h>
#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>
#include <HttpLayer.h>
#include <DnsLayer.h>
#include <IcmpLayer.h>
#include <ArpLayer.h>

class PacketAnalyzer {
private:
    uint64_t packetCount = 0;
    uint64_t ethernetCount = 0;
    uint64_t ipv4Count = 0;
    uint64_t ipv6Count = 0;
    uint64_t tcpCount = 0;
    uint64_t udpCount = 0;
    uint64_t httpCount = 0;
    uint64_t dnsCount = 0;
    uint64_t icmpCount = 0;
    uint64_t arpCount = 0;

    void analyzeLayer(pcpp::Layer* layer);
    void analyzeEthernetLayer(pcpp::EthLayer* ethLayer);
    void analyzeIPv4Layer(pcpp::IPv4Layer* ipLayer);
    void analyzeIPv6Layer(pcpp::IPv6Layer* ipLayer);
    void analyzeTcpLayer(pcpp::TcpLayer* tcpLayer);
    void analyzeUdpLayer(pcpp::UdpLayer* udpLayer);
    void analyzeHttpLayer(pcpp::Layer* httpLayer);
    void analyzeDnsLayer(pcpp::DnsLayer* dnsLayer);
    void analyzeIcmpLayer(pcpp::IcmpLayer* icmpLayer);
    void analyzeArpLayer(pcpp::ArpLayer* arpLayer);
    
    std::string getProtocolName(pcpp::ProtocolType protocol);
    std::string getTcpFlags(pcpp::TcpLayer* tcpLayer);
    std::string getHttpMethod(pcpp::HttpRequestLayer::HttpMethod method);

public:
    void analyzePacket(const pcpp::Packet& packet);
    void printStatistics();
};