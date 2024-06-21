#ifndef ROAD_H
#define ROAD_H

#include "gl.h"

struct road {
    int tWidth,
	bWidth,
	tX,
	tY,
	bX,
	bY;
};

typedef struct road road_t; // compacts storage of road metadata

void gl_draw_trapezoid(int x, int y, int topWidth, int bottonWidth, int h, color_t c);
void drawRoad();
void drawplayerCar(int x, int y);
void stripeTrap(int x, int y, int topWidth, int bottonWidth, int h, color_t c, color_t c2);
void stipeSide(int x, int y, int topWidth, int bottomWidth, int h, color_t c, color_t c2, int yOffset);
void drawForeAndBackground(int width, int height);
void drawBuildings(int x, int y);
void drawTrap(int x, int y, int topWidth, int bottonWidth, int h, color_t c);
void clearCar(int x, int y);
void main_game(void);
void drawCrate(int x, int y);
#endif
