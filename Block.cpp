#include "Block.h"

Block::Block()
{}

Block::~Block()
{}

const int Block::GetXCoord() const
{
	return METERS_TO_PIXELS(body->GetPosition().x);
}

const int Block::GetYCoord() const
{
 	return METERS_TO_PIXELS(body->GetPosition().y);
}

const float Block::GetAngleInDegree() const
{
	return (body->GetAngle() * 57.3f);
}

const float Block::GetAngleInRad() const
{
	return (float)body->GetAngle();
}
