/**
 * @file GeoTransform.hpp
 * @brief Provides geodetic data structures and transformations using the Helmert 7-parameter algorithm.
 * @details Includes definitions for common datums and ellipsoids and helpers to convert between LLH and Cartesian coordinates.
 *
 * ## Example Usage
 * 
 * ```cpp
 * LLH p1(51.4778, -0.0014, 45.0);  // Greenwich
 * LLH p2(48.8584, 2.2945, 35.0);   // Paris
 * Cartesian c1 = toCartesian(p1, Datums::WGS84Datum.ellipsoid);
 * Cartesian c2 = toCartesian(p2, Datums::WGS84Datum.ellipsoid);
 * Cartesian delta = c2 - c1;
 * double dist = delta.distanceTo(Cartesian(0,0,0));
 * ```
 *
 * Use `toCartesian` to convert LLH points to ECEF Cartesian coordinates, then subtract to get X/Y/Z delta.
 * Use `applyHelmert` to convert between datums (e.g. OSGB36 to WGS84).
 */

#ifndef GEOTRANSFORM_HPP
#define GEOTRANSFORM_HPP

#include <cmath>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <ostream>

/// @brief Structure representing an ellipsoid with semi-major axis and flattening
struct Ellipsoid {
    double a;   ///< Semi-major axis (meters)
    double f;   ///< Flattening

    constexpr Ellipsoid(double semiMajor, double flattening)
        : a(semiMajor), f(flattening) {}
};

/// @brief WGS84 ellipsoid parameters
constexpr Ellipsoid WGS84{6378137.0, 1.0 / 298.257223563};

/// @brief Other standard ellipsoids
namespace Ellipsoids {
    constexpr Ellipsoid GRS80{6378137.0, 1.0 / 298.257222101};
    constexpr Ellipsoid Airy1830{6377563.396, 1.0 / 299.3249646};
    constexpr Ellipsoid Intl1924{6378388.0, 1.0 / 297.0};
    constexpr Ellipsoid Bessel1841{6377397.155, 1.0 / 299.1528128};
    constexpr Ellipsoid Clarke1866{6378206.4, 1.0 / 294.9786982};
}

/// @brief Helmert 7-parameter transformation structure
struct Helmert {
    double tx;   ///< Translation in meters
    double ty;
    double tz;
    double rx;   ///< Rotation in arc-seconds
    double ry;
    double rz;
    double s;    ///< Scale in parts per million

    constexpr Helmert(double t_x, double t_y, double t_z,
                      double r_x, double r_y, double r_z,
                      double scale)
        : tx(t_x), ty(t_y), tz(t_z), rx(r_x), ry(r_y), rz(r_z), s(scale) {}
};

/// @brief Datum definition containing an ellipsoid and Helmert transform to WGS84
struct Datum {
    Ellipsoid ellipsoid;
    Helmert toWGS84;

    constexpr Datum(Ellipsoid e, Helmert h) : ellipsoid(e), toWGS84(h) {}
};

/// @brief Well-known datums and transformation parameters
namespace Datums {
    constexpr Datum OSGB36{
        Ellipsoids::Airy1830,
        Helmert{375.0, -111.0, 431.0, 0.0, 0.0, 0.0, -100.0}
    };

    constexpr Datum NAD27{
        Ellipsoids::Clarke1866,
        Helmert{-8.0, 160.0, 176.0, 0.0, 0.0, 0.0, 0.0}
    };

    constexpr Datum NAD83{
        Ellipsoids::GRS80,
        Helmert{1.004, -1.910, -0.515, 0.0267, 0.00034, 0.011, 0.0}
    };

    constexpr Datum ED50{
        Ellipsoids::Intl1924,
        Helmert{89.5, 93.8, 123.1, 0.0, 0.0, 0.156, -1.2}
    };

    constexpr Datum Tokyo{
        Ellipsoids::Bessel1841,
        Helmert{-148.0, 507.0, 685.0, 0.0, 0.0, 0.0, 0.0}
    };

    constexpr Datum WGS84Datum{WGS84, Helmert{0, 0, 0, 0, 0, 0, 0}};
}

/// @brief Geodetic coordinate in latitude, longitude (degrees), and height (meters)
struct LLH {
    double lat; ///< Latitude in degrees
    double lon; ///< Longitude in degrees
    double h;   ///< Height in meters

    LLH(double latitude, double longitude, double height)
        : lat(latitude), lon(longitude), h(height) {}
};

/// @brief Cartesian XYZ coordinate in meters
struct Cartesian {
    double x;
    double y;
    double z;

    Cartesian(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    /// @brief Constructs a delta vector from two LLH points using an ellipsoid
    Cartesian(const LLH& from, const LLH& to, const Ellipsoid& ellipsoid);

    /// @brief Vector subtraction
    Cartesian operator-(const Cartesian& other) const {
        return Cartesian{x - other.x, y - other.y, z - other.z};
    }

    /// @brief Vector addition
    Cartesian operator+(const Cartesian& other) const {
        return Cartesian{x + other.x, y + other.y, z + other.z};
    }

    /// @brief Euclidean distance to another Cartesian point
    double distanceTo(const Cartesian& other) const;
};

/// @brief Convert from geodetic LLH to Cartesian XYZ using the given ellipsoid
Cartesian toCartesian(const LLH& llh, const Ellipsoid& ellipsoid);

/// @brief Convert from Cartesian XYZ to geodetic LLH using the given ellipsoid
LLH toLLH(const Cartesian& ecef, const Ellipsoid& ellipsoid);

/// @brief Apply Helmert transform to Cartesian coordinates
Cartesian applyHelmert(const Cartesian& pt, const Helmert& h);

#endif // GEOTRANSFORM_HPP