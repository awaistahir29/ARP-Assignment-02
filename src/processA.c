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

char shm_fn[] = "my_shm";
char sem_fn[] = "my_sem";

int main(int argc, char *argv[])
{

    caddr_t shmptr;
    unsigned int mode;
    int shmdes, index;
    sem_t *semdes;
    int SHM_SIZE;
    mode = S_IRWXU|S_IRWXG;

    /* Open the shared memory object */
    if ( (shmdes = shm_open(shm_fn,O_CREAT|O_RDWR|O_TRUNC, mode)) == -1 ) {
        perror("shm_open failure");
        exit(1);
    }
    
    /* Preallocate a shared memory area */
    SHM_SIZE = sysconf(_SC_PAGE_SIZE);
    if(ftruncate(shmdes, SHM_SIZE) == -1){
        perror("ftruncate failure");
        exit(1);
    }

    if((shmptr = mmap(0, SHM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, shmdes,0)) == (caddr_t) -1){
        perror("mmap failure");
        exit(1);
    }
    
    /* Create a semaphore in locked state */
    semdes = sem_open(sem_fn, O_CREAT, 0644, 0);
    if(semdes == (void*)-1){
        perror("sem_open failure");
        exit(1);
    }

    /* Access to the shared memory area */
    for(index = 0; index < 100; index++){
        printf("write %d into the shared memory shmptr[%d]\n", index*2, index);
        shmptr[index]=index*2;
    }
    /* Release the semaphore lock */
    sem_post(semdes);
    munmap(shmptr, SHM_SIZE);

    /* Close the shared memory object */
    close(shmdes);

    /* Close the Semaphore */
    sem_close(semdes);

    /* Delete the shared memory object */
    shm_unlink(shm_fn);

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
    int width = 1600;
    int height = 600;
    int depth = 4;
    bmp = bmp_create(width, height, depth);

    // Code for drawing a centered circle of given radius...
    int radius = atoi("20");
    for(int x = -radius; x <= radius; x++) {
                for(int y = -radius; y <= radius; y++) {
                    // If distance is smaller, point is within the circle
                    if(sqrt(((x-10)*x) + (y*y) < radius)) {
                        /*
                        * Color the pixel at the specified (x,y) position
                        * with the given pixel values
                        */
                        bmp_set_pixel(bmp, width/4 + x, height/10 + y, pixel);
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
