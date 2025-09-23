#include "../headers/dtekv-lib.h"
#include "../headers/globals.h"
#include "../headers/game_loop.h"
#include "../headers/game_logic.h"
#include "../headers/gpio_utils.h"
#include "../headers/main_menu.h"
#include "../headers/highscore_menu.h"

int main()
{    
    clearScreen();
    
    gpioInit();  
    runHighscoreLetters();
    print(players.players[0].choosenLetter);
    //drawLetter('C',100,100,1);
    /*
    while (1)
    {
    int menuPick = runMenu();
    
        switch (menuPick)
        {
            case 0:
                clearScreen();
                runGame();
                clearScreen();

                break;
            case 1:
                //highScoreMenu();
                clearScreen();
                break;
            case 2:
                clearScreen();
                return 0;
                break;  
        }
    } 
        */
}