//Header files included
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>
#include <ctime>
#include "balls.h"
#include "main.h"
#include "display.h"
#include "terrain.h"

float y2=3.0*height/8;

//Constructor for initilising terrain objects
Terrain :: Terrain()
{
    srand(time(NULL));
    for(int i=0;i<50;i++)
    {
        cx[i]=(float(rand()%100)/100.0)*2.4*10-1.2*10;
        cz[i]=(float(rand()%100)/100.0)*2.0-1.0;
        sel[i]=(rand()%2);
    }   
}
//Function to draw a cone
void Terrain :: drawCone(float xx, float zz)
{
    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glTranslatef(xx,-y2,zz-7.0);
    glRotatef(-90,1,0,0);
    glutSolidCone(0.3,0.3, 50.0, 50.0);
    init_material(0.2, 0.4, 0.4, 100 * (0.5),0.25,0.50,0.70);
    glPopMatrix();
}
// function to draw Sphere
void Terrain :: drawSphere(float xx, float zz)
{
    glLoadIdentity();
    double eqn[] = { 0.0,1.0, 0.0, 0.0};
    glEnable(GL_CLIP_PLANE0);   
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glTranslatef(xx,-y2,zz-7.0);
    glClipPlane(GL_CLIP_PLANE0, eqn);
    glutSolidSphere(0.3, 50.0, 50.0);
    init_material(0.2, 0.4, 0.4, 100 * (0.5),0.25,0.50,0.70);
    glPopMatrix();
    glDisable(GL_CLIP_PLANE0);
}

//Function responsible to draw complete terrain
void Terrain::drawTerrain()
{
    glLoadIdentity();
    double eqn[] = { 1.0,0.0, 0.0, 1.5};
    double eqn1[] = { -1.0,0.0, 0.0, 1.5};
    double eqn2[] = { 0.0,0.0, -1.0, -5.5};
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glClipPlane(GL_CLIP_PLANE1, eqn);
    glClipPlane(GL_CLIP_PLANE2, eqn1);
    glClipPlane(GL_CLIP_PLANE3, eqn2);
    for(int i=0;i<50;i++)
    {
        if(sel[i]==0)
            drawCone(cx[i]+slide,cz[i]);
        else
            drawSphere(cx[i]+slide,cz[i]);
    }
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glDisable(GL_CLIP_PLANE3);
}
