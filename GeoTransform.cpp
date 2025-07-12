#include "GeoTransform.hpp"
#include <cmath>

namespace {
    constexpr double deg2rad(double deg) {
        return deg * M_PI / 180.0;
    }

    constexpr double rad2deg(double rad) {
        return rad * 180.0 / M_PI;
    }
}

Cartesian toCartesian(const LLH& llh, const Ellipsoid& ellipsoid) {
    double a = ellipsoid.a;
    double f = ellipsoid.f;
    double e2 = 2 * f - f * f;

    double lat_rad = deg2rad(llh.lat);
    double lon_rad = deg2rad(llh.lon);

    double sinLat = std::sin(lat_rad);
    double cosLat = std::cos(lat_rad);
    double sinLon = std::sin(lon_rad);
    double cosLon = std::cos(lon_rad);

    double N = a / std::sqrt(1 - e2 * sinLat * sinLat);

    double x = (N + llh.h) * cosLat * cosLon;
    double y = (N + llh.h) * cosLat * sinLon;
    double z = (N * (1 - e2) + llh.h) * sinLat;

    return Cartesian{x, y, z};
}

LLH toLLH(const Cartesian& ecef, const Ellipsoid& ellipsoid) {
    double a = ellipsoid.a;
    double f = ellipsoid.f;
    double e2 = 2 * f - f * f;
    double x = ecef.x;
    double y = ecef.y;
    double z = ecef.z;

    double lon = std::atan2(y, x);
    double p = std::sqrt(x * x + y * y);
    double lat = std::atan2(z, p * (1 - f));
    double prevLat = 0.0;
    double h = 0.0;

    while (std::fabs(lat - prevLat) > 1e-12) {
        prevLat = lat;
        double sinLat = std::sin(lat);
        double N = a / std::sqrt(1 - e2 * sinLat * sinLat);
        h = p / std::cos(lat) - N;
        lat = std::atan2(z, p * (1 - e2 * N / (N + h)));
    }

    return LLH{rad2deg(lat), rad2deg(lon), h};
}

Cartesian applyHelmert(const Cartesian& pt, const Helmert& h) {
    double rx_rad = deg2rad(h.rx / 3600.0);
    double ry_rad = deg2rad(h.ry / 3600.0);
    double rz_rad = deg2rad(h.rz / 3600.0);
    double scale = 1 + h.s * 1e-6;

    double x = h.tx + scale * (pt.x - rz_rad * pt.y + ry_rad * pt.z);
    double y = h.ty + scale * (rz_rad * pt.x + pt.y - rx_rad * pt.z);
    double z = h.tz + scale * (-ry_rad * pt.x + rx_rad * pt.y + pt.z);

    return Cartesian{x, y, z};
}

Cartesian::Cartesian(const LLH& from, const LLH& to, const Ellipsoid& ellipsoid) {
    Cartesian c1 = toCartesian(from, ellipsoid);
    Cartesian c2 = toCartesian(to, ellipsoid);
    x = c2.x - c1.x;
    y = c2.y - c1.y;
    z = c2.z - c1.z;
}

double Cartesian::distanceTo(const Cartesian& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
