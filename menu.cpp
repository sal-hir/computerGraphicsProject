#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

void drawMenu() {
    glColor3f(1, 1, 1);
    drawText(-120, 150, "ALGO JUMP!", GLUT_BITMAP_TIMES_ROMAN_24);
    drawText(-150, 50, "1: Moving World (Translation)", GLUT_BITMAP_9_BY_15);
    drawText(-150, 20, "2: Shrinking Steps (Scaling)", GLUT_BITMAP_9_BY_15);
    drawText(-150, -10, "3: Swinging Rods (Rotation)", GLUT_BITMAP_9_BY_15);
    drawText(-150, -40, "4: Mirrored Dimension (Reflection)", GLUT_BITMAP_9_BY_15);
}

void drawGameOver() {
    glColor3f(1, 0, 0); drawText(-80, 50, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1, 1, 1); drawText(-50, 10, "Score: " + std::to_string(score), GLUT_BITMAP_9_BY_15); 
    drawText(-80, -30, "Press M for Menu", GLUT_BITMAP_9_BY_15);
}