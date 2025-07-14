#ifndef NETWORK_COMMS_HPP
#define NETWORK_COMMS_HPP

#include "IComms.hpp"
#include <string>
#include <iostream>
#include <vector>

// Platform-specific includes for sockets
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/types.h> // Added for fd_set, timeval
    #include <sys/socket.h>
    #include <netinet/in.h> // Corrected typo here
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/select.h>
#endif

/**
 * @brief Implements IComms for network (TCP/UDP) communication.
 * Currently supports TCP client connections.
 */
class NetworkComms : public IComms {
public:
    /**
     * @brief Enum for network protocol type.
     */
    enum class Protocol {
        TCP,
        UDP // Not implemented yet, but for future expansion
    };

    /**
     * @brief Constructor for NetworkComms.
     */
    NetworkComms();

    /**
     * @brief Destructor. Closes the connection.
     */
    ~NetworkComms();

    /**
     * @brief Establishes a connection to a remote host.
     * @param host The hostname or IP address to connect to.
     * @param port The port number to connect to.
     * @param protocol The network protocol to use (TCP or UDP).
     * @return True if connection is successful, false otherwise.
     */
    bool connect(const std::string& host, const std::string& port, Protocol protocol = Protocol::TCP);

    /**
     * @brief Closes the network connection.
     */
    void close();

    /**
     * @brief Reads a specified number of bytes from the network with a timeout.
     * @param numBytes The maximum number of bytes to attempt to read.
     * @param timeoutMs The timeout in milliseconds for the read operation.
     * @return A string containing the bytes read. Returns an empty string if no data
     *         is available within the timeout or an error occurs.
     */
    std::string readBytes(size_t numBytes, unsigned int timeoutMs) override;

    /**
     * @brief Checks if the network connection is currently open.
     * @return True if open, false otherwise.
     */
    bool isOpen() const override;

private:
#ifdef _WIN32
    SOCKET _socket;
#else
    int _socket;
#endif
    bool _isOpen;
    Protocol _protocol;

    // Helper for platform-specific socket initialization
    bool initSocketLayer();
    // Helper for platform-specific socket cleanup
    void cleanupSocketLayer();
};

#endif // NETWORK_COMMS_HPP
