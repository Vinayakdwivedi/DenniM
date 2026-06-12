// /**
//  * desktop_sine.cpp
//  * ─────────────────────────────────────────────────────────────────────────────
//  * Example program demonstrating the full cppviz library API.
//  * Reproduces every shape and signal from the original monolithic main().
//  *
//  * Build (from project root):
//  *   g++ -std=c++17 -I. cppviz/examples/desktop_sine.cpp -o desktop_sine
//  *
//  * Output files (written to the current working directory):
//  *   triangle.svg  rotated_triangle.svg  signal.svg  rotated_signal.svg
//  *   square.svg    rectangle.svg         pentagon.svg  nonagon_rotated.svg
//  * ─────────────────────────────────────────────────────────────────────────────
//  */

// #include "../core/Coordinate.h"
// #include "../core/MathUtils.h"
// #include "../core/Transform.h"
// #include "../shapes/Shapes.h"
// #include "../shapes/Signals.h"
// #include "../renderers/SVGRenderer.h"

// #include <iostream>

// int main() {
//     using namespace cppviz;

//     Shapes      shape;
//     Signal      signal;
//     SVGRenderer renderer;       // default: 800×401, lime on black

//     // ── 1. Right triangle (unrotated) ────────────────────────────────────────
//     {
//         Coordinate triangle = shape.RightTriangle(200, 400);
//         renderer.renderShape(triangle, "triangle.svg");
//         std::cout << "[OK] triangle.svg\n";
//     }

//     // ── 2. Right triangle rotated 2° ─────────────────────────────────────────
//     {
//         Coordinate triangle = shape.RightTriangle(200, 400);
//         Coordinate rotated  = Rotate(triangle, 2.0);
//         renderer.renderShape(rotated, "rotated_triangle.svg");
//         std::cout << "[OK] rotated_triangle.svg\n";
//     }

//     // ── 3. Sine signal (2 s, 200 samples, 3 Hz) ──────────────────────────────
//     {
//         SignalDataResult sig = signal.SineSignal(2.0f, 200, 3.0f);
//         renderer.renderSignal(sig, "signal.svg");
//         std::cout << "[OK] signal.svg\n";
//     }

//     // ── 4. Sine signal rotated 7° in coordinate space ────────────────────────
//     {
//         SignalDataResult sig     = signal.SineSignal(2.0f, 200, 3.0f);
//         SignalDataResult rotated = sig;
//         rotated.coord           = Rotate(sig.coord, 7.0);
//         renderer.renderSignal(rotated, "rotated_signal.svg");
//         std::cout << "[OK] rotated_signal.svg\n";
//     }

//     // ── 5. Square (side=100, offset 50,50) ───────────────────────────────────
//     {
//         Coordinate sq = shape.Square(100, 50, 50);
//         renderer.renderShape(sq, "square.svg");
//         std::cout << "[OK] square.svg\n";
//     }

//     // ── 6. Rectangle (200×100, offset 50,50) ─────────────────────────────────
//     {
//         Coordinate rect = shape.Rectangle(200, 100, 50, 50);
//         renderer.renderShape(rect, "rectangle.svg");
//         std::cout << "[OK] rectangle.svg\n";
//     }

//     // ── 7. Regular pentagon (side=100, centered at 400,200) ──────────────────
//     {
//         Coordinate pentagon = shape.RegularPolygon(5, 100);
//         renderer.renderShape(pentagon, "pentagon.svg");
//         std::cout << "[OK] pentagon.svg\n";
//     }

//     // ── 8. Regular 9-gon rotated 40° ─────────────────────────────────────────
//     {
//         Coordinate nonagon = shape.RegularPolygon(9, 80);
//         Coordinate rotated = Rotate(nonagon, 40.0);
//         renderer.renderShape(rotated, "nonagon_rotated.svg");
//         std::cout << "[OK] nonagon_rotated.svg\n";
//     }
    

//     std::cout << "\nAll SVG files written successfully.\n";
//     return 0;
// }

#include "../core/Viewport.h"
#include "../shapes/Shapes.h"
#include "../shapes/Signals.h"
#include "../core/Transform.h"

int main() {
    cppviz::Viewport vp(1580, 1000);

    // Add shapes
    cppviz::Shapes shape;
    vp.scene.add(shape.Square(50, 175, 175));
    vp.scene.add(shape.Rectangle(120, 60, 100, 300));
        vp.scene.add(shape.RegularPolygon(5, 80));
        vp.scene.add(shape.RegularPolygon(9, 80, 600, 400));
    
        // Add a rotated triangle
        cppviz::Coordinate triangle = shape.RightTriangle(200, 400);
        vp.scene.add(cppviz::Translate(triangle, 300, 100));
    
        // Add a rotated nonagon
        cppviz::Coordinate nonagon = shape.RegularPolygon(9, 80);
        vp.scene.add(cppviz::Rotate(nonagon, 40.0));

    // Add a signal waveform
    cppviz::Signal signal;
    cppviz::SignalDataResult sig = signal.SineSignal(2.0f, 200, 3.0f);
    vp.scene.add(sig);

    // Manipulate the view
    vp.zoomIn(0.8f);
    vp.panBy(0, 0);

    vp.saveToFile("output.svg");
}