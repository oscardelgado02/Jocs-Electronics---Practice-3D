#include "sound.h"

//empty constructor
Sound::Sound(){}

//constructor
Sound::Sound(const char* fileName) {
	LoadSample(fileName, 0, 0, 1, false);
}

//second constructor
Sound::Sound(const char* fileName, bool looped) {
	LoadSample(fileName, 0, 0, 1, looped);
}

//third constructor
Sound::Sound(const char* fileName, int offset, int length, int max, bool looped) {
	LoadSample(fileName, offset, length, max, looped);
}

void Sound::LoadSample(const char* fileName, int offset, int length, int max, bool looped) {
	//Cargamos un sample del disco duro (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound

	if (looped) {
		hSample = BASS_SampleLoad(false, fileName, offset, length, max, BASS_SAMPLE_LOOP);
	}
	else {
		hSample = BASS_SampleLoad(false, fileName, offset, length, max, BASS_SAMPLE_MONO);
	}

	if (hSample == 0) {
		std::cout << " + ERROR load " << fileName << std::endl;
	}
	else {
		std::cout << " + AUDIO load " << fileName << std::endl;
	}
}

void Sound::PlayGameSound() {
	hSampleChannel = BASS_SampleGetChannel(hSample, false);
	BASS_ChannelPlay(hSampleChannel, true);
}

void Sound::PauseGameSound() {
	BASS_ChannelPause(hSampleChannel);
}