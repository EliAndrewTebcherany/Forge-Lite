#ifndef FORGE_LITE_H
#define FORGE_LITE_H

typedef struct {
    float r, g, b, a;
} Color;

typedef struct {
    unsigned int id;
    int width;
    int height;
} Texture;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} Rectangle;

typedef enum {
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE,
    SHAPE_CIRCLE,
    SHAPE_LINE
} ShapeType;

// Core Window Controls
void init(int width, int height, const char* title);
int window_is_open(void);
void update_screen(void);
void shutdown_framework(void);
void setBackgroundColor(Color color);

// ⏱️ NEW FEATURE: Time Management API
float getDeltaTime(void);       // Returns elapsed frame time in seconds (e.g., 0.016s for 60FPS)

// Textures & Images
Texture loadTexture(const char* filepath);
void drawShape(ShapeType type, Color color, Texture texture, float x, float y, float width, float height);
void drawImage(Texture texture, float x, float y, float width, float height);

// Inputs
int isKeyDown(int key);
void getMousePosition(float* x, float* y);
int isMouseButtonDown(int button);

// Collisions
int checkCollision(Rectangle r1, Rectangle r2);
Rectangle getShapeBounds(ShapeType type, float x, float y, float width, float height);

#endif // FORGE_LITE_H
