#include "Forge-Lite.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../3rdparty/stb_image.h"

static GLFWwindow* g_window = NULL;
static GLuint g_vao = 0, g_vbo = 0, g_shader = 0;
static Color g_clear_color = {0.0f, 0.0f, 0.0f, 1.0f};

// Internal Time Tracking Registers
static double g_last_frame_time = 0.0;
static float g_delta_time = 0.0f;

// Upgraded modern layout shaders tracking position coordinates and layout texCoords
static const char* vs_src = 
    "#version 450 core\n"
    "layout (location = 0) in vec2 p;\n"
    "layout (location = 1) in vec2 t;\n"
    "out vec2 uv;\n"
    "uniform mat4 uM;\n"
    "void main() { gl_Position = uM * vec4(p, 0.0, 1.0); uv = t; }\n";

static const char* fs_src = 
    "#version 450 core\n"
    "in vec2 uv;\n"
    "out vec4 f;\n"
    "uniform vec4 uC;\n"
    "uniform sampler2D uTex;\n"
    "uniform int uUseTex;\n"
    "void main() { f = (uUseTex == 1) ? texture(uTex, uv) : uC; }\n";

void init(int width, int height, const char* title) {
    if (!glfwInit()) return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    g_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!g_window) { glfwTerminate(); return; }
    glfwMakeContextCurrent(g_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Initialize the baseline clock frame timestamp anchor
    g_last_frame_time = glfwGetTime();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER), fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &vs_src, NULL); glCompileShader(vs);
    glShaderSource(fs, 1, &fs_src, NULL); glCompileShader(fs);
    g_shader = glCreateProgram(); glAttachShader(g_shader, vs); glAttachShader(g_shader, fs);
    glLinkProgram(g_shader); glDeleteShader(vs); glDeleteShader(fs);

    float L = 0.0f, R = (float)width, B = (float)height, T = 0.0f;
    float proj[] = {2.0f/(R-L),0,0,0, 0,2.0f/(T-B),0,0, 0,0,-1.0f,0, -(R+L)/(R-L),-(T+B)/(T-B),0,1.0f};
    glUseProgram(g_shader);
    glUniformMatrix4fv(glGetUniformLocation(g_shader, "uM"), 1, GL_FALSE, proj);

    glGenVertexArrays(1, &g_vao); glGenBuffers(1, &g_vbo);
    glBindVertexArray(g_vao); glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    
    // Reserve space for position (2x float) + texture UV mapping coordinate (2x float)
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 260, NULL, GL_DYNAMIC_DRAW);
    
    // Positions (Location 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // UV Layout Coordinates (Location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

float getDeltaTime(void) {
    return g_delta_time;
}

void setBackgroundColor(Color color) { 
    g_clear_color = color; 
}

int window_is_open(void) { 
    return g_window ? !glfwWindowShouldClose(g_window) : 0; 
}

void update_screen(void) { 
    if (g_window) { 
        glfwSwapBuffers(g_window); 
        glfwPollEvents(); 

        // Calculate Delta Time slices accurately
        double current_time = glfwGetTime();
        g_delta_time = (float)(current_time - g_last_frame_time);
        g_last_frame_time = current_time;

        // Caps maximum delta step to prevent physics clipping / extreme jumps during frame hitches
        if (g_delta_time > 0.1f) g_delta_time = 0.1f;

        glClearColor(g_clear_color.r, g_clear_color.g, g_clear_color.b, g_clear_color.a); 
        glClear(GL_COLOR_BUFFER_BIT); 
    } 
}

void shutdown_framework(void) { 
    glDeleteBuffers(1, &g_vbo); 
    glDeleteVertexArrays(1, &g_vao); 
    glDeleteProgram(g_shader); 
    glfwDestroyWindow(g_window); 
    glfwTerminate(); 
}


Texture loadTexture(const char* filepath) {
    Texture tex = {0, 0, 0};
    int w, h, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(filepath, &w, &h, &channels, 4);
    
    if (!data) {
        printf("Forge-Lite Error: Failed to load image asset source at path: %s\n", filepath);
        return tex;
    }

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    tex.width = w; tex.height = h;
    return tex;
}

