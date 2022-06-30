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

	distanceEnemyToPlayer(); //to calculate enemy distance to player
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

	return getPlayerEntity()->getPlayerCamera();
}

PlayerEntity* World::getPlayerEntity() {

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->name.compare("player") == 0) {
			PlayerEntity* player = (PlayerEntity*)entities[i];
			return player;
		}
	}
}

std::vector<Entity*> World::getEnemyEntities() {

	std::vector<Entity*> enemies;

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->name.compare("enemy") == 0) {
			enemies.push_back((EnemyEntity*)entities[i]);
			return enemies;
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

void World::distanceEnemyToPlayer() {
	Vector3 playerPos = World::getInstance()->getPlayerEntity()->getPosition();
	std::vector<Entity*> enemies = getEnemyEntities();

	for (int i = 0; i < enemies.size(); i++) {
		((EnemyEntity*)enemies[i])->setTargetPlayer(playerPos);
	}
}

bool World::checkIfDead() {
	Vector3 player = getPlayerEntity()->getPosition();
	bool x_status = false;
	bool z_status = false;

	float size = 1.5f;

	std::vector<Entity*> enemies = getEnemyEntities();

	for (int i = 0; i < enemies.size(); i++) {
		
		Vector3 enemy = enemies[i]->getPosition();
			
		x_status = x_status || ((enemy.x - size < player.x) && (enemy.x + size > player.x));
		z_status = z_status || ((enemy.z - size < player.z) && (enemy.z + size > player.z));
	}

	return x_status && z_status;
}

Camera* World::getKillerCam() {

	Game* g = Game::instance; //Game instance
	float dist_factor = (1.0 / 2);

	//we get the player and all the enemies
	Vector3 player = getPlayerEntity()->getPosition();
	std::vector<Entity*> enemies = getEnemyEntities();

	//variables to check and save the nearest enemy
	Vector3 nearest_enemy = Vector3(0.0,0.0,0.0);
	Vector3 forward = Vector3(0.0,0.0,1.0); //where the enemy is watching
	float distance = 10000.0f;

	for (int i = 0; i < enemies.size(); i++) {

		Vector3 enemy = enemies[i]->getPosition();
		forward = enemies[i]->model.rotateVector(Vector3(0.0, 0.0, 1.0)).normalize()*dist_factor;

		if (distance > nearest_enemy.distance(player)) {
			distance = nearest_enemy.distance(player);
			nearest_enemy = enemy;
		}
	}

	//final sets
	nearest_enemy.y = 3.3f;

	Camera* cam = new Camera();
	cam->lookAt(nearest_enemy + forward, nearest_enemy, Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	cam->setPerspective(70.f, g->window_width / (float)g->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	return cam;
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

void World::restartEntity(Vector3 position, Vector3 rotation, Vector3 scale, int idx) {

	Matrix44 model;

	model.translate(position.x, position.y, position.z);

	//APPLY ROTATION
	model.rotate((PI / 180) * rotation.x, Vector3(0, 1, 0)); //rotate in x
	model.rotate((PI / 180) * rotation.y, Vector3(0, 0, 1)); //rotate in y
	model.rotate((PI / 180) * rotation.z, Vector3(1, 0, 0)); //rotate in z

	model.scale(10.0f * scale.x, 10.0f * scale.y, 10.0f * scale.z);

	entities[idx]->setModel(model);
}