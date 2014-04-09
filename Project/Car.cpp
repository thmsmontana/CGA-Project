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

#include "Car.h"



#define PI 3.14159265359

#define WHEEL_CIRC_POINTS 10
#define CARCASS_INTERWHEEL_L_POINTS 13
#define CARCASS_OVERFLOW_POINTS 3
#define CARCASS_EXTREMITY_CIRC_POINTS 4
#define CARCASS_FRONT_POINTS 2

#define WHEEL_EXT_RADIUS 5.0
#define WHEEL_INN_RADIUS 1.5
#define WHEEL_WIDTH 3.0
#define WHEEL_GAP 0.3
#define INTERWHEEL_W 18.0
#define INTERWHEEL_L 20.0
#define BACK_OVERFLOW 1.0
#define FRONT_OVERFLOW 4.0
#define CARCASS_HEIGHT 8.5
#define CARCASS_EXTREMITY_RADIUS 1.0
#define TOP_HEIGHT 8.0
#define TOP_LENGTH_RATIO 0.8
#define TOP_WIDTH_RATIO 0.9
#define TOP_OFFSET 3.0

#define FRONT 1.0
#define BACK -1.0


// Begin globals.


static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate the scene.
static bool wireframe = true;
static int isAnimate = 0; // Animated?
static float animationPeriod = 50;
static float t = 0.0;




using namespace std;

static unsigned int carListId;

Car::Car()
{

}


Car::~Car()
{
}


void Car::draw()
{
	glPushMatrix();
	glTranslatef(0.0, -4.0, -5.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.06, 0.06, 0.06);
	glCallList(carListId);
	glPopMatrix();
}



void drawWheel(float ext_radius, float inn_radius, float width)
{
	// Setting up the cos and sin values for later use.
	float cosine[WHEEL_CIRC_POINTS + 1], sine[WHEEL_CIRC_POINTS + 1], angle;
	for (int t = 0; t < WHEEL_CIRC_POINTS + 1; ++t)
	{
		angle = t * PI * 2.0 / WHEEL_CIRC_POINTS;
		cosine[t] = cos(angle);
		sine[t] = sin(angle);
	}

	glBegin(GL_TRIANGLE_STRIP);
	for (int t = 0; t < WHEEL_CIRC_POINTS; ++t)
	{
		glVertex3f(ext_radius * cosine[t], ext_radius * sine[t], -width / 2);
		glVertex3f(ext_radius * cosine[t], ext_radius * sine[t], width / 2);
		glVertex3f(ext_radius * cosine[t + 1], ext_radius * sine[t + 1], width / 2);
		glVertex3f(inn_radius * cosine[t], inn_radius * sine[t], width / 2);
		glVertex3f(inn_radius * cosine[t + 1], inn_radius * sine[t + 1], width / 2);
		glVertex3f(inn_radius * cosine[t], inn_radius * sine[t], -width / 2);
		glVertex3f(inn_radius * cosine[t + 1], inn_radius * sine[t + 1], -width / 2);
		glVertex3f(ext_radius * cosine[t], ext_radius * sine[t], -width / 2);
	}
	glVertex3f(ext_radius, 0, -width / 2);
	glEnd();
}

void drawWheelCarcass(float wheel_radius, float wheel_gap, float carcass_height)
{
	float radius = wheel_radius + wheel_gap;
	float angle, x;
	glBegin(GL_TRIANGLE_STRIP);
	for (int t = 0; t < WHEEL_CIRC_POINTS / 2 + 1; ++t)
	{
		angle = t * 2 * PI / WHEEL_CIRC_POINTS;
		x = radius * cos(angle);
		glVertex3f(x, radius * sin(angle), 0.0);
		glVertex3f(x, carcass_height, 0.0);
	}
	glEnd();
}

