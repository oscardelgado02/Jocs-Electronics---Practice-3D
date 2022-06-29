#include "enemyentity.h"

EnemyEntity::EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {
	played_sound = false;
	sounds.push_back(new Sound("data/sounds/sustos/mixkit-cinematic-horror-trailer-long-sweep-561.wav"));
	sounds.push_back(new Sound("data/sounds/sustos/mixkit-horror-impact-773.wav"));
	//sounds.push_back(new Sound("data/sounds/sustos/mixkit-monster-footstep-1975.wav"));
	target_player = Vector3(0.0f, 0.0f, 0.0f); //initial value
	yaw = 0.0f;
	anim = Animation::Get("data/animaciones/bulin47.skanim");
}

EnemyEntity::~EnemyEntity() {}

void EnemyEntity::render() {
	//get the last camera that was activated
	Camera* camera = Camera::current;
	Matrix44 model = this->model;
	std::vector<Light*> lights = LightManager::getInstance()->getLights();


	if (shader && checkFrustum())
	{
		//enable shader
		shader->enable();

		//multipass
		anim->assignTime(Game::instance->time);
		multiPass(lights, camera);
		anim->skeleton.renderSkeleton(camera, model);

		//disable shader
		shader->disable();
	}
}

void EnemyEntity::update(float dt) {
	float speed = 2.0f;

	//Vector3 nextStep = ia.sendStep(model.getTranslation());

	movementAndRotation(dt, speed);
	playSounds();
	
}

float EnemyEntity::distanceToPlayer() {
	return target_player.distance(getPosition());
}

void EnemyEntity::setTargetPlayer(Vector3 target) {
	target_player = target;
}

void EnemyEntity::playSounds() {

	float jumpscare1_dist = 10.0f;
	float jumpscare2_dist = 5.0f;
	float step_distance = 20.0f;

	//sound of enemy
	if (this->checkFrustum() && !played_sound) {

		if (distanceToPlayer() < jumpscare2_dist) {
			sounds[JUMPSCARE2]->PlayGameSound();
			played_sound = true;
		}
		else if (distanceToPlayer() < jumpscare1_dist) {
			sounds[JUMPSCARE1]->PlayGameSound();
			played_sound = true;
		}
	}

	/*
	if ((distance_to_player < step_distance) && !played_sound) {
		sounds[FOOTSTEP_MONSTER]->PlayGameSound();
	}
	*/
}

void EnemyEntity::movementAndRotation(float dt, float speed) {

	float max_distance = 20.0f;
	
	//enemy movement
	if (!this->checkFrustum() || distanceToPlayer() > max_distance) {
		played_sound = false;

		//enemy direction to player
		Vector3 side = model.rotateVector(Vector3(1.0, 0.0, 0.0)).normalize();
		Vector3 forward = model.rotateVector(Vector3(0.0, 0.0, 1.0)).normalize();

		Vector3 direction = target_player - model.getTranslation();

		//model.lookAt(model.getTranslation(), direction, Vector3(0.0,1.0,0.0));

		float sideDot = side.dot(direction);
		float forwardDot = forward.dot(direction);

		yaw = sign(sideDot) * dt;

		model.rotate(yaw, Vector3(0.0, -1.0, 0.0));

		Vector3 nextPos = forward * speed * dt;

		model.translateGlobal(nextPos.x, 0.0f, nextPos.z);
	}
}

/*
//para que baile danza kuduro el personaje
void EnemyEntity::movementAndRotation(float dt, float speed) {

	float max_distance = 20.0f;

	//enemy movement
	if (this->checkFrustum() || distanceToPlayer() > max_distance) {
		played_sound = false;

		//enemy direction to player
		Vector3 side = model.rotateVector(Vector3(1.0, 0.0, 0.0)).normalize();
		Vector3 forward = model.rotateVector(Vector3(0.0, 0.0, -1.0)).normalize();

		Vector3 direction = target_player - model.getTranslation();

		//model.lookAt(model.getTranslation(), direction, Vector3(0.0,1.0,0.0));

		float sideDot = side.dot(direction);
		float forwardDot = forward.dot(direction);

		if (forwardDot < 0.90f) yaw += sign(sideDot) * dt;

		model.rotate(yaw, Vector3(0.0, -1.0, 0.0));

		Vector3 nextPos = forward * speed * dt;

		//model.translate(nextPos.x, 0.0f, nextPos.z);
	}
}
*/

void EnemyEntity::multiPass(std::vector<Light*> lights, Camera* camera) {
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
		mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS); //as default

}