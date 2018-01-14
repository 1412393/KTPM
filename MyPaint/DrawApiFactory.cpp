#include "DrawApiFactory.h"

DrawApi *DrawApiFactory::Line = NULL;
DrawApi *DrawApiFactory::Rectangle = NULL;
DrawApi *DrawApiFactory::Elip = NULL;
DrawApi *DrawApiFactory::Triangle = NULL;
DrawApi *DrawApiFactory::Pentagon = NULL;
DrawApi *DrawApiFactory::Star = NULL;
DrawApi *DrawApiFactory::Arrow = NULL;





DrawApi* DrawApiFactory::getDrawApi(CShapeType type) {
	switch (type)
	{
		case F_Line: {
			if (Line == NULL) Line = new DrawLineApi();
			return Line;
			break;
		}
		case F_Rectangle: {
			if (Rectangle == NULL) Rectangle = new DrawRectangleApi();
			return Rectangle;
			break;
		}
		case F_Elip: {
			if (Elip == NULL) Elip = new DrawElipApi();
			return Elip;
			break;
		}
		case F_Triangle: {
			if (Triangle == NULL) Triangle = new DrawTriangleApi();
			return Triangle;
			break;
		}
		case F_Pentagon: {
			if (Pentagon == NULL) Pentagon = new DrawPentagonApi();
			return Pentagon;
			break;
		}
		case F_Star: {
			if (Star == NULL) Star = new DrawStarApi();
			return Star;
			break;
		}
		case F_Arrow: {
			if (Arrow == NULL) Arrow = new DrawArrowApi();
			return Arrow;
			break;
		}
		default: return NULL;
	}

}

