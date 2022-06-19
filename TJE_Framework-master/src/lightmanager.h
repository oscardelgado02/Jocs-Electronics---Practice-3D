#pragma once
#include "light.h"

class LightManager
{
	static LightManager* instance;
	std::vector<Light*> lights;

	//constructor
	LightManager();

public:
	
	static LightManager* getInstance();

	void initLights();
	std::vector<Light*> getLights();
	void LightManager::updatePlayerLight(Vector3 pos);
};

