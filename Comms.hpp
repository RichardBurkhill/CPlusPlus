/**
 * @file Comms.hpp
 * @brief Cross-platform communication class supporting WinSock and BSD sockets.
 * @details Provides basic TCP client functionality with framing strategies based on
 * carriage return or timeout.
 */

#ifndef COMMS_HPP
#define COMMS_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <thread>

#ifdef _WIN32
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    using SocketType = SOCKET;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    using SocketType = int;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR   (-1)
#endif

/// @brief Supported socket APIs
enum class SocketBackend {
    WinSock,
    BSD
};

/// @brief Message framing strategy
enum class FramingStrategy {
    CarriageReturn, ///< Message ends with '\r'
    Timeout         ///< Message ends when no data for 30ms
};

/// @brief Basic TCP client communications class
class Comms {
public:
    Comms(SocketBackend backend, FramingStrategy framing);
    ~Comms();

    void connectToServer(const std::string& ip, int port);
    void sendMessage(const std::string& message);
    std::string receiveMessage();

private:
    SocketBackend backend;
    FramingStrategy framing;
    SocketType sock;

    void initializeSocketAPI();
    void cleanupSocketAPI();
    void closeSocket();
};

#endif // COMMS_HPP
