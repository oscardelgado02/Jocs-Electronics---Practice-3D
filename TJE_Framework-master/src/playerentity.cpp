#include "playerentity.h"
#include "game.h"

PlayerEntity::PlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {}

PlayerEntity::~PlayerEntity(){}

void PlayerEntity::render() {
	EntityMesh::render();
}

void PlayerEntity::update(float dt) {
	float speed = dt * 10.0f; //the speed is defined by the seconds_elapsed so it goes constant

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
		moveFirstPersonCam(cam, Vector3(0.0f, 0.0f, 1.0f * speed));
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S)) {
		moveFirstPersonCam(cam, Vector3(0.0f, 0.0f, -1.0f * speed));
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A)) {
		moveFirstPersonCam(cam, Vector3(1.0f * speed, 0.0f, 0.0f));
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D)) {
		moveFirstPersonCam(cam, Vector3(-1.0f * speed, 0.0f, 0.0f));
	}

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