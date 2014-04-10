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
#include "rgb_hsv.h"

#define PI 3.14159265358979324

using namespace std;


static unsigned int ringListId, obstacleListId;
static int angle = 0.0;


Tunnel::Tunnel() : offset(0.0), prevOffset(0.0), obstacle_proba(0.0), t(0.0)
{
	int i;
	for (i = 0; i < MAX_RINGS; ++i)
	{
		pushRing();
	}
}

Tunnel::Tunnel(float obstacle_probability) :offset(0.0), prevOffset(0.0), obstacle_proba(obstacle_probability), t(0.0) {
	int i;
	for (i = 0; i < MAX_RINGS; ++i)
	{
		pushRing();
	}
}
Tunnel::~Tunnel()
{
}

void Tunnel::pushRing()
{
	hsv color;
	color.h = angle;
	angle += 5 * cos(t/5.0) * cos(t/5.0);
	if (angle > 360.0) angle -= 360.0;
	color.s = 1.0;
	color.v = 1.0;
	rgb rgbcolor = hsv2rgb(color);
	float ringColor[] = {rgbcolor.r, rgbcolor.g, rgbcolor.b};
	Ring *r = new Ring(angleParams(), ringColor);
	if (rand() % 10 < 10 * obstacle_proba)
	{
		r->setObstacle(rand() % TUNNEL_SIDES);
	}
	rings.push_back(r);
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
		pushRing();
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
		glRotatef(r->angle, r->dirX, r->dirY, 0.0);
	}
	glPopMatrix();
}


float *Tunnel::angleParams() {
	float *values = (float *)malloc(sizeof(float)* 3);
	values[0] = 0.7 * sin(t);
	values[1] = cos(cos(t));
	values[2] = sin(cos(t));
	t += 0.01;
	return values;
}



Ring::Ring() : angle(0.0), dirX(1.0), dirY(0.0), obstacle(-1), r(0), g(0), b(1) {}
Ring::Ring(float* angleParams) : angle(angleParams[0]), dirX(angleParams[1]), dirY(angleParams[2]), obstacle(-1), r(0), g(0), b(1) {}
Ring::Ring(float* angleParams, float* c) : angle(angleParams[0]), dirX(angleParams[1]), dirY(angleParams[2]), obstacle(-1), r(c[0]), g(c[1]), b(c[2]) {}

Ring::~Ring() {}

void Ring::draw()
{
	glColor4f(r, g, b, 1.0);
	glCallList(ringListId);
	if (obstacle != -1) {
		glPushMatrix();
		glRotatef(obstacle * 360.0 / TUNNEL_SIDES, 0.0, 0.0, 1.0);
		float emission[] = { 1.0, 0.0, 0.0, 1.0 };
		glColor4fv(emission);
		glCallList(obstacleListId);

		glPopMatrix();
	}
}


void Ring::setObstacle(int obs) {
	obstacle = obs;
}



void setupTunnelLists() {
	unsigned int id = glGenLists(2);
	makeRingList(id++);
	makeObstacleList(id);
}

void makeRingList(unsigned int id)
{
	ringListId = id;
	glNewList(id, GL_COMPILE);
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
		*(coords++) = -SECTION_WIDTH * 0.2;
		t += 2 * PI / TUNNEL_SIDES;
	}
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = 0;
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = -SECTION_WIDTH * 0.2;


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coords_start);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (TUNNEL_SIDES + 1));
	glDisableClientState(GL_VERTEX_ARRAY);
	glEndList();
}

void makeObstacleList(unsigned int id)
{
	obstacleListId = id;
	glNewList(id, GL_COMPILE);
	float *coords = (float *)malloc(sizeof(float)* (3 * 8));
	float *coords_start = coords;
	float angle = 2 * PI / TUNNEL_SIDES;
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
	*(coords++) = RADIUS * cos(angle);
	*(coords++) = RADIUS * sin(angle);
	*(coords++) = 0;
	//6
	*(coords++) = RADIUS * cos(angle);
	*(coords++) = RADIUS * sin(angle);
	*(coords++) = -SECTION_WIDTH;
	//7
	*(coords++) = RADIUS * 0.5 * cos(angle);
	*(coords++) = RADIUS * 0.5 * sin(angle);
	*(coords++) = -SECTION_WIDTH;
	//8
	*(coords++) = RADIUS * 0.5  * cos(angle);
	*(coords++) = RADIUS * 0.5 * sin(angle);
	*(coords++) = 0;

	int indices[14] = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coords_start);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEndList();
}
