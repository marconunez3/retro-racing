/* File pedal.c
 *
 * Author: Manuel Argueta
 *
 * The file 'pedal.c' contains the implementation of a module that interprets data from
 * a 10K pontetiometer into acceleration data that can be utilized for distinct purposes.
 * The analog data from the potentiometer is bucketed into ranges to further reduce the effects
 * of the slight noise present in the data.  
 */

#include "adc.h"
#include "pedal.h"
#include "malloc.h"
#include <stddef.h>

#define ACCEL_OFFSET 8
#define MAX_ACCEL 20
#define FRICTION 7
#define POT_MAX 130

// the ACCEL_INTERVAL constant defines much change in the potent
#define ACCEL_INTERVAL 700
#define BRAKE_INTERVAL ACCEL_INTERVAL
#define CLUTCH_INTERVAL 10000

static const int intervalTypes[] = {ACCEL_INTERVAL, BRAKE_INTERVAL, CLUTCH_INTERVAL};

struct pedal_controller {
    int pin;
    pedal_type type;
};

pedal_t *pedal_init(int analog_pin, pedal_type type) {
    pedal_t *module = malloc(sizeof(*module));
    module->pin = analog_pin;
    module->type = type;
    return module;
}

int findAccelRange(pedal_t *pedal, int potVal) {
    return (potVal /intervalTypes[pedal->type]);
}

int getAccel(pedal_t *pedal) {
    int currPotVal = adc_read(pedal->pin, false),
	accelVal = findAccelRange(pedal, currPotVal); 
    if (accelVal > POT_MAX || (accelVal - ACCEL_OFFSET) < 0) accelVal = ACCEL_OFFSET;
    else if ((accelVal - ACCEL_OFFSET) > MAX_ACCEL) accelVal = MAX_ACCEL + ACCEL_OFFSET;
	int result = accelVal - ACCEL_OFFSET - FRICTION;
    return (result >= -6 && result <= 4) ? 0 : result; // wide range to return 0 for maintaining a constant speed
}
