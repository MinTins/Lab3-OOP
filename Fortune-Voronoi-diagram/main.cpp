/*
K-23 Flakey Roman
Метод Форчуни. Діаграма Вороного
*/


#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <GL/glew.h>
#include <glut.h>
#include "recursiveVoronoi.h"
#include <windows.h>
#include <stack>
#define EPSILON std::numeric_limits<double>::epsilon()
double w = 600;
double ww = 800;
void display(void);
void onEF(int n);
void reshape(int width, int height);
std::vector<Vector2> corpos = { Vector2(0.0, w/2),Vector2(w / 2, 0.0),Vector2(w, w / 2) ,Vector2(w / 2, w) };
DiscreteGlobalGrid discreteGlobalGrid;

void drawPoints()
{
	std::vector<Vector2> points = discreteGlobalGrid.points;
	glBegin(GL_QUADS);
	for (std::size_t i = 0; i < points.size(); ++i)
	{
		Vector2 p = points[i];
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww - 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww - 0.01);
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww + 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww - 0.01);
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww + 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww + 0.01);
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww - 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww + 0.01);
	}
	glEnd();
}
void drawOtherPoints()
{
	std::vector<Vector2> points = discreteGlobalGrid.otherTestPoints;
	glBegin(GL_QUADS);
	for (std::size_t i = 0; i < points.size(); ++i)
	{
		Vector2 p = points[i];
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww - 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww - 0.01);
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww + 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww - 0.01);
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww + 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww + 0.01);
		glVertex2f(-1 + 2 * (p.x + (ww - w) / 2) / ww - 0.01, -1 + 2 * (p.y + (ww - w) / 2) / ww + 0.01);
	}
	glEnd();
}
void drawDiagram()
{
	std::vector<Edge> edges = discreteGlobalGrid.edges;
	int dcount = edges.size();
	for (size_t i = 0; i < dcount; i++)
	{
		Edge e = edges[i];
		std::vector<Vector2> origin = e.origin;
		std::vector<Vector2> destination = e.destination;
		int vcount = origin.size();
		for (size_t j = 0; j < vcount; j++)
		{
			glBegin(GL_LINES);
			glVertex2f(-1 + 2 * (origin[j].x + (ww - w) / 2) / ww, -1 + 2 * (origin[j].y + (ww - w) / 2) / ww);
			glVertex2f(-1 + 2 * (destination[j].x + (ww - w) / 2) / ww, -1 + 2 * (destination[j].y + (ww - w) / 2) / ww);
			glEnd();
		}
	}
}

void drawVoronoi()
{
	drawDiagram();
}

void display(void)
{
	std::cout << "display\n";
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	glFlush();
}
void onEF(int n)
{

	glutTimerFunc(20, onEF, 0);
	glClear(GL_COLOR_BUFFER_BIT);//Clear the screen
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	drawVoronoi();
	glutSwapBuffers();
}

void reshape(int width, int height)
{

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(22.5, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

double get_angle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double theta = atan2(x1 - x3, y1 - y3) - atan2(x2 - x3, y2 - y3);
	if (theta > std::_Pi)
		theta -= 2 * std::_Pi;
	if (theta < -std::_Pi)
		theta += 2 * std::_Pi;

	theta = abs(theta * 180.0 / std::_Pi);
	return theta;
}

int main(int argc, char **argv)
{
	double a = get_angle(373.5030592, 0, 294.2053188, 62.68338888, 338.1511887, 0.750755333);
	double b = get_angle(294.2053188,62.68338888,283.9666317,40.06746694,338.1511887,0.750755333);
	double c = get_angle(283.9666317,40.06746694,282.3138898,0,338.1511887,0.750755333);
	double d = get_angle(282.3138898,0,373.5030592,0,338.1511887,0.750755333);
	recursiveVoronoi *rv = new recursiveVoronoi;
	rv->begin(corpos, 1);
	discreteGlobalGrid = rv->getDiscreteGlobalGrid();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("first OpenGL Window");

	glutTimerFunc(100, onEF, 0);
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}
