#ifndef CLASS_FONT
#define CLASS_FONT

#include "SDL/include/SDL.h"
#include <stdio.h>
#include <string>
#include <map>

//Basic unit of the FontManager. Holds a bitmap with a font that is used to write in a pretty cool and fancy look.
class Font
{
public:
	Font(const char* fontBMP, const char* traductor);
	~Font();
		
	//Print the given text on a certain position with this font bitmap
	void printText(const char* textToWrite, const float& x, const float& y, float resize = 1.0f);

public:
	int characterHeight, characterWidth;

	SDL_Rect simbolPosition[45];
	SDL_Rect simbolSize;

private:
	std::map<char, int> traductorMap;

	SDL_Texture* fontPNG = nullptr;
};

#endif // !CLASS_FONT
