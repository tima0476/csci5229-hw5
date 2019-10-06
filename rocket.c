/*
 * Timothy Mason, homework 5: Lighting
 * CSCI 5229  Computer Graphics - University of Colorado Boulder
 *
 * rocket.c
 *
 * Module for drawing a cartoon rocket.
 * ToDo:  Generalize this for drawing any arbitrary cylindrical surface
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
#include "rocket.h"
#include "color.h"

// The following shapes were manually traced and digitized on graph paper.
static duopoint rocket_profile[] = {
	{0,32},
	{1,31.4},
	{2,30.6},
	{3,29.5},
	{4,28.4},
	{4.9,27.3},
	{5.6,26.1},
	{6.2,25},
	{6.8,24},
	{7.1,23},
	{7.5,22},
	{7.8,21},
	{8.1,20},
	{8.4,19},
	{8.7,18},
	{8.8,16.9},
	{8.8,16},
	{8.9,15},
	{8.9,14},
	{8.9,13},
	{8.9,12},
	{8.9,11},
	{8.7,10},
	{8.5,9},
	{8.2,8},
	{8,7},
	{7.5,5.9},
	{7,5},
	{6.6,4},
	{6,3},
	{5.2,2},
	{4.8,1},
	{3.8,0}
};
#define ROCKET_POINT_COUNT	(sizeof(rocket_profile) / sizeof(rocket_profile[0]))

static duopoint rocket_fin[] = {
	{6,17.1},
	{7,17},
	{8.1,17},
	{9.2,16.8},
	{10,16.5},
	{10.9,16},
	{11.8,15},
	{12.1,14},
	{12.6,13},
	{12.9,12},
	{13.1,11},
	{13.4,10},
	{13.6,9},
	{13.8,8},
	{13.9,7},
	{14,6},
	{14.1,5},
	{14.2,4},
	{14.3,3},
	{14.3,2},
	{14.3,1},
	{14.3,0},

	{13.8,0},
	{13.6,1},
	{13.3,2},
	{13.1,3},
	{12.8,4},
	{12.7,5},
	{12.4,6},
	{12.1,7},
	{12,7.5},
	{11.8,8},
	{11.6,8.6},
	{11.3,9},
	{11.2,9.6},
	{10.9,10},
	{10.4,10.3},
	{9.9,10.7},
	{9.2,10.7},
	{8.8,10.8},
	{8,10.9},
	{7,11},
	{6.6,11},
	{6,11.2}
};
#define ROCKET_FIN_POINT_COUNT	(sizeof(rocket_fin) / sizeof(rocket_fin[0]))


/*
 * Draw vertex in cylindrical coordinates (r, theta, z)
 */
void cylVertex(double r, double th, double z)
{
   glVertex3d(r*Cos(th), r*Sin(th), z);
}

/*
 * Draw a radially symmetric solid
 *
 *    profile: pointer to an array of x,y coordinates representing the surface profile of the solid
 *    size: The number of points in the profile array
 *    bz,by,bz: 3D coordinates of the base of the solid
 *    rx,ry,rz: 3D vector for rotation of the solid.
 *    ph:  Angle to rotate the solid around (rx,ry,rz)
 *    s: the scale of the solid
 *    h: the base hue of the solid (value from 0 to 360) (ref: http://colorizer.org/ for a good interactive color chooser)
 */
void lathe(dpp profile, int size, double bx, double by, double bz, double rx, double ry, double rz, double ph, double s, double h)
{
   const int d=15;
   int th,i;
   double r, g, b;      // For receiving color values from color conversion

   HSV2RGB(h,1,1, &r, &g, &b);

   // Save transformation
   glPushMatrix();

   // Offset and scale
   glTranslated(bx,by,bz);
   glRotated(ph, rx, ry, rz);
   glScaled(s,s,s);

   glColor3f(r,g,b);

   // Latitude bands
   for (i=1; i<size; i++)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0; th<=360; th+=d)
      {
         cylVertex(profile[i-1].x, th, profile[i-1].y);
         cylVertex(profile[i].x, th, profile[i].y);
      }
      glEnd();
   }

   // Top cap; if necessary
   if (profile[0].x != 0.0)
   {
      glBegin(GL_TRIANGLE_FAN);
      glColor3f(0,0,1);    // Top cap is always blue
      glVertex3d(0, profile[0].y, 0);
      for (th = 0; th <= 360; th += d)
         cylVertex(profile[0].x, th, profile[0].y);
      glEnd();
   }

   // Bottom cap; if necessary
   if (profile[size-1].x != 0.0)
   {
      // Draw a triangle fan from the origin to the final circle.
      glBegin(GL_TRIANGLE_FAN);
      glColor3f(1, 1, 0);    // base cap is always orange
      glVertex3d(0, profile[size-1].y, 0);
      for (th = 0; th <= 360; th += d)
         cylVertex(profile[size-1].x, th, profile[size-1].y);
      glEnd();
   }

   // undo transformations
   glPopMatrix();
}

/*
 * Draw rocket fins equidistant around the rotation
 *
 *    bz,by,bz: 3D coordinates of the base of the rocket
 *    rx,ry,rz: 3D vector for rotation of the rocket.
 *    ph:  Angle to rotate the rocket
 *    s: the scale of the rocket
 *    fc: the number of fins on the rocket
 */
void draw_fins(double bx, double by, double bz, double rx, double ry, double rz, double ph, double s, int fc)
{
   int dth = 360/fc;
   int th,i;

   // Save transformation
   glPushMatrix();

   // Offset and scale
   glTranslated(bx,by,bz);
   glRotated(ph, rx, ry, rz);
   glScaled(s,s,s);

   // Draw rocket fins, spaced equally around the cylinder   
   for (th=0; th<=360; th += dth)
   {
      glBegin(GL_QUAD_STRIP); // The fin shape is non-convex, so can't use a simple polygon
      glColor3f(1,0,0);       // No choice; rocket fins are RED!

      // The rocket fin is non-convex, so it cannot be drawn as a single polygon.  This loop will draw it
      // instead as a strip of quads, making the assumption that the first point in the array is adjacent to
      // the last point, the 2nd point is adjacent to the 2nd-to-last point, and so on.
      for (i=0; i<(ROCKET_FIN_POINT_COUNT/2); i++)
      {
         cylVertex(rocket_fin[i].x, th, rocket_fin[i].y);
         cylVertex(rocket_fin[ROCKET_FIN_POINT_COUNT-i-1].x, th, rocket_fin[ROCKET_FIN_POINT_COUNT-i-1].y);
      }

      glEnd();
   }

   // undo transformations
   glPopMatrix();   
}

/*
 * Draw a cartoon rocket ship
 *
 *    bz,by,bz: 3D coordinates of the base of the rocket
 *    rx,ry,rz: 3D vector for rotation of the rocket.
 *    ph:  Angle to rotate the rocket
 *    s: the scale of the rocket
 *    h: the base hue of the rocket (value from 0 to 360) (ref: http://colorizer.org/ for a good interactive color chooser)
 *    fc: how many fins the rocket gets
 */
void rocket(double bx, double by, double bz, double rx, double ry, double rz, double ph, double s, double h, int fc)
{
   // Draw the main rocket cylinder
   lathe(rocket_profile, ROCKET_POINT_COUNT, bx, by, bz, rx, ry, rz, ph, s, h);

   // Now add some fins
   draw_fins(bx, by, bz, rx, ry, rz, ph, s, fc);
}
