#include "enemyentity.h"

EnemyEntity::EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {

}

EnemyEntity::~EnemyEntity() {}

void EnemyEntity::render() {
	EntityMesh::render();
}

void EnemyEntity::update(float dt) {
	float speed = 2.0f;

	//Vector3 nextStep = ia.sendStep(model.getTranslation());
	
	if (!this->checkFrustum()) {
		Vector3 nextStep = Camera::current->eye;

		Vector3 direction = normalize(nextStep - model.getTranslation());
		Vector3 nextPos = direction * speed * dt;

		model.translate(nextPos.x, 0.0f, nextPos.z);
	}
}