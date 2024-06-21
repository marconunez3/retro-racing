/* File: adc.h
 * The file adc.h will contain driver code for interfacing the ADS1115 
 * Analog-to-Digital Signal converter that is based on the I2C communication protocol.
 */

#include "i2c.h"
#include <stddef.h>

typedef enum { CONT_MODE = 0x0, SINGLE_MODE = 0x0100} adc_mode_t;

typedef enum {
    G_1 = 1,
    G_2,
    G_4,
    G_8,
    G_16
} adc_gain_t;

typedef enum {
    D_8 = 0,
    D_16,
    D_32,
    D_64,
    D_128,
    D_250,
    D_475,
    D_860
} adc_dr_t;


/* 
 * adc_config(adc_mode_t mode)
 *
 * @param  mode the mode in which the ADC operates
 * 		continous conversion - CONT_MODE
 * 		power-down single-shot mode - SINGLE_MODE (default)
 *
 * The `adc_config` takes in an operation mode for ADC1115 and initializes the 
 * gain to 1 (default) and data rate to 128 (default). It creates an I2C device 
 * internally that allows for communication between the MangoPi and the converter.
 */
void adc_config(adc_mode_t mode, adc_gain_t gain, adc_dr_t dr);

/*
 * read_adc_byte(int analog_pin,  bool is_diff)
 *
 * @param analog_pin - the analog pin (A0 - A3) to be read from
 * @param is_diff - wether the input pin is being used in differential mode or single ended input mode
 * 
 * The `adc_read` function takes in a pin to read analog data from and the type of input being used (differential
 * or single-ended) and returns the most recently converted value that is associated to that pin.
 */
int adc_read(int analog_pin, bool is_diff);

/*
 * set_ADC_mode(adc_mode_t mode)
 *
 * @param mode - conversion mode to set ADC1115 to
 *
 * The `set_ADC_mode` is a simple setter function to alter the operation mode of the converter
 */
void set_ADC_mode(adc_mode_t mode);

/*
 * adc_mode_t get_ADC_mode()
 *
 * The `get_ADC_mode` is a simple getter function to get the current operation mode (CONT_MODE or SINGLE_MODE) 
 */
adc_mode_t get_ADC_mode();

/*
 * set_ADC_gain(adc_gain_t gain)
 *
 * @param gain - gain setting to set ADC1115 to
 *
 * The `set_ADC_gain` is a simple setter function to alter the voltage gain of the converter
 */
void set_ADC_gain(adc_gain_t gain);

/*
 * adc_gain_t get_ADC_gain()
 *
 * The `get_ADC_gain` is a simple getter function to get the current voltage gain of converter(G1 - G16) 
 */
adc_gain_t get_ADC_gain();

/*
 * set_ADC_dr(adc_dr_t data_rate)
 *
 * @param data_rate - data rate to set ADC1115 to operate in
 *
 * The `set_ADC_dr` is a simple setter function to alter the data rate (rate of conversion) the converter operates at
 */
void set_ADC_dr(adc_dr_t data_rate);

/*
 * adc_dr_t get_ADC_dr()
 *
 * The `get_ADC_mode` is a simple getter function to get the current data rate (D_8 - D860). These are the number of conversions
 * per second that the ADC is performing. 
 */
adc_dr_t get_ADC_dr();





