#include "printf.h"
#include "timer.h"
#include "uart.h"
#include "parseImage.h"
#include "gl.h"

static void test_parseImage(void){
    int width = 400;
    int height = 225;

    gl_init(width, height, GL_DOUBLEBUFFER);

    gl_clear(GL_PURPLE);
    gl_swap_buffer();

    parseImage(width, height);
    gl_swap_buffer();
    timer_delay(390);
}

// int main(){
//     timer_init();
//     uart_init();
//     printf("Running test_parseImage...\n");


//     // tests
//     test_parseImage();

//     printf("Successfully passed all tests!\n");
//     return 0;
// }