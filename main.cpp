#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include "globals.h"
#include "algorithms.h"
#include "levels.h"

int gameState = 0, subState = 0, stateTimer = 0, score = 0, jumpCount = 0;
float cameraY = 0, pX = 0, pY = 0, pVy = 0, gravity = 0.4, jumpStrength = 12.0;
bool moveLeft = false, moveRight = false;
std::vector<Platform> platforms; std::vector<Obstacle> obstacles;

void initLevel(int level) {
    gameState = level; subState = 0; stateTimer = 60; // 1-second intro
    pX = 0; pY = -200; pVy = 12; cameraY = -300; score = 0; jumpCount = 0;
    platforms.clear(); obstacles.clear();
    if(level==1) resetLevel1(); else if(level==2) resetLevel2(); else if(level==3) resetLevel3(); else if(level==4) resetLevel4();
}

void handlePhysicsAndCamera() {
    pVy -= gravity; pY += pVy;

    // Level 4 Input Reversal check
    bool flipInput = (gameState == 4 && (jumpCount / 2) % 2 != 0);
    if ((moveLeft && !flipInput) || (moveRight && flipInput)) pX -= 5;
    if ((moveRight && !flipInput) || (moveLeft && flipInput)) pX += 5;

    if (pX < -250) pX = 250; if (pX > 250) pX = -250;

    for (auto& plat : platforms) {
        float effWidth = (gameState == 2) ? plat.w * (1.0f - (plat.bounces * 0.33f)) : plat.w;
        // Collision Detection
        if (pVy < 0 && plat.bounces < 3 && pY - 15 <= plat.y && pY - 15 >= plat.y - 15 && pX >= plat.x - effWidth/2 && pX <= plat.x + effWidth/2) {
            pVy = jumpStrength;
            plat.bounces++;
            jumpCount++;
        }
    }

    if (pY > cameraY + 100) { cameraY = pY - 100; score = (int)cameraY + 300; }

    // Generator
    if (platforms.back().y < cameraY + 400) {
        platforms.push_back({(float)(rand() % 400 - 200), platforms.back().y + (rand() % 40 + 50), (gameState==2)?100.0f:60.0f, (gameState==1)?(float)(rand()%5-2):0, 0});
        if (gameState == 3 && rand() % 100 < 50) obstacles.push_back({platforms.back().x, platforms.back().y + 40, 60, 0});
    }

    if (platforms.front().y < cameraY - 350) platforms.erase(platforms.begin());
    if (pY < cameraY - 300) { subState = 2; stateTimer = 0; } // Trigger Game Over Intro
}

void drawPlayer() {
    glColor3f(1, 1, 1); glPushMatrix(); glTranslatef(pX, pY, 0);
    drawLineDDA(-10, -10, 10, -10); drawLineDDA(10, -10, 10, 10);
    drawLineDDA(10, 10, -10, 10); drawLineDDA(-10, 10, -10, -10);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); glLoadIdentity();
    if (gameState == 0) drawMenu();
    else if (subState == 0) drawIntro();
    else if (subState == 2) drawOutro();
    else {
        glPushMatrix(); glTranslatef(0, -cameraY, 0);
        if (gameState == 1) drawLevel1(); else if (gameState == 2) drawLevel2(); else if (gameState == 3) drawLevel3(); else if (gameState == 4) drawLevel4();
        drawPlayer();
        glPopMatrix();
        glColor3f(1, 1, 1); drawText(-230, 270, "Score: " + std::to_string(score) + " | Jumps: " + std::to_string(jumpCount), GLUT_BITMAP_9_BY_15);
    }
    glutSwapBuffers();
}

void update(int value) {
    if (gameState >= 1 && gameState <= 4) {
        if (subState == 0) { stateTimer--; if (stateTimer <= 0) subState = 1; } // Intro finish
        else if (subState == 1) { // Playing
            handlePhysicsAndCamera();
            if (gameState == 1) updateLevel1(); else if (gameState == 2) updateLevel2(); else if (gameState == 3) updateLevel3(); else if (gameState == 4) updateLevel4();
        }
        else if (subState == 2) { stateTimer++; } // Outro falling text
    }
    glutPostRedisplay(); glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (gameState == 0 && key >= '1' && key <= '4') initLevel(key - '0');
    if (subState == 2 && (key == 'm' || key == 'M')) { gameState = 0; subState = 0; glClearColor(0,0,0,1); }
    if (key == 27) exit(0);
}
void specialDown(int key, int x, int y) { if (key == GLUT_KEY_LEFT) moveLeft = true; if (key == GLUT_KEY_RIGHT) moveRight = true; }
void specialUp(int key, int x, int y) { if (key == GLUT_KEY_LEFT) moveLeft = false; if (key == GLUT_KEY_RIGHT) moveRight = false; }

int main(int argc, char** argv) {
    srand(time(0)); glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); glutInitWindowSize(500, 600);
    glutCreateWindow("Algo Jump - Final Edition");
    glMatrixMode(GL_PROJECTION); gluOrtho2D(-250, 250, -300, 300); glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(display); glutKeyboardFunc(keyboard); glutSpecialFunc(specialDown); glutSpecialUpFunc(specialUp);
    glutTimerFunc(16, update, 0); glutMainLoop(); return 0;
}
