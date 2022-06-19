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
	lights.push_back(new Light(Vector3(255, 255, 255), 10.0, 1000.0, 0.0, 0.15));
	lights.push_back(new Light(Vector3(255, 255, 255), 10.0, 1000.0, 0.0, 1.0));
}

std::vector<Light*> LightManager::getLights() {
	return lights;
}