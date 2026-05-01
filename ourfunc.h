#include <stdio.h>      // printf()
#include <stdlib.h>
#include <sys/ioctl.h>  // ioctl(), TIOCGWINSZ
#include <unistd.h>     // STDIN_FILENO, STDOUT_FILENO
#include <termios.h>    // terminal attribute control
#include<limits.h>		// for u_short 
#include <time.h>

typedef struct cell {
	_Bool block;
	_Bool visited;
	_Bool path;
} CELL;

typedef struct mat {
	u_short height;
	u_short width;
	u_short padtop;
	u_short padleft;
} MAT;

typedef struct PUZZMK {
	_Bool availNeigh;
	u_short pathind;
} PUZZMK;


#define N 2
#define W 3
#define S 1
#define E 0

void setMaze(CELL **);
int getMatSize(MAT *, struct winsize);
void setPage(struct winsize, MAT*, CELL *);
void setTermProp(struct termios *, struct termios *);
void revertTermProp(struct termios *);
int puzzleEngine (MAT, struct winsize);
void boundChecker (_Bool[], int, MAT);
void painter (MAT matrix, CELL *maze, int, int, int);
void printQuitWizard(MAT);
void clearWizard(MAT);




