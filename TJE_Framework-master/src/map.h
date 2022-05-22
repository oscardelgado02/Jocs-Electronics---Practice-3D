#pragma once

#include "utils.h"
#include "world.h"
#include "texture.h"

//index of parameters
#define TYPE 0
#define MESH 1
#define TEXTURE 2
#define POSITION 3
#define ROTATION 4

//index of type of object
#define PLAYER 0
#define ENEMY 1
#define WALL 2

class Map
{
public:
	World* world;

	//constructor
	Map();

	//methods
	void Map::loadEntity(int type, Mesh* mesh, Texture* texture, Vector3 position, Vector3 rotation);
	void loadMap(char* path);
};

