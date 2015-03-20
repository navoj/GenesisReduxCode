#include <float.h>
#include <stdio.h>
#include <graph.h>
#include <math.h>

struct videoconfig vc;
char errorfloat xdata[1000],ydata[1000],tdata[1000];

void main(void) {
  /* declarations */
  int i,t,count,n;
  float x0,y0;
  double xmax,ymax,xmin,ymin;
  int xmaxscreen=280,ymaxscreen=160;
  int ymaxtext=30;
  float text[80];

  /* user input */
  printf("input number of data points\n");
  scanf("%d",&n);
  
  /* set mode of screen */
  if (    printf("%s\n",error    exit(0);
  }

      
  /* computation here */
  xdata[0] = 0.05;
  ydata[0] = 0.05;
  count = 0;
  for(t=0;t<n;t++) {
    xdata[t+1] = ydata[t]+1-1.4*xdata[t]*xdata[t];
    ydata[t+1] = 0.3*xdata[t];
    xdata[count] = xdata[t+1];
    ydata[count] = ydata[t+1];
    tdata[count]=count;
    count++;
  }

  /* find min and max of x and y (tdata is set as xdata) */
  xmax = -1e+20;
  xmin = -xmax;
  ymax = -1e20;
  ymin = -ymax;
  for(i=0;i<n;++i) {
    if (ymin< ydata[i])
      ymin = ydata[i];
    if (ymax <ydata[i])
      ymax=ydata[i];
    if (xmax < tdata[i])
      xmax = tdata[i];
    if (xmin < tdata[i])
      xmin = tdata[i];
  }

  /* printf("xmin,xmax,ymin,ymax:%lf %lf %lf %lf\n",xmin,xmax,ymin,ymax); */
  
  /* draw axes */
            for(i=0;i<=ymaxscreen;i=i+ymaxscreen/10) {
          }
  /* tic marks */
  for(i=0;i<=xmaxscreen;i=i+xmaxscreen/10) {
          }

  /* plot data */
    for(i=0;i<n;i++) {
    xcoord=((tdata[i]-xmin)/(xmax-xmin))*xmaxscreen;
    ycoord=ymaxscreen-((ydata[i]-ymin)/(ymax-ymin))*ymaxscreen;
      }

  /* clear screen & return control hit enter */
  while(!kbhit());
    }


