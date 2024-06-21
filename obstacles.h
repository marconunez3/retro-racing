/* File: obstacle.h 
 *
 * This file contains the header file code for a module that generates obstacles.
 */

#include "scene.h"
#include "car.h"
#include <stddef.h>

typedef enum { BOTTOM = 0, TOP } spawn_type;

typedef enum { ECAR = 0, CRATE } obst_type;

typedef struct obstacle obst_t;

void generateObstacles(int currScore, obst_t *currObs[], int *currObsCount, road_t *road);

void drawCurrObstacles(obst_t *currObs[], int *currObsCount, road_t *road, int offsetAmount, vehicle *car);

unsigned int getRandInt(int min, int max);

