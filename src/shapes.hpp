#pragma once

#include "turtle_rgb.hpp"

#include <algorithm>
#include <functional>

namespace projectcode {

inline Color rgb(int r, int g, int b) {
    auto clampc = [](int v) {
        return static_cast<unsigned char>(std::clamp(v, 0, 255));
    };
    return Color{clampc(r), clampc(g), clampc(b)};
}

inline bool draw_polygon(TurtleRGB &t, int sides, double length, Color color,
                         const std::function<bool()> &flush = nullptr) {
    if (sides < 3) return true;
    t.set_pen(color);
    double angle = 360.0 / static_cast<double>(sides);
    for (int i = 0; i < sides; ++i) {
        t.forward(length);
        t.turn_left(angle);
        if (flush && !flush()) return false;
    }
    return true;
}

inline bool draw_spiral(TurtleRGB &t, int steps, double increment, Color color,
                        double turn_degrees = 18.0,
                        const std::function<bool()> &flush = nullptr) {
    t.set_pen(color);
    double length = increment;
    for (int i = 0; i < steps; ++i) {
        t.forward(length);
        t.turn_right(turn_degrees);
        length += increment;
        if (flush && !flush()) return false;
    }
    return true;
}

} // namespace projectcode
