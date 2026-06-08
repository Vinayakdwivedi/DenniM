#pragma once
#include "../core/Coordinate.h"
#include <fstream>
#include <string>
#include <stdexcept>

namespace cppviz {

/**
 * Writes Coordinate data to SVG files on disk.
 *
 * Desktop-only renderer: uses std::ofstream to produce standalone .svg files
 * that can be opened in any browser or vector editor.
 *
 * Two rendering modes are provided:
 *   - renderShape()   – raw pixel coordinates (shapes live in pixel space).
 *   - renderSignal()  – maps (time, amplitude) → pixels using the signal
 *                       metadata stored in SignalDataResult.
 *
 * SVG canvas defaults: 800 × 401 px, black background, lime stroke.
 */
class SVGRenderer {
public:

    // ── Configuration ────────────────────────────────────────────────────────

    int         canvasWidth  = 800;
    int         canvasHeight = 401;
    std::string strokeColor  = "lime";
    int         strokeWidth  = 2;
    std::string fillColor    = "none";
    std::string background   = "black"; ///< set to "none" to skip background rect

    // ── Shape rendering ──────────────────────────────────────────────────────

    /**
     * Renders a Coordinate set whose values are already in pixel space.
     *
     * Produces an SVG <path> element using M (move-to) for the first point
     * and L (line-to) for every subsequent point.
     *
     * @param data      Coordinate set to render.
     * @param filename  Output .svg file path (e.g. "shape.svg").
     * @throws std::runtime_error if the file cannot be opened or data is empty.
     */
    void renderShape(const Coordinate& data, const std::string& filename) const {
        if (data.empty()) {
            throw std::runtime_error("SVGRenderer::renderShape — Coordinate set is empty.");
        }

        std::string path = buildPath(data);
        writeFile(filename, wrapSVG(path));
    }

    // ── Signal rendering ─────────────────────────────────────────────────────

    /**
     * Renders a SignalDataResult, mapping (time, amplitude) → pixel (x, y).
     *
     * Mapping:
     *   pixel_x = (t / totalTime)  × canvasWidth
     *   pixel_y = (canvasHeight/2) − amplitude × (canvasHeight/2)
     *
     * This places 0-amplitude at the vertical midpoint and ±1 amplitude at
     * the top/bottom edges of the canvas.
     *
     * @param data      Signal data returned by Signal::SineSignal() (or similar).
     * @param filename  Output .svg file path (e.g. "signal.svg").
     * @throws std::runtime_error if the file cannot be opened or data is empty.
     */
    void renderSignal(const SignalDataResult& data, const std::string& filename) const {
        if (data.coord.empty()) {
            throw std::runtime_error("SVGRenderer::renderSignal — SignalDataResult is empty.");
        }

        const float midY = static_cast<float>(canvasHeight) / 2.0f;

        auto toPixelX = [&](float t) -> float {
            return (t / data.time) * static_cast<float>(canvasWidth);
        };
        auto toPixelY = [&](float amp) -> float {
            return midY - amp * midY;
        };

        // Build the SVG path in pixel space
        Coordinate pixelCoord;
        const int n = static_cast<int>(data.coord.size());
        pixelCoord.x_value.reserve(n);
        pixelCoord.y_value.reserve(n);

        for (int i = 0; i < n; ++i) {
            pixelCoord.x_value.push_back(toPixelX(data.coord.x_value[i]));
            pixelCoord.y_value.push_back(toPixelY(data.coord.y_value[i]));
        }

        std::string path = buildPath(pixelCoord);
        writeFile(filename, wrapSVG(path));
    }

private:

    // ── Internal helpers ─────────────────────────────────────────────────────

    /// Converts a Coordinate set (already in pixel space) into an SVG path string.
    std::string buildPath(const Coordinate& data) const {
        std::string path;
        path.reserve(data.size() * 24); // rough pre-allocation

        path += "M " + fmt(data.x_value[0]) + " " + fmt(data.y_value[0]) + " ";
        for (std::size_t i = 1; i < data.size(); ++i) {
            path += "L " + fmt(data.x_value[i]) + " " + fmt(data.y_value[i]) + " ";
        }
        return path;
    }

    /// Wraps an SVG path string in a complete, valid SVG document.
    std::string wrapSVG(const std::string& path) const {
        std::string svg;
        svg += "<svg xmlns=\"http://www.w3.org/2000/svg\""
               " width=\""  + std::to_string(canvasWidth)  + "\""
               " height=\"" + std::to_string(canvasHeight) + "\">\n";

        // Optional background fill
        if (background != "none") {
            svg += "  <rect width=\"100%\" height=\"100%\" fill=\"" + background + "\"/>\n";
        }

        svg += "  <path d=\"" + path + "\""
               " stroke=\""       + strokeColor              + "\""
               " fill=\""         + fillColor                + "\""
               " stroke-width=\"" + std::to_string(strokeWidth) + "\"/>\n";
        svg += "</svg>\n";
        return svg;
    }

    /// Writes @p content to a file, throwing on failure.
    void writeFile(const std::string& filename, const std::string& content) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("SVGRenderer: could not open file \"" + filename + "\"");
        }
        file << content;
        file.close();
    }

    /// Converts float to string with 3 decimal places.
    static std::string fmt(float v) {
        // to_string gives 6 decimals; we trim to 3 for cleaner SVG output
        std::string s = std::to_string(v);
        auto dot = s.find('.');
        if (dot != std::string::npos && dot + 4 < s.size()) {
            s = s.substr(0, dot + 4);
        }
        return s;
    }
};

} // namespace cppviz