#define GLUT_DISABLE_ATEXIT_HACK
#include "algorithms.h"
#include <GL/glut.h>
#include <cmath>

void drawPixel(int x, int y) { glBegin(GL_POINTS); glVertex2i(x, y); glEnd(); }

void drawLineDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1, dy = y2 - y1;
    float steps = std::max(abs(dx), abs(dy));
    float xInc = dx / steps, yInc = dy / steps;
    float x = x1, y = y1;
    for (int i = 0; i <= steps; i++) { drawPixel(round(x), round(y)); x += xInc; y += yInc; }
}

void drawLineBres(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1), sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1, err = dx - dy;
    while (true) {
        drawPixel(x1, y1); if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void drawCircleBres(int cx, int cy, int r) {
    int x = 0, y = r, d = 3 - 2 * r;
    while (y >= x) {
        drawPixel(cx+x, cy+y); drawPixel(cx-x, cy+y); drawPixel(cx+x, cy-y); drawPixel(cx-x, cy-y);
        drawPixel(cx+y, cy+x); drawPixel(cx-y, cy+x); drawPixel(cx+y, cy-x); drawPixel(cx-y, cy-x);
        x++; if (d > 0) { y--; d = d + 4 * (x - y) + 10; } else d = d + 4 * x + 6;
    }
}

void drawText(float x, float y, std::string s, void* font) {
    glRasterPos2f(x, y); for (char c : s) glutBitmapCharacter(font, c);
}
