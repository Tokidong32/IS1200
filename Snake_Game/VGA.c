#define VGA_BUFFR ((volatile int *) 0x04000100)
#define VGA_BACKBUFFR ((volatile int*) 0x04000104)
#define VGA_RES ((volatile int*) 0x04000108)
#define VGA_STATCTRL ((volatile int*) 0x0400010C) 
#define VGA_BASE_MEM ((volatile unsigned char*) 0x08000000)

extern void print_dec(unsigned int);

void handle_interrupt(unsigned cause){}

int main()
{
    /*
    unsigned char* pixel_buffer = 0x08000000;
    
    int maskx = 0x0000001FF; //9 bitar 
    int masky = 0x00001FE00;// 8 bitar

    int y_offset = 0x0000001FF;
    int x_offset = 0x000000000;

    int *addr = 0;
    
    int x_offest = 0;
    int y_offset = 9;

    int x = 100;
    int y = 100;
    */
    print_dec(*VGA_STATCTRL & 2);

    volatile unsigned char *VGA = VGA_BASE_MEM;

    for (int i = 0; i < 480*320; i++)
    {
        VGA[i] = i/320;
    }

    unsigned int y_ofs= 0;
    
    while (1)
    {
        *(VGA_BACKBUFFR) = (unsigned int) (VGA+y_ofs*320);
        *(VGA_BUFFR) = 0;
        y_ofs= (y_ofs+ 1) % 240;
        for (int i = 0; i < 1000000; i++)
        {
            asm volatile ("nop");
        }
    }
}

