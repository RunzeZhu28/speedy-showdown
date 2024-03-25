#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
volatile int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];
int police_x_box[3],police_y_box[3];
int police_dx, police_dy;
int thief_x_box[3],thief_y_box[3];
int thief_dx, thief_dy;

void plot_pixel(int, int, short int);
void clear_screen();
void draw_line(int , int , int , int , short int);
void swap(int*,int*);
void wait_for_vsync();
void draw_box(int , int);
char ps2(void);
#define RLEDs ((volatile long *) 0xFF200000)

int main(void)
{
    char text[] = "Once upon a time a little girl tried to make a living by selling matches in the street." ;
    int index = 0;
    police_x_box[0] = 100;
    police_y_box[0] = 30;
    thief_x_box[0] = 279;
    thief_y_box[0] = 30;
    int clockwise = 1;
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    int begin = 0;
    int type = 0;
    while (1)
    {   
        
        for(int i=0; i<11;i++){
            draw_line(100,30+i,279,30+i,0);//top
            draw_line(40,140+i,279,140+i,0);//bottom
            draw_line(40+i,60,40+i,140,0);//left
            draw_line(279+i,30,279+i,150,0);//right
			draw_line(40,60+i,90,60+i,0);
			draw_line(90+i,30,90+i,70,0);
        }
        char target = tolower(text[index]);
        char input = ps2();
        if (target == input){
            index = index +1;
            
            *RLEDs = index;
        }
        /* Erase any boxes and lines that were drawn in the last iteration */
        //draw_box(1,1); 
        
        if(clockwise){
            if(police_x_box[0] ==100 && police_y_box[0] <= 30){
                 police_dx = 2;
                 police_dy = 0;
				 police_x_box[0] = 100;
				police_y_box[0] = 30;
             }
             else if(police_x_box[0] >=279 && police_y_box[0] == 30){
                 police_dx = 0;
                 police_dy = 2;
				 police_x_box[0] =279;
				 police_y_box[0]= 30;
             }
             else if(police_x_box[0] ==279 && police_y_box[0] >= 140){
                 police_dx = -2;
                 police_dy = 0;
				 police_x_box[0] = 279;
				 police_y_box[0] = 140;
             }
             else if(police_x_box[0] <= 40 && police_y_box[0] == 140){
                 police_dx = 0;
                 police_dy = -2;
				 police_x_box[0] = 40;
				 police_y_box[0] = 140;
				 
             }
             else if(police_x_box[0] == 40 && police_y_box[0] <= 60){
                 police_dx = 2;
                 police_dy = 0;
				 police_x_box[0] = 40;
				 police_y_box[0] = 60;
             }
             else if(police_x_box[0] >= 90 && police_y_box[0] == 60 && police_x_box[0] <150){
                 police_dx = 0;
                 police_dy = -2;
				 police_x_box[0] = 90;
				 police_y_box[0] = 60;
             }
             else if(police_x_box[0] == 90 && police_y_box[0] <= 30){
                 police_dx = 2;
                 police_dy = 0;
				 police_x_box[0] = 90;
				 police_y_box[0] = 30;
             }
        }
        if(type = 0){
            type = 1;
        }
        else{
            begin = begin +1;
            if(begin == 10){
                type = 0;
            }
        }
        if(clockwise &&type){
            if(thief_x_box[0] ==100 && thief_y_box[0] <= 30){
                 thief_dx = 2;
                 thief_dy = 0;
				 thief_x_box[0] = 100;
				thief_y_box[0] = 30;
             }
             else if(thief_x_box[0] >=279 && thief_y_box[0] == 30){
                 thief_dx = 0;
                 thief_dy = 2;
				 thief_x_box[0] =279;
				 thief_y_box[0]= 30;
             }
             else if(thief_x_box[0] ==279 && thief_y_box[0] >= 140){
                 thief_dx = -2;
                 thief_dy = 0;
				 thief_x_box[0] = 279;
				 thief_y_box[0] = 140;
             }
             else if(thief_x_box[0] <= 40 && thief_y_box[0] == 140){
                 thief_dx = 0;
                 thief_dy = -2;
				 thief_x_box[0] = 40;
				 thief_y_box[0] = 140;
				 
             }
             else if(thief_x_box[0] == 40 && thief_y_box[0] <= 60){
                 thief_dx = 2;
                 thief_dy = 0;
				 thief_x_box[0] = 40;
				 thief_y_box[0] = 60;
             }
             else if(thief_x_box[0] >= 90 && thief_y_box[0] == 60 && thief_x_box[0] <150){
                 thief_dx = 0;
                 thief_dy = -2;
				 thief_x_box[0] = 90;
				 thief_y_box[0] = 60;
             }
             else if(thief_x_box[0] == 90 && thief_y_box[0] <= 30){
                 thief_dx = 2;
                 thief_dy = 0;
				 thief_x_box[0] = 90;
				 thief_y_box[0] = 30;
             }
        }
        draw_box(1,0);
        draw_box(0,0);
        /*police_x_box[2] = police_x_box[1];
        police_x_box[1] = police_x_box[0];
        police_y_box[2] = police_y_box[1];
        police_y_box[1] = police_y_box[0];*/
        police_x_box[0] = police_x_box[0] + police_dx;
        police_y_box[0] = police_y_box[0] + police_dy;

        
        thief_x_box[0] = thief_x_box[0] + thief_dx;
        thief_y_box[0] = thief_y_box[0] + thief_dy;
        
        

        // code for drawing the boxes and lines (not shown)
        // code for updating the locations of boxes (not shown)

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

// code for subroutines (not shown)

void wait_for_vsync(){
    int status;
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *pixel_ctrl_ptr = 1;
    status = *(pixel_ctrl_ptr + 3);
    while((status & 1) !=0){
        status = *(pixel_ctrl_ptr + 3);
    }
}

void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;

        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

        *one_pixel_address = line_color;
}

void swap(int* x, int* y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

void clear_screen(){
    for(int x = 0; x<=319; x++){
        for(int y=0; y<=239;y++){
            volatile short int *one_pixel_address;
            one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
                plot_pixel(x,y, 0xffff);
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color){
    if (x0 == x1){
        for(int y = y0; y<=y1;y++){
            plot_pixel(x0,y,line_color);
        }
        return;
    }
    int steep = abs(y1-y0)/abs(x1-x0);
    if(steep){
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if(x0 >x1){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    int deltax = x1 - x0;
    int deltay = abs(y1-y0);
    int error = -deltax/2;
    int y = y0;
    int y_step;
    if (y0<y1){
        y_step = 1;
    }
    else{
        y_step = -1;
    }
    for(int x=x0; x<=x1;x++){
        if(steep){
            plot_pixel(y,x,line_color);
        }
        else{
            plot_pixel(x,y,line_color);
        }
        error = error + deltay;
        if(error>0){
            y = y + y_step;
            error = error - deltax;
        }
    }
}

void draw_box(int a, int clear){
    if(a == 1){
        /*if(clear){
            for(int i = 0; i<11;i++){
                for(int j = 0;j<11;j++){
                    plot_pixel(police_x_box[2]+i,police_y_box[2]+j, 0);
                }
            }
        }
        else{
            for(int i = 0; i<11;i++){
                for(int j = 0;j<11;j++){
                    plot_pixel(police_x_box[0]+i,police_y_box[0]+j, 0x07e0);
                }
            }
        }
    }*/
    for(int i = 0; i<11;i++){
            for(int j = 0;j<11;j++){
                plot_pixel(police_x_box[0]+i,police_y_box[0]+j, 0x07e0);
            }
        }
    }
    else{
        for(int i = 0; i<11;i++){
            for(int j = 0;j<11;j++){
                plot_pixel(thief_x_box[0]+i,thief_y_box[0]+j, 0xf81f);
            }
        }
    }
}

char ps2(void){
    unsigned char byte1 = 0;
	unsigned char byte2 = 0;
	unsigned char byte3 = 0;
    volatile int* ps2_ptr = (int*)0xff200100;
    int data = *(ps2_ptr);
    bool valid = data & 0x8000;
    int make_code[26] = {0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B, 0x42, 0x4B, 0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C, 0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A };
    if(valid){
        byte1 = byte2;
		byte2 = byte3;
		byte3 = data & 0xFF;
    }
    char output = 0;
    for(int i = 0; i <26; i++){
        if(byte3 == make_code[i]){
            output = 97 + i;
            return output;
        }
    }
    if (byte3 == 0x29){
        output = 32;
    }
    else if (byte3 == 0x41){
        output = ',';
    }
    else if (byte3 == 0x49){
        output = '.';
    }
    return output;
}