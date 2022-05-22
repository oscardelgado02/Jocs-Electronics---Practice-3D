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

#include <cmath>

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;

Game* Game::instance = NULL;

const int samurai_width = 200;
const int samurai_height = 200;
float padding = 20.0f;

//Frustum variables
float lodDistance = 200.0f;
float no_render_dist = 1000.0f;

//World instance
World* world;

//Map variable
Map map = Map();

//method to init entities
/*
void initEntities() {
	for (size_t i = 0; i < samurai_width; i++) {
		for (size_t j = 0; j < samurai_height; j++) {
			Matrix44 model;
			model.translate(i * padding, 0.0f, j * padding);

			world->addEntityMesh("samurai", model, mesh, texture, shader, Vector4(1, 1, 1, 1));
		}
	}
}
*/

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
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
 	texture->load("data/texture.tga");
	//texture->load("data/Character_Samurai_Warrior_White_3.mtl");

	// example of loading Mesh from Mesh Manager
	//mesh = Mesh::Get("data/box.ASE");
	mesh = Mesh::Get("data/Character_Samurai_Warrior_White_3.obj");

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//create world
	world = World::getInstance();

	//init map
	map.loadMap("data/level/level1PROVISIONAL.txt");

	//add entities to world
	//Matrix44 model;
	//world->addEntityMesh("cube", model, mesh, texture, shader, Vector4(1, 1, 1, 1));
	//world->addPlayerEntity("player", model, mesh, texture, shader, Vector4(1, 1, 1, 1));
	//initEntities();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

/*void renderSamurais() {

	//enable shader
	shader->enable();

	Camera* cam = Game::instance->camera;
	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_time", time);

	for (size_t i = 0; i < samurai_width; i++) {
		for (size_t j = 0; j < samurai_height; j++) {
			Matrix44 model;
			model.translate(i * padding, 0.0f, j * padding);

			Vector3 samuraiPos = model.getTranslation();

			Vector3 camPos = cam->eye;
			float dist = samuraiPos.distance(camPos);

			if (dist > no_render_dist) {
				continue;
			}

			//Mesh* mesh = Mesh::Get("data/Character_Samurai_Warrior_White_3.obj");

			if (dist < lodDistance){
				//set la calidad a mas pocha de los modelos
			}

			BoundingBox worldAABB = transformBoundingBox(model, mesh->box);
			if (!cam->testBoxInFrustum(worldAABB.center, worldAABB.halfsize)) {
				continue;
			}

			shader->setUniform("u_model", model);
			mesh->render(GL_TRIANGLES);
		}
	}

	//disable shader
	shader->disable();
}
*/


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
   
	//create model matrix for cube
	/*
	Matrix44 m;
	m.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

	

	if(shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1,1,1,1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_time", time);

		//do the draw call
		mesh->render( GL_TRIANGLES );

		//disable shader
		shader->disable();
	}
	*/

	//render entities
	world->renderEntities();

	///////////PREUBAS//////////
	//renderSamurais();
	////////////////////////////

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around
	if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);

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
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break;
		case SDLK_SPACE: world->deleteAllEntities(); break; //delete all entities
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

