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
        color = PINK;
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
*
* place or remove a letter between 1-3 and A-C at offset X and Y
*
*/
void drawLetter(char letter, int offsetX, int offsetY, char remove)
{
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
    (remove == 1 ? width = 19 : (width = (lastIndx-firstIndx)/21));

   
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
void drawNumber(int value, int offset){

  int pxToPaint[14][10];
  for (int y = 0; y < 14; y++){
    for (int x = 0; x < 10; x++){
      pxToPaint[y][x] = 0;
    } 
  }  

  switch (value)
  {
  case 0:
      //övre raden
    for (int y = 0; y < 2; y++){
      for (int x = 0; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //vänstra raden
    for (int y = 0; y < 14; y++){
      for (int x = 0; x < 2; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //högra raden
    for (int y = 0; y < 14; y++){
      for (int x = 8; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //nedre raden
    for (int y = 12; y < 14; y++){
      for (int x = 0; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    break;
  case 1:
    //mitten raden
    for (int y = 0; y < 14; y++){
      for (int x = 4; x < 6; x++){
        pxToPaint[y][x] = 1;
      }
    }
    break;
  case 2:
    //övre raden
    for (int y = 0; y < 2; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //snedden
    for (int y = 4; y < 12; y++){
      for (int x = 11 - y; x <= 11 - y + 3 ; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //nedre raden
    for (int y = 12; y < 14; y++){
      for (int x = 0; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //sista bitarna
    pxToPaint[1][0] = 1;
    pxToPaint[2][0] = 1;
    pxToPaint[2][1] = 1;
    
    pxToPaint[3][8] = 1;
    pxToPaint[3][9] = 1;
    pxToPaint[2][8] = 1;
    pxToPaint[2][9] = 1;
    break;
  case 3:
    //övre raden
    for (int y = 0; y < 2; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //mitten raden
    for (int y = 6; y < 8; y++){
      for (int x = 4; x < 8; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //högra raden
    for (int y = 1; y < 13; y++){
      for (int x = 8; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //nedre raden
    for (int y = 12; y < 14; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //sista bitarna
    pxToPaint[1][0] = 1;
    pxToPaint[2][0] = 1;
    pxToPaint[2][1] = 1;
    
    pxToPaint[2][7] = 1;

    pxToPaint[11][7] = 1;
    
    pxToPaint[11][0] = 1;
    pxToPaint[11][1] = 1;
    pxToPaint[12][0] = 1;
    break;
  case 4:
    //vänstra raden
    for (int y = 0; y < 8; y++){
      for (int x = 0; x < 2; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //mitten raden
    for (int y = 6; y < 8; y++){
      for (int x = 2; x < 8; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //högra raden
    for (int y = 0; y < 14; y++){
      for (int x = 8; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    break;
  case 5:
    //översta raden
    for (int y = 0; y < 2; y++){
      for (int x = 0; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //vänstra raden
    for (int y = 2; y < 7; y++){
      for (int x = 0; x < 2; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //mitten raden
    for (int y = 6; y < 8; y++){
      for (int x = 2; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //högra raden
    for (int y = 8; y < 13; y++){
      for (int x = 8; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //nedre raden
    for (int y = 12; y < 14; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //sista bitarna
    pxToPaint[7][1] = 1;
    pxToPaint[7][9] = 1;
    
    pxToPaint[11][7] = 1;
    pxToPaint[13][8] = 1;
    
    pxToPaint[11][1] = 1;
    pxToPaint[11][0] = 1;
    pxToPaint[12][0] = 1;
    break;
  case 6:
    //övre raden
    for (int y = 0; y < 2; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }    
    //vänstra raden
    for (int y = 1; y < 13; y++){
      for (int x = 0; x < 2; x++){
        pxToPaint[y][x] = 1;
      }
    }    
    //mitten raden
    for (int y = 6; y < 8; y++){
      for (int x = 2; x < 8; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //högra raden
    for (int y = 7; y < 13; y++){
      for (int x = 8; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //nedre raden
    for (int y = 12; y < 14; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    break;
  case 7:
    //övre raden
    for (int y = 0; y < 2; y++){
      for (int x = 1; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //mitten raden
    for (int y = 6; y < 8; y++){
      for (int x = 3; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //snedden
    pxToPaint[3][8] = 1;
    pxToPaint[3][7] = 1;
    pxToPaint[2][8] = 1;
    pxToPaint[2][7] = 1;
    
    pxToPaint[4][6] = 1;
    pxToPaint[4][7] = 1;
    pxToPaint[5][6] = 1;
    pxToPaint[5][7] = 1;
    
    pxToPaint[8][5] = 1;
    pxToPaint[8][6] = 1;
    pxToPaint[9][5] = 1;
    pxToPaint[9][6] = 1;
    
    pxToPaint[10][4] = 1;
    pxToPaint[10][5] = 1;
    pxToPaint[11][4] = 1;
    pxToPaint[11][5] = 1;
    
    pxToPaint[12][3] = 1;
    pxToPaint[12][4] = 1;
    pxToPaint[13][3] = 1;
    pxToPaint[13][4] = 1;
    break;
  case 8:
    //övre raden
    for (int y = 0; y < 2; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //mitten raden
    for (int y = 6; y < 8; y++){
      for (int x = 2; x < 8; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //nedre raden
    for (int y = 12; y < 14; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //högra raden
    for (int y = 1; y < 13; y++){
      for (int x = 8; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //vänstra raden
    for (int y = 1; y < 13; y++){
      for (int x = 0; x < 2; x++){
        pxToPaint[y][x] = 1;
      }
    } 
    break;
  default:// 9
    //övre raden
    for (int y = 0; y < 2; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //högra raden
    for (int y = 1; y < 13; y++){
      for (int x = 8; x < 10; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //vänstra raden
    for (int y = 1; y < 7; y++){
      for (int x = 0; x < 2; x++){
        pxToPaint[y][x] = 1;
      }
    } 
    //mitten raden
    for (int y = 6; y < 8; y++){
      for (int x = 1; x < 8; x++){
        pxToPaint[y][x] = 1;
      }
    }
    //nedre raden
    for (int y = 12; y < 14; y++){
      for (int x = 1; x < 9; x++){
        pxToPaint[y][x] = 1;
      }
    }
    break;
  }

  for (int y = 0; y < 14; y++){
    for (int x = 0; x < 10; x++){
      if(pxToPaint[y][x]){
        drawPixel(x+70+offset,y+8,0xff);
      }else{
        drawPixel(x+70+offset,y+8,0x00);
      }
    } 
  }  
}

/*
skriver ut poängen under spelets gång
int score: är poängen som ska skrivas ut
*/
void drawScore(int score){
    int i = 1000;
    int offset = 0;
    while (i>0)
    {
        drawNumber((score/i) % 10,offset);  
        i = i/10;
        offset += 12;
    }
}