#include <stdio.h>
#include <stdlib.h>

typedef struct Cell
{
    int type; // 0=Empty,1=Wall,2=Head,3=Body,4=End,5=Apple
    int posX;
    int posY;
    struct Cell *nextCell; 
}Cell;

typedef struct GameBoard
{
    int rows;
    int colums;
    Cell **board;
    
}GameBoard;

Cell newCell(int type, int rows, int colums){
    Cell cell;
    cell.type = type;
    cell.posX = rows;
    cell.posY = colums;
    cell.nextCell = NULL;
    return cell;
}

GameBoard *newBoard(int rows, int colums){
    GameBoard *gameBoard = malloc(sizeof(GameBoard));
    gameBoard->board = malloc(rows*sizeof(Cell*));
    gameBoard->rows = rows;
    gameBoard->colums = colums;

    for (int i = 0; i < rows; i++){
        gameBoard->board[i] = malloc(colums*sizeof(Cell));
        for (int j = 0; j < colums; j++){
            gameBoard->board[i][j] = newCell(0,i,j);//emptyBord
        }
    }
    return gameBoard;
}

void makeWalls(GameBoard *game){
    int wall = 1;
    for (int i = 0; i < game->colums; i++){
        game->board[0][i].type = wall;
        game->board[game->rows-1][i].type = wall;
    }
    for (int i = 1; i < game->rows - 1; i++){
        game->board[i][0].type = wall;
        game->board[i][game->colums-1].type = wall;
    }
}

void printBoard(GameBoard *game){
    for (int i = 0; i < game->rows; i++){
        for (int j = 0; j < game->colums; j++){
            printf("[%d]",game->board[i][j].type);
        }
        printf("\n");
    }
    
}

void moveCell(Cell *from, Cell *to){
    from->nextCell = to;
    to->type = 2; 
}

void tick(GameBoard *game, Cell *snakeHead, Cell *snakeEnd, int direction){

    switch (direction)
    {
    case 1: // höger
        moveCell(snakeHead, &game->board[snakeHead->posY][snakeHead->posX + 1]);
        snakeHead = &game->board[snakeHead->posY][snakeHead->posX + 1];
        break;
    case 2: // vänster
        moveCell(snakeHead, &game->board[snakeHead->posY][snakeHead->posX - 1]);
        snakeHead = &game->board[snakeHead->posY][snakeHead->posX-1];
        break;
    case 3: // up
        moveCell(snakeHead, &game->board[snakeHead->posY-1][snakeHead->posX]);
        snakeHead = &game->board[snakeHead->posY-1][snakeHead->posX];
        break;
    case 4: // ner
        moveCell(snakeHead, &game->board[snakeHead->posY+1][snakeHead->posX]);
        snakeHead = &game->board[snakeHead->posY+1][snakeHead->posX];
        break;
    default:
        break;
    }
}

int main(){
    int rows = 8;
    int colums = 8;
    
    GameBoard *game = newBoard(rows,colums);
    makeWalls(game);

    Cell *snakeHead = &game->board[3][3];
    game->board[3][3].type = 2;
    Cell *snakeEnd = &game->board[3][4];
    game->board[3][4].type = 4;
    snakeEnd->nextCell = snakeHead;

    printBoard(game);
    printf("\n");
    tick(game,snakeHead,snakeEnd,2);

    printBoard(game);
    printf("\n");
    tick(game,snakeHead,snakeEnd,3);

    printBoard(game);
    printf("\n");
    tick(game,snakeHead,snakeEnd,1);

    printBoard(game);
}