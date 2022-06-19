#pragma once

#include "input.h"

//images variables
#define FONT 0
#define MINIFONT 1
#define KEYS 2
#define PLAYER_IMAGE 3
#define GOAL_IMAGE 4
#define PLANET_IMAGE 5
#define PARTICLE_IMAGE 6
#define ROCK_IMAGE 7
#define TUTO_BACKGROUND 8
#define FINAL_BACKGROUND 9

//key variables
#define LEFT_KEY_IMAGE 0
#define UP_KEY_IMAGE 1
#define DOWN_KEY_IMAGE 2
#define RIGHT_KEY_IMAGE 3
#define Z_KEY_IMAGE 4
#define X_KEY_IMAGE 5
#define NUMBER_KEY_IMAGE 6

enum class STAGE_ID {
	INTRO = 0,
	TUTORIAL1 = 1,
	TUTORIAL2 = 2,
	PLAY = 3,
	END = 4
};

class Stage
{
public:

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