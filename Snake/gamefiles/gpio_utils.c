#include "../headers/gpio_utils.h"

void gpioInit()
{
  *GPIO_DIRE = 0xf;
}