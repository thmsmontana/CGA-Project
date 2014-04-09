
#include <time.h>

#include "Tunnel.h"
#include "Car.h"
#pragma once
class Game
{
public:
	Game();
	virtual ~Game();
private:
	int score;
	int age;
	float position;
	bool playing;
	clock_t previous_draw;
	Tunnel tunnel;
	Car car;
public:
	void draw();
	void playPause();
	void left();
	void right();
};

void setupLists();