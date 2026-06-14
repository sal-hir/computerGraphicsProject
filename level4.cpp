#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

void resetLevel4() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.0, 0.0, 0.0, 1);
    platforms.push_back({0, -250, 80, 0});
    for(int i = 0; i < 10; i++) platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, 0});
}

void updateLevel4() {
    // Standard static logic
}

void drawLevel4() {
    glColor3f(0.0, 1.0, 1.0);
    bool isMirrored = ((score / 500) % 2 != 0); // Flips every 500 points

    glPushMatrix();
    if (isMirrored) glScalef(-1, 1, 1); // REFLECTION MATH across Y-axis
    
    for (auto& plat : platforms) {
        drawCircleBres(plat.x, plat.y, 5); // Draw circles for platforms
        drawLineDDA(plat.x - plat.w/2, plat.y, plat.x + plat.w/2, plat.y);
    }
    glPopMatrix();
}