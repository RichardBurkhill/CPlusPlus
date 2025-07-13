/**
 * @file Comms.cpp
 * @brief Implementation of the Comms class for TCP client communications using WinSock or BSD sockets.
 */

#include "Comms.hpp"
#include <iostream>
#include <cstring>

Comms::Comms(SocketBackend backendType, FramingStrategy strategy)
    : backend(backendType), framing(strategy), sock(INVALID_SOCKET)
{
    initializeSocketAPI();
}

Comms::~Comms() {
    closeSocket();
    cleanupSocketAPI();
}

void Comms::initializeSocketAPI() {
#ifdef _WIN32
    if (backend == SocketBackend::WinSock) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
    }
#endif
}

void Comms::cleanupSocketAPI() {
#ifdef _WIN32
    if (backend == SocketBackend::WinSock) {
        WSACleanup();
    }
#endif
}

void Comms::connectToServer(const std::string& ip, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
#ifdef _WIN32
    serverAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
#else
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
#endif

    if (connect(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        closeSocket();
        throw std::runtime_error("Failed to connect to server");
    }
}

void Comms::sendMessage(const std::string& message) {
    std::string msgToSend = message;
    if (framing == FramingStrategy::CarriageReturn) {
        msgToSend += '\r';
    }

    int bytesSent = send(sock, msgToSend.c_str(), static_cast<int>(msgToSend.length()), 0);
    if (bytesSent == SOCKET_ERROR) {
        throw std::runtime_error("Failed to send message");
    }
}

std::string Comms::receiveMessage() {
    std::string received;
    char buffer[512];
    auto lastReceive = std::chrono::steady_clock::now();

    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        timeval tv{};
        tv.tv_sec = 0;
        tv.tv_usec = 30000; // 30ms

        int activity = select(sock + 1, &readfds, nullptr, nullptr, &tv);

        if (activity > 0 && FD_ISSET(sock, &readfds)) {
            int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) {
                break;
            }

            buffer[bytesReceived] = '\0';
            received += buffer;

            if (framing == FramingStrategy::CarriageReturn &&
                received.find('\r') != std::string::npos) {
                break;
            }

            lastReceive = std::chrono::steady_clock::now();
        } else if (framing == FramingStrategy::Timeout) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastReceive).count() > 30) {
                break;
            }
        }
    }

    if (framing == FramingStrategy::CarriageReturn) {
        size_t pos = received.find('\r');
        if (pos != std::string::npos) {
            received = received.substr(0, pos);
        }
    }

    return received;
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