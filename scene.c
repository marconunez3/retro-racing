
#include "headerFiles/build.h"
#include "headerFiles/playerCar.h"
#include "headerFiles/heart.h"
#include "headerFiles/half_heart.h"
#include "headerFiles/empty_heart.h"
#include "headerFiles/crate.h"
#include "headerFiles/startTitle.h"

#include "keyboard.h"
#include "printf.h"
#include "strings.h"
#include "gl.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "interrupts.h"

#include "keyboard.h"
#include "printf.h"
#include "strings.h"
#include <stddef.h>
#include "gl.h"
#include "malloc.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "interrupts.h"
#include "adc.h"
#include "pedal.h"
#include "car.h"
#include "scene.h"
#include "obstacles.h"




#define GL_SKYTOP   0xFF009cc7
#define GL_SKYMID   0xFF02bef2
#define GL_SKYLAST  0xFF00ddff

#define GL_ROAD     0xFF403e3e

// grass
#define GL_GRASS0  0xFF00ff55
#define GL_GRASS1  0xFF00e64d
#define GL_GRASS2  0xFF00cc44
#define GL_GRASS3  0xFF00b33c
#define GL_GRASS4  0xFF009933


#define RIGHT_EDGE 250
#define LEFT_EDGE 120


#define ONE_SEC 24000000 // one second in ticks
#define MAX_SPEED 100
#define FRICTION 7
#define START_LANE_X 175
#define START_LANE_Y 190
#define CAR_WIDTH 30
#define CAR_HEIGHT 30


extern const unsigned char buildData[];
extern const unsigned char playerCarData[];
extern const unsigned char heartData[];
extern const unsigned char half_heartData[];
extern const unsigned char empty_heartData[];
extern const unsigned char crateData[];
extern const unsigned char startTitleData[];




// chris gregg
void drawBuildings(int x, int y) {
    for (int i = 0; i < sizeof(buildData); i += 4) {
        color_t pixel = buildData[i] << 24 | // alpha
            buildData[i + 1] << 16 | // red
            buildData[i + 2] << 8 |  // green
            buildData[i + 3];        // blue
        int col = (i / 4) % build_WIDTH;
        int row = (i / 4) / build_WIDTH;
        if (pixel != 0xFFFF00CC){
            int colOffsetted = col + x;
            int rowOffsetted = row + y;
            gl_draw_pixel(colOffsetted, rowOffsetted, pixel);
        }
    }
}

void drawplayerCar(int x, int y) {
    for (int i = 0; i < sizeof(playerCarData); i += 4) {
        color_t pixel = playerCarData[i] << 24 | // alpha
            playerCarData[i + 1] << 16 | // red
            playerCarData[i + 2] << 8 |  // green
            playerCarData[i + 3];        // blue
        int col = (i / 4) % playerCar_WIDTH;
        int row = (i / 4) / playerCar_WIDTH;
        if (pixel != 0xFFFF00CC){
            int colOffsetted = col + x;
            int rowOffsetted = row + y;
            gl_draw_pixel(colOffsetted, rowOffsetted, pixel);
        }
    }
}

void drawHeart(int x, int y) {
    for (int i = 0; i < sizeof(heartData); i += 4) {
        color_t pixel = heartData[i] << 24 | // alpha
            heartData[i + 1] << 16 | // red
            heartData[i + 2] << 8 |  // green
            heartData[i + 3];        // blue
        int col = (i / 4) % heart_WIDTH;
        int row = (i / 4) / heart_WIDTH;
        if (pixel != 0xFFFF00CC){
            int colOffsetted = col + x;
            int rowOffsetted = row + y;
            gl_draw_pixel(colOffsetted, rowOffsetted, pixel);
        }
    }
}

