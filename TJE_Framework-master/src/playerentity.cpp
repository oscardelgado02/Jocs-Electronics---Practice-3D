#include "playerentity.h"
#include "game.h"
#include "world.h"
#include "bass.h"

PlayerEntity::PlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {
	cooldown = 3.0f;
	cooldown_enable = true;
}

PlayerEntity::~PlayerEntity(){}

void PlayerEntity::render() {
	//EntityMesh::render(); //para que no se vea el player
}

void PlayerEntity::update(float dt) {
	float speed = dt * 2.0f; //the speed is defined by the seconds_elapsed so it goes constant
	Vector3 playerVel = Vector3(0.0f, 0.0f, 0.0f);
	bool status_footsteps = false; //control footsteps sound

	Camera* cam = Camera::current;

	//mouse input to rotate the cam
	model.rotate(Input::mouse_delta.x * 0.002f, Vector3(0.0f, -1.0f, 0.0f));
	cam->rotate(Input::mouse_delta.x * 0.002f, Vector3(0.0f, -1.0f, 0.0f));
	cam->rotate(Input::mouse_delta.y * 0.002f, cam->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));

	speed = runAndCooldown(dt, speed);

	//player movement
	if (Input::isKeyPressed(SDL_SCANCODE_W)) {
		playerVel = playerVel + Vector3(0.0f, 0.0f, 1.0f * speed);
		//play footsteps sound
		status_footsteps = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S)) {
		playerVel = playerVel + Vector3(0.0f, 0.0f, -1.0f * speed);
		//play footsteps sound
		status_footsteps = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A)) {
		playerVel = playerVel + Vector3(1.0f * speed, 0.0f, 0.0f);
		//play footsteps sound
		status_footsteps = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D)) {
		playerVel = playerVel + Vector3(-1.0f * speed, 0.0f, 0.0f);
		//play footsteps sound
		status_footsteps = true;
	}

	playSounds(status_footsteps);

	Vector3 nextPos = detectPlayerCollision(dt, playerVel);
	movePlayer(nextPos);

	//update lantern position
	LightManager::getInstance()->updatePlayerLight(this->model.getTranslation() + Vector3(0.0,0.9,0.0));
	
	/*
	printf("Cam: %f,%f,%f\n", cam->eye.x, cam->eye.y, cam->eye.z);
	printf("Cam center: %f,%f,%f\n", cam->center.x, cam->center.y, cam->center.z);
	printf("Model: %f,%f,%f\n\n", this->model.getTranslation().x, this->model.getTranslation().y, this->model.getTranslation().z);
	*/
}

void PlayerEntity::movePlayer(Vector3 nextPos) {

	Camera* cam = Camera::current;
	Vector3 playerPos = model.getTranslation();
	
	Vector3 movement = nextPos - playerPos;
	movement.y = 0.0f;

	//change model coordinates
	this->model.translateGlobal(movement.x, 0.0f, movement.z);
	cam->updateViewMatrix();
	cam->eye = cam->eye + movement;
	cam->center = cam->center + movement;
}

float PlayerEntity::runAndCooldown(float dt, float speed) {
	//async input to move the camera around
	/*if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) && cooldown_enable) {
		speed *= 3; //move faster with left shift
		cooldown -= dt;
	}
	else {
		if (cooldown < 5.0f) {
			cooldown += dt;
		}
	}*/

	//debug mode
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) {
		speed *= 10; //move faster with left shift
	}
	
	return speed;
}

void PlayerEntity::playSounds(bool status_footsteps) {

	Sound* sound = Sound::getInstance();

	//cooldown of running
	if (cooldown < 0.0f) {
		cooldown_enable = false;
		//play exhale sound
		sound->PlayGameSound(EXHALE);
	}
	if (cooldown > 2.0f) {
		cooldown_enable = true;
	}

	if (status_footsteps) {
		sound->PlayGameSound(FOOTSTEPS); //play
	}
}

Vector3 PlayerEntity::detectPlayerCollision(float dt, Vector3 playerVel) {

	Camera* cam = Camera::current;
	World* world = World::getInstance();
	Vector3 playerPos = model.getTranslation();
	Vector3 localDelta = cam->getLocalVector(playerVel);
	localDelta.y = 0;

	Vector3 nextPos = playerPos - localDelta;
	Vector3 character_center = nextPos + Vector3(0, 0.65f, 0);

	for (size_t i = 0; i < world->getEntitiesSize(); i++) {

		Entity* currentEntity = world->getEntity(i);
		EntityMesh* entityMesh = (EntityMesh*)currentEntity;

		Vector3 coll;
		Vector3 collnorm;

		float radius = 0.4f;

		if (currentEntity->name.compare("player") == 0) {
			continue;
		}

		if (currentEntity->name.compare("bridge") == 0) {
			radius = 0.2f;
		}

		//comprobamos si colisiona el objeto con la esfera (radio 3)
		if (!entityMesh->mesh->testSphereCollision(entityMesh->model, character_center, radius, coll, collnorm))
			continue; //si no colisiona, pasamos al siguiente objeto

		//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * dt * 10.0f;
		nextPos = playerPos - push_away; //move to previous pos but a little bit further

		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		nextPos.y = 0;
	}

	return nextPos;
}

Camera* PlayerEntity::getPlayerCamera() {

	//set position of the camera to the player pos

	Game* g = Game::instance;
	Camera* cam = new Camera();
	cam->lookAt(Vector3(model.getTranslation().x, 1.9f, model.getTranslation().z), Vector3(model.getTranslation().x, 1.9f, model.getTranslation().z - 1), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	cam->setPerspective(70.f, g->window_width / (float)g->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	return cam;
}