#include "./../include/processA_utilities.h"

#include <stdio.h>
#include <bmpfile.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
/*
Simply moving the circle in ncurces window which moves the 20x bigger circle in the 
shared memory.
Keep updating the circle in bitmap when movements occur in ncurces window of the processA
*/
int width = 1600;
int height = 600;
int depth = 4;
int init_width;
int init_height;
int new_width;
int new_height;
int shift = 20;
int radius;

// Data structure for storing the bitmap file
bmpfile_t *bmp;
// Data type for defining pixel colors (BGRA)
rgb_pixel_t pixel = {0, 0, 255, 0};
rgb_pixel_t pixel1 = {255, 255, 255, 255};

// Function to Remove the previous circle
void rmv_prevCircle(){
    for(int x = -radius; x <= radius; x++) {
                        for(int y = -radius; y <= radius; y++) {
                            // If distance is smaller, point is within the circle
                            if(sqrt(((x-10)*x) + (y*y) < radius)) {
                                bmp_set_pixel(bmp, new_width + x, new_height + y, pixel1);
                            }
                        }
                    }
}

// Function to Draw New Circle
void draw_newCircle(){
    for(int x = -radius; x <= radius; x++) {
                        for(int y = -radius; y <= radius; y++) {
                            // If distance is smaller, point is within the circle
                            if(sqrt(((x-10)*x) + (y*y) < radius)) {
                                bmp_set_pixel(bmp, new_width + x, new_height + y, pixel);
                            }
                        }
                    }
}

int main(int argc, char *argv[])
{
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();

    
    // Data type for defining pixel colors (BGRA)
    rgb_pixel_t pixel = {0, 0, 255, 0};
       
    bmp = bmp_create(width, height, depth);
    init_width = width/2;
    init_height = height / 2;
    new_width = init_width;
    new_height = init_height;

    // Code for drawing a centered circle of given radius...
    radius = atoi("30");
    for(int x = -radius; x <= radius; x++) {
                for(int y = -radius; y <= radius; y++) {
                    // If distance is smaller, point is within the circle
                    if(sqrt(((x-10)*x) + (y*y) < radius)) {
                        bmp_set_pixel(bmp, init_width + x, init_height + y, pixel);
                    }
                }
            }
    
    // Infinite loop
    while (TRUE)
    {
        // Get input in non-blocking mode
        int cmd = getch();

        // If user resizes screen, re-draw UI...
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }

        // Else, if user presses print button...
        else if(cmd == KEY_MOUSE) {
            if(getmouse(&event) == OK) {
                if(check_button_pressed(print_btn, &event)) {
                    mvprintw(LINES - 1, 1, "Print button pressed");
                    // Save image as .bmp file
                    bmp_save(bmp, "out/new.bmp");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS - BTN_SIZE_X - 2; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }
            }
        }
        
        // If input is an arrow key, move circle accordingly...
        else if(cmd == KEY_LEFT || cmd == KEY_RIGHT || cmd == KEY_UP || cmd == KEY_DOWN) {
            move_circle(cmd);
            draw_circle();
            switch (cmd)
            {
                case KEY_LEFT:
                    rmv_prevCircle();
                    new_width = new_width - shift;
                    draw_newCircle();
                    break;
                case KEY_RIGHT:
                    rmv_prevCircle();
                    new_width = new_width + shift;
                    draw_newCircle();
                    break;
                case KEY_UP:
                    rmv_prevCircle();
                    new_height = new_height - shift;
                    draw_newCircle();
                    break;
                case KEY_DOWN:
                    rmv_prevCircle();
                    new_height = new_height + shift;
                    draw_newCircle();
                    break;
                default:
                    break;
            }
        }
        
    }

    // Free resources before termination
    bmp_destroy(bmp);
    endwin();
    return 0;
}
