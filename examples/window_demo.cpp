#include "../src/window.hpp"
#include "../src/shapes.hpp"

using namespace projectcode;

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    WindowOptions opts;
    opts.width = 800;
    opts.height = 600;
    opts.title = L"ProjectCODE Turtle";

    auto draw = [&](CanvasRGB &, TurtleRGB &t, std::function<bool()> flush) {
        t.set_delay_ms(50);

        t.move_to(150, 300); 
        flush();
        draw_polygon(t, 4, 120, rgb(255, 69, 0), flush);
        t.pen_up(); 
        t.move_to(420, 200); 
        t.pen_down(); 
        flush();
        draw_polygon(t, 3, 140, rgb(0, 128, 255), flush);
        t.pen_up(); 
        t.move_to(500, 400); 
        t.pen_down(); 
        flush();
        draw_spiral(t, 60, 3.0, rgb(34, 139, 34), 18.0, flush);
        t.pen_up(); 
        t.move_to(100, 100); 
        t.pen_down(); 
        flush();
        t.set_pen(rgb(128, 0, 128)); 
        t.turn_left(90); 
        t.forward(180); 
        flush();
        t.stamp_dot(5, rgb(0, 0, 0)); 
        flush();
    };

    return run_window(opts, draw);
}
