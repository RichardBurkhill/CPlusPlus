#include "NMEAParser.hpp"
#include <iostream>
#include <iomanip>

// Simple checksum validation (for demonstration purposes)
// A real NMEA parser would calculate the XOR sum of all characters between $ and *.
bool NMEAParser::validateChecksum(const std::string& sentence) {
    size_t asteriskPos = sentence.find('*');
    if (asteriskPos == std::string::npos || asteriskPos + 2 >= sentence.length()) {
        // No checksum or incomplete checksum
        return false;
    }

    // For this placeholder, we just check if there are two hex characters after '*'
    // A real implementation would calculate and compare the checksum.
    char c1 = sentence[asteriskPos + 1];
    char c2 = sentence[asteriskPos + 2];

    return (isxdigit(c1) && isxdigit(c2));
}

std::shared_ptr<NMEAMessage> NMEAParser::parse(const std::string& sentence) {
    if (sentence.empty() || sentence[0] != '$') {
        throw std::runtime_error("Invalid NMEA sentence format: does not start with '$'");
    }

    if (!validateChecksum(sentence)) {
        throw std::runtime_error("NMEA checksum validation failed for: " + sentence);
    }

    // Extract the message type (e.g., GPGGA, GPRMC)
    size_t commaPos = sentence.find(',');
    if (commaPos == std::string::npos) {
        throw std::runtime_error("Invalid NMEA sentence: no comma found");
    }
    std::string messageTypeStr = sentence.substr(1, commaPos - 1);

    // Create appropriate NMEAMessage derived class based on type
    if (messageTypeStr == "GPGGA" || messageTypeStr == "GNGGA") {
        return std::make_shared<GGAMessage>(sentence);
    } else if (messageTypeStr == "GPRMC" || messageTypeStr == "GNRMC") {
        return std::make_shared<RMCMessage>(sentence);
    } else {
        // For unknown types, return a generic NMEAMessage or throw
        throw std::runtime_error("Unsupported NMEA message type: " + messageTypeStr);
    }
}
