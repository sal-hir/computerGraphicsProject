#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include "globals.h"
#include "algorithms.h"
#include "levels.h"

// Define globals
int gameState = 0; float cameraY = 0; int score = 0;
float pX = 0, pY = 0, pVy = 0, gravity = 0.4, jumpStrength = 12.0;
bool moveLeft = false, moveRight = false;
std::vector<Platform> platforms; std::vector<Obstacle> obstacles;

void handlePhysicsAndCamera() {
    pVy -= gravity; pY += pVy;
    if (moveLeft) pX -= 5; if (moveRight) pX += 5;
    if (pX < -250) pX = 250; if (pX > 250) pX = -250;

    for (auto& plat : platforms) {
        if (pVy < 0 && pY - 15 <= plat.y && pY - 15 >= plat.y - 15 && pX >= plat.x - plat.w/2 && pX <= plat.x + plat.w/2) {
            pVy = jumpStrength;
            if (gameState == 2) plat.w *= 0.8; // Shrink logic for Level 2
        }
    }
    if (pY > cameraY + 100) { cameraY = pY - 100; score = (int)cameraY + 300; }
    
    // Platform Generation
    if (platforms.back().y < cameraY + 400) {
        platforms.push_back({(float)(rand() % 400 - 200), platforms.back().y + (rand() % 40 + 50), (gameState==2)?150.0f:60.0f, (gameState==1)?(float)(rand()%5-2):0});
        if (gameState == 3 && rand() % 100 < 50) obstacles.push_back({platforms.back().x, platforms.back().y + 30, 40, 0});
    }
    
    if (platforms.front().y < cameraY - 350) platforms.erase(platforms.begin());
    if (pY < cameraY - 300) gameState = 5; // Game Over
}

void drawPlayer() {
    glColor3f(1, 1, 1);
    glPushMatrix(); glTranslatef(pX, pY, 0);
    drawLineDDA(-10, -10, 10, -10); drawLineDDA(10, -10, 10, 10);
    drawLineDDA(10, 10, -10, 10); drawLineDDA(-10, 10, -10, -10);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); glLoadIdentity();
    if (gameState == 0) drawMenu();
    else if (gameState == 5) drawGameOver();
    else {
        glPushMatrix(); glTranslatef(0, -cameraY, 0);
        
        if (gameState == 1) drawLevel1();
        else if (gameState == 2) drawLevel2();
        else if (gameState == 3) drawLevel3();
        else if (gameState == 4) drawLevel4();

        drawPlayer();
        glPopMatrix();
        
        glColor3f(1, 1, 1); drawText(-230, 270, "Score: " + std::to_string(score), GLUT_BITMAP_9_BY_15);
    }
    glutSwapBuffers();
}

void update(int value) {
    if (gameState >= 1 && gameState <= 4) {
        handlePhysicsAndCamera();
        if (gameState == 1) updateLevel1();
        else if (gameState == 2) updateLevel2();
        else if (gameState == 3) updateLevel3();
        else if (gameState == 4) updateLevel4();
    }
    glutPostRedisplay(); glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (gameState == 0 && key >= '1' && key <= '4') { 
        gameState = key - '0'; pX=0; pY=-200; pVy=12; cameraY=-300; score=0; platforms.clear(); obstacles.clear();
        if (gameState == 1) resetLevel1(); else if (gameState == 2) resetLevel2(); else if (gameState == 3) resetLevel3(); else if (gameState == 4) resetLevel4();
    }
    if (gameState == 5 && (key == 'm' || key == 'M')) gameState = 0;
    if (key == 27) exit(0);
}
void specialDown(int key, int x, int y) { if (key == GLUT_KEY_LEFT) moveLeft = true; if (key == GLUT_KEY_RIGHT) moveRight = true; }
void specialUp(int key, int x, int y) { if (key == GLUT_KEY_LEFT) moveLeft = false; if (key == GLUT_KEY_RIGHT) moveRight = false; }

int main(int argc, char** argv) {
    srand(time(0)); glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); glutInitWindowSize(500, 600);
    glutCreateWindow("Algo Jump - Modular Git Version");
    glMatrixMode(GL_PROJECTION); gluOrtho2D(-250, 250, -300, 300); glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(display); glutKeyboardFunc(keyboard); glutSpecialFunc(specialDown); glutSpecialUpFunc(specialUp);
    glutTimerFunc(16, update, 0); glutMainLoop(); return 0;
}