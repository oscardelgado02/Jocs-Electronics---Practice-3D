#include "enemyentity.h"

EnemyEntity::EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {}

EnemyEntity::~EnemyEntity() {}

void EnemyEntity::render() {
	EntityMesh::render();
}

void EnemyEntity::update(float dt) {}