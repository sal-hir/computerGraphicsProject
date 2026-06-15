#include "levels.h"
#include "globals.h"
#include "algorithms.h"
#include <GL/glut.h>                                                       //main graphics library
#include <cstdlib>                                                         //Standard C++ library for rand()

void resetLevel2() {
    gravity = 0.4;                                //Gravity controls how fast player falls pull downward
    jumpStrength = 12.0;                   //controls jump power push upward
    glClearColor(0.05f, 0.02f, 0.12f, 1);                                  // background color
    platforms.push_back({0, -250, 100, 0, 0});                             // first platform creation (x,y,width,velocity,
    for(int i = 0; i < 10; i++)
        platforms.push_back({
                            (float)(rand() % 200 - 100),                    // random X position
                            -100.0f + i * 80,                               // Y goes higher each time
                            100, 0, 0                                       // width=100, no speed, bounces=0
        });
}

void updateLevel2() {
    for (auto& plat : platforms) { //Loop all platforms
        if (pY >= plat.y && pY <= plat.y + 5 &&                            //Is player at the right height
            pX >= plat.x - plat.w/2 && pX <= plat.x + plat.w/2 &&          //Is player within platform width
            pVy <= 0) {                                                    //Is player falling down
            plat.bounces++;                                                // Shrink platform
        }
    }
}

void drawLevel2() {
    for (auto& plat : platforms) {                                         //Loop through every platform
        if (plat.bounces >= 3) continue;                                   // If jumped on 3 times it skip drawing

        float currentWidth = plat.w * (1.0f - (plat.bounces * 0.33f));     // Shrink by 33% each bounce

        if (plat.bounces == 0) glColor3f(0, 1, 0);                         // Green (Healthy)
        else if (plat.bounces == 1) glColor3f(1, 1, 0);                    // Yellow (Warning)
        else glColor3f(1, 0, 0);                                           // Red (One jump left)

        glPushMatrix();                                                    //save current position
        glTranslatef(plat.x, plat.y, 0);                                   //move drawing space to platform's position (Translation)
        glScalef(currentWidth / 100.0f, 1.0, 1.0);                         //resize the platform width (Scaling only x)
        drawLineBres(-50, 0, 50, 0);                                       //draw the actual platform line
        glPopMatrix();                                                     //restore back to original position
    }
}
