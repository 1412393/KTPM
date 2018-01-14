#pragma once
#include "DrawApi.h"
#include <map>
#include "CShape.h"

class  DrawApiFactory
{
	static DrawApi *Line, *Rectangle, *Elip, *Triangle, *Pentagon, *Star, *Arrow;

public:

	static DrawApi* getDrawApi(CShapeType type);
};