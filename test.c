#include "Forge-Lite.h"

int main(void) {
    init(800, 600, "Forge Lite Collision System");

    // Color Configurations
    Color dark_bg = {0.1f, 0.1f, 0.12f, 1.0f};
    Color red = {1.0f, 0.2f, 0.2f, 1.0f};
    Color orange = {1.0f, 0.6f, 0.0f, 1.0f};
    Color green = {0.2f, 1.0f, 0.2f, 1.0f};
    Texture noTex = {0, 0, 0};

    setBackgroundColor(dark_bg);

    // Controllable Player properties
    float playerX = 100.0f;
    float playerY = 200.0f;
    float playerSize = 80.0f;

    // Static Wall properties
    float wallX = 400.0f;
    float wallY = 200.0f;
    float wallSize = 120.0f;

    while (window_is_open()) {
        // Move player square with standard inputs
        if (isKeyDown('D')) playerX += 5.0f;
        if (isKeyDown('A')) playerX -= 5.0f;
        if (isKeyDown('W')) playerY -= 5.0f;
        if (isKeyDown('S')) playerY += 5.0f;

        // 1. Gather bounding boxes using our automatic mapping function
        Rectangle playerBox = getShapeBounds(SHAPE_RECTANGLE, playerX, playerY, playerSize, playerSize);
        Rectangle wallBox   = getShapeBounds(SHAPE_RECTANGLE, wallX, wallY, wallSize, wallSize);

        // 2. Perform the overlap logic test split
        Color wallCurrentColor = orange;
        if (checkCollision(playerBox, wallBox)) {
            wallCurrentColor = green; // Turn green upon touch contact!
        }

        // 3. Render objects onto canvas
        drawShape(SHAPE_RECTANGLE, wallCurrentColor, noTex, wallX, wallY, wallSize, wallSize);
        drawShape(SHAPE_RECTANGLE, red, noTex, playerX, playerY, playerSize, playerSize);

        update_screen();
    }

    shutdown_framework();
    return 0;
}
