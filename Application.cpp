#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleMainMenu.h"
#include "ModuleGameLevel.h"
#include "FontManager.h"
#include "Font.h"

using namespace std;

Application::Application()
{
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(main_menu = new ModuleMainMenu(false));
	modules.push_back(game_level = new ModuleGameLevel(false));	
	modules.push_back(renderer = new ModuleRender());
	
	fontManager = new FontManager();
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		delete (*it);

	delete fontManager;
}

//Init and Start are called in order here.
//IMPORTANT: INIT WILL ONLY BE CALLED ONCE AT APP STARTUP!!
//When the system Enables a module, only Start should be called, so control on Init calls not supposed to be repeated.
bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		(*it)->Init();

	fontManager->Init();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	ChangeScene(main_menu);

	return ret;
}

//PreUpdate, Update and PostUpdate are called in order to allow the different modules to structure their calls.
//f.e. Render paints everything on PostUpdate, after all the game logic is calculated.
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	
	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled() == true)
			ret = (*it)->PreUpdate();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled() == true)
			ret = (*it)->Update();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled() == true)
			ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	fontManager->CleanUp();

	return ret;
}

//Change between scenes (Modules that hold game behaviour) uploading the previous one.
void Application::ChangeScene(Module* nextScene, Module* actualScene)
{
	if (actualScene == nullptr)
	{
		nextScene->Enable();
	}
	else
	{
		actualScene->Disable();
		nextScene->Enable();
	}
}

