#include "Sound.h"

Sound::Sound()
{
	FMOD::System_Create(&system); // create an instance of the engine
	system->init(32, FMOD_INIT_NORMAL, 0); // initialise with 32 channels
}

Sound::~Sound()
{
	for (int i = 0; i < NUM_SOUNDS; i++) sounds[i]->release();
	system->close();
	system->release();
}

bool Sound::Load()
{
	//TO DO
	system->createStream("music1.mp3",     FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &sounds[SOUND_MUSIC1]);

	return true;
}

void Sound::Play(int soundID)
{
	//TO DO
	if (soundID == SOUND_MUSIC1) {
		system->playSound(FMOD_CHANNEL_FREE, sounds[SOUND_MUSIC1], false, &music1Channel);
		music1Channel->setVolume(0.2f); //between 0 and 1
	}
	else if (soundID == SOUND_MUSIC2) {
		system->playSound(FMOD_CHANNEL_FREE, sounds[SOUND_MUSIC2], false, &music2Channel);
		music2Channel->setVolume(0.2f); //between 0 and 1
	}
	else if (soundID == SOUND_MUSIC3) {
		system->playSound(FMOD_CHANNEL_FREE, sounds[SOUND_MUSIC3], false, &music3Channel);
		music3Channel->setVolume(0.2f); //between 0 and 1
	}
	else system->playSound(FMOD_CHANNEL_FREE, sounds[soundID], false, 0);
}

void Sound::StopAll()
{
	music1Channel->stop();
	music2Channel->stop();
	music3Channel->stop();
}

void Sound::Update()
{
	system->update();
}