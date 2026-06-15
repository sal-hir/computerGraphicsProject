#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>

void drawMenu() {
    glColor3f(1, 1, 1);
    drawText(-120, 150, "Pixel Leap!", GLUT_BITMAP_TIMES_ROMAN_24);
    drawText(-150, 50, "1: Moving Platforms", GLUT_BITMAP_9_BY_15);
    drawText(-150, 20, "2: Crumble Rush", GLUT_BITMAP_9_BY_15);
    drawText(-150, -10, "3: Boost/Slam Rods", GLUT_BITMAP_9_BY_15);
    drawText(-150, -40, "4: 2-Jump Mirror", GLUT_BITMAP_9_BY_15);
}

void drawIntro() {
    glColor3f(0.0f, 0.8f, 0.0f); // Matrix green color for sparks
    glPointSize(2.0f);            // Make the dots visible
    // Draw the level text
    glColor3f(0, 1, 0);
    drawText(-100, 0, "LEVEL " + std::to_string(gameState) + " STARTING", GLUT_BITMAP_TIMES_ROMAN_24);

    // Loop across the screen from left to right in steps of 20 pixels
    for (int x = -240; x <= 240; x += 20) {
        // set vertical column  starting height offset
        int heightOffset = (x % 7) * 60;
    // Calculate radius (starts small, maxes out at 35 to prevent crashes)
    float progress = (120.0f - stateTimer) / 120.0f;
    int r = 2 + (int)(progress * 33);

         //  measure from -300 (bottom of screen) upward for y value
        float sparkY = -300 + (((60 - stateTimer) * 4 + heightOffset) % 600);
        
    // 1. Draw 3 growing circle outlines
    glColor3f(0.0f, 1.0f, 0.0f);
    drawCircleBres(0, -50, r);
    drawCircleBres(0, -50, r * 2); // Second circle
    drawCircleBres(0, -50, r * 3); // Third circle
    glFlush();
    }
    glPointSize(1.0f); // Reset pixel point size back to default

    glColor3f(0, 1, 0); // Bright Green
    drawText(-100, 0, "LEVEL " + std::to_string(gameState) + " STARTING", GLUT_BITMAP_TIMES_ROMAN_24);
}
void drawOutro() {
     glColor3f(0.8f, 0.8f, 0.5f); //star color
    glPointSize(2.0f);            // Make the dots visible

    // Loop across the screen from left to right in steps of 40 pixels
    for (int x = -240; x <= 240; x += 40) {
        // Give each vertical column a unique starting height offset
        int heightOffset = (x % 7) * 80;

        // Calculate a downward falling position
        float starY = 300 - ((stateTimer * 4 + heightOffset) % 600);

        // Draw the background dot
        drawPixel(x, (int)starY);
    }
    glPointSize(1.0f); // Reset pixel point size back to default

    //game over text
    glColor3f(1, 0, 0); // Red
    drawText(-80, 100, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1, 1, 1); // White
    drawText(-50, 50, "Score: " + std::to_string(score), GLUT_BITMAP_9_BY_15);
    drawText(-100, 0, "Press M to return to Menu", GLUT_BITMAP_9_BY_15);
}
