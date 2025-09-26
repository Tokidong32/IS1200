#include "../headers/dtekv-lib.h"
#include "../headers/highscore_menu.h"
#include "../headers/timer_utils.h"
#include "../headers/gpio_utils.h"
#include "../headers/globals.h"
#include "../headers/vga_utils.h"

const char availableLetters[7] = {'A','B','C','1','2','3'};

const int letterPlacementY = RESOLUTION_Y/2-21;
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
    
    case '4':   
        *firstIndexOfChar   =  (13+13+12+17+13+14)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14+16)*21);
        break;
    
    case '5':   
        *firstIndexOfChar   =  (13+13+12+17+13+14+16)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14+16+12)*21);
        break;
    
    case '6':   
        *firstIndexOfChar   =  (13+13+12+17+13+14+16+12)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14+16+12+14)*21);
        break;
    
    case '7':   
        *firstIndexOfChar   =  (13+13+12+17+13+14+16+12+14)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14+16+12+14+13)*21);
        break;
    
    case '8':   
        *firstIndexOfChar   =  (13+13+12+17+13+14+16+12+14+13)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14+16+12+14+13+13)*21);
        break;
        
    case '9':   
        *firstIndexOfChar   =  (13+13+12+17+13+14+16+12+14+13+13)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14+16+12+14+13+13+14)*21);
        break;

    case '0':   
        *firstIndexOfChar   =  (13+13+12+17+13+14+16+12+14+13+13+14)*21;
        *lastIndexOfChar    = ((13+13+12+17+13+14+16+12+14+13+13+14+14)*21);
        break;
    }    
}


void makeLetterMenu()
{   
    int i = 4;
    while (i >= 0 && currentGameScore >= players.players[i].playerScore){
       i--;
    }

    int hole = i+1;

    for (int j = 3; j > i; j--){
        players.players[j+1] = players.players[j];
    }

    PlayerScore* curplayer = &players.players[hole];
    
    curplayer->playerScore = currentGameScore;

    curplayer->choosenLetter[0] = availableLetters[0];
    curplayer->choosenLetter[1] = availableLetters[1];
    curplayer->choosenLetter[2] = availableLetters[2];
    curplayer->choosenLetter[3] = availableLetters[3];
    curplayer->choosenLetter[4] = availableLetters[4];
    curplayer->choosenLetter[5] = availableLetters[5];


  
    int letterPlacementX = charOffsetX;
    int activeOption = 0;
    int indx = 0;
    int counter = 0;
    while (indx != 6)
    {
        player_button = *GPIO_DATA & 0xf;
       
        if(*TMR_STAT & 1)
        {
            if(counter++ % 5)
            {
                drawHighscoreHighlight(BLACK,indx-1,indx-1);
                drawHighscoreHighlight(DARKGREEN,indx,indx);
                drawHighscoreHighlight(BLACK,indx+1,indx+1);
            }
            else
            {
                drawHighscoreHighlight(BLACK,indx,indx);
            } 

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
                if(letterPlacementX <= charOffsetX+(20*6)-20)
                {
                    letterPlacementX += 20;
                    indx++;
                }
                break;

            case 0b1000:
                if(letterPlacementX > charOffsetX)
                {
                    letterPlacementX -= 20;
                    indx--;
                }
                break;
            }
            runTimer(); 
        }
    }
    resetTimer();    
    clearScreen();
}

void runHighscoreLetters()
{
    resetTimer(); 
    runTimer();
    makeLetterMenu();
    player_button = 0;
    delay(100);
    clearScreen();
}

void runHighScoreMenu()
{
    clearScreen();
        
    for (int eachPlayer = 0; eachPlayer < 5; eachPlayer++){
        for (int eachLetter = 0; eachLetter < 6; eachLetter++){
            drawLetter(players.players[eachPlayer].choosenLetter[eachLetter],eachLetter*20+RESOLUTION_X/6,eachPlayer*30+90, 0);
        }
    }

    for (int eachPlayer = 0; eachPlayer < 5; eachPlayer++){

        int score = players.players[eachPlayer].playerScore;

        if(score >= 0)
        {
            //DRAW EACH NUMBER
            drawNumber(score%10,RESOLUTION_X/6+200,eachPlayer*30+90,0); // FIRST DIGIT
            drawNumber(score/10%10,RESOLUTION_X/6+180,eachPlayer*30+90,0);   
            drawNumber(score/100%10,RESOLUTION_X/6+160,eachPlayer*30+90,0);
            drawNumber(score/1000%10,RESOLUTION_X/6+140,eachPlayer*30+90,0); //LAST DIGIT
        }
    }

    while (player_button != 8){
        player_button = *GPIO_DATA & 0xf;
    }
    delay(100);
    player_button = 0;
    clearScreen();
}