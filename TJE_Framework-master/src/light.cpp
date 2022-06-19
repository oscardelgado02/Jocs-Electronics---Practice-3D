#include "light.h"

Light::Light(Vector3 pos, Vector3 c, float s, float md, float i) {
	position = pos;
	color = c;
	shininess = s;
	max_distance = md;
	intensity = i;
	enabled = true;
}