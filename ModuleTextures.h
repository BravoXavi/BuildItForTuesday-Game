#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include<list>
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;

//This Modules controls the different textures that are used within the game. Every texture should be loaded and unloaded here.
class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	void Init();
	bool CleanUp();

	//Call this method to load any texture
	SDL_Texture* const Load(const char* path);
	void Unload(SDL_Texture* texture);

private:
	std::list<SDL_Texture*> textures;
};

#endif // __MODULETEXTURES_H__