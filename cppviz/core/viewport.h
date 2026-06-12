#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "math/vector2.h"
#include "camera.h"
#include "./scene.h"

namespace cppviz {

class Viewport {
public:
    unsigned int width, height;
    Camera       camera;
    Scene        scene;

    Viewport(unsigned int w, unsigned int h) : width(w), height(h) {}

    // --- View controls ---
    void panBy(float dx, float dy)            { camera.panBy(dx, dy); }
    void zoomIn (float factor = 1.2f)         { camera.zoomBy(factor); }
    void zoomOut(float factor = 1.2f)         { camera.zoomBy(1.0f / factor); }
    void zoomAt (float factor, Vector2 pivot) { camera.zoomBy(factor, pivot); }
    void resetView()                          { camera.reset(); }

    // --- Render ---
    std::string render() const {
        std::ostringstream svg;
        const std::string transform = camera.toSVGTransform();

        svg << "<svg xmlns=\"http://www.w3.org/2000/svg\""
            << " width=\""   << width  << "\""
            << " height=\""  << height << "\""
            << " viewBox=\"0 0 " << width << " " << height << "\""
            << " style=\"background:#1e1e2e\">\n";

        svg << "  <g transform=\"" << transform << "\">\n";

        for (const auto& obj : scene.coordinates()) {
            svg << "    " << toSVGElement(obj, transform) << "\n";
        }

        for (const auto& obj : scene.signals()) {
            svg << "    " << toSVGElement(obj, transform) << "\n";
        }

        svg << "  </g>\n</svg>";
        return svg.str();
    }

    void saveToFile(const std::string& path) const {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Viewport: could not open file \"" + path + "\"");
        }
        file << render();
    }

private:
    static std::string fmt(float v) {
        std::string s = std::to_string(v);
        auto dot = s.find('.');
        if (dot != std::string::npos && dot + 4 < s.size()) {
            s = s.substr(0, dot + 4);
        }
        return s;
    }

    std::string buildPath(const Coordinate& data) const {
        if (data.empty()) {
            throw std::runtime_error("Viewport: Coordinate set is empty.");
        }

        std::string path;
        path.reserve(data.size() * 24);
        path += "M " + fmt(data.x_value[0]) + " " + fmt(data.y_value[0]) + " ";
        for (std::size_t i = 1; i < data.size(); ++i) {
            path += "L " + fmt(data.x_value[i]) + " " + fmt(data.y_value[i]) + " ";
        }
        return path;
    }

    std::string buildSignalPath(const SignalDataResult& data) const {
        if (data.coord.empty()) {
            throw std::runtime_error("Viewport: SignalDataResult is empty.");
        }

        std::string path;
        path.reserve(data.coord.size() * 24);

        const float canvasWidth  = static_cast<float>(width);
        const float canvasHeight = static_cast<float>(height);
        const float midY         = canvasHeight / 2.0f;

        auto toPixelX = [&](float t) -> float {
            return data.time == 0.0f ? 0.0f : (t / data.time) * canvasWidth;
        };
        auto toPixelY = [&](float amp) -> float {
            return midY - amp * midY;
        };

        path += "M " + fmt(toPixelX(data.coord.x_value[0])) + " " + fmt(toPixelY(data.coord.y_value[0])) + " ";
        for (std::size_t i = 1; i < data.coord.size(); ++i) {
            path += "L " + fmt(toPixelX(data.coord.x_value[i])) + " " + fmt(toPixelY(data.coord.y_value[i])) + " ";
        }
        return path;
    }

    std::string toSVGElement(const Coordinate& data, const std::string& extraTransform) const {
        return "<path d=\"" + buildPath(data) + "\""
               " transform=\"" + extraTransform + "\""
               " stroke=\"lime\" fill=\"none\" stroke-width=\"2\"/>";
    }

    std::string toSVGElement(const SignalDataResult& data, const std::string& extraTransform) const {
        return "<path d=\"" + buildSignalPath(data) + "\""
               " transform=\"" + extraTransform + "\""
               " stroke=\"lime\" fill=\"none\" stroke-width=\"2\"/>";
    }
};

} // namespace cppviz