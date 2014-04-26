#include <list>

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
	float angle;
	float dirX, dirY;
private:
	int obstacle;
	float r, g, b;
};

#pragma once
class Tunnel
{
public:
	Tunnel();
	Tunnel(float obstacle_probability);
	virtual ~Tunnel();
	void draw(int c);
private:
	void pushRing();
	float offset;
	std::list <Ring *> rings;
	float prevOffset;
	float obstacle_proba;
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
#define SPEED 50
#define COEF_OBSTACLE 0.7