#ifndef NMEA_READER_HPP
#define NMEA_READER_HPP

#include "IComms.hpp" // Include the new IComms interface
#include "NMEAParser.hpp" // Assuming NMEAParser.hpp is available
#include <string>
#include <optional>
#include <memory> // For std::shared_ptr
#include <iostream> // For debugging output

/**
 * @brief A class to read and parse NMEA sentences from an IComms stream.
 *
 * This class handles buffering and extracting complete NMEA sentences from a
 * raw byte stream provided by any class implementing IComms, and then uses NMEAParser to
 * validate and parse them.
 */
class NMEAReader {
public:
    /**
     * @brief Constructs an NMEAReader.
     * @param comms A reference to an initialized IComms object (e.g., Serial_Comms or NetworkComms).
     * @param readTimeoutMs The timeout in milliseconds for each individual read operation
     *                      from the communication medium.
     */
    NMEAReader(IComms& comms, unsigned int readTimeoutMs = 100);

    /**
     * @brief Reads data from the communication medium, buffers it, and attempts to parse
     *        a complete and valid NMEA sentence.
     *
     * This method will continue reading from the communication medium until a complete
     * NMEA sentence is found and successfully parsed, or until a read timeout
     * occurs without any new data or a complete sentence in the buffer.
     *
     * @return An optional shared pointer to an NMEAMessage if a valid sentence
     *         is parsed, otherwise std::nullopt.
     */
    std::optional<std::shared_ptr<NMEAMessage>> readAndParseSentence();

private:
    IComms& _comms; // Changed from Serial_Comms& to IComms&
    unsigned int _readTimeoutMs;
    std::string _receiveBuffer; // Buffer to hold partial sentences and multiple sentences

    /**
     * @brief Attempts to extract a complete NMEA sentence from the internal buffer.
     *
     * A complete NMEA sentence starts with '$' and ends with '\r\n'.
     * This method also handles discarding leading garbage data.
     *
     * @return An optional string containing the complete NMEA sentence (without CRLF)
     *         if found, otherwise std::nullopt. The extracted sentence is removed
     *         from the internal buffer.
     */
    std::optional<std::string> extractCompleteSentence();
};

#endif // NMEA_READER_HPP
