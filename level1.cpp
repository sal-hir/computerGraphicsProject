#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

void resetLevel1() {
    gravity = 0.4; jumpStrength = 12.0;
    glClearColor(0.02, 0.02, 0.15, 1);
    platforms.push_back({0, -250, 80, 2, 0});
    for(int i = 0; i < 10; i++) {
        float vx;
        do { vx = (float)(rand()%5 - 2); } while(vx == 0);
        platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, vx, 0});
    }
    planets.clear();
    for(int i = 0; i < 3; i++) {
        Planet p;
        p.x = (float)(rand() % 400 - 200);
        p.y = -100.0f + i * 200;
        p.radius = 15.0f;
        p.baseRadius = 15.0f;   // store original size
        p.pulseTimer = 0.0f;    // pulse starts at 0
        p.collected = false;
        planets.push_back(p);
    }
}

void updateLevel1() {
    // Move platforms - reflection
    for (auto& plat : platforms) {
        plat.x += plat.vx;
        if (plat.x > 250 || plat.x < -250) plat.vx *= -1;
    }
    // Pulse planets - scaling
    for (auto& planet : planets) {
        planet.pulseTimer += 0.05f;
        // radius moves between (baseRadius - 5) and (baseRadius + 5)
        planet.radius = planet.baseRadius + sin(planet.pulseTimer) * 5.0f;
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
    // Draw planets using Bresenham Circle
    for (auto& planet : planets) {
        if (!planet.collected) {
            glColor3f(0.8, 0.3, 0.1);  // red outer ring
            drawCircleBres(planet.x, planet.y, (int)planet.radius);
            glColor3f(0.5, 0.7, 1.0);  // blue inner ring
            drawCircleBres(planet.x, planet.y, (int)(planet.radius - 5));
        }
    }
}
