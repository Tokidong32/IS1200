#include "game_logic.h"
#include "globals.h"
#include "vga_utils.h"
#include "timer_utils.h"

#ifndef game_loop
#define game_loop

extern void enable_interrupt();

int gameTick(Board *game, Cell **head, Cell **end, int dir);

void handle_interrupt(unsigned cause);

int runGame();

#endif