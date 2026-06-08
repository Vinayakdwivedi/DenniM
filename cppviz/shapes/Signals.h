#pragma once
#include "../core/Coordinate.h"
#include "../core/MathUtils.h"
#include <cmath>
#include <vector>

namespace cppviz {

/**
 * Factory class that generates signal waveforms as SignalDataResult objects.
 *
 * The SignalDataResult carries both the raw (time, amplitude) coordinate
 * data and the metadata required by SignalRenderer to map values to pixels.
 */
class Signal {
public:

    /**
     * Generates a sampled sine wave: y(t) = sin(2π·freq·t).
     *
     * Time runs from 0 to @p time (exclusive of the last sample).
     * The step size is time / sample, so exactly @p sample points
     * are produced starting at t = 0.
     *
     * @param time    Total duration in seconds.
     * @param sample  Number of samples.
     * @param freq    Frequency in Hz.
     * @return        SignalDataResult with x = time axis, y = amplitude.
     */
    SignalDataResult SineSignal(float time, int sample, float freq) const {
        SignalDataResult result;
        result.time   = time;
        result.sample = sample;
        result.freq   = freq;

        result.coord.x_value.reserve(sample);
        result.coord.y_value.reserve(sample);

        const float step_size = time / static_cast<float>(sample);
        float t = 0.0f;

        for (int i = 0; i < sample; ++i) {
            // Use the high-precision PI constant from MathUtils
            float y_point = std::sin(2.0f * static_cast<float>(PI) * freq * t);
            result.coord.x_value.push_back(t);
            result.coord.y_value.push_back(y_point);
            t += step_size;
        }

        return result;
    }
};

} // namespace cppviz