void drawInterWheelCarcass(float interwheel_l, float carcass_height)
{
	float x;
	glBegin(GL_TRIANGLE_STRIP);
	for (int t = 0; t < CARCASS_INTERWHEEL_L_POINTS + 1; ++t)
	{
		x = (t * interwheel_l / CARCASS_INTERWHEEL_L_POINTS) - (interwheel_l / 2);
		glVertex3f(x, 0.0, 0.0);
		glVertex3f(x, carcass_height, 0.0);
	}
	glEnd();
}

void drawExtremity(float interwheel_w, float interwheel_l, float wheel_width, float overflow, float angle_radius, float carcass_height, float wheel_radius, float wheel_gap, float direction)
{
	float x, z;
	glPushMatrix();
	glScalef(direction, 1.0, 1.0);
	glTranslatef(interwheel_l / 2.0 + 2.0 * (wheel_radius + wheel_gap), 0.0, 0.0);

	glBegin(GL_TRIANGLE_STRIP);
	z = wheel_width / 2.0;
	for (int t = 0; t < CARCASS_OVERFLOW_POINTS; ++t)
	{
		x = t * overflow / CARCASS_OVERFLOW_POINTS;
		glVertex3f(x, 0.0, z);
		glVertex3f(x, carcass_height, z);
	}
	float angle;
	float centerX = overflow, centerZ = wheel_width / 2.0 - angle_radius;
	for (int t = 0; t < CARCASS_EXTREMITY_CIRC_POINTS; ++t)
	{
		angle = PI * t / (2.0 * CARCASS_EXTREMITY_CIRC_POINTS);
		x = centerX + angle_radius * sin(angle);
		z = centerZ + angle_radius * cos(angle);
		glVertex3f(x, 0.0, z);
		glVertex3d(x, carcass_height, z);
	}
	x = overflow + angle_radius;
	for (int t = 0; t < CARCASS_FRONT_POINTS + 1; ++t)
	{
		z = -t * interwheel_w / (2.0 * CARCASS_FRONT_POINTS);
		glVertex3f(x, 0.0, z);
		glVertex3f(x, carcass_height, z);
	}
	glEnd();
	glPopMatrix();
}

void drawFrontHood(float interwheel_l, float interwheel_w, float carcass_height, float wheel_gap, float wheel_width, float wheel_radius, float carcass_angle_radius, float front_overflow)
{
	glPushMatrix();
	glTranslatef(interwheel_l / 2.0, 0.0, 0.0);
	GLfloat bigZ = (interwheel_w + wheel_width) / 2.0;
	GLfloat grid2x2[2][2][3] =
	{ { { 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, bigZ } },
	{ { 2.0 * (wheel_radius + wheel_gap) + front_overflow, carcass_angle_radius, 0.0 },
	{ 2.0 * (wheel_radius + wheel_gap) + front_overflow, carcass_angle_radius, bigZ - carcass_angle_radius } } };
	GLfloat *grid = &grid2x2[0][0][0];
	int xSize = 4;
	int ySize = 4;
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0,	/* U ranges 0..1 */
		3,			/* U stride, 3 floats per coord */
		2,			/* U is 2nd order, ie. linear */
		0.0, 1.0,	/* V ranges 0..1 */
		2 * 3,		/* V stride, row is 4 coords, 3 floats per coord */
		2,			/* V is 2nd order, ie linear */
		grid);		/* control points */
	glMapGrid2f(
		3, 0.0, 1.0,
		3, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	if (wireframe) glEvalMesh2(GL_LINE, 0, 3, 0, 3);
	else glEvalMesh2(GL_FILL, 0, 3, 0, 3);


	float angle, x, y, z;
	float centerX = front_overflow + 2.0 * (wheel_gap + wheel_radius);
	glBegin(GL_TRIANGLE_STRIP);
	for (int t = 0; t < CARCASS_EXTREMITY_CIRC_POINTS; ++t)
	{
		angle = PI * t / (2.0 * CARCASS_EXTREMITY_CIRC_POINTS);
		x = centerX + carcass_angle_radius * sin(angle);
		y = carcass_angle_radius * cos(angle);
		glVertex3f(x, y, 0.0);
		glVertex3d(x, y, bigZ - carcass_angle_radius);
	}
	glEnd();

	float centerZ = bigZ - carcass_angle_radius;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, bigZ);
	for (int t = 0; t < CARCASS_EXTREMITY_CIRC_POINTS; ++t)
	{
		angle = PI * t / (2.0 * CARCASS_EXTREMITY_CIRC_POINTS);
		y = carcass_angle_radius * sin(angle);
		z = centerZ + carcass_angle_radius * cos(angle);
		glVertex3d((wheel_radius + wheel_gap) * 2.0 + front_overflow, y, z);
	}
	glEnd();


	glTranslatef((wheel_radius + wheel_gap) * 2.0 + front_overflow, 0.0, bigZ - carcass_angle_radius);
	if (wireframe) glutWireSphere(carcass_angle_radius, CARCASS_EXTREMITY_CIRC_POINTS, CARCASS_EXTREMITY_CIRC_POINTS);
	else glutSolidSphere(carcass_angle_radius, CARCASS_EXTREMITY_CIRC_POINTS, CARCASS_EXTREMITY_CIRC_POINTS);

	glPopMatrix();
}