void drawHalfHeart(int x, int y) {
    for (int i = 0; i < sizeof(half_heartData); i += 4) {
        color_t pixel = half_heartData[i] << 24 | // alpha
            half_heartData[i + 1] << 16 | // red
            half_heartData[i + 2] << 8 |  // green
            half_heartData[i + 3];        // blue
        int col = (i / 4) % half_heart_WIDTH;
        int row = (i / 4) / half_heart_WIDTH;
        if (pixel != 0xFFFF00CC){
            int colOffsetted = col + x;
            int rowOffsetted = row + y;
            gl_draw_pixel(colOffsetted, rowOffsetted, pixel);
        }
    }
}

void drawEmptyHeart(int x, int y) {
    for (int i = 0; i < sizeof(empty_heartData); i += 4) {
        color_t pixel = empty_heartData[i] << 24 | // alpha
            empty_heartData[i + 1] << 16 | // red
            empty_heartData[i + 2] << 8 |  // green
            empty_heartData[i + 3];        // blue
        int col = (i / 4) % empty_heart_WIDTH;
        int row = (i / 4) / empty_heart_WIDTH;
        if (pixel != 0xFFFF00CC){
            int colOffsetted = col + x;
            int rowOffsetted = row + y;
            gl_draw_pixel(colOffsetted, rowOffsetted, pixel);
        }
    }
}

void drawCrate(int x, int y) {
    for (int i = 0; i < sizeof(crateData); i += 4) {
        color_t pixel = crateData[i] << 24 | // alpha
            crateData[i + 1] << 16 | // red
            crateData[i + 2] << 8 |  // green
            crateData[i + 3];        // blue
        int col = (i / 4) % crate_WIDTH;
        int row = (i / 4) / crate_WIDTH;
        if (pixel != 0xFFFF00CC){
            int colOffsetted = col + x;
            int rowOffsetted = row + y;
            gl_draw_pixel(colOffsetted, rowOffsetted, pixel);
        }
    }
}

void drawLives(int lives){
    int emptyHearts = 3;
    for (int i = lives/2; i > 0; i--){
        drawHeart(345 + (i-1)*17, 7);
        emptyHearts--;
    }
    // check if there is a half heart
    if (lives % 2 != 0){
        drawHalfHeart(345 + (lives/2)*17, 7);
        emptyHearts--;
    }

    // check how many empty hearts to draw
    for (int i = emptyHearts; i > 0; i--){
        drawEmptyHeart(345 + (3 - i)*17, 7);
    }
}

// clear the car at the given x, y
void clearCar(int x, int y) {
    int cleared = 900;
    for (int i = 0; i < sizeof(playerCarData); i += 4) {
        int col = (i / 4) % playerCar_WIDTH;
        int row = (i / 4) / playerCar_WIDTH;
        int colOffsetted = col + x;
        int rowOffsetted = row + y;
        cleared -=1;

        gl_draw_pixel(colOffsetted, rowOffsetted, GL_ROAD);
    }

    printf("%d\n", cleared);
}



//clear the heart at the given x, y
void clearHeart(int x, int y) {
    for (int i = 0; i < sizeof(heartData); i += 4) {
        int col = (i / 4) % heart_WIDTH;
        int row = (i / 4) / heart_WIDTH;
        int colOffsetted = col + x;
        int rowOffsetted = row + y;
        gl_draw_pixel(colOffsetted, rowOffsetted, GL_SKYTOP);
    }
}

void clearCrate(int x, int y) {
    for(int i = 0; i < sizeof(crateData); i+=4){
        int col = (i / 4) % crate_WIDTH;
        int row = (i / 4) / crate_WIDTH;
        int colOffsetted = col + x;
        int rowOffsetted = row + y;
        gl_draw_pixel(colOffsetted, rowOffsetted, GL_ROAD);
    }
}



void drawTrap(int x, int y, int topWidth, int bottomWidth, int h, color_t c) {
    for (int i = 0; i < h; i++) {
        int curWidth = topWidth + ((bottomWidth - topWidth) * i / h);
        int xStart = x + (topWidth - curWidth) / 2;

        for (int j = 0; j < curWidth; j++) {
            gl_draw_pixel(xStart + j, y + i, c);
        }
    }
}

