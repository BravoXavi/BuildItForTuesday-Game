#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleMainMenu.h"
#include "ModuleInput.h"
#include "FontManager.h"

ModuleMainMenu::ModuleMainMenu(bool active) : Module(active)
{}

ModuleMainMenu::~ModuleMainMenu()
{}

bool ModuleMainMenu::Start()
{
	timer = SDL_GetTicks();
	background = App->textures->Load("assets/MainMenuBackground.png");
	App->audio->PlayMusic("assets/sfx/music.ogg");

	return true;
}

bool ModuleMainMenu::CleanUp()
{
	bool ret = true;

	App->textures->Unload(background);

	return ret;
}

update_status ModuleMainMenu::Update()
{
	App->renderer->Blit(0.0, false, background, 0.0f, 0.0f, nullptr);

	if (SDL_GetTicks() - timer > 700)
	{
		timer = SDL_GetTicks();
		showText = !showText;
	}	

	if(showText)
		App->fontManager->redFont->printText("PRESS ENTER TO START", (SCREEN_WIDTH / 2) - 124, (SCREEN_HEIGHT / 2) - 15, 1.7f);

	//Change scene in case RETURN key pressed.
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->audio->StopMusic();
		App->ChangeScene((Module*)App->game_level, this);
	}

	return UPDATE_CONTINUE;
}