/* File: pedal.h
 * 
 * Author: Manuel Argueta
 *
 * The 'pedal.h' module is an abstracted module that interacts with the lower level ADC module in
 * order to use a 10K potentimeter as a form of reading 'accleeration' singals from a 3D printed 
 * racing simulator pedal.
 */

typedef enum {
    THROTTLE = 0,
    BRAKE,
    CLUTCH
} pedal_type;

typedef struct pedal_controller pedal_t;

pedal_t *pedal_init(int analog_pin, pedal_type type);

int getAccel(pedal_t *pedal);
