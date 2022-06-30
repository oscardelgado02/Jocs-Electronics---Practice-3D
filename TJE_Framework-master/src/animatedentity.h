#pragma once
#include "entitymesh.h"
#include "animation.h"
#include "game.h"

class AnimatedEntity : public EntityMesh
{
	Mesh* animated_mesh;
	Animation* current_animation;
	std::vector<Animation*> animations;

public:
	AnimatedEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Mesh* animated_mesh, Animation* idle_animation); //constructor

	void render();
	void update(float dt);
	void addAnimation(Animation* animation);
	void changeAnimation(int idx);
	void multiPass(std::vector<Light*> lights, Camera* camera);
};

