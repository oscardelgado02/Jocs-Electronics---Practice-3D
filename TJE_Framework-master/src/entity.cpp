#include "entity.h"

Entity::Entity(std::string name, Matrix44 model) {
	this->name = name;
	this->model = model;
}

Vector3 Entity::getPosition() {
	return model.getTranslation();
}

void Entity::setModel(Matrix44 model) {
	this->model = model;
}
