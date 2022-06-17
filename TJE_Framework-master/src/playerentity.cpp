#include "playerentity.h"
#include "game.h"
#include "world.h"

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

	Camera* cam = Game::instance->camera;

	//mouse input to rotate the cam
	model.rotate(Input::mouse_delta.x * 0.002f, Vector3(0.0f, -1.0f, 0.0f));
	cam->rotate(Input::mouse_delta.x * 0.002f, Vector3(0.0f, -1.0f, 0.0f));
	cam->rotate(Input::mouse_delta.y * 0.002f, cam->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));

	/*
	if ((Input::mouse_state))
	{
		model.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		cam->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		cam->rotate(Input::mouse_delta.y * 0.005f, cam->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}
	*/

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) && cooldown_enable) {
		speed *= 3; //move faster with left shift
		cooldown -= dt;
	}
	else {
		if (cooldown < 3.0f) {
			cooldown += dt;
		}
	}

	//cooldown of running
	if (cooldown < 0.0f) {
		cooldown_enable = false;
	}
	if (cooldown > 2.0f) {
		cooldown_enable = true;
	}

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
	//detectPlayerCollision2(cam, dt, playerVel, speed);
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

	World* world = World::getInstance();
	Vector3 playerPos = model.getTranslation();
	Vector3 nextPos = playerPos + playerVel;

	//calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
	Vector3 character_center = nextPos + Vector3(0, 0.6f, 0);

	//para cada objecto de la escena...

	for (size_t i = 0; i < world->getEntitiesSize(); i++) {
		
		Entity* currentEntity = world->getEntity(i);
		EntityMesh* entityMesh = (EntityMesh*) currentEntity;

		Vector3 coll;
		Vector3 collnorm;

		if (currentEntity->name.compare("player") == 0) {
			continue;
		}

		//comprobamos si colisiona el objeto con la esfera (radio 3)
		if ((entityMesh->mesh->testSphereCollision(entityMesh->model, character_center, 0.5f, coll, collnorm)) == false)
			continue; //si no colisiona, pasamos al siguiente objeto

			//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * playerVel;
		nextPos = playerPos - push_away; //move to previous pos but a little bit further

		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		nextPos.y = 0;

		//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
		//velocity = reflect(velocity, collnorm) * 0.95;

		//change eye coordinates
		cam->eye.x = cam->eye.x - push_away.x;
		cam->eye.z = cam->eye.z - push_away.z;

		//change center coordinates
		cam->center.x = cam->center.x - push_away.x;
		cam->center.z = cam->center.z - push_away.z;

		//world coordinates cam Vector3
		Vector3 modelCoordinates = Vector3(-push_away.x, 0.0f, -push_away.z);

		//change model coordinates
		this->model.translateGlobal(modelCoordinates.x, 0.0f, modelCoordinates.z);

		cam->updateViewMatrix();
	}
}

void PlayerEntity::detectPlayerCollision2(Camera* cam, float dt, Vector3 playerVel, float speed) {

	World* world = World::getInstance();
	Vector3 playerPos = model.getTranslation();
	Vector3 nextPos = playerPos + (playerVel);

	//calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
	Vector3 character_center = nextPos + Vector3(0, 0.95f, 0);
	Vector3 character_center_current = playerPos + Vector3(0, 0.95f, 0);

	//para cada objecto de la escena...

	for (size_t i = 0; i < world->getEntitiesSize(); i++) {

		Entity* currentEntity = world->getEntity(i);
		EntityMesh* entityMesh = (EntityMesh*)currentEntity;

		Vector3 coll;
		Vector3 collnorm;

		if (currentEntity->name.compare("player") == 0) {
			continue;
		}

		//comprobamos si colisiona el objeto con la esfera (radio 3)
		if (!(entityMesh->mesh->testSphereCollision(entityMesh->model, character_center_current, 0.5f, coll, collnorm)) || !(entityMesh->mesh->testSphereCollision(entityMesh->model, character_center, 0.5f, coll, collnorm)))
			continue; //si no colisiona, pasamos al siguiente objeto

			//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * playerVel;
		nextPos = playerPos - push_away; //move to previous pos but a little bit further

		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		nextPos.y = 0;

		//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
		//velocity = reflect(velocity, collnorm) * 0.95;

		//change eye coordinates
		cam->eye.x = cam->eye.x - push_away.x;
		cam->eye.z = cam->eye.z - push_away.z;

		//change center coordinates
		cam->center.x = cam->center.x - push_away.x;
		cam->center.z = cam->center.z - push_away.z;

		//world coordinates cam Vector3
		Vector3 modelCoordinates = Vector3(-push_away.x, 0.0f, -push_away.z);

		//change model coordinates
		this->model.translateGlobal(modelCoordinates.x, 0.0f, modelCoordinates.z);

		cam->updateViewMatrix();
	}
}