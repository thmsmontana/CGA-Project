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

#include "Game.h"

using namespace std;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static bool isAnimate = false;
static float animationPeriod = 10;
Game game;

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLoadIdentity();
	glRotatef(10.0, -1.0, 0.0, 0.0);
	glTranslatef(0.0, 2.0, -1.0);

	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	glColor3f(1.0, 1.0, 1.0);
	game.draw();

	glutSwapBuffers();

}

// Initialization routine.
void setup(void)
{
	glEnable(GL_DEPTH_TEST); // Enable depth testing.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	game = Game();
}

// Timer function.
void animate(int value)
{
	cout << "";
	glutTimerFunc(animationPeriod, animate, 1);
	glutPostRedisplay();
}


// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 500.0);
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
		game.playPause();
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
		game.right();
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		game.left();
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
	glutInitWindowPosition(50, 50);
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
