#include "ourfunc.h"

int makeProblem (MAT *, CELL *, PUZZMK *, struct winsize);
int solveProblem (MAT, CELL *, PUZZMK *);
int direction(int, int, CELL*, MAT, PUZZMK[]);
int validate(int, int, CELL *, MAT);

int puzzleEngine (MAT matrix, struct winsize window) {
	CELL *maze = (CELL *) malloc(matrix.height * matrix.width *sizeof(CELL));
	PUZZMK *stack = (PUZZMK *) malloc(matrix.height * matrix.width *sizeof(PUZZMK));
	if (maze == NULL || stack == NULL) { // if there is no available memory in system malloc returns NULL
		printf("Memory insufficient, quitting the program\n");
		fflush(stdout);
		return -1;
	}
	for (int i = 0; i < matrix.height; i++) {
		for (int j = 0; j < matrix.width; j++) {
		 	maze[i * matrix.width + j] = (CELL) {1, 0, 0};
		}
	}
	makeProblem(&matrix, maze, stack, window);
		solveProblem(matrix, maze, stack);

	free(stack);
	free(maze);
	return 0;
}

int makeProblem (MAT *matrix, CELL *maze, PUZZMK *stack, struct winsize window) {
	int ptr = 0;
	int head = 0;
	stack[ptr].pathind = head;
	maze[head] = (CELL) {0, 0, 0}; // not block
	do {
		int dir;
		dir = direction(head, ptr, maze, *matrix, stack);
		// now we have direction
		// backtracking condition
		if (dir == -1) {
			do {
				ptr--;
			} while (ptr > -1 && stack[ptr].availNeigh == 0);
			if (ptr != -1) head = stack[ptr].pathind;
			continue;
		}
		//forward moving
		switch (dir) {
		case E:
			head++; break;
		case W:
			head--; break;
		case N:
			head -= matrix->width; break;
		case S:
			head += matrix->width; break;
		}
		ptr++;
		stack[ptr].pathind = head;
		maze[head] = (CELL) {0, 0, 0};
	} while (ptr > -1);
	// explicitly making destination valid
	int destiny = (matrix->height - 1) * matrix->width + (matrix->width - 1);
	maze[destiny] = (CELL) {0, 0, 0};
	maze[destiny - 1] = (CELL) {0, 0, 0};
	maze[destiny - matrix->width] = (CELL) {0, 0, 0};
	maze[destiny - matrix->width - 1] = (CELL) {0, 0, 0};
	setPage(window, matrix, maze);
	return 0;
}

int direction (int head, int ptr, CELL *maze, MAT matrix, PUZZMK stack[]) {
	// check available neighbours
	_Bool neighbour[4] = {1, 1, 1, 1};
	boundChecker (neighbour, head, matrix);
    if (neighbour[W] && !validate(head - 1, head, maze, matrix)) neighbour[W] = 0;				// west
	if (neighbour[N] && !validate(head - matrix.width, head, maze, matrix)) neighbour[N] = 0;	// north
	if (neighbour[S] && !validate(head + matrix.width, head, maze, matrix)) neighbour[S] = 0;	// south
	if (neighbour[E] && !validate(head + 1, head, maze, matrix)) neighbour[E] = 0;				// east
	
	if (!neighbour[N] && !neighbour[W] && !neighbour[S] && !neighbour[E]) { // time to backtrack
		return -1;
	}
	int dir;
	int total = neighbour[N] + neighbour[W] + neighbour[S] + neighbour[E];
	int r = rand() % total;
	for (dir = 0; dir < 4; dir++) {
		if (neighbour[dir]) {
			if (r == 0) break;
			r--;
		}
	}
	if (total > 1) stack[ptr].availNeigh = 1;
	return dir;
}

void boundChecker (_Bool neighbour [], int head, MAT matrix) {
	if (head < matrix.width) { // row 1
		neighbour[N] = 0;
	}
	if (head % matrix.width == 0) { // col 1
		neighbour[W] = 0;
	}
	if ((head + 1) % matrix.width == 0) {// last col
		neighbour[E] = 0;
	}
	if (head >= (matrix.width * (matrix.height - 1))) { //last row
	   	neighbour[S] = 0;
	}
}

int validate(int futureHead, int head, CELL *maze, MAT matrix) {
	if (maze[futureHead].block == 0) return 0;
	if (futureHead >= matrix.width &&
        maze[futureHead - matrix.width].block == 0 &&
        futureHead - matrix.width != head) return 0;
	if (futureHead % matrix.width != 0 &&
        maze[futureHead - 1].block == 0 &&
        futureHead - 1 != head) return 0;
    if (futureHead < matrix.width * (matrix.height - 1) &&
        maze[futureHead + matrix.width].block == 0 &&
        futureHead + matrix.width != head) return 0;
	if ((futureHead + 1) % matrix.width != 0 &&
        maze[futureHead + 1].block == 0 &&
        futureHead + 1 != head) return 0;	
    return 1;
}

