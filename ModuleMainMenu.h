#ifndef __MODULEMAINMENU_H__
#define __MODULEMAINMENU_H__

#include "Module.h"
#include "Font.h"

struct SDL_Texture;

//Main Menu scene. Shows the game title background and waits for user input to change scene.
class ModuleMainMenu : public Module
{
public:
	ModuleMainMenu(bool active = true);
	~ModuleMainMenu();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture * background;
	Uint32 timer;

	bool showText = true;

};

#endif // __MODULEMAINMENU_H__