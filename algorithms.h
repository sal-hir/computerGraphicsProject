#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <string>

void drawPixel(int x, int y);
void drawLineDDA(int x1, int y1, int x2, int y2);
void drawLineBres(int x1, int y1, int x2, int y2);
void drawCircleBres(int cx, int cy, int r);
void floodFill(float x, float y, float r, float g, float b);
void drawClippedLine(double x1, double y1, double x2, double y2, double xMin, double xMax, double yMin, double yMax);
void drawText(float x, float y, std::string s, void* font);

#endif