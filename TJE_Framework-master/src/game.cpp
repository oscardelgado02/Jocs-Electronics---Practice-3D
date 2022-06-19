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
#include "pathfinders.h"

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

//Pathfinding variables
int startx;
int starty;
int targetx;
int targety;
uint8* grid;
int output[100];
int W = 100;
int H = 100;
float tileSizeX = 10.0f;
float tileSizeY = 10.0f;

//World instance
World* world;

//Map variable
Map levelMap = Map();

//Ambience sound
Sound ambience_sound;

void initGrass() { //para poner un suelo de césped
	for (size_t i = 0; i < grass_width; i++) {
		for (size_t j = 0; j < grass_height; j++) {
			Matrix44 model;
			model.translate((i * padding) - grass_width, 0.0f, (j * padding) - grass_height);

			world->addEntityMesh("grass", model, grassmesh, grasstexture, shader, Vector4(1, 1, 1, 1));
		}
	}
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


	//the map info should be an array W*H of bytes where 0 means block, 1 means walkable
	grid = new uint8[W * H];

	for (size_t i = 0; i < W * H; i++) {
		grid[i] = 1;
	}


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

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/dark.fs");

	//create world
	world = World::getInstance();

	//init map
	//map.loadMap("data/level/level1.txt");
	levelMap.loadMap("data/level/level1PROVISIONAL.txt");
	initGrass();

	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//ambience sound
	ambience_sound = Sound("data/sounds/ambience/mixkit-creepy-tomb-ambience-2500.wav", true);
	ambience_sound.PlayGameSound();
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

	//render entities
	world->renderEntities();

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
		camera->eye = Vector3(camera->eye.x, camera->eye.y, camera->eye.z + 3.0f);
		camera->center = Vector3(camera->center.x, camera->center.y, camera->center.z + 3.0f);
	}

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();

	//update entities
	world->updateEntities(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; BASS_Free(); break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break;
		case SDLK_SPACE: world->deleteAllEntities(); break; //delete all entities

		case SDLK_4: {
			Vector2 mouse = Input::mouse_position;
			Game* g = Game::instance;
			Vector3 dir = camera->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
			Vector3 rayOrigin = camera->eye;

			Vector3 spawnPos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), rayOrigin, dir);
			startx = clamp(spawnPos.x / tileSizeX, 0, W);
			starty = clamp(spawnPos.z / tileSizeY, 0, H);

			break; //delete all entities
		}

		case SDLK_5: {
			Vector2 mouse = Input::mouse_position;
			Game* g = Game::instance;
			Vector3 dir = camera->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
			Vector3 rayOrigin = camera->eye;

			Vector3 spawnPos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), rayOrigin, dir);
			targetx = clamp(spawnPos.x / tileSizeX, 0, W);
			targety = clamp(spawnPos.z / tileSizeY, 0, H);

			//we call the path function, it returns the number of steps to reach target, otherwise 0
			int path_steps = AStarFindPathNoTieDiag(
				startx, starty, //origin (tienen que ser enteros)
				targetx, targety, //target (tienen que ser enteros)
				grid, //pointer to map data
				W, H, //map width and height
				output, //pointer where the final path will be stored
				100); //max supported steps of the final path

			//check if there was a path
			if (path_steps != -1)
			{
				for (int i = 0; i < path_steps; ++i)
					std::cout << "X: " << (output[i] % W) << ", Y: " << floor(output[i] / W) << std::endl;
			}

			break; //delete all entities
		}

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

