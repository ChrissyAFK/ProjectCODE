#pragma once

#include "turtle_rgb.hpp"

#include <functional>
#include <optional>

namespace projectcode {

inline Color rgb(int r, int g, int b) {
    return Color{static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b)};
}

// Optional flush callback: if provided, called after each segment; if it returns false, stop early.
inline void draw_polygon(TurtleRGB &t, int sides, double length, Color color,
                         const std::optional<std::function<bool()>> &flush = std::nullopt) {
    if (sides < 3) return;
    t.set_pen(color);
    t.pen_down();
    double angle = 360.0 / static_cast<double>(sides);
    for (int i = 0; i < sides; ++i) {
        t.forward(length);
        t.turn_left(angle);
        if (flush && !(*flush)()) return;
    }
}

inline void draw_spiral(TurtleRGB &t, int steps, double increment, double turn_degrees, Color color,
                        const std::optional<std::function<bool()>> &flush = std::nullopt) {
    t.set_pen(color);
    t.pen_down();
    double length = increment;
    for (int i = 0; i < steps; ++i) {
        t.forward(length);
        t.turn_right(turn_degrees);
        length += increment;
        if (flush && !(*flush)()) return;
    }
}

inline void move_to_with_pen(TurtleRGB &t, double x, double y, bool pen_down,
                             const std::optional<std::function<bool()>> &flush = std::nullopt) {
    if (pen_down) {
        t.pen_down();
        t.move_to(x, y, true);
    } else {
        t.pen_up();
        t.move_to(x, y, false);
    }
    if (flush) (*flush)();
}

} // namespace projectcode
