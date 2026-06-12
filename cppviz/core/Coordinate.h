#pragma once
#include <vector>

namespace cppviz {

/**
 * Holds a 2D polyline / point-cloud as parallel x/y float arrays.
 * Both vectors must always be the same length.
 */
struct Coordinate {
    std::vector<float> x_value;
    std::vector<float> y_value;


    /// Returns the number of points stored.
    std::size_t size() const { return x_value.size(); }

    /// Returns true when there are no points.
    bool empty() const { return x_value.empty(); }
};

/**
 * Metadata bundle returned by Signal generators.
 * Carries the raw coordinate data together with the signal parameters
 * needed by the renderer (time range, sample count, frequency).
 */
struct SignalDataResult {
    Coordinate coord;
    float      time   = 1.0f;
    int        sample = 0;
    float      freq   = 1.0f;
};

} // namespace cppviz