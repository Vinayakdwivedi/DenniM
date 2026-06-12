#pragma once
#include <vector>
#include <iostream>
#include "Coordinate.h"
#include "MathUtils.h"
#include <cmath>


namespace cppviz {

/**
 * Rotates all points in a Coordinate set around their centroid.
 *
 * The centroid is computed as the arithmetic mean of all (x, y) points.
 * Each point is then rotated by @p angle_deg degrees counter-clockwise
 * using a standard 2-D rotation matrix:
 *
 *   xr = x·cos(θ) − y·sin(θ)
 *   yr = x·sin(θ) + y·cos(θ)
 *
 * @param data       Input coordinate set (unmodified).
 * @param angle_deg  Rotation angle in degrees (positive = CCW).
 * @return           New Coordinate set with rotated points.
 */
inline Coordinate Rotate(const Coordinate& data, double angle_deg) {
    Coordinate result;

    const double theta    = toRadians(angle_deg);
    const double cosTheta = std::cos(theta);
    const double sinTheta = std::sin(theta);

    // ── Compute centroid ────────────────────────────────────────────────────
    double center_x = 0.0;
    double center_y = 0.0;
    const int n = static_cast<int>(data.size());

    for (int i = 0; i < n; ++i) {
        center_x += data.x_value[i];
        center_y += data.y_value[i];
    }
    center_x /= n;
    center_y /= n;

    // ── Rotate each point around the centroid ────────────────────────────────
    result.x_value.reserve(n);
    result.y_value.reserve(n);

    for (int i = 0; i < n; ++i) {
        double x = data.x_value[i] - center_x;
        double y = data.y_value[i] - center_y;

        double xr = x * cosTheta - y * sinTheta;
        double yr = x * sinTheta + y * cosTheta;

        result.x_value.push_back(static_cast<float>(xr + center_x));
        result.y_value.push_back(static_cast<float>(yr + center_y));
    }

    return result;
}

/**
 * Translates all points in a Coordinate set by (dx, dy).
 *
 * @param data  Input coordinate set (unmodified).
 * @param dx    Horizontal offset.
 * @param dy    Vertical offset.
 * @return      New Coordinate set with translated points.
 */
inline Coordinate Translate(const Coordinate& data, float dx, float dy) {
    Coordinate result;
    const int n = static_cast<int>(data.size());
    result.x_value.reserve(n);
    result.y_value.reserve(n);

    for (int i = 0; i < n; ++i) {
        result.x_value.push_back(data.x_value[i] + dx);
        result.y_value.push_back(data.y_value[i] + dy);
    }
    return result;
}

/**
 * Scales all points in a Coordinate set relative to their centroid.
 *
 * @param data    Input coordinate set (unmodified).
 * @param factor  Uniform scale factor (>1 enlarges, <1 shrinks).
 * @return        New Coordinate set with scaled points.
 */
inline Coordinate Scale(const Coordinate& data, float factor) {
    Coordinate result;
    const int n = static_cast<int>(data.size());

    double center_x = 0.0, center_y = 0.0;
    for (int i = 0; i < n; ++i) {
        center_x += data.x_value[i];
        center_y += data.y_value[i];
    }
    center_x /= n;
    center_y /= n;

    result.x_value.reserve(n);
    result.y_value.reserve(n);
    for (int i = 0; i < n; ++i) {
        result.x_value.push_back(static_cast<float>(center_x + (data.x_value[i] - center_x) * factor));
        result.y_value.push_back(static_cast<float>(center_y + (data.y_value[i] - center_y) * factor));
    }
    return result;
}

} // namespace cppviz