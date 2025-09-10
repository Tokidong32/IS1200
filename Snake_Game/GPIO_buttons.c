#include <stdint.h>


void button_press()
{   
    /* */
    volatile int * gpio1_dir   =  (volatile int *)0x040000e4;
    *gpio1_dir =  *gpio1_dir | 0xe;

    volatile int * gpio1_data  =  (volatile int *)0x040000e0;
    volatile int* button_up    =  (volatile int *)0x40000e0;
    volatile int* button_right =  (volatile int *)0x40000e1;
    volatile int* button_down  =  (volatile int *)0x40000e2;
    volatile int* button_left  =  (volatile int *)0x40000e3;
}