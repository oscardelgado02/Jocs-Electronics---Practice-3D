#pragma once

#include "input.h"
#include "world.h"
#include "game.h"

enum class STAGE_ID {
	INTRO = 0,
	TUTORIAL = 1,
	PLAY = 2,
	END = 3
};

class Stage
{
public:
	Game* g = Game::instance;
	World* world = World::getInstance(); //we get world instance

	//constructors
	Stage();

	//methods
	virtual STAGE_ID GetId() = 0;
	virtual void Render() = 0;
	virtual void Update(float dt) = 0;
};

void InitStages(std::vector<Stage*>* stages);
Stage* GetStage(std::vector<Stage*> stages, STAGE_ID id);
void SetStage(STAGE_ID id, STAGE_ID* currentStage);

class IntroStage : public Stage {
public:

	//constructor
	IntroStage();

	//methods
	STAGE_ID GetId();
	void Render();
	void Update(float dt);
};

class TutorialStage : public Stage {
public:

	//constructor
	TutorialStage();

	//methods
	STAGE_ID GetId();
	void Render();
	void Update(float dt);
};

class PlayStage : public Stage {
public:

	//constructor
	PlayStage();

	//methods
	STAGE_ID GetId();
	void Render();
	void Update(float dt);
};

class EndStage : public Stage {
public:

	//constructor
	EndStage();

	//methods
	STAGE_ID GetId();
	void Render();
	void Update(float dt);
};