#ifndef GLOBALS_H
#define GLOBALS_H
#include <vector>

extern int gameState;   // 0=Menu, 1-4=Levels
extern int subState;    // 0=Intro Animation, 1=Playing, 2=Game Over Animation
extern int stateTimer;  // General purpose timer for animations
extern float cameraY;
extern int score;
extern int jumpCount;   // Tracks total successful jumps

extern float pX, pY, pVy;
extern float gravity, jumpStrength;
extern bool moveLeft, moveRight;

struct Platform { float x, y, w, vx; int bounces; };
struct Obstacle { float x, y, w, angle; };
struct Planet { float x, y, radius; bool collected; }; //level 1

extern std::vector<Platform> platforms;
extern std::vector<Obstacle> obstacles;
extern std::vector<Planet> planets; //level 1

#endif
