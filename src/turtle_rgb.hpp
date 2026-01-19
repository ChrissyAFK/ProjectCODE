#pragma once

#include "canvas_rgb.hpp"

#include <cmath>
#include <chrono>
#include <thread>

namespace projectcode {

class TurtleRGB {
public:
    explicit TurtleRGB(CanvasRGB &canvas, double start_x = 0.0, double start_y = 0.0)
        : canvas_(canvas), x_(start_x), y_(start_y) {
        clamp_to_canvas();
    }

    void set_delay_ms(unsigned delay_ms) { delay_ms_ = delay_ms; }

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
        apply_delay();
    }

    void move_to(double new_x, double new_y, bool draw = false) {
        if (draw) {
            draw_line(static_cast<int>(std::round(x_)), static_cast<int>(std::round(y_)),
                      static_cast<int>(std::round(new_x)), static_cast<int>(std::round(new_y)));
        }
        x_ = new_x;
        y_ = new_y;
        clamp_to_canvas();
        apply_delay();
    }

    void turn_left(double degrees) { heading_degrees_ = normalize_angle(heading_degrees_ + degrees); }
    void turn_right(double degrees) { heading_degrees_ = normalize_angle(heading_degrees_ - degrees); }

    void pen_down() { pen_is_down_ = true; }
    void pen_up() { pen_is_down_ = false; }

    void set_pen(Color color) { pen_color_ = color; }

    void set_heading(double degrees) { heading_degrees_ = normalize_angle(degrees); }
    double heading() const { return heading_degrees_; }

    double x() const { return x_; }
    double y() const { return y_; }

    void stamp_dot(int radius = 3, Color color = {0, 0, 0}) {
        draw_filled_circle(static_cast<int>(std::round(x_)), static_cast<int>(std::round(y_)), radius, color);
    }

private:
    CanvasRGB &canvas_;
    double x_;
    double y_;
    double heading_degrees_ = 0.0; // 0 degrees points right
    bool pen_is_down_ = true;
    Color pen_color_{0, 0, 0};
    unsigned delay_ms_ = 0;

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
        int dx = std::abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;

        int x = x0;
        int y = y0;
        while (true) {
            canvas_.set_pixel(x, y, pen_color_);
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

    void draw_filled_circle(int cx, int cy, int r, Color color) {
        if (r <= 0) return;
        int r2 = r * r;
        for (int dy = -r; dy <= r; ++dy) {
            for (int dx = -r; dx <= r; ++dx) {
                if (dx * dx + dy * dy <= r2) {
                    canvas_.set_pixel(cx + dx, cy + dy, color);
                }
            }
        }
    }

    void apply_delay() {
        if (delay_ms_ == 0) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms_));
    }
};

} // projectcode by Christopher Shen
