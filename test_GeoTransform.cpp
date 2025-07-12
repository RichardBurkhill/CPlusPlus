#include "GeoTransform.hpp"
#include <gtest/gtest.h>
#include <cmath>

constexpr double EPSILON = 1e-3;  // millimeter precision

TEST(GeoTransformTests, ToCartesianAndBack_WGS84) {
    LLH original(51.4778, -0.0014, 45.0); // Greenwich Observatory
    Cartesian xyz = toCartesian(original, WGS84);
    LLH result = toLLH(xyz, WGS84);

    EXPECT_NEAR(original.lat, result.lat, 1e-6);
    EXPECT_NEAR(original.lon, result.lon, 1e-6);
    EXPECT_NEAR(original.h, result.h, EPSILON);
}

TEST(GeoTransformTests, HelmertTransformationToWGS84) {
    LLH osgb36_point(51.4778, -0.0014, 45.0);
    Cartesian cart_osgb36 = toCartesian(osgb36_point, Datums::OSGB36.ellipsoid);

    Cartesian cart_wgs84 = applyHelmert(cart_osgb36, Datums::OSGB36.toWGS84);
    LLH result = toLLH(cart_wgs84, WGS84);

    // Expect result to be close to known WGS84 for Greenwich
    EXPECT_NEAR(result.lat, 51.4780, 1e-4);
    EXPECT_NEAR(result.lon, -0.0014, 1e-4);
    //Test failure, need to use surveyed points in both OSGB36 and WGS84
}

TEST(GeoTransformTests, DistanceBetweenTwoPoints) {
    LLH pt1(51.0, 0.0, 0.0);
    LLH pt2(52.0, 0.0, 0.0);

    Cartesian c1 = toCartesian(pt1, WGS84);
    Cartesian c2 = toCartesian(pt2, WGS84);

    double dist = c1.distanceTo(c2);

    EXPECT_NEAR(dist, 111195.0, 100.0); // Approx distance for 1 deg latitude in meters
}

TEST(GeoTransformTests, CartesianDeltaFromLLH) {
    LLH pt1(50.0, -1.0, 100.0);
    LLH pt2(50.0001, -1.0, 100.0);

    Cartesian delta(pt1, pt2, WGS84);

    double magnitude = std::sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

    EXPECT_NEAR(magnitude, 11.1, 0.1);  // Allow for small rounding errors
}

