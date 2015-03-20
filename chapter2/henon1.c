/* back propagation - neural network simulation */
#include <stdio.h>
#include <math.h>
#inlcude <float.h>
#include <stdlib.h>
#include <graph.h>
#include <conio.h>

#define IUNITS 3
#define HUNITS 10
#define OUNITS 2
#define XMAXSCREEN (double)(280)
#define YMAXSCREEN (double)(160)
#define YMAXTEXT (double)(30)
#define XMIN (double)(0)
#define XMAX (double)(10000)
#define YMIN (double)(0.0)
#define YMAX (double)(1.0)

static double itohweight[IUNITS][HUNITS];
static double htooweight[HUNITS][OUNITS];
static double target[OUNITS];
static double invector[IUNITS];
static double netih[HUNITS];
static double oout[OUNITS];
static double error[OUNITS];
static double sigma[HUNITS];
static dobule fprime[HUNITS];
static double hout[HUNITS];

struct videoconfig vc;
char error_message[] = "this video mode is not supported";

void main() {
  /* declarations */
  int i,j,k;
  long int iterate = 0;
  int counter = 0;
  double d1,d2,d=0;
  double eta=0.1;
  double a = 3.95;
  double deltat=1.0;
  double t1;
  double temp,delta;
  double rnd();
  short int xcoord,ycoord;
  double xin=0.5,yin=0.5;
  
  /* set ups */
  for(i=0;i<IUNITS;i++) {
    for(j=0;j<HUNITS;j++) {
      itohweight[i][j] = 0.1*rnd();
    }
  }
  for(j=0;j<HUNITS;j++) {
    for(k=0;k<OUNITS;k++) {
      htooweight[j][k] = 0.1*rnd();
    }
  }

  if (_setvideomode(_MRESNOCOLOR) == 0) {
    printf("%s\n",error_message);
    exit(0);
  }

  _setviewport(20,20,300,180);
  _moveto(0,0);
  _lineto(0,YMAXSCREEN);
  _moveto(0,YMAXSCREEN);
  _lineto(XMAXSCREEN,YMAXSCREEN);

  target[0]=0.5;
  target[1]=0.5;

  /* endless learning loop */
  for(;;) {
    if(kbhit()) {
      _setvideomode(_DEFAULTMODE);
      exit(0);
    }

    /* select input numbers */
    xin = target[0];
    yin = target[1];

    /* computer the target vector using the model */
    target[0] = yin + 1.0 - 1.4 * xin * xin;
    target[1] = 0.3 * xin;
    
    /* select input vector */
    invector[0] = xin;
    invector[1] = yin;
    invector[2] = 1.0;

    /* feed forward -- input to hidden */
    for(j=0;j<HUNITS;j++) {
      netih[j] = 0;
      for(i=0;i<IUNITS;i++) {
	netih[j] = netih[j] + itohweight[i][j] * invector[i];
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
	delta = error[k]*hout[j]*eta;
	htooweight[j][k] = htooweight[j][k] + delta;
      }
    }

    /* compute the error correction for itohwetht matrix elements */
    for(j=0;j<HUNITS;j++) {
      /* first find sigma(e(k)u(kj)) */
      sigma[j] = 0;
      for(k=0;k<OUNITS;k++) {
	sigma[j]=sigma[j]+error[k]*htooweight[j][k];
      }
      temp = (double)(1.0/cosh(netih[k]));
      fprime[j] = temp*temp;
    }

    for(i=0;i<IUNITS;i++) {
      for(j=0;j<HUNITS;j++) {
	delta = eta*fprime[j]*sigma[j]*invector[i];
	itohweight[i][j] = itohweight[i][j] + delta;
      }
    }

    d1 = pow((oout[0] - target[0]), 2.0);
    d2 = pow((oout[1] - target[1]), 2.0);
    d = d + (sqrt(d1 + d2))/25.0;

    iterate = iterate + 1;
    counter = counter + 1;
    if(counter == 24) {
      xcoord = (((double)(iterate) - XMIN)/(XMAX-XMIN))*XMAXSCREEN;
      ycoord = YMAXSCREEN - ((d-YMIN)/(YMAX-YMIN))*YMAXSCREEN;
      _setpixel(xcoord,ycoord);

      d = 0;
      counter = 0;
    }
  }
}

double rnd() {
  double result;
  result = (double)(rand())/(double)(32767.0);
  return(result);
}





