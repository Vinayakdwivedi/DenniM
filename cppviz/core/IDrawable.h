#pragma once
#include <string>

namespace cppviz {

class BoundingBox;

class IDrawable {
public:
    virtual ~IDrawable() = default;
    // Returns SVG element string (no <svg> wrapper)
    // transform is a pre-multiplied matrix string from the viewport
    virtual std::string toSVGElement(const std::string& extraTransform = "") const = 0;
    virtual BoundingBox getBounds() const = 0;
};

} // namespace cppviz