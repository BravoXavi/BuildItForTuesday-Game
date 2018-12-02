#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

//This Module holds information about the SDL Window we're going to work with and the Surface on that window.
class ModuleWindow : public Module
{
public:
	ModuleWindow();
	~ModuleWindow();

	void Init();
	bool CleanUp();

public:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;
};

#endif // __MODULEWINDOW_H__