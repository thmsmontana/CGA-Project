#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <math.h>
#include <time.h>


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "Tunnel.h"
#include "rgb_hsv.h"
#include "Game.h"
#include "imageloader.h"

#define PI 3.14159265358979324

using namespace std;


static unsigned int ringListId, obstacleListId;
static int angle = 0.0;

GLuint loadTexture(Image* image) {

	GLuint textureId;

	glGenTextures(1, &textureId); //Make room for our texture

	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit

	//Map the image to the texture

	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D

		0,                            //0 for now

		GL_RGB,                       //Format OpenGL uses for image

		image->width, image->height,  //Width and height

		0,                            //The border of the image

		GL_RGB, //GL_RGB, because pixels are stored in RGB format

		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored

		//as unsigned numbers

		image->pixels);               //The actual pixel data

	return textureId; //Returns the id of the texture

}

GLUquadric *quad;
GLuint _textureId; //The id of the textur


Tunnel::Tunnel() : offset(0.0), prevOffset(0.0), obstacle_proba(0.0), t(0.0)
{
	int i;
	for (i = 0; i < MAX_RINGS; ++i)
	{
		pushRing();
	}
}

void initTunnel()
{
	quad = gluNewQuadric();
	Image* image = loadBMP(PATH_OBSTACLE_TEXTURE);

	_textureId = loadTexture(image);
}

Tunnel::Tunnel(float obstacle_probability) :offset(0.0), prevOffset(0.0), obstacle_proba(obstacle_probability), t(0.0) {
	int i;
	for (i = 0; i < MAX_RINGS; ++i)
	{
		pushRing();
	}

	initTunnel();
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
	float ringColor[] = {rgbcolor.r, rgbcolor.g, rgbcolor.b, ALPHA_RING};
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
	list<Ring *>::iterator begin = rings.begin();
	glPushMatrix();
	drawRec(offset, &begin);
	glPopMatrix();
}


void Tunnel::drawRec(float offset, list<Ring *>::iterator *it)
{
	Ring *r = *((*it)++);
	if (*it != rings.end())
	{
		glTranslated(0.0, 0.0, -SECTION_WIDTH);
		glRotated(r->angle, r->dirX, r->dirY, 0.0);
		
		drawRec(offset, it);

		// "pop"
		glRotated(-(r->angle), r->dirX, r->dirY, 0.0);
		glTranslated(0.0, 0.0, SECTION_WIDTH);
	}

	glPushMatrix();
	glTranslatef(0.0, 0.0, offset);
	r->draw();
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



Ring::Ring() : angle(0.0), dirX(1.0), dirY(0.0), obstacle(-1), r(0), g(0), b(1), alpha(1) {}
Ring::Ring(float* angleParams) : angle(angleParams[0]), dirX(angleParams[1]), dirY(angleParams[2]), obstacle(-1), r(0), g(0), b(1), alpha(1) {}
Ring::Ring(float* angleParams, float* c) : angle(angleParams[0]), dirX(angleParams[1]), dirY(angleParams[2]), obstacle(-1), r(c[0]), g(c[1]), b(c[2]), alpha(c[3]) {}

Ring::~Ring() {}

void Ring::draw()
{
	float ringColor[] = { r, g, b, alpha };
	float obstacleColor[] = {1.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ringColor);
	glCallList(ringListId);
	if (obstacle != -1) {
		glPushMatrix();
		glRotatef((obstacle+0.5) * OBSTACLE_ANGULAR_WIDTH , 0.0, 0.0, 1.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, obstacleColor);
		glCallList(obstacleListId);

		glPopMatrix();
	}
}


void Ring::setObstacle(int obs) {
	obstacle = obs;
}

int Ring::getObstacle(){
	return obstacle;
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
		*(coords++) = -SECTION_WIDTH * 1.0;
		t += 2 * PI / TUNNEL_SIDES;
	}
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = 0;
	*(coords++) = RADIUS;
	*(coords++) = 0;
	*(coords++) = -SECTION_WIDTH * 1.0;


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

	float angle = 2 * PI / TUNNEL_SIDES;

	/*
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
	*(coords++) = RADIUS * COEF_OBSTACLE;
	*(coords++) = 0;
	*(coords++) = 0;
	//4
	*(coords++) = RADIUS * COEF_OBSTACLE;
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
	*(coords++) = RADIUS * COEF_OBSTACLE * cos(angle);
	*(coords++) = RADIUS * COEF_OBSTACLE * sin(angle);
	*(coords++) = -SECTION_WIDTH;
	//8
	*(coords++) = RADIUS * COEF_OBSTACLE  * cos(angle);
	*(coords++) = RADIUS * COEF_OBSTACLE * sin(angle);
	*(coords++) = 0;

	int indices[14] = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coords_start);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, indices);*/

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	gluQuadricTexture(quad, 1);

	float movX = RADIUS * COEF_OBSTACLE;
	float rad = RADIUS * COEF_OBSTACLE / 4;
	glTranslatef(movX, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluSphere(quad, rad , 20, 20);
	glTranslatef(-movX, 0.0, 0.0);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_VERTEX_ARRAY);
	glEndList();
}


bool Tunnel::hasObstacleAtPosition(float p)
{
	
	Ring r = *rings.front();
	int faceObstacle = r.getObstacle();

	if (faceObstacle == -1)
		return false;

	int leftBoundaryObstacle = OBSTACLE_ANGULAR_WIDTH*faceObstacle;
	int rightBoundaryObstacle = leftBoundaryObstacle + OBSTACLE_ANGULAR_WIDTH;
	


	int leftBoundaryShip = p - SHIP_ANGULAR_WIDTH / 2;
	if (leftBoundaryShip < 0)
		leftBoundaryShip += 360;

	int rightBoundaryShip = p + SHIP_ANGULAR_WIDTH / 2;
	if (rightBoundaryShip >= 360)
		rightBoundaryShip -= 360;

	if (leftBoundaryShip >= leftBoundaryObstacle && leftBoundaryShip <= rightBoundaryObstacle)
		return true;



	if (rightBoundaryShip >= leftBoundaryObstacle && leftBoundaryShip <= rightBoundaryObstacle)
		return true;


	return false;

	
}