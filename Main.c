#include "ourfunc.h"

/*
  ---------------Rat Maze Generator and solver-----------------------
  
  Architecture
  Two screens : Menu screen (canonical mode and echo mode enabled)
				Game screen (canonical mode and echo mode disabled)

  step 1 - get terminal attributes, window size (for computing UI)
  step 2 - loop starts
  step 3 - print menu screen and get maze dimensions from user
  step 4 - change terminal properties
  step 5 - generate a new puzzle
  step 6 - print game screen with new puzzle
  step 7 - solve it step by step following user commands
  step 8 - get input after finishing the maze
  step 9 - revert the terminal properties
  step 10- if q quit the program
  step 11- else repeat the loop
*/

int main (void) {	
    struct winsize window; 

	MAT matrix;
    struct termios oldTerm;
    struct termios newTerm;

	srand(time(NULL));
		
	while (1) {
		// getting terminal window dimensions from system
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
		// printing Menu screen and getting maze dimensions from user
		if (getMatSize(&matrix, window) == -1) break;
		// changing terminal properties (disableing canonical mode and echo mode)
		setTermProp(&oldTerm, &newTerm);
		// generate and solve a new puzzle
		if (puzzleEngine(matrix, window) == -1) {
	   		revertTermProp (&oldTerm);
			break;
		}
		// revert terminal properties
		revertTermProp (&oldTerm);
	}
	// come back to the original terminal command line
    printf("\033[?1049l"); 
    return 0;
}

