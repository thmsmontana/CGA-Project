/////////////////////////////////////////////////////////////////////         
// circle.cpp
//
// This program draws a line loop with vertices equally apart on 
// a fixed circle. The larger the number of vertices the better
// the loop approximates the circle.
//
// Interaction:
// Press +/- to increase/decrease the number of vertices of the loop. 
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "Ring.h"
#include "Tunnel.h"

using namespace std;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float position = 0.0;
static bool isAnimate = false;
static float animationPeriod = 0.1;
static float t = 0.0;
Tunnel tunnel;

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLoadIdentity();
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);
	
	glTranslatef(0.0, 1.7, 1.0);

	glPushMatrix();
	glRotatef(position, 0.0, 0.0, 1.0);
	tunnel.draw(t);
	glPopMatrix();
	glutSwapBuffers();

}

// Initialization routine.
void setup(void)
{
	glEnable(GL_DEPTH_TEST); // Enable depth testing.
	setupRingLists();
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

// Timer function.
void animate(int value)
{
	cout << "";
	if (isAnimate)
	{
		t += 0.015;
	}
	glutTimerFunc(animationPeriod, animate, 1);
	glutPostRedisplay();
}


// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}


// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case ' ':
		if (isAnimate) isAnimate = 0;
		else isAnimate = 1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void playerInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		position -= 1;
		if (position < 0.0) position += 360.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		position += 1;
		if (position > 360.0) position -= 360.0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Spacebar to start" << endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Game");
	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(playerInput);
	glutTimerFunc(animationPeriod, animate, 1);
	glutMainLoop();

	return 0;
}
