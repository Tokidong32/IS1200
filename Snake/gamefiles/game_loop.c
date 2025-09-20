#include "../headers/game_loop.h"
#include "../headers/dtekv-lib.h"

Board *game_board;
Cell *snake_head;
Cell *snake_end;

int snake_lastdir = 4;

int gameTick(Board *game, Cell **head, Cell **end, int next_dir) {

    Cell *next = 0;

    if(snake_lastdir % 2 == 0){
        if(next_dir % 2 == 1){
            next = getCellInDirection(game,*head,next_dir);
            snake_lastdir = next_dir;
        }
        else{
            next = getCellInDirection(game,*head,snake_lastdir);
        }
    }
    else{
        if(next_dir % 2 == 0){
            next = getCellInDirection(game,*head,next_dir);
            snake_lastdir = next_dir;
        }
        else{
            next = getCellInDirection(game,*head,snake_lastdir);
        }
    }

    if (next->type > 0 ) // vi har koliderat med något
    {
        if(next->type <= 4) // vi har koliderat med en vägg eller kroppen/ om det är slutet så kommer vi inte kollidera
        {
            return 0;
        }
        // vi har ätit ätt äpple och kommer inte flytta slutet för att växa
        player_score += 1;
        newApple(game);
    }
    else{ // vi kryper på en tom ruta och flyttar slutet för att inte växa
        *end = moveEnd((*end));
    }
    *head = moveHead((*head), next);

    return 1;
}

void handle_interrupt(unsigned cause)
{
  *TMR_STAT &= 0;
  *TMR_PRDL = 0x0B20;
  *TMR_PRDH = 0x20;

  if(gameTick(game_board, &snake_head, &snake_end, player_button))
  {
    drawBoard(game_board);
    *TMR_CTRL |= 1 << 2;
  }
  else
  {
    *TMR_CTRL |= 1 << 3;
  }  
}

void timer_init()
{
  *TMR_CTRL |= 1; 

  *TMR_PRDL = 0x0B20;
  *TMR_PRDH = 0x20;

  enable_interrupt();
  *TMR_CTRL |= 1 << 2;
}


int run_game()
{
  Board newBoard;
  Cell* newHead;
  Cell* newEnd;
  
  newBoard.rows = 20;
  newBoard.colums = 20;

  for (int x = 0; x < newBoard.rows ; x++)
  {
    for (int y = 0; y < newBoard.colums ; y++)
    {
      newBoard.cells[x][y] = newCell(0,x,y);
    }
  }
  
  game_init(&newBoard,&newHead,&newEnd);

  game_board = &newBoard;
  snake_head = newHead;
  snake_end = newEnd;

  timer_init();
  
  while (1)
  { 
    if(!(*TMR_STAT & 2))
    {
      print_dec(player_score);
      return 0;
    }
  }
  return 1;
}
