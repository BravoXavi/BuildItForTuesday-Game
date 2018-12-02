#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_UPDATE,
	MAIN_FINISH
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;

	App = new Application();
	App->Init();

	main_states state = MAIN_UPDATE;
	
	//Start UPDATE loop until someone returns UPDATE_STOP.
	while (state != MAIN_FINISH)
	{
		int update_return = App->Update();

		if (update_return == UPDATE_STOP)
		{
			state = MAIN_FINISH;
		}
	}

	if (App->CleanUp())
	{
		main_return = EXIT_SUCCESS;
	}

	delete App;
	return main_return;
}