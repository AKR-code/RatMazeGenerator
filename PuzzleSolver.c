#include "ourfunc.h"
int smell (int, int, CELL *, MAT, PUZZMK[]);
int paintHandler (int *, CELL *, MAT, int, int , int);

#define STEP 1
#define QUICK 5
#define SKIP -1
int solveProblem(MAT matrix, CELL *maze, PUZZMK *stack) {
	int head = 0;
	int ptr = 0;
	int printMode = STEP;
    stack[ptr].pathind = head;
	maze[head] = (CELL) {0, 1, 1};
    int minPaintRow, maxPaintRow;
   	minPaintRow = 0;// print mouse
    maxPaintRow = 1;
	while (head != matrix.height * matrix.width - 1) {
	    int dir;
		dir = smell(head, ptr, maze, matrix, stack);
		if (dir == -1) {
		    while (ptr > -1 && stack[ptr].availNeigh == 0) {
				maze[head] = (CELL) {0, 1, 0};
				ptr--;
				if (ptr > -1) {
					head = stack[ptr].pathind;
					if (minPaintRow > head / matrix.width) minPaintRow = head / matrix.width;
					if (maxPaintRow < head / matrix.width) maxPaintRow = head / matrix.width;
				}
			}
			if (ptr == -1) {
				printf("solution not possible");
				fflush(stdout);
				return 0; // here we should not exit the program, it is just a part of program we soccuessfully found that there is no possible path
			}
			continue;
		}
		switch (dir) {
		case E:
			head++; break;
		case W:
			head--; break;
		case N:
			head -= matrix.width; break;
		case S:
			head += matrix.width; break;
		}
		ptr++;
		stack[ptr].pathind = head;
		maze[head] = (CELL) {0, 1, 1};

		if (minPaintRow > head / matrix.width) minPaintRow = head / matrix.width;
		if (maxPaintRow < head / matrix.width) maxPaintRow = head / matrix.width;
		
		if (printMode == STEP ||
			head == matrix.height * matrix.width - 1) { 
			if (paintHandler(&printMode, maze, matrix, head, minPaintRow, maxPaintRow) == -1) return -1;
			minPaintRow = maxPaintRow = head / matrix.width;
		} else if (printMode > STEP) {
			printMode--;
		} 
	}
	return 0;
}

int paintHandler (int *printMode, CELL *maze, MAT matrix, int head, int minPaintRow, int maxPaintRow) {
	char c;
	painter(matrix, maze, minPaintRow, maxPaintRow + 1, head);
	read(STDIN_FILENO, &c, 1);
	if (c == 'q') {
   	   	printQuitWizard(matrix);
   		read(STDIN_FILENO, &c, 1);
  	   	if (c == 'q') {
   	   		return -1;
   	   	} else {
   	   		clearWizard(matrix);
   	  	}
	} else if (c == 'N') {
		*printMode = QUICK;
	} else if (c == 's') {
		*printMode = SKIP;
	}
	return 0;
}

int smell (int head, int ptr, CELL *maze, MAT matrix, PUZZMK stack[]) {
	// check available neighbours
	_Bool neighbour[4] = {1, 1, 1, 1};
    boundChecker (neighbour, head, matrix);
	if (neighbour[N] &&
		(maze[head - matrix.width].block == 1 ||
		maze[head - matrix.width].visited == 1
		)) neighbour[N] = 0;
	if (neighbour[W] &&
		(maze[head - 1].block == 1 ||
		maze[head - 1].visited == 1
		)) neighbour[W] = 0;
	if (neighbour[S] &&
		(maze[head + matrix.width].block == 1 ||
		 maze[head + matrix.width].visited == 1
		 )) neighbour[S] = 0;
	if (neighbour[E] &&
		(maze[head + 1].block == 1 ||
		 maze[head + 1].visited == 1
		 )) neighbour[E] = 0;
	stack[ptr].availNeigh = 0;
	int dir = -1;
	int total = neighbour[N] + neighbour[W] + neighbour[S] + neighbour[E];
	for (int i = 0; i < 4; i++) {
		if (neighbour[i]) {
			dir = i;
			break;
		}
	}
	if (total > 1) stack[ptr].availNeigh = 1; 
	return dir;
}
