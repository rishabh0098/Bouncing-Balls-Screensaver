//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>

class Ball
{
	public:
	//The ball parameters
	int id;
	float px, py, pz;
	float vx, vy, vz;
	int red, green, blue;	//original color of balls
	float r2,g2,b2;	//changed colors on collision
	float Radius;
	
	//Ball functions
	Ball();	//Constructor
	void renderBitmapString(float x, float y, void *font,const char *string);//function to display ball id on the ball
	void draw_ball();// function to draw ball

};
