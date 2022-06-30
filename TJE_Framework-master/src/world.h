#pragma once

#include "utils.h"
#include "entity.h"
#include "entitymesh.h"
#include "playerentity.h"
#include "enemyentity.h"
#include "wallentity.h"
#include "light.h"

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
	void addEntity(std::string name, Matrix44 model);
	void deleteEntity(int idx);
	void addEntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
	void addPlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
	void addEnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Mesh* animated_mesh, Animation* idle_animation);
	void addWallEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
	void addAnimatedEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Mesh* animated_mesh, Animation* idle_animation);
	void renderEntities();
	void updateEntities(float dt);
	void deleteAllEntities();
	int getEntitiesSize();
	Camera* getPlayerEntityCamera();
	PlayerEntity* getPlayerEntity();
	std::vector<Entity*> World::getEnemyEntities();
	WallEntity* getGoalEntity();
	void distanceEnemyToPlayer();
	bool checkIfDead();
	Camera* getKillerCam();
	bool checkIfScape();
	void restartEntity(Vector3 position, Vector3 rotation, Vector3 scale, int idx);
};

