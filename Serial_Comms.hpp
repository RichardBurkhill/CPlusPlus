#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

// Platform-specific includes
#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif

class Serial_Comms {
public:

    // Enum for common baud rates
    enum class BaudRate {
        BR_9600,
        BR_19200,
        BR_38400,
        BR_57600,
        BR_115200,
        BR_230400
    };

    // Enum for data bits
    enum class DataBits {
        DB_5,
        DB_6,
        DB_7,
        DB_8
    };

    // Enum for parity
    enum class Parity {
        None,
        Odd,
        Even,
        Mark, // Windows only, often treated as None or Odd
        Space // Windows only, often treated as None or Even
    };

    // Enum for stop bits
    enum class StopBits {
        SB_1,
        SB_1_5, // Windows only
        SB_2
    };

    // Enum for flow control
    enum class FlowControl {
        None,
        Hardware, // RTS/CTS
        Software  // XON/XOFF
    };

    // Enum for message termination methods
    enum class TerminationMethod {
        None,
        CR,    // Carriage Return (\r)
        LF,    // Line Feed (\n)
        CRLF   // Carriage Return + Line Feed (\r\n)
    };

    // Constructor
    Serial_Comms();

    // Destructor
    ~Serial_Comms();

    // Open the serial port
    bool open(const std::string& portName);

    // Close the serial port
    void close();

    // Configure the serial port parameters
    bool configure(
        BaudRate baudRate,
        DataBits dataBits,
        Parity parity,
        StopBits stopBits,
        FlowControl flowControl
    );

    // Write data to the serial port with optional termination
    bool write(const std::string& data, TerminationMethod termination = TerminationMethod::None);

    // Read data from the serial port until a terminator is found or timeout/maxLength reached
    std::string read(
        TerminationMethod termination,
        unsigned int timeoutMs,
        size_t maxLength = 0 // 0 means no max length, read until terminator or timeout
    );

    // Read a fixed number of bytes from the serial port with timeout
    std::string readBytes(size_t numBytes, unsigned int timeoutMs);

    // Check if the port is open
    bool isOpen() const;

private:
    // Platform-specific handle/file descriptor
#ifdef _WIN32
    HANDLE hSerial;
#else
    int fd;
#endif
    bool _isOpen;

    // Helper to convert BaudRate enum to platform-specific value
    unsigned int getBaudRateValue(BaudRate baudRate);

    // Helper to apply termination characters
    std::string applyTermination(const std::string& data, TerminationMethod termination);
};

// --- Implementation ---

Serial_Comms::Serial_Comms() :
#ifdef _WIN32
    hSerial(INVALID_HANDLE_VALUE),
#else
    fd(-1),
#endif
    _isOpen(false) {}

Serial_Comms::~Serial_Comms() {
    close();
}

bool Serial_Comms::open(const std::string& portName) {
    if (_isOpen) {
        std::cerr << "Error: Port already open." << std::endl;
        return false;
    }

#ifdef _WIN32
    hSerial = CreateFileA(
        portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,      // No sharing
        NULL,   // No security attributes
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, // | FILE_FLAG_OVERLAPPED for async I/O
        NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port " << portName << ": " << GetLastError() << std::endl;
        return false;
    }

    // Set default timeouts (can be reconfigured later)
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting default timeouts: " << GetLastError() << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

#else // Linux
    fd = ::open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK); // O_NONBLOCK for non-blocking reads

    if (fd == -1) {
        std::cerr << "Error opening serial port " << portName << ": " << strerror(errno) << std::endl;
        return false;
    }

    // Clear O_NONBLOCK for blocking reads by default, will be handled by VMIN/VTIME
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);

#endif
    _isOpen = true;
    return true;
}

void Serial_Comms::close() {
    if (_isOpen) {
#ifdef _WIN32
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
#else
        ::close(fd);
        fd = -1;
#endif
        _isOpen = false;
    }
}

