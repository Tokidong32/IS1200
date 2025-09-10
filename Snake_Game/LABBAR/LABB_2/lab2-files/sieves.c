#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define COLUMNS 6

int j = 1;

void print_number(int n)  //The side effect is that i change a global variable in order to keep track of the correct column. 
{
  printf("%10d ", n);

  if(j % COLUMNS == 0)
  {
    printf("\n");
  }

  j++;
}

void print_sieves(int n)
{
    int A[n+1]; 

    /*
    ADDs the numbers from 2 to n to the array
    */
    for (int i = 0; i <= n; i++)
    {
        A[i] = i;
    }
    
    A[1] = 0; // This marks the 1 as a zero
    
    for (int p = 2; p <= sqrt(n); p++)
    {
        for (int i = 2*p; i <= n; i += p)
        {
            A[i] = 0;            
        }
    }



    for (int i = 0; i <= n; i++)
    {
        if(A[i] != 0)
        {
            print_number(A[i]);
        }
    }
    
}
// The Stack is not large enough to store enough numbers to achivce a 2 second wait time. Largest was 999 999
int main(int argc, char *argv[])
{
    if(argc == 2)
        print_sieves(atoi(argv[1]));

    else
        printf("Please state an integer number.");

  return 0;
}
