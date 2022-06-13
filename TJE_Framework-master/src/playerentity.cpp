#include "playerentity.h"
#include "game.h"
#include "world.h"

PlayerEntity::PlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {}

PlayerEntity::~PlayerEntity(){}

void PlayerEntity::render() {
	EntityMesh::render();
}

void PlayerEntity::update(float dt) {
	float speed = dt * 10.0f; //the speed is defined by the seconds_elapsed so it goes constant
	Vector3 playerVel = Vector3(0.0f, 0.0f, 0.0f);

	Camera* cam = Game::instance->camera;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		model.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		cam->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		cam->rotate(Input::mouse_delta.y * 0.005f, cam->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift

	if (Input::isKeyPressed(SDL_SCANCODE_W)) {
		playerVel = Vector3(0.0f, 0.0f, 1.0f * speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S)) {
		playerVel = Vector3(0.0f, 0.0f, -1.0f * speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A)) {
		playerVel = Vector3(1.0f * speed, 0.0f, 0.0f);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D)) {
		playerVel = Vector3(-1.0f * speed, 0.0f, 0.0f);
	}

	detectPlayerCollision(cam, dt, playerVel);
	moveFirstPersonCam(cam, playerVel);

	/*
	printf("Cam: %f,%f,%f\n", cam->eye.x, cam->eye.y, cam->eye.z);
	printf("Model: %f,%f,%f\n\n", this->model.getTranslation().x, this->model.getTranslation().y, this->model.getTranslation().z);
	*/
}

void PlayerEntity::moveFirstPersonCam(Camera* cam, Vector3 delta) {

	//cam Vector3
	Vector3 localDelta = cam->getLocalVector(delta);

	//change eye coordinates
	cam->eye.x = cam->eye.x - localDelta.x;
	cam->eye.z = cam->eye.z - localDelta.z;

	//change center coordinates
	cam->center.x = cam->center.x - localDelta.x;
	cam->center.z = cam->center.z - localDelta.z;

	//world coordinates cam Vector3
	Vector3 modelCoordinates = Vector3(-localDelta.x, 0.0f, -localDelta.z);

	//change model coordinates
	this->model.translateGlobal(modelCoordinates.x, 0.0f, modelCoordinates.z);

	cam->updateViewMatrix();
}

void PlayerEntity::detectPlayerCollision(Camera* cam, float dt, Vector3 playerVel) {
	
	/*
	Game* g = Game::instance;
	World* world = World::getInstance();
	Vector3 dir = Vector3(cam->center.x, 0.0f, cam->center.z);
	Vector3 rayOrigin = cam->eye;

	for (size_t i = 0; i < world->getEntitiesSize(); i++) {
		EntityMesh* entity = (EntityMesh*) world->getEntity(i);
		Vector3 pos; Vector3 normal;

		if (entity->mesh->testRayCollision(entity->model, rayOrigin, dir, pos, normal)) {
			return true;
		}
	}

	return false;
	*/
	World* world = World::getInstance();

	Vector3 playerPos = this->model.getTranslation();
	Vector3 nextPos = playerPos + playerVel;
	Vector3 character_center = nextPos + Vector3(0, 0.5f, 0);

	for (size_t i = 0; i < world->getEntitiesSize(); i++) {
		EntityMesh* entity = (EntityMesh*)world->getEntity(i);
		Vector3 coll; Vector3 normal;

		if (!entity->mesh->testSphereCollision(entity->model, character_center, 0.5f, coll, normal)) {
			continue;
		}

		Vector3 push_away = normalize(coll - character_center) * dt;
		nextPos = playerPos - push_away;

		//change eye coordinates
		cam->eye.x = cam->eye.x - push_away.x;
		cam->eye.z = cam->eye.z - push_away.z;

		//change center coordinates
		cam->center.x = cam->center.x - push_away.x;
		cam->center.z = cam->center.z - push_away.z;
	}
}