void drawShape(ShapeType type, Color color, Texture texture, float x, float y, float width, float height) {
    int hasColor = (color.a > 0.001f || color.r > 0.0f || color.g > 0.0f || color.b > 0.0f);
    int hasTexture = (texture.id > 0);

    // Beginner Rule Assertions: Check structural configurations explicitly
    if (!hasColor && !hasTexture) {
        printf("\n🛑 FORGE-LITE RUNTIME ERROR:\n    You called drawShape() but provided neither a color nor a texture! Choose one to draw.\n\n");
        exit(1);
    }
    if (hasColor && hasTexture) {
        printf("\n🛑 FORGE-LITE RUNTIME ERROR:\n    Conflict in drawShape()! You passed BOTH a solid Color and an image Texture. Choose only one.\n\n");
        exit(1);
    }

    glUseProgram(g_shader);
    glBindVertexArray(g_vao);

    if (hasTexture) {
        glUniform1i(glGetUniformLocation(g_shader, "uUseTex"), 1);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    } else {
        glUniform1i(glGetUniformLocation(g_shader, "uUseTex"), 0);
        glUniform4f(glGetUniformLocation(g_shader, "uC"), color.r, color.g, color.b, color.a);
    }

    if (type == SHAPE_RECTANGLE) {
        // Layout: X, Y, U, V
        float v[] = { 
            x,y, 0,1,         x,y+height, 0,0,   x+width,y+height, 1,0,
            x,y, 0,1,         x+width,y+height, 1,0,   x+width,y, 1,1 
        };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    } else if (type == SHAPE_TRIANGLE) {
        float v[] = { x+width/2.0f,y, 0.5f,1.0f,   x,y+height, 0,0,   x+width,y+height, 1,0 };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    } else if (type == SHAPE_CIRCLE) {
        float v[260]; 
        float rx = width / 2.0f, ry = height / 2.0f;
        float cx = x + rx, cy = y + ry;
        
        v[0] = cx; v[1] = cy; v[2] = 0.5f; v[3] = 0.5f; // Center point
        for (int i = 0; i <= 63; i++) {
            float a = i * (2.0f * 3.14159265f / 63.0f);
            float cosVal = cosf(a);
            float sinVal = sinf(a);
            int idx = 4 + (i * 4);
            v[idx]   = cx + (cosVal * rx);
            v[idx+1] = cy + (sinVal * ry);
            v[idx+2] = 0.5f + (cosVal * 0.5f); // Map circular UV bounds
            v[idx+3] = 0.5f + (sinVal * 0.5f);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 65);
    } else if (type == SHAPE_LINE) {
        float v[] = { x,y, 0,0,   x+width,y+height, 1,1 };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
        glLineWidth(2.0f); 
        glDrawArrays(GL_LINES, 0, 2);
    }
}

void drawImage(Texture texture, float x, float y, float width, float height) {
    Color transparentColor = {0.0f, 0.0f, 0.0f, 0.0f};
    float finalWidth = (width <= 0.0f) ? (float)texture.width : width;
    float finalHeight = (height <= 0.0f) ? (float)texture.height : height;
    drawShape(SHAPE_RECTANGLE, transparentColor, texture, x, y, finalWidth, finalHeight);
}

int isKeyDown(int key) {
    if (!g_window) return 0;
    if (key >= 'a' && key <= 'z') key -= 32; 
    return glfwGetKey(g_window, key) == GLFW_PRESS;
}

void getMousePosition(float* x, float* y) {
    if (!g_window) { *x = 0; *y = 0; return; }
    double mouseX, mouseY;
    glfwGetCursorPos(g_window, &mouseX, &mouseY);
    *x = (float)mouseX;
    *y = (float)mouseY;
}

int isMouseButtonDown(int button) {
    if (!g_window) return 0;
    return glfwGetMouseButton(g_window, button) == GLFW_PRESS;
}

int checkCollision(Rectangle r1, Rectangle r2) {
    return (r1.x < r2.x + r2.width  &&
            r1.x + r1.width > r2.x  &&
            r1.y < r2.y + r2.height &&
            r1.y + r1.height > r2.y);
}

Rectangle getShapeBounds(ShapeType type, float x, float y, float width, float height) {
    Rectangle bounds;
    if (type == SHAPE_LINE) {
        bounds.x = (width < 0.0f) ? x + width : x;
        bounds.y = (height < 0.0f) ? y + height : y;
        bounds.width = fabsf(width);
        bounds.height = fabsf(height);
    } else {
        bounds.x = x;
        bounds.y = y;
        bounds.width = width;
        bounds.height = height;
    }
    return bounds;
}
