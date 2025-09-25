#include "../headers/timer_utils.h"

//Game Timer
void timerInterruptInit()
{
    *TMR_CTRL |= 1; 

    *TMR_PRDL = 0x0B20;
    *TMR_PRDH = 0x20;

    enable_interrupt();
    *TMR_CTRL |= 1 << 2;
}

//menu Timmer
void runTimer()
{
    *TMR_STAT |= 1;    
    *TMR_PRDL = 0x0900;
    *TMR_PRDH = 0x3D;

    *TMR_CTRL |= 1 << 2;
}

void resetTimer()
{
    *TMR_STAT &= 0;
    *TMR_CTRL &= 0; 
    *TMR_PRDL &= 0;
    *TMR_PRDH &= 0;
}