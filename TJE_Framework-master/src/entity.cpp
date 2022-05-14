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

float Entity::distanceToCam() {

	//needed variables
	Camera* cam = Camera::current;

	Vector3 modelPos = model.getTranslation();
	Vector3 camPos = cam->eye;

	return modelPos.distance(camPos);
}