#ifndef GLOBALS_H //contains shared variables
#define GLOBALS_H
#include <vector>

extern int gameState;   // 0=Menu, 1-4=Levels
extern int subState;    // 0=Intro Animation, 1=Playing, 2=Game Over Animation
extern int stateTimer;  // General purpose timer for animations
extern float cameraY;
extern int score;
extern int jumpCount;   // Tracks total successful jumps

extern float pX, pY, pVy; //pX = player's X position (left/right)                      pVy = player's vertical speed (falling/jumping)
extern float gravity, jumpStrength; //gravity = pulls player down every frame          jumpStrength = how high player jumps
extern bool moveLeft, moveRight;

struct Platform { float x, y, w, vx; int bounces; }; // platform object       bounces = how many times player jumped on it
struct Obstacle { float x, y, w, angle; };

extern std::vector<Platform> platforms;   //list of all platforms currently in the game
extern std::vector<Obstacle> obstacles;

#endif