void drawBackHood(float interwheel_l, float interwheel_w, float carcass_height, float wheel_gap, float wheel_width, float wheel_radius, float carcass_angle_radius, float back_overflow)
{
	glPushMatrix();
	glTranslatef(-interwheel_l / 2.0 - 2.0 * (wheel_radius + wheel_gap), 0.0, 0.0);

	glBegin(GL_TRIANGLE_STRIP);
	float angle;
	float x, z, centerZ = (interwheel_w + wheel_width) / 2.0 - carcass_angle_radius;
	for (int t = 0; t < CARCASS_EXTREMITY_CIRC_POINTS; ++t)
	{
		angle = PI * t / (2.0 * CARCASS_EXTREMITY_CIRC_POINTS);
		x = -back_overflow - carcass_angle_radius * sin(angle);
		z = centerZ + carcass_angle_radius * cos(angle);
		glVertex3f(0.0, 0.0, z);
		glVertex3d(x, 0.0, z);
	}
	x = -back_overflow - carcass_angle_radius;
	z = centerZ;
	for (int t = 0; t < CARCASS_FRONT_POINTS; ++t)
	{
		z -= interwheel_w / (2.0 * CARCASS_FRONT_POINTS);
		glVertex3f(0.0, 0.0, z);
		glVertex3f(x, 0.0, z);
	}
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	glPopMatrix();
}

