/* program to simulate a boltzman update of a hopfield neural network
   Transputer T414 version 11/88 */

/* 
old include paths:

#include "c:\lsc\include\stdio.h"
#include "c:\lsc\include\math.h"
#include "c:\lsc\include\float.h"

*/

#include <stdio.h>
#include <math.h>
#include <float.h>

#define RNDMAX 2147483647.0
#define DILUTIONFACTOR 0.2

static float weight[100][100];
static long int ivector[100],activation[100],oldvalue[100];
static long int tvector[100],ttvector[100];

void main(void) {
  /* declarations */
  long int i,j,k,memories,updateno,ielement = 0;
  int noneurons, nomemories;
  float hightemp,lowtemp;
  float temperature;
  double logistic;
  float netinput;
  float randomnumber;
  double t, energy;
  float threshold;
  int seed;

  int rnd();
  int random();
  double expt(); /* compute exp base e */
  double loge(); /* compute log base e */

  /* user input */
  printf("input the number of neurons \n");
  scanf("%d",&noneurons);
  printf("input and threshold\n");
  scanf("%f",&threshold);
  printf("input seed \n");
  scanf("%d",&seed);

  /* number of memory states */
  nomemories = (int)(0.1*noneurons);

  /* calculate the weight matrix from the product of the memory vectorss with their transpose */
  srand(seed);

  for (memories = 0; memories <= nomemories-1; ++memories) {
    for (k = 0; k <= noneurons-1; ++k) {
      tvector[k] = (int)(rnd()); /* the memory state */
      printf("%d ", tvector[k]);
      ttvector[k] = tvector[k]; /* the transpose state */
    }
    printf("\n\n");
    for (i = 0; i <= noneurons-1; ++i) {
      /* sum the matrices */
      for (j = 0; j <= noneurons-1; ++j) {
	weight[i][j] = weight[i][j]+tvector[i]*ttvector[j];
	if  (i == j) {
	  weight[i][j] = 0;
	} else {
	  weight[i][j] = weight[i][j];
	}
      }
    }
  }

  /* print out the final weight matrix */
  for (i = 0; i <= noneurons-1; ++i) {
    for (j = 0; j <= noneurons-1; ++j) {
      printf("%d ", (int)(weight[i][j]));
    }
    /* printf("\n"); */
  }
  printf("\n\n");

  /* clip the matrix here -- if needed */

  /* input a vector to use as test vector */
  for (k = 0; k <= noneurons-1; ++k) {
    printf("enter the input vector element %d\n", k);
    scanf("%ld", &ielement);
    ivector[k] = ielement;
    activation[k] = ivector[k];
  }

  /* find a random vector to use as input to test the network ivector */
  /*
    for (k=0; k <= noneurons-1; ++k) {
      ivector[k] = (int)(rnd());
      activation[k] = ivector[k];
      printf("%d ", ivector[k]);
      }
      printf("\n\n");
  */

  /* begin calculation loops from high temperature to low temperature */
  temperature = 10.0;
  for (t = 2; t <= 50; t = t + 2) {
    /* save old value for use later in energy calculation */
    for (j = 0; j <= noneurons-1; ++j) {
      oldvalue[j] = activation[j];
    }
    temperature = temperature/loge(1+t);
    for (updateno = 0; updateno <= t*100.0*noneurons; updateno++) {
      /* codde here for basic calculation */
      i = (long int)(random(noneurons));
      /* netinput for single neuron */
      netinput = 0;
      for (j = 0; j <= noneurons-1; ++j) {
	netinput += activation[j]*weight[i][j];
      }
      netinput = netinput - threshold;
      /* printf("%f ", netinput); */
      /* end basic code here */

      /* update algorithm here */
      logistic = (double)(1.0/(1.0+expt(netinput,temperature)));
      randomnumber = (double)(rand()/(double)(RNDMAX));
      if (randomnumber > logistic) {
	activation[i] = 1;
      } else {
	    activation[i] = 0;
      }

      if (temperature < 0.05) {
	if (netinput == 0.0)
	  activation[i] = 0;
	if (netinput == 1.0)
	  activation[i] = 1;
      }
    }

    /* energy calculation */
    energy = 0;
    for (j = 0; j <= noneurons-1; j++) {
      energy = energy + (oldvalue[j]*activation[j]);
      energy = -energy;
      printf("\n");
      for (i = 0; i <= noneurons-1; i++) {
	printf("%d ", (int)(activation[i]));
      }
      printf("\n%lf %lf\n\n ", temperature, energy);
    }
  }
}

/* return a random 0 or 1 for use in the vector generation */
int rnd() {
  int result;
  result = rnd();
  if (result < DILUTIONFACTOR*(double)(RNDMAX))
    result = 1;
  else
    result = 0;
  return(result);
}

/* return a random integer between 0 and number of neurons for use in random update of the network */
int random(int n) {
  int result;
  float tmp;

  tmp = rand()/RNDMAX;
  result = (int)(n * tmp);

  return(result);
}

/* calculate an exponent */
double expt(float net, float temp) {
  double answer;
  float x;

  x = -1*net/tem;

  answer = 1+x+x*x/2;
  answer = answer + x*x*x/6;
  answer = answer + x*x*x*x/24;
  answer = answer + x*x*x*x*x/120;
  answer = answer + x*x*x*x*x*x/720;
  answer = answer + x*x*x*x*x*x*x/5040;
  answer = answer + x*x*x*x*x*x*x*x/40320;
  answer = answer + x*x*x*x*x*x*x*x*x/362880;
  answer = answer + x*x*x*x*x*x*x*x*x*x/3628800;

  return(answer);
}

/* calculate log base e */
double loge(double temp) {
  int ct, lt;
  double t,y,sqrt();

  ct = 1;
  lt = 1;

  if (temp < 1) {
    temp = 1/temp;
    lt = -1;
  }

  while (temp > 2) {
    temp = sqrt(temp);
    ct *= 2;
  }

  t = (temp-1)/(temp+1);
  y = 0.868591718 * t;
  y = y + 0.289335524 * (t*t*t);
  y = y + 0.17522071 * (t*t*t*t*t);
  y = y + 0.094376476 * (t*t*t*t*t*t*t);
  y = y + 0.1799337714 *(t*t*t*t*t*t*t*t*t);

  return(y/0.43429466*ct*lt);
}



	    
