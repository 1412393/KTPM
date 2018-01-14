#pragma once
#include "DrawApi.h"
#include "CShape.h"


class  ShapeFactory
{
	static ShapeFactory* instance;
	ShapeFactory() {};


public:
	static ShapeFactory*  getInstance();
	static CShape *createShape(CShapeType type);
};