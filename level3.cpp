#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

float rotAngle = 0;

void resetLevel3() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.1, 0.2, 0.1, 1);
    platforms.push_back({0, -250, 80, 0});
    for(int i = 0; i < 10; i++) {
        platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, 0});
        if (i % 2 == 0) obstacles.push_back({platforms.back().x, platforms.back().y + 30, 40, 0});
    }
}

void updateLevel3() {
    rotAngle += 3.0f; // Update rotation
}

void drawLevel3() {
    glColor3f(0.5, 0.5, 0.5);
    for (auto& plat : platforms) drawLineDDA(plat.x - plat.w/2, plat.y, plat.x + plat.w/2, plat.y);
    
    glColor3f(1.0, 0.0, 0.0);
    for (auto& obs : obstacles) {
        glPushMatrix();
        glTranslatef(obs.x, obs.y, 0);
        glRotatef(rotAngle, 0, 0, 1); // ROTATION MATH
        drawLineBres(-obs.w/2, 0, obs.w/2, 0);
        glPopMatrix();
    }
}