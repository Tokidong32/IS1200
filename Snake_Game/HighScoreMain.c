#include <stdio.h>
#include <stdlib.h>

typedef struct Placement
{
    int score;
    char name[4];
}Placement;

typedef struct ScoreBoard
{
    Placement placements[5];
}ScoreBoard;

void printPlacment(Placement* place,int order ){
    printf(" %3d   | %5d | %4s\n", order,place->score, place->name);
}

void printScoreBoard(ScoreBoard* scoreBoard){
    printf(" Place | Score | Name \n");
    for(int i = 0; i < 5; i++){
        printPlacment(&scoreBoard->placements[i],i+1);
    }
}

ScoreBoard* newScoreBoard(){
    ScoreBoard* scoreBoard = malloc(sizeof(ScoreBoard));
    return scoreBoard;
}

void newName(char* name) {
    scanf("%3s", name);
}

void addScore(ScoreBoard* scoreBoard, int score){
    int i = 4;
    while ( i >= 0 && scoreBoard->placements[i].score <= score)
    {
        i--;
    }

    if (i < 4) {
    for (int j = 4; j > i + 1; j--) {
        scoreBoard->placements[j] = scoreBoard->placements[j - 1];
    }

    scoreBoard->placements[i + 1].score = score;
    newName(scoreBoard->placements[i + 1].name);
    }
}

int main(){
    ScoreBoard* scoreBoard = newScoreBoard();
    printScoreBoard(scoreBoard);
    addScore(scoreBoard,9);
    addScore(scoreBoard,6);
    addScore(scoreBoard,8);
    addScore(scoreBoard,10);
    addScore(scoreBoard,7);
    printScoreBoard(scoreBoard);
}
