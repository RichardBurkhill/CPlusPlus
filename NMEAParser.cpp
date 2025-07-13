// See full explanation and documentation in the associated header.
#include "NMEAParser.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <cctype>

bool NMEAParser::validateChecksum(const std::string& sentence) {
    if (sentence.empty() || sentence[0] != '$') return false;
    size_t asterisk = sentence.find('*');
    if (asterisk == std::string::npos || asterisk + 2 >= sentence.size()) return false;

    unsigned char checksum = 0;
    for (size_t i = 1; i < asterisk; ++i) {
        checksum ^= static_cast<unsigned char>(sentence[i]);
    }

    std::string hex = sentence.substr(asterisk + 1, 2);
    unsigned int expected;
    std::istringstream(hex) >> std::hex >> expected;

    return checksum == static_cast<unsigned char>(expected);
}

std::vector<std::string> NMEAParser::tokenize(const std::string& sentence) {
    size_t start = sentence.find('$') + 1;
    size_t end = sentence.find('*');
    std::string body = sentence.substr(start, end - start);

    std::vector<std::string> fields;
    std::stringstream ss(body);
    std::string item;
    while (std::getline(ss, item, ',')) {
        fields.push_back(item);
    }
    return fields;
}

double NMEAParser::parseLatitude(const std::string& value, char direction) {
    if (value.empty()) return 0.0;
    double deg = std::stod(value.substr(0, 2));
    double min = std::stod(value.substr(2));
    double lat = deg + (min / 60.0);
    return (direction == 'S') ? -lat : lat;
}

double NMEAParser::parseLongitude(const std::string& value, char direction) {
    if (value.empty()) return 0.0;
    double deg = std::stod(value.substr(0, 3));
    double min = std::stod(value.substr(3));
    double lon = deg + (min / 60.0);
    return (direction == 'W') ? -lon : lon;
}

std::optional<std::shared_ptr<NMEAMessage>> NMEAParser::parseSentence(const std::string& sentence) {
    if (!validateChecksum(sentence)) return std::nullopt;

    auto fields = tokenize(sentence);
    if (fields.empty()) return std::nullopt;

    const std::string& type = fields[0];
    try {
        if (type == "GPRMC" && fields.size() >= 10) {
            auto msg = std::make_shared<GPRMC>();
            msg->utcTime = fields[1];
            msg->status = fields[2][0];
            msg->latitude = parseLatitude(fields[3], fields[4][0]);
            msg->longitude = parseLongitude(fields[5], fields[6][0]);
            msg->speed = std::stod(fields[7]);
            msg->course = std::stod(fields[8]);
            msg->date = fields[9];
            return msg;
        } else if (type == "GPGGA" && fields.size() >= 10) {
            auto msg = std::make_shared<GPGGA>();
            msg->utcTime = fields[1];
            msg->latitude = parseLatitude(fields[2], fields[3][0]);
            msg->longitude = parseLongitude(fields[4], fields[5][0]);
            msg->fixQuality = std::stoi(fields[6]);
            msg->numSatellites = std::stoi(fields[7]);
            msg->hdop = std::stod(fields[8]);
            msg->altitude = std::stod(fields[9]);
            msg->altitudeUnits = fields[10][0];
            return msg;
        } else if (type == "GPGLL" && fields.size() >= 6) {
            auto msg = std::make_shared<GPGLL>();
            msg->latitude = parseLatitude(fields[1], fields[2][0]);
            msg->longitude = parseLongitude(fields[3], fields[4][0]);
            msg->utcTime = fields[5];
            msg->status = fields[6][0];
            return msg;
        } else if (type == "GPVTG" && fields.size() >= 8) {
            auto msg = std::make_shared<GPVTG>();
            msg->courseTrue = std::stod(fields[1]);
            msg->courseTrueUnit = fields[2][0];
            msg->speedKnots = std::stod(fields[5]);
            msg->speedKnotsUnit = fields[6][0];
            msg->speedKmh = std::stod(fields[7]);
            msg->speedKmhUnit = fields[8][0];
            return msg;
        } else if (type == "GPGSA" && fields.size() >= 17) {
            auto msg = std::make_shared<GPGSA>();
            msg->mode = fields[1][0];
            msg->fixType = std::stoi(fields[2]);
            for (int i = 3; i <= 14; ++i) {
                if (!fields[i].empty()) msg->satellitesUsed.push_back(std::stoi(fields[i]));
            }
            msg->pdop = std::stod(fields[15]);
            msg->hdop = std::stod(fields[16]);
            msg->vdop = std::stod(fields[17]);
            return msg;
        } else if (type == "GPGSV" && fields.size() >= 4) {
            auto msg = std::make_shared<GPGSV>();
            msg->totalMessages = std::stoi(fields[1]);
            msg->messageNumber = std::stoi(fields[2]);
            msg->satellitesInView = std::stoi(fields[3]);
            return msg;
        }
    } catch (...) {
        return std::nullopt;
    }

    return std::nullopt;
}
