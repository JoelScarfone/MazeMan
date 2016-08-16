#include "fmod.hpp" //fmod c++ header
#pragma comment(lib, "fmodex_vc.lib"); //fmod library

//Sound array size
#define NUM_SOUNDS 6

//Sound identifiers
#define	SOUND_MUSIC1      0
#define	SOUND_MUSIC2      1
#define SOUND_MUSIC3      2

#define	SOUND_ARROW         3
#define	SOUND_EXPLOSION     4
#define	SOUND_FIREBALL      5


class Sound
{
public:
	Sound();
	~Sound();

	bool Load();
	void Play(int soundID);
	void Stop(int soundID);
	void StopAll();
	void Update();

	FMOD::System*   system; //handle to FMOD engine
	FMOD::Sound*    sounds[NUM_SOUNDS]; //sounds that will be loaded and played
	FMOD::Channel*  music1Channel;
	FMOD::Channel*  music2Channel;
	FMOD::Channel*  music3Channel;
};