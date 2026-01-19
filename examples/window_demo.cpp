#include "../src/window.hpp"
#include "../src/shapes.hpp"

using namespace projectcode;

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    WindowOptions opts;
    opts.width = 800;
    opts.height = 600;
    opts.title = L"ProjectCODE Turtle";

    auto draw = [&](CanvasRGB &, TurtleRGB &t, std::function<bool()> flush) {
        t.set_delay_ms(5);

        move_to_with_pen(t, 150, 300, false, flush);
        draw_polygon(t, 4, 120, rgb(255, 69, 0), flush);

        move_to_with_pen(t, 420, 200, false, flush);
        draw_polygon(t, 3, 140, rgb(0, 128, 255), flush);

        move_to_with_pen(t, 500, 400, false, flush);
        draw_spiral(t, 60, 3.0, 18.0, rgb(34, 139, 34), flush);

        move_to_with_pen(t, 100, 100, false, flush);
        t.set_pen(rgb(128, 0, 128));
        t.turn_left(90);
        t.forward(180);
        flush();

        t.stamp_dot(5, rgb(0, 0, 0));
        flush();
    };

    return run_window(opts, draw);
}
