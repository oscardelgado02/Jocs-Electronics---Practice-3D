#pragma once

#include "includes.h"
#include "input.h"
#include "shader.h"
#include "entity.h"

class Light
{
public:
	Vector3 position;
	Vector3 color;
	float shininess;
	float max_distance;
	float intensity;
	bool enabled;

	//constructor
	Light(Vector3 pos, Vector3 c, float s, float md, float i);
};

