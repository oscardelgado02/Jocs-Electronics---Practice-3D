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

void World::addEnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) {
	entities.push_back(new EnemyEntity(name, model, mesh, texture, shader, color));
}

void World::addWallEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) {
	entities.push_back(new WallEntity(name, model, mesh, texture, shader, color));
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

int World::getEntitiesSize() {
	return entities.size();
}

Camera* World::getPlayerEntityCamera() {

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->name.compare("player") == 0) {
			PlayerEntity* player = (PlayerEntity*)entities[i];
			return player->getPlayerCamera();
		}
	}
}

PlayerEntity* World::getPlayerEntity() {

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->name.compare("player") == 0) {
			PlayerEntity* player = (PlayerEntity*)entities[i];
			return player;
		}
	}
}

WallEntity* World::getGoalEntity() {

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->name.compare("goal") == 0) {
			WallEntity* goal = (WallEntity*)entities[i];
			return goal;
		}
	}
}

bool World::checkIfScape() {
	Vector3 player = getPlayerEntity()->getPosition();
	Vector3 goal = getGoalEntity()->getPosition();

	float size = 1.5f;

	bool x_status = (goal.x - size < player.x) && (goal.x + size > player.x);
	bool z_status = (goal.z - size < player.z) && (goal.z + size > player.z);

	/*
	printf("Player: %f,%f,%f\n", player.x, player.y, player.z);
	printf("Goal: %f,%f,%f\n", goal.x, goal.y, goal.z);
	printf("Status x: %d, Status z: %d\n\n", x_status, z_status);
	*/

	return x_status && z_status;
}