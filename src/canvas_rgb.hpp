#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace projectcode {

struct Color {
    unsigned char r{0};
    unsigned char g{0};
    unsigned char b{0};
};

class CanvasRGB {
public:
    CanvasRGB(std::size_t width, std::size_t height, Color background = {255, 255, 255})
        : width_(width), height_(height), pixels_(width * height, background) {
        assert(width_ > 0 && height_ > 0 && "Canvas dimensions must be positive");
    }

    std::size_t width() const { return width_; }
    std::size_t height() const { return height_; }

    void clear(Color background) { std::fill(pixels_.begin(), pixels_.end(), background); }

    void set_pixel(int x, int y, Color color) {
        if (x < 0 || y < 0) return;
        if (static_cast<std::size_t>(x) >= width_ || static_cast<std::size_t>(y) >= height_) return;
        pixels_[index(static_cast<std::size_t>(x), static_cast<std::size_t>(y))] = color;
    }

    Color get_pixel(int x, int y) const {
        if (x < 0 || y < 0) return {0, 0, 0};
        if (static_cast<std::size_t>(x) >= width_ || static_cast<std::size_t>(y) >= height_) return {0, 0, 0};
        return pixels_[index(static_cast<std::size_t>(x), static_cast<std::size_t>(y))];
    }

    const std::vector<Color> &data() const { return pixels_; }

    // Save as binary PPM (P6). Simple and dependency-free.
    bool save_ppm(const std::string &filename) const {
        std::ofstream out(filename, std::ios::binary);
        if (!out) return false;
        out << "P6\n" << width_ << " " << height_ << "\n255\n";
        for (const auto &px : pixels_) {
            out.write(reinterpret_cast<const char *>(&px.r), 1);
            out.write(reinterpret_cast<const char *>(&px.g), 1);
            out.write(reinterpret_cast<const char *>(&px.b), 1);
        }
        return true;
    }

    // Save as uncompressed 24-bit BMP. Also dependency-free.
    bool save_bmp(const std::string &filename) const {
        // BMP rows are padded to 4-byte boundaries.
        const std::uint32_t row_stride = static_cast<std::uint32_t>(width_ * 3);
        const std::uint32_t row_padded = (row_stride + 3u) & ~3u;
        const std::uint32_t pixel_data_size = row_padded * static_cast<std::uint32_t>(height_);
        const std::uint32_t header_size = 54u; // 14-file + 40-info
        const std::uint32_t file_size = header_size + pixel_data_size;

        std::ofstream out(filename, std::ios::binary);
        if (!out) return false;

        // BITMAPFILEHEADER
        out.put('B');
        out.put('M');
        write_u32(out, file_size);
        write_u16(out, 0); // reserved1
        write_u16(out, 0); // reserved2
        write_u32(out, header_size);

        // BITMAPINFOHEADER
        write_u32(out, 40); // info header size
        write_u32(out, static_cast<std::uint32_t>(width_));
        write_u32(out, static_cast<std::uint32_t>(height_));
        write_u16(out, 1);   // planes
        write_u16(out, 24);  // bpp
        write_u32(out, 0);   // compression = BI_RGB
        write_u32(out, pixel_data_size);
        write_u32(out, 2835); // x ppm (~72 DPI)
        write_u32(out, 2835); // y ppm
        write_u32(out, 0);    // colors used
        write_u32(out, 0);    // important colors

        // Pixel data: BMP stores bottom-to-top, each row padded to 4 bytes, BGR order.
        std::vector<unsigned char> padding(row_padded - row_stride, 0);
        for (std::size_t row = 0; row < height_; ++row) {
            std::size_t y = height_ - 1 - row; // flip vertically
            for (std::size_t x = 0; x < width_; ++x) {
                const auto &px = pixels_[index(x, y)];
                out.put(static_cast<char>(px.b));
                out.put(static_cast<char>(px.g));
                out.put(static_cast<char>(px.r));
            }
            if (!padding.empty()) {
                out.write(reinterpret_cast<const char *>(padding.data()), padding.size());
            }
        }

        return static_cast<bool>(out);
    }

private:
    std::size_t width_;
    std::size_t height_;
    std::vector<Color> pixels_;

    std::size_t index(std::size_t x, std::size_t y) const { return y * width_ + x; }

    static void write_u16(std::ofstream &out, std::uint16_t v) {
        out.put(static_cast<char>(v & 0xFF));
        out.put(static_cast<char>((v >> 8) & 0xFF));
    }

    static void write_u32(std::ofstream &out, std::uint32_t v) {
        out.put(static_cast<char>(v & 0xFF));
        out.put(static_cast<char>((v >> 8) & 0xFF));
        out.put(static_cast<char>((v >> 16) & 0xFF));
        out.put(static_cast<char>((v >> 24) & 0xFF));
    }
};

} // projectcode by Christopher Shen
