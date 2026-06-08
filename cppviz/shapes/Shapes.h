#pragma once
#include "../core/Coordinate.h"
#include "../core/MathUtils.h"
#include <cmath>
#include <vector>

namespace cppviz {

/**
 * Factory class that generates 2-D shape outlines as Coordinate sets.
 *
 * Every method returns a closed polyline (last point == first point)
 * so it renders as a filled-looking outline in SVG.
 */
class Shapes {
public:

    // ── Regular Polygon ──────────────────────────────────────────────────────

    /**
     * Generates a regular N-sided polygon.
     *
     * The first vertex is placed at the top (−π/2 offset) so the shape
     * always sits flat-side-down for even side counts.
     *
     * @param sides     Number of sides (≥ 3).
     * @param length    Side length in pixels.
     * @param center_x  X coordinate of the center (default: 400).
     * @param center_y  Y coordinate of the center (default: 200).
     * @return          Closed polyline with `sides + 1` points.
     */
    Coordinate RegularPolygon(int sides, int length,
                               int center_x = 400, int center_y = 200) const
    {
        Coordinate points;
        // Circumscribed-circle radius derived from side length
        double r = length / (2.0 * std::sin(PI / sides));

        points.x_value.reserve(sides + 1);
        points.y_value.reserve(sides + 1);

        for (int i = 0; i <= sides; ++i) {
            double angle = (2.0 * PI * i) / sides - PI / 2.0;
            points.x_value.push_back(static_cast<float>(center_x + r * std::cos(angle)));
            points.y_value.push_back(static_cast<float>(center_y + r * std::sin(angle)));
        }
        return points;
    }

    // ── Rectangle ────────────────────────────────────────────────────────────

    /**
     * Generates an axis-aligned rectangle.
     *
     * @param width        Width in pixels.
     * @param height       Height in pixels.
     * @param margin_left  Left edge X position (default: 1).
     * @param margin_top   Top edge Y position  (default: 1).
     * @return             Closed polyline with 5 points.
     */
    Coordinate Rectangle(int width, int height,
                          int margin_left = 1, int margin_top = 1) const
    {
        Coordinate points;
        points.x_value.reserve(5);
        points.y_value.reserve(5);

        // top-left → top-right → bottom-right → bottom-left → top-left
        const int r = margin_left + width;
        const int b = margin_top  + height;

        points.x_value.push_back(margin_left);  points.y_value.push_back(margin_top);
        points.x_value.push_back(r);             points.y_value.push_back(margin_top);
        points.x_value.push_back(r);             points.y_value.push_back(b);
        points.x_value.push_back(margin_left);  points.y_value.push_back(b);
        points.x_value.push_back(margin_left);  points.y_value.push_back(margin_top); // close
        return points;
    }

    // ── Square ───────────────────────────────────────────────────────────────

    /**
     * Generates an axis-aligned square (convenience wrapper over Rectangle).
     *
     * @param side         Side length in pixels.
     * @param margin_left  Left edge X position (default: 1).
     * @param margin_top   Top edge Y position  (default: 1).
     * @return             Closed polyline with 5 points.
     */
    Coordinate Square(int side,
                       int margin_left = 1, int margin_top = 1) const
    {
        return Rectangle(side, side, margin_left, margin_top);
    }

    // ── Right Triangle ───────────────────────────────────────────────────────

    /**
     * Generates a right triangle.
     *
     * The right angle is at (margin_left, perpendicular).
     * Layout:
     *   (margin_left, margin_top)
     *     │  (hypotenuse)
     *   (margin_left, perpendicular) ── (base, perpendicular)
     *
     * @param perpendicular  Y coordinate of the right-angle corner.
     * @param base           X coordinate of the far corner.
     * @param margin_left    X coordinate of the top/left corners (default: 1).
     * @param margin_top     Y coordinate of the apex (default: 1).
     * @return               Closed polyline with 4 points.
     */
    Coordinate RightTriangle(int perpendicular, int base,
                              int margin_left = 1, int margin_top = 1) const
    {
        Coordinate points;
        points.x_value.reserve(4);
        points.y_value.reserve(4);

        points.x_value.push_back(margin_left);  points.y_value.push_back(margin_top);
        points.x_value.push_back(margin_left);  points.y_value.push_back(perpendicular);
        points.x_value.push_back(base);          points.y_value.push_back(perpendicular);
        points.x_value.push_back(margin_left);  points.y_value.push_back(margin_top); // close
        return points;
    }
};

} // namespace cppviz