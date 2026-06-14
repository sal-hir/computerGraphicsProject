#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>
#include <cstdlib>

void resetLevel1() {
    gravity = 0.4; jumpStrength = 12.0;
    glClearColor(0.02, 0.02, 0.15, 1);  // Space dark background

    platforms.push_back({0, -250, 80, 2, 0});

     for(int i = 0; i < 10; i++) {
        float vx;
        do { vx = (float)(rand()%5 - 2); } while(vx == 0);
        platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, vx, 0});
    }

// Initial planets
    planets.clear();
    for(int i = 0; i < 3; i++) {
        Planet p;
        p.x = (float)(rand() % 400 - 200);
        p.y = -100.0f + i * 200;
        p.radius = 15.0f;
        p.collected = false;
        planets.push_back(p);
    }
}

void updateLevel1() {
    for (auto& plat : platforms) {
        plat.x += plat.vx;
        if (plat.x > 250 || plat.x < -250) plat.vx *= -1;
    }
}

void drawLevel1() {
    // Draw stars
    srand(42);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; i < 80; i++) {
        int sx = rand() % 500 - 250;
        int sy = rand() % 600 - 300;
        drawPixel(sx, sy + (int)cameraY);
    }

    // Draw platforms - cyan glow
    for (auto& plat : platforms) {
        glColor3f(0.0, 1.0, 1.0);
        drawLineDDA(plat.x - plat.w/2, plat.y, plat.x + plat.w/2, plat.y);
        glColor3f(0.0, 0.5, 0.5);
        drawLineDDA(plat.x - plat.w/2, plat.y - 1, plat.x + plat.w/2, plat.y - 1);
    }
}
