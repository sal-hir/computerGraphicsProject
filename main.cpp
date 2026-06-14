#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "globals.h"
#include "algorithms.h"
#include "levels.h"

//gamestate (0 menu, 1-4 levels) substate (0 intro, 1 playing, 2 game over)
int gameState = 0,
subState = 0,
stateTimer = 0,
score = 0,
jumpCount = 0;
//camera position, player positions, player velocity, gravity(how quickly come down), jump_strength(how high)
float cameraY = 0,
 pX = 0,
  pY = 0,
  pVy = 0,
  gravity = 0.4,
  jumpStrength = 12.0;
//left right movements
bool moveLeft = false, moveRight = false;

std::vector<Platform> platforms; std::vector<Obstacle> obstacles;
std::vector<Planet> planets;  //level 1 planets

void initLevel(int level) {
    gameState = level;
    subState = 0;
    stateTimer = 120; // 2 second intro
    pX = 0;
    pY = -200;
    pVy = 12;
    cameraY = -300;  //starts in the bottom
    score = 0;
    jumpCount = 0;
    platforms.clear(); obstacles.clear(); //clear out arrays
    if(level==1)
        resetLevel1();
    else if(level==2)
        resetLevel2();
    else if(level==3)
        resetLevel3();
    else if(level==4)
        resetLevel4();
}

void handlePhysicsAndCamera() {
    pVy -= gravity; //track velocity
    pY += pVy; //update base on velocity

    //left right movements
    if (moveLeft)
        pX -= 5;
    if (moveRight)
        pX += 5;
//loop around if character goes off screen
    if (pX < -250)
        pX = 250;
    if (pX > 250)
        pX = -250;

    for (auto& plat : platforms) {
            //platform width (width changes for level 3)
        float effWidth = (gameState == 2) ? plat.w * (1.0f - (plat.bounces * 0.33f)) : plat.w;
        // Collision Detection
        if (pVy < 0 && pY - 15 <= plat.y && pY - 15 >= plat.y - 15 && pX >= plat.x - effWidth/2 && pX <= plat.x + effWidth/2) {
            pVy = jumpStrength;
            plat.bounces++;
            jumpCount++;
        }
    }

     // Planet collision - Level 1 only
    if (gameState == 1) {
        for (auto& planet : planets) {
            if (!planet.collected) {
                float dx = pX - planet.x;
                float dy = pY - planet.y;
                float dist = sqrt(dx*dx + dy*dy);
                if (dist < planet.radius + 10) {
                    planet.collected = true;
                    score += 50;
                }
            }
        }

        // Spawn new planets
        if (rand() % 100 < 5) {
            Planet p;
            p.x = (float)(rand() % 400 - 200);
            p.y = platforms.back().y + rand() % 100;
            p.radius = 15.0f;
            p.collected = false;
            planets.push_back(p);
        }

        // Clean old planets
        planets.erase(
            std::remove_if(planets.begin(), planets.end(),
                [&](const Planet& p) { return p.y < cameraY - 350; }),
            planets.end()
        );
    }

    if (pY > cameraY + 100)
        {
            cameraY = pY - 100; //move camera
            score = (int)cameraY + 300;
        }

    // Generator
    if (platforms.back().y < cameraY + 400) {
        platforms.push_back({(float)(rand() % 400 - 200), platforms.back().y + (rand() % 40 + 50), (gameState==2)?100.0f:60.0f, (gameState==1)?(float)(rand()%5-2):0, 0});
        if (gameState == 3 && rand() % 100 < 50) obstacles.push_back({platforms.back().x, platforms.back().y + 40, 60, 0});
    }

    if (platforms.front().y < cameraY - 350) platforms.erase(platforms.begin());
    if (pY < cameraY - 300) { subState = 2; stateTimer = 0; } // Trigger Game Over Intro
}

