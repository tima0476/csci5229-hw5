#ifndef __color_h__
#define __color_h__

//
// Utility functions for colorspace conversions.  Copied from in-class 
// example 12 (ex12.c)
//

void CMYK2RGB(double C, double M, double Y, double K,double* R, double* G, double* B);
void RGB2CMYK(double R,double G,double B,double* C,double* M,double* Y,double* K);
void RGB2HSV(double R,double G,double B,double* H,double* S,double* V);
void HSV2RGB(double H, double S, double V, double* R, double* G, double* B);

#endif // __color_h__