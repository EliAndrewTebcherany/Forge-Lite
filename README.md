# 🛠️ Forge-Lite SDK

Welcome to **Forge-Lite**, a lightweight graphics framework written in pure C. Forge-Lite bundles window management, input handling, image loading, and modern OpenGL rendering into a fast, self-contained SDK designed for beginners and hobby engine developers alike.

---

## ⚡ Quick Example

```c
#include "Forge-Lite.h"

int main(void)
{
    init(800, 600, "Hello Forge-Lite");

    Color red = {1.0f, 0.0f, 0.0f, 1.0f};

    while (window_is_open())
    {
        drawShape(
            SHAPE_CIRCLE,
            red,
            (Texture){0},
            100,
            100,
            100,
            100
        );

        update_screen();
    }

    shutdown_framework();
    return 0;
}
```

---

## ⚙️ Under the Hood

Forge-Lite combines several industry-standard graphics technologies into a single beginner-friendly package:

- **OpenGL 4.5 Core Profile** → Hardware accelerated rendering
- **GLFW** → Window creation, keyboard input, mouse input, and platform abstraction
- **GLAD** → Modern OpenGL function loading
- **stb_image** → Lightweight image and texture loading
- **CMake** → Cross-platform build system

All low-level graphics setup is handled internally by Forge-Lite, allowing developers to focus on building applications instead of configuring graphics infrastructure.

---

## 📦 What's Inside the SDK Folder

Once compiled, this SDK distributes two primary components:

- **`Forge-Lite.h`** : Public framework interface.
- **`libforge_lite.a`** : Static compiled Forge-Lite engine archive.

---

## 📋 API Function Reference

| Function Signature | Description | Behavior |
|-------------------|-------------|----------|
| `void init(int width, int height, const char* title)` | Creates the application window and initializes the rendering backend. | Initializes Forge-Lite. |
| `int window_is_open(void)` | Returns the window state. | `1` = Running, `0` = Closed |
| `void update_screen(void)` | Presents the frame, polls events, updates timing, and clears the screen. | Advances one frame. |
| `void setBackgroundColor(Color color)` | Defines the color used when clearing the screen. | Changes background color. |
| `float getDeltaTime(void)` | Retrieves elapsed time between frames. | Returns seconds as `float`. |
| `Texture loadTexture(const char* filepath)` | Loads an image into GPU memory. | Returns a `Texture`. |
| `void drawShape(ShapeType type, Color color, Texture texture, float x, float y, float width, float height)` | Draws a geometric primitive using either a color or a texture. | Renders a shape. |
| `void drawImage(Texture texture, float x, float y, float width, float height)` | Draws a texture directly. | Renders an image. |
| `int isKeyDown(int key)` | Checks whether a keyboard key is currently pressed. | Returns `1` or `0`. |
| `void getMousePosition(float* x, float* y)` | Retrieves mouse coordinates. | Writes directly into pointers. |
| `int isMouseButtonDown(int button)` | Checks mouse button state. | Returns `1` or `0`. |
| `int checkCollision(Rectangle r1, Rectangle r2)` | Performs AABB collision detection. | Returns `1` if colliding. |
| `Rectangle getShapeBounds(ShapeType type, float x, float y, float width, float height)` | Generates a bounding rectangle from a shape. | Returns a `Rectangle`. |
| `void shutdown_framework(void)` | Releases all resources and closes the framework. | Terminates Forge-Lite. |

---

## 🎨 Shape System Philosophy

Forge-Lite minimizes the number of concepts beginners must learn.

Instead of exposing a large collection of drawing functions, Forge-Lite uses a unified API:

```c
drawShape(
    type,
    color,
    texture,
    x,
    y,
    width,
    height
);
```

Examples:

```c
// Rectangle
drawShape(
    SHAPE_RECTANGLE,
    red,
    (Texture){0},
    50,
    50,
    150,
    100
);

// Triangle
drawShape(
    SHAPE_TRIANGLE,
    blue,
    (Texture){0},
    250,
    50,
    150,
    100
);

// Perfect Circle
drawShape(
    SHAPE_CIRCLE,
    green,
    (Texture){0},
    450,
    50,
    100,
    100
);

// Ellipse
drawShape(
    SHAPE_CIRCLE,
    green,
    (Texture){0},
    450,
    50,
    200,
    100
);
```

A circle is simply an ellipse whose width and height are equal.

This allows users to learn one API while unlocking multiple shape types.

---

## 🖼️ Texture Example

```c
Texture logo = loadTexture("logo.png");

drawImage(
    logo,
    100,
    100,
    0,
    0
);
```

Passing `0` for width and height automatically uses the original image dimensions.

---

## ⌨️ Input Example

```c
if (isKeyDown('W'))
{
    playerY -= 200.0f * getDeltaTime();
}

if (isMouseButtonDown(0))
{
    // Left mouse button pressed
}
```

---

## 💥 Collision Example

```c
Rectangle player =
    getShapeBounds(
        SHAPE_RECTANGLE,
        playerX,
        playerY,
        50,
        50
    );

Rectangle wall =
    getShapeBounds(
        SHAPE_RECTANGLE,
        300,
        200,
        100,
        100
    );

if (checkCollision(player, wall))
{
    // Collision detected
}
```

---

## 🚀 Compilation Blueprint

### Project Layout

```text
your_project/
├── Forge-Lite.h
├── libforge_lite.a
└── main.c
```

### Windows (GCC)

```bash
gcc main.c libforge_lite.a -lgdi32 -lwinmm -o game.exe
```

### Linux (GCC)

```bash
gcc main.c libforge_lite.a -ldl -lpthread -lm -o game
```

---

## 🔧 Building Forge-Lite From Source

For contributors and engine developers:

```bash
# Create a fresh build tree
cmake -S . -B build --fresh

# Build Forge-Lite
cmake --build build --config Debug
```

---

## 🎯 Project Goals

Forge-Lite aims to:

- Simplify modern OpenGL development.
- Provide a lightweight alternative to larger frameworks.
- Hide graphics boilerplate from beginners.
- Offer a consistent API based on a small set of concepts.
- Remain easy to learn without sacrificing flexibility.

---

## 📜 License

MIT License

Copyright (c) Forge-Lite Contributors
``
