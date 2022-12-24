#include "./../include/processB_utilities.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

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

char shm_fn[] = "my_shm";
char sem_fn[] = "my_sem";

int main(int argc, char const *argv[])
{

    caddr_t shmptr;
    int shmdes, index;
    sem_t *semdes;
    int SHM_SIZE;
    /* Open the shared memory object */
    SHM_SIZE = sysconf(_SC_PAGE_SIZE);
    if ( (shmdes = shm_open(shm_fn, O_RDWR, 0)) == -1 ) {
        perror("shm_open failure");
        exit(1);
    }
    if((shmptr = mmap(0, SHM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED,
    shmdes,0)) == (caddr_t) -1){
    perror("mmap failure");
    exit(1);
    }

    /* Open the Semaphore */
    semdes = sem_open(sem_fn, 0, 0644, 0);
    if(semdes == (void*) -1){
    perror("sem_open failure");
    exit(1);
    }

    /* Lock the semaphore */
    if(!sem_wait(semdes)){

    /* Access to the shared memory area */
    for(index = 0; index < 100; index++)
        printf("The shared memory shmptr[%d] = %d\n", index,shmptr[index]);

    /* Release the semaphore lock */
    sem_post(semdes);
    }
    munmap(shmptr, SHM_SIZE);

    /* Close the shared memory object */
    close(shmdes);

    /* Close the Semaphore */
    sem_close(semdes);
    sem_unlink(sem_fn);

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
