/*
 * Timothy Mason, homework 5: Lighting
 * CSCI 5229  Computer Graphics - University of Colorado Boulder
 *
 * ball.c
 *
 * Module for drawing a sphere.  This is from in-class example 13 (ex13.c).  It
 * has been moved to a separate module for improved clarity.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "CSCIx229.h"
#include "ball.h"
#include "color.h"

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 *     emission (e)
 *     shininess (s)
 *     increment (inc)
 */
void ball(double x,double y,double z,double r, int e, int s, int inc)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*e,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   // glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,s);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUADS);  // Use quads instead of quadstrip so each quad can be drawn counterclockwise to allow use of face culling.
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
         Vertex(th+2*inc,ph+inc);
         Vertex(th+2*inc,ph);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
}