void drawSmallTrap(int x, int y, int topWidth, int bottomWidth, int h, color_t c) {
    int startLine = (h > 100) ? h - 100 : 0;

    for (int i = startLine; i < h; i++) {
        int curWidth = topWidth + ((bottomWidth - topWidth) * i / h);
        int xStart = x + (topWidth - curWidth) / 2;

        for (int j = 0; j < curWidth; j++) {
            gl_draw_pixel(xStart + j, y + i, c);
        }
    }
}

void drawLane(int yOffset) {
    int numLines = 5;  
    int lineHeight = 130 / (numLines * 2);  
    int lineWTop = 60 / 20;  
    int lineWBottom = 130 / 20;  


    for (int i = 0; i < numLines; i++) {
        int yLine = yOffset + i * lineHeight * 2;
        drawTrap(200, yLine, lineWTop, lineWBottom, lineHeight, GL_WHITE);

        //scrolling
        if (yLine + lineHeight >= 130) {
            drawTrap(200, yLine - 130, lineWTop, lineWBottom, lineHeight, GL_WHITE);
        }
    }
}

void stripeSide(int x, int y, int topWidth, int bottomWidth, int h, color_t c, color_t c2, int yOffset) {
    int numLines = 20;  
    int linesPerColor = numLines / 2;

    for (int i = h - 1; i >= 0; i--) {
        int stripeIndex = (h - 1 - i + yOffset) / linesPerColor; // which stripe are we on

        color_t currentColor;
        if (stripeIndex % 2 == 0) {
            currentColor = c;
        } else {
            currentColor = c2;
        }

        int curWidth = topWidth + ((bottomWidth - topWidth) * i / h); // width of current line of pixels
        int xStart = x + (topWidth - curWidth) / 2;
        int yStart = y + i;

        for (int j = 0; j < curWidth / 10; j++) { // left stripe
            if (gl_read_pixel(xStart + j, yStart) != GL_ROAD) {
                gl_draw_pixel(xStart + j, yStart, currentColor);
            }
        }

        for (int j = curWidth - (curWidth / 10); j < curWidth; j++) { // right stripe
            if (gl_read_pixel(xStart + j, yStart) != GL_ROAD) {
                gl_draw_pixel(xStart + j, yStart, currentColor);
            }
        }
    }
}

void drawGroundSky(int x, int y, int width, int height, color_t color) {
    gl_draw_rect(x, y, width, height, color);
}

void drawForeAndBackground(int width, int height){
    drawGroundSky(0, 0, width, 40, GL_SKYTOP);
    drawGroundSky(0, 40, width, 30, GL_SKYMID);
    drawGroundSky(0, 70, width, 60, GL_SKYLAST);
    
    // draw grass
    drawGroundSky(0, 125, width, 25, GL_GRASS1);
    drawGroundSky(0, 150, width, 25, GL_GRASS2);
    drawGroundSky(0, 175, width, 25, GL_GRASS3);
    drawGroundSky(0, 200, width, 25, GL_GRASS4);

    drawLives(0);
}


void drawRoad(int width, int height) {
    drawForeAndBackground(width, height);
    drawBuildings(0, 25);


    gl_swap_buffer();
    drawForeAndBackground(width, height);
    drawBuildings(0,25);

    int topLen = 25,
	bottomRoadLen = 600;

    int roadInitX = (width / 2) - (topLen / 2);
    
    drawTrap(roadInitX, 125, topLen, bottomRoadLen-70, height, GL_ROAD);

    gl_swap_buffer();
    drawTrap(roadInitX, 125, topLen, bottomRoadLen-70, height, GL_ROAD);
}

