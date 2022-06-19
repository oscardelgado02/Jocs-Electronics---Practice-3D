#include "light.h"

Light::Light(Vector3 c, float s, float md, float a, float i) {
	color = c;
	shininess = s;
	max_distance = md;
	angle = a;
	intensity = i;
}

void Light::setUniforms(Shader* shader) {
	shader->setUniform("u_light_color", color);
	shader->setUniform("u_shininess", shininess);
	shader->setUniform("u_intensity", intensity);
}