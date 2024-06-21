/* File: car.c
 *
 * Author : Marco Nunez
 *
 * This file contains the implementation code for moving
 * a given vehicle around the road.
 */

#include "assert.h"
#include "gpio.h"
#include "gpio_interrupt.h"
#include "uart.h"
#include "printf.h"
#include "strings.h"
#include "car.h"
#include "gl.h"
#include "adc.h"
#include "scene.h"

#define STEER_PIN 1
#define SENSITIVITY 500
#define GL_ROAD     0xFF403e3e
#define RIGHT_EDGE 255
#define LEFT_EDGE 148
#define MIN_HOME_RANGE 10000
#define MAX_HOME_RANGE 12500

int getSteerAmount(void) {
	int position = adc_read(STEER_PIN, false); 
	if (position > 32000) position = 1; // turning right to the max ends up with really large garbage values
										
	if (position > MAX_HOME_RANGE) return -1 * (position - MAX_HOME_RANGE) / 500; // logic is flipped due to orientation of the potentiometer
	else if (position < MIN_HOME_RANGE) return -1 * (position - MIN_HOME_RANGE) / 500; // negative
	
	return 0;
}

int canMove(vehicle *car) {
	color_t movedToPixel;
	// if steering right, we must check pixels right of its top-left position + width
	if (car->steeredAmount > 0) {
		movedToPixel = car->x + car->width + car->steeredAmount;
		return movedToPixel < RIGHT_EDGE;
	}
	else movedToPixel = car->x + car->steeredAmount;
	return movedToPixel > LEFT_EDGE; // returns true if moving to space on road
}

void moveCar(vehicle *car) {
	if (canMove(car)) {
		car->x += car->steeredAmount;
	}
}
