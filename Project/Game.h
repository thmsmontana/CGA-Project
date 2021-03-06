
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
	void loadExternalTextures();
	int getScore();
	int getHP();

private:
	void handleCollision();
	int score;
	int hp;
	int age;
	float position;
	bool playing;
	int invul;
	clock_t previous_draw;
	Tunnel tunnel;
	Car car;
};

void setupLists();
void drawBackground();
void drawHealthPoints(int);

#define OBSTACLE_PROBABILITY 0.5
#define POSITION_INCREMENT 5
#define SHIP_ANGULAR_WIDTH 360/TUNNEL_SIDES
#define INVUL_TIME 1000
#define MAX_HP 5

#define FOVY 60
#define ASPECT 1
#define ZNEAR 1 //Do not touch
#define ZFAR 500

#define PATH_BACKGROUND "textures/stars.bmp"
#define PATH_HEALTH_POINTS "textures/heart.bmp"
