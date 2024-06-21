#include "printf.h"
#include "timer.h"
#include "uart.h"
#include "gl.h"
#include "keyboard.h"
#include "scene.h"

#include "console.h"
#include "interrupts.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include "uart.h"


#include "gpio.h"
#include "gpio_extra.h"
#include "gpio_interrupt.h"
#include "keyboard.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

void test_main_loop() {
    gl_init(400, 225, GL_DOUBLEBUFFER);
    main_game();
}

void test_draw_trap() {
    int WIDTH = 400, HEIGHT = 225;
    gl_init(WIDTH, HEIGHT, GL_DOUBLEBUFFER);
    gl_clear(GL_BLUE);

    gl_swap_buffer();
    printf("HERE");
    drawRoad(WIDTH, 150);

    timer_delay(5);
}


int main(void) {
    gpio_init();
    timer_init();
    uart_init();
	interrupts_init();

    printf("\nStarting main() in %s\n", __FILE__);
	test_main_loop();
    //test_draw_trap();

    printf("\nCompleted execution of main() in %s\n", __FILE__);
    return 0;
}
