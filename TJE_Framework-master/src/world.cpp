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

void World::addEntity(std::string name, Matrix44 model) {
	entities.push_back(new Entity(name, model));
}

void World::deleteEntity(int idx) {
	entities[idx]->~Entity();
}

void World::addEntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) {
	entities.push_back(new EntityMesh(name, model, mesh, texture, shader, color));
}

void World::addPlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) {
	entities.push_back(new PlayerEntity(name, model, mesh, texture, shader, color));
}

void World::renderEntities() {
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->render();
	}
}

void World::updateEntities(float dt) {
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->update(dt);
	}
}

void World::deleteAllEntities() {
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->~Entity();
	}
}