#include <GL/glut.h>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// --- GAME STATES & GLOBALS ---
int gameState = 0; // 0=Menu, 1-5=Levels, 6=GameOver
float cameraY = 0;
int score = 0;

// Player Variables
float pX = 0, pY = 0, pVy = 0;
float gravity = 0.4, jumpStrength = 12.0;
bool moveLeft = false, moveRight = false;

// Hyperspeed Variables
int lastHyperTime = -60000; // Initialize so it's ready immediately
const int HYPER_COOLDOWN = 60000; // 60 seconds in milliseconds

// Level Structures
struct Platform { float x, y, w, vx; };
struct Obstacle { float x, y, w, vx; };
std::vector<Platform> platforms;
std::vector<Obstacle> obstacles;

// --- UTILITY ALGORITHMS ---
void drawPixel(int x, int y) { glBegin(GL_POINTS); glVertex2i(x, y); glEnd(); }

void drawLineDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1, dy = y2 - y1;
    float steps = std::max(abs(dx), abs(dy));
    float xInc = dx / steps, yInc = dy / steps;
    float x = x1, y = y1;
    for (int i = 0; i <= steps; i++) { drawPixel(round(x), round(y)); x += xInc; y += yInc; }
}

void drawLineBres(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1), sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1, err = dx - dy;
    while (true) {
        drawPixel(x1, y1); if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void drawCircleBres(int cx, int cy, int r) {
    int x = 0, y = r, d = 3 - 2 * r;
    while (y >= x) {
        drawPixel(cx+x, cy+y); drawPixel(cx-x, cy+y); drawPixel(cx+x, cy-y); drawPixel(cx-x, cy-y);
        drawPixel(cx+y, cy+x); drawPixel(cx-y, cy+x); drawPixel(cx+y, cy-x); drawPixel(cx-y, cy-x);
        x++; if (d > 0) { y--; d = d + 4 * (x - y) + 10; } else d = d + 4 * x + 6;
    }
}

void floodFill(float x, float y, float r, float g, float b) {
    glColor3f(r, g, b); glRectf(x-10, y-10, x+10, y+10);
}

// --- CLIPPING ALGORITHM (For UI Cooldown Bar) ---
const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;
int getCode(double x, double y, double xMin, double xMax, double yMin, double yMax) {
    int code = 0;
    if (x < xMin) code |= LEFT; else if (x > xMax) code |= RIGHT;
    if (y < yMin) code |= BOTTOM; else if (y > yMax) code |= TOP;
    return code;
}
void drawClippedLine(double x1, double y1, double x2, double y2, double xMin, double xMax, double yMin, double yMax) {
    int code1 = getCode(x1, y1, xMin, xMax, yMin, yMax), code2 = getCode(x2, y2, xMin, xMax, yMin, yMax);
    bool accept = false;
    while (true) {
        if ((code1 == 0) && (code2 == 0)) { accept = true; break; }
        else if (code1 & code2) break;
        else {
            int out = code1 ? code1 : code2; double x, y;
            if (out & TOP) { x = x1 + (x2 - x1) * (yMax - y1) / (y2 - y1); y = yMax; }
            else if (out & BOTTOM) { x = x1 + (x2 - x1) * (yMin - y1) / (y2 - y1); y = yMin; }
            else if (out & RIGHT) { y = y1 + (y2 - y1) * (xMax - x1) / (x2 - x1); x = xMax; }
            else if (out & LEFT) { y = y1 + (y2 - y1) * (xMin - x1) / (x2 - x1); x = xMin; }
            if (out == code1) { x1 = x; y1 = y; code1 = getCode(x1, y1, xMin, xMax, yMin, yMax); }
            else { x2 = x; y2 = y; code2 = getCode(x2, y2, xMin, xMax, yMin, yMax); }
        }
    }
    if (accept) drawLineDDA(x1, y1, x2, y2);
}

// --- SHAPES & ENTITIES ---
void drawPlayerShape(int level) {
    glPushMatrix(); glTranslatef(pX, pY, 0);
    if (level == 1) { // Standard Square
        glColor3f(0, 1, 0);
        drawLineDDA(-15, -15, 15, -15); drawLineDDA(15, -15, 15, 15);
        drawLineDDA(15, 15, -15, 15); drawLineDDA(-15, 15, -15, -15);
    } else if (level == 2) { // Moon Jump Triangle
        glColor3f(1, 0.5, 0);
        drawLineBres(-15, -15, 15, -15); drawLineBres(15, -15, 0, 15); drawLineBres(0, 15, -15, -15);
    } else if (level == 3) { // Heavy Circle
        glColor3f(0, 0.5, 1); drawCircleBres(0, 0, 15);
    } else if (level == 4) { // Moving World Diamond
        glColor3f(1, 0, 1);
        drawLineDDA(0, -20, 15, 0); drawLineDDA(15, 0, 0, 20); drawLineDDA(0, 20, -15, 0); drawLineDDA(-15, 0, 0, -20);
        floodFill(0, 0, 1.0, 0.5, 1.0);
    } else if (level == 5) { // Chaos Star
        static float angle = 0; angle += 10;
        glRotatef(angle, 0, 0, 1); glColor3f(1, 1, 0);
        drawLineDDA(-20, 0, 20, 0); drawLineDDA(0, -20, 0, 20);
        drawLineDDA(-15, -15, 15, 15); drawLineDDA(-15, 15, 15, -15);
    }
    glPopMatrix();
}

void drawObstacle(float x, float y, float w) {
    glColor3f(1, 0, 0); // Red Danger
    drawLineBres(x - w, y - w, x + w, y + w);
    drawLineBres(x - w, y + w, x + w, y - w);
}

void drawText(float x, float y, std::string s, void* font = GLUT_BITMAP_9_BY_15) {
    glRasterPos2f(x, y); for (char c : s) glutBitmapCharacter(font, c);
}

// --- GAME LOGIC ---
void resetGame() {
    pX = 0; pY = -200; pVy = 12.0; cameraY = -300; score = 0;
    platforms.clear(); obstacles.clear();

    // Level Modifiers
    if (gameState == 1) { gravity = 0.4; jumpStrength = 12.0; glClearColor(0.1, 0.1, 0.15, 1); }
    else if (gameState == 2) { gravity = 0.2; jumpStrength = 8.0; glClearColor(0.2, 0.1, 0.3, 1); } // Moon physics
    else if (gameState == 3) { gravity = 0.6; jumpStrength = 15.0; glClearColor(0.1, 0.2, 0.1, 1); } // Heavy physics
    else if (gameState == 4) { gravity = 0.4; jumpStrength = 12.0; glClearColor(0.3, 0.1, 0.1, 1); } // Moving platforms
    else if (gameState == 5) { gravity = 0.5; jumpStrength = 14.0; glClearColor(0.0, 0.0, 0.0, 1); } // Fast

    platforms.push_back({0, -250, 80, 0});
    for(int i = 0; i < 10; i++) {
        float vx = (gameState == 4 || gameState == 5) ? (rand()%5 - 2) : 0; // Moving platforms for L4/L5
        platforms.push_back({(float)(rand() % 400 - 200), -100.0f + i * 80, 60, vx});
    }
}

void update(int value) {
    if (gameState >= 1 && gameState <= 5) {
        pVy -= gravity; pY += pVy;
        if (moveLeft) pX -= (gameState == 2) ? 3 : 6; // Slower horizontal on moon
        if (moveRight) pX += (gameState == 2) ? 3 : 6;
        if (pX < -250) pX = 250; if (pX > 250) pX = -250;

        // Platform Physics & Movement
        for (auto& plat : platforms) {
            plat.x += plat.vx;
            if (plat.x > 250 || plat.x < -250) plat.vx *= -1; // Bounce off walls
            if (pVy < 0 && pY - 15 <= plat.y && pY - 15 >= plat.y - 15 && pX >= plat.x - plat.w/2 && pX <= plat.x + plat.w/2) {
                pVy = jumpStrength;
            }
        }

        // Obstacle Physics & Collision
        for (auto& obs : obstacles) {
            obs.x += obs.vx;
            if (obs.x > 250 || obs.x < -250) obs.vx *= -1;
            // AABB Collision with Obstacle -> Game Over
            if (abs(pX - obs.x) < 20 && abs(pY - obs.y) < 20) gameState = 6;
        }

        if (pY > cameraY + 100) { cameraY = pY - 100; score = (int)cameraY + 300; }

        // Generator
        if (platforms.back().y < cameraY + 400) {
            float vx = (gameState == 4 || gameState == 5) ? (rand()%5 - 2) : 0;
            platforms.push_back({(float)(rand() % 400 - 200), platforms.back().y + (rand() % 40 + 50), 60, vx});
            // 20% chance to spawn an obstacle above the platform
            if (rand() % 100 < 20) {
                obstacles.push_back({(float)(rand() % 400 - 200), platforms.back().y + 40, 15, (float)(rand()%4 + 1)});
            }
        }

        // Cleanup off-screen
        if (platforms.front().y < cameraY - 350) platforms.erase(platforms.begin());
        if (!obstacles.empty() && obstacles.front().y < cameraY - 350) obstacles.erase(obstacles.begin());

        if (pY < cameraY - 300) gameState = 6;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// --- RENDERING ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT); glLoadIdentity();

    if (gameState == 0) {
        glColor3f(1, 1, 1);
        drawText(-100, 150, "ALGO JUMP!", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(-150, 50, "1: Basic (DDA)"); drawText(-150, 20, "2: Moon Float (Bresenham)");
        drawText(-150, -10, "3: Heavy (Midpoint)"); drawText(-150, -40, "4: Moving World (FloodFill)");
        drawText(-150, -70, "5: Chaos (Transforms)");
    }
    else if (gameState == 6) {
        glColor3f(1, 0, 0); drawText(-80, 50, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(1, 1, 1); drawText(-50, 10, "Score: " + std::to_string(score)); drawText(-80, -30, "Press M for Menu");
    }
    else {
        glPushMatrix(); glTranslatef(0, -cameraY, 0);

        glColor3f(0.2, 0.8, 0.2); // Platforms
        for (auto& plat : platforms) glRectf(plat.x - plat.w/2, plat.y - 5, plat.x + plat.w/2, plat.y + 5);
        for (auto& obs : obstacles) drawObstacle(obs.x, obs.y, obs.w); // Obstacles

        drawPlayerShape(gameState);
        glPopMatrix();

        // UI Layer
        glColor3f(1, 1, 1);
        drawText(-230, 270, "Score: " + std::to_string(score));

        // UI: Hyperspeed Cooldown Bar (Uses Line Clipping!)
        int timeElapsed = glutGet(GLUT_ELAPSED_TIME) - lastHyperTime;
        float chargePercent = std::min(1.0f, (float)timeElapsed / HYPER_COOLDOWN);

        drawText(50, 270, "HYPER (H):");
        glColor3f(0.3, 0.3, 0.3); // Background bar
        glBegin(GL_LINE_LOOP); glVertex2f(150, 270); glVertex2f(230, 270); glVertex2f(230, 280); glVertex2f(150, 280); glEnd();

        glColor3f(0.0, 1.0, 1.0); // Filled charge (clipping demonstration)
        float fillMaxX = 150 + (80 * chargePercent);
        // Draw 10 vertical lines to act as a "fill", clipped by the bounding box
        for(int i=150; i<230; i+=2) {
            drawClippedLine(i, 260, i, 290, 150, fillMaxX, 270, 280);
        }
        if (chargePercent >= 1.0f) { glColor3f(1, 1, 0); drawText(160, 255, "READY!"); }
    }
    glutSwapBuffers();
}

// --- INPUT ---
void keyboard(unsigned char key, int x, int y) {
    if (gameState == 0 && key >= '1' && key <= '5') { gameState = key - '0'; resetGame(); }
    if (gameState == 6 && (key == 'm' || key == 'M')) gameState = 0;

    // HYPER SPEED TRIGGER
    if ((key == 'h' || key == 'H') && gameState >= 1 && gameState <= 5) {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        if (currentTime - lastHyperTime >= HYPER_COOLDOWN) {
            pVy = 35.0; // Huge boost
            lastHyperTime = currentTime;
        }
    }
    if (key == 27) exit(0);
}
void specialDown(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) moveLeft = true; if (key == GLUT_KEY_RIGHT) moveRight = true;
}
void specialUp(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) moveLeft = false; if (key == GLUT_KEY_RIGHT) moveRight = false;
}

int main(int argc, char** argv) {
    srand(time(0)); glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); glutInitWindowSize(500, 600);
    glutCreateWindow("Algo Jump - Obstacles & Hyperspeed");
    glMatrixMode(GL_PROJECTION); gluOrtho2D(-250, 250, -300, 300); glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(display); glutKeyboardFunc(keyboard); glutSpecialFunc(specialDown); glutSpecialUpFunc(specialUp);
    glutTimerFunc(16, update, 0); glutMainLoop(); return 0;
}
