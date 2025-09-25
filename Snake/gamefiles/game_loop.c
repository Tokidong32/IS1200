#include "../headers/game_loop.h"
#include "../headers/dtekv-lib.h"
#include "../headers/gpio_utils.h"

Board game_board;
Cell *snake_head;
Cell *snake_end;
volatile int game_running;
int snake_lastdir = 4;

int gameTick(Board *game) {

    Cell *next = 0;

    if(snake_lastdir % 2 == 0){
        if(player_button % 2 == 1){
            next = getCellInDirection(game,snake_head,player_button);
            snake_lastdir = player_button;
        }
        else{
            next = getCellInDirection(game,snake_head,snake_lastdir);
        }
    }
    else{
        if(player_button % 2 == 0){
            next = getCellInDirection(game,snake_head,player_button);
            snake_lastdir = player_button;
        }
        else{
            next = getCellInDirection(game,snake_head,snake_lastdir);
        }
    }

    switch (next->type)
    {
    case 1: // krockat med en vägg
      return 0;
      break;
    case 3: // krockat i ormen
      return 0;
      break;
    case 5: // ätit äpple
      currentGameScore += 1;
      newApple(game);
      break;
    default:
            snake_end = moveEnd(snake_end);
      break;
    }
    snake_head = moveHead(snake_head, next);
    return 1;
}


void handle_interrupt(unsigned cause)
{
  *TMR_STAT &= 0;
  game_running = gameTick(&game_board);
  if(game_running == 1){
    drawScore(currentGameScore);
    drawBoard(&game_board);
    *TMR_PRDL = 0x0B20;
    *TMR_PRDH = 0x30;
    *TMR_CTRL |= 1 << 2; // start
  }
  else{
    *TMR_STAT &= 0;
    *TMR_CTRL |= 1 << 3; // stop ?
    clearScreen();
  }  
}
//------------KAN ligga i annan Fill-----------
void timer_init(){
  *TMR_CTRL |= 1; 
  *TMR_PRDL = 0x0B20;
  *TMR_PRDH = 0x30;

  enable_interrupt();
  *TMR_CTRL |= 1 << 2;
}

void runGame(){
  
  game_board.rows = 20;
  game_board.colums = 20;
  game_running = 1;

  for (int y = 0; y < game_board.rows ; y++){
    for (int x = 0; x < game_board.colums ; x++){
      game_board.cells[y][x] = newCell(0,y,x);
    }
  }
  
  game_init(&game_board,&snake_head,&snake_end);
  timer_init();
  
  while (game_running == 1)
  {
   int tmp = (*GPIO_DATA & 0xf);
   switch (tmp)
   {
    case 1: // upp
      player_button = 1;
      break;
    case 2: // höger
      player_button = 2;
      break;
    case 4:  // ner
      player_button = 3;
      break;
    case 8:  // vänster
      player_button = 4;
      break;
    }
  }
  return;
}
