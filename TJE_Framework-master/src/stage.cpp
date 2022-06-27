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
	drawText(2, 2, "PRESS SPACE TO START", Vector3(g->window_width / 2.0, 1.0, g->window_height / 2.0), 2);
	world->renderEntities();
}

void IntroStage::Update(float dt) {
	g->camera = camera;
}

//TUTORIAL STAGE

TutorialStage::TutorialStage(){
}

STAGE_ID TutorialStage::GetId() { return STAGE_ID::TUTORIAL; }

void TutorialStage::Render() {
	drawText(2, 2, "TUTORIAL", Vector3(g->window_width / 2.0, 1.0, g->window_height / 2.0), 2);
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

DeadStage::DeadStage() {

}

STAGE_ID DeadStage::GetId() { return STAGE_ID::DEAD; }

void DeadStage::Render() {
	drawText(2, 2, "DEAD", Vector3(g->window_width / 2.0, 1.0, g->window_height / 2.0), 2);
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