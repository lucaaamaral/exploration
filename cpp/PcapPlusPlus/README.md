# PcapPlusPlus Packet Parser POC

A proof of concept demonstrating how to integrate PcapPlusPlus external libraries using CMake for packet parsing and analysis.

## Overview

This project showcases the integration of PcapPlusPlus libraries as external dependencies using CMake's ExternalProject functionality. It demonstrates both packet creation/parsing capabilities and file I/O operations for pcap files.

## Architecture

### External Dependencies

The project uses two main external libraries:

1. **libpcap** - The foundational packet capture library
   - Built as a static library with D-Bus support
   - Provides low-level packet capture and file I/O functionality
   - Installed to `build/libpcap-install/`

2. **PcapPlusPlus** - High-level C++ packet manipulation library
   - **Common++**: Basic utilities and common functionality
   - **Packet++**: Packet parsing, creation, and manipulation (independent of libpcap)
   - **Pcap++**: File I/O operations and live capture (depends on libpcap)

### Library Relationships

```
packet_parser_poc
└── PcapPlusPlus::Pcap++
    ├── PcapPlusPlus::Packet++
    │   └── PcapPlusPlus::Common++
    └── libpcap (+ system deps: pthread, dbus-1)
```

## Build System

### CMake Configuration

The project uses `ExternalProject_Add` to build dependencies:

1. **libpcap** builds first as a prerequisite
2. **PcapPlusPlus** builds second with `PCAP_ROOT` pointing to the libpcap installation
3. **Main executable** links against the imported targets

### Key CMake Features

- **Static linking**: All libraries built as static (.a) files
- **Dependency management**: Proper build order using `DEPENDS`
- **Imported targets**: Clean interface using `PcapPlusPlus::*` namespaced targets
- **Documentation**: Comments explaining library purposes and relationships

## Building

```bash
# Configure and build
mkdir build && cd build
cmake ..
cmake --build .

# Run the executable
./bin/packet_parser_poc
```

## Project Structure

```
├── CMakeLists.txt              # Main build configuration
├── README.md                   # This file
├── assets/                     # Sample capture files for testing
├── include/                    # Public headers
│   ├── PacketAnalyzer.h
│   └── PacketUtils.h
├── src/                        # Source files
│   ├── main.cpp
│   ├── PacketAnalyzer.cpp
│   └── PacketUtils.cpp
├── external/                   # External dependencies
│   ├── libpcap/               # Packet capture library
│   └── PcapPlusPlus/          # High-level packet manipulation
└── build/                     # Build artifacts (generated)
    ├── bin/packet_parser_poc  # Final executable
    ├── libpcap-install/       # Installed libpcap
    └── PcapPlusPlus-prefix/   # Built PcapPlusPlus
```

## Sample Capture Files

The `assets/` directory contains sample packet capture files downloaded from [Wireshark's Sample Captures](https://wiki.wireshark.org/samplecaptures). These files provide diverse examples for testing and demonstrating the packet parsing capabilities:

### Network Protocols
- **arp.pcap**: Address Resolution Protocol packets
- **bgp.pcapng.gz**: Border Gateway Protocol routing updates
- **eigrp-for-ipv6-auth.pcap**: Enhanced Interior Gateway Routing Protocol for IPv6
- **lldp.detailed.pcap**: Link Layer Discovery Protocol with detailed information
- **mesh.pcap**: Mesh networking communications
- **sctp.cap**: Stream Control Transmission Protocol
- **IPv6to4.pcap**: IPv6-to-IPv4 tunneling traffic

### Application Layer
- **http.cap**: HTTP web traffic (requests and responses)
- **smb.cap**: Windows file sharing (Server Message Block)
- **telnet-cooked.pcap** & **telnet-raw.pcap**: Telnet terminal sessions
- **NTP_sync.pcap**: Network Time Protocol synchronization
- **grpc_person_search_protobuf_with_image.pcapng**: gRPC with Protocol Buffers

### Modern Protocols
- **openflow_v1.3_messages.pcapng.gz**: Software-defined networking (OpenFlow)
- **protobuf_tcp_addressbook.pcapng**: Protocol Buffers over TCP
- **protobuf_udp_addressbook.pcapng**: Protocol Buffers over UDP

### Security & Wireless
- **wpa-Induction.pcap**: WPA wireless security protocols

### Hardware/Device Communication
- **VariousUSBDevices.pcap**: USB device communications
- **eth.cap** & **eth_v1.cap**: Basic Ethernet frame examples

### Usage with the Application
```bash
# Analyze a specific capture file
./bin/packet_parser_poc assets/http.cap

# Parse ARP traffic
./bin/packet_parser_poc assets/arp.pcap

# Examine Protocol Buffers communication
./bin/packet_parser_poc assets/protobuf_tcp_addressbook.pcapng
```

## Features Demonstrated

### Packet Analysis
- Protocol detection (Ethernet, IPv4/IPv6, TCP/UDP, HTTP, DNS, ICMP, ARP)
- Layer-by-layer packet parsing
- Statistics collection and reporting

### Packet Creation
- Programmatic packet construction
- Multi-layer packet building (Ethernet → IP → TCP → Payload)
- Checksum calculation

### File I/O (when Pcap++ available)
- Reading pcap/pcapng files
- Support for compressed files (.pcapng.gz)
- Packet-by-packet processing
- Integration with analysis engine

## Library Usage Notes

### Packet++ Only (No File I/O)
When built with `PCAPPP_BUILD_PCAPPP=OFF`:
- Only Common++ and Packet++ are available
- Packet parsing and creation work normally
- File I/O functionality is not available
- No libpcap dependency required

### Full PcapPlusPlus (With File I/O)
When built with `PCAPPP_BUILD_PCAPPP=ON` (current configuration):
- All components available: Common++, Packet++, and Pcap++
- File reading/writing capabilities included
- libpcap dependency required
- Additional system dependencies (pthread, dbus-1)

## Technical Details

### Compiler Requirements
- C++17 or later
- GCC/Clang with standard library support

### System Dependencies
- pthread (POSIX threads)
- dbus-1 (D-Bus library for libpcap)
- Standard Linux development tools

### Build Configuration
- Static libraries for reproducible builds
- Position-independent code enabled
- Debug symbols in Debug builds
- Strict compiler warnings enabled

## Troubleshooting

### Common Issues

1. **"PCAP library not found"**
   - Ensure libpcap builds successfully first
   - Check PCAP_ROOT points to correct installation

2. **D-Bus linking errors**
   - Install libdbus-1-dev package
   - Ensure dbus-1 is in link libraries

3. **Missing headers**
   - Verify external/ submodules are properly initialized
   - Check include directory paths in CMake configuration

4. **Compressed file issues**
   - Some sample files (.gz) may need decompression first
   - Use `gunzip` to decompress .pcapng.gz files

## License

This project is for demonstration purposes. Please refer to individual library licenses:
- libpcap: BSD license
- PcapPlusPlus: Unlicense
- Sample capture files: Various sources, refer to [Wireshark's Sample Captures](https://wiki.wireshark.org/samplecaptures) for attribution