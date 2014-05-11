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
#include <fstream>




using namespace std;

static unsigned int base; // Displays the lists base index.

static unsigned int texture[2]; // Array of texture indices.




// Struct of bitmap file.
struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char *data;
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
	BitMapFile *bmp = new BitMapFile;
	unsigned int size, offset, headerSize;

	// Read input file name.
	ifstream infile(filename.c_str(), ios::binary);

	// Get the starting point of the image data.
	infile.seekg(10);
	infile.read((char *)&offset, 4);

	// Get the header size of the bitmap.
	infile.read((char *)&headerSize, 4);

	// Get width and height values in the bitmap header.
	infile.seekg(18);
	infile.read((char *)&bmp->sizeX, 4);
	infile.read((char *)&bmp->sizeY, 4);

	// Allocate buffer for the image.
	size = bmp->sizeX * bmp->sizeY * 24;
	bmp->data = new unsigned char[size];

	// Read bitmap data.
	infile.seekg(offset);
	infile.read((char *)bmp->data, size);

	// Reverse color from bgr to rgb.
	int temp;
	for (GLuint i = 0; i < size; i += 3)
	{
		temp = bmp->data[i];
		bmp->data[i] = bmp->data[i + 2];
		bmp->data[i + 2] = temp;
	}

	return bmp;
}

// Load external textures.
void Game::loadExternalTextures()
{

	// Create texture index array.
	glGenTextures(2, texture);

	// Local storage for bmp image data.
	BitMapFile *image[2];

	// Load the textures.
	image[0] = getBMPData(PATH_BACKGROUND);
	image[1] = getBMPData(PATH_HEALTH_POINTS);


	// Activate texture index texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Specify an image as the texture to be bound with the currently active texture index.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


	// Activate texture index texture[1]. 
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Specify an image as the texture to be bound with the currently active texture index.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);

	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}




void drawBackground()
{

	// Activate a texture.
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Map the texture onto a square polygon.
	float ymax = (ZFAR * tan(FOVY)) * 2;
	float diag = ymax * sqrt(2);
	ymax = diag ;
	float xmax = ymax;

	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-xmax, -ymax, -ZFAR + 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(xmax, -ymax, -ZFAR + 10);
	glTexCoord2f(1.0, 1.0); glVertex3f(xmax, ymax, -ZFAR + 10);
	glTexCoord2f(0.0, 1.0); glVertex3f(-xmax, ymax, -ZFAR + 10);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawHealthPoints(int hp)
{

	// Activate a texture.
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Map the texture onto a square polygon.
	float ymax = (ZNEAR * tan(FOVY)) * 2;
	float diag = ymax * sqrt(2);
	ymax = diag/8;
	float xmax = ymax;

	

	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);

	// position the Health points on the top right corner
	float yPosHP = 0.38;
	float xPosHP = 1.63;
	glTranslatef(xPosHP, -yPosHP, 0.0);

	glPushMatrix();
	for (int i = 0; i < hp; i++)
	{
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0); glVertex3f(-xmax, -ymax, -ZNEAR - 1);
		glTexCoord2f(1.0, 0.0); glVertex3f(xmax, -ymax, -ZNEAR - 1);
		glTexCoord2f(1.0, 1.0); glVertex3f(xmax, ymax, -ZNEAR - 1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-xmax, ymax, -ZNEAR - 1);
		glEnd();
		glTranslatef(-xmax * 2, 0.0, 0.0);
	}
	glPopMatrix();

	glTranslatef(-xPosHP, yPosHP, 0.0);

	glDisable(GL_TEXTURE_2D);

	
}

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
	hp = MAX_HP;

	loadExternalTextures();
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

int Game::getHP()
{
	return hp;
}

void Game::handleCollision() 
{
	invul = INVUL_TIME;

	//PlaySound(TEXT(collision.wav"), NULL, SND_ASYNC | SND_APPLICATION | SND_LOOP);
	hp--;
	if (hp == 0)
	{
		playing = false;
	}
		

	score -= 10;
	if (score < 0) score = 0;
}



void Game::draw()
{
	drawHealthPoints(getHP());
	glPushMatrix();
	glRotatef(position + 90, 0.0, 0.0, - 1.0);

	drawBackground();

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