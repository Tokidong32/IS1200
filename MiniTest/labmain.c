/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void delay(int);
extern int nextprime( int );
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define VGA_START ((volatile char*) 0x08000000)
#define VGA_END ((volatile char*) 0x08000000 + (240 * 320))
#define VGA_CTRL ((volatile int*) 0x0400010C)
#define VGA_STAT ((volatile int*) 0x0400010C)

#define RED 0xE0
#define BROWN 0xE7
#define WHITE 0xFF
#define GREEN 0x1C
#define WIDTH 320

int mytime = 0x5957;
volatile int* pDisplay = (volatile int*) 0x04000050;
volatile int* pStat = (volatile int*)  0x0400010c;

int lastDir = 4;
int score = 0;

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
    Cell cells[20][20];
} Board;

static unsigned int seed = 123456789; // startvärde, kan ändras

unsigned int pseudoRandom() {
    seed = (1103515245 * seed + 12345);
    return (seed >> 16) & 0x7FFF; 
}

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
    int color = model->type;
    switch (color)
    {
    case 5:
        color = RED;
        break;
    case 4:
        color = BROWN;
        break;
    case 3:
        color = BROWN;
        break;
      
    case 2:
        color = BROWN;
        break;
    case 1:
        color = WHITE;
        break;
    case 0:
        color = 0;
        break;
    }
    
    for (int x = (model->pos.x * 9) + 70; x < (model->pos.x * 9) + 79; x++)
    {
        for (int y = (model->pos.y * 9) + 30; y < (model->pos.y * 9) + 39; y++)
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
/* Your code goes into main as well as any needed functions. */

Cell* moveEnd(Cell *from){
    from->type = 0;
    from->nextCell->type = 4;
    Cell *next = from->nextCell;
    from->nextCell = NULL; 
    return next;
}

Cell* moveHead(Cell *from, Cell *to ){
    from->type = 3;
    from->nextCell = to;
    to->type = 2;
    return to;
}
// INTE DET BÄSTA SÄTTET MÖJLIGT
void newApple(Board *game){
    //scana efter godkända Celler
        //en array med pos
    Pos* validPos[(game->rows-2)*(game->colums-2)];
    int nofValidPos = 0;
    for (int i = 1; i < game->rows-1; i++){
        for (int j = 1; j < game->colums-1; j++){
            if(game->cells[i][j].type == 0){
        //om cellens typ är 0 dvs tom läggs en pekare till den cellen in i arrayen
                validPos[nofValidPos++  /*increment antal godkända Celler*/] = &game->cells[i][j].pos;
            }
        }
    }
    int choose = pseudoRandom() % nofValidPos;
    int x = validPos[choose]->x;
    int y = validPos[choose]->y;

    game->cells[x][y].type = 5;
}

Cell* getCellInDirection(Board* game, Cell* from, int dir){
    int posX = from->pos.x;
    int posY = from->pos.y;
    Cell* to;

    switch ( dir ) //vilken riktning var väljd
    {
    case 1: // höger
        to = &game->cells[posX][posY+1];
        break;
    case 3: // vänster
        to = &game->cells[posX][posY-1];
        break;
    case 2: // up
        to = &game->cells[posX-1][posY];
        break;
    case 4: // ner
        to = &game->cells[posX+1][posY];
        break;
    default:
        to = NULL;
      break;
    }
    return to;
}

int gameTick(Board *game, Cell **head, Cell **end, int dir, int* score) {

    Cell *next = NULL;

    if(lastDir % 2 == 0){
        if(dir % 2 == 1){
            next = getCellInDirection(game,*head,dir);
            lastDir = dir;
        }
        else{
            next = getCellInDirection(game,*head,lastDir);
        }
    }
    else{
        if(dir % 2 == 0){
            next = getCellInDirection(game,*head,dir);
            lastDir = dir;
        }
        else{
            next = getCellInDirection(game,*head,lastDir);
        }
    }

    if (next->type > 0 ) // vi har koliderat med något
    {
        if(next->type <= 4) // vi har koliderat med en vägg eller kroppen/ om det är slutet så kommer vi inte kollidera
        {
            return 0;
        }
        // vi har ätit ätt äpple och kommer inte flytta slutet för att växa
        *score += (*score + 1);
        newApple(game);
    }
    else{ // vi kryper på en tom ruta och flyttar slutet för att inte växa
        *end = moveEnd((*end));
    }
    *head = moveHead((*head), next);

    return 1;
}

void startPos(Board *game, Cell **head, Cell **end){
    int middelY = game->colums/2;
    int middelX = game->rows/2;

    (*head) = &game->cells[middelX][middelY];
    game->cells[middelX][middelY].type = 2;
    (*end) = &game->cells[middelX-1][middelY];
    game->cells[middelX-1][middelY].type = 4;
    (*end)->nextCell = *head;
    game->cells[middelX+2][middelY].type = 5;
}

extern void enable_interrupt();

#define TMR_STAT ((volatile short*)0x04000020)
#define TMR_CTRL ((volatile short*)0x04000024)
#define TMR_PRDL ((volatile short*)0x04000028)
#define TMR_PRDH ((volatile short*)0x0400002C)

#define GPIO_DATA ((volatile int*)0x040000e0)
#define GPIO_DIRE ((volatile int*)0x040000e4)

Board* game;
Cell* head;
Cell* end;
int btn = 0;

void handle_interrupt(unsigned cause)
{
  *TMR_STAT &= 0;
  *TMR_PRDL = 0x0B20;
  *TMR_PRDH = 0x20;

  gameTick(game,&head,&end,btn,&score);
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

void GPIOInit(){
  *GPIO_DIRE = 0xf;
}

/* This is the main function */
int main()
{
    int score = 0;
    Board newGame;
    Cell* newHead;
    Cell* newEnd;
    for (int i = 0; i < 20; i++)
    {
      for (int j = 0; j < 20; j++)
      {
        newGame.cells[i][j] = newCell(0,i,j);
      }
    }
    newGame.rows = 20;
    newGame.colums = 20;

    game = &newGame;
    
    startPos(game,&newHead,&newEnd);
    head = newHead;
    end = newEnd;
    
    makeWalls(game);

    timer_init();
    GPIOInit();

   while (1)
    {
        if(tick(game,head,end,3,&score)){
            printBoard(game);
            printf("\n");

        }else{
            printf("gameOver");
            freeBoard(game);
            break;
        }
    }
  score = score+0;
}

   
    