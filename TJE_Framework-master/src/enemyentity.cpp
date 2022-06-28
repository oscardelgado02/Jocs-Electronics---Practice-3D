#include "enemyentity.h"

EnemyEntity::EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {
	played_sound = false;
	sounds.push_back(new Sound("data/sounds/sustos/mixkit-cinematic-horror-trailer-long-sweep-561.wav"));
	sounds.push_back(new Sound("data/sounds/sustos/mixkit-horror-impact-773.wav"));
	//sounds.push_back(new Sound("data/sounds/sustos/mixkit-monster-footstep-1975.wav"));
	target_player = Vector3(0.0f, 0.0f, 0.0f); //initial value
	last_direction = Vector3(0.0f, 0.0f, 0.0f);
}

EnemyEntity::~EnemyEntity() {}

void EnemyEntity::render() {
	EntityMesh::render();
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
	
	//enemy direction to player
	Vector3 nextStep = Camera::current->eye;
	Vector3 direction = normalize(nextStep - model.getTranslation());
	Vector3 nextPos = direction * speed * dt;

	//enemy movement
	if (!this->checkFrustum() || distanceToPlayer() > max_distance) {
		played_sound = false;
		float direction_delta = (direction.x - last_direction.x) + (direction.z - last_direction.z);

		model.translate(nextPos.x, 0.0f, nextPos.z);
		model.rotate(direction_delta, Vector3(0.0f, 1.0f, 0.0f));//enemy rotation
	}
}