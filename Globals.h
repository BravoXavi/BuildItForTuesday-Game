#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL/include/SDL_rect.h"

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

struct Vector2
{
	float x;
	float y;
};

#define TITLE "BuildItForTuesday - Xavier Bravo 2018"

// Configuration -----------
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 720

#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 50

#define BLOCK_PORTION ((int)(BLOCK_WIDTH / 2) / 4)

//Since Box2D uses 'Meters' as parameter, we need to convert them to Pixels. 50 Pixels = 1 Meter, so the cube measures 1 Meter.
#define METERS_TO_PIXELS(m) ((int) floor(50.0f * m))
#define PIXELS_TO_METERS(p) ((float) 0.02f * p)

#endif //__GLOBALS_H__