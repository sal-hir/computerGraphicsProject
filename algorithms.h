#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <string>
void drawPixel(int x, int y);
void drawLineDDA(int x1, int y1, int x2, int y2);
void drawLineBres(int x1, int y1, int x2, int y2);
void drawCircleBres(int cx, int cy, int r);
void drawText(float x, float y, std::string s, void* font);
#endif
