#include "lightmanager.h"

LightManager* LightManager::instance = NULL;

LightManager* LightManager::getInstance() {
	if (!instance)
		instance = new LightManager;
	return instance;
}

LightManager::LightManager() {
	initLights();
}

void LightManager::initLights() {
	lights.push_back(new Light(Vector3(0, 500.0, 500.0), Vector3(255, 255, 255), 10.0, 1000.0, 0.1)); //medianoche:(65, 81, 98) luna:(247, 225, 195) blood_moon:(212, 48, 54)
	lights.push_back(new Light(Vector3(0, 0, 0), Vector3(215, 178, 49)*0.5, 5.0, 10.0, 0.15));


	lights.push_back(new Light(Vector3(30.000, 2.500, -35.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(35.000, 2.500, -35.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(30.000, 2.500, -44.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(35.000, 2.500, -44.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));

	lights.push_back(new Light(Vector3(10.000, 4.000, -38.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(10.000, 4.000, -48.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(-11.000, 4.000, -38.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(-11.000, 4.000, -48.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));

	lights.push_back(new Light(Vector3(15.500, 3.500, -41.700), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(23.500, 3.500, -41.700), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(23.500, 3.500, -51.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));
	lights.push_back(new Light(Vector3(15.500, 3.500, -51.500), Vector3(215, 178, 49), 1.0, 3.0, 0.01));

	
}

std::vector<Light*> LightManager::getLights() {
	return lights;
}

void LightManager::updatePlayerLight(Vector3 pos) {
	lights[1]->position = pos;
}