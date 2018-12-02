#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include <vector>
#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

//This Module controls everythign regarding the sound on the game, Music or generic FX.
class ModuleAudio : public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	void Init();
	bool CleanUp();

	void PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	const void StopMusic() const;

	//Load WAV Sounds and return the ID
	unsigned int LoadFx(const char* path);

	//Play a sound with the ID given by LoadFx
	bool PlayFx(unsigned int fx, int repeat = 0);	

private:
	Mix_Music*	music = nullptr;
	std::vector<Mix_Chunk*>	fx;
};

#endif // __MODULEAUDIO_H__