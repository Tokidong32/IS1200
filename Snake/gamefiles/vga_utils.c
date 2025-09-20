#include "../headers/vga_utils.h"

void drawPixel(int x, int y, int color)
{
    volatile char* addr = VGA_START;

    int offset = (y * RESOLUTION_X) + x;
    addr[offset] = color;
}

void drawCell(Cell* model){

    int color = 0;
    switch (model->type)
    {
    //Apple colour
    case 5:
        color = RED;
        break;
    //Snake_end colour
    case 4:
        color = PINK;
        break;
    //Snake_body colour
    case 3:
        color = PINK;
        break;
    //Snake_head colour
    case 2:
        color = PINK;
        break;
    //Wall colour
    case 1:
        color = WHITE;
        break;
    //Floor colour
    case 0:
        color = BLACK;
        break;
    }

    for (int x = (model->pos.x * 9) + 70; x < (model->pos.x * 9) + 79; x++)
    {
        for (int y = (model->pos.y * 9)+30; y < (model->pos.y * 9) + 39; y++)
        {
            drawPixel(x,y,color);
        }
    }
}

void drawBoard(Board* game){

    for (int x = 0; x < game->rows; x ++){
        for (int y = 0; y < game->colums; y ++){
            drawCell(&game->cells[x][y]);
        }
    }
}