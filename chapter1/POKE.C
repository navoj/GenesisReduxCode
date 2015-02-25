#include <stdio.h>
#include <stdlib.h>

void main(void) {
	unsigned char far *a;
	int i,j,counter=0;
	int address;
	a=(char far*) 0xb8000000;
	for(;;) {
		counter = 0;
		for(i=1;i<24;i++) {
			for(j=2;j<159;j++) {
				address = counter;
				*(a+address)=88;
				counter++;
			}
		}
		if(kbhit()) {
			printf("%d ",address);
			exit(0);
		}
	}
}
