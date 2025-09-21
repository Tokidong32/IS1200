#include "../headers/main_menu.h"
#include "../headers/globals.h"

int moveSelect(int active_option)
{
    while (player_button != 0b0010)
    {
        switch (player_button)
        {
            case 0b0001:
                active_option--;
                active_option %= 3;
                
                drawMainMenu(active_option);
                clearScreen();
                break;
            case 0b0100:
                active_option++;
                active_option %= 3;
                
                drawMainMenu(active_option);
                clearScreen();
                break;
        }
    }
    clearScreen();
    drawMainMenu(active_option);

    return active_option;
}

int runMenu()
{
    int active_options = 0;

    return moveSelect(active_options);
}