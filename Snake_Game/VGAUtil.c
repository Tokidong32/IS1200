#include <stdlib.h>
#include <stdio.h>

#define VGA_START ((volatile char*) 0x08000000)
#define VGA_END ((volatile char*) 0x08000000 + (240 * 320))
#define VGA_CTRL ((volatile int*) 0x0400010C)
#define VGA_STAT ((volatile int*) 0x0400010C)

#define RED 0xE0
#define BROWN 0xA4
#define WHITE 0xFF
#define GREEN 0x1C
#define WIDTH 320

typedef struct Pos {
    int x;
    int y;
} Pos;

typedef struct Cell {
    int type; // 0=Empty,1=Wall,2=Head,3=Body,4=End,5=Apple
    Pos pos;
    struct Cell *nextCell; 
} Cell;

typedef struct Board {
    int rows;
    int colums;
    Cell **cells;
} Board;

Pos newPos(int x, int y){
    Pos pos;
    pos.x = x;
    pos.y = y;
    return pos;
}

Cell newCell(int type, int row, int colum){
    Cell cell;
    cell.type = type;
    cell.pos = newPos(row, colum);
    cell.nextCell = NULL;
    return cell;
}

Board *newBoard(int rows, int colums){
    Board *gameBoard = malloc(sizeof(Board));
    gameBoard->cells = malloc(rows*sizeof(Cell*));
    gameBoard->rows = rows;
    gameBoard->colums = colums;

    for (int x = 0; x < rows; x++){
        gameBoard->cells[x] = malloc(colums*sizeof(Cell));
        for (int y = 0; y < colums; y++){
            gameBoard->cells[x][y] = newCell(0,x,y);//emptyBord
        }
    }
    return gameBoard;
}

void makeWalls(Board* game){
    int wall = 1;
    for (int i = 0; i < game->colums; i++){
        game->cells[0][i].type = wall;
        game->cells[game->rows-1][i].type = wall;
    }
    for (int i = 1; i < game->rows - 1; i++){
        game->cells[i][0].type = wall;
        game->cells[i][game->colums-1].type = wall;
    }
}

void drawPixel(int x, int y, int color){
    volatile char* addr = VGA_START;

    int offset = y * WIDTH + x;
    addr[offset] = color;
}

void drawCell(Cell* model){

    int color;

    switch (model->type)
    {
    case 5:
        color = RED;
        break;
    case 4:
        color = BROWN;
        break;
    case 3:
        color = BROWN;
    case 2:
        color = BROWN;
    case 1:
        color = WHITE;
    case 0:
        color = GREEN;
    }

    for (int x = model->pos.x * 9; x < (model->pos.x * 9) + 9; x++)
    {
        for (int y = model->pos.y * 9; y < (model->pos.y * 9) + 9; y++)
        {
            drawPixel(x,y,color);
        }
    }
}

void drawBoard(Board* game){

    for (int x = 0; x < game->colums; x ++){
        for (int y = 0; y < game->colums; y ++){
            drawCell(&game->cells[x][y]);
        }
    }
}

int main(){
    Board* game = newBoard(20,20);
    makeWalls(game);
    drawBoard(game);

}