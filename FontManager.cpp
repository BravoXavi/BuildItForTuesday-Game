#include "FontManager.h"

FontManager::FontManager()
{}

FontManager::~FontManager()
{}

void FontManager::Init()
{
	//Load the different font files ...
	redFont = new Font("assets/fonts/redFont.png", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>,?¿!¡ ");
	greenFont = new Font("assets/fonts/greenFont.png", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>,?¿!¡ ");
}

void FontManager::CleanUp()
{
	delete redFont;
	delete greenFont;
}