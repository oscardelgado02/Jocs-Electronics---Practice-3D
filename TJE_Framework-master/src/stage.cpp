#include "stage.h"

Stage::Stage() {
	camera = new Camera();
}

//INTRO STAGE

IntroStage::IntroStage() {
	//camera->lookAt(Vector3(-1.14f, 15.9f, 40.01f), Vector3(-1.13f, 15.6f, 39.01f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->lookAt(Vector3(10.75f, 22.9f, -15.07f), Vector3(10.57f, 22.5f, -15.98f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, g->window_width / (float)g->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	camera->number = (int)STAGE_ID::INTRO;
}

STAGE_ID IntroStage::GetId() { return STAGE_ID::INTRO; }

void IntroStage::Render() {
	//render entities
	world->renderEntities();

	//text
	std::string text = "STALKERS";
	Vector3 color = Vector3(0.6, 0, 0); //first sentence color

	for (int i = 0; i < 2; i++) {
		if (i == 1) { text = "PRESS SPACE TO START"; color = Vector3(1, 1, 1);}

		float size = 16/(i*2+1);
		float scalated_size = (g->window_width * size / (1000.0));
		int screen_adjust = 3.0 * text.size() * scalated_size;
		int offset = 30 * scalated_size * i;

		drawText((g->window_width / 2.0) - (screen_adjust), (g->window_height / 3.0) + offset, text, color, scalated_size);
	}
}

void IntroStage::Update(float dt) {
	g->camera = camera;
}

//TUTORIAL STAGE

TutorialStage::TutorialStage() {}

STAGE_ID TutorialStage::GetId() { return STAGE_ID::TUTORIAL; }

void TutorialStage::Render() {
	//text
	std::string text = "It is the year 1203 and you heard rumours about an abandoned village where strange things happen.";
	Vector3 color = Vector3(1, 1, 1); //first sentence color

	for (int i = 0; i < 8; i++) {
		if (i == 1) { text = "The last villager reported time ago that there were \"things\" that stalked villagers in the shadows..."; color = Vector3(1, 1, 1); }
		if (i == 2) { text = "And that some people have disappeared..."; color = Vector3(0.6, 0.0, 0.0); }
		if (i == 3) { text = "You decide to investigate the village, but when you enter, the gate closes behind you."; color = Vector3(1, 1, 1); }
		if (i == 4) { text = "You need to find another gate to escape from there."; color = Vector3(0.8, 0.6, 0.0); }
		if (i == 5) { text = "And remember: the villager also said that those things stalked at the distance,"; color = Vector3(0.8, 0.6, 0.0); }
		if (i == 6) { text = "and that when they were close, they waited until you didn't look at them to chase you."; color = Vector3(0.8, 0.6, 0.0); }
		if (i == 7) { text = "PRESS SPACE TO CONTINUE"; color = Vector3(1, 1, 1); }

		float size = 1.7;
		float scalated_size = (g->window_width * size / (1000.0));
		int screen_adjust = 2.7 * text.size() * scalated_size;
		int offset = 30 * scalated_size * i;
		drawText((g->window_width / 2.0) - (screen_adjust), (g->window_height / 6.0) + offset, text, color, scalated_size);
	}
}

void TutorialStage::Update(float dt) {}

//PLAY STAGE

PlayStage::PlayStage() {
	camera = world->getPlayerEntityCamera();
	camera->number = (int)STAGE_ID::PLAY;
}

STAGE_ID PlayStage::GetId() { return STAGE_ID::PLAY; }

void PlayStage::Render() {
	//render entities
	world->renderEntities();
}

void PlayStage::Update(float dt) {
	//update entities
	g->camera = camera;
	world->updateEntities(dt);
}

//DEAD STAGE

DeadStage::DeadStage() {}

STAGE_ID DeadStage::GetId() { return STAGE_ID::DEAD; }

void DeadStage::Render() {
	//render entities
	world->renderEntities();
	//text
	std::string text = "YOU DIED";
	Vector3 color = Vector3(0.6, 0, 0); //first sentence color

	for (int i = 0; i < 2; i++) {
		if (i == 1) { text = "PRESS SPACE TO RESTART"; color = Vector3(1, 1, 1); }

		float size = 8 / (i * 2 + 1);
		float scalated_size = (g->window_width * size / (1000.0));
		int screen_adjust = 3.0 * text.size() * scalated_size;
		int offset = 30 * scalated_size * i;

		drawText((g->window_width / 2.0) - (screen_adjust), (g->window_height / 1.5) + offset, text, color, scalated_size);
	}
}

void DeadStage::Update(float dt) {
	//get killer cam
	camera = world->getKillerCam();
	camera->number = (int)STAGE_ID::DEAD;

	//get enemies and change animation
	std::vector<Entity*> enemies = world->getEnemyEntities();

	for (int i = 0; i < enemies.size(); i++) {
		((EnemyEntity*)enemies[i])->changeAnimation(ANIM_JUMPSCARE);
	}

	if (g->camera->number != camera->number) {
		Sound::getInstance()->PlayGameSound(JUMPSCARE2); //se reproduce una sola vez antes de cambiar de cámara
	}
	g->camera = camera;
}

//END STAGE

EndStage::EndStage() {
	camera->lookAt(Vector3(-29.53f, 19.9f, -96.69f), Vector3(-28.83f, 19.68f, -97.37f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, g->window_width / (float)g->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	camera->number = (int)STAGE_ID::INTRO;
}

STAGE_ID EndStage::GetId() { return STAGE_ID::END; }

void EndStage::Render() {
	//render entities
	world->renderEntities();

	//text
	std::string text = "Congratulations, you managed to escaped.";
	Vector3 color = Vector3(1, 1, 1); //first sentence color

	for (int i = 0; i < 8; i++) {
		if (i == 1) { text = "You try to find someone to help you to fight those \"things\", but nobody believes you..."; color = Vector3(1, 1, 1); }
		if (i == 2) { text = "Those monsters will still live, if there is any way to say it,"; color = Vector3(1, 1, 1); }
		if (i == 3) { text = "if no one fight them. That village will be cursed forever..."; color = Vector3(1, 1, 1); }
		if (i == 4) { text = "And also you..."; color = Vector3(0.6, 0.0, 0.0); }
		if (i == 5) { text = "Did you really think you escaped from us?"; color = Vector3(0.6, 0.0, 0.0); }
		if (i == 6) { text = "WE ARE ALWAYS WAITING IN THE SHADOWS"; color = Vector3(0.6, 0.0, 0.0); }
		if (i == 7) { text = "PRESS SPACE TO... EXIT?"; color = Vector3(1, 1, 1); }

		float size = 1.7;
		float scalated_size = (g->window_width * size / (1000.0));
		int screen_adjust = 2.7 * text.size() * scalated_size;
		int offset = 30 * scalated_size * i;
		drawText((g->window_width / 2.0) - (screen_adjust), (g->window_height / 6.0) + offset, text, color, scalated_size);
	}
}

void EndStage::Update(float dt) {
	g->camera = camera;
}

//OTHER METHODS

void InitStages(std::vector<Stage*>* stages) {
	stages->reserve(5);
	stages->push_back(new IntroStage());
	stages->push_back(new TutorialStage());
	stages->push_back(new PlayStage());
	stages->push_back(new DeadStage());
	stages->push_back(new EndStage());
}

Stage* GetStage(std::vector<Stage*> stages, STAGE_ID id) {
	return stages[(int)id];
}

void SetStage(STAGE_ID id, STAGE_ID* currentStage) {
	*currentStage = id;
}