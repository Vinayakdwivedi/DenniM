#include "../core/Viewport.h"
#include "../shapes/Shapes.h"
#include "../shapes/Signals.h"
#include "../core/Transform.h"

int main() {
    cppviz::Viewport vp(800, 900);
    cppviz::Shapes shape;

    // ── Face ──────────────────────────────────────────────────
    // Big circle for the head (centered around 400, 500)
    vp.scene.add(shape.Circle(180, 400, 500));

    // ── Eyes ──────────────────────────────────────────────────
    vp.scene.add(shape.Circle(25, 330, 450));   // left eye
    vp.scene.add(shape.Circle(25, 470, 450));   // right eye

    // ── Smile (Rectangle + two small squares as corners) ──────
    // Central smile bar
    vp.scene.add(shape.Rectangle(120, 18, 340, 560));

    // Left smile corner square (rotated look via position)
    cppviz::Coordinate leftCorner = shape.Square(18, 0, 0);
    vp.scene.add(cppviz::Translate(leftCorner, 322, 545));  // bottom-left

    // Right smile corner square
    cppviz::Coordinate rightCorner = shape.Square(18, 0, 0);
    vp.scene.add(cppviz::Translate(rightCorner, 460, 545)); // bottom-right

    // ── Hat brim (wide rectangle) ─────────────────────────────
    vp.scene.add(shape.Rectangle(300, 22, 250, 335));

    // ── Hat top (tall rectangle) ──────────────────────────────
    vp.scene.add(shape.Rectangle(180, 120, 310, 215));

    // ── Hat flaps — two RightTriangles on each side ───────────
    // Left flap: triangle at origin, then translate to brim left-end
    cppviz::Coordinate leftFlap = shape.RightTriangle(50, -70);
    vp.scene.add(cppviz::Translate(leftFlap, 310, 283));

    // Right flap: mirrored position on the right
    cppviz::Coordinate rightFlap = shape.RightTriangle(50, 70);
    vp.scene.add(cppviz::Translate(rightFlap, 490, 283));

    // ── Nose (small square) ───────────────────────────────────
    cppviz::Coordinate nose = shape.Square(20, 0, 0);
    vp.scene.add(cppviz::Translate(nose, 390, 505));

    vp.zoomIn(1.0f);
    vp.panBy(0, 0);
    vp.saveToFile("smiley.svg");

    return 0;
}