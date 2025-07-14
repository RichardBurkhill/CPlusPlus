#include "NetworkComms.hpp"
#include <stdexcept> // For std::runtime_error
#include <chrono>
#include <thread>

#ifndef _WIN32
#include <netdb.h> // Explicitly include for addrinfo, getaddrinfo, etc. on non-Windows
#endif

#ifdef _WIN32
// Windows-specific socket initialization
bool NetworkComms::initSocketLayer()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

// Windows-specific socket cleanup
void NetworkComms::cleanupSocketLayer()
{
    WSACleanup();
}
#else
// Linux/macOS-specific socket initialization (no special init needed)
bool NetworkComms::initSocketLayer()
{
    return true;
}

// Linux/macOS-specific socket cleanup (no special cleanup needed)
void NetworkComms::cleanupSocketLayer()
{
    // No specific cleanup for socket layer on Linux/macOS
}
#endif

NetworkComms::NetworkComms() : _isOpen(false), _protocol(Protocol::TCP)
{
#ifdef _WIN32
    _socket = INVALID_SOCKET;
#else
    _socket = -1;
#endif
    if (!initSocketLayer())
    {
        std::cerr << "Error: Failed to initialize socket layer." << std::endl;
        // Depending on severity, could throw an exception here
    }
}

NetworkComms::~NetworkComms()
{
    close();
    cleanupSocketLayer();
}

bool NetworkComms::connect(const std::string &host, const std::string &port, Protocol protocol)
{
    if (_isOpen)
    {
        std::cerr << "Error: Already connected." << std::endl;
        return false;
    }

    _protocol = protocol;

    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = (protocol == Protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;

    if ((status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return false;
    }

    // Loop through all the results and connect to the first we can
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next)
    {
#ifdef _WIN32
        _socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_socket == INVALID_SOCKET)
        {
            std::cerr << "socket error: " << WSAGetLastError() << std::endl;
            continue;
        }
#else
        _socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_socket == -1)
        {
            std::cerr << "socket error: " << strerror(errno) << std::endl;
            continue;
        }
#endif

        if (protocol == Protocol::TCP)
        {
            if (::connect(_socket, p->ai_addr, p->ai_addrlen) == -1)
            {
#ifdef _WIN32
                std::cerr << "connect error: " << WSAGetLastError() << std::endl;
                closesocket(_socket);
                _socket = INVALID_SOCKET;
#else
                std::cerr << "connect error: " << strerror(errno) << std::endl;
                ::close(_socket);
                _socket = -1;
#endif
                continue;
            }
        }
        else
        { // UDP - no explicit connect, just bind if needed or sendto
            // For UDP, 'connect' sets the default destination for send/recv
            // but doesn't establish a connection in the TCP sense.
            // We'll treat it as connected if socket creation is successful.
        }

        _isOpen = true;
        break; // If we get here, we're connected
    }

    freeaddrinfo(res); // All done with this structure

    if (!_isOpen)
    {
        std::cerr << "Error: Failed to connect to " << host << ":" << port << std::endl;
        return false;
    }

    // Set socket to non-blocking for readBytes timeout
#ifdef _WIN32
    u_long mode = 1; // 1 to enable non-blocking socket
    if (ioctlsocket(_socket, FIONBIO, &mode) != 0)
    {
        std::cerr << "ioctlsocket failed with error: " << WSAGetLastError() << std::endl;
        close();
        return false;
    }
#else
    int flags = fcntl(_socket, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "fcntl(F_GETFL) error: " << strerror(errno) << std::endl;
        close();
        return false;
    }
    if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl(F_SETFL, O_NONBLOCK) error: " << strerror(errno) << std::endl;
        close();
        return false;
    }
#endif

    std::cout << "Successfully connected to " << host << ":" << port << std::endl;
    return true;
}

void NetworkComms::close()
{
    if (_isOpen)
    {
#ifdef _WIN32
        closesocket(_socket);
        _socket = INVALID_SOCKET;
#else
        ::close(_socket);
        _socket = -1;
#endif
        _isOpen = false;
        std::cout << "Network connection closed." << std::endl;
    }
}

std::string NetworkComms::readBytes(size_t numBytes, unsigned int timeoutMs)
{
    if (!_isOpen)
    {
        // std::cerr << "Error: Network not connected. Cannot read bytes." << std::endl;
        return "";
    }

    std::string receivedData;
    receivedData.reserve(numBytes);
    char buffer[1]; // Read one byte at a time for simplicity, can optimize with larger buffer

    fd_set readSet;
    struct timeval tv;

    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);

    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;

    int selectResult = select(static_cast<int>(_socket) + 1, &readSet, NULL, NULL, &tv);

    if (selectResult == -1)
    {
#ifdef _WIN32
        std::cerr << "select error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "select error: " << strerror(errno) << std::endl;
#endif
        close(); // Close connection on select error
        return "";
    }
    else if (selectResult == 0)
    {
        // Timeout, no data available
        return "";
    }

    // Data is available, attempt to read
    if (FD_ISSET(_socket, &readSet))
    {
        // Read up to numBytes, or until no more data is immediately available
        // This loop is to drain as much data as possible within the non-blocking context
        // without waiting for another select timeout.
        while (receivedData.length() < numBytes)
        {
            int bytesRead = recv(_socket, buffer, 1, 0);
            if (bytesRead > 0)
            {
                receivedData += buffer[0];
            }
            else if (bytesRead == 0)
            {
                // Connection closed by peer
                std::cerr << "Network connection closed by peer." << std::endl;
                close();
                break;
            }
            else
            {
                // bytesRead < 0
#ifdef _WIN32
                int error = WSAGetLastError();
                if (error == WSAEWOULDBLOCK)
                {
                    // No more data immediately available
                    break;
                }
                std::cerr << "recv error: " << error << std::endl;
#else
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // No more data immediately available
                    break;
                }
                std::cerr << "recv error: " << strerror(errno) << std::endl;
#endif
                close(); // Close connection on recv error
                break;
            }
        }
    }

    return receivedData;
}

bool NetworkComms::isOpen() const
{
    return _isOpen;
}
