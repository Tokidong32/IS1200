#include "../headers/MainMenu.h"

typedef struct MainMenu
{
    int curScore;
    int selector;
}Menu;

Menu newMenu(){
    Menu menu;
    menu.selector = 0;
    menu.curScore = 0;
    return menu;
}
/*

void printAddScore(int score){
    printf("you scored %d.\n", score);
}

void printMenu(Menu* menu){

    printf("-------Awsome Snake Game---------\n");
    for (int i = 0; i < 3; i++)
    {
        if(menu->selector == i) printf(" ---->\t");
        else printf("\t");
        switch (i)
        {
        case 0:
            printf("Play\n");
            break;
        case 1:
            printf("High Score\n");
            break;
        case 2:
            printf("Quit\n");
            break;
        }
    }
}
void moveSelect(Menu* menu, int move){
    switch (move)
    {
    case 1:
        menu->curScore = runSnakeGame();
        printAddScore(menu->curScore);
        break;
    case 2: // move up
        menu->selector = (menu->selector + move) % 3;
        break;
    case 4: //move down
        menu->selector = (menu->selector - 1 + move) % 3;
        break;
    }
}

void main(){
    Menu menu = newMenu();
    moveSelect(&menu, 2);
    printMenu(&menu);
    moveSelect(&menu, 2);
    printMenu(&menu);
    moveSelect(&menu, 2);
    printMenu(&menu);
    moveSelect(&menu, 1);
}
*/