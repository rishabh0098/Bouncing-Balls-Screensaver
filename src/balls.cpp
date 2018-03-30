//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <unistd.h>
#include <string.h>
#include <sstream>
using namespace std;

//HEADER FILES INCLUDED
#include "balls.h"
#include "main.h"
#include "display.h"

/*
 * \brief	The Ball Class Constructor Definition, initialises the variables and ball surface	
 */
Ball::Ball()
{
	//Initialize the ball variables randomly
	px=rand() *3.0* ((float)width) /4* RAND_MAX + ((float)(-3)*width/8);
	py=rand() *3.0* ((float)height-0.3) /4* RAND_MAX + ((float)(-3)*(height-0.3)/8);
	pz=rand() *3.0* ((float)length) /4* RAND_MAX + ((float)(-3)*length/8);
	vx=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
	vy=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
	vz=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
	red=rand() * ((float)255) / RAND_MAX ;
	green=rand() * ((float)255) / RAND_MAX;
	blue=rand() * ((float)255) / RAND_MAX;
	r2=red;
	g2=green;
	b2=blue;
	Radius=0.18;
}
/*
* \brief Display Ball number on the ball
*/
void Ball::renderBitmapString(float x, float y, void *font,const char *string)
{
	const char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(font, *c);
	}
}
/*
 * \brief	Draws the 3D ball
 */
void Ball::draw_ball()
{
	stringstream strs;
	strs << id;
	string temp_str = strs.str();
	char* const i = (char*) temp_str.c_str();
	glLoadIdentity();
    // Draw the surface
	glPushMatrix();
	glEnable(GL_DEPTH_TEST); 
	glTranslatef(px,py, pz-7.0);
	glutSolidSphere( Radius, 50.0, 50.0);
	glTranslatef(0,0, Radius);
	renderBitmapString(0,0,GLUT_BITMAP_9_BY_15,i);
	glPopMatrix();
}

