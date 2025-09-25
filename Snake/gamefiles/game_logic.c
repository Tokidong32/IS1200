#include "../headers/game_logic.h"
#include "../headers/globals.h"

static unsigned int seed = 123456789;

unsigned int pseudoRandom() {
    seed = (1103515245 * seed + 12345) & 0x7fffffff; 
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
    cell.nextCell = 0;
    return cell;
}

void makeWalls(Board* game){
    int wall = 1;

    for (int i = 0; i < game->rows; i++){
        game->cells[i][0].type = wall;
        game->cells[i][game->colums-1].type = wall;
    }

    for (int i = 1; i < game->colums-1; i++){
        game->cells[0][i].type = wall;
        game->cells[game->rows-1][i].type = wall;
    }

}


Cell* moveEnd(Cell *from){
    from->type = 0;
    from->nextCell->type = 4;
    Cell *next = from->nextCell;
    from->nextCell = 0; 
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
    Cell* validPos[(game->rows-2)*(game->colums-2)];
    int nofValidPos = 0;
    for (int y = 1; y < game->rows-1; y++){
        for (int x = 1; x < game->colums-1; x++){
            if(game->cells[y][x].type == 0){
        //om cellens typ är 0 dvs tom läggs en pekare till den cellen in i arrayen
                validPos[nofValidPos++] = &game->cells[y][x];
            }
        }
    }
    int choose = pseudoRandom() % nofValidPos;
    validPos[choose]->type = 5;
}

Cell* getCellInDirection(Board* game, Cell* from, int dir){
    int posX = from->pos.x;
    int posY = from->pos.y;
    Cell* to;

    switch ( dir ) //vilken riktning var väljd
    {
    case 0b0001: // up
        to = &game->cells[posX-1][posY];
        break;
    case 0b0010: // höger
        to = &game->cells[posX][posY+1];
        break;
    case 0b0100: // ner
        to = &game->cells[posX+1][posY];
        break;
    case 0b1000: // vänster
        to = &game->cells[posX][posY-1];
        break;
    default:
        to = 0;
      break;
    }
    return to;
}

void game_init(Board *game, Cell **head, Cell **end)
{
    currentGameScore = 0;

    int boardcol = game->colums;
    int boardrow = game->rows;

    //creates snake_head at 1/3 of row and 1/2 of coloumns
    *head = &game->cells[boardrow/3][boardcol/2];
    game->cells[boardrow/3][boardcol/2].type = 2;

    //creates and links snake_end with snake_head
    *end = &game->cells[(boardrow/3)-1][boardcol/2];
    game->cells[(boardrow/3)-1][boardcol/2].type = 4;
    (*end)->nextCell = *head;

    //Places first apple
    game->cells[(boardrow*2/3)][boardcol/2].type = 5;

    makeWalls(game);
}
