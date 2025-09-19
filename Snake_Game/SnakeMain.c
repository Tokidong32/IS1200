#include "SnakeMain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int lastDir = 4;

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

    for (int i = 0; i < rows; i++){
        gameBoard->cells[i] = malloc(colums*sizeof(Cell));
        for (int j = 0; j < colums; j++){
            gameBoard->cells[i][j] = newCell(0,i,j);//emptyBord
        }
    }
    return gameBoard;
}

void freeBoard(Board *board){
    for (int i = 0; i < board->rows; i++)
    {
        free(board->cells[i]);
    }
    free(board->cells);
    free(board);
}

void makeWalls(Board *game){
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

void printBoard(Board *game){
    for (int i = 0; i < game->rows; i++){
        for (int j = 0; j < game->colums; j++){
            printf("[%d]",game->cells[i][j].type);
        }
        printf("\n");
    }
    printf("\n");
}

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
    srand(time(NULL));
    int choose = rand() % nofValidPos;
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
    }
    return to;
}

int tick(Board *game, Cell **head, Cell **end, int dir, int* score) {
    int posX = (*head)->pos.x;
    int posY = (*head)->pos.y;

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
        *score++;
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

int main(){
    int rows = 19;
    int colums = 19;
    int score = 0;
    
    Board *game = newBoard(rows,colums);
    makeWalls(game);
    Cell *snakeHead;
    Cell *snakeEnd;
    startPos(game,&snakeHead,&snakeEnd);
    printBoard(game);

    while (1)
    {
        if(tick(game,&snakeHead,&snakeEnd,3,&score)){
            printBoard(game);
            printf("\n");

        }else{
            printf("gameOver");
            freeBoard(game);
            break;
        }
    }
    
    return score;
}
