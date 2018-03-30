//LIBRARIES INCLUDED
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>

//HEADER FILES INCLUDED
#include "display.h"
#include "main.h"
#include "balls.h"
#include "terrain.h"

int ballno;

float counter=0.0;

extern Ball b[COUNT];
extern Terrain t;

float x=3*width/8;
float y=3*height/8;
float z=3*length/8;


/*
 * \brief	Initialises the graphic properties
 * \param	Ka	-	Ambient material property
 * \param	Kd	-	Diffuse material property
 * \param	Ks	-	Specular material property
 */

void init_material(float Ka,float Kd, float Ks, float Kp, float Mr, float Mg, float Mb)
{
   // Material variables
 float ambient[] = { Ka * Mr, Ka * Mg, Ka * Mb, 1.0 };
 float diffuse[] = { Kd * Mr, Kd * Mg, Kd * Mb, 1.0 };
 float specular[] = { Ks * Mr, Ks * Mg, Ks * Mb, 1.0 };

   // Initialize material
 glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
 glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
 glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Kp);
}

/*
 * \brief	Initialize light source
 */

void init_light(int light_source, float Lx, float Ly, float Lz, float Lr, float Lg, float Lb)
{
   // Light variables
 float light_position[] = { Lx, Ly, Lz, 0.0 };
 float light_color[] = { Lr, Lg, Lb, 1.0 };

   // Initialize light source
 glEnable(GL_LIGHTING);
 glEnable(light_source);
 glLightfv(light_source, GL_POSITION, light_position);
 glLightfv(light_source, GL_AMBIENT, light_color);
 glLightfv(light_source, GL_DIFFUSE, light_color);
 glLightfv(light_source, GL_SPECULAR, light_color);
 glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
 glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
 glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
 glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
 glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

/*
 * \brief	initialises the screen
 */

void init()
{
  // Initialize OpenGL
  glClearColor(0.0, 0.0, 0.0, 1.0);//Background Color
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
  glShadeModel(GL_SMOOTH);   // Enable smooth shading
  init_light(GL_LIGHT0, 0, 1, 1, 1, 1, 1);
  init_light(GL_LIGHT1, 1, 0, 1, 1, 1, 1); 
}


void DrawCube()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f,0.0f,-7.0f);

  // glRotatef(counter,1.0,1.0,0.0); //step to rotate the cube
  // counter+=0.9;

//BACK  
  glBegin(GL_POLYGON);  
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.2,0.6,1);
  glVertex3f(  x, -y, -z);
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.2,0.6,1);
  glVertex3f(  x,  y, -z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.2,0.6,1);
  glVertex3f( -x,  y, -z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.2,0.6,1);
  glVertex3f( -x, -y, -z );
  glEnd();

//RIGHT  
  glBegin(GL_POLYGON);
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.11,0.56,0.99);
  glVertex3f( x, -y, -z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.11,0.56,0.99);
  glVertex3f( x,  y, -z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.39,0.58,0.92);
  glVertex3f( x,  y,  z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.39,0.58,0.92);
  glVertex3f( x, -y,  z );
  glEnd();
  
//LEFT
  glBegin(GL_POLYGON);
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.39,0.58,0.92);
  glVertex3f( -x, -y,  z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.39,0.58,0.92);
  glVertex3f( -x,  y,  z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.11,0.56,0.99);
  glVertex3f( -x,  y, -z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.11,0.56,0.99);
  glVertex3f( -x, -y, -z );
  glEnd();  
  
//TOP
  glBegin(GL_POLYGON);
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.30,0.50,0.84);
  glVertex3f(  x,  y,  z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.05,0.50,0.90);
  glVertex3f(  x,  y, -z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.05,0.50,0.90);
  glVertex3f( -x,  y, -z );
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.30,0.50,0.84);
  glVertex3f( -x,  y,  z );
  glEnd();
  
//BOTTOM
  glBegin(GL_POLYGON);
  init_material(0.2, 0.4, 0.4, 100 * (0.5),0.27,0.50,0.70);
  glVertex3f(  x, -y, -z );
  glVertex3f(  x, -y,  z );
  glVertex3f( -x, -y,  z );
  glVertex3f( -x, -y, -z );
  glEnd();  
}


/*
 * \brief	The Glut Dispaly Function
 */

void display()
{   
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  DrawCube();//Draw Cube
  t.drawTerrain();//Draw Terrain
  for (int i = 0; i < COUNT; i++)
  {
    init_material(0.2, 0.4, 0.4, 100 * (0.5), (float)b[i].r2/255, (float)b[i].g2/255, (float)b[i].b2/255);
    b[i].draw_ball();//Draw each ball
  }	
  glutSwapBuffers();
}

void reshape(int x, int y)
{
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    glViewport(0,0,x,y);  //Use the whole window for rendering
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity(); 
    gluPerspective(35.0f,(GLdouble)x/(GLdouble)y,0.1f,10.0f);    
}