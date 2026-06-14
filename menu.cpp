#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

void drawMenu() {
    glColor3f(1, 1, 1);
    drawText(-120, 150, "Pixel-Leap", GLUT_BITMAP_TIMES_ROMAN_24);
    drawText(-150, 50, "1: Moving Platforms", GLUT_BITMAP_9_BY_15);
    drawText(-150, 20, "2: 3-Step Shrink", GLUT_BITMAP_9_BY_15);
    drawText(-150, -10, "3: Boost/Slam Rods", GLUT_BITMAP_9_BY_15);
    drawText(-150, -40, "4: 2-Jump Mirror", GLUT_BITMAP_9_BY_15);
}

void drawIntro() {
    float offset = (60 - stateTimer) * 5; // Slides text in
    glColor3f(0, 1, 0);
    drawText(-100 + offset, 0, "LEVEL " + std::to_string(gameState) + " STARTING", GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawOutro() {
    float fall = stateTimer * 3; // Text falls down
    glColor3f(1, 0, 0);
    drawText(-80, 100 - fall, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1, 1, 1);
    drawText(-50, 50 - fall, "Score: " + std::to_string(score), GLUT_BITMAP_9_BY_15);
    drawText(-100, 0 - fall, "Press M to return to Menu", GLUT_BITMAP_9_BY_15);
}
