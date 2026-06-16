#define GLUT_DISABLE_ATEXIT_HACK
#include "algorithms.h"
#include <GL/glut.h>
#include <cmath>

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);  //tell OpenGL we are drawing points
    glVertex2i(x, y); //place the dot here
    glEnd();
}

void drawLineDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1, dy = y2 - y1;
    float steps = std::max(abs(dx), abs(dy));
    float xInc = dx / steps, yInc = dy / steps;
    float x = x1, y = y1;
    for (int i = 0; i <= steps; i++) {
            drawPixel(round(x), round(y));
            x += xInc; y += yInc;
    }
}

void drawLineBres(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (true) {
        drawPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break; //plot current pixel & stop when end point reached
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; } //decides whether to move in X direction
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

#include <queue>
#include <vector>

void boundaryFill4(int x, int y, float* borderColor, float* fillColor)
{
    std::queue<std::pair<int, int>> q;
    q.push({x, y});

    // To prevent stack overflow and incomplete fills, use an iterative queue
    // and keep track of visited pixels. We use a locally allocated array.
    std::vector<std::vector<bool>> visited(501, std::vector<bool>(601, false));

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        int cx = current.first;
        int cy = current.second;
        q.pop();

        if (cx < 0 || cx > 500 || cy < 0 || cy > 600) continue;
        if (visited[cx][cy]) continue;
        visited[cx][cy] = true;

        float currentColor[3];
        glReadPixels(cx, cy, 1, 1, GL_RGB, GL_FLOAT, currentColor);

        bool isBoundary =
            (std::abs(currentColor[0] - borderColor[0]) < 0.05f &&
             std::abs(currentColor[1] - borderColor[1]) < 0.05f &&
             std::abs(currentColor[2] - borderColor[2]) < 0.05f);

        bool isAlreadyFilled =
            (std::abs(currentColor[0] - fillColor[0]) < 0.05f &&
             std::abs(currentColor[1] - fillColor[1]) < 0.05f &&
             std::abs(currentColor[2] - fillColor[2]) < 0.05f);

        if (!isBoundary && !isAlreadyFilled)
        {
            glColor3f(fillColor[0], fillColor[1], fillColor[2]);
            drawPixel(cx, cy);

            q.push({cx + 1, cy});
            q.push({cx - 1, cy});
            q.push({cx, cy + 1});
            q.push({cx, cy - 1});
        }
    }
}
