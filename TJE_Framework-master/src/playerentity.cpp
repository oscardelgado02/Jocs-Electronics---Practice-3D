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
		//cam->center = model.getTranslation() + Vector3(0.0f, 2.2f, -10.0f);
		model.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		cam->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		cam->rotate(Input::mouse_delta.y * 0.005f, cam->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around

	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		
		Vector3 oldCamEye = cam->eye;
		cam->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (cam->eye.y != 2.2f) {
			cam->move(Vector3(0.0f, cam->eye.y - oldCamEye.y, 0.0f));
		}

	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		
		Vector3 oldCamEye = cam->eye;
		cam->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (cam->eye.y != 2.2f) {
			cam->move(Vector3(0.0f, cam->eye.y - oldCamEye.y, 0.0f));
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		
		Vector3 oldCamEye = cam->eye;
		cam->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (cam->eye.y != 2.2f) {
			cam->move(Vector3(0.0f, cam->eye.y - oldCamEye.y, 0.0f));
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		Vector3 oldCamEye = cam->eye;
		cam->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
		if (cam->eye.y != 2.2f) {
			cam->move(Vector3(0.0f, cam->eye.y - oldCamEye.y, 0.0f));
		}
	}
	
}