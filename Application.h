#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class FontManager;

// Game modules ---
class ModuleMainMenu;
class ModuleGameLevel;

//Functional core of the application and holder of all the Modules. This class will be the one calling all Init, Start, Post/-/Pre Update and Cleanups of the different modules.
class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleMainMenu* main_menu;
	ModuleGameLevel* game_level;

	FontManager* fontManager;

public:
	void ChangeScene(Module* nextScene, Module* actualScene = nullptr);

private:
	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__