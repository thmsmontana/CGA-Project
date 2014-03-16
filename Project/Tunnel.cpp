#include <cstdlib>
#include <cmath>
#include <iostream>
#include <math.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <list>

#include "Tunnel.h"
#include "Ring.h"


#define MAX_RINGS 120
#define ANGLE 0.4

using namespace std;
Tunnel::Tunnel()
{
	offset = 0;
	int i;
	for (i = 0; i < MAX_RINGS; ++i)
	{
		Ring *r = new Ring(ANGLE);
		rings.push_back(r);
	}
}


Tunnel::~Tunnel()
{
}


void Tunnel::draw(float t)
{
	offset = fmodf(t, 2.0);
	if (offset < prevOffset)
	{
		list <Ring *>::iterator it = rings.begin();
		free(*it);
		rings.pop_front();
		Ring *r = new Ring(ANGLE);
		rings.push_back(r);
	}
	prevOffset = offset;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	for each (Ring *r in rings)
	{
		glPushMatrix();
		glTranslatef(0.0, 0.0, offset);
		r->draw();
		glPopMatrix();
		glTranslatef(0.0, 0.0, -2.0);
		glRotatef(r->angle, 0.0, 1.0, 0.0);
	}
	glPopMatrix();
}
