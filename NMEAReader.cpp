#include "NMEAReader.hpp"
#include <algorithm> // For std::find

// Changed constructor parameter from Serial_Comms& to IComms&
NMEAReader::NMEAReader(IComms &comms, unsigned int readTimeoutMs)
    : _comms(comms), // Changed _serialComms to _comms
      _readTimeoutMs(readTimeoutMs)
{
}

std::optional<std::shared_ptr<NMEAMessage>> NMEAReader::readAndParseSentence()
{
    while (true)
    {
        // 1. Try to extract a complete sentence from the buffer first
        std::optional<std::string> nmeaSentence = extractCompleteSentence();
        if (nmeaSentence.has_value())
        {
            // Found a complete sentence, try to parse it
            try
            {
                std::shared_ptr<NMEAMessage> message = NMEAParser::parse(nmeaSentence.value());
                return message; // Successfully parsed a valid NMEA message
            }
            catch (const std::runtime_error &e)
            {
                // Parsing failed (e.g., invalid checksum, unknown sentence type)
                std::cerr << "NMEA Parsing Error: " << e.what() << " for sentence: " << nmeaSentence.value() << std::endl;
                // Continue to read more data, as this sentence was invalid
            }
        }

        // 2. If no complete sentence or parsing failed, read more data from communication medium
        // Changed _serialComms.readBytes to _comms.readBytes
        std::string newData = _comms.readBytes(128, _readTimeoutMs); // Read up to 128 bytes

        if (newData.empty())
        {
            // No new data received within timeout, and no complete sentence in buffer
            // This could mean no data is coming, or a sentence is very long and still partial.
            // If the buffer is empty, we truly have no data.
            if (_receiveBuffer.empty())
            {
                return std::nullopt; // No data, no sentence, nothing to do
            }
            // If buffer is not empty, it means we have a partial sentence and timed out.
            // We can either return nullopt and wait for next call, or keep trying.
            // For now, we return nullopt to indicate no *complete* sentence was found.
            return std::nullopt;
        }

        // 3. Append new data to the buffer
        _receiveBuffer.append(newData);
        // std::cout << "Buffer after append: " << _receiveBuffer << std::endl; // Debugging
    }
}

std::optional<std::string> NMEAReader::extractCompleteSentence()
{
    // Find the start of an NMEA sentence
    size_t startPos = _receiveBuffer.find('$');

    if (startPos == std::string::npos)
    {
        // No '$' found, buffer contains only garbage or partial data without a start.
        // Clear the buffer to prevent it from growing indefinitely with garbage.
        _receiveBuffer.clear();
        return std::nullopt;
    }

    // Discard any leading garbage before the '$'
    if (startPos > 0)
    {
        // std::cout << "Discarding garbage: " << _receiveBuffer.substr(0, startPos) << std::endl; // Debugging
        _receiveBuffer.erase(0, startPos);
    }

    // Now, the buffer starts with '$'. Find the end of the sentence (CRLF).
    size_t endPos = _receiveBuffer.find("\r\n");

    if (endPos == std::string::npos)
    {
        // No CRLF found, sentence is incomplete.
        return std::nullopt;
    }

    // A complete sentence found. Extract it.
    // NMEA sentences include the '$' but not the CRLF.
    std::string completeSentence = _receiveBuffer.substr(0, endPos);

    // Remove the extracted sentence (including CRLF) from the buffer
    _receiveBuffer.erase(0, endPos + 2); // +2 for \r\n

    // Basic NMEA sentence validation: must contain a checksum part (*XX)
    size_t checksumDelimiterPos = completeSentence.find('*');
    if (checksumDelimiterPos == std::string::npos || checksumDelimiterPos + 2 >= completeSentence.length())
    {
        // Invalid NMEA format: no checksum delimiter or checksum too short
        std::cerr << "Invalid NMEA format (missing/short checksum): " << completeSentence << std::endl;
        return std::nullopt; // Treat as invalid and discard
    }

    // std::cout << "Extracted sentence: " << completeSentence << std::endl; // Debugging
    return completeSentence;
}