bool Serial_Comms::configure(
    BaudRate baudRate,
    DataBits dataBits,
    Parity parity,
    StopBits stopBits,
    FlowControl flowControl
) {
    if (!_isOpen) {
        std::cerr << "Error: Port not open. Cannot configure." << std::endl;
        return false;
    }

#ifdef _WIN32
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting current serial port state: " << GetLastError() << std::endl;
        return false;
    }

    dcbSerialParams.BaudRate = getBaudRateValue(baudRate);

    switch (dataBits) {
        case DataBits::DB_5: dcbSerialParams.ByteSize = 5; break;
        case DataBits::DB_6: dcbSerialParams.ByteSize = 6; break;
        case DataBits::DB_7: dcbSerialParams.ByteSize = 7; break;
        case DataBits::DB_8: dcbSerialParams.ByteSize = 8; break;
    }

    switch (parity) {
        case Parity::None:  dcbSerialParams.Parity = NOPARITY;   break;
        case Parity::Odd:   dcbSerialParams.Parity = ODDPARITY;  break;
        case Parity::Even:  dcbSerialParams.Parity = EVENPARITY; break;
        case Parity::Mark:  dcbSerialParams.Parity = MARKPARITY; break;
        case Parity::Space: dcbSerialParams.Parity = SPACEPARITY; break;
    }

    switch (stopBits) {
        case StopBits::SB_1:   dcbSerialParams.StopBits = ONESTOPBIT;     break;
        case StopBits::SB_1_5: dcbSerialParams.StopBits = ONE5STOPBITS;   break;
        case StopBits::SB_2:   dcbSerialParams.StopBits = TWOSTOPBITS;    break;
    }

    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE; // Default to disable DTR
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE; // Default to disable RTS

    switch (flowControl) {
        case FlowControl::None:
            dcbSerialParams.fOutxCtsFlow = FALSE;
            dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
            dcbSerialParams.fOutxXonOut = FALSE;
            dcbSerialParams.fInX = FALSE;
            break;
        case FlowControl::Hardware:
            dcbSerialParams.fOutxCtsFlow = TRUE;
            dcbSerialParams.fRtsControl = RTS_CONTROL_ENABLE; // Or RTS_CONTROL_HANDSHAKE
            dcbSerialParams.fOutxXonOut = FALSE;
            dcbSerialParams.fInX = FALSE;
            break;
        case FlowControl::Software:
            dcbSerialParams.fOutxCtsFlow = FALSE;
            dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
            dcbSerialParams.fOutxXonOut = TRUE;
            dcbSerialParams.fInX = TRUE;
            break;
    }

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state: " << GetLastError() << std::endl;
        return false;
    }

#else // Linux
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error getting termios attributes: " << strerror(errno) << std::endl;
        return false;
    }

    // Set baud rate
    cfsetospeed(&tty, getBaudRateValue(baudRate));
    cfsetispeed(&tty, getBaudRateValue(baudRate));

    // Set data bits
    tty.c_cflag &= ~CSIZE; // Clear current data bit setting
    switch (dataBits) {
        case DataBits::DB_5: tty.c_cflag |= CS5; break;
        case DataBits::DB_6: tty.c_cflag |= CS6; break;
        case DataBits::DB_7: tty.c_cflag |= CS7; break;
        case DataBits::DB_8: tty.c_cflag |= CS8; break;
    }

    // Set parity
    tty.c_cflag &= ~PARENB; // Disable parity by default
    tty.c_cflag &= ~PARODD; // Disable odd parity by default
    switch (parity) {
        case Parity::None:
            tty.c_cflag &= ~PARENB;
            break;
        case Parity::Odd:
            tty.c_cflag |= PARENB;
            tty.c_cflag |= PARODD;
            break;
        case Parity::Even:
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD;
            break;
        case Parity::Mark: // Not directly supported, often treated as None or Odd
        case Parity::Space: // Not directly supported, often treated as None or Even
            std::cerr << "Warning: Mark/Space parity not directly supported on Linux. Using None." << std::endl;
            tty.c_cflag &= ~PARENB;
            break;
    }

    // Set stop bits
    tty.c_cflag &= ~CSTOPB; // Clear stop bit setting (1 stop bit)
    switch (stopBits) {
        case StopBits::SB_1:
            // CSTOPB is 0 for 1 stop bit
            break;
        case StopBits::SB_1_5: // Not directly supported on Linux, use 1 or 2
            std::cerr << "Warning: 1.5 stop bits not directly supported on Linux. Using 1 stop bit." << std::endl;
            break;
        case StopBits::SB_2:
            tty.c_cflag |= CSTOPB; // Set for 2 stop bits
            break;
    }

    // Set flow control
    tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control by default
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control by default

    switch (flowControl) {
        case FlowControl::None:
            break;
        case FlowControl::Hardware:
            tty.c_cflag |= CRTSCTS;
            break;
        case FlowControl::Software:
            tty.c_iflag |= (IXON | IXOFF | IXANY);
            break;
    }

    // Local mode (enable receiver, ignore modem control lines)
    tty.c_cflag |= (CLOCAL | CREAD);

    // Raw input mode (no special processing of input characters)
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Raw output mode (no special processing of output characters)
    tty.c_oflag &= ~OPOST;

    // Input flags (disable parity checking, strip 8th bit, etc.)
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    // Set VMIN and VTIME for read operations (blocking read with timeout)
    // VMIN = 0, VTIME = 0: Non-blocking read (read returns immediately)
    // VMIN > 0, VTIME = 0: Blocking read until VMIN bytes are received
    // VMIN = 0, VTIME > 0: Read with timeout (VTIME * 0.1 seconds)
    // VMIN > 0, VTIME > 0: Blocking read until VMIN bytes or timeout after first byte
    tty.c_cc[VMIN] = 0;  // Minimum number of characters to read
    tty.c_cc[VTIME] = 0; // Timeout in 0.1s increments (will be set dynamically for reads)

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting termios attributes: " << strerror(errno) << std::endl;
        return false;
    }

