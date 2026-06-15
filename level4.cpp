#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

std::vector<int> previousBounces;
int lastLandedPlatform = -1;

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

int getClipCode(float x, float y, float xmin, float xmax, float ymin, float ymax) {
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

bool clipLine(float& x1, float& y1, float& x2, float& y2, float xmin, float xmax, float ymin, float ymax) {
    int code1 = getClipCode(x1, y1, xmin, xmax, ymin, ymax);
    int code2 = getClipCode(x2, y2, xmin, xmax, ymin, ymax);

    while (true) {
        if ((code1 | code2) == 0) return true;
        if ((code1 & code2) != 0) return false;

        int codeOut = code1 ? code1 : code2;
        float x, y;

        if (codeOut & TOP) {
            x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
            y = ymax;
        } else if (codeOut & BOTTOM) {
            x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
            y = ymin;
        } else if (codeOut & RIGHT) {
            y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
            x = xmax;
        } else {
            y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
            x = xmin;
        }

        if (codeOut == code1) {
            x1 = x; y1 = y;
            code1 = getClipCode(x1, y1, xmin, xmax, ymin, ymax);
        } else {
            x2 = x; y2 = y;
            code2 = getClipCode(x2, y2, xmin, xmax, ymin, ymax);
        }
    }
}

void drawClippedLine(float x1, float y1, float x2, float y2) {
    float xmin = -230, xmax = 230;
    float ymin = cameraY - 290, ymax = cameraY + 290;
    if (clipLine(x1, y1, x2, y2, xmin, xmax, ymin, ymax)) {
        drawLineDDA((int)x1, (int)y1, (int)x2, (int)y2);
    }
}

void resetLevel4() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.0, 0.0, 0.1, 1);
    platforms.push_back({0, -250, 80, 0, 0});
    for(int i = 0; i < 10; i++) platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, 0, 0});

    previousBounces.clear();
    for (auto& plat : platforms) previousBounces.push_back(plat.bounces);
    lastLandedPlatform = -1;
}

void updateLevel4() {
    if (previousBounces.size() != platforms.size()) {
        previousBounces.clear();
        for (auto& plat : platforms) previousBounces.push_back(plat.bounces);
        return;
    }

    int landedPlatform = -1;
    for (int i = 0; i < platforms.size(); i++) {
        if (platforms[i].bounces > previousBounces[i]) landedPlatform = i;
    }

    if (landedPlatform != -1 && landedPlatform != lastLandedPlatform) {
        for (int i = 0; i < platforms.size(); i++) {
            if (i != landedPlatform) platforms[i].x = -platforms[i].x;
        }
    }

    if (landedPlatform != -1) lastLandedPlatform = landedPlatform;
    for (int i = 0; i < platforms.size(); i++) previousBounces[i] = platforms[i].bounces;
}

void drawLevel4() {
    glColor3f(0.0, 1.0, 1.0);
    for (auto& plat : platforms) {
        drawCircleBres(plat.x, plat.y, 5);
        drawClippedLine(plat.x - plat.w/2, plat.y, plat.x + plat.w/2, plat.y);
    }
}
