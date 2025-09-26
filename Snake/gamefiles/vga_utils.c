#include "../headers/vga_utils.h"
#include "../headers/main_menu_image.h"
#include "../headers/highscore_letters_image.h"

#include "../headers/dtekv-lib.h"

void drawPixel(int x, int y, int color)
{
    volatile char* addr = VGA_START;

    int offset = (y * RESOLUTION_X) + x;
    addr[offset] = color;
}

void drawCell(Cell* model){

    int color = 0;
    switch (model->type)
    {
    //Apple colour
    case 5:
        color = RED;
        break;
    //Snake_end colour
    case 4:
        color = GREEN;
        break;
    //Snake_body colour
    case 3:
        color = GREEN;
        break;
    //Snake_head colour
    case 2:
        color = GREEN;
        break;
    //Wall colour
    case 1:
        color = WHITE;
        break;
    //Floor colour
    case 0:
        color = BLACK;
        break;
    }

    for (int x = (model->pos.x * 9) + 70; x < (model->pos.x * 9) + 79; x++)
    {
        for (int y = (model->pos.y * 9)+30; y < (model->pos.y * 9) + 39; y++)
        {
            drawPixel(x,y,color);
        }
    }
}

void drawBoard(Board* game){

    for (int x = 0; x < game->rows; x ++){
        for (int y = 0; y < game->colums; y ++){
            drawCell(&game->cells[x][y]);
        }
    }
}

void clearScreen()
{
    volatile char* addr = VGA_START;
    for (int i = 0; i < RESOLUTION_X*RESOLUTION_Y; i++)
    {
        addr[i] = BLACK;
    }
}

void drawMenuHighlight(int highlight,int lastHighlight)
{    
    /*
    *
    * Remove old lines by overwriting old line with black colour.
    *
    * Function replaced switch case to determine where the line should be drawn in the y axis
    *  (RESOLUTION_Y/8) places the y axis within where the menu is drawn
    *  Then the rest is a linear function to place the line at the correct y axis 
    */
    float prevLineFloat = (RESOLUTION_Y/8)+ MAIN_MENU_RES_Y*(((10*lastHighlight)+19.0)/40)+10;
    int prevLineInt = (int)prevLineFloat;
    
    int offsetX = RESOLUTION_X/4;

    for (int x = MAIN_MENU_RES_X/10; x < MAIN_MENU_RES_X-MAIN_MENU_RES_X/10; x++)
    {
        drawPixel(x+offsetX,prevLineInt,BLACK);
        drawPixel(x+offsetX,prevLineInt+1,BLACK);
        drawPixel(x+offsetX,prevLineInt+2,BLACK);
    }

    /*
    *   draw the newly selected line with the new highlight value
    */
    float newLineFloat = (RESOLUTION_Y/8)+ MAIN_MENU_RES_Y*(((10*highlight)+19.0)/40)+10;
    int newLineInt = (int)newLineFloat;

    for (int x = MAIN_MENU_RES_X/10; x < MAIN_MENU_RES_X-MAIN_MENU_RES_X/10; x++)
    {
        drawPixel(x+offsetX,newLineInt,GREEN);
        drawPixel(x+offsetX,newLineInt+1,GREEN);
        drawPixel(x+offsetX,newLineInt+2,GREEN);
    }
}

void drawMainMenu()
{
    //placement
    int offsetX = RESOLUTION_X/4;
    int offsetY = RESOLUTION_Y/8;

    //Draw x and y of main menu image
    for (int x = 0; x < MAIN_MENU_RES_X; x++)
    {
        for (int y = 0; y < MAIN_MENU_RES_Y; y++)
        {   
            /*
            Turn multicolor image to BW
            image_data is the binary image as a linear array
            */
            if(image_data[y*MAIN_MENU_RES_X+x]) 
            {
                drawPixel(x+offsetX,y+offsetY,WHITE);
            }
            else
            {
                drawPixel(x+offsetX,y+offsetY,BLACK);
            }
        }
    }
}

