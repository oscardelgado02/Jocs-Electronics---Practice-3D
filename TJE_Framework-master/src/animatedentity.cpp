#include "animatedentity.h"

AnimatedEntity::AnimatedEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Mesh* animated_mesh, Animation* idle_animation) : EntityMesh(name, model, mesh, texture, shader, color) {
	this->animated_mesh = animated_mesh;
	this->current_animation = idle_animation;
	this->animations.push_back(current_animation);
}

void AnimatedEntity::render() {
	//get the last camera that was activated
	Camera* camera = Camera::current;
	Matrix44 model = this->model;
	std::vector<Light*> lights = LightManager::getInstance()->getLights();

	if (shader && checkFrustum())
	{
		//enable shader
		shader->enable();

		//multipass
		current_animation->assignTime(Game::instance->time);
		multiPass(lights, camera);

		//disable shader
		shader->disable();
	}
}

void AnimatedEntity::update(float dt) {}

void AnimatedEntity::addAnimation(Animation* animation) {
	animations.push_back(animation);
}

void AnimatedEntity::changeAnimation(int idx) {
	current_animation = animations[idx];
}

void AnimatedEntity::multiPass(std::vector<Light*> lights, Camera* camera) {
	//allow to render pixels that have the same depth as the one in the depth buffer
	glDepthFunc(GL_LEQUAL);

	//set blending mode to additive
	//this will collide with materials with blend...
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int i = 0; i < lights.size(); ++i)
	{
		//first pass doesn't use blending
		if (i == 0)
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);

		//pass the light data to the shader
		setUniforms(lights[i], camera);
		//do the draw call
		this->animated_mesh->renderAnimated(GL_TRIANGLES, &current_animation->skeleton);
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS); //as default

}