#include <list>

#pragma once
class Ring
{
public:
	Ring(void);
	virtual ~Ring();
	Ring(float _angle);
	void draw();
	void setObstacle(int obs);
	float angle;
private:
	int obstacle;
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
};


void setupLists();
void makeRingList();
void makeObstacleList();

#define TUNNEL_SIDES 10
#define SECTION_WIDTH 2.0
#define RADIUS 3.0
#define MAX_RINGS 250
#define ANGLE 0.4
#define SPEED 20