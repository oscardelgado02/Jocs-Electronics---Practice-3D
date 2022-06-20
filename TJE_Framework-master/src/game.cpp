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
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 20.0f;
FBO* fbo = NULL;

Game* Game::instance = NULL;

Mesh* grassmesh = NULL;
Texture* grasstexture = NULL;
const int grass_width = 50;
const int grass_height = 50;
float padding = 2.5f;

//World instance
World* world;

//Map variable
Map levelMap = Map();

//Ambience sound
Sound ambience_sound;

//stages variables
std::vector<Stage*> stages;
STAGE_ID currentStage = STAGE_ID::INTRO;

void initGrass() { //para poner un suelo de césped
	for (size_t i = 0; i < grass_width; i++) {
		for (size_t j = 0; j < grass_height; j++) {
			Matrix44 model;
			model.translate((i * padding) - grass_width, 0.0f, (j * padding) - grass_height);

			world->addEntityMesh("grass", model, grassmesh, grasstexture, shader, Vector4(1, 1, 1, 1));
		}
	}

	//Matrix44 model;
	//world->addEntityMesh("grass", model, grassmesh, grasstexture, Shader::Get("data/shaders/basic.vs", "data/shaders/dark.fs"), Vector4(1, 1, 1, 1));
}

void initSky() { //para poner un cielo
	Matrix44 model;
	Mesh* sky_mesh = NULL;
	Texture* sky_texture = NULL;
	sky_texture = new Texture();
	sky_texture->load("data/sky/stars.tga");
	sky_mesh = Mesh::Get("data/sky/box.ase");

	world->addEntityMesh("sky", model, sky_mesh, sky_texture, shader, Vector4(1, 1, 1, 1));
}

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
	camera->lookAt(Vector3(0.f, 1.9f, 22.0f),Vector3(0.f,1.9f,21.0f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	//grass mesh and texture init
	grasstexture = new Texture();
	grasstexture->load("data/texture_samurai.tga");
	grassmesh = Mesh::Get("data/SM_Env_Tile_Grass_01_25.obj");
	//grassmesh = new Mesh();
	//grassmesh->createPlane(100);

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/dark.fs");

	//create world
	world = World::getInstance();

	//init map
	//map.loadMap("data/level/level1.txt");
	levelMap.loadMap("data/level/level1PROVISIONAL.txt");
	initGrass();
	initSky();
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//ambience sound
	ambience_sound = Sound("data/sounds/ambience/mixkit-creepy-tomb-ambience-2500.wav", true);
	ambience_sound.PlayGameSound();

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
	if (Input::isKeyPressed(SDL_SCANCODE_P)) {
		camera->eye = Vector3(camera->eye.x, camera->eye.y + 3.0f, camera->eye.z);
		camera->center = Vector3(camera->center.x, camera->center.y + 3.0f, camera->center.z);
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) { //used to skip stages
		int nextStageIndex = (((int)currentStage) + 1);
		if (nextStageIndex >= stages.size()) must_exit = true; else SetStage((STAGE_ID)nextStageIndex, &currentStage);
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
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

