#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>
#include <cstdlib>

void resetLevel2() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.2, 0.1, 0.3, 1);
    platforms.push_back({0, -250, 100, 0, 0});
    for(int i = 0; i < 10; i++) platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 100, 0, 0});
}

void updateLevel2() {
    // Width shrinking handled mathematically in draw based on plat.bounces
}

void drawLevel2() {
    for (auto& plat : platforms) {
        if (plat.bounces >= 3) continue; // It vanished!

        float currentWidth = plat.w * (1.0f - (plat.bounces * 0.33f)); // Shrink by 33% each bounce

        if (plat.bounces == 0) glColor3f(0, 1, 0); // Green (Healthy)
        else if (plat.bounces == 1) glColor3f(1, 1, 0); // Yellow (Warning)
        else glColor3f(1, 0, 0); // Red (One jump left)

        glPushMatrix();
        glTranslatef(plat.x, plat.y, 0);
        glScalef(currentWidth / 100.0f, 1.0, 1.0);
        drawLineBres(-50, 0, 50, 0);
        glPopMatrix();
    }
}