#endif
    return true;
}

bool Serial_Comms::write(const std::string& data, TerminationMethod termination) {
    if (!_isOpen) {
        std::cerr << "Error: Port not open. Cannot write." << std::endl;
        return false;
    }

    std::string dataToSend = applyTermination(data, termination);

#ifdef _WIN32
    DWORD bytesWritten;
    if (!WriteFile(hSerial, dataToSend.c_str(), dataToSend.length(), &bytesWritten, NULL)) {
        std::cerr << "Error writing to serial port: " << GetLastError() << std::endl;
        return false;
    }
    if (bytesWritten != dataToSend.length()) {
        std::cerr << "Warning: Not all bytes written to serial port." << std::endl;
    }
#else // Linux
    ssize_t bytesWritten = ::write(fd, dataToSend.c_str(), dataToSend.length());
    if (bytesWritten == -1) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
        return false;
    }
    if (static_cast<size_t>(bytesWritten) != dataToSend.length()) {
        std::cerr << "Warning: Not all bytes written to serial port." << std::endl;
    }
#endif
    return true;
}

std::string Serial_Comms::read(TerminationMethod termination, unsigned int timeoutMs, size_t maxLength) {
    if (!_isOpen) {
        std::cerr << "Error: Port not open. Cannot read." << std::endl;
        return "";
    }

    std::string receivedData;
    char buffer[1]; // Read one byte at a time
    auto startTime = std::chrono::high_resolution_clock::now();

    // Define termination characters
    char termChar1 = 0;
    char termChar2 = 0;
    if (termination == TerminationMethod::CR || termination == TerminationMethod::CRLF) {
        termChar1 = '\r';
    }
    if (termination == TerminationMethod::LF || termination == TerminationMethod::CRLF) {
        termChar2 = '\n';
    }

#ifdef _WIN32
    COMMTIMEOUTS originalTimeouts;
    if (!GetCommTimeouts(hSerial, &originalTimeouts)) {
        std::cerr << "Error getting original timeouts: " << GetLastError() << std::endl;
        return "";
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 0; // No interval timeout
    timeouts.ReadTotalTimeoutConstant = timeoutMs; // Total timeout for the read operation
    timeouts.ReadTotalTimeoutMultiplier = 0; // No multiplier
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting read timeouts: " << GetLastError() << std::endl;
        return "";
    }
#else // Linux
    struct termios originalTty;
    if (tcgetattr(fd, &originalTty) != 0) {
        std::cerr << "Error getting original termios attributes: " << strerror(errno) << std::endl;
        return "";
    }
    struct termios tty = originalTty;
    tty.c_cc[VMIN] = 0; // Read at least 0 bytes
    tty.c_cc[VTIME] = timeoutMs / 100; // Timeout in 0.1s increments
    if (tty.c_cc[VTIME] == 0 && timeoutMs > 0) tty.c_cc[VTIME] = 1; // Ensure at least 1 unit if timeout > 0
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting read timeouts: " << strerror(errno) << std::endl;
        return "";
    }
#endif

    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        if (elapsed > timeoutMs) {
            // std::cerr << "Read timeout occurred." << std::endl; // Uncomment for debug
            break;
        }

        if (maxLength > 0 && receivedData.length() >= maxLength) {
            // std::cerr << "Max length reached." << std::endl; // Uncomment for debug
            break;
        }

#ifdef _WIN32
        DWORD bytesRead;
        if (!ReadFile(hSerial, buffer, 1, &bytesRead, NULL)) {
            if (GetLastError() == ERROR_IO_PENDING) {
                // This would be handled with overlapped I/O, but for simple blocking,
                // we just assume it's a timeout if no data is available.
                std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Small delay to prevent busy-waiting
                continue;
            }
            std::cerr << "Error reading from serial port: " << GetLastError() << std::endl;
            break;
        }
        if (bytesRead == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Small delay if no byte read
            continue;
        }
#else // Linux
        ssize_t bytesRead = ::read(fd, buffer, 1);
        if (bytesRead == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue; // No data available yet, continue waiting
            }
            std::cerr << "Error reading from serial port: " << strerror(errno) << std::endl;
            break;
        }
        if (bytesRead == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue; // No data available yet, continue waiting
        }
