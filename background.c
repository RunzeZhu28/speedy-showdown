#include <stdlib.h>
volatile int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];
int x_box[8][3],y_box[8][3];
short int colour_box[8];
short int colour[10] = {0xffff, 0xf800, 0x07e0, 0x001f, 0x0ff0, 0x8000, 0xf81f, 0x07ff, 0x1234, 0xabcd};
int dx[8], dy[8];

void plot_pixel(int, int, short int);
void clear_screen();
void draw_line(int , int , int , int , short int);
void swap(int*,int*);
void wait_for_vsync();
void draw_box(int , int);

int main(void)
{
    for (int i = 0; i<8; i++){
        dx[i] = (rand()%2) *2 - 1;
        dy[i] = (rand()%2) *2 - 1;
        x_box[i][0] = rand()%319;
        y_box[i][0] = rand()%239;
        colour_box[i] = colour[rand()%10];
    }
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
        
        /* Erase any boxes and lines that were drawn in the last iteration */
        /*for(int i=0;i<8;i++){
            draw_box(i,1);
            draw_line(x_box[i][2], y_box[i][2], x_box[(i+1)%8][2], y_box[(i+1)%8][2],0);
            if(x_box[i][0] == 0 ){
                dx[i] = 1; 
            }
            else if(x_box[i][0] == 318){
                dx[i] = -1;
            }
            if(y_box[i][0] == 0 ){
                dy[i] = 1; 
            }
            else if(y_box[i][0] == 238){
                dy[i] = -1;
            }  
        }
        for(int i =0;i<8;i++){
            draw_box(i,0);
            draw_line(x_box[i][0], y_box[i][0], x_box[(i+1)%8][0], y_box[(i+1)%8][0],colour_box[i]);
        }

        for(int i=0; i<8;i++){
            x_box[i][2] = x_box[i][1];
            x_box[i][1] = x_box[i][0];
            y_box[i][2] = y_box[i][1];
            y_box[i][1] = y_box[i][0];

            x_box[i][0] = x_box[i][0]+dx[i];
            y_box[i][0] = y_box[i][0]+dy[i];
        }*/
        
        

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

void draw_box(int i, int clear){
    if(clear){
        plot_pixel(x_box[i][2],y_box[i][2], 0);
        plot_pixel(x_box[i][2]+1,y_box[i][2], 0);
        plot_pixel(x_box[i][2],y_box[i][2]+1, 0);
        plot_pixel(x_box[i][2]+1,y_box[i][2]+1, 0);
    }
    else{
        plot_pixel(x_box[i][0],y_box[i][0], colour_box[i]);
        plot_pixel(x_box[i][0]+1,y_box[i][0], colour_box[i]);
        plot_pixel(x_box[i][0],y_box[i][0]+1, colour_box[i]);
        plot_pixel(x_box[i][0]+1,y_box[i][0]+1, colour_box[i]);
    }
}