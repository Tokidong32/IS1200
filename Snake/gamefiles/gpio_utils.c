#include "../headers/gpio_utils.h"

void gpio_Init()
{
  *GPIO_DIRE = 0xf;
}