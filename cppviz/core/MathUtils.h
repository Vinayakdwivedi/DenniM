#pragma once
#include <cmath>

namespace cppviz {

/// High-precision π constant used throughout the library.
constexpr double PI = 3.14159265358979323846;

/**
 * Converts degrees to radians.
 * @param deg  Angle in degrees.
 * @return     Equivalent angle in radians.
 */
inline double toRadians(double deg) {
    return deg * PI / 180.0;
}

/**
 * Converts radians to degrees.
 * @param rad  Angle in radians.
 * @return     Equivalent angle in degrees.
 */
inline double toDegrees(double rad) {
    return rad * 180.0 / PI;
}

} // namespace cppviz