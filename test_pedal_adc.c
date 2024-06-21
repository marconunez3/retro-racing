/* File: test_pedal_adc.c
 * This file contains test code to ensure the proper functionlity of the adc and pedal modules.
*/

#include "printf.h"
#include "adc.h"
#include "pedal.h"
#include "assert.h"
#include "timer.h"
#include <stddef.h>

// tests adc_confif
static void test_adc_basic(void) { 
    adc_config(CONT_MODE);
    printf("Successfully configured ADC chip.\n");
}

static void test_adc_read(void) {
    adc_config(CONT_MODE); 
    printf("Successfully configured ADC chip.\n");
    while (1) {
        int currVal = adc_read(0, false);
        printf("currVal = ( %d )\n", currVal);
        timer_delay_ms(100);
    }
}

static void test_pedal_throttle(void) {
    pedal_t *throttlePedal = pedal_init(0, 7, THROTTLE);
    while (1) {
	int currVal = getAccel(throttlePedal);
        printf("Current Acceleration = ( %d m/s^2 )\n", currVal);
        timer_delay_ms(100);
    }
}

int main() {
    timer_init();
    uart_init();
    i2c_init();
    printf("Begin testing adc.h and pedal.h modules\n");
    
    //test_adc_basic();
    //test_adc_read();
    test_pedal_throttle();
    printf("End testing adc.h and pedal.h modules.");
    return 0;
}


