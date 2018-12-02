#ifndef CLASS_FONTMANAGER
#define CLASS_FONTMANAGER

#include <map>
#include "Font.h"

//FontManager controls the different types of fonts loaded from bitmaps. 
class FontManager 
{
public:
	FontManager();
	~FontManager();

	void Init();
	void CleanUp();

public:
	Font* redFont;
	Font* greenFont;

};

#endif // !CLASS_FONTMANAGER
