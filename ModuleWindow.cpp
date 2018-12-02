#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

#include <assert.h>

ModuleWindow::ModuleWindow()
{}

ModuleWindow::~ModuleWindow()
{}

void ModuleWindow::Init()
{
	int code = SDL_Init(SDL_INIT_VIDEO);
	assert(code >= 0 && SDL_GetError());

	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	assert(window != nullptr && SDL_GetError());

	screen_surface = SDL_GetWindowSurface(window);		
}

bool ModuleWindow::CleanUp()
{
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	SDL_Quit();

	return true;
}

