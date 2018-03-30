//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <GL/glut.h>
#include <unistd.h>
#include<ctime>
#include <sstream>
using namespace std;

//HEADER FILES INCLUDED
#include "main.h"
#include "physics.h"
#include "balls.h"
#include "display.h"
#include "terrain.h"

float width=4.0;
float height=4.0;
float length=4.0;
float speed=0.0008;

int selected_ball=0,selected_ball_id=-1;

//Objects of class Ball 
Ball b[COUNT];
//Object of class Terrain 
Terrain t;

string buffer="";


//pause/play flag
bool play=true;

//speed increase/decrease flags
int acc[COUNT]={};
int dcc[COUNT]={};

//Counter value to move the camera
float slide=0.0;

float xc=3*width/8;
float yc=3*height/8;

sem_t mutex_rdcnt, mutex_wrcnt, mutex_3, w, r;

/*
 * \brief	This is the Glut Idle Function 
 */

void idle()
{
	glutPostRedisplay();
}

/*
 * \brief	The thread call function, controls the movement of the balls
 * \param	j	-	thread id, used as ball id
 */

void *myThreadFun(void *j)
{
	int i= (long int) j;
	
	//static 	Array of ball data shared among the threads
	static float px[COUNT];   
	static float py[COUNT];  
	static float pz[COUNT];
	static float vx[COUNT];  
	static float vy[COUNT];  
	static float vz[COUNT];
	static float rad[COUNT];
	
	px[i]=b[i].px; 
	py[i]=b[i].py; 
	pz[i]=b[i].pz;
	vx[i]=b[i].vx; 
	vy[i]=b[i].vy;
	vz[i]=b[i].vz;

	static int readcount=0;
	static int writecount=0;

	while (true)
	{
		if(!play)
		{ 
			//Pause the threads
			continue;
		}
		else 
		{
			float sp=sqrt(vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i]);//Speed calculated
			
			if(dcc[i]&&sp>=0.00016) //minimum speed assumed
			{	
				float delta=0.8;
				dcc[i]=0;
				vx[i]*=delta;
				vy[i]*=delta;
				vz[i]*=delta;
			}
			
			if(acc[i]&&sp<=0.016) //maximum speed assumed
			{
				float delta = 1.25;
				acc[i]=0;
				vx[i]*=delta;
				vy[i]*=delta;
				vz[i]*=delta;
			}
			
			//Update postions
			px[i] += vx[i];
			py[i] += vy[i];
			pz[i] += vz[i];
			b[i].py=py[i]; 
			b[i].px=px[i];
			b[i].pz=pz[i];
			
			
			usleep(1000);
			
			
			//check for collision with walls
			float *arr= wall(px[i],py[i],pz[i],vx[i],vy[i],vz[i],b[i].Radius);
			px[i]=arr[0];
			py[i]=arr[1];
			pz[i]=arr[2];
			vx[i]=arr[3];
			vy[i]=arr[4];
			vz[i]=arr[5];

			//Checks collision with terrain objects
			for(int l=0;l<50;l++)
			{
				if(t.cx[l]+slide<=xc && t.cx[l]+slide>=-xc)
				{
					if(overlap(b[i].px,b[i].py,b[i].pz,t.cx[l]+slide,-yc,t.cz[l],b[i].Radius,0.3))
					{
						b[i].px=-0.1*vx[i];
						b[i].py=-0.1*vy[i];
						b[i].pz=-0.1*vz[i];
						b[i].vx=-vx[i];
						b[i].vy=-vy[i];
						b[i].vz=-vz[i];
						vx[i]=-vx[i];
						vy[i]=-vy[i];
						vz[i]=-vz[i];
					}
				}
			}

			if(i<COUNT-1)
			{               
				for(int k=i+1;k<COUNT;k++)
				{       
					sem_wait(&mutex_3);
					sem_wait(&r);
					sem_wait(&mutex_rdcnt);
					readcount++;
					if (readcount == 1)
						sem_wait(&w);
					
					sem_post(&mutex_rdcnt);
					sem_post(&r);
					sem_post(&mutex_3);
					
					// Reading started
					float pxk=px[k]; float pyk=py[k];float pzk=pz[k]; float vxk=vx[k]; float vyk=vy[k]; float vzk=vz[k];
					float pxi=px[i]; float pyi=py[i]; float pzi=pz[i]; float vxi=vx[i]; float vyi=vy[i]; float vzi=vz[i];
					float ri=b[i].Radius,rk=b[k].Radius;
					// Reading completed
					
					sem_wait(&mutex_rdcnt);
					readcount--;
					if (readcount == 0) 
						sem_post(&w);
					sem_post(&mutex_rdcnt);

					//detect overlapping of two balls i adn k
					if (overlap(pxi,pyi,pzi,pxk,pyk,pzk,ri,rk))
					{
						float step = 0.1;
						
						while (overlap(pxi,pyi,pzi,pxk,pyk,pzk,ri,rk))
						{
							pxi-=step*vxi;
							pyi-=step*vyi;
							pzi-=step*vzi; 
							pxk-=step*vxk;
							pyk-=step*vyk;
							pzk-=step*vzk; 
						}
						float vel[6];
						float *temp= velocities(pxi,pyi,pzi,vxi,vyi,vzi,pxk,pyk,pzk,vxk,vyk,vzk);
						vel[0]=*temp++;
						vel[1]=*temp++;
						vel[2]=*temp++;
						vel[3]=*temp++;
						vel[4]=*temp++;
						vel[5]=*temp; 
						
						sem_wait(&mutex_wrcnt);
						writecount++;
						if (writecount == 1) 
							sem_wait(&r);
						sem_post(&mutex_wrcnt); 
						sem_wait(&w);
						
						px[i]=pxi; 
						py[i]=pyi; 
						pz[i]=pzi; 
						px[k]=pxk; 
						py[k]=pyk;  
						pz[k]=pzk;
						vx[i]=vel[0]; 
						vy[i]=vel[1];
						vz[i]=vel[2];
						vx[k]=vel[3]; 
						vy[k]=vel[4];
						vz[k]=vel[5]; 
						
						sem_post(&w); 
						sem_wait(&mutex_wrcnt);
						writecount--;
						if (writecount == 0) 
							sem_post(&r);
						sem_post(&mutex_wrcnt);
						
						b[i].vx=vx[i];
						b[i].vy=vy[i];
						b[i].vz=vz[i]; 					 
					}
				}
			}   
		}
	}
}
// Function handling keyboard input
void keyboard (unsigned char key, int x, int y) 
{
	if (key==' ') 
	{
		if(play)
		{
			play=false;
		}
		else
		{
			play=true;
		}
	}

	if (isdigit(key))
	{
		if(selected_ball==0)
		{
			buffer+=key;
		}
	}
	else if(key == '\r' && selected_ball==0)
	{
		std::stringstream str(buffer);
		int val;
		str >> val;
		if(val<=COUNT)
		{
			selected_ball_id=val;
			selected_ball=1;
			b[selected_ball_id].r2=255;
			b[selected_ball_id].g2=255;
			b[selected_ball_id].b2=255;
		}
		else
		{
			buffer="";
		}
	}
	else if(key == '\r' && selected_ball==1)
	{
		b[selected_ball_id].r2=b[selected_ball_id].red;
		b[selected_ball_id].g2=b[selected_ball_id].green;
		b[selected_ball_id].b2=b[selected_ball_id].blue;
		selected_ball=0;
		selected_ball_id=-1;
		buffer="";
	}

	if(selected_ball==1)
	{

		float vx,vy,vz,sp;
		vx=b[selected_ball_id].vx;
		vy=b[selected_ball_id].vy;
		vz=b[selected_ball_id].vz;
		sp=sqrt(vx*vx+vy*vy+vz*vz);

		if (key=='[') 
		{
			dcc[selected_ball_id]=1;	
		}
		
		if (key==']') 
		{
			acc[selected_ball_id]=1;	
		}

		if (key=='+')
		{
			if(b[selected_ball_id].Radius<=0.380)
				b[selected_ball_id].Radius+=0.005;
		}

		if (key=='-')
		{
			if(b[selected_ball_id].Radius>=0.040)
				b[selected_ball_id].Radius-=0.005;
		}
	}    
}

//function for handling specila keys of keyboard
void SpecialInput(int key, int x, int y)
{
	if(key==GLUT_KEY_LEFT && slide>-11.5)
	{
		slide-=0.01;		
	}
	if(key==GLUT_KEY_RIGHT && slide<11.5)
	{
		slide+=0.01;		
	}
}


/*
 * \brief	The Main Function
 */

int main(int argc, char *argv[])
{
	sem_init(&mutex_rdcnt, 0, 1);
	sem_init(&mutex_wrcnt, 0, 1);
	sem_init(&mutex_3, 0, 1);
	sem_init(&w, 0, 1);
	sem_init(&r, 0, 1);
	pthread_t tid;
	
	glutInit (&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(250, 250);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("ScreenSaver");
	
	init();
	
	for(long int j=0;j<COUNT;j++) //Create threads
	{
		b[j].id=j;
		pthread_create(&tid, NULL, myThreadFun, (void *)j);
	}
	
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialInput);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
