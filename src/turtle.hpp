#pragma once

#include "canvas.hpp"

#include <cmath>
#include <stdexcept>

namespace projectcode {

class Turtle {
public:
    explicit Turtle(Canvas &canvas, double start_x = 0.0, double start_y = 0.0)
        : canvas_(canvas), x_(start_x), y_(start_y) {
        clamp_to_canvas();
    }

    void forward(double distance) {
        if (distance == 0.0) return;
        double rad = deg_to_rad(heading_degrees_);
        double new_x = x_ + distance * std::cos(rad);
        double new_y = y_ - distance * std::sin(rad); // invert Y for top-left origin
        draw_line(static_cast<int>(std::round(x_)), static_cast<int>(std::round(y_)),
                  static_cast<int>(std::round(new_x)), static_cast<int>(std::round(new_y)));
        x_ = new_x;
        y_ = new_y;
        clamp_to_canvas();
    }

    void move_to(double new_x, double new_y, bool draw = false) {
        if (draw) {
            draw_line(static_cast<int>(std::round(x_)), static_cast<int>(std::round(y_)),
                      static_cast<int>(std::round(new_x)), static_cast<int>(std::round(new_y)));
        }
        x_ = new_x;
        y_ = new_y;
        clamp_to_canvas();
    }

    void turn_left(double degrees) { heading_degrees_ = normalize_angle(heading_degrees_ + degrees); }
    void turn_right(double degrees) { heading_degrees_ = normalize_angle(heading_degrees_ - degrees); }

    void pen_down() { pen_is_down_ = true; }
    void pen_up() { pen_is_down_ = false; }

    void set_pen(char pen_char) { pen_char_ = pen_char; }

    void set_heading(double degrees) { heading_degrees_ = normalize_angle(degrees); }
    double heading() const { return heading_degrees_; }

    double x() const { return x_; }
    double y() const { return y_; }

private:
    Canvas &canvas_;
    double x_;
    double y_;
    double heading_degrees_ = 0.0; // 0 degrees points right
    bool pen_is_down_ = true;
    char pen_char_ = '*';

    static double deg_to_rad(double degrees) {
        constexpr double pi = 3.14159265358979323846;
        return degrees * pi / 180.0;
    }

    static double normalize_angle(double degrees) {
        double result = std::fmod(degrees, 360.0);
        if (result < 0) result += 360.0;
        return result;
    }

    void clamp_to_canvas() {
        x_ = std::max(0.0, std::min(x_, static_cast<double>(canvas_.width() - 1)));
        y_ = std::max(0.0, std::min(y_, static_cast<double>(canvas_.height() - 1)));
    }

    void draw_line(int x0, int y0, int x1, int y1) {
        if (!pen_is_down_) return;
        // Bresenham's line algorithm for integer grid drawing.
        int dx = std::abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;

        int x = x0;
        int y = y0;
        while (true) {
            canvas_.set_pixel(x, y, pen_char_);
            if (x == x1 && y == y1) break;
            int e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y += sy;
            }
        }
    }
};

} // projectcode by Christopher Shen
