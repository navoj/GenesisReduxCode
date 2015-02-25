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

unsigned short int rule_vector[32];

struct videoconfig vc;
char error_message[] = "this video mode is not supported";

void main() {
  int i, j;
  int k = 0;
  int sigma;
  double rnd;
  int ivec_plus, jvec_plus;
  int ivec_less, jvec_less;

	/*
	unsigned short int huge am[320][200];
	unsigned short int huge newam[320][200];
	*/
  if ((am = (unsigned short int**)halloc(320,sizeof(unsigned short int*))) == NULL) {
	printf("Memory allocation for **am failed.\n");
	exit(0);
  }
  for (i=0;i<320;i++) {
	am[i] = (unsigned short int*)halloc(200,sizeof(unsigned short int));
  }
  if ((newam = (unsigned short int**)halloc(320,sizeof(unsigned short int*))) == NULL) {
	printf("Memory allocation for **newam failed.\n");
	exit(0);
  }
  for (i=0;i<320;i++) {
	newam[i] = (unsigned short int*)halloc(200,sizeof(unsigned short int));
  }



  /* set mode of screen */
  if (_setvideomode(_MRESNOCOLOR) == 0)
    {
      printf("%s\n", error_message);
      exit(0);
    }
  _getvideoconfig(&vc);
  _clearscreen(_GCLEARSCREEN);

  _setviewport(0, 0, 320, 200);

  /* load array */
  for (j = 0; j < 200; ++j) {
    for (i = 0; i < 320; i++) {
      rnd = rand()/32767.0;
      if (rnd < 0.4) {
	am[i][j] = 1;
	_setcolor(1);
      } else {
	am[i][j] = 0;
	_setcolor(0);
      }
      _setpixel(i, j);
    }
  }

  /* set rule vector */
  rule_vector[0] = 0;
  rule_vector[1] = 1;
  rule_vector[2] = 0;
  rule_vector[3] = 1;
  rule_vector[4] = 0;
  rule_vector[5] = 1;
  rule_vector[6] = 0;
  rule_vector[7] = 1;
  rule_vector[8] = 0;
  rule_vector[9] = 1;
  rule_vector[10] = 0;
  rule_vector[11] = 0;
  rule_vector[12] = 0;
  rule_vector[13] = 0;
  rule_vector[14] = 1;
  rule_vector[15] = 1;
  rule_vector[16] = 0;
  rule_vector[17] = 1;
  rule_vector[18] = 0;
  rule_vector[19] = 1;
  rule_vector[20] = 0;
  rule_vector[21] = 0;
  rule_vector[22] = 1;
  rule_vector[23] = 1;
  rule_vector[24] = 0;
  rule_vector[25] = 1;
  rule_vector[26] = 1;
  rule_vector[27] = 1;
  rule_vector[28] = 1;
  rule_vector[29] = 1;
  rule_vector[30] = 1;
  rule_vector[31] = 1;

  /* run the cellular automata */
  while (!kbhit()) {
    /* generation counter */
    k++;

    /* read array */
    for (j = 0; j < 200; j++) {
      jvec_plus = j + 1;
      jvec_less = j - 1;
      if (j == 199) jvec_plus = 0;
      else if (j == 0) jvec_less = 199;

      for (i = 0; i < 320; i++) {
	/* sum the neighborhood */
	ivec_plus = i + 1;
	ivec_less = i - 1;
	if (i == 319) ivec_plus = 0;
	else if (i == 0) ivec_less = 319;

	sigma = am[i][j];
	sigma += (am[i][jvec_plus])<<1;
	sigma += (am[i][jvec_less])<<2;
	sigma += (am[ivec_plus][j])<<3;
	sigma += (am[ivec_less][j])<<4;

	/* new cell state */
	newam[i][j] = rule_vector[sigma];
      }
    }

    /* set new values */
    for (j = 0; j < 200; j++) {
      for (i = 0; i < 320; i++) {
	if (newam[i][j] == 1) {
	  am[i][j] = 1;
	  _setcolor(1);
	}
	if (newam[i][j] == 0) {
	  am[i][j] = 0;
	  _setcolor(0);
	}
	_setpixel(i,j);
      }
    }
  }

  _clearscreen(_GCLEARSCREEN);
  _setvideomode(_DEFAULTMODE);
  printf("number of generations %d\n", k);
  hfree((void huge*)am);
  hfree((void huge*)newam);
}





