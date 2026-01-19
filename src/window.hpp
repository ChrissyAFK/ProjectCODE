#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "canvas_rgb.hpp"
#include "turtle_rgb.hpp"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace projectcode {

struct WindowOptions {
    int width = 800;
    int height = 600;
    Color background{240, 248, 255};
    std::wstring title = L"ProjectCODE";
};

namespace detail {
struct Framebuffer {
    std::vector<std::uint8_t> bytes;
    BITMAPINFO bmi{};
};

inline Framebuffer make_framebuffer(const CanvasRGB &canvas) {
    const int width = static_cast<int>(canvas.width());
    const int height = static_cast<int>(canvas.height());
    const std::size_t row_stride = static_cast<std::size_t>(width) * 3;
    const std::size_t row_padded = (row_stride + 3u) & ~3u; // 4-byte alignment
    Framebuffer fb;
    fb.bytes.resize(row_padded * static_cast<std::size_t>(height), 0);

    fb.bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    fb.bmi.bmiHeader.biWidth = width;
    fb.bmi.bmiHeader.biHeight = -height; // top-down to match canvas coords
    fb.bmi.bmiHeader.biPlanes = 1;
    fb.bmi.bmiHeader.biBitCount = 24;
    fb.bmi.bmiHeader.biCompression = BI_RGB;
    fb.bmi.bmiHeader.biSizeImage = static_cast<DWORD>(row_padded * static_cast<std::size_t>(height));

    const auto &pixels = canvas.data();
    for (int y = 0; y < height; ++y) {
        std::uint8_t *row = fb.bytes.data() + static_cast<std::size_t>(y) * row_padded;
        for (int x = 0; x < width; ++x) {
            const Color &c = pixels[static_cast<std::size_t>(y) * canvas.width() + static_cast<std::size_t>(x)];
            row[x * 3 + 0] = c.b;
            row[x * 3 + 1] = c.g;
            row[x * 3 + 2] = c.r;
        }
    }

    return fb;
}

inline bool pump_messages() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
} // namespace detail

// Run a user-provided draw routine inside a Win32 window.
// draw_fn signature: void(CanvasRGB& canvas, TurtleRGB& turtle, std::function<bool()> flush)
// flush() repaints the window with the current canvas and pumps messages; returns false if WM_QUIT.
template <class DrawFn>
int run_window(const WindowOptions &opts, DrawFn draw_fn) {
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // Register window class once.
    static bool registered = false;
    static const wchar_t CLASS_NAME[] = L"ProjectCODEWindowClass";
    if (!registered) {
        WNDCLASSW wc{};
        wc.lpfnWndProc = detail::WndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassW(&wc);
        registered = true;
    }

    RECT rect{0, 0, opts.width, opts.height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, opts.title.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) {
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    HDC hdc = GetDC(hwnd);

    CanvasRGB canvas(static_cast<std::size_t>(opts.width), static_cast<std::size_t>(opts.height), opts.background);
    TurtleRGB turtle(canvas, 0, 0);

    auto paint = [&](const CanvasRGB &c) {
        detail::Framebuffer fb = detail::make_framebuffer(c);
        StretchDIBits(hdc,
                      0, 0, opts.width, opts.height,
                      0, 0, opts.width, opts.height,
                      fb.bytes.data(), &fb.bmi, DIB_RGB_COLORS, SRCCOPY);
    };

    std::function<bool()> flush = [&]() {
        paint(canvas);
        return detail::pump_messages();
    };

    draw_fn(canvas, turtle, flush);

    // Final paint.
    paint(canvas);

    // Keep window open until closed.
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseDC(hwnd, hdc);
    return 0;
}

} // namespace projectcode