void drawTop(float interwheel_l, float interwheel_w, float wheel_width, float wheel_radius, float wheel_gap, float top_height, float top_length_ratio, float top_width_ratio, float top_offset)
{
	glPushMatrix();
	glTranslatef(-top_offset, 0.0, 0.0);
	GLfloat plafond[2][2][3] =
	{ { { -interwheel_l * top_length_ratio / 2.0, top_height, 0.0 },
	{ -interwheel_l * top_length_ratio / 2.0, top_height, top_width_ratio * (interwheel_w + wheel_width / 2.0) / 2.0 } },
	{ { interwheel_l * top_length_ratio / 2.0, top_height, 0.0 },
	{ interwheel_l * top_length_ratio / 2.0, top_height, top_width_ratio * (interwheel_w + wheel_width / 2.0) / 2.0 } } };
	GLfloat *grid = &plafond[0][0][0];
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0,	/* U ranges 0..1 */
		3,			/* U stride, 3 floats per coord */
		2,			/* U is 2nd order, ie. linear */
		0.0, 1.0,	/* V ranges 0..1 */
		2 * 3,		/* V stride, row is 4 coords, 3 floats per coord */
		2,			/* V is 2nd order, ie linear */
		grid);		/* control points */
	glMapGrid2f(
		10, 0.0, 1.0,
		10, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	if (wireframe) glEvalMesh2(GL_LINE, 0, 3, 0, 3);
	else glEvalMesh2(GL_FILL, 0, 3, 0, 3);
	glPopMatrix();

	GLfloat vitreAvant[2][2][3] =
	{ { { interwheel_l / 2.0, 0.0, 0.0 },
	{ interwheel_l / 2.0, 0.0, (interwheel_w + wheel_width) / 2.0 } },
	{ { interwheel_l * top_length_ratio / 2.0 - top_offset, top_height, 0.0 },
	{ interwheel_l * top_length_ratio / 2.0 - top_offset, top_height, top_width_ratio * (interwheel_w + wheel_width / 2.0) / 2.0 } } };
	grid = &vitreAvant[0][0][0];
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0,	/* U ranges 0..1 */
		3,			/* U stride, 3 floats per coord */
		2,			/* U is 2nd order, ie. linear */
		0.0, 1.0,	/* V ranges 0..1 */
		2 * 3,		/* V stride, row is 4 coords, 3 floats per coord */
		2,			/* V is 2nd order, ie linear */
		grid);		/* control points */
	glMapGrid2f(
		3, 0.0, 1.0,
		3, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	if (wireframe) glEvalMesh2(GL_LINE, 0, 3, 0, 3);
	else glEvalMesh2(GL_FILL, 0, 3, 0, 3);


	GLfloat vitreArriere[2][2][3] =
	{ { { -interwheel_l / 2.0 - (wheel_radius + wheel_gap) * 2.0, 0.0, 0.0 },
	{ -interwheel_l / 2.0 - (wheel_radius + wheel_gap) * 2.0, 0.0, (interwheel_w + wheel_width) / 2.0 } },
	{ { -interwheel_l * top_length_ratio / 2.0 - top_offset, top_height, 0.0 },
	{ -interwheel_l * top_length_ratio / 2.0 - top_offset, top_height, top_width_ratio * (interwheel_w + wheel_width / 2.0) / 2.0 } } };
	grid = &vitreArriere[0][0][0];
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0,	/* U ranges 0..1 */
		3,			/* U stride, 3 floats per coord */
		2,			/* U is 2nd order, ie. linear */
		0.0, 1.0,	/* V ranges 0..1 */
		2 * 3,		/* V stride, row is 4 coords, 3 floats per coord */
		2,			/* V is 2nd order, ie linear */
		grid);		/* control points */
	glMapGrid2f(
		3, 0.0, 1.0,
		3, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	if (wireframe) glEvalMesh2(GL_LINE, 0, 3, 0, 3);
	else glEvalMesh2(GL_FILL, 0, 3, 0, 3);


	GLfloat cote[2][2][3] =
	{ { { interwheel_l / 2.0, 0.0, (interwheel_w + wheel_width) / 2.0 },
	{ -interwheel_l / 2.0 - (wheel_radius + wheel_gap) * 2.0, 0.0, (interwheel_w + wheel_width) / 2.0 } },
	{ { interwheel_l * top_length_ratio / 2.0 - top_offset, top_height, top_width_ratio * (interwheel_w + wheel_width / 2.0) / 2.0 },
	{ -interwheel_l * top_length_ratio / 2.0 - top_offset, top_height, top_width_ratio * (interwheel_w + wheel_width / 2.0) / 2.0 } } };
	grid = &cote[0][0][0];
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0,	/* U ranges 0..1 */
		3,			/* U stride, 3 floats per coord */
		2,			/* U is 2nd order, ie. linear */
		0.0, 1.0,	/* V ranges 0..1 */
		2 * 3,		/* V stride, row is 4 coords, 3 floats per coord */
		2,			/* V is 2nd order, ie linear */
		grid);		/* control points */
	glMapGrid2f(
		3, 0.0, 1.0,
		3, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	if (wireframe) glEvalMesh2(GL_LINE, 0, 3, 0, 3);
	else glEvalMesh2(GL_FILL, 0, 3, 0, 3);
}

