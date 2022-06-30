#pragma once
#include "bass.h"
#include "utils.h"

#define AMBIENCE 0
#define FOOTSTEPS 1
#define EXHALE 2
#define JUMPSCARE1 3
#define JUMPSCARE2 4

class Sound
{
	static Sound* instance;
	Sound(); //constructor

public:

	std::vector<HSAMPLE*> hSamples;
	std::vector<std::string> filenames;

	static Sound* getInstance();

	//load methods
	HSAMPLE* Get(const char* fileName);
	HSAMPLE* Get(const char* fileName, bool looped);
	HSAMPLE* Get(const char* fileName, int offset, int length, int max, bool looped);

	HSAMPLE* LoadSample(const char* fileName, int offset, int length, int max, bool looped);
	void PlayGameSound(int idx);

	void InitSound();
};