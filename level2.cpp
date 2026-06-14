#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

void resetLevel2() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.2, 0.1, 0.3, 1);
    platforms.push_back({0, -250, 150, 0}); // Start with huge platforms
    for(int i = 0; i < 10; i++) platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 150, 0});
}

void updateLevel2() {
    // Scaling logic handled in main physics loop (if player bounces, platform.w *= 0.8)
}

void drawLevel2() {
    glColor3f(1.0, 0.5, 0.0);
    for (auto& plat : platforms) {
        glPushMatrix();
        glTranslatef(plat.x, plat.y, 0);
        glScalef(plat.w / 100.0f, 1.0, 1.0); // SCALING MATH
        drawLineBres(-50, 0, 50, 0); 
        glPopMatrix();
    }
}