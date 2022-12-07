/*
K-23 Flakey Roman

Fortune-Voronoi diagram
*/



#include <iostream>
#include<string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/glut.h>
#include "Voronoi.h"
#include "VPoint.h"


void display(void);
void onEF(int n);
void reshape(int width, int height);

vor::Voronoi * v;
vor::Vertices * ver; vor::Vertices * dir; vor::Edges * edg; 
double w = 10000;
const int POINTS_COUNT = 100;
const float SPEED_DIVIDER = 500; 

int main(int argc, char** argv)
{
	using namespace vor;

	v = new Voronoi();
	ver = new Vertices();
	dir = new Vertices();

	int mode;

	std::cout << "K-23 Flakey Roman\nFortune-Voronoi Diagram\n\nChoose method:\n	1) random point\n	2) own points" << std::endl;
	std::cin >> mode;
	

	if (mode == 1) {

		srand(time(NULL));

		for (int i = 0; i < POINTS_COUNT; i++)
		{
			ver->push_back(
				new VPoint(
					w * (double)rand() / (double)RAND_MAX,
					w * (double)rand() / (double)RAND_MAX
				)
			);

			dir->push_back(
				new VPoint(
					(double)rand() / (double)RAND_MAX - 0.5,
					(double)rand() / (double)RAND_MAX - 0.5
				)
			);
		}
	}
	else if (mode == 2) {
		std::string filename;
		std::cout << "Input name of your file with points:" << std::endl;
		std::cin >> filename;

		std::ifstream PointFile(filename);

		std::string s;
		while(std::getline(PointFile, s)) {
			std::istringstream sin(s);
			double x,y;
			sin >> x;
			sin >> y;

			ver->push_back(
				new VPoint(x,y)
			);

			dir->push_back(
				new VPoint(x-0.5,y-0.5)
			);
		
			}
		}
	
	glutInit(&argc, argv); 	glutInitDisplayMode (GLUT_SINGLE); 	glutInitWindowSize (800, 800); 	glutInitWindowPosition (100, 100); 	glutCreateWindow ("Lab 03"); 
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	
	glutTimerFunc(100, onEF, 0); 	glutDisplayFunc(display); 	glutReshapeFunc(reshape); 	glutMainLoop(); 	return 0;
}

void drawVoronoi()
{
	vor::Vertices::iterator j = dir->begin();
	for(vor::Vertices::iterator i = ver->begin(); i != ver->end(); ++i)
	{
		(*i)->x += (*j)->x * w/SPEED_DIVIDER;
		(*i)->y += (*j)->y * w/SPEED_DIVIDER;
		if( (*i)->x > w || (*i)->x < 0) (*j)->x *= -1;
		if( (*i)->y > w || (*i)->y < 0) (*j)->y *= -1;
		++j;
	}
	
	int c = 10;

	edg = v->GetEdges(ver, w, w);  
	for(vor::Vertices::iterator i = ver->begin(); i!= ver->end(); ++i)
	{
		glBegin(GL_QUADS);
		glColor3b(c*2, c*3, c*4);
		glVertex2f( -1+2*(*i)->x/w -0.01,  -1+2*(*i)->y/w - 0.00);
		glVertex2f( -1+2*(*i)->x/w +0.00,  -1+2*(*i)->y/w - 0.01);
		glVertex2f( -1+2*(*i)->x/w +0.01,  -1+2*(*i)->y/w + 0.00);
		glVertex2f( -1+2*(*i)->x/w -0.00,  -1+2*(*i)->y/w + 0.01);
		glEnd();
		c += 2;
	}
	
	for(vor::Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
	{
		glBegin(GL_LINES);
		glColor3b(120, 140, 160); 		glVertex2f(-1+2*(*i)->start->x/w, -1+2*(*i)->start->y/w);
		glColor3b(20, 40, 60); 		glVertex2f( -1+2*(*i)->end->x/w, -1+2*(*i)->end->y/w);
		glEnd();
	}
}

void display (void) 
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f); 
	glFlush();
}


void onEF(int n)
{
	glutTimerFunc(50, onEF, 0);
	glClear(GL_COLOR_BUFFER_BIT); 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 	
	drawVoronoi();
	glutSwapBuffers();
}

void reshape (int width, int height) 
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); 	glMatrixMode(GL_PROJECTION); 	glLoadIdentity(); 	gluPerspective(22.5, (GLfloat)width / (GLfloat)height, 1.0, 100.0); 	glMatrixMode(GL_MODELVIEW); }