#include "./../include/processB_utilities.h"


/*
Simply accessing the shared memory (BitMap) and performing some feature extraction to display trajectory.
Step 0:
    Reduce teh pixels again with the factor of 20x so that we get normal circled value that we gave
    in the process A.

Step 1:
    We will get the pixels from the shared memory which is the bitmap by scanning the pixels
    using the function (bmp_get_pixel()) more details in header file of the bitmap
    /usr/local/include -> file containing the functions
    Which return the pixels that we had set in the process A 

Step 2:
    Scan whole pixels and do some feature extraction like Comparing each line passing from the circle
    to compute the length of the line and if length is equal to the diameter of the circle then the
    centered point of the circle is the mid point of the circle

Step 3:
Taking that mid point we draw the trajectory:
    BY storing all the mid points to an array and display these centre points(Pixels) along with the realtime
    coming centered pixels in to the screen
*/
int main(int argc, char const *argv[])
{
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();

    // Infinite loop
    while (TRUE) {

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

        else {
            mvaddch(LINES/2, COLS/2, '0');
            refresh();
        }
    }

    endwin();
    return 0;
}
