#include <float.h>
#include <stdio.h>
#include <graph.h>
#include <math.h>

unsigned short int am[100][100];
unsigned short int newam[100][100];

struct videoconfig vc;
char error_message[] = "this video mode is not supported";
void main() {
	/* declarations */
	int i,j,k;
	int sigma;
	int xmaxscreen=100, ymaxscreen=100;
	int xcoord,ycoord;
	double rnd;

	/* set mode of screen */
	if (_setvideomode(_MRESNOCOLOR) == 0)
	{
		printf("%s\n", error_message);
		exit(0);
	}
	_getvideoconfig(&vc);
	_clearscreen(_GCLEARSCREEN);

	_setviewport(0,0,100,100);

	/* load array */
	for (j=0; j < 100; ++j) {
		for (i=0; i < 100; i++) {
			rnd = rand()/32767.0;
			if (rnd < 0.55) {
				am[i][j] = 1;
				_setcolor(1);
			}
			else {
				am[i][j] = 0;
				_setcolor(0);
			}
			_setpixel(i,j);
		}
	}

	/* cellular automata rules */
	for (k=0; k < 50; k++) {
		/* read array */
		for (j=0; j < 100; j++) {
			for (i=0; i < 100; i++) {
				sigma = am[i%100][j%100] + am[i%100][(j%100)-1];
				sigma = sigma + am[(i%100)-1][j%100] + am[(i%100)+1][j%100];
				sigma = sigma + am[i%100][(j%100)+1];
				/* the following line takes care of all the cases not covered below */
				newam[i%100][j%100] = am[i%100][j%100];

				if ((sigma == 2) & (am[i%100][j%100] == 1)) {
					newam[i%100][j%100] = 0;
				}
				if ((sigma == 3) & (am[i%100][j%100] == 0)) {
					newam[i%100][j%100] = 1;
				}
				if ((sigma == 4) & (am[i%100][j%100] == 0)) {
					newam[i%100][j%100] = 1;
				}
			}
		}

		/* set new values */
		for (j=0; j < 100; j++) {
			for (i = 0; i < 100; i++) {
				am[i][j] = newam[i][j];
				_setcolor(am[i][j]);
				_setpixel(i,j);
			}
		}
	}

	/* clear screen & return control hit enter */
	while (!kbhit());
	_clearscreen(_GCLEARSCREEN);
	_setvideomode(_DEFAULTMODE);
}
