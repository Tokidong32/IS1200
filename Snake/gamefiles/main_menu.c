#include "../headers/main_menu.h"
#include "../headers/globals.h"
#include "../headers/gpio_utils.h"

int moveSelect()
{
    int prev_option = 0;
    int activeOption = 0;
    while (player_button != 0b0010)
    {
        player_button = *GPIO_DATA & 0xf;
        player_button = 1; //*****************REMOVE THIS************* TO MAKE BUTTON WORK
        prev_option = activeOption;
        switch (player_button)
        {
            case 0b0001:
                
                (activeOption <= 0) ? activeOption = 2: activeOption--;
                drawMenuHighlight(activeOption, prev_option);
                break;
            case 0b0100:
                activeOption++;
                activeOption %= 3;
                drawMenuHighlight(activeOption,prev_option);
                break;
        }
    }
    drawMenuHighlight(activeOption,prev_option);

    return activeOption;
}

int runMenu()
{
    drawMainMenu();
    return moveSelect();
}