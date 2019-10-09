/*
 *  Lighting
 *
 *  Expands on the Rockets + Planet scene of HW3 by adding basic lighting
 *  This code is based heavily on example 13 (ex13.zip) provided in class
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC/q/Q    Exit
 */
#include "CSCIx229.h"
#include "color.h"
#include "rocket.h"
#include "ball.h"

#define MIN(a,b)  ((a) < (b) ? (a) : (b))
#define MAX(a,b)  ((a) > (b) ? (a) : (b))

int axes=1;             //  Display axes
int mode=1;             //  Projection mode
int move=1;             //  Move light
int th=25;              //  Azimuth of view angle
int ph=25;              //  Elevation of view angle
int fov=55;             //  Field of view (for perspective)
int light=1;            //  Lighting
double asp=1;           //  Aspect ratio
double dim=3.0;         //  Size of world
int inc=10;             //  Ball drawing increment (in degrees)

// Light values
int one       =   1;    // Unit value
int distance  =   5;    // Light distance
int smooth    =   1;    // Smooth/Flat shading
int local     =   0;    // Local Viewer Model
int emission  =   0;    // Emission intensity (%)
int ambient   =  30;    // Ambient intensity (%)
int diffuse   = 100;    // Diffuse intensity (%)
int specular  =   0;    // Specular intensity (%)
int shininess =   0;    // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;    // Light azimuth
float ylight  =   0;    // Elevation of light

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      
      //  Draw light position as a yellow ball (still no lighting here)
      glColor3f(1,1,0);
      ball(Position[0],Position[1],Position[2] , 0.1, emission, shininess, inc);
      
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      
      //  Enable lighting
      glEnable(GL_LIGHTING);
      
      //  Location of viewer for specular calculationsÏ
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      
      // Enable two sided mode (needed for the rocket fins)
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      
      //  Enable light 0
      glEnable(GL_LIGHT0);
      
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0, GL_AMBIENT,  Ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  Diffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
      glLightfv(GL_LIGHT0, GL_POSITION, Position);
   }
   else
      glDisable(GL_LIGHTING);

   //  Draw scene
   glColor3f(0,.25,1);
   ball(0,0,0, 0.3, emission, shininess, inc);

   // Draw some rockets.   (ref: http://colorizer.org/ for a good interactive color chooser)
   rocket(   1,   1,   0,  1, 1, 0,  30, 1.0/70.0, 120.0/360.0, shininess, emission, 3, inc);    // Green, 3 fins
   rocket(  -1,   0,   0,  1, 0, 1,  85, 1.0/60.0,  90.0/360.0, shininess, emission, 4, inc);    // Cyan, 4 fins
   rocket(   0, 0.5, 1.5,  0, 1, 1, 161, 1.0/80.0, 300.0/360.0, shininess, emission, 5, inc);    // Magenta, 5 fins
   rocket(   0,-0.5,  -1,  0, 1, 0,  35, 1.0/90.0,  45.0/360.0, shininess, emission, 6, inc);    // Purple, 6 fins
   rocket( 1.1, 1.1, 1.1,  0, 0, 0,   0, 1.0/80.0, 240.0/360.0, shininess, emission, 7, inc);    // Blue, 7 fins

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0, 0.0, 0.0);
      glVertex3d(len, 0.0, 0.0);

      glVertex3d(0.0, 0.0, 0.0);
      glVertex3d(0.0, len, 0.0);
      
      glVertex3d(0.0, 0.0, 0.0);
      glVertex3d(0.0, 0.0, len);
      glEnd();
      
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   switch (key)
   {
      case GLUT_KEY_RIGHT:             //  Right arrow key - increase angle by 5 degrees
         th += 5;
         break;

      case GLUT_KEY_LEFT:              //  Left arrow key - decrease angle by 5 degrees
         th -= 5;
         break;

      case GLUT_KEY_UP:                //  Up arrow key - increase elevation by 5 degrees
         ph += 5;
         break;

      case GLUT_KEY_DOWN:              //  Down arrow key - decrease elevation by 5 degrees
         ph -= 5;
         break;

      case GLUT_KEY_PAGE_DOWN:         //  PageUp key - increase dim
         dim += 0.1;
         break;

      case GLUT_KEY_PAGE_UP:           //  PageDown key - decrease dim (but not less than 1)
         dim -= 0.1;
         dim = MAX(dim, 1);
         break;

      case GLUT_KEY_F1:                //  Smooth color model
         smooth = 1-smooth;
         break;

      case GLUT_KEY_F2:                //  Local Viewer
         local = 1-local;
         break;

      case GLUT_KEY_F3:                //  Toggle light distance
         distance = (distance==1) ? 5 : 1;
         break;

      case GLUT_KEY_F8:                //  Toggle ball increment
         inc = (inc==10)?3:10;
         break;

      case GLUT_KEY_F9:                //  Flip sign
         one = -one;
         break;
   }

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   switch(ch) {   
      case 27:       // Exit on ESC or q
      case 'q':
      case 'Q':
         exit(0);
         break;

      case '0':      //  Reset view angle
         th = ph = 0;
         break;

      case 'x':      // Toggle axes display
      case 'X':
         axes = 1-axes;
         break;

      case 'l':      //  Toggle lighting
      case 'L':
         light = 1-light;
         break;

      case 'p':      //  Switch projection mode
      case 'P':
         mode = 1-mode;
         break;

      case 'm':      //  Toggle light movement
      case 'M':
         move = 1-move;
         break;

      case '<':      // Move light clockwise about y axis
         zh += 1;
         break;

      case '>':      // Move light counter-clockwise about y axis
         zh -= 1;
         break;

      case '-':       // Decrease field of view angle
         fov--;
         break;

      case '+':      // Increase field of view angle
         fov++;
         break;

      case '[':      // Decrease light elevation (down y axis)
         ylight -= 0.1;
         break;

      case ']':      // Increase light elevation (up y axis)
         ylight += 0.1;
         break;

      case 'a':      // Decrease ambient level, but not less than 0
         ambient -= 5;
         ambient = MAX(ambient,0);
         break;

      case 'A':      // Increase ambient level, but not more than 100
         ambient += 5;
         ambient = MIN(ambient,100);
         break;

      case 'd':      // Decrease diffuse level, but not less than 0
         diffuse -= 5;
         diffuse = MAX(diffuse,0);
         break;

      case 'D':      // Increase diffuse level, but not more than 100
         diffuse += 5;
         diffuse = MIN(diffuse,100);
         break;

      case 's':      // Decrease specular level, but not less than 0
         specular -= 5;
         specular = MAX(specular,0);
         break;

      case 'S':      // Increase specular level, but not more than 100
         specular += 5;
         specular = MIN(specular,100);
         break;

      case 'e':      // Decrease emission level, but not less than 0
         emission -= 5;
         emission = MAX(emission,0);
         break;

      case 'E':      // Increase emission level, but not more than 100
         emission += 5;
         emission = MIN(emission,100);
         break;

      case 'n':      // Decrease shininess level, but not less than 0
         shininess -= 1;
         shininess = MAX(shininess,0);
         break;

      case 'N':      // Increase shininess level, but not more than 7
         shininess += 1;
         shininess = MIN(shininess,7);
         break;
   }

   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   
   //  Reproject
   Project(mode?fov:0,asp,dim);
   
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1024,1024);
   glutCreateWindow("Timothy Mason");
   
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   
   return 0;
}
