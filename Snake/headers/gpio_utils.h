#ifndef gpio_utils
#define gpio_utils

#define GPIO_DATA ((volatile int*)0x040000e0)
#define GPIO_DIRE ((volatile int*)0x040000e4)

void gpio_Init();

#endif