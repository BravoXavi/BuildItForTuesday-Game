#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Font.h"
#include <map>
#include <vector>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

//ModuleRender will take care of anything regarding printing on screen (Blit on SDL) or the camera.
//Every calling done to this Module will be Rendered in the next RenderPresent (PostUpdate).
class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	void Init();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	//This method will increase the Y position of the camera depending on the given Y value.
	const void UpdateCameraPosition(const int& topYValue);

	//Send an SDL_Texture to be added to the Renderer.
	const void Blit(double angle, bool affectedByCamera, SDL_Texture* texture, const float& x, const float& y, SDL_Rect* section, const float& newWidth = -1.0f, const float& newHeight = -1.0f, SDL_RendererFlip flip = SDL_FLIP_NONE ) const;
	
	//Generic SDL printings for creating Lines or Quads.
	const void DrawLine(bool use_camera, const int& x1, const int& y1, const int& x2, const int& y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	const void DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

private:
	int cameraSpeed = 4;

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
};

#endif // __MODULERENDER_H__