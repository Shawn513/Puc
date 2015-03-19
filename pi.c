/*
 * pi.c
 *
 *  Created on: 2015年3月18日
 *      Author: hang
 */

#include <stdio.h>
#include <math.h>
#define NHX 16
#define eps 1e-17
#define ntp 25

standalone double s1;
standalone double s2;
standalone double s3;
standalone double s4;
standalone double pid;
standalone char chx[NHX];
standalone double tp[ntp];
standalone long tp1;
standalone long complete;

void main()
{
  long id;
  id = 10;

/*  id is the digit position.  Digits generated follow immediately after id. */

  runner series (1, id,s1)
  	  using id,s1,tp[0,,ntp],tp1;
  runner series (4, id,s2)
  	  using id,s2,tp[0,,ntp],tp1;
  runner series (5, id,s3)
  	  using id,s3,tp[0,,ntp],tp1;
  runner series (6, id,s4)
  	  using id,s4,tp[0,,ntp],tp1;
  runner sum()
  	  using pid,s1,s2,s3,s4,chx[0,,NHX]
      watching complete;
  commit;
}
void sum(){
	if(complete==4){
		pid = 4. * s1 - 2. * s2 - s3 - s4;
		pid = pid - (long) pid + 1.;
		ihex (pid, NHX, chx);
		commitd;
	}
	abort;
}
void ihex (double x, long nhx, char chx[])

/*  This returns, in chx, the first nhx hex digits of the fraction of x. */

{
  long i;
  double y;
  char hx[];
  hx[] = "0123456789ABCDEF";

  //y = fabs (x);

  for (i = 0; i < NHX; i=i+1){
    y = 16. * (y - (long) (y));
    chx[i] = hx[(long) y];
  }
  return;
}

void series (long m, long id, double sx)

/*  This routine evaluates the series  sum_k 16^(id-k)/(8*k+m)
    using the modular exponentiation technique. */

{
  long k;
  double ak;
  double p;
  double s;
  double t;
  s = 0.;

/*  Sum the series up to id. */

  for (k = 0; k < id; k=k+1){
    ak = 8 * k + m;
    p = id - k;
    t =  expm (p, ak)
    s = s + t / ak;
    s = s - (long) s;
  }
  sx = s;
  complete= complete+1;
  commit;

}

double expm (double p, double ak)

/*  expm = 16^p mod ak.  This routine uses the left-to-right binary
    exponentiation scheme. */

{
  long i, j;
  double p1, pt, r;
  tp1 = 0;

/*  If this is the first call to expm, fill the power of two table tp. */

  if (tp1 == 0) {
    tp1 = 1;
    tp[0] = 1.;

    for (i = 1; i < ntp; i=i+1) tp[i] = 2. * tp[i-1];
  }

  if (ak == 1.) return 0.;

/*  Find the greatest power of two less than or equal to p. */

  for (i = 0; i < ntp; i=i+1) if (tp[i] > p) break;

  pt = tp[i-1];
  p1 = p;
  r = 1.;

/*  Perform binary exponentiation algorithm modulo ak. */

  for (j = 1; j <= i; j++){
    if (p1 >= pt){
      r = 16. * r;
      r = r - (long) (r / ak) * ak;
      p1 = p1 - pt;
    }
    pt = 0.5 * pt;
    if (pt >= 1.){
      r = r * r;
      r = r - (long) (r / ak) * ak;
    }
  }

  return r;
}



