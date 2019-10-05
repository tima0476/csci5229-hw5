//  colors.c
//
//  This is a utility module to provide colorspace translation functions.  Functions
//  blatantly copied from in-class example 12 (ex12.c)
//

// Three componenent minimum and maximum - used locally not externally, so define here
// and not in colors.h
#define MIN(a,b,c) ((a)<(b) &&  (a)<(c) ? (a) : ((b) < (c) ? (b) : (c)))
#define MAX(a,b,c) ((a)>(b) &&  (a)>(c) ? (a) : ((b) > (c) ? (b) : (c)))

/*
 *  Convert CMYK to RGB
 */
void CMYK2RGB(double C, double M, double Y, double K,double* R, double* G, double* B)
{
   *R = (1-K)*(1-C);
   *G = (1-K)*(1-M);
   *B = (1-K)*(1-Y);
}

/*
 *  Convert RGB to CMYK
 */
void RGB2CMYK(double R,double G,double B,double* C,double* M,double* Y,double* K)
{
   double max = MAX(R,G,B);
   *K = 1-max;
   if (max==0)
      *C = *M = *Y = 0;
   else
   {
      *C = (max-R)/max;
      *M = (max-G)/max;
      *Y = (max-B)/max;
   }
}

/*
 *  Convert RGB to HSV
 */
void RGB2HSV(double R,double G,double B,double* H,double* S,double* V)
{
   double min = MIN(R,G,B);
   double max = MAX(R,G,B);
   double del = max - min;

   *V = max;
   if (max==0 || del==0)
      *S = *H = 0;
   else
   {
      *S = del/max;
      if (R==max)
         *H = (0 + (G-B)/del)/6;  // between yellow & magenta
      else if (G==max)
         *H = (2 + (B-R)/del)/6;  // between cyan & yellow
      else
         *H = (4 + (R-G)/del)/6;  // between magenta & cyan
      if (*H<0) *H += 1;
   }
}

/*
 *  Convert HSV to RGB
 */
void HSV2RGB(double H, double S, double V, double* R, double* G, double* B)
{
   double h6 = H>=1 ? 0 : 6*H;
   int    i6 = h6;
   double f6 = h6-i6;
   double f1 = V*(1-S);
   double f2 = V*(1-S*f6);
   double f3 = V*(1-S*(1-f6));

   switch (i6)
   {
      case 0:  *R = V;  *G = f3; *B = f1; break;
      case 1:  *R = f2; *G = V;  *B = f1; break;
      case 2:  *R = f1; *G = V;  *B = f3; break;
      case 3:  *R = f1; *G = f2; *B = V;  break;
      case 4:  *R = f3; *G = f1; *B = V;  break;
      default: *R = V;  *G = f1; *B = f2; break;
   }
}
