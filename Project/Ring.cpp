#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "Ring.h"



#define PI 3.14159265358979324

#define SIDES 10
#define WIDTH 2.0
#define RADIUS 3.0

Ring::Ring()
{
}


void Ring::draw()
{
	float t = 0; // Angle parameter.
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	float *coords = (float *)malloc(sizeof(float) * (6 * (SIDES + 1)));
	float *coords_start = coords;


	for (i = 0; i < SIDES; ++i)
	{
		*(coords++) = RADIUS * cos(t);
		*(coords++) = RADIUS * sin(t);
		*(coords++) = 0;
		
		*(coords++) = RADIUS * cos(t);
		*(coords++) = RADIUS * sin(t);
		*(coords++) = -WIDTH;
		t += 2 * PI / SIDES;
	}
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = 0;
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = -WIDTH;


	glColor3f(1.0, 0.0, 0.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coords_start);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (SIDES + 1));
	glDisableClientState(GL_VERTEX_ARRAY);
}

float Ring::width() {
	return WIDTH;
}

Ring::~Ring()
{
}