/* File: car.h
 *
 * The file car.h contains functions for handling the movement and
 * controls of the car as well as initializes the vehicle struct.
 */

#pragma once

typedef struct {
	int x, y,
		width, height,
		speed, acceleration,
		timeDriven,
		steeredAmount,
		health;
} vehicle;

/*
 * getSteerAmount(void)
 *
 * getSteerAmount is a simple getter function that returns a positive
 * or negative integer value depending on if the wheel was turned right 
 * or left, and 0 if not steered outside of its default home range.
 */
int getSteerAmount(void);

/*
 * moveCar(vehicle *car, int steeredAmount)
 * 
 * @param car - vehicle the player is controlling
 * @param steeredAmount - amount the wheel has been turned
 *
 * The moveCar function is a function that moves the car based on a
 * steeredAmount, only moving the car if it is still in bounds
 */
void moveCar(vehicle *car);

