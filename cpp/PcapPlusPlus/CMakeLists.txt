cmake_minimum_required(VERSION 3.18)
project(PcapPlusPlusPacketParserPOC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(include)

# Add external libraries using ExternalProject
include(ExternalProject)

# Build libpcap first as it's a dependency of PcapPlusPlus
ExternalProject_Add(libpcap
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/libpcap
    CMAKE_ARGS 
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_SHARED_LIBS=OFF
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libpcap-install
        -DENABLE_DBUS=OFF
        -DENABLE_BLUETOOTH=OFF
    INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
)

# Build PcapPlusPlus with libpcap as dependency
ExternalProject_Add(PcapPlusPlus
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/PcapPlusPlus
    CMAKE_ARGS 
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DPCAPPP_BUILD_EXAMPLES=OFF
        -DPCAPPP_BUILD_TESTS=OFF
        -DPCAPPP_BUILD_PCAPPP=ON
        # PCAPPP_BUILD_PCAPPP controls compilation of Pcap++ allowing Common++ and Packet++ to be compiled.
        # Pcap++ depends on libpcap and provides file I/O.
        -DPCAP_ROOT=${CMAKE_BINARY_DIR}/libpcap-install
        -DCMAKE_CXX_FLAGS=-Wno-unused-function
    INSTALL_COMMAND ""
    DEPENDS libpcap
)

# Create imported targets for PcapPlusPlus libraries
add_library(PcapPlusPlus::Common++ STATIC IMPORTED)
set_target_properties(PcapPlusPlus::Common++ PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/PcapPlusPlus-prefix/src/PcapPlusPlus-build/Common++/libCommon++.a
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/PcapPlusPlus/Common++/header"
)
add_dependencies(PcapPlusPlus::Common++ PcapPlusPlus)

add_library(PcapPlusPlus::Packet++ STATIC IMPORTED)
set_target_properties(PcapPlusPlus::Packet++ PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/PcapPlusPlus-prefix/src/PcapPlusPlus-build/Packet++/libPacket++.a
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/PcapPlusPlus/Packet++/header"
    INTERFACE_LINK_LIBRARIES PcapPlusPlus::Common++
)
add_dependencies(PcapPlusPlus::Packet++ PcapPlusPlus)

add_library(PcapPlusPlus::Pcap++ STATIC IMPORTED)
set_target_properties(PcapPlusPlus::Pcap++ PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/PcapPlusPlus-prefix/src/PcapPlusPlus-build/Pcap++/libPcap++.a
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/PcapPlusPlus/Pcap++/header"
    INTERFACE_LINK_LIBRARIES "PcapPlusPlus::Packet++;${CMAKE_BINARY_DIR}/libpcap-install/lib/libpcap.a;pthread;dbus-1"
)
add_dependencies(PcapPlusPlus::Pcap++ PcapPlusPlus libpcap)


# Create executable from source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(packet_parser_poc ${SOURCES})

# Link libraries
target_link_libraries(packet_parser_poc 
    PcapPlusPlus::Packet++
    PcapPlusPlus::Pcap++
)

# Set output directory
set_target_properties(packet_parser_poc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable debug symbols in debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(packet_parser_poc PRIVATE -g -O0)
endif()

# Add compiler flags
target_compile_options(packet_parser_poc PRIVATE -Wall -Wextra)