void drawPlayer(int level) {

    if(level == 1){

    glColor3f(1, 1, 1); glPushMatrix(); glTranslatef(pX, pY, 0);
    drawLineDDA(-10, -10, 10, -10); drawLineDDA(10, -10, 10, 10);
    drawLineDDA(10, 10, -10, 10); drawLineDDA(-10, 10, -10, -10);
    glPopMatrix();

    }

      if(level == 2){

    glColor3f(0.4, 0.7, 1); glPushMatrix(); glTranslatef(pX, pY, 0);
    drawLineDDA(-10, -10, 10, -10); drawLineDDA(10, -10, 10, 10);
    drawLineDDA(10, 10, -10, 10); drawLineDDA(-10, 10, -10, -10);
    glPopMatrix();

    }

      if(level == 3){

    glColor3f(1, 0, 0.8); glPushMatrix(); glTranslatef(pX, pY, 0);
    drawLineDDA(-10, -10, 10, -10); drawLineDDA(10, -10, 10, 10);
    drawLineDDA(10, 10, -10, 10); drawLineDDA(-10, 10, -10, -10);
    glPopMatrix();

    }

      if(level == 4){

    glColor3f(0.3, 0.7, 0); glPushMatrix(); glTranslatef(pX, pY, 0);
    drawLineDDA(-10, -10, 10, -10); drawLineDDA(10, -10, 10, 10);
    drawLineDDA(10, 10, -10, 10); drawLineDDA(-10, 10, -10, -10);
    glPopMatrix();

    }

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); glLoadIdentity();
    if (gameState == 0) drawMenu();
    else if (subState == 0) drawIntro();
    else if (subState == 2) drawOutro();
    else {
        glPushMatrix(); glTranslatef(0, -cameraY, 0);
        if (gameState == 1) drawLevel1(); else if (gameState == 2) drawLevel2(); else if (gameState == 3) drawLevel3(); else if (gameState == 4) drawLevel4();

         // Draw planets - Level 1 only
        if (gameState == 1) {
            for (auto& planet : planets) {
                if (!planet.collected) {
                    glColor3f(0.8, 0.3, 0.1); // Red outer
                    for(int a = 0; a < 360; a++) {
                        float rad = a * 3.14159f / 180;
                        drawPixel(planet.x + cos(rad) * planet.radius,
                                 planet.y + sin(rad) * planet.radius);
                    }
                    glColor3f(0.5, 0.7, 1.0); // Blue inner
                    for(int a = 0; a < 360; a++) {
                        float rad = a * 3.14159f / 180;
                        drawPixel(planet.x + cos(rad) * (planet.radius - 5),
                                 planet.y + sin(rad) * (planet.radius - 5));
                    }
                }
            }
        }

        drawPlayer(gameState);
        glPopMatrix();
        glColor3f(1, 1, 1); drawText(-230, 270, "Score: " + std::to_string(score) + " | Jumps: " + std::to_string(jumpCount), GLUT_BITMAP_9_BY_15);
        if (gameState == 1)
            drawText(-230, 250, "Collect planets: +50!", GLUT_BITMAP_8_BY_13);
    }
    glutSwapBuffers(); //for double buffering
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

//level selection and reset
void keyboard(unsigned char key, int x, int y) {
    if (gameState == 0 && key >= '1' && key <= '4')
        initLevel(key - '0'); //to get int value from ascii
    if (subState == 2 && (key == 'm' || key == 'M'))
    {
        gameState = 0;
        subState = 0;
        glClearColor(0,0,0,1);
    }
    if (key == 27) //esc button kills the game
        exit(0);
}
void specialDown(int key, int x, int y) //clicking down arrow keys
{
    if (key == GLUT_KEY_LEFT)
        moveLeft = true;
    if (key == GLUT_KEY_RIGHT)
        moveRight = true;
}
void specialUp(int key, int x, int y) //releasing arrow keys
{
    if (key == GLUT_KEY_LEFT)
        moveLeft = false;
    if (key == GLUT_KEY_RIGHT)
        moveRight = false;
}

int main(int argc, char** argv) {
    srand(time(0));  //random number generator
    glutInit(&argc, argv); //initilise GLUT library
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //configure colours and double buffer
    glutInitWindowSize(500, 600); //window size
    glutCreateWindow(" •• Pixel Leap •• ");
    glClearColor(0.0f, 0.0f, 0.2f, 0.4f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-250, 250, -300, 300); //coordinate plane
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(display); //the display function to run on refreshes
    glutKeyboardFunc(keyboard); //keyboard function to track key presses
    glutSpecialFunc(specialDown); //special key press
    glutSpecialUpFunc(specialUp);//special key release
    glutTimerFunc(16, update, 0); //redraw game evry 16 ms
    glutMainLoop(); //loop the window over and over
    return 0;
}
