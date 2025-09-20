#include "../headers/dtekv-lib.h"
#include "../headers/globals.h"
#include "../headers/game_loop.h"
#include "../headers/game_logic.h"
#include "../headers/gpio_utils.h"

int main()
{    
    gpio_Init();

    run_game();
    print("\n");

    run_game();
    print("\n");

    run_game();


    print("\n");
    print("Stopped");
}