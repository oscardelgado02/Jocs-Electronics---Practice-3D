#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "world.h"
#include "map.h"
#include "sound.h"
#include "lightmanager.h"
#include "stage.h"
#include "ia.h"

#include <cmath>

//some globals
Shader* shader = NULL;
float angle = 0;
float mouse_speed = 20.0f;
FBO* fbo = NULL;

Game* Game::instance = NULL;

float padding = 2.5f;

//World instance
World* world;

//Map variables
Map levelMap = Map();
char* map_path = "data/level/leveldefinitivo.txt";

//Ambience sound
Sound* sound;

//stages variables
std::vector<Stage*> stages;
STAGE_ID currentStage = STAGE_ID::INTRO;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = true;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//sound init
	//Inicializamos BASS al arrancar el juego (id_del_device, muestras por segundo, ...)
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		//error abriendo la tarjeta de sonido...
	}

	//create our camera
	camera = new Camera();

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/dark.fs");

	//create world
	world = World::getInstance();

	//init map
	levelMap.loadMap(map_path);

	//init sound
	sound = Sound::getInstance();
	sound->InitSound();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//ambience sound
	sound->PlayGameSound(AMBIENCE);

	//Stages initialization
	InitStages(&stages);
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//STAGE RENDER
	GetStage(stages, currentStage)->Render();

	//Draw the floor grid
	//drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		if ((int)currentStage < (int)STAGE_ID::PLAY) { //used to skip intro and tutorial stage
			int nextStageIndex = (((int)currentStage) + 1);
			SetStage((STAGE_ID)nextStageIndex, &currentStage);
		}
		if ((int)currentStage == (int)STAGE_ID::DEAD) { //used to restart the level if dead
			SetStage(STAGE_ID::PLAY, &currentStage);
			levelMap.restartEntities(map_path);
			GetStage(stages, currentStage)->camera = world->getPlayerEntityCamera();
		}
		if ((int)currentStage == (int)STAGE_ID::END) { //used to close the game if end
			must_exit = true; BASS_Free();
		}
	}

	/*
	//debug mode
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) { //used to skip stages
		int nextStageIndex = (((int)currentStage) + 1);
		if (nextStageIndex >= stages.size()) must_exit = true; else SetStage((STAGE_ID)nextStageIndex, &currentStage);
	}
	*/
	
	//check if player is dead
	if (world->checkIfDead()) {
		SetStage(STAGE_ID::DEAD, &currentStage);
	}

	//check if player scaped
	if (world->checkIfScape()) {
		SetStage(STAGE_ID::END, &currentStage);
	}
	
	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();

	//STAGE UPDATE
	GetStage(stages, currentStage)->Update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; BASS_Free(); break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	
	//camera->aspect =  width / (float)height;

	//resize all cameras
	for (int i = 0; i < stages.size(); i++) {
		stages[i]->camera->aspect = width / (float)height;
	}

	window_width = width;
	window_height = height;
}

