#include "world.h"

World* World::instance = NULL;

World::World() {

}

World* World::getInstance() {
	if (!instance)
		instance = new World;
	return instance;
}

Entity* World::getEntity(int idx) {
	return entities[idx];
}

void World::addEntity(Entity* e) {
	return entities.push_back(e);
}

void World::addEntityMesh(EntityMesh* e) {
	return entities.push_back(e);
}

void World::renderEntities() {
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->render();
	}
}