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
	uint8* grid;
	int W;
	int H;
	float tileSizeX;
	float tileSizeY;
	
	//constructor
	IA();

	Vector3 sendStep(Vector3 position);
};