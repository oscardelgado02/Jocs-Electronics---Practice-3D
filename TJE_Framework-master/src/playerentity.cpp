#include "playerentity.h"
#include "game.h"

PlayerEntity::PlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {}

PlayerEntity::~PlayerEntity(){}

void PlayerEntity::render() {
	EntityMesh::render();
}

void PlayerEntity::update(float dt) {
	float speed = dt * 20.0f; //the speed is defined by the seconds_elapsed so it goes constant


	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		model.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
	}

	//async input to move the camera around

	Camera* cam = Game::instance->camera;

	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		model.translate(0.0f, 0.0f, 1.0f * speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		model.translate(0.0f, 0.0f, -1.0f * speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		model.translate(1.0f * speed, 0.0f, 0.0f);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		model.translate(-1.0f * speed, 0.0f, 0.0f);
	}
}