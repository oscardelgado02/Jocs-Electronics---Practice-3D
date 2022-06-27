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
	lights.push_back(new Light(Vector3(0, 500.0, 500.0), Vector3(255,255,255), 10.0, 1000.0, 0.1)); //medianoche:(65, 81, 98) luna:(247, 225, 195) blood_moon:(212, 48, 54)
	lights.push_back(new Light(Vector3(0, 0, 0), Vector3(215, 178, 49), 5.0, 10.0, 0.15));

	

	lights.push_back(new Light(Vector3(30.000, 2.500, -35.500), Vector3(215, 178, 49), 1.0, 3.0, 0.2));
	lights.push_back(new Light(Vector3(35.000, 2.500, -35.500), Vector3(215, 178, 49), 1.0, 3.0, 0.2));
	lights.push_back(new Light(Vector3(30.000, 2.500, -44.500), Vector3(215, 178, 49), 1.0, 3.0, 0.2));
	lights.push_back(new Light(Vector3(35.000, 2.500, -44.500), Vector3(215, 178, 49), 1.0, 3.0, 0.2));

	
}

std::vector<Light*> LightManager::getLights() {
	return lights;
}

void LightManager::updatePlayerLight(Vector3 pos) {
	lights[1]->position = pos;
}