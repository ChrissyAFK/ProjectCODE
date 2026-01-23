# ProjectCODE Turtle C++ (educational mini-library)

A lightweight, header-only friendly C++17 mini-library inspired by Python's `turtle` module. It draws on an ASCII canvas to make basic programming constructs (loops, variables, conditionals) tangible for beginners.

## Features
- `Canvas` for ASCII rendering (configurable size and blank character)
- `Turtle` with `forward`, `turn_left`, `turn_right`, `move_to`, `pen_up/pen_down`, `set_pen`, and heading control
- Bresenham line drawing for clean straight segments
- Top-left origin with Y increasing downward (common for console grids)
- Minimal dependencies (C++17 STL only)

## File layout
- `src/canvas.hpp`: Simple ASCII 2D character grid
- `src/turtle.hpp`: ASCII turtle implementation
- `src/canvas_rgb.hpp`: Color (RGB) canvas with PPM and BMP export
- `src/turtle_rgb.hpp`: Color turtle implementation
- `src/shapes.hpp`: Convenience helpers (rgb, draw_polygon, draw_spiral)
- `src/window.hpp`: Win32 helper to open a window and run user draw code
- `examples/basic_demo.cpp`: ASCII demo showing loops, conditionals, pen control
- `examples/color_demo.cpp`: Color demo that saves `color_output.ppm`
- `examples/window_demo.cpp`: Color demo that opens a Win32 window (no external deps)
- `examples/filled_square_demo.cpp`: Windowed demo using a variable and for-loop to draw and fill a square

## Build & run (PowerShell, C++17)

```powershell
# From repository root
$ g++ -std=c++17 -I./src examples/basic_demo.cpp -o demo
$ ./demo
```

Expected output: an ASCII canvas containing a square, triangle, spiral, and a short vertical stroke, demonstrating loops, pen state changes, and conditionals.

Color demo:

```powershell
$ g++ -std=c++17 -I./src examples/color_demo.cpp -o color_demo
$ ./color_demo
```

This writes `color_output.ppm` (binary PPM, P6) and `color_output.bmp` (24-bit BMP). Both are viewable in most image viewers. You can also convert to PNG:

```powershell
# With ImageMagick installed
$ magick convert color_output.ppm color_output.png
```

Windowed demo (Windows, links against gdi32 only):

```powershell
$ g++ -std=c++17 -lgdi32 -I./src examples/window_demo.cpp -o window_demo
$ ./window_demo
```
The window is driven by `projectcode::run_window` in `src/window.hpp`; your draw lambda receives `CanvasRGB`, `TurtleRGB`, and a `flush()` callback to repaint/pump messages as you draw.

Tip: both demos set a small per-move delay so drawing is visible. Adjust with `t.set_delay_ms(...)`.

## Using in your own code
```cpp
#include "turtle.hpp"
#include "canvas.hpp"

int main() {
	projectcode::Canvas canvas(40, 20);
	projectcode::Turtle t(canvas, 5, 5);

	t.forward(10);
	t.turn_left(90);
	t.set_pen('#');
	t.forward(8);

	canvas.render();
}
```

Windowed usage (opens a window for you):
```cpp
#include "window.hpp"

int wmain() { // or int main() on MSVC
	projectcode::WindowOptions opts; opts.title = L"My Turtle";
	return projectcode::run_window(opts, [](projectcode::CanvasRGB &canvas,
											projectcode::TurtleRGB &t,
											std::function<bool()> flush) {
		t.move_to(50, 50);
		t.set_pen({255, 0, 0});
		for (int i = 0; i < 36; ++i) {
			t.forward(10);
			t.turn_left(10);
			if (!flush()) return; // repaint window and pump messages
		}
	});
}
```

## Teaching ideas
- **Loops**: draw polygons, spirals, or repeated patterns
- **Variables**: store lengths/angles and tweak them live
- **Conditionals**: change `set_pen` or direction based on state
- **Functions**: wrap patterns like `draw_polygon` and reuse

## Notes
- Coordinates outside the canvas are ignored; drawing clamps to the valid grid.
- Heading `0` points right; positive angles turn counterclockwise.
