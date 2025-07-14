#ifndef NMEA_PARSER_HPP
#define NMEA_PARSER_HPP

#include <string>
#include <stdexcept>
#include <memory>

// Forward declaration for NMEAMessage base class
class NMEAMessage
{
public:
    enum class MessageType
    {
        UNKNOWN,
        GGA, // Global Positioning System Fix Data
        RMC, // Recommended Minimum Specific GNSS Data
        // Add more NMEA message types as needed
    };

    virtual ~NMEAMessage() = default;
    virtual MessageType getType() const = 0;
    virtual std::string toString() const = 0;
    std::string rawSentence;
};

// Example derived NMEA message type
class GGAMessage : public NMEAMessage
{
public:
    GGAMessage(const std::string &raw) { rawSentence = raw; }
    MessageType getType() const override { return MessageType::GGA; }
    std::string toString() const override { return "GGA Message: " + rawSentence; }
    // Add parsed GGA fields here
};

// Example derived NMEA message type
class RMCMessage : public NMEAMessage
{
public:
    RMCMessage(const std::string &raw) { rawSentence = raw; }
    MessageType getType() const override { return MessageType::RMC; }
    std::string toString() const override { return "RMC Message: " + rawSentence; }
    // Add parsed RMC fields here
};

/**
 * @brief A placeholder NMEA parser class.
 * In a real application, this would contain full parsing logic and checksum validation.
 */
class NMEAParser
{
public:
    /**
     * @brief Parses a complete NMEA sentence.
     * @param sentence The NMEA sentence string (e.g., "$GPGGA,...").
     * @return A shared pointer to an NMEAMessage object.
     * @throws std::runtime_error if the sentence is invalid or parsing fails.
     */
    static std::shared_ptr<NMEAMessage> parse(const std::string &sentence);

private:
    // Helper to validate checksum (simplified for placeholder)
    static bool validateChecksum(const std::string &sentence);
};

#endif // NMEA_PARSER_HPP
