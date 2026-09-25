# 🛠️ Forge-Lite SDK

Forge-Lite is a lightweight graphics framework written in pure C. It provides native window management, OpenGL rendering, 2D drawing, texture loading, keyboard and mouse input, and collision detection in a small, self-contained SDK.

## Features

- Native desktop window creation
- OpenGL-based 2D rendering
- Shapes, textures, and images
- Keyboard and mouse input
- Delta-time tracking
- Rectangle collision detection
- Static library distribution

## SDK Contents

After building the project, the SDK includes:

- **`Forge-Lite.h`** — Public API declarations and data types.
- **`libforge_lite.a`** — The compiled static library.

## API Reference

| Function | Description |
| --- | --- |
| `void init(int width, int height, const char* title)` | Creates the application window and initializes the graphics system. Call this before using the framework. |
| `int window_is_open(void)` | Returns `1` while the window is open and `0` after it has been closed. |
| `void update_screen(void)` | Presents the current frame, updates delta time, processes events, and clears the screen for the next frame. Call once per frame. |
| `void setBackgroundColor(Color color)` | Sets the color used to clear the screen. |
| `float getDeltaTime(void)` | Returns the elapsed time since the previous frame in seconds. |
| `Texture loadTexture(const char* filepath)` | Loads an image into GPU memory. A texture with `id == 0` indicates failure. |
| `void drawShape(ShapeType type, Color color, Texture texture, float x, float y, float width, float height)` | Draws a shape using a color or texture. Provide either a color or a texture, not both. |
| `void drawImage(Texture texture, float x, float y, float width, float height)` | Draws a texture. Pass `0` for `width` or `height` to use the texture's original dimensions. |
| `int isKeyDown(int key)` | Returns `1` if the specified key is pressed and `0` otherwise. Lowercase letters are converted to uppercase. |
| `void getMousePosition(float* x, float* y)` | Writes the mouse position in window coordinates to `x` and `y`. |
| `int isMouseButtonDown(int button)` | Returns `1` if a mouse button is held and `0` otherwise. Button `0` is left click and button `1` is right click. |
| `int checkCollision(Rectangle r1, Rectangle r2)` | Returns `1` if two rectangles overlap and `0` otherwise. |
| `Rectangle getShapeBounds(ShapeType type, float x, float y, float width, float height)` | Returns the axis-aligned bounding rectangle for a shape. |
| `void shutdown_framework(void)` | Releases framework resources and destroys the application window. Call before exiting. |

Refer to **`Forge-Lite.h`** for the exact definitions of `Color`, `Texture`, `ShapeType`, and `Rectangle`.

## Building an Application

Place your application source file next to the SDK files:

```text
your_project/
├── Forge-Lite.h
├── libforge_lite.a
└── main.c
```

### Windows with GCC

Run the following command from the project directory:

```bash
gcc main.c libforge_lite.a -lgdi32 -lwinmm -o game.exe
```

## Building Forge-Lite from Source

Forge-Lite uses CMake. Configure and build the project with:

```bash
cmake -S . -B build
cmake --build build --config Debug
```

For a release build, use:

```bash
cmake --build build --config Release
```

## Typical Program Structure

```c
#include "Forge-Lite.h"

int main(void) {
    init(800, 600, "Forge-Lite Example");

    while (window_is_open()) {
        update_screen();

        /* Draw shapes and images here. */
    }

    shutdown_framework();
    return 0;
}
```

## Contributing

Contributions, bug reports, and feature requests are welcome. Please open an issue before making substantial changes.

## License

See the repository's `LICENSE` file for licensing information.
