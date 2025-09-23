#include "game_logic.h"

#ifndef vga_utils
#define vga_utlis

#define RESOLUTION_X 320
#define RESOLUTION_Y 240

#define VGA_START ((volatile char*) 0x08000000)
#define VGA_END ((volatile char*) 0x08000000 + (RESOLUTION_X * RESOLUTION_Y))
#define VGA_STATCTRL ((volatile int*) 0x0400010C)

#define RED    0xE0
#define GREEN  0x1C
#define BLUE   0xA3
#define PURPLE 0x82
#define PINK   0xE7
#define WHITE  0xFF
#define BLACK  0x00

void drawPixel(int x, int y, int color);

void drawCell(Cell* model);

void drawBoard(Board* game);

void clearScreen();

void drawMainMenu();

void drawMenuHighlight(int highlight, int lastHighlight);

void drawLetter(char letter, int offsetX, int offsetY, char remove);

void drawScore(int score);

void drawNumber(int value, int offset);
#endif 
