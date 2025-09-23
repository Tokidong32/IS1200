#include "../headers/main_menu.h"
#include "../headers/globals.h"
#include "../headers/gpio_utils.h"
#include "../headers/timer_utils.h"

int moveSelect()
{
    int prev_option = 0;
    int activeOption = 0;

    while (player_button != 0b0010)
    {
        player_button = *GPIO_DATA & 0xf;
       
        if(*TMR_STAT & 1)
        {
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
            runTimer();
        }
    }
    drawMenuHighlight(activeOption,prev_option);

    resetTimer();

    return activeOption;
}

int runMenu()
{
    resetTimer();

    drawMainMenu();
        
    runTimer();
    return moveSelect();
}