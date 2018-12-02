#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

#include <assert.h>

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

using namespace std;

ModuleTextures::ModuleTextures()
{
}

ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

void ModuleTextures::Init()
{
	int flags = IMG_INIT_PNG;

	int init = IMG_Init(flags);
	assert(init != 0 && IMG_GetError());
}

bool ModuleTextures::CleanUp()
{
	for(list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		SDL_DestroyTexture(*it);

	textures.clear();
	return true;
}

//Call this method to load any texture
SDL_Texture* const ModuleTextures::Load(const char* path)
{
	SDL_Texture* texture = nullptr;

	SDL_Surface* surface = IMG_Load(path);
	assert(surface != nullptr && IMG_GetError());

	texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);
	assert(texture != nullptr && SDL_GetError());

	textures.push_back(texture);
		
	SDL_FreeSurface(surface);
	
	return texture;
}

//Always unload all textures before closing the application
void ModuleTextures::Unload(SDL_Texture* texture)
{
	for(list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if(*it == texture)
		{
			SDL_DestroyTexture(*it);
			textures.erase(it);
			break;
		}
	}
}