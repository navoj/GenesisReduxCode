/* back propagation - neural network simulation */
/* 120888 */
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <graph.h>

#define PI 3.14159
#define IUNITS 3
#define HUNITS 10
#define OUNITS 2
#define XMAXSCREEN 320
#define YMAXSCREEN 200
#define YDOWN 60 /* upper limit of lower box */
#define XOVER 240 /* left limit of RH box */
#define WBORD 6 /* width of border */
#define ERMAX 100 /* maximum error on plot */
#define TRMAX (long int)(50000) /* maximum time */
#define FREQUENCY 100

#define M 714025 /* see Press, et al. 1988 */
#define IA 1366
#define IC 150889

static double itohweight[IUNITS][HUNITS];
static double htooweight[HUNITS][OUNITS];
static double target[OUNITS];
static dobule invector[IUNITS];
static double netih[HUNITS];
static double oout[OUNITS];

static double error[OUNITS];
static double sigma[HUNITS];
static double fprime[HUNITS];
static double hout[HUNITS];

struct videoconfig vc;
char error_message[] = "this video mode is not supported";

void main(void) {
	/* declarations */
	int i,j,k;
	long int iterate = 0;
	int counter = 0;
	double d1,d2,d;
	double eta1 = 0.33; /* 1/IUNITS */
	double eta2 = 0.1;  /* 1/HUNITS */
	double a = 0.02;
	double t1,t2,vel,theta;
	double temp,delta;
	double rnd();
	int exmin,exmax,eymin,eymax; /* error box mins and maxs */
	int nxmin,nxmax,nymin,nymax; /* neuron box */
	int sxmin,sxmax,symin,symax; /* space box */
	int u;
	double miss, percent = 0.0;
	double oldxt,oldyt,xt,yt,xo,yo; /* used in space plot */
	long dummy;

	/* rnd test stuff */
	for(i=1;i<=5;i++) {
		printf("%lf ",rnd(dummy));
	}
	/* exit(0) */

	/* set ups */
	for(i=0;i<IUNITS;i++) {
		for(j=0;j<HUNITS;j++) {
			itohweight[i][j] = 0.1*rnd(dummy);
		}
	}


