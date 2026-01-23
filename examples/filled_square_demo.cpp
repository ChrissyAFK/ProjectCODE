#include "../src/window.hpp"
#include "../src/shapes.hpp"
#include <functional>

using namespace projectcode;

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    WindowOptions opts;
    opts.width = 500;
    opts.height = 500;
    opts.title = L"Filled Square Demo";

    auto draw = [&](CanvasRGB &, TurtleRGB &t, std::function<bool()> flush) {
        t.set_delay_ms(50);

        // Use a variable for side length to teach variable usage
        int side = 200;
        t.move_to(100, 300); // starting position
        // Draw the square outline with a for loop
        for (int i = 0; i < 4; ++i) {
            t.set_delay_ms(500);
            t.forward(side);
            t.turn_left(90);
            if (!flush()) return;
        }

        // Mark turtle head
        t.stamp_dot(4, rgb(0, 0, 0));
        flush();
    };

    return run_window(opts, draw);
}
