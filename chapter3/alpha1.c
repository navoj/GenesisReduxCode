#include "\quickc\include\dos.h"
#include "\quickc\include\float.h"
#include "\quickc\include\stdio.h"
#include "\quickc\include\graph.h"
#include "\quickc\include\math.h"

struct videoconfig vc;
char error_message[] = "this video mode is not supported";

void main(void) {
  /* declarations */
  int newcoord();
  int point();
  int sgn();

  int nx,ny;
  int x,y,newx,newy;
  int vx,vy;
  long int contact = 0, move = 0;
  long int delay;

  long int newselected = 0;

  /* set mode of screen */
  if (_setvideomode(_MRES4COLOR) == 0) {
    printf("%s\n",error_message);
    exit(0);
  }
  
