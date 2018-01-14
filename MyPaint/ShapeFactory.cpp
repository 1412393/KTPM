#include "ShapeFactory.h"
#include "DrawApiFactory.h"

ShapeFactory* ShapeFactory::instance=NULL;


ShapeFactory*  ShapeFactory::getInstance()
	{
		if (!instance)
			instance = new ShapeFactory;
		return instance;
	}

CShape* ShapeFactory::createShape(CShapeType type) {
		switch (type) {
			case F_Line: {
				DrawApi *dr = DrawApiFactory::getDrawApi(type);
				return new CLine(dr);
				break;
			}
			case F_Rectangle: {
				DrawApi *dr = DrawApiFactory::getDrawApi(type);
				return new CRectangle(dr);
				break;
			}
			case F_Elip: {
				DrawApi *dr = DrawApiFactory::getDrawApi(type);
				return new CElip(dr);
				break;
			}
			case F_Triangle: {
				DrawApi *dr = DrawApiFactory::getDrawApi(type);
				return new CTriangle(dr);
				break;
			}
			case F_Pentagon: {
				DrawApi *dr = DrawApiFactory::getDrawApi(type);
				return new CPentagon(dr);
				break;
			}
			case F_Star: {
				DrawApi *dr = DrawApiFactory::getDrawApi(type);
				return new CStar(dr);
				break;
			}
			case F_Arrow: {
				DrawApi *dr = DrawApiFactory::getDrawApi(type);
				return new CArrow(dr);
				break;
			}
			default: return NULL;
		}
}
