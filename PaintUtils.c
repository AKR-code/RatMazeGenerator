#include "ourfunc.h"

int inputHandler(MAT *matrix, int, int);

int getMatSize (MAT *matrix, struct winsize window) {
	// title is 67 character wide
	int pad = (window.ws_col - 67) / 2;
   
	printf("\033[?1049h");	// switch to alternate page
	printf("\n");
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m██████╗  █████╗ ████████╗\033[91m    ███╗   ███╗ █████╗ ███████╗███████╗ █╗\n");
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m██╔══██╗██╔══██╗╚══██╔══╝\033[91m    ████╗ ████║██╔══██╗╚══███╔╝██╔════╝ █║\n");
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m██████╔╝███████║   ██║   \033[91m    ██╔████╔██║███████║  ███╔╝ █████╗   █║\n");
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m██╔══██╗██╔══██║   ██║   \033[91m    ██║╚██╔╝██║██╔══██║ ███╔╝  ██╔══╝   █║\n");
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m██║  ██║██║  ██║   ██║   \033[91m    ██║ ╚═╝ ██║██║  ██║███████╗███████╗ ╚╝\n");	
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m██║  ╚═╝╚═╝  ██║   ██║   \033[91m    ██║     ╚═╝╚═╝  ██║╚══════╝╚══════╝ █╗\n");
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m╚═╝          ╚═╝   ██║   \033[91m    ╚═╝             ╚═╝                 ╚╝\n");
	for (int i = 0; i < pad; i++) printf(" ");
	printf("\033[94m                   ╚═╝\n\n\033[0m");
	printf("Enter the size to create a new puzzle: \n");
	printf("height - ");
    if (inputHandler (matrix, window.ws_row - 5, 0) == -1) return -1;
	printf("width  - ");
	if (inputHandler (matrix, window.ws_col / 2 - 13, 1) == -1) return -1;
	return 0;
}

int inputHandler (MAT *matrix, int lim, int type) {
	do {
		char buf[32];
		char *end;
		long val; 
	    if (fgets(buf, sizeof(buf), stdin) == NULL) { // empty input
			printf("failed to take input, try again: ");
			continue;
		}
		if (buf[0] == 'q' && buf[1] == '\n') { //quiting from game
			return -1;
		}
		val = strtol(buf, &end, 10);
		if (end == buf || *end != '\n' || val < 3 || val > lim || val > 250) { 
			printf("INVALID INPUT; enter a number greater than 2 & less than ");
			if (lim > 250) printf("250 - ");
			else printf("%d - ", lim);
		} else {
			if (type == 0) {
				matrix->height = (int) val;
			} else {
				matrix->width = (int) val;
			}
			break;
		}
	} while (1);
	return 0;
} 

void painter (MAT matrix, CELL *maze, int start, int end, int head) {
	for (int i = start; i < end; i++) {
		printf("\033[%d;%dH", matrix.padtop + i + 1, matrix.padleft + 1); // move cursor to the right position in the screen
		for (int j = 0; j < matrix.width; j++) {
			if (i * matrix.width + j == head) {
				if (head != 0 && head % (matrix.width - 1) != 0 && maze[head + 1].path == 1)
					printf("\033[40m\033[37m:\033[42m\033[30m~\033[0m");
				else 
					printf("\033[42m\033[30m~\033[40m\033[37m:\033[0m");
			} else if (maze[i * matrix.width + j].block == 1) {
				printf("\033[41m  \033[0m");
			} else if (maze[i * matrix.width + j].path == 1) {
				printf("\033[42m  \033[0m");
			} else if (maze[i * matrix.width + j].visited == 1) {
				printf("\033[47m\033[30m▒▒\033[0m");
			} else {
				printf("\033[47m  \033[0m");
			}
		}
	}
	fflush(stdout);
}

void setPage(struct winsize window, MAT *matrix, CELL *maze) {
   	printf("\033[?1049h");
	matrix->padtop = (window.ws_row - matrix->height) / 2;
	matrix->padleft = (window.ws_col - 2 * matrix->width) / 2;
	printf("\033[%d;%dHrat house -> ", matrix->padtop + 1, matrix->padleft - 12);
	painter(*matrix, maze, 0, matrix->height, 0);
	printf(" <- cheese\n");
	printf("\nHelp the rat to reach the cheese!!\n");
	fflush(stdout);
	return;
}

void printQuitWizard (MAT matrix) {
	printf("\033[%d;%dHDo you really want to quit the game? press 'q' again to quit.", matrix.padtop + matrix.height + 1, 1);
	fflush(stdout);
	
}

void clearWizard (MAT matrix) {
	printf("\033[%d;%dH", matrix.padtop + matrix.height + 1, 1);
	for (int i = 0; i < 70; i++) {
		printf(" ");
	}
}

void setTermProp(struct termios *oldTerm, struct termios *newTerm){
    tcgetattr(STDIN_FILENO, oldTerm);	// save old attributes
	*newTerm = *oldTerm;	
	newTerm->c_lflag &= ~ICANON;  // disable canonical and echo modes
	newTerm->c_lflag &= ~ECHO; 
	tcsetattr (STDIN_FILENO, TCSANOW, newTerm);
}

void revertTermProp(struct termios *oldTerm){
	tcsetattr (STDIN_FILENO, TCSANOW, oldTerm);
}

