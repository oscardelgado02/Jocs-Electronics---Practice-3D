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
	void updatePlayerLight(Vector3 pos);
};

