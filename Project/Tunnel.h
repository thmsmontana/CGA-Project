#include <list>
using namespace std;

#pragma once
class Ring
{
public:
	Ring(void);
	Ring(float* angleParams);
	Ring(float* angleParams, float* c);
	virtual ~Ring();
	void draw();
	void setObstacle(int obs);
	int getObstacle();
	float angle;
	float dirX, dirY;
private:
	int obstacle;
	float r, g, b, alpha;
};

#pragma once
class Tunnel
{
public:
	Tunnel();
	Tunnel(float obstacle_probability);
	virtual ~Tunnel();
	void draw(int c);
	bool hasObstacleAtPosition(float position);
	float obstacle_proba;
private:
	void drawRec(float offset, list<Ring *>::iterator *it);
	void pushRing();
	float offset;
	std::list <Ring *> rings;
	float prevOffset;
	float *angleParams();
	float t;
};


void setupTunnelLists();
void makeRingList(unsigned int listId);
void makeObstacleList(unsigned int listId);

#define TUNNEL_SIDES 15
#define SECTION_WIDTH 2.0
#define RADIUS 3.0
#define MAX_RINGS 250
#define ALPHA_RING 0.7
#define SPEED 30
#define COEF_OBSTACLE 0.7
#define OBSTACLE_ANGULAR_WIDTH 360/TUNNEL_SIDES

#define PATH_OBSTACLE_TEXTURE "textures/moon.bmp"

