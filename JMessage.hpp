#ifndef LINK16_JMESSAGE_HPP
#define LINK16_JMESSAGE_HPP

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>

namespace Link16 {

// Read 16-bit big-endian word from buffer
inline uint16_t readBE16(const uint8_t* data) {
    return (uint16_t(data[0]) << 8) | uint16_t(data[1]);
}

// Read 32-bit big-endian dword from buffer
inline uint32_t readBE32(const uint8_t* data) {
    return (uint32_t(data[0]) << 24) |
           (uint32_t(data[1]) << 16) |
           (uint32_t(data[2]) << 8)  |
           uint32_t(data[3]);
}

// Base class for J-series messages
class JMessage {
public:
    virtual ~JMessage() = default;

    uint16_t message_type;   // e.g. 3 for J3
    uint16_t message_length; // words (16-bit) including header

    void parseHeader(const uint8_t* buffer, size_t size) {
        if (size < 4) throw std::runtime_error("Buffer too small for JMessage header");
        message_type = readBE16(buffer);
        message_length = readBE16(buffer + 2);
        if (message_length * 2 > size) throw std::runtime_error("Buffer smaller than message length");
    }

    virtual void parseBody(const uint8_t* buffer, size_t size) = 0;

    virtual std::string toString() const = 0;

    static std::unique_ptr<JMessage> createFromBuffer(const uint8_t* buffer, size_t size);
};

// J3 Identity Message
class J3Message : public JMessage {
public:
    uint8_t platform_id;      // 8 bits - platform ID
    uint8_t emitter_category; // 8 bits - emitter category
    uint8_t system_status;    // 8 bits - system status bits
    uint8_t exercise_id;      // 8 bits - exercise ID

    // Parse J3-specific body (after header)
    void parseBody(const uint8_t* buffer, size_t size) override {
        if (size < 4) throw std::runtime_error("Buffer too small for J3 body");
        platform_id = buffer[0];
        emitter_category = buffer[1];
        system_status = buffer[2];
        exercise_id = buffer[3];
    }

    std::string toString() const override {
        std::ostringstream oss;
        oss << "J3Message: Platform ID=" << unsigned(platform_id)
            << ", Emitter Category=" << unsigned(emitter_category)
            << ", System Status=0x" << std::hex << int(system_status)
            << ", Exercise ID=" << std::dec << unsigned(exercise_id);
        return oss.str();
    }
};

// J12 Position Message
class J12Message : public JMessage {
public:
    // Example decoded fields:
    // Latitude and Longitude: stored as scaled integers (microdegrees)
    int32_t latitude_microdeg;   // 32 bits
    int32_t longitude_microdeg;  // 32 bits
    int16_t altitude_feet;       // 16 bits signed
    uint16_t horizontal_velocity; // 16 bits unsigned (knots * 10)
    uint16_t heading_deg;         // 16 bits unsigned

    void parseBody(const uint8_t* buffer, size_t size) override {
        if (size < 14) throw std::runtime_error("Buffer too small for J12 body");

        // Assume buffer is big-endian bytes for these fields:
        latitude_microdeg = static_cast<int32_t>(readBE32(buffer));
        longitude_microdeg = static_cast<int32_t>(readBE32(buffer + 4));
        altitude_feet = static_cast<int16_t>(readBE16(buffer + 8));
        horizontal_velocity = readBE16(buffer + 10);
        heading_deg = readBE16(buffer + 12);
    }

    std::string toString() const override {
        std::ostringstream oss;
        oss << "J12Message: Lat=" << latitude_microdeg / 1e6
            << "°, Lon=" << longitude_microdeg / 1e6
            << "°, Alt=" << altitude_feet << " ft"
            << ", Velocity=" << horizontal_velocity / 10.0 << " knots"
            << ", Heading=" << heading_deg << "°";
        return oss.str();
    }
};

inline std::unique_ptr<JMessage> JMessage::createFromBuffer(const uint8_t* buffer, size_t size) {
    if (size < 4) throw std::runtime_error("Buffer too small for message");
    uint16_t msgType = readBE16(buffer);
    uint16_t msgLen = readBE16(buffer + 2);
    if (msgLen * 2 > size) throw std::runtime_error("Buffer smaller than message length");

    std::unique_ptr<JMessage> msg;
    switch (msgType) {
        case 3:  msg = std::make_unique<J3Message>(); break;
        case 12: msg = std::make_unique<J12Message>(); break;
        // Add other message types here as needed
        default:
            throw std::runtime_error("Unsupported J message type: " + std::to_string(msgType));
    }
    msg->parseHeader(buffer, size);
    msg->parseBody(buffer + 4, size - 4);
    return msg;
}

} // namespace Link16

#endif // LINK16_JMESSAGE_HPP
