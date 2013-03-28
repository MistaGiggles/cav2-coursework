#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#define GLUT_KEY_ESCAPE 27
#ifndef GLUT_WHEEL_UP
    #define GLUT_WHEEL_UP 3
    #define GLUT_WHEEL_DOWN 4
#endif

#include "Vector.h"
#include "Matrix.h"
#include "Volume.h"

#define WIDTH 256
#define HEIGHT 256

unsigned int modThresh = 128;
enum modes {demo, ray2d};
int mode = demo;

enum color {red, green, blue, all};
int colormode = all;

enum perspective2d {front, side, top};
int perp2d = front;


static Volume* head = NULL;

void Update() {
	glutPostRedisplay();
}

void Draw() {
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);
	
  
  

    	switch(mode) {
    		case demo:
    			for(int y = 0; y < head->GetHeight(); y++)
  				for(int z = 0; z < head->GetDepth(); z++) {

				for (int x = 0; x < head->GetWidth(); x++) {
					
	        		unsigned char val = head->Get(x, y, z);
	        
			        // Change this value to see different contours
			        unsigned int threshold = modThresh;
					if (val > threshold) {

						Vector3 color = Vector3(val, val, val) / 255.0;
						glColor3f(color.r(), color.g(), color.b());

						switch(perp2d)
							{
								case front:
									glVertex3f(y, z, 0);
								break;

								case top:
									glVertex3f(x, z, 0);
								break;

								case side:
									glVertex3f(x, y, 0);
								break;
							}
						break;
					}
				}
				}
			break;

			case ray2d:
				
				for(int y = 0; y < head->GetHeight(); y++)
	  				for(int z = 0; z < head->GetDepth(); z++) {
		  				// Starting at front
		  				float r,g,b;
		  				float dist = 1;
		  				r = g = b = 0;
		  				bool draw = true;

						for (int x = 0; x < head->GetWidth(); x++) {
							
			        		unsigned char val = head->Get(x, y, z);
			        		float alpha = (float)val/255.0f;
			        		// r is skin 55 - 70
			        		// b is dense skull 160

			        		// New alpha = Current Alpha + (1-Current Alpha) * Accumulated Alpha
					        if(alpha > 0.1 && alpha < 0.3)
					        {
					        	r = r + (1-r)*alpha;
					        }
					      		
					        if(alpha > 0.3 && alpha < 0.6)
					        {
					        	g = g + (1-g)*alpha;
					        }

					        if(alpha > 0.61 )
					        {

					        	b = b + (1-b)*alpha;
					        }
					        
					        
					        dist = (float)x/(float)head->GetWidth();
					        if(r>0.9||g>1||b>1) break;


								
							


						}
						// RAY TRACED
						//float dist = (float)x/(float)head->GetWidth();
						switch(colormode) {
							case red:
								glColor3f(r,0,0);
							break;

							case green:
								glColor3f(0,g,0);
							break;

							case blue:
								glColor3f(0,0,b);
							break;

							case all:
								glColor3f(r,0,b);
							break;
						}
						
						glVertex3f(y,z,0);
					} 
				
			break;
    	
  }
  
	glEnd();
	
	glFlush();
	glutSwapBuffers();
  
}

void KeyEvent(unsigned char key, int x, int y) {

	switch(key) {
    case GLUT_KEY_ESCAPE:
      exit(EXIT_SUCCESS);
      break;
    
    case GLUT_KEY_END:
    	break;
	}
	
}

void SpecKeyEvent(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_F1:
			mode = demo;
		break;
		case GLUT_KEY_F2:
			mode = ray2d;
		break;


		case GLUT_KEY_F9:
			colormode = all;
		break;
		case GLUT_KEY_F10:
			colormode = red;
		break;
		case GLUT_KEY_F11:
			colormode = green;
		break;
		case GLUT_KEY_F12:
			colormode = blue;
		break;
		case GLUT_KEY_LEFT:
			if(perp2d==front) {
				perp2d = side;
				break;
			} else if(perp2d == side) {
				perp2d = top;
				break;
			} else if (perp2d == top) {
				perp2d = front;
				break;
			}
		case GLUT_KEY_UP:
	    	modThresh += 1;
	    	std::cout<<"Current Threshhold = "<<modThresh<<std::endl;
    	break;
    	case GLUT_KEY_DOWN:
	    	modThresh -= 1;
	    	std::cout<<"Current Threshhold = "<<modThresh<<std::endl;
    	break;
	}
}

int main(int argc, char **argv) {

	head = new Volume("head");
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("cav2");

	glClearColor(0.5, 0.5, 0.5, 1.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -512, 512);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	
	glutKeyboardFunc(KeyEvent);
	glutSpecialFunc(SpecKeyEvent);
	glutDisplayFunc(Draw);
	glutIdleFunc(Update);
	
	glutMainLoop();
	
	delete head;
};
