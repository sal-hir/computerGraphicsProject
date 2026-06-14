#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

bool wasMirrored = false;

void resetLevel4() {
    gravity = 0.4; jumpStrength = 12.0; glClearColor(0.0, 0.0, 0.1, 1);
    platforms.push_back({0, -250, 80, 0, 0});
    for(int i = 0; i < 10; i++) platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, 0, 0});
    wasMirrored = false;
}

void updateLevel4() {
    bool shouldBeMirrored = ((jumpCount / 2) % 2 != 0);

    // Only flip physical coordinates when the state changes
    if (shouldBeMirrored != wasMirrored) {
        for (auto& plat : platforms) {
            plat.x = -plat.x; // PHYSICAL FLIP: Physics now matches Visuals
        }
        wasMirrored = shouldBeMirrored;
    }
}

void drawLevel4() {
    // We do NOT use glScalef here anymore.
    // The physics are already flipped, so we just draw them at their new coordinates.
    glColor3f(0.0, 1.0, 1.0);
    for (auto& plat : platforms) {
        drawCircleBres(plat.x, plat.y, 5);
        drawLineDDA(plat.x - plat.w/2, plat.y, plat.x + plat.w/2, plat.y);
    }
}
