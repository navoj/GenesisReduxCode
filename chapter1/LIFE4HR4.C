#include <float.h>
#include <stdio.h>
#include <graph.h>
#include <math.h>
#include <malloc.h>

/*
unsigned short int huge am[320][200];
unsigned short int huge newam[320][200];
*/
unsigned short int huge **am;
unsigned short int huge **newam;

struct videoconfig vc;
char error_message[] = "this video mode is not supported";

void main(void) {
	/* declarations */
	int i,j,k;
	int sigma;
	int xmaxscreen=320, ymaxscreen=200;
	int xcoord,ycoord;
	double rnd;
	if ((am=(unsigned short int**)halloc(320,sizeof(unsigned short int*)))==NULL) {
		printf("Could not allocate am\n");
		exit(0);
	}
	for(i=0;i<320;i++) {
		am[i] = (unsigned short int*)halloc(200,sizeof(unsigned short int));
	}
	if ((newam=(unsigned short int**)halloc(320,sizeof(unsigned short int*)))==NULL) {
		printf("Could not allocate newam\n");
		exit(0);
	}
	for(i=0;i<320;i++) {
		newam[i] = (unsigned short int*)halloc(200,sizeof(unsigned short int));
	}


	/* set mode of screen */
	if (_setvideomode(_MRESNOCOLOR) == 0) {
		printf("%s\n",error_message);
		exit(0);
	}
	_getvideoconfig(&vc);
	_clearscreen(_GCLEARSCREEN);
	_setviewport(0,0,320,200);

	/* load array */
	for(j=0;j<200;++j) {
		for(i=0;i<320;i++) {
			rnd = rand()/32767.0;
			if(rnd<0.4) {
				am[i][j] = 1;
				_setcolor(1);
			} else {
				am[i][j] = 0;
				_setcolor(0);
			}
			_setpixel(i,j);
		}
	}

	/* cellular automata rules */
	for(k=0;k<99;k++) {
		/* read array */
		for(j=0;j<200;j++) {
			for(i=0;i<320;i++) {
				sigma = am[(i%320)-1][(j%200)-1]+am[i%320][(j%200)-1]+am[(i%320)+1][(j%200)-1];
				sigma = sigma + am[(i%320)-1][j%200]+am[(i%320)+1][j%200];
				sigma = sigma + am[(i%320)-1][(j%200)+1]+am[i%320][(j%200)+1]+am[(i%320)+1][(j%200)+1];

				if (sigma<=1) {
					newam[i%320][j%200] = 0;
				}
				if (sigma==3) {
					newam[i%320][j%200] = 1;
				}
				if (sigma>=4) {
					newam[i%320][j%200] = 0;
				}
			}
		}

		/* set new values */
		for(j=0;j<200;j++) {
			for(i=0;i<320;i++) {
				if(newam[i%320][j%200]==1) {
					am[i%320][j%200] = 1;
					_setcolor(1);
				}
				if(newam[i%320][j%200]==0) {
					am[i%320][j%200] = 0;
					_setcolor(0);
				}
				_setpixel(i,j);
			}
		}
	}

	/* clear screen & return control hit enter */
	while(!kbhit());
	_clearscreen(_GCLEARSCREEN);
	_setvideomode(_DEFAULTMODE);
	hfree((void huge*)am);
	hfree((void huge*)newam);
}


