#include "Serial_Comms.hpp"
#include "NMEAReader.hpp"
#include "NMEAParser.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    std::cout << "Starting NMEA Reader Example..." << std::endl;

    // 1. Initialize Serial_Comms
    Serial_Comms serialPort;
    std::string portName;

#ifdef _WIN32
    portName = "COM1"; // Change to your Windows COM port
#else
    portName = "/dev/ttyUSB0"; // Change to your Linux/macOS serial port
#endif

    if (!serialPort.open(portName))
    {
        std::cerr << "Error: Could not open serial port " << portName << std::endl;
        return 1;
    }

    // Configure serial port (e.g., 4800 baud, 8N1, no flow control for NMEA)
    if (!serialPort.configure(
            Serial_Comms::BaudRate::BR_4800, // NMEA standard baud rate
            Serial_Comms::DataBits::DB_8,
            Serial_Comms::Parity::None,
            Serial_Comms::StopBits::SB_1,
            Serial_Comms::FlowControl::None))
    {
        std::cerr << "Error: Could not configure serial port." << std::endl;
        serialPort.close();
        return 1;
    }

    std::cout << "Serial port " << portName << " opened and configured." << std::endl;

    // 2. Initialize NMEAReader with the Serial_Comms instance
    NMEAReader nmeaReader(serialPort, 500); // 500ms timeout for each read operation

    std::cout << "NMEA Reader initialized. Waiting for sentences..." << std::endl;

    // 3. Main loop to read and parse NMEA sentences
    while (true)
    {
        std::optional<std::shared_ptr<NMEAMessage>> message = nmeaReader.readAndParseSentence();

        if (message.has_value())
        {
            std::cout << "Parsed NMEA Message: " << message.value()->toString() << std::endl;
        }
        else
        {
            // No complete sentence found within the timeout, or no data available
            // std::cout << "No NMEA sentence available (timeout or no data)." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Prevent busy-waiting
        }
    }

    // This part will not be reached in the infinite loop, but good practice for cleanup
    serialPort.close();
    std::cout << "Serial port closed. Exiting." << std::endl;

    return 0;
}
