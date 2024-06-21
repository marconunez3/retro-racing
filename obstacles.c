/* File: obstacles.c 
 * 
 * This file contains the implementation code for obstacles in the game that delineate their randomized movement as well as
 * the generation of distinct obstacles;
 *
 */

#include "scene.h"
#include "obstacles.h"
#include "car.h"
#include "malloc.h"
#include "gl.h"
#include "printf.h"
#include "timer.h"
#include <stdbool.h>

// all obstacle sprites are square images
#define CRATE_DIM 25
#define ECAR_DIM 30 
#define PIXEL_OFFSET 2
#define SCALE_FACTOR 4
#define INIT_SEC 0
#define FINAL_SEC 3
#define MAX_ALLOWED 3
#define OBSTACLE_INTERVAL 50
#define ROAD_Y 125
#define ROAD_X 188
#define TICK_CONST 1000000000

const int SECTOR_Y_THRESH[] = { 125, 150, 175, 200 };

struct obstacle {
    int x, y,
	dim,
	currentSector,
	deltaX,
	deltaY;

    obst_type type;
    spawn_type spawn;
};

int abs_n(int n) {
    if (n < 0) return n * -1;
    return n;
}

bool isInBounds(obst_t *obstacle, road_t *road) {
    bool isInVert = (obstacle->y >= road->tY) && (obstacle->y <= gl_get_height() - 1);
    return isInVert;
}

unsigned int randInt(int min, int max) {
    unsigned int randVal = min + (timer_get_ticks() / TICK_CONST);
    if (randVal > max) randVal = max;
    return randVal;
}

void setDeltas(obst_t *obstacle, road_t *road) {
    int yOffset = 2;
    // can be modified to have obstacles take diagaonal paths
    obstacle->deltaX = 0;
    obstacle->deltaY = yOffset;
}

obst_t *genObstacle(spawn_type spawn, obst_type type,road_t *road, int* currObs) { 
    int initDim = type ? CRATE_DIM : ECAR_DIM,
	yInit = ROAD_Y; 
    int xInit = ROAD_X + PIXEL_OFFSET, 
	xFinal = (ROAD_X + 25) - (PIXEL_OFFSET + initDim);
    
    obst_t *newObstacle = malloc(sizeof(newObstacle));
    newObstacle->spawn = spawn; 
    newObstacle->type = type;
    newObstacle->dim = initDim;
    newObstacle->x = randInt(xInit, xFinal);
    newObstacle->y = yInit;
    newObstacle->currentSector = spawn ? INIT_SEC : FINAL_SEC;
    setDeltas(newObstacle, road);
    return newObstacle;
}

int getMaxObstacles(int currScore) {
    if (currScore < OBSTACLE_INTERVAL) return 1;
    int calculatedObst = (currScore / OBSTACLE_INTERVAL), 
	maxObst = (calculatedObst < MAX_ALLOWED) ? calculatedObst : MAX_ALLOWED;
    return maxObst;
}

void generateObstacles(int currScore, obst_t *currObs[], int *currObsCount, road_t *road) {  
    int maxObst = getMaxObstacles(currScore);
    if (*currObsCount < maxObst) {
	currObs[*currObsCount] = genObstacle(TOP, CRATE, road, currObsCount);
	*currObsCount += 1;
    }
    return;
}

void resizeObstacle(obst_t *obstacle) {
    int currSector = obstacle->currentSector,
	currThresh = SECTOR_Y_THRESH[currSector];
    if (obstacle->spawn == TOP && obstacle->y > currThresh) {
	if (currSector == FINAL_SEC) return;
	obstacle->dim += obstacle->type ? CRATE_DIM / SCALE_FACTOR : ECAR_DIM / SCALE_FACTOR;
	obstacle->currentSector = (currSector + 1) <= FINAL_SEC ? (currSector + 1) : FINAL_SEC; 
    } else if (obstacle->spawn == BOTTOM  && obstacle->y < currThresh) {	
	obstacle->dim -= obstacle->type ? CRATE_DIM / SCALE_FACTOR : ECAR_DIM / SCALE_FACTOR;
	obstacle->currentSector = (currSector - 1) >= INIT_SEC ? (currSector - 1) : INIT_SEC; 
     }
}

void removeObstacle(obst_t *currObs[], int i, int *currObsCount) {
    for (int j = i + 1; j < *currObsCount; j++) {
	currObs[j - 1] = currObs[j];
    }
    *currObsCount -= 1; 
}

bool inRange(int n, int min, int max) {
    return (n >= min && n <= max);
}

bool obstacleCollison(obst_t *currObs, obst_t *currObstacles[], int i, int *currObsCount, vehicle *car) {
    int carRightX = (car->x + car->width), carLeftX = car->x,
	obsRightX = (currObs->x + currObs->dim), obsLeftX = currObs->x;
    bool isInHorizRange = inRange(obsLeftX, carLeftX, carRightX) || inRange(obsRightX, carLeftX, carRightX);

    int carBottY = (car->y + car->height), carTopY = car->y,
	obsBottY = (currObs->y + currObs->dim);
    bool isInVertRange = inRange(obsBottY, carTopY, carBottY);
    if (isInHorizRange && isInVertRange) { // car within range of obs
	removeObstacle(currObstacles, i , currObsCount);
	car->health -= 1; 
	return true;
    }
    return false;
}

void drawCurrObstacles(obst_t *currObs[], int *currObsCount, road_t *road, int offsetAmount, vehicle *car) {
    for (int i = 0; i < *currObsCount; i++) {
	int obstDim = currObs[i]->dim,
	    x = currObs[i]->x, y = currObs[i]->y;
	if (obstacleCollison(currObs[i], currObs, i, currObsCount, car))  return;
	if (isInBounds(currObs[i], road)) { 
	    drawCrate(x, y);
	} else {
	    removeObstacle(currObs, i, currObsCount);	// replace with drawImage function
	    return;
	}
	currObs[i]->y += 2;	
    }    
}

