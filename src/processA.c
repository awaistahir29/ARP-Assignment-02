#include "./../include/processA_utilities.h"

#include <stdio.h>
#include <bmpfile.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();

    // Data structure for storing the bitmap file
    bmpfile_t *bmp;
    // Data type for defining pixel colors (BGRA)
    rgb_pixel_t pixel = {0, 0, 255, 0};

    /*
    if (argc < 3) {
        printf("Please specify filename and radius as arguments!");
        return EXIT_FAILURE;
    }
    
    /* Instantiate bitmap, passing three parameters:
    *   - width of the image (in pixels)
    *   - Height of the image (in pixels)
    *   - Depth of the image (1 for greyscale images, 4 for colored images)
    */
    int width = 100;
    int height = width;
    int depth = 4;
    bmp = bmp_create(width, height, depth);

    // Code for drawing a centered circle of given radius...
    int radius = atoi("20");
    for(int x = -radius; x <= radius; x++) {
                for(int y = -radius; y <= radius; y++) {
                    // If distance is smaller, point is within the circle
                    if(sqrt(x*x + y*y) < radius) {
                        /*
                        * Color the pixel at the specified (x,y) position
                        * with the given pixel values
                        */
                        bmp_set_pixel(bmp, width/2 + x, height/2 + y, pixel);
                    }
                }
            }

    // Save image as .bmp file
    bmp_save(bmp, "out/test.bmp");
    // Free resources before termination
    bmp_destroy(bmp);

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
        }
        
    }
    
    endwin();
    return 0;
}
