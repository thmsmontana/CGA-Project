#include <list>
#include "Ring.h"

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