/*
*   Draw letter Highlight in highscore menu
*/
void drawHighscoreHighlight(int color,int highlight,int lastHighlight)
{    
    /*
    * Remove previous line
    */

    int offsetX = RESOLUTION_X/4;
    int offsetY =  RESOLUTION_Y/2;

    offsetX += lastHighlight*20;

    for (int x = 0; x < 18; x++)
    {
        drawPixel(x+offsetX,offsetY,  BLACK);
        drawPixel(x+offsetX,offsetY+1,BLACK);
    }

    /*
    * add new line
    */
    offsetX = RESOLUTION_X/4;
    offsetX += highlight*20;

    for (int x = 0; x < 18; x++)
    {
        drawPixel(x+offsetX,offsetY,  color);
        drawPixel(x+offsetX,offsetY+1,color);
    }
}
/*
*
* place or remove a letter between 1-3 and A-C at offset X and Y
*
*/
void drawLetter(char letter, int offsetX, int offsetY, char remove){
    int firstIndx = 0;
    int lastIndx = 0;

    getLetterFromArray(letter, &firstIndx, &lastIndx);
    /*
    * The difference between last and first index is the area for the letter
    * Allowing us to calculate the width
    * 
    * If we are removing an object we always want to ensure that the letter is gone by using a width of 19
    */
    int width = 0;
    (remove == 1 ? width = 18 : (width = (lastIndx-firstIndx)/21));

   
    int height = 21;

    //Different width for the pictures of each letter. Have to adjust to correct width
    //Height is always 21 pixels   
    
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            //y*width+x turns the XY into correct consecutive string for drawpixel. firstIndx is added to draw correct letter from array
            switch (remove)
            {
            case 1:
                drawPixel(x+offsetX,y+offsetY,BLACK);
                break;
            default:
                if(letter_images[y*width+x+firstIndx]) {
                    drawPixel(x+offsetX,y+offsetY,WHITE);
                }
                else{
                    drawPixel(x+offsetX,y+offsetY,BLACK);
                }
                break;
            }
        }
    }
}


/*
skriver ut en siffra på rätt plats på spel skärmen
int value: är siffran som ska skrivas ut
int offset: är förskutningen i x-led     
*/
void drawNumber(int value, int offsetX, int offsetY,char remove)
{
  value += 48;
  switch (value)
  {
  case '0':
    drawLetter('0',offsetX,offsetY,remove);
    break;
  case '1':
    drawLetter('1',offsetX,offsetY,remove);
    break;
  case '2':
   drawLetter('2',offsetX,offsetY,remove);
    break;
  case '3':
   drawLetter('3',offsetX,offsetY,remove);
    break;
  case '4':
   drawLetter('4',offsetX,offsetY,remove);
    break;
  case '5':
   drawLetter('5',offsetX,offsetY,remove);
    break;
  case '6':
    drawLetter('6',offsetX,offsetY,remove);
    break;
  case '7':
    drawLetter('7',offsetX,offsetY,remove);  
    break;
  case '8':
    drawLetter('8',offsetX,offsetY,remove); 
    break;
  default:
   drawLetter('9',offsetX,offsetY,remove);
    break;
  }
}

/*
skriver ut poängen under spelets gång
int score: är poängen som ska skrivas ut
*/
void drawScore(int score)
{
  int offset = RESOLUTION_X/6;
  //REMOVE NUMBER
  drawNumber(score%10,offset+80,RESOLUTION_Y/24-2,1);
  drawNumber(score/10%10,offset+60,RESOLUTION_Y/24-2,1);
  drawNumber(score/100%10,offset+40,RESOLUTION_Y/24-2,1);
  drawNumber(score/1000%10,offset+20,RESOLUTION_Y/24-2,1);
  //DRAW BACK NEW NUMBER
  drawNumber(score%10,offset+80,RESOLUTION_Y/24-2,0);
  drawNumber(score/10%10,offset+60,RESOLUTION_Y/24-2,0);
  drawNumber(score/100%10,offset+40,RESOLUTION_Y/24-2,0);
  drawNumber(score/1000%10,offset+20,RESOLUTION_Y/24-2,0);
}