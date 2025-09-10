/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

int* list1;
int* list2; 

int counter=0;

void copycodes(char* text,int* list,int* counter)
{
  while(*text != 0)
  {
    char c = *text;    
    *list = c;

    list++;
    text++;

    *counter = *counter + 1;
  }
}

void work()
{

  copycodes(text1, list1, &counter);
  copycodes(text2, list2, &counter);
}



void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
    
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}

int main(void){
  list1 = (int*) malloc(80); //80 bytes
  list2 = (int*) malloc(80); //80 bytes
  
  work();
    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", counter);
    
    endian_proof((char*) &counter);

    free(list1);
    free(list2);
}
