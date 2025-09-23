#ifndef highscore_menu
#define highscore_menu

typedef struct PlayerScore
{
    char choosenLetter[7];
    int playerScore;
}PlayerScore;

typedef struct AllPlayedPlayers
{
   PlayerScore players[5];
   int currentEmptyIndx;
}AllPlayedPlayers;


void getLetterFromArray(char charToFind,int *firstIndexOfChar, int *lastIndexOfChar);

void runHighscoreLetters();
#endif
