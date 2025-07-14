#include "NetworkComms.hpp"
#include "NMEAReader.hpp"
#include "NMEAParser.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <optional> // For std::optional

int main() {
    std::cout << "Starting NMEA Reader Example (Network Comms)..." << std::endl;

    // 1. Initialize NetworkComms
    NetworkComms networkComms;
    const std::string AIS_HUB_HOST = "data.aishub.net";
    const std::string AIS_HUB_PORT = "80"; // Standard HTTP port, often used for raw TCP streams

    if (!networkComms.connect(AIS_HUB_HOST, AIS_HUB_PORT, NetworkComms::Protocol::TCP)) {
        std::cerr << "Error: Could not connect to " << AIS_HUB_HOST << ":" << AIS_HUB_PORT << std::endl;
        return 1;
    }

    std::cout << "Connected to " << AIS_HUB_HOST << ":" << AIS_HUB_PORT << std::endl;

    // 2. Initialize NMEAReader with the NetworkComms instance (which is an IComms)
    NMEAReader nmeaReader(networkComms, 500); // 500ms timeout for each read operation

    std::cout << "NMEA Reader initialized. Waiting for sentences..." << std::endl;

    // 3. Main loop to read and parse NMEA sentences
    while (true) {
        std::optional<std::shared_ptr<NMEAMessage>> message = nmeaReader.readAndParseSentence();

        if (message.has_value()) {
            std::cout << "Parsed NMEA Message: " << message.value()->toString() << std::endl;
        } else {
            // No complete sentence found within the timeout, or no data available
            // std::cout << "No NMEA sentence available (timeout or no data)." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Prevent busy-waiting
        }
    }

    // This part will not be reached in the infinite loop, but good practice for cleanup
    networkComms.close();
    std::cout << "Network connection closed. Exiting." << std::endl;

    return 0;
}
