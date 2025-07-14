#ifndef I_COMMS_HPP
#define I_COMMS_HPP

#include <string>
#include <vector>

/**
 * @brief Abstract interface for communication classes (Serial, Network, etc.).
 * This allows NMEAReader to work with different underlying communication methods.
 */
class IComms
{
public:
    virtual ~IComms() = default;

    /**
     * @brief Reads a specified number of bytes from the communication medium with a timeout.
     * @param numBytes The maximum number of bytes to attempt to read.
     * @param timeoutMs The timeout in milliseconds for the read operation.
     * @return A string containing the bytes read. Returns an empty string if no data
     *         is available within the timeout or an error occurs.
     */
    virtual std::string readBytes(size_t numBytes, unsigned int timeoutMs) = 0;

    /**
     * @brief Checks if the communication medium is currently open.
     * @return True if open, false otherwise.
     */
    virtual bool isOpen() const = 0;
};

#endif // I_COMMS_HPP
