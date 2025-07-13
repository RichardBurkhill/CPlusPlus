/**
 * @file Comms.cpp
 * @brief Implementation of the Comms class supporting various framing strategies.
 */

#include "Comms.hpp"
#include <iostream>
#include <cstring>

#ifdef _WIN32
    static bool winsock_initialized = false;
#endif

Comms::Comms(SocketBackend backend, FramingStrategy framing)
    : backend(backend), framing(framing), sock(INVALID_SOCKET) {
    initializeSocketAPI();
}

Comms::~Comms() {
    closeSocket();
    cleanupSocketAPI();
}

void Comms::initializeSocketAPI() {
#ifdef _WIN32
    if (!winsock_initialized) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
        winsock_initialized = true;
    }
#endif
}

void Comms::cleanupSocketAPI() {
#ifdef _WIN32
    // Optional: WSACleanup() could be called, but avoid if other sockets are in use.
#endif
}

void Comms::closeSocket() {
    if (sock != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        sock = INVALID_SOCKET;
    }
}

void Comms::connectToServer(const std::string& ip, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        closeSocket();
        throw std::runtime_error("Invalid address");
    }

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closeSocket();
        throw std::runtime_error("Connection failed");
    }
}

void Comms::sendMessage(const std::string& message) {
    std::string framedMessage = message;

    switch (framing) {
        case FramingStrategy::CRLF:
            framedMessage += "\r\n";
            break;
        case FramingStrategy::LengthPrefix: {
            uint32_t len = htonl(static_cast<uint32_t>(message.size()));
            framedMessage = std::string(reinterpret_cast<char*>(&len), sizeof(len)) + message;
            break;
        }
        case FramingStrategy::Timeout:
        case FramingStrategy::None:
            break; // No framing
    }

    if (send(sock, framedMessage.c_str(), static_cast<int>(framedMessage.size()), 0) == SOCKET_ERROR) {
        throw std::runtime_error("Failed to send message");
    }
}

std::string Comms::receiveMessage() {
    switch (framing) {
        case FramingStrategy::CRLF:
            return receiveUntilCRLF();
        case FramingStrategy::LengthPrefix:
            return receiveWithLengthPrefix();
        case FramingStrategy::Timeout:
            return receiveWithTimeout();
        case FramingStrategy::None: {
            char buffer[1024];
            int received = recv(sock, buffer, sizeof(buffer), 0);
            if (received <= 0) {
                throw std::runtime_error("Connection closed or error receiving data");
            }
            return std::string(buffer, received);
        }
    }
    return {};
}

std::string Comms::receiveUntilCRLF() {
    std::string buffer;
    char ch;
    while (true) {
        int result = recv(sock, &ch, 1, 0);
        if (result <= 0) break;
        buffer += ch;
        if (buffer.size() >= 2 && buffer.substr(buffer.size() - 2) == "\r\n") {
            buffer.resize(buffer.size() - 2); // remove CRLF
            break;
        }
    }
    return buffer;
}

std::string Comms::receiveWithLengthPrefix() {
    uint32_t len = 0;
    int total = 0;
    char* lenPtr = reinterpret_cast<char*>(&len);
    while (total < 4) {
        int received = recv(sock, lenPtr + total, 4 - total, 0);
        if (received <= 0) {
            throw std::runtime_error("Failed to receive message length");
        }
        total += received;
    }
    len = ntohl(len);

    std::string buffer(len, 0);
    total = 0;
    while (total < static_cast<int>(len)) {
        int received = recv(sock, &buffer[total], len - total, 0);
        if (received <= 0) {
            throw std::runtime_error("Failed to receive message body");
        }
        total += received;
    }

    return buffer;
}

std::string Comms::receiveWithTimeout() {
    std::string buffer;
    char temp[256];
    auto last_received = std::chrono::steady_clock::now();

    while (true) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sock, &fds);

        timeval timeout {};
        timeout.tv_sec = 0;
        timeout.tv_usec = 30000; // 30ms

        int ret = select(sock + 1, &fds, nullptr, nullptr, &timeout);
        if (ret <= 0) break;

        int received = recv(sock, temp, sizeof(temp), 0);
        if (received <= 0) break;

        buffer.append(temp, received);
        last_received = std::chrono::steady_clock::now();
    }

    return buffer;
}