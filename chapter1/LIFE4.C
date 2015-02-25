/* life simulation */
#include <stdio.h>
#include <stdlib.h>

static unsigned short int am[25][160];

FILE *fe;

void main(void) {
	/* declarations */
	unsigned char far *a;
	unsigned short int b;
	int i,j,x,y,counter,sigma,address;
	/* counter is used in address decoding */
	int cycle = 0;
	int seed;

	a = (unsigned char far *) 0xb8000000;

	/* user input */
	printf("input random seed \n");
	scanf("%d",&seed);
	srand(seed);

	/* clear graphics memory */
	counter = 0;
	for(x=0;x<25;x++) {
		for(y=0;y<160;y++) {
			*(a+counter) = 0;
			counter++;
		}
	}
	/* poke (random) */
	counter = 0;
	for(x=0;x<25;x++) {
		for(y=0;y<160;y++) {
			if(rand()/32767.0 < 0.5) {
				*(a+counter)=255;
			}
			counter++;
		}
	}
	/* begin life iterations */
	for(;;) {
		if(kbhit()) {
			exit(0);
		}
		/* peek to fill matrix */
		counter = 0;
		for(i=1;i<24;i++) {
			for(j=2;j<159;j++) {
				address = i*160+j;
				b = *(a+address);
				am[i][j] = b;
				counter = counter+1;
			}
		}

		/* cellular automata rules for life */
		counter = 0;
		for(i=1;i<24;i++) {
			for(j=2;j<159;j++) {
				sigma = am[i-1][j-1]+am[i][j-1]+am[i+1][j-1];
				sigma = sigma + am[i-1][j]+am[i+1][j];
				sigma = sigma + am[i-1][j+1]+am[i][j+1]+am[i+1][j+1];
				counter = counter + 1;
				address = i*160+j;
				if(sigma <= 255*1) {
					*(a+address) = 0;
				}
				if(sigma == 255*3) {
					*(a+address) = 255;
				}
				if (sigma >= 255*4) {
					*(a+address) = 0;
				}
			}
		}
	}
}

