#ifndef CLASS_BLOCK
#define CLASS_BLOCK

#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class b2Body;

//Main game element. Holds a physic Box2D body and calls for returning position parameters in pixel values.
class Block
{
public:
	Block();
	~Block();

	//Get X and Y values in Pixels (Box2D calculates in meters)
	const int GetXCoord() const;
	const int GetYCoord() const;

	//Get rotation angle of bodies in both types (Box2D default is RAD)
	const float GetAngleInDegree() const;
	const float GetAngleInRad() const;

public:
	b2Body * body;
	int width, height;
};

#endif // !CLASS_BLOCK