void drawHalfOfTheCar(float t) {
	glPushMatrix();
	glTranslatef(0.0, 0.0, INTERWHEEL_W / 2);


	// back
	glColor3f(1.0, 1.0, 1.0);
	drawExtremity(INTERWHEEL_W, INTERWHEEL_L, WHEEL_WIDTH, BACK_OVERFLOW, CARCASS_EXTREMITY_RADIUS, CARCASS_HEIGHT, WHEEL_EXT_RADIUS, WHEEL_GAP, BACK);
	glPushMatrix();

	glTranslatef(-(INTERWHEEL_L / 2 + WHEEL_GAP + WHEEL_EXT_RADIUS), 0.0, 0.0);
	glPushMatrix();
	glRotatef(t, 0.0, 0.0, -1.0);
	drawWheel(WHEEL_EXT_RADIUS, WHEEL_INN_RADIUS, WHEEL_WIDTH);
	glPopMatrix();

	glTranslatef(0.0, 0.0, WHEEL_WIDTH / 2);
	drawWheelCarcass(WHEEL_EXT_RADIUS, WHEEL_GAP, CARCASS_HEIGHT);

	glPopMatrix();

	// middle carcass
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();

	glTranslatef(0.0, 0.0, WHEEL_WIDTH / 2);
	drawInterWheelCarcass(INTERWHEEL_L, CARCASS_HEIGHT);

	glPopMatrix();

	// front
	glColor3f(1.0, 1.0, 1.0);
	drawExtremity(INTERWHEEL_W, INTERWHEEL_L, WHEEL_WIDTH, FRONT_OVERFLOW, CARCASS_EXTREMITY_RADIUS, CARCASS_HEIGHT, WHEEL_EXT_RADIUS, WHEEL_GAP, FRONT);
	glPushMatrix();

	glTranslatef(INTERWHEEL_L / 2 + WHEEL_GAP + WHEEL_EXT_RADIUS, 0.0, 0.0);
	glPushMatrix();
	glRotatef(t, 0.0, 0.0, -1.0);
	drawWheel(WHEEL_EXT_RADIUS, WHEEL_INN_RADIUS, WHEEL_WIDTH);
	glPopMatrix();

	glTranslatef(0.0, 0.0, WHEEL_WIDTH / 2);
	drawWheelCarcass(WHEEL_EXT_RADIUS, WHEEL_GAP, CARCASS_HEIGHT);

	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, CARCASS_HEIGHT, 0.0);
	drawFrontHood(INTERWHEEL_L, INTERWHEEL_W, CARCASS_HEIGHT, WHEEL_GAP, WHEEL_WIDTH, WHEEL_EXT_RADIUS, CARCASS_EXTREMITY_RADIUS, FRONT_OVERFLOW);
	drawBackHood(INTERWHEEL_L, INTERWHEEL_W, CARCASS_HEIGHT, WHEEL_GAP, WHEEL_WIDTH, WHEEL_EXT_RADIUS, CARCASS_EXTREMITY_RADIUS, BACK_OVERFLOW);
	drawTop(INTERWHEEL_L, INTERWHEEL_W, WHEEL_WIDTH, WHEEL_EXT_RADIUS, WHEEL_GAP, TOP_HEIGHT, TOP_LENGTH_RATIO, TOP_WIDTH_RATIO, TOP_OFFSET);
	glPopMatrix();
}




void setupShipList()
{
	carListId = glGenLists(1);
	glNewList(carListId, GL_COMPILE); glPushMatrix();
	glTranslatef(t / 360.0 * 2 * PI * WHEEL_EXT_RADIUS, 0.0, 0.0);
	// right side of the car
	glPushMatrix();
	drawHalfOfTheCar(t);
	glPopMatrix();
	// left side of the car
	glPushMatrix();
	glScalef(1.0, 1.0, -1.0);
	drawHalfOfTheCar(t);
	glPopMatrix();
	glPopMatrix();
	glEndList();
}