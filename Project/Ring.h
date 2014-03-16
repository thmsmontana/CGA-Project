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
};

void setupRingLists();