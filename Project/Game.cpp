#include <cstdlib>
#include <iostream>
#include <time.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "Game.h"
#include "Car.h"
#include <windows.h>




using namespace std;

static unsigned int base; // Displays the lists base index.

Game::Game()
{
	age = 0;
	score = 0;
	playing = false;
	tunnel = Tunnel(OBSTACLE_PROBABILITY);
	car = Car();
	previous_draw = 0;
	position = 0.0;
	setupLists();
	invul = 0;
}


Game::~Game()
{
}

void Game::update()
{
	if (playing)
		if (previous_draw != 0)
		{
			age += clock() - previous_draw;

			if (invul <= 0)
				invul = 0;
			else
				invul -= clock() - previous_draw;

		}
			

	if (tunnel.hasObstacleAtPosition(position) && invul == 0) 
		handleCollision();
}


void Game::handleCollision() 
{
	invul = INVUL_TIME;

	//PlaySound(TEXT(collision.wav"), NULL, SND_ASYNC | SND_APPLICATION | SND_LOOP);

	score -= 10;
	if (score < 0) score = 0;
}



void Game::draw()
{
	glPushMatrix();
	glRotatef(position + 90, 0.0, 0.0, - 1.0);
	tunnel.draw(age);
	glPopMatrix();

	//car.draw();

	previous_draw = clock();
}


void Game::playPause()
{
	playing = !playing;
}


void Game::right()
{
	position += POSITION_INCREMENT;
	if (position >= 360.0) position -= 360.0;
}


void Game::left()
{
	position -= POSITION_INCREMENT;
	if (position < 0.0) position += 360.0;
}



int Game::getScore()
{
	return score;
}


void setupLists()
{
	setupTunnelLists();
	setupShipList();
}