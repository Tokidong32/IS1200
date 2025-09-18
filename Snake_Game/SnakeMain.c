#include <stdio.h>
#include <stdlib.h>


typedef struct Cell
{
    int type; // 0=Empty,1=Wall,2=Head,3=Body,4=End,5=Apple
    int posX;
    int posY;
    struct Cell *nextCell; 
}Cell;

typedef struct Board
{
    int rows;
    int colums;
    Cell **cells;
    
}Board;


Cell newCell(int type, int rows, int colums){
    Cell cell;
    cell.type = type;
    cell.posX = rows;
    cell.posY = colums;
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

Board *game;
Cell *snakeHead;
Cell *snakeEnd;

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
    return from->nextCell;
}

Cell* moveHead(Cell *from, Cell *to ){
    from->type = 3;
    from->nextCell = to;
    to->type = 2;
    return to;
}

void newApple(Board *game){
    //scana efter godkända Celler
        //en array med Cell pekare
        //om cellens typ är 0 dvs tom läggs en pekare till den cellen in i arrayen
        //increment antal godkända Celler

    //välj en av de godkända Cellr
        //rand() %  antal godkända Celler
        
    //ändra cellen till ett äpple
        //Cell->type = 5;
}

void tick(Board *game, Cell **head, Cell **end, int dir) {
    int posX = (*head)->posX;
    int posY = (*head)->posY;

    Cell *next = NULL;
    switch (dir)
    {
    case 1:
        next = &game->cells[posX][posY+1];
        break;
    case 2:
        next = &game->cells[posX][posY-1];
        break;
    case 3: // up
        next = &game->cells[posX-1][posY];
        break;
    case 4: // ner
        next = &game->cells[posX+1][posY];
        break;
    default:
        // ERROR
        break;
    }

    if (next->type > 0 ) // vi har koliderat med något
    {
        if(next->type <= 4) // vi har koliderat med en vägg eller kroppen/ om det är slutet så kommer vi inte kollidera
        {
            //GAMEOVER
        }
        // vi har ätit ätt äpple och kommer inte flytta slutet för att växa
    }
    else{ // vi kryper på en tom ruta och flytar slutet för att inte växa
        *end = moveEnd((*end));
    }
    
    *head = moveHead((*head), next);
}




int main(){
    int rows = 8;
    int colums = 8;

    game = newBoard(rows,colums);
    makeWalls(game);
    snakeHead = malloc(sizeof(Cell*));
    snakeEnd = malloc(sizeof(Cell*));

    game->cells[2][2].type = 5;
    snakeHead = &game->cells[3][3];
    game->cells[3][3].type = 2;
    snakeEnd = &game->cells[3][4];
    game->cells[3][4].type = 4;
    snakeEnd->nextCell = snakeHead;

    printBoard(game);
    printf("\n");

    tick(game,&snakeHead,&snakeEnd,2);
    printBoard(game);

    tick(game,&snakeHead,&snakeEnd,3);
    printBoard(game);

    tick(game,&snakeHead,&snakeEnd,1);
    printBoard(game);
}