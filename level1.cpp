#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>
#include <cstdlib>

void resetLevel1() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.1, 0.1, 0.15, 1);
    platforms.push_back({0, -250, 80, 0, 0});
    for(int i = 0; i < 10; i++) platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, (float)(rand()%5 - 2), 0});
}

void updateLevel1() {
    for (auto& plat : platforms) {
        plat.x += plat.vx;
        if (plat.x > 250 || plat.x < -250) plat.vx *= -1;
    }
}

void drawLevel1() {
    glColor3f(0.2, 0.8, 0.2);
    for (auto& plat : platforms) drawLineDDA(plat.x - plat.w/2, plat.y, plat.x + plat.w/2, plat.y);
}
