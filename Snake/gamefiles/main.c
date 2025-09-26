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
    while (1)
    {
        switch (runMenu())
        {
            case 0:
                runGame();
                runHighscoreLetters();
                break;
            case 1:
                runHighScoreMenu();
                break;
            case 2:
                clearScreen();
                return 0;
                break;  
        }
    } 
}