#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace projectcode {

// A simple 2D character canvas for ASCII drawing.
class Canvas {
public:
    Canvas(std::size_t width, std::size_t height, char blank = ' ')
        : width_(width), height_(height), blank_(blank), pixels_(height, std::string(width, blank)) {
        assert(width_ > 1 && height_ > 1 && "Canvas dimensions must be positive");
    }

    std::size_t width() const { return width_; }
    std::size_t height() const { return height_; }

    void clear() {
        for (auto &row : pixels_) {
            std::fill(row.begin(), row.end(), blank_);
        }
    }

    void set_pixel(int x, int y, char pen) {
        if (x < 0 || y < 0) return;
        if (static_cast<std::size_t>(x) >= width_ || static_cast<std::size_t>(y) >= height_) return;
        pixels_[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = pen;
    }

    void render(std::ostream &os = std::cout) const {
        for (std::size_t row = 0; row < height_; ++row) {
            os << pixels_[row] << '\n';
        }
    }

private:
    std::size_t width_;
    std::size_t height_;
    char blank_;
    std::vector<std::string> pixels_;
};

} // projectcode by Christopher Shen
