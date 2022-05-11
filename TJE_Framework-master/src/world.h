#pragma once

#include "utils.h"
#include "entity.h"
#include "entitymesh.h"

class World
{
	static World* instance;

	std::vector<Entity*> entities;

	//constructor
	World();

public:

	static World* getInstance();

	//methods
	Entity* getEntity(int idx);
	void addEntity(Entity* e);
	void addEntityMesh(EntityMesh* e);
	void renderEntities();

};

