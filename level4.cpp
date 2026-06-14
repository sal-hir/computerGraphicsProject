#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>
#include <cstdlib>

void resetLevel4() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.0, 0.0, 0.1, 1);
    platforms.push_back({0, -250, 80, 0, 0});
    for(int i = 0; i < 10; i++) platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, 0, 0});
}

void updateLevel4() {
    // Physical mirroring of keyboard inputs handled in main.cpp based on jumpCount
}

void drawLevel4() {
    bool isMirrored = (jumpCount / 2) % 2 != 0;

    glPushMatrix();
    if (isMirrored) {
        glScalef(-1, 1, 1); // Flip the entire scene visually
        glColor3f(1.0, 0.0, 1.0); // Purple when mirrored
    } else {
        glColor3f(0.0, 1.0, 1.0); // Cyan when normal
    }

    for (auto& plat : platforms) {
        drawCircleBres(plat.x, plat.y, 5);
        drawLineDDA(plat.x - plat.w/2, plat.y, plat.x + plat.w/2, plat.y);
    }
    glPopMatrix();
}
