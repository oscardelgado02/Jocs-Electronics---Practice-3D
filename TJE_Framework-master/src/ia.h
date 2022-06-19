#pragma once

#include "pathfinders.h"
#include "utils.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include "texture.h"

class IA
{
public:
	//Pathfinding variables
	int startx;
	int starty;
	int targetx;
	int targety;
	uint8* grid;
	int output[100];
	int path_steps;
	int W = 100;
	int H = 100;
	float tileSizeX = 10.0f;
	float tileSizeY = 10.0f;
	
	//constructor
	IA();

	Vector3 sendStep(int idx);
	void setCenter(Vector3 position);
	void setTarget();
};