road_t *setRoadInfo(int tWidth, int bWidth, int tX, int tY, int bX) {
    road_t *newRoad = malloc(sizeof(newRoad));
    newRoad->tWidth = tWidth;
    newRoad->bWidth = bWidth;
    newRoad->tX = tX;
    newRoad->tY = tY;
    newRoad->bX = bX;
    return newRoad;
}

void drawStartTitle(int x, int y) {
    for (int i = 0; i < sizeof(startTitleData); i += 4) {
        color_t pixel = startTitleData[i] << 24 | // alpha
            startTitleData[i + 1] << 16 | // red
            startTitleData[i + 2] << 8 |  // green
            startTitleData[i + 3];        // blue
        int col = (i / 4) % startTitle_WIDTH;
        int row = (i / 4) / startTitle_WIDTH;
        if (pixel != 0xFFFF00CC){
            int colOffsetted = col + x;
            int rowOffsetted = row + y;
            gl_draw_pixel(colOffsetted, rowOffsetted, pixel);
        }
    }
}

void displayTitle(){
    gl_clear(GL_BLACK);
    drawForeAndBackground(400, 225);
    drawStartTitle(50, 50);
    gl_swap_buffer();
    while(1);
}

// main game loop
void main_game(void) {
    adc_config(CONT_MODE, G_1, D_475);
    pedal_t *pedal = pedal_init(0, THROTTLE);

    vehicle car = {START_LANE_X, START_LANE_Y, CAR_WIDTH, CAR_HEIGHT, 0, 0, 0, getSteerAmount()};

    unsigned long startTime = timer_get_ticks();
    car.timeDriven = 0;

    int topLen = 25,
	bottomLen = 600,
	bottomRoadLen = 600,
	edgeOffset = 0,
	currScore = 0,
	topY = 125,
	currObsCount = 0,
	roadInitX = (400 / 2) - (topLen / 2),
        roadBottomX = (400 / 2) - (bottomLen / 2);
    car.health = 6;
    obst_t *currentObs[10];
    road_t *road = setRoadInfo(topLen, bottomLen, roadInitX, topY, roadBottomX); 

    drawRoad(400, 225);
    while (1) {	
        stripeSide(roadInitX,topY, topLen, bottomLen, 225, GL_RED, GL_WHITE, edgeOffset); 
	unsigned long currentTicks = timer_get_ticks();	// get current time of frame
	unsigned long elapsedTime = currentTicks - startTime;
	if (elapsedTime >= ONE_SEC) { // not perfect, will fall out of sync over long runs
	    startTime = currentTicks;
	    car.speed += car.acceleration;
	    currScore += 2;
	    if (car.speed < 0) car.speed = 0; // decelerated speed to 0
	    else if (car.speed > MAX_SPEED) car.speed = MAX_SPEED;
	    car.timeDriven++;
	}
	
	car.steeredAmount = getSteerAmount();
	car.acceleration = getAccel(pedal);
	
        edgeOffset+=2;
        drawLives(car.health);

	// RUN THIS FORM OF CLEARING ONLY WHEN THERE ARE NO OBSTACLES
        //int prevX = car.x;
        //int prevY = car.y;

	moveCar(&car);
	
        /*if (prevX != car.x){
            clearCar(prevX, prevY);
            drawplayerCar(car.x, car.y);
            gl_swap_buffer();
            stripeSide(roadInitX, topY, topLen, bottomLen, 225, GL_RED, GL_WHITE, edgeOffset);
            clearCar(prevX, prevY);
            drawplayerCar(car.x, car.y);
        }*/

        drawTrap(roadInitX, topY, topLen, bottomRoadLen-70, 225, GL_ROAD);
	generateObstacles(currScore, currentObs, &currObsCount, road);
        drawCurrObstacles(currentObs, &currObsCount, road, 2, &car);
        drawplayerCar(car.x, car.y);

        if (car.health == 0) break;

   	gl_swap_buffer();
	timer_delay_ms(1000 / (car.speed + 1)); // draw new road faster if speed is faster
    }

    displayTitle();
}
