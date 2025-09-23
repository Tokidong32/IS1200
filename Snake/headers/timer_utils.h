#ifndef timer_utils
#define timer_utils

#define TMR_STAT ((volatile short*)0x04000020)
#define TMR_CTRL ((volatile short*)0x04000024)
#define TMR_PRDL ((volatile short*)0x04000028)
#define TMR_PRDH ((volatile short*)0x0400002C)

extern void enable_interrupt();

void timerInterruptInit();
void runTimer();
void resetTimer();

#endif