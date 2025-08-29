#include "PacketAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <SystemUtils.h>

void PacketAnalyzer::analyzePacket(const pcpp::Packet& packet) {
    packetCount++;
    
    std::cout << "\n=== Packet #" << packetCount << " ===" << std::endl;
    std::cout << "Total packet size: " << packet.getRawPacket()->getRawDataLen() << " bytes" << std::endl;
    
    // Analyze each layer
    for (auto* layer = packet.getFirstLayer(); layer != nullptr; layer = layer->getNextLayer()) {
        analyzeLayer(layer);
    }
}

void PacketAnalyzer::analyzeLayer(pcpp::Layer* layer) {
    std::cout << "Layer: " << getProtocolName(layer->getProtocol()) 
              << " (Header: " << layer->getHeaderLen() 
              << " bytes, Payload: " << layer->getLayerPayloadSize() << " bytes)" << std::endl;

    switch (layer->getProtocol()) {
        case pcpp::Ethernet:
            analyzeEthernetLayer(static_cast<pcpp::EthLayer*>(layer));
            break;
        case pcpp::IPv4:
            analyzeIPv4Layer(static_cast<pcpp::IPv4Layer*>(layer));
            break;
        case pcpp::IPv6:
            analyzeIPv6Layer(static_cast<pcpp::IPv6Layer*>(layer));
            break;
        case pcpp::TCP:
            analyzeTcpLayer(static_cast<pcpp::TcpLayer*>(layer));
            break;
        case pcpp::UDP:
            analyzeUdpLayer(static_cast<pcpp::UdpLayer*>(layer));
            break;
        case pcpp::HTTPRequest:
        case pcpp::HTTPResponse:
            analyzeHttpLayer(layer);
            break;
        case pcpp::DNS:
            analyzeDnsLayer(static_cast<pcpp::DnsLayer*>(layer));
            break;
        case pcpp::ICMP:
            analyzeIcmpLayer(static_cast<pcpp::IcmpLayer*>(layer));
            break;
        case pcpp::ARP:
            analyzeArpLayer(static_cast<pcpp::ArpLayer*>(layer));
            break;
        default:
            break;
    }
}

void PacketAnalyzer::analyzeEthernetLayer(pcpp::EthLayer* ethLayer) {
    ethernetCount++;
    std::cout << "  Ethernet - Src MAC: " << ethLayer->getSourceMac()
              << ", Dst MAC: " << ethLayer->getDestMac()
              << ", EtherType: 0x" << std::hex << pcpp::netToHost16(ethLayer->getEthHeader()->etherType) << std::dec << std::endl;
}

void PacketAnalyzer::analyzeIPv4Layer(pcpp::IPv4Layer* ipLayer) {
    ipv4Count++;
    auto* ipHeader = ipLayer->getIPv4Header();
    std::cout << "  IPv4 - Src: " << ipLayer->getSrcIPAddress()
              << ", Dst: " << ipLayer->getDstIPAddress()
              << ", Protocol: " << (int)ipHeader->protocol
              << ", TTL: " << (int)ipHeader->timeToLive
              << ", ID: 0x" << std::hex << pcpp::netToHost16(ipHeader->ipId) << std::dec << std::endl;
}

void PacketAnalyzer::analyzeIPv6Layer(pcpp::IPv6Layer* ipLayer) {
    ipv6Count++;
    std::cout << "  IPv6 - Src: " << ipLayer->getSrcIPAddress()
              << ", Dst: " << ipLayer->getDstIPAddress()
              << ", Next Header: " << (int)ipLayer->getIPv6Header()->nextHeader
              << ", Hop Limit: " << (int)ipLayer->getIPv6Header()->hopLimit << std::endl;
}

void PacketAnalyzer::analyzeTcpLayer(pcpp::TcpLayer* tcpLayer) {
    tcpCount++;
    auto* tcpHeader = tcpLayer->getTcpHeader();
    std::cout << "  TCP - Src Port: " << tcpLayer->getSrcPort()
              << ", Dst Port: " << tcpLayer->getDstPort()
              << ", Seq: " << pcpp::netToHost32(tcpHeader->sequenceNumber)
              << ", Ack: " << pcpp::netToHost32(tcpHeader->ackNumber)
              << ", Flags: " << getTcpFlags(tcpLayer) << std::endl;
}

void PacketAnalyzer::analyzeUdpLayer(pcpp::UdpLayer* udpLayer) {
    udpCount++;
    std::cout << "  UDP - Src Port: " << udpLayer->getSrcPort()
              << ", Dst Port: " << udpLayer->getDstPort()
              << ", Length: " << pcpp::netToHost16(udpLayer->getUdpHeader()->length) << std::endl;
}

void PacketAnalyzer::analyzeHttpLayer(pcpp::Layer* httpLayer) {
    httpCount++;
    if (httpLayer->getProtocol() == pcpp::HTTPRequest) {
        auto* reqLayer = static_cast<pcpp::HttpRequestLayer*>(httpLayer);
        std::cout << "  HTTP Request - Method: " << getHttpMethod(reqLayer->getFirstLine()->getMethod())
                  << ", URI: " << reqLayer->getFirstLine()->getUri()
                  << ", Version: " << reqLayer->getFirstLine()->getVersion() << std::endl;
        
        // Print some key headers
        if (auto* hostField = reqLayer->getFieldByName("Host"))
            std::cout << "    Host: " << hostField->getFieldValue() << std::endl;
        if (auto* userAgent = reqLayer->getFieldByName("User-Agent"))
            std::cout << "    User-Agent: " << userAgent->getFieldValue() << std::endl;
    } else {
        auto* respLayer = static_cast<pcpp::HttpResponseLayer*>(httpLayer);
        std::cout << "  HTTP Response - Status: " << respLayer->getFirstLine()->getStatusCode()
                  << ", Version: " << respLayer->getFirstLine()->getVersion() << std::endl;
    }
}

