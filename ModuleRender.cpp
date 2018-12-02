#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "FontManager.h"

#include <assert.h>

#include "SDL/include/SDL.h"

ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;
}

ModuleRender::~ModuleRender()
{}

void ModuleRender::Init()
{
	renderer = SDL_CreateRenderer(App->window->window, -1, SDL_RENDERER_PRESENTVSYNC);
	assert(renderer != nullptr && SDL_GetError());
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);

	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	if(renderer != nullptr)
		SDL_DestroyRenderer(renderer);

	return true;
}

//This method will increase the Y position of the camera depending on the given Y value.
const void ModuleRender::UpdateCameraPosition(const int& topYValue)
{
	if ((SCREEN_HEIGHT - topYValue) - App->renderer->camera.y > 220)
	{
		App->renderer->camera.y += cameraSpeed;
	}
}

//Send an SDL_Texture to be printed on screen.
const void ModuleRender::Blit(double angle, bool affectedByCamera, SDL_Texture* texture, const float& x, const float& y, SDL_Rect* section, const float& newWidth, const float& newHeight, SDL_RendererFlip flip) const
{
	bool clipImage = false;

	if ((SCREEN_HEIGHT - App->renderer->camera.y < y) && affectedByCamera)
		clipImage = true;
	
	if (!clipImage)
	{
		SDL_Rect rect;

		rect.x = (int)x;
		rect.y = affectedByCamera ? (int)y + camera.y : (int)y;

		if (newWidth >= 0.0f && newHeight >= 0.0f)
		{
			rect.w = (int)newWidth;
			rect.h = (int)newHeight;
		}
		else if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}

		SDL_Point p = { INT_MAX, INT_MAX };

		int code = SDL_RenderCopyEx(renderer, texture, section, &rect, angle, nullptr, flip);
		assert(code == 0 && SDL_GetError());
	}
}

//Generic SDL printings for creating Lines or Quads.
const void ModuleRender::DrawLine(bool use_camera, const int& x1, const int& y1, const int& x2, const int& y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
	{
		result = SDL_RenderDrawLine(renderer, camera.x + x1, camera.y + y1, camera.x + x2, camera.y + y2);
	}
	else
	{
		result = SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		assert(result == 0 && SDL_GetError());
	}
}

const void ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int code = SDL_RenderFillRect(renderer, &rect);
	assert(code == 0 && SDL_GetError());
}
