#include "game_logic.h"
#include "globals.h"
#include "vga_utils.h"

#ifndef game_loop
#define game_loop

#define TMR_STAT ((volatile short*)0x04000020)
#define TMR_CTRL ((volatile short*)0x04000024)
#define TMR_PRDL ((volatile short*)0x04000028)
#define TMR_PRDH ((volatile short*)0x0400002C)


extern void enable_interrupt();

int gameTick(Board *game, Cell **head, Cell **end, int dir);

void handle_interrupt(unsigned cause);

int runGame();

#endif