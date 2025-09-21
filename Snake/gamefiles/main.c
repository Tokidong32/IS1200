#include "../headers/dtekv-lib.h"
#include "../headers/globals.h"
#include "../headers/game_loop.h"
#include "../headers/game_logic.h"
#include "../headers/gpio_utils.h"
#include "../headers/main_menu.h"

int main()
{    
    gpioInit();  
    int menuPick = runMenu();
    
    switch (menuPick)
    {
        case 0:
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