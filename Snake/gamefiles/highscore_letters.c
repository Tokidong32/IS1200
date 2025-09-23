#include "../headers/highscore_menu.h"
#include "../headers/timer_utils.h"
#include "../headers/gpio_utils.h"
#include "../headers/globals.h"
#include "../headers/vga_utils.h"

const char availableLetters[7] = {'A','B','C','1','2','3'};

const int letterPlacementY = RESOLUTION_Y/2;
const int charOffsetX = RESOLUTION_X/4;

//Ugly hash"ish" function that recieves the wanted letter from 1-3 and A-C and brings the corresponding first index and last index for that letter in the array
void getLetterFromArray(char charToFind,int *firstIndexOfChar, int *lastIndexOfChar)
{
    switch (charToFind)
    {
    case '1':
        *firstIndexOfChar   = 0;
        *lastIndexOfChar    = ((13)*21);
        break;  
    case '2':   
        *firstIndexOfChar   =  (13)*21;
        *lastIndexOfChar    = ((13+13)*21);
        break;  
    case '3':   
        *firstIndexOfChar   =  (13+13)*21;
        *lastIndexOfChar    = ((13+13+12)*21);
        break;  
    case 'A':   
        *firstIndexOfChar   =  (13+13+12)*21;
        *lastIndexOfChar    = ((13+13+12+17)*21);
        break;  
    case 'B':   
        *firstIndexOfChar   =  (13+13+12+17)*21;
        *lastIndexOfChar    = ((13+13+12+17+13)*21);
        break;  
    case 'C':   
        *firstIndexOfChar   =  (13+13+12+17+13)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14)*21);
        break;
    }
}


void makeMenu()
{   
    PlayerScore* curplayer = &players.players[players.currentEmptyIndx];
    curplayer->playerScore = currentGameScore;

    int activeOption = 0;
    int letterPlacementX = charOffsetX;
    int indx = 0;

    while (indx < 6)
    {
        player_button = *GPIO_DATA & 0xf;
       
        if(*TMR_STAT & 1)
        {
            switch (player_button)
            {
            case 0b0001:
                drawLetter(availableLetters[activeOption],letterPlacementX,letterPlacementY,1);

                (activeOption <= 0) ? activeOption = 5: activeOption--;
                curplayer->choosenLetter[indx] = availableLetters[activeOption];

                drawLetter(availableLetters[activeOption],letterPlacementX,letterPlacementY,0);
                break;

            case 0b0100:
                drawLetter(availableLetters[activeOption],letterPlacementX,letterPlacementY,1);

                (activeOption >= 5) ? activeOption = 0: activeOption++;
                curplayer->choosenLetter[indx] = availableLetters[activeOption];

                drawLetter(availableLetters[activeOption],letterPlacementX,letterPlacementY,0);
                break;

            case 0b0010:
                (letterPlacementX <= charOffsetX+(20*6)-20) ? letterPlacementX += 20:0;
                indx ++;
                indx %= 6;
                break;

            case 0b1000:
                (letterPlacementX >= (charOffsetX-20)+1) ? letterPlacementX -= 20:0;
                indx --;
                indx %= 6;
                break;
            }
            runTimer(); 
        }
    }
    players.currentEmptyIndx++;
    players.currentEmptyIndx %= 5;
    resetTimer();
}

void runHighscoreLetters()
{
    resetTimer(); 
    runTimer();
    makeMenu();
}
