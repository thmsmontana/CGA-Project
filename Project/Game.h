
#include <time.h>

#include "Tunnel.h"
#include "Car.h"
#pragma once
class Game
{
public:
	Game();
	virtual ~Game();
	void draw();
	void playPause();
	void left();
	void right();
	void update();
	int getScore();
private:
	void handleCollision();
	int score;
	int age;
	float position;
	bool playing;
	int invul;
	clock_t previous_draw;
	Tunnel tunnel;
	Car car;
};

void setupLists();

#define OBSTACLE_PROBABILITY 0.5
#define POSITION_INCREMENT 2
#define SHIP_ANGULAR_WIDTH 360/TUNNEL_SIDES
#define INVUL_TIME 1000