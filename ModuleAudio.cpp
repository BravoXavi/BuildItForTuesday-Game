#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"

#include <assert.h>

#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

using namespace std;

ModuleAudio::ModuleAudio( bool start_enabled) : Module( start_enabled)
{}

ModuleAudio::~ModuleAudio()
{}

void ModuleAudio::Init()
{
	SDL_Init(0);

	int code = SDL_InitSubSystem(SDL_INIT_AUDIO);
	assert(code == 0 && SDL_GetError());

	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);
	assert(init != 0 && SDL_GetError());
	
	code = Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
	assert(code == 0 && SDL_GetError());
}

bool ModuleAudio::CleanUp()
{
	if(music != nullptr)
	{
		Mix_FreeMusic(music);
	}

	for(vector<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end(); ++it)
		Mix_FreeChunk(*it);

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

//Load WAV Sounds and return the ID
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);
	assert(chunk != nullptr && Mix_GetError());

	fx.push_back(chunk);
	ret = fx.size() - 1;	

	return ret;
}

//Play a sound with the ID given by LoadFx
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(id < fx.size())
	{
		Mix_PlayChannel(-1, fx[id], repeat);
		ret = true;
	}

	return ret;
}

void ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	if (music != nullptr)
	{
		StopMusic();
	}

	music = Mix_LoadMUS(path);
	assert(music != nullptr && Mix_GetError());

	int code = Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f));
	assert(code >= 0 && Mix_GetError());
}

const void ModuleAudio::StopMusic() const
{
	Mix_HaltMusic();
}