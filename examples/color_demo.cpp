#include "../src/turtle_rgb.hpp"
#include "../src/shapes.hpp"
#include <iostream>

using namespace projectcode;

int main() {
    CanvasRGB canvas(800, 600, rgb(240, 248, 255)); // light background
    TurtleRGB t(canvas, 150, 300);
    t.set_delay_ms(10); // slow down to visualize drawing

    draw_polygon(t, 4, 120, rgb(255, 69, 0));      // square in orange-red

    t.pen_up();
    t.move_to(420, 200);
    t.pen_down();
    draw_polygon(t, 3, 140, rgb(0, 128, 255));     // triangle in blue

    t.pen_up();
    t.move_to(500, 400);
    t.pen_down();
    draw_spiral(t, 60, 3.0, rgb(34, 139, 34));     // spiral in forest green

    t.pen_up();
    t.move_to(100, 100);
    t.pen_down();
    t.set_pen(rgb(128, 0, 128));                   // purple segment
    t.turn_left(90);
    t.forward(180);

    // Mark turtle location with a small black dot (like Python's turtle head)
    t.stamp_dot(4, rgb(0, 0, 0));

    bool ok_ppm = canvas.save_ppm("color_output.ppm");
    bool ok_bmp = canvas.save_bmp("color_output.bmp");

    if (!ok_ppm || !ok_bmp) {
        std::cerr << "Failed to write output images\n";
        return 1;
    }

    std::cout << "Wrote color_output.ppm (PPM P6) and color_output.bmp (24-bit BMP)." << std::endl;
    return 0;
}
