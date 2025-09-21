#include "../headers/vga_utils.h"
#include "../headers/main_menu_image.h"


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
        color = PINK;
        break;
    //Snake_head colour
    case 2:
        color = PINK;
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
    int offsetX = RESOLUTION_X/4;
    
    /*
    *
    *Remove old lines by overwriting old line with black colour.
    *
    * Function replaced switch case to determine where the line should be drawn in the y axis
    * 
    */
    float prevLineFloat = (RESOLUTION_Y/8)+ MAIN_MENU_RES_Y*(((10*lastHighlight)+19.0)/40)+10;
    int prevLineInt = (int)prevLineFloat;

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
