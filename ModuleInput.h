#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"

#include "SDL/include/SDL_scancode.h"

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

//Module controling all user input. The implementation is the generic SDL one. All information is parsed in PreUpdate to make sure is done after
//all the game calculations are executed.
class ModuleInput : public Module
{
public:
	ModuleInput();
	virtual ~ModuleInput();

	void Init();
	update_status PreUpdate();
	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

private:
	bool windowEvents[WE_COUNT];
	KeyState* keyboard;
};

#endif // __MODULEINPUT_H__