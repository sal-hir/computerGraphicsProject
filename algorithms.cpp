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

void floodFill(float x, float y, float r, float g, float b) {
    glColor3f(r, g, b); glRectf(x-10, y-10, x+10, y+10);
}

const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;
int getCode(double x, double y, double xMin, double xMax, double yMin, double yMax) {
    int code = 0;
    if (x < xMin) code |= LEFT; else if (x > xMax) code |= RIGHT;
    if (y < yMin) code |= BOTTOM; else if (y > yMax) code |= TOP;
    return code;
}

void drawClippedLine(double x1, double y1, double x2, double y2, double xMin, double xMax, double yMin, double yMax) {
    int code1 = getCode(x1, y1, xMin, xMax, yMin, yMax), code2 = getCode(x2, y2, xMin, xMax, yMin, yMax);
    bool accept = false;
    while (true) {
        if ((code1 == 0) && (code2 == 0)) { accept = true; break; }
        else if (code1 & code2) break;
        else {
            int out = code1 ? code1 : code2; double x, y;
            if (out & TOP) { x = x1 + (x2 - x1) * (yMax - y1) / (y2 - y1); y = yMax; }
            else if (out & BOTTOM) { x = x1 + (x2 - x1) * (yMin - y1) / (y2 - y1); y = yMin; }
            else if (out & RIGHT) { y = y1 + (y2 - y1) * (xMax - x1) / (x2 - x1); x = xMax; }
            else if (out & LEFT) { y = y1 + (y2 - y1) * (xMin - x1) / (x2 - x1); x = xMin; }
            if (out == code1) { x1 = x; y1 = y; code1 = getCode(x1, y1, xMin, xMax, yMin, yMax); }
            else { x2 = x; y2 = y; code2 = getCode(x2, y2, xMin, xMax, yMin, yMax); }
        }
    }
    if (accept) drawLineDDA(x1, y1, x2, y2);
}

void drawText(float x, float y, std::string s, void* font) {
    glRasterPos2f(x, y); for (char c : s) glutBitmapCharacter(font, c);
}