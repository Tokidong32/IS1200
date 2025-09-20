#ifndef game_logic

#define game_logic


// ====================== Structar ======================
typedef struct Pos {
    int x;
    int y;
} Pos;

typedef struct Cell {
    int type; // 0=Floor, 1=Wall, 2=Head, 3=Body, 4=End, 5=Apple
    Pos pos;
    struct Cell *nextCell; 
} Cell;

typedef struct Board {
    int rows;
    int colums;
    Cell cells[20][20];
} Board;

// ====================== Funktioner ======================

// Helpers
Pos newPos(int x, int y);
Cell newCell(int type, int row, int colum);

// Board
Board *newBoard(int rows, int colums);
void freeBoard(Board *board);
void makeWalls(Board *game);

// Snake rörelse
Cell* moveEnd(Cell *from);
Cell* moveHead(Cell *from, Cell *to);
Cell* getCellInDirection(Board* game, Cell* from, int dir);
void game_init(Board *game, Cell **head, Cell **end);

// Äpple
void newApple(Board *game);

#endif 