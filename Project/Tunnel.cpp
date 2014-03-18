#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <math.h>
#include <list>
#include <time.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "Tunnel.h"

#define PI 3.14159265358979324


using namespace std;


static unsigned int base; // Displays the lists base index.
void setupLists() {
	base = glGenLists(2);
	glListBase(base);

	makeRingList();
	makeObstacleList();
}

void makeRingList()
{
	glNewList(base, GL_COMPILE);
	float t = 0; // Angle parameter.
	int i;

	float *coords = (float *)malloc(sizeof(float)* (6 * (TUNNEL_SIDES + 1)));
	float *coords_start = coords;


	for (i = 0; i < TUNNEL_SIDES; ++i)
	{
		*(coords++) = RADIUS * cos(t);
		*(coords++) = RADIUS * sin(t);
		*(coords++) = 0;

		*(coords++) = RADIUS * cos(t);
		*(coords++) = RADIUS * sin(t);
		*(coords++) = -SECTION_WIDTH;
		t += 2 * PI / TUNNEL_SIDES;
	}
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = 0;
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = -SECTION_WIDTH;


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coords_start);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (TUNNEL_SIDES + 1));
	glDisableClientState(GL_VERTEX_ARRAY);
	glEndList();
}

void makeObstacleList()
{
	glNewList(base + 1, GL_COMPILE);
	float *coords = (float *)malloc(sizeof(float)* (3 * 8));
	float *coords_start = coords;

	//1
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = 0;
	//2
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = -SECTION_WIDTH;
	//3
	*(coords++) = RADIUS * 0.5;
	*(coords++) = 0;
	*(coords++) = 0;
	//4
	*(coords++) = RADIUS * 0.5;
	*(coords++) = 0;
	*(coords++) = -SECTION_WIDTH;
	//5
	*(coords++) = RADIUS * cos(PI * 0.2);
	*(coords++) = RADIUS * sin(PI * 0.2);
	*(coords++) = 0;
	//6
	*(coords++) = RADIUS * cos(PI * 0.2);
	*(coords++) = RADIUS * sin(PI * 0.2);
	*(coords++) = -SECTION_WIDTH;
	//7
	*(coords++) = RADIUS * 0.5 * cos(PI * 0.2);
	*(coords++) = RADIUS * 0.5 * sin(PI * 0.2);
	*(coords++) = -SECTION_WIDTH;
	//8
	*(coords++) = RADIUS * 0.5  * cos(PI * 0.2);
	*(coords++) = RADIUS * 0.5 * sin(PI * 0.2);
	*(coords++) = 0;

	int indices[14] = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coords_start);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEndList();
}



Tunnel::Tunnel()
{
	offset = 0;
	prevOffset = 0;
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

void Tunnel::draw(int c)
{
	float age = 1.0f * c / (float) CLOCKS_PER_SEC;
	offset = fmodf(age * SPEED, 2.0);
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





Ring::Ring() : angle(0.0) {}
Ring::Ring(float a) : angle(a) {}
Ring::~Ring() {}

void Ring::draw()
{
	glCallList(base);
	//if (obstacle != -1) {
	//	glPushMatrix();
	//	glRotatef(obstacle * PI * 2 / TUNNEL_SIDES, 0.0, 0.0, 1.0);
	//	glCallList(base + 1);
	//	glPopMatrix();
	//}
	glCallList(base + 1);
}