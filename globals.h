#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

extern int gameState; // 0=Menu, 1-4=Levels, 5=GameOver
extern float cameraY;
extern int score;

extern float pX, pY, pVy;
extern float gravity, jumpStrength;
extern bool moveLeft, moveRight;

struct Platform { float x, y, w, vx; };
struct Obstacle { float x, y, w, vx; };

extern std::vector<Platform> platforms;
extern std::vector<Obstacle> obstacles;

#endif