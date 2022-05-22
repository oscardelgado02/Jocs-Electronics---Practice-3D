#include "wallentity.h"

WallEntity::WallEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(name, model, mesh, texture, shader, color) {}

WallEntity::~WallEntity() {}

void WallEntity::render() {
	EntityMesh::render();
}

void WallEntity::update(float dt) {}