void PacketAnalyzer::analyzeDnsLayer(pcpp::DnsLayer* dnsLayer) {
    dnsCount++;
    auto* dnsHeader = dnsLayer->getDnsHeader();
    std::cout << "  DNS - ID: 0x" << std::hex << pcpp::netToHost16(dnsHeader->transactionID) << std::dec
              << ", Questions: " << pcpp::netToHost16(dnsHeader->numberOfQuestions)
              << ", Answers: " << pcpp::netToHost16(dnsHeader->numberOfAnswers)
              << ", Type: " << (dnsHeader->queryOrResponse ? "Response" : "Query") << std::endl;
}

void PacketAnalyzer::analyzeIcmpLayer(pcpp::IcmpLayer* icmpLayer) {
    icmpCount++;
    auto* icmpHeader = icmpLayer->getIcmpHeader();
    std::cout << "  ICMP - Type: " << (int)icmpHeader->type
              << ", Code: " << (int)icmpHeader->code << std::endl;
}

void PacketAnalyzer::analyzeArpLayer(pcpp::ArpLayer* arpLayer) {
    arpCount++;
    std::cout << "  ARP - Operation: " << (pcpp::netToHost16(arpLayer->getArpHeader()->opcode) == 1 ? "Request" : "Reply")
              << ", Sender MAC: " << arpLayer->getSenderMacAddress()
              << ", Target MAC: " << arpLayer->getTargetMacAddress() << std::endl;
}

std::string PacketAnalyzer::getProtocolName(pcpp::ProtocolType protocol) {
    switch (protocol) {
        case pcpp::Ethernet: return "Ethernet";
        case pcpp::IPv4: return "IPv4";
        case pcpp::IPv6: return "IPv6";
        case pcpp::TCP: return "TCP";
        case pcpp::UDP: return "UDP";
        case pcpp::HTTPRequest: return "HTTP Request";
        case pcpp::HTTPResponse: return "HTTP Response";
        case pcpp::DNS: return "DNS";
        case pcpp::ICMP: return "ICMP";
        case pcpp::ARP: return "ARP";
        case pcpp::PacketTrailer: return "Packet Trailer";
        case pcpp::GenericPayload: return "Generic Payload";
        default: return "Unknown (" + std::to_string(protocol) + ")";
    }
}

std::string PacketAnalyzer::getTcpFlags(pcpp::TcpLayer* tcpLayer) {
    std::string flags;
    auto* tcpHeader = tcpLayer->getTcpHeader();
    if (tcpHeader->synFlag) flags += "SYN ";
    if (tcpHeader->ackFlag) flags += "ACK ";
    if (tcpHeader->finFlag) flags += "FIN ";
    if (tcpHeader->rstFlag) flags += "RST ";
    if (tcpHeader->pshFlag) flags += "PSH ";
    if (tcpHeader->urgFlag) flags += "URG ";
    if (tcpHeader->eceFlag) flags += "ECE ";
    if (tcpHeader->cwrFlag) flags += "CWR ";
    return flags.empty() ? "None" : flags;
}

std::string PacketAnalyzer::getHttpMethod(pcpp::HttpRequestLayer::HttpMethod method) {
    switch (method) {
        case pcpp::HttpRequestLayer::HttpGET: return "GET";
        case pcpp::HttpRequestLayer::HttpPOST: return "POST";
        case pcpp::HttpRequestLayer::HttpCONNECT: return "CONNECT";
        case pcpp::HttpRequestLayer::HttpDELETE: return "DELETE";
        case pcpp::HttpRequestLayer::HttpHEAD: return "HEAD";
        case pcpp::HttpRequestLayer::HttpOPTIONS: return "OPTIONS";
        case pcpp::HttpRequestLayer::HttpPATCH: return "PATCH";
        case pcpp::HttpRequestLayer::HttpPUT: return "PUT";
        case pcpp::HttpRequestLayer::HttpTRACE: return "TRACE";
        default: return "Unknown";
    }
}

void PacketAnalyzer::printStatistics() {
    std::cout << "\n=== Packet Analysis Statistics ===" << std::endl;
    std::cout << "Total packets: " << packetCount << std::endl;
    std::cout << "Ethernet frames: " << ethernetCount << std::endl;
    std::cout << "IPv4 packets: " << ipv4Count << std::endl;
    std::cout << "IPv6 packets: " << ipv6Count << std::endl;
    std::cout << "TCP segments: " << tcpCount << std::endl;
    std::cout << "UDP datagrams: " << udpCount << std::endl;
    std::cout << "HTTP messages: " << httpCount << std::endl;
    std::cout << "DNS messages: " << dnsCount << std::endl;
    std::cout << "ICMP messages: " << icmpCount << std::endl;
    std::cout << "ARP messages: " << arpCount << std::endl;
}