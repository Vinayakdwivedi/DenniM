#pragma once
#include "math/vector2.h"

namespace cppviz {

struct Camera {
    Vector2 pan   = {0.0f, 0.0f};  // world-space offset
    float   zoom  = 1.0f;           // scale factor

    void panBy(float dx, float dy) { pan.x += dx; pan.y += dy; }
    void zoomBy(float factor, Vector2 pivot = {0,0}) {
        // zoom toward a pivot point
        pan.x = pivot.x + (pan.x - pivot.x) * factor;
        pan.y = pivot.y + (pan.y - pivot.y) * factor;
        zoom *= factor;
    }
    void reset() { pan = {0,0}; zoom = 1.0f; }

    // Returns SVG transform string: "translate(px, py) scale(z)"
    std::string toSVGTransform() const {
        return "translate(" + std::to_string(pan.x) + "," +
               std::to_string(pan.y) + ") scale(" +
               std::to_string(zoom) + ")";
    }
};

} // namespace cppviz