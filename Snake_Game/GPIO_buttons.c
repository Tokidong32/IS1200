extern void enable_interrupt();

#define TMR_STAT ((volatile short*)0x04000020)
#define TMR_CTRL ((volatile short*)0x04000024)
#define TMR_PRDL ((volatile short*)0x04000028)
#define TMR_PRDH ((volatile short*)0x0400002C)

#define GPIO_DATA ((volatile int*)0x040000e0)
#define GPIO_DIRE ((volatile int*)0x040000e4)



void handle_interrupt(unsigned cause)
{
  *TMR_STAT &= 0;
  *TMR_PRDL = 0x0B20;
  *TMR_PRDH = 0x20;

  *TMR_CTRL |= 1 << 2;
}

void timer_init()
{
  *TMR_CTRL |= 0b1; 

  *TMR_PRDL = 0;
  *TMR_PRDH = 0;

  enable_interrupt();
  *TMR_CTRL |= 1 << 2;
}

void set_light(int led)
{
volatile int* LED = (volatile int*)0x04000000;
  *LED = led; 
}


/* This is the main function */
int main()
{
  *GPIO_DIRE = 0xffff;

  *GPIO_DATA &= 0x00000000;

  timer_init();

  while (1)
  {
    if(*TMR_STAT & 1)
    {
      int player_switch = *GPIO_DATA;
      player_switch &= 0xf;

      set_light(player_switch);
    }
  }
}
