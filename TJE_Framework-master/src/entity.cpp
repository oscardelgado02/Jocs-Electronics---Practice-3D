#include "entity.h"

Entity::Entity(std::string name, Matrix44 model) {
	this->name = name;
	this->model = model;
}