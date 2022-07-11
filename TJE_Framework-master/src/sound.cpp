#include "sound.h"

Sound* Sound::instance = NULL;

//empty constructor
Sound::Sound(){
	
}

Sound* Sound::getInstance() {
	if (!instance)
		instance = new Sound;
	return instance;
}

//constructor
HSAMPLE* Sound::Get(const char* fileName) {
	//check if exists
	for (int i = 0; i < filenames.size(); i++) {
		if (filenames[i].compare(fileName) == 0) {
			return hSamples[i];
		}
	}

	//else load
	LoadSample(fileName, 0, 0, 1, false);
}

//second constructor
HSAMPLE* Sound::Get(const char* fileName, bool looped) {
	//check if exists
	for (int i = 0; i < filenames.size(); i++) {
		if (filenames[i].compare(fileName) == 0) {
			return hSamples[i];
		}
	}

	//else load
	LoadSample(fileName, 0, 0, 1, looped);
}

//third constructor
HSAMPLE* Sound::Get(const char* fileName, int offset, int length, int max, bool looped) {
	//check if exists
	for (int i = 0; i < filenames.size(); i++) {
		if (filenames[i].compare(fileName) == 0) {
			return hSamples[i];
		}
	}

	//else load
	LoadSample(fileName, offset, length, max, looped);
}

HSAMPLE* Sound::LoadSample(const char* fileName, int offset, int length, int max, bool looped) {
	//Cargamos un sample del disco duro (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound

	HSAMPLE* loadSample = new HSAMPLE();

	if (looped) {
		*loadSample = BASS_SampleLoad(false, fileName, offset, length, max, BASS_SAMPLE_LOOP);
	}
	else {
		*loadSample = BASS_SampleLoad(false, fileName, offset, length, max, BASS_SAMPLE_MONO);
	}

	if (*loadSample == 0) {
		std::cout << " + ERROR load " << fileName << std::endl;
	}
	else {
		hSamples.push_back(loadSample);
		filenames.push_back(fileName);
		std::cout << " + AUDIO load " << fileName << std::endl;
	}

	return loadSample;
}

void Sound::PlayGameSound(int idx) {
	HCHANNEL hSampleChannel = BASS_SampleGetChannel(*hSamples[idx], false);
	BASS_ChannelPlay(hSampleChannel, true);
}

void Sound::InitSound() {
	hSamples.reserve(5);
	Get("data/sounds/ambience/mixkit-scary-forest-at-night-2486.wav", true);
	Get("data/sounds/footsteps/steps.wav");
	Get("data/sounds/exhale.wav");
	Get("data/sounds/sustos/mixkit-cinematic-horror-trailer-long-sweep-561.wav", 0, 0, 3, false);
	Get("data/sounds/sustos/mixkit-horror-impact-773.wav", 0, 0, 3, false);
}