#endif
        receivedData += buffer[0];

        // Check for termination
        if (termination != TerminationMethod::None) {
            if (termination == TerminationMethod::CR && buffer[0] == termChar1) {
                break;
            } else if (termination == TerminationMethod::LF && buffer[0] == termChar2) {
                break;
            } else if (termination == TerminationMethod::CRLF) {
                if (receivedData.length() >= 2 &&
                    receivedData[receivedData.length() - 2] == termChar1 &&
                    receivedData[receivedData.length() - 1] == termChar2) {
                    break;
                }
            }
        }
    }

    // Restore original timeouts/termios settings
#ifdef _WIN32
    SetCommTimeouts(hSerial, &originalTimeouts);
#else
    tcsetattr(fd, TCSANOW, &originalTty);
#endif

    return receivedData;
}

std::string Serial_Comms::readBytes(size_t numBytes, unsigned int timeoutMs) {
    if (!_isOpen) {
        std::cerr << "Error: Port not open. Cannot read bytes." << std::endl;
        return "";
    }

    std::string receivedData;
    receivedData.reserve(numBytes); // Pre-allocate memory
    char buffer[1];
    auto startTime = std::chrono::high_resolution_clock::now();

#ifdef _WIN32
    COMMTIMEOUTS originalTimeouts;
    if (!GetCommTimeouts(hSerial, &originalTimeouts)) {
        std::cerr << "Error getting original timeouts: " << GetLastError() << std::endl;
        return "";
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = timeoutMs;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting read timeouts: " << GetLastError() << std::endl;
        return "";
    }
#else // Linux
    struct termios originalTty;
    if (tcgetattr(fd, &originalTty) != 0) {
        std::cerr << "Error getting original termios attributes: " << strerror(errno) << std::endl;
        return "";
    }
    struct termios tty = originalTty;
    tty.c_cc[VMIN] = 0; // Read at least 0 bytes
    tty.c_cc[VTIME] = timeoutMs / 100; // Timeout in 0.1s increments
    if (tty.c_cc[VTIME] == 0 && timeoutMs > 0) tty.c_cc[VTIME] = 1;
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting read timeouts: " << strerror(errno) << std::endl;
        return "";
    }
#endif

    while (receivedData.length() < numBytes) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        if (elapsed > timeoutMs) {
            // std::cerr << "Read bytes timeout occurred. Read " << receivedData.length() << " of " << numBytes << " bytes." << std::endl; // Uncomment for debug
            break;
        }

#ifdef _WIN32
        DWORD bytesRead;
        if (!ReadFile(hSerial, buffer, 1, &bytesRead, NULL)) {
            if (GetLastError() == ERROR_IO_PENDING) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            std::cerr << "Error reading from serial port: " << GetLastError() << std::endl;
            break;
        }
        if (bytesRead == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
#else // Linux
        ssize_t bytesRead = ::read(fd, buffer, 1);
        if (bytesRead == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            std::cerr << "Error reading from serial port: " << strerror(errno) << std::endl;
            break;
        }
        if (bytesRead == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
#endif
        receivedData += buffer[0];
    }

    // Restore original timeouts/termios settings
#ifdef _WIN32
    SetCommTimeouts(hSerial, &originalTimeouts);
#else
    tcsetattr(fd, TCSANOW, &originalTty);
#endif

    return receivedData;
}

bool Serial_Comms::isOpen() const {
    return _isOpen;
}

unsigned int Serial_Comms::getBaudRateValue(BaudRate baudRate) {
#ifdef _WIN32
    switch (baudRate) {
        case BaudRate::BR_9600:   return CBR_9600;
        case BaudRate::BR_19200:  return CBR_19200;
        case BaudRate::BR_38400:  return CBR_38400;
        case BaudRate::BR_57600:  return CBR_57600;
        case BaudRate::BR_115200: return CBR_115200;
        case BaudRate::BR_230400: return 230400; // Custom baud rate for Windows
        case BaudRate::BR_460800: return 460800; // Custom baud rate for Windows
        case BaudRate::BR_921600: return 921600; // Custom baud rate for Windows
        default: return CBR_9600; // Default to 9600
    }
#else // Linux
    switch (baudRate) {
        case BaudRate::BR_9600:   return B9600;
        case BaudRate::BR_19200:  return B19200;
        case BaudRate::BR_38400:  return B38400;
        case BaudRate::BR_57600:  return B57600;
        case BaudRate::BR_115200: return B115200;
        case BaudRate::BR_230400: return B230400;
        default: return B9600; // Default to 9600
    }
#endif
}

std::string Serial_Comms::applyTermination(const std::string& data, TerminationMethod termination) {
    std::string terminatedData = data;
    switch (termination) {
        case TerminationMethod::CR:
            terminatedData += '\r';
            break;
        case TerminationMethod::LF:
            terminatedData += '\n';
            break;
        case TerminationMethod::CRLF:
            terminatedData += "\r\n";
            break;
        case TerminationMethod::None:
            // No termination needed
            break;
    }
    return terminatedData;
}

#endif // SERIAL_COMMS_H
