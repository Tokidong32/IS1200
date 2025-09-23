#include "../headers/game_loop.h"
#include "../headers/dtekv-lib.h"
#include "../headers/gpio_utils.h"

Board *game_board;
Cell *snake_head;
Cell *snake_end;

int snake_lastdir = 4;

int end_game = 0;

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
        currentGameScore += 1;
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
    drawScore(currentGameScore);
    drawBoard(game_board);
    *TMR_CTRL |= 1 << 2;
  }
  else
  {
    *TMR_CTRL |= 1 << 3;
  }  
}

void runGame()
{
  end_game = 0;

  Board newBoard;
  Cell* newHead;
  Cell* newEnd;
  
  newBoard.rows = 20;
  newBoard.colums = 20;

  for (int y = 0; y < newBoard.rows ; y++){
    for (int x = 0; x < newBoard.colums ; x++){
      newBoard.cells[y][x] = newCell(0,y,x);
    }
  }
  
  game_init(&newBoard,&newHead,&newEnd);

  game_board = &newBoard;
  snake_head = newHead;
  snake_end = newEnd;

  timerInterruptInit();
  while (1)
  { 
    int tmp = (*GPIO_DATA & 0xf);
    switch (tmp)
    {
    case 1:
      player_button = 1;
      break;
    case 2:
      player_button = 2;
      break;
    case 4:
      player_button = 3;
      break;
    case 8:
      player_button = 4;
      break;
    case 15:
      end_game = 1;
    }
    if(end_game)
    {
      break;
    }
  }
}
