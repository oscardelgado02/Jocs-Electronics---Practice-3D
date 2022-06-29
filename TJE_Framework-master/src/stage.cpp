#include "stage.h"

Stage::Stage() {
	camera = new Camera();
}

//INTRO STAGE

IntroStage::IntroStage() {
	camera->lookAt(Vector3(-1.14f, 15.9f, 40.01f), Vector3(-1.13f, 15.6f, 39.01f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, g->window_width / (float)g->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

STAGE_ID IntroStage::GetId() { return STAGE_ID::INTRO; }

void IntroStage::Render() {
	world->renderEntities();

	std::string text = "Stalkers";

	for (int i = 0; i < 2; i++) {
		if (i == 1) text = "PRESS SPACE TO START";

		float size = 16/(i*2+1);
		float scalated_size = (g->window_width * size / (1000.0));
		int screen_adjust = 3.0 * text.size() * scalated_size;
		int offset = 30 * scalated_size * i;

		drawText((g->window_width / 2.0) - (screen_adjust), (g->window_height / 3.0) + offset, text, Vector3(1, 1, 1), scalated_size);
	}
}

void IntroStage::Update(float dt) {
	g->camera = camera;
}

//TUTORIAL STAGE

TutorialStage::TutorialStage(){}

STAGE_ID TutorialStage::GetId() { return STAGE_ID::TUTORIAL; }

void TutorialStage::Render() {
	std::string text = "It is the year 1203 and you heard rumours about and old village where strange things happen.";

	for (int i = 0; i < 8; i++) {
		if (i == 1) text = "The last villager reported time ago that there were \"things\" that stalked villagers in the shadows...";
		if (i == 2) text = "And that some people have disappeared...";
		if (i == 3) text = "You decide to investigate the village, but when you enter, the gate closes behind you.";
		if (i == 4) text = "You need to find another gate to escape from there.";
		if (i == 5) text = "And remember: the villager also said that those things stalked at the distance,";
		if (i == 6) text = "and that when they were close, they waited until you didn't look at them to chase you.";
		if (i == 7) text = "PRESS SPACE TO CONTINUE";

		float size = 1.7;
		float scalated_size = (g->window_width * size / (1000.0));
		int screen_adjust = 2.7 * text.size() * scalated_size;
		int offset = 30 * scalated_size * i;
		drawText((g->window_width / 2.0) - (screen_adjust), (g->window_height / 6.0) + offset, text, Vector3(1, 1, 1), scalated_size);
	}
}

void TutorialStage::Update(float dt) {}

//PLAY STAGE

PlayStage::PlayStage() {
	camera = world->getPlayerEntityCamera();
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
	std::string text = "YOU DIED";
	Vector3 color = Vector3(1, 0, 0);

	for (int i = 0; i < 2; i++) {
		if (i == 1) text = "PRESS SPACE TO RESTART"; color = Vector3(1, 1, 1);

		float size = 8 / (i * 2 + 1);
		float scalated_size = (g->window_width * size / (1000.0));
		int screen_adjust = 3.0 * text.size() * scalated_size;
		int offset = 30 * scalated_size * i;

		drawText((g->window_width / 2.0) - (screen_adjust), (g->window_height / 2.0) + offset, text, color, scalated_size);
	}
}

void DeadStage::Update(float dt) {

}

//END STAGE

EndStage::EndStage() {

}

STAGE_ID EndStage::GetId() { return STAGE_ID::END; }

void EndStage::Render() {
	drawText(2, 2, "END", Vector3(g->window_width / 2.0, 1.0, g->window_height / 2.0), 2);
}

void EndStage::Update(float dt) {

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