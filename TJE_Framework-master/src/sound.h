#pragma once
#include "bass.h"
#include "utils.h"


class Sound
{
public:

	HSAMPLE hSample;

	Sound(const char* fileName); //constructor
	Sound(const char* fileName, bool looped); //second constructor
	Sound(const char* fileName, int offset, int length, int max, bool looped); //third constructor
	~Sound() {};

	void LoadSample(const char* fileName, int offset, int length, int max, bool looped);
	void PlayGameSound();
};

