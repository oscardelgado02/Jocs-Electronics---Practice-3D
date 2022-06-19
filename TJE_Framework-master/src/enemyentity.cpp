#include "enemyentity.h"

EnemyEntity::EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {
	idx = 0;
}

EnemyEntity::~EnemyEntity() {}

void EnemyEntity::render() {
	EntityMesh::render();
}

void EnemyEntity::update(float dt) {
	float speed = 0.2f;

	Vector3 nextStep = ia.sendStep(idx);

	if (nextStep.x == NULL) {
		idx = 0;
	}
	else {
		Vector3 direction = normalize(model.getTranslation() - nextStep);

		Vector3 nextPos = model.getTranslation() + direction * speed * dt;

		model.translate(nextPos.x, nextPos.y, nextPos.z);

		if ((nextStep.x / ia.tileSizeX) != (nextPos.x / ia.tileSizeX)) {
			if ((nextStep.z / ia.tileSizeY) != (nextPos.z / ia.tileSizeY)) {
				idx++;
			}
		}
	}

	ia.setCenter(model.getTranslation());
	ia.setTarget();
}