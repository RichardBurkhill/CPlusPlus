/**
 * @file NMEAParser.hpp
 * @brief NMEA 0183 Parser for multiple GPS sentence types.
 * @details Supports $GPRMC, $GPGGA, $GPGLL, $GPVTG, $GPGSA, and $GPGSV with automatic detection.
 */

#ifndef NMEA_PARSER_HPP
#define NMEA_PARSER_HPP

#include <string>
#include <memory>
#include <vector>
#include <optional>

/// Base class for all NMEA messages
class NMEAMessage {
public:
    virtual ~NMEAMessage() = default;
    virtual std::string type() const = 0;
};

/// RMC - Recommended Minimum Navigation Information
struct GPRMC : public NMEAMessage {
    std::string utcTime;
    char status;
    double latitude;
    char latDir;
    double longitude;
    char lonDir;
    double speed;
    double course;
    std::string date;

    std::string type() const override { return "GPRMC"; }
};

/// GGA - Global Positioning System Fix Data
struct GPGGA : public NMEAMessage {
    std::string utcTime;
    double latitude;
    char latDir;
    double longitude;
    char lonDir;
    int fixQuality;
    int numSatellites;
    double hdop;
    double altitude;
    char altitudeUnits;

    std::string type() const override { return "GPGGA"; }
};

/// GLL - Geographic Position - Latitude/Longitude
struct GPGLL : public NMEAMessage {
    double latitude;
    char latDir;
    double longitude;
    char lonDir;
    std::string utcTime;
    char status;

    std::string type() const override { return "GPGLL"; }
};

/// VTG - Course Over Ground and Ground Speed
struct GPVTG : public NMEAMessage {
    double courseTrue;
    char courseTrueUnit;
    double speedKnots;
    char speedKnotsUnit;
    double speedKmh;
    char speedKmhUnit;

    std::string type() const override { return "GPVTG"; }
};

/// GSA - GPS DOP and Active Satellites
struct GPGSA : public NMEAMessage {
    char mode;
    int fixType;
    std::vector<int> satellitesUsed;
    double pdop;
    double hdop;
    double vdop;

    std::string type() const override { return "GPGSA"; }
};

/// GSV - Satellites in View
struct GPGSV : public NMEAMessage {
    int totalMessages;
    int messageNumber;
    int satellitesInView;

    std::string type() const override { return "GPGSV"; }
};

/// Parser class for NMEA sentences
class NMEAParser {
public:
    static std::optional<std::shared_ptr<NMEAMessage>> parseSentence(const std::string& sentence);

private:
    static bool validateChecksum(const std::string& sentence);
    static std::vector<std::string> tokenize(const std::string& sentence);
    static double parseLatitude(const std::string& value, char direction);
    static double parseLongitude(const std::string& value, char direction);
};

#endif // NMEA_PARSER_HPP
