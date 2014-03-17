#include <list>

#pragma once
class Tunnel
{
public:
	Tunnel();
	virtual ~Tunnel();
	void draw(float t);
private:
	float offset;
	std::list <Ring *> rings;
	float prevOffset;
};

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


void setupLists();