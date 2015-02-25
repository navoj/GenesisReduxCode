#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#define VERTICAL 64
#define HORIZONTAL 64
#define NUM_ELEMENTS 4096
#define VERT_MAX 63
#define HORZ_MAX 63
#define VERTICAL_LESS 63
#define HORIZONTAL_LESS 63
#define VERTICAL_PLUS 0
#define HORIZONTAL_PLUS 0

#define POINTS_PER 100
#define GENS	   1000
#define STAT_GENS  1000

#define NUM_COLORS	8
#define NUM_COLORS_MINUS_ONE 7

#define K_TO_THE_N           32768
#define K_TO_THE_N_MINUS_ONE 32767

#define SEVEN_EIGTHS 0.875

/*
int am[HORIZONTAL][VERTICAL];
int newam[HORIZONTAL][VERTICAL];
int rule_vector[K_TO_THE_N];
*/
int huge **am;
int huge **newam;
int huge *rule_vector;

int sigma;
double lambda;
int hamming;
double average_ham;

/*
int entropic[VERTICAL][HORIZONTAL][NUM_COLORS];
*/

int huge ***entropic;
double entropy;

int all_same;
int value_chk;

void main() {
	int i,j,k,l;
	int lambda_number;
	int number_of_nums;
	int counter;
	int stat_counter;
	int position;
	int ivec_plus, jvec_plus, ivec_less, jvec_less;
	int stats_counter;
	int rnd;
	double p0, p1, p2, p3, p4, p5, p6, p7;

	/* Initialize huge arrays */
	/*
	int am[HORIZONTAL][VERTICAL];
	int newam[HORIZONTAL][VERTICAL];
	int rule_vector[K_TO_THE_N];
	*/
	/*
	int entropic[VERTICAL][HORIZONTAL][NUM_COLORS];
	*/

	/* entropic */
	if ((entropic = (int***)halloc(VERTICAL,sizeof(int**))) == NULL) {
		printf("Failed to allocate memory for ***entropic\n");
		exit(0);
	}
	for (i=0; i < VERTICAL; i++) {
		entropic[i] = (int**)halloc(HORIZONTAL,sizeof(int*));
		for (j=0;j<HORIZONTAL;j++) {
			entropic[i][j] = (int*)halloc(NUM_COLORS,sizeof(int));
		}
	}

	/* rule_vector */

	if ((rule_vector = (int *)halloc(K_TO_THE_N,sizeof(int))) == NULL) {
		printf("Failed to allocated memory for *rule_vector\n");
		exit(0);
	}

	/* am */
	if ((am = (int**)halloc(HORIZONTAL,sizeof(int*))) == NULL) {
		printf("Failed to allocate memory for **am\n");
		exit(0);
	}
	for (i = 0; i < HORIZONTAL;i++) {
		am[i] = (int *)halloc(VERTICAL,sizeof(int));
	}

	/* newam */
	if ((newam = (int**)halloc(HORIZONTAL,sizeof(int*))) == NULL) {
		printf("Failed to allocate memory for **newam\n");
		exit(0);
	}
	for (i = 0; i < HORIZONTAL;i++) {
		newam[i] = (int *)halloc(VERTICAL,sizeof(int));
	}

	srand(getpid());

	/* select a lambda / cellular automata rules */
	/* 200 values of lambda */
	for (lambda = 0.004375; lambda <= SEVEN_EIGTHS; lambda = lambda + 0.004375) {
		/* select random rule based on lambda */
		number_of_nums = (int)(K_TO_THE_N * lambda);

		/* iterate each lambda for POINTS_PER times to collect statistics */
		for (lambda_number = 0; lambda_number < POINTS_PER; lambda_number++) {
			/* load screen array */
			for (j = 0; j < VERTICAL; ++j) {
				for (i = 0; i < HORIZONTAL; i++) {
					am[j][i] = rand()%NUM_COLORS;
				}
			}
		for (i = 0; i < K_TO_THE_N; i++) {
			rule_vector[i] = 0;
		}

		counter = number_of_nums - 7; /* 11111 -> 1, 22222 -> 2 etc. etc. */
		rule_vector[011111] = 1;
		rule_vector[022222] = 2;
		rule_vector[033333] = 3;
		rule_vector[044444] = 4;
		rule_vector[055555] = 5;
		rule_vector[066666] = 6;
		rule_vector[077777] = 7;

		while (counter) {
			/* select position */
			position = ((rand()%K_TO_THE_N_MINUS_ONE) + 1);

			/* check position */
			if (rule_vector[position] == 0) {
				rnd = ((rand()%NUM_COLORS_MINUS_ONE) + 1);
				rule_vector[position] = rnd;
				/* decrement counter */
				counter--;
			}
		} /* end rule selection */
		k = 0;

		/* for GENS generations */
		while (k < GENS) {
			/* generation counter */
			k++;

			/* read screeen array */
			for (j = 0; j < VERTICAL; j++) {
				jvec_plus = j + 1;
				jvec_less = j - 1;
				if (j == VERT_MAX) jvec_plus = VERTICAL_PLUS;
				else if (j == 0) jvec_less = VERTICAL_LESS;

				for (i = 0; i < HORIZONTAL; i++) {
					/* sum the neighborhood */
					ivec_plus = i + 1;
					ivec_less = i - 1;
					if (i == HORZ_MAX) ivec_plus = HORIZONTAL_PLUS;
					else if (i == 0) ivec_less = HORIZONTAL_LESS;
					sigma = am[j][i];
					sigma += (am[j][ivec_plus])<<3;
					sigma += (am[j][ivec_less])<<6;
					sigma += (am[jvec_plus][i])<<9;
					sigma += (am[jvec_less][i])<<12;

					/* new cell position */
					newam[j][i] = rule_vector[sigma];
				}
			} /* end read screen array */
			all_same = 1;
			value_chk = newam[0][0];
			for (j = 0; j < VERTICAL; j++) {
			  for (i = 0; i < HORIZONTAL; i++) {
			    am[j][i] = newam[j][i];
			    if (newam[j][i] != value_chk) { all_same = 0; }
			  }
			}
			if (all_same) break;
		} /* end GENS generation loop */
		
		if (!all_same) {
		  /* collect statistical information */
		  stat_counter = 0;
		  hamming = 0;
		  for (j = 0; j < VERTICAL; j++) 
		    for (i = 0; i < HORIZONTAL; i++) 
		      for (l = 0; l < NUM_COLORS; l++) 
			(entropic[j][i][l]) = 0;
			while (stat_counter < STAT_GENS) {
			  /* read screen array */
			  for (j = 0; j < VERTICAL; j++) {
			    jvec_plus = j + 1;
			    jvec_less = j - 1;
			    if (j == VERT_MAX) jvec_plus = VERTICAL_PLUS;
			    else if (j == 0) jvec_less = VERTICAL_LESS;

			    for (i = 0; i < HORIZONTAL; i++) {
			      /* sum the neighborhood */
			      ivec_plus = i + 1;
			      ivec_less = i - 1;
			      if (i == HORZ_MAX) ivec_plus = HORIZONTAL_PLUS;
			      else if (i == 0) ivec_less = HORIZONTAL_LESS;
			      sigma = am[j][i];
			      sigma += (am[j][ivec_plus])<<3;
			      sigma += (am[j][ivec_less])<<6;
			      sigma += (am[jvec_plus][i])<<9;
			      sigma += (am[jvec_less][i])<<12;

			      /* new cell color */
			      newam[j][i] = rule_vector[sigma];
			    }
			  } /* end read screen array */
			  for (j = 0; j < VERTICAL; j++) {
			    for (i = 0; i < HORIZONTAL; i++) {
			      if (newam[j][i] != am[j][i]) { hamming++; }
			      /* set new values for screen array */
			      am[j][i] = newam[j][i];
			      if (am[j][i] == 0) (entropic[j][i][0])++;
			      else if (am[j][i] == 1) (entropic[j][i][1])++;
			      else if (am[j][i] == 2) (entropic[j][i][2])++;
			      else if (am[j][i] == 3) (entropic[j][i][3])++;
			      else if (am[j][i] == 4) (entropic[j][i][4])++;
			      else if (am[j][i] == 5) (entropic[j][i][5])++;
			      else if (am[j][i] == 6) (entropic[j][i][6])++;
			      else if (am[j][i] == 7) (entropic[j][i][7])++;
				}
			  }
			  stat_counter++;
			} /* end stats. collection */
			average_ham = (double)hamming/(double)STAT_GENS;
			entropy = 0.0;
			for (j = 0; j < VERTICAL; j++) {
			  for (i = 0; i < HORIZONTAL; i++) {
			    p0 = (double)entropic[j][i][0]/(double)STAT_GENS;
			    p1 = (double)entropic[j][i][1]/(double)STAT_GENS;
			    p2 = (double)entropic[j][i][2]/(double)STAT_GENS;
			    p3 = (double)entropic[j][i][3]/(double)STAT_GENS;
			    p4 = (double)entropic[j][i][4]/(double)STAT_GENS;
			    p5 = (double)entropic[j][i][5]/(double)STAT_GENS;
			    p6 = (double)entropic[j][i][6]/(double)STAT_GENS;
			    p7 = (double)entropic[j][i][7]/(double)STAT_GENS;

			    if (p0 == 0.00) { /* entropy += 0.00; */ }
			    else if (p0 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p0*log(p0);
			    
			    if (p1 == 0.00) { /* entropy += 0.00; */ }
			    else if (p1 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p1*log(p1);

			    if (p2 == 0.00) { /* entropy += 0.00; */ }
			    else if (p2 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p2*log(p2);

			    if (p3 == 0.00) { /* entropy += 0.00; */ }
			    else if (p2 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p3*log(p3);

			    if (p4 == 0.00) { /* entropy += 0.00; */ }
			    else if (p4 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p4*log(p4);
			    
			    if (p5 == 0.00) { /* entropy += 0.00; */ }
			    else if (p5 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p5*log(p5);

			    if (p6 == 0.00) { /* entropy += 0.00; */ }
			    else if (p6 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p6*log(p6);

			    if (p7 == 0.00) { /* entropy += 0.00; */ }
			    else if (p7 == 1.00) { /* entropy += 0.00; */ }
				else entropy += p7*log(p7);
			    }
			  }
			  entropy = -(entropy/(double)NUM_ELEMENTS);
			}
			else { /* all the same */
			  average_ham = 0.00;
			  entropy = 0.00;
			}
			
			printf("%d\t", number_of_nums);
			printf("%f\t", lambda);
			printf("\t%f", average_ham);
			printf("\t%f\n", entropy);
			printf("\n");
			fflush(stdout);
		      } /* end lambda_number 0 to POINTS_PER loop */
		    } /* end main lambda loop */
			hfree((void huge *)rule_vector);
			hfree((void huge *)newam);
			hfree((void huge *)am);
			hfree((void huge *)entropic);
		  } /* end main() */







