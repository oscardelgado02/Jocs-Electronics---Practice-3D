#include "enemyentity.h"

EnemyEntity::EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {
	played_sound = false;
	sounds.push_back(new Sound("data/sounds/sustos/mixkit-cinematic-horror-trailer-long-sweep-561.wav"));
	sounds.push_back(new Sound("data/sounds/sustos/mixkit-horror-impact-773.wav"));
	//sounds.push_back(new Sound("data/sounds/sustos/mixkit-monster-footstep-1975.wav"));
}

EnemyEntity::~EnemyEntity() {}

void EnemyEntity::render() {
	EntityMesh::render();
}

void EnemyEntity::update(float dt) {
	float speed = 2.0f;
	float max_distance = 20.0f;
	float jumpscare1_dist = 18.0f;
	float jumpscare2_dist = 5.0f;
	float step_distance = 20.0f;

	//Vector3 nextStep = ia.sendStep(model.getTranslation());
	
	//sound of enemy
	if (this->checkFrustum() && !played_sound) {

		if (distanceToCam() < jumpscare2_dist) {
			sounds[JUMPSCARE2]->PlayGameSound();
			played_sound = true;
		}
		else if (distanceToCam() < jumpscare1_dist) {
			sounds[JUMPSCARE1]->PlayGameSound();
			played_sound = true;
		}
	}

	/*if (distanceToCam() < step_distance) {
		sounds[FOOTSTEP_MONSTER]->PlayGameSound();
	}*/

	//enemy movement
	if (!this->checkFrustum() || distanceToCam() > max_distance) {
		played_sound = false;
		
		Vector3 nextStep = Camera::current->eye;
		Vector3 direction = normalize(nextStep - model.getTranslation());
		Vector3 nextPos = direction * speed * dt;

		model.translate(nextPos.x, 0.0f, nextPos.z);
	}
}