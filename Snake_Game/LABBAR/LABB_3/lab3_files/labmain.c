/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
#include <stdio.h>

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}

void set_leds(int led_mask)
{
  volatile int* LED = (volatile int*)0x04000000;
  *LED = led_mask; 
}

void set_displays(int display_number, int value);


int get_sw()
{
  volatile int* switch_status = (volatile int*)0x04000010;
  return (*switch_status) & 0x3ff;
}

int get_btn()
{
   volatile int* button_status = (volatile int*)0x040000d0;
   return *button_status & 1;
}

/* Your code goes into main as well as any needed functions. */
int main() 
{
  // Call labinit()
  labinit();
  // Enter a forever loop

  /* PART C OF ASSIGNMENT 1
  set_leds(0x7);
  
  while (1)
   {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 1000 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
  }
  
   */

   /*PART D OF ASSIGNMENT 1*/
   int counter = 0;
   while (counter != 16) {
      set_leds(counter);
      time2string( textstring, mytime ); // Converts mytime to string
      display_string( textstring ); //Print out the string 'textstring'
      delay( 100 );          // Delays 1 sec (adjust this value)
      tick( &mytime );     // Ticks the clock once

      counter++;
   }

   int hours = 0;
   while (1)
   {
      set_leds(counter);
      time2string( textstring, mytime ); // Converts mytime to string
      display_string( textstring ); //Print out the string 'textstring'
      delay( 100 );          // Delays 1 sec (adjust this value)
      tick( &mytime );     // Ticks the clock once


      int disp_time = mytime;
      for (int i = 1; i < 5; i++)
      {
         set_displays(i,disp_time&0xf);
         disp_time >>=4;
      }
      if((mytime & 0xffff) == 0)
      {
         hours++;
      }
      set_displays(5,hours%10);
      set_displays(6,(hours/10)%10);

  
      if(get_btn())
      {
        int switch_status = get_sw();

        int number_value = switch_status & (0x3f);

        int turn_off = switch_status & (0x1 << 6);

        int show_display_segment = switch_status & (0x3 << 8);
      
        show_display_segment >>= 8;

          switch(show_display_segment)
          {
          case 0b01:
              set_displays(1, number_value%10);      
              set_displays(2, number_value/10%10);
            break;
          case 0b10:
              set_displays(3, number_value%10);      
              set_displays(4, number_value/10%10);
            break;
          case 0b11:
              set_displays(5, number_value%10);      
              set_displays(6, number_value/10%10);
            break;
          }

        if(turn_off)
        {
          break; 
        }
      }
   } 
}


void set_displays(int display_number, int value)
{
  int offset = 4;
  display_number -= 1; //Display number starts from 1 
  volatile int* adressOfDisplay = (volatile int*)0x04000050+((display_number)*offset);
  volatile int* display = adressOfDisplay;

  
  //print correct value on seven segment
  switch (value)
  {
  case 0:
    *display = 0b1000000;
    break;
  case 1:
    *display = 0b1111001;
    break;
  case 2:
    *display = 0b0100100;
    break;
  case 3:
    *display = 0b0110000;
    break;
  case 4:
    *display = 0b0011001;
    break;
  case 5:
    *display = 0b0010010;
    break;
  case 6:
    *display = 0b0000010;
    break;
  case 7:
    *display = 0b1111000;
    break;
  case 8:
    *display = 0b0000000;
    break;
  case 9:
    *display = 0b0011000;
    break;
  default: 
    *display = 0b0000000;
    break;
  }

  if((display_number+1) % 2 == 0 && ((display_number != 0)))
  {
    *display |= (1 << 7);
  }
}