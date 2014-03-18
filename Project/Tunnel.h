#include <list>

#pragma once
class Ring
{
public:
	Ring(void);
	virtual ~Ring();
	Ring(float _angle);
	void draw();
	float getWidth();
	float angle;
	int obstacle;
};

#pragma once
class Tunnel
{
public:
	Tunnel();
	virtual ~Tunnel();
	void draw(int c);
private:
	float offset;
	std::list <Ring *> rings;
	float prevOffset;
};



void setupLists();
void makeRingList();
void makeObstacleList();

#define TUNNEL_SIDES 10
#define SECTION_WIDTH 2.0
#define RADIUS 3.0
#define MAX_RINGS 250
#define ANGLE 0.4
#define SPEED 10