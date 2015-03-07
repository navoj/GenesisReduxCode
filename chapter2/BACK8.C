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
static double invector[IUNITS];
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

	for(j=0;j<HUNITS;j++) {
		for(k=0;k<OUNITS;k++) {
			htooweight[j][k] = 0.1*rnd(dummy);
		}
	}
	/* setup graphics screen */
	if(_setvideomode(_MRES4COLOR) == 0) {
		printf("%s\n",error_message);
		exit(0);
	}
	_setbkcolor(_BLACK);
	_selectpalette(3);
	_setviewport(0,0,320,200);
	_setcolor(1);
	_rectangle(_GFILLINTERIOR,0,0,XMAXSCREEN,YMAXSCREEN);
	_setcolor(3);

	/* error rectangle */
	_rectangle(_GFILLINTERIOR,WBORD,WBORD,XOVER-WBORD,YDOWN-WBORD);
	/* neurons rect */
	_rectangle(_GFILLINTERIOR,XOVER,WBORD,XMAXSCREEN-WBORD,YMAXSCREEN-WBORD);
	/* space rect */
	_rectangle(_GFILLINTERIOR,WBORD,YDOWN,XOVER-WBORD,YMAXSCREEN-WBORD);
	/* set up error box axes */
	exmin = 2*WBORD;
	exmax = XOVER-2*WBORD;
	eymin = YDOWN-2*WBORD;
	eymax = 2*WBORD;

	_setcolor(0);
	_moveto(exmin,eymin);
	_lineto(exmin,eymax);
	_moveto(exmin,eymin);
	_lineto(exmax,eymin);

	for(u=1;u<11;u++) {
		_moveto(exmin+u*(exmax-exmin)/10,eymin);
		_lineto(exmin+u*(exmax-exmin)/10,eymin+(eymax-eymin)/15);
	}
	for(u=1;u<6;u++) {
		_moveto(exmin,eymin+u*(eymax-eymin)/5);
		_lineto(exmin+(exmax-exmin)/75,eymin+u*(eymax-eymin)/5);
	}
	/* set up axes for neuron box */
	nxmin = XOVER + WBORD;
	nymax = XMAXSCREEN - 2*WBORD;
	nymin = YMAXSCREEN - 2*WBORD;
	nymax = 4*WBORD;

	/* blue boxes */
	_setcolor(1);
	for(u=0;u<10;u++) {
		_rectangle(_GBORDER,nxmin,nymin+u*(nymax-nymin)/10,nxmax,nymin+(u+1)*(nymax-nymin)/10);
	}

	/* line */
	/* repeat this code after update */
	_setcolor(0);
	_moveto((nxmax+nxmin)/2,nymin);
	_lineto((nxmax+nxmin)/2,nymax);

	/* + and - signs */
	_moveto(nxmin+0.2*(nxmax-nxmin),nymax-WBORD);
	_lineto(nxmin+0.3*(nxmax-nxmin),nymax-WBORD);
	_moveto(nxmin+0.7*(nxmax-nxmin),nymax-WBORD);
	_lineto(nxmin+0.8*(nxmax-nxmin),nymax-WBORD);
	_moveto(nxmin+0.75*(nxmax-nxmin),nymax-WBORD+0.05*(nxmax-nxmin));
	_lineto(nxmin+0.75*(nxmax-nxmin),nymax-WBORD-0.05*(nxmax-nxmin));

	/* space axes */
	sxmin = 2*WBORD;
	sxmax = 0.75*(XOVER-4*WBORD)+2*WBORD;
	symin = YMAXSCREEN - 2*WBORD;
	symax = YDOWN + WBORD;

	_setcolor(0);
	_moveto(sxmin,symax);
	_lineto(sxmin,symin);
	_lineto(sxmax,symin);
	for(u=1;u<11;u++) {
		_moveto(sxmin+u*(sxmax-sxmin)/10,symin);
		_lineto(sxmin+u*(sxmax-sxmin)/10,symin+(symax-symin)/40);
	}
	for(u=1;u<13;u++) {
		_moveto(sxmin,symin+u*(symax-symin)/12);
		_lineto(sxmin+(sxmax-sxmin)/60,symin+u*(symax-symin)/12);
	}

	/* learning loop */
	for(iterate=0;iterate<TRMAX;iterate++) {
		if(kbhit()) {
			_setvideomode(_DEFAULTMODE);
			exit(0);
		}

		/* select random numbers */
		t1 = rnd(dummy);
		t2 = rnd(dummy);
		vel = 0.116 + (0.2 - 0.116)*t1;
		theta = (60 + 30*t2)*PI/180;

		/* compute the target vector using the model */
		target[0] = 10*vel*cos(theta);
		target[1] = 10*vel*sin(theta)-0.5*a*100;

		/* select input vector */
		invector[0] = t1;
		invector[1] = t2;
		invector[2] = 1.0;

		/* feed forward -- input to hidden */
		for(j=0;j<HUNITS;j++) {
			netih[j] = 0;
			for(i=0;i<IUNITS;i++) {
				netih[j] = netih[j] + itohweight[i][j]*invector[i];
			}
			hout[j] = tanh(netih[j]);
		}

		/* feed forward -- hidden to output */
		for(k=0;k<OUNITS;k++) {
			oout[k] = 0;
			for(j=0;j<HUNITS;j++) {
				oout[k] = oout[k] + htooweight[j][k]*hout[j];
			}
			error[k] = target[k] - oout[k];
		}

		/* compute the error correction for htooweight matrix elements */
		for(k=0;k<OUNITS;k++) {
			for(j=0;j<HUNITS;j++) {
				delta = error[k]*hout[j]*eta2;
				htooweight[j][k] = htooweight[j][k]+delta;
			}
		}

		/* compute the error correction for itohweitht matrix elements */
		for(j=0;j<HUNITS;j++) {
			/* first find sigma(e(k)u(kj)) */
			sigma[j] = 0;
			for(k=0;k<OUNITS;k++) {
				sigma[j] = sigma[j] + error[k]*htooweight[j][k];
			}
			temp = (double)(1.0/cosh(netih[j]));
			fprime[j] = temp *temp;
		}

		for(i=0;i<IUNITS;i++) {
			for(j=0;j<HUNITS;j++) {
				delta = eta1*fprime[j]*sigma[j]*invector[i];
				itohweight[i][j] = itohweight[i][j] + delta;
			}
		}
	d1 = pow((oout[0]-target[0]),2.0);
	d2 = pow((oout[1]-target[1]),2.0);
	miss = sqrt(d1+d2);
	d = d + (sqrt(d1+d2))/FREQUENCY;
	if (d>0.5) {
		d = 0.5;
	}
	if (miss <= 0.01) {
		percent = percent + 100/FREQUENCY;
	}
	counter = counter + 1;
	if (counter == FREQUENCY) {
		/* plot stuff here */
		_setcolor(0);
		_setpixel(exmin+iterate*(exmax-exmin)/TRMAX,eymin+percent*(eymax-eymin)/ERMAX);
		/* neurons */
		_setcolor(3);
		_rectangle(_GFILLINTERIOR,nxmin,nymin,nxmax,nymax);
		_setcolor(1);
		for(u=0;u<10;u++) {
			_rectange(_GBORDER,nxmin,nymin+u*(nymax-nymin)/10,nxmax,nymin+(u+1)*(nymax-nymin)/10);
		}
		_setcolor(2);
		for(u=0;u<10;u++) {
			_rectangle(_GFILLINTERIOR,(nxmax-nxmin)/2,nymin+u*(nymax-nymin)/10,(nxmax+nxmin)/2+(nxmax-nxmin)*hout[u]/2,nymin+(u+1)*(nymax-nymin)/10);
		}
		_setcolor(0);
		_moveto((nxmax+nxmin)/2,nymin);
		_lineto((nxmax+nxmin)/2,nymax);
		/* space data plot */
		_setcliprgn(sxmin+3,symin-3,XOVER-WBORD,YDOWN+WBORD);
		_setcolor(3);
		_rectangle(_GFILLINTERIOR,sxmin+3,symin-3,XOVER-WBORD,YDOWN+WBORD);
		oldxt = 0;
		oldyt = 0;
		_moveto(sxmin,symin);
		for(u=0;u<51;u++) {
			_setcolor(2);
			xo = 1.3 - u*(1.3 - oout[0])/50.0;
			yo = u*oout[1]/50.0;
			xt = vel*u*cos(theta)/5.0;
			yt = (vel*u*sin(theta)/5.0) - 0.5*a*u*u/25.0;
			_setpixel(sxmin+xo*(sxmax-sxmin),symin+yo*(symax-symin)/1.2);
			_setcolor(0);
			_moveto(sxmin+oldxt*(sxmax-sxmin),symin+oldyt*(symax-symin)/1.2);
			_lineto(sxmin+xt*(sxmax-sxmin), symin+yt*(symax-symin)/1.2);
			oldxt = xt;
			oldyt = yt;
		}
		if(miss <= 0.01) {
			_setcolor(2);
			_moveto(sxmin+xt*(sxmax-sxmin),symin+yt*(symax-symin)/1.2+3);
			_lineto(sxmin+xt*(sxmax-sxmin),symin+yt*(symax-sxmin)/1.2-3);
			_moveto(sxmin+xt*(sxmax-sxmin)-3,symin+yt*(symax-symin)/1.2);
			_lineto(sxmin+xt*(sxmax-sxmin)+3,symin+yt*(symax-symin)/1.2);
			_moveto(sxmin+xt*(sxmax-sxmin)-2,symin+yt*(symax-symin)/1.2-2);
			_lineto(sxmin+xt*(sxmax-sxmin)+2,symin+yt*(symax-symin)/1.2-2);
			_lineto(sxmin+xt*(sxmax-sxmin)-2,symin+yt*(symax-symin)/1.2+2);
		}
		_setcliprgn(0,0,XMAXSCREEN,YMAXSCREEN);

		percent = 0;
		d = 0;
		counter = 0;
	}
}
while(!kbhit());
_setvideomode(_DEFAULTMODE);
}

double rnd(idum)
long *idum;
{
	static long iy,ir[98];
	static int iff = 0;
	int j;

	if(*idum < 0 || iff == 0) {
		iff = 1;
		if((*idum=(IC-(*idum)) % M) < 0) *idum = -(*idum);
		for(j=1;j<=97;j++) {
			*idum = (IA*(*idum)+IC) % M;
			ir[j] = (*idum);
		}
		*idum =(IA*(*idum)+IC) % M;
		iy = (*idum);
	}
	j=1+97.0*iy/M;
	iy = ir[j];
	*idum = (IA*(*idum)+IC) % M;
	ir[j] = (*idum);
	return (double)iy/M;
}
