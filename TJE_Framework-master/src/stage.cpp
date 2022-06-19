#include "stage.h"

Stage::Stage() {}

//INTRO STAGE

IntroStage::IntroStage() {}

STAGE_ID IntroStage::GetId() { return STAGE_ID::INTRO; }

void IntroStage::Render() {
	drawText(2, 2, "PRESS SPACE TO START", Vector3(g->window_width/2.0, g->window_height/2.0, 1), 2);
}

void IntroStage::Update(float dt) {

}

//TUTORIAL STAGE

TutorialStage::TutorialStage(){
}

STAGE_ID TutorialStage::GetId() { return STAGE_ID::TUTORIAL; }

void TutorialStage::Render() {
	drawText(2, 2, "TUTORIAL", Vector3(g->window_width / 2.0, g->window_height / 2.0, 1), 2);
}

void TutorialStage::Update(float dt) {}

//PLAY STAGE

PlayStage::PlayStage() {

}

STAGE_ID PlayStage::GetId() { return STAGE_ID::PLAY; }

void PlayStage::Render() {
	//render entities
	world->renderEntities();
}

void PlayStage::Update(float dt) {
	//update entities
	world->updateEntities(dt);
}

//END STAGE

EndStage::EndStage() {

}

STAGE_ID EndStage::GetId() { return STAGE_ID::END; }

void EndStage::Render() {
	drawText(2, 2, "END", Vector3(g->window_width / 2.0, g->window_height / 2.0, 1), 2);
}

void EndStage::Update(float dt) {

}

//OTHER METHODS

void InitStages(std::vector<Stage*>* stages) {
	stages->reserve(4);
	stages->push_back(new IntroStage());
	stages->push_back(new TutorialStage());
	stages->push_back(new PlayStage());
	stages->push_back(new EndStage());
}

Stage* GetStage(std::vector<Stage*> stages, STAGE_ID id) {
	return stages[(int)id];
}

void SetStage(STAGE_ID id, STAGE_ID* currentStage) {
	*currentStage = id;
}