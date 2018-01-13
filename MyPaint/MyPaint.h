#pragma once
#include "resource.h"
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <shlwapi.h>
#include <vector>
#include <typeinfo>
#include <Commdlg.h>
#include <fstream>
#include <fstream>
#include <wchar.h>


#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shlwapi.lib")
using namespace std;
#define _USE_MATH_DEFINES
#define LINE 0
#define RECTANGLE 1
#define ELIP 2
#define TRIANGLE 3
#define PENTAGON  4
#define STAR 5
#define ARROW 6

#define ANCHOR1 11
#define ANCHOR2 12
#define ANCHOR3 13
#define ANCHOR4 14
#define ANCHOR5 15
#define ANCHOR6 16
#define ANCHOR7 17
#define ANCHOR8 18
#define MIDANCHOR 19

class CShape {
public:
	virtual int typeShape() = 0;
	virtual RECT coordinates() = 0;
	virtual COLORREF get_colorLine() = 0;
	virtual COLORREF get_colorbrush() = 0;
	virtual int get_sizeLine() = 0;
	virtual int get_styleLine() = 0;
	virtual int get_solid() = 0;
	virtual int get_hatchbrush() = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(int a, int b, int c, int d, int sizepen, int stylepen) = 0;
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) = 0;
};

class CLine : public CShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int colorline;
	int sizeLine;
	int stylePen;
	COLORREF colorLine;
	int hacthBrush;
	int Solid;
	COLORREF colorBrush;
public:
	RECT coordinates() {
		RECT temp;
		temp.left = x1;
		temp.top = y1;
		temp.right = x2;
		temp.bottom = y2;
		return temp;
	}
	COLORREF get_colorLine() {
		return colorLine;
	}
	COLORREF get_colorbrush() {
		return colorBrush;
	}
	int get_sizeLine() {
		return sizeLine;
	}
	int get_styleLine() {
		return stylePen;
	}
	int get_solid() {
		return Solid;
	}
	int get_hatchbrush() {
		return hacthBrush;
	}
	void Draw(HDC hdc) {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);
		HBRUSH hBrush = CreateSolidBrush(colorBrush);
		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		SelectObject(hdc, pen);
		SelectObject(hdc, hBrush);
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
		DeleteObject(hBrush);
		DeleteObject(pen);
	}

	CShape* Create() { return new CLine; }
	int typeShape() {
		return 0;
	}
	void SetData(int a, int b, int c, int d, int sizepen, int stylepen) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
		sizeLine = sizepen;
		stylePen = stylepen;
	}
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) {
		colorLine = colorline;
		colorBrush = colorbrush;
		hacthBrush = hatchbrush;
		Solid = solid;
	}
};

class CRectangle : public CShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int colorline;
	int sizeLine;
	int stylePen;
	COLORREF colorLine;
	int hacthBrush;
	int Solid;
	COLORREF colorBrush;
public:
	RECT coordinates() {
		RECT temp;
		temp.left = x1;
		temp.top = y1;
		temp.right = x2;
		temp.bottom = y2;
		return temp;
	}
	COLORREF get_colorLine() {
		return colorLine;
	}
	COLORREF get_colorbrush() {
		return colorBrush;
	}
	int get_sizeLine() {
		return sizeLine;
	}
	int get_styleLine() {
		return stylePen;
	}
	int get_solid() {
		return Solid;
	}
	int get_hatchbrush() {
		return hacthBrush;
	}
	void Draw(HDC hdc) {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);

		HBRUSH hBrush = CreateSolidBrush(colorBrush);

		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		
		SelectObject(hdc, pen);
		if (colorBrush == -1) {
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			SetBkMode(hdc, TRANSPARENT);
		}
		else
		SelectObject(hdc, hBrush);

		Rectangle(hdc, x1, y1, x2, y2);
		DeleteObject(hBrush);
		DeleteObject(pen);
	}

	CShape* Create() { return new CRectangle; }

	int typeShape() {
		return 1;
	}
	void SetData(int a, int b, int c, int d, int sizepen, int stylepen) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
		sizeLine = sizepen;
		stylePen = stylepen;
	}
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) {
		colorLine = colorline;
		colorBrush = colorbrush;
		hacthBrush = hatchbrush;
		Solid = solid;
	}
};

class CElip : public CShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int colorline;
	int sizeLine;
	int stylePen;
	COLORREF colorLine;
	int hacthBrush;
	int Solid;
	COLORREF colorBrush;
public:
	RECT coordinates() {
		RECT temp;
		temp.left = x1;
		temp.top = y1;
		temp.right = x2;
		temp.bottom = y2;
		return temp;
	}
	COLORREF get_colorLine() {
		return colorLine;
	}
	COLORREF get_colorbrush() {
		return colorBrush;
	}
	int get_sizeLine() {
		return sizeLine;
	}
	int get_styleLine() {
		return stylePen;
	}
	int get_solid() {
		return Solid;
	}
	int get_hatchbrush() {
		return hacthBrush;
	}
	void Draw(HDC hdc) {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);
		HBRUSH hBrush = CreateSolidBrush(colorBrush);
		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		SelectObject(hdc, pen);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, x1, y1, x2, y2);
		DeleteObject(hBrush);
		DeleteObject(pen);
	}

	CShape* Create() { return new CElip; }
	int typeShape() {
		return 2;
	}
	void SetData(int a, int b, int c, int d, int sizepen, int stylepen) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
		sizeLine = sizepen;
		stylePen = stylepen;
	}
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) {
		colorLine = colorline;
		colorBrush = colorbrush;
		hacthBrush = hatchbrush;
		Solid = solid;
	}
};

class CTriangle : public CShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int colorline;
	int sizeLine;
	int stylePen;
	COLORREF colorLine;
	int hacthBrush;
	int Solid;
	COLORREF colorBrush;
public:
	CShape* Create() { return new CElip; }
	RECT coordinates() {
		RECT temp;
		temp.left = x1;
		temp.top = y1;
		temp.right = x2;
		temp.bottom = y2;
		return temp;
	}
	COLORREF get_colorLine() {
		return colorLine;
	}
	COLORREF get_colorbrush() {
		return colorBrush;
	}
	int get_sizeLine() {
		return sizeLine;
	}
	int get_styleLine() {
		return stylePen;
	}
	int get_solid() {
		return Solid;
	}
	int get_hatchbrush() {
		return hacthBrush;
	}

	int typeShape() {
		return 3;
	}
	void SetData(int a, int b, int c, int d, int sizepen, int stylepen) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
		sizeLine = sizepen;
		stylePen = stylepen;
	}
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) {
		colorLine = colorline;
		colorBrush = colorbrush;
		hacthBrush = hatchbrush;
		Solid = solid;
	}
	void Draw(HDC hdc) {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);
		HBRUSH hBrush = CreateSolidBrush(colorBrush);
		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		SelectObject(hdc, pen);
		SelectObject(hdc, hBrush);
		
		POINT p[3];
		p[0].x = abs(x2 + x1) / 2;
		p[0].y = y1;
		p[1].x = x1;
		p[1].y = y2;
		p[2].x = x2;
		p[2].y = y2;
		Polygon(hdc, p, 3);

		DeleteObject(hBrush);
		DeleteObject(pen);
	}
};

class CPentagon : public CShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int colorline;
	int sizeLine;
	int stylePen;
	COLORREF colorLine;
	int hacthBrush;
	int Solid;
	COLORREF colorBrush;
public:
	CShape* Create() { return new CElip; }
	RECT coordinates() {
		RECT temp;
		temp.left = x1;
		temp.top = y1;
		temp.right = x2;
		temp.bottom = y2;
		return temp;
	}
	COLORREF get_colorLine() {
		return colorLine;
	}
	COLORREF get_colorbrush() {
		return colorBrush;
	}
	int get_sizeLine() {
		return sizeLine;
	}
	int get_styleLine() {
		return stylePen;
	}
	int get_solid() {
		return Solid;
	}
	int get_hatchbrush() {
		return hacthBrush;
	}

	int typeShape() {
		return 4;
	}
	void SetData(int a, int b, int c, int d, int sizepen, int stylepen) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
		sizeLine = sizepen;
		stylePen = stylepen;
	}
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) {
		colorLine = colorline;
		colorBrush = colorbrush;
		hacthBrush = hatchbrush;
		Solid = solid;
	}
	void Draw(HDC hdc) {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);
		HBRUSH hBrush = CreateSolidBrush(colorBrush);
		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		SelectObject(hdc, pen);
		SelectObject(hdc, hBrush);

		POINT p[5];
		int y = abs(y2 - y1) * 2 / 5;
		int x = abs(x2 - x1) * 1 / 5;
		if (x1 > x2)
			x *= -1;
		if (y1 > y2)
			y *= -1;
		p[0].x = (x1 + x2) / 2;
		p[0].y = y1;
		p[1].x = x1;
		p[1].y = y1 + y;
		p[2].x = x1 + x;
		p[2].y = y2;
		p[3].x = x2 - x;
		p[3].y = y2;
		p[4].x = x2;
		p[4].y = y1 + y;
		Polygon(hdc, p, 5);

		DeleteObject(hBrush);
		DeleteObject(pen);
	}
};

class CStar : public CShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int colorline;
	int sizeLine;
	int stylePen;
	COLORREF colorLine;
	int hacthBrush;
	int Solid;
	COLORREF colorBrush;
public:
	CShape* Create() { return new CElip; }
	RECT coordinates() {
		RECT temp;
		temp.left = x1;
		temp.top = y1;
		temp.right = x2;
		temp.bottom = y2;
		return temp;
	}
	COLORREF get_colorLine() {
		return colorLine;
	}
	COLORREF get_colorbrush() {
		return colorBrush;
	}
	int get_sizeLine() {
		return sizeLine;
	}
	int get_styleLine() {
		return stylePen;
	}
	int get_solid() {
		return Solid;
	}
	int get_hatchbrush() {
		return hacthBrush;
	}

	int typeShape() {
		return 5;
	}
	void SetData(int a, int b, int c, int d, int sizepen, int stylepen) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
		sizeLine = sizepen;
		stylePen = stylepen;
	}
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) {
		colorLine = colorline;
		colorBrush = colorbrush;
		hacthBrush = hatchbrush;
		Solid = solid;
	}
	void Draw(HDC hdc) {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);
		HBRUSH hBrush = CreateSolidBrush(colorBrush);
		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		SelectObject(hdc, pen);
		SelectObject(hdc, hBrush);

		POINT p[10];
		//int y = abs(y2 - y1);
		//int x = abs(x2 - x1);
		//p[0].x = x1 + x / 2;
		//p[0].y = y1;
		//p[1].x = x1 + x / 3;
		//p[1].y = y1 + y / 3;
		//p[2].x = x1;
		//p[2].y = y1 + y / 3;
		//p[3].x = x1 + x / 4;
		//p[3].y = y1 + y / 2;
		//p[4].x = x1 + x / 8;
		//p[4].y = y2;
		//p[5].x = x1 + x / 2;
		//p[5].y = y2 - y / 3;
		//p[6].x = x2 - x / 8;
		//p[6].y = y2;
		//p[7].x = x2 - x / 4;
		//p[7].y = y1 + y / 2;
		//p[8].x = x2;
		//p[8].y = y1 + y / 3;
		//p[9].x = x2 - x / 3;
		//p[9].y = y1 + y / 3;

		int X = x1 > x2 ? x2 + abs(x1 - x2)/2 : x1 + abs(x1 - x2)/2;
		int Y = y1 > y2 ? y2 + abs(y1 - y2)/2 : y1 + abs(y1 - y2)/2;
		
		for (int i = 0; i < 10; i++) {
			int radius = abs(Y - y1) / 2;

			double iRadius = (i % 2 == 0) ? radius : (radius * 0.5);
			double angle = (i * 720.0) / 10;

			p[i].x = X + iRadius * cos(angle * 180.0 / 3.14); 
			p[i].y = Y + iRadius * sin(angle * 180.0 / 3.14);
		}

		Polygon(hdc, p, 10);
		
		DeleteObject(hBrush);
		DeleteObject(pen);
	}
};

class CArrow : public CShape {
	int x1;
	int y1;
	int x2;
	int y2;
	int colorline;
	int sizeLine;
	int stylePen;
	COLORREF colorLine;
	int hacthBrush;
	int Solid;
	COLORREF colorBrush;
public:
	CShape* Create() { return new CElip; }
	RECT coordinates() {
		RECT temp;
		temp.left = x1;
		temp.top = y1;
		temp.right = x2;
		temp.bottom = y2;
		return temp;
	}
	COLORREF get_colorLine() {
		return colorLine;
	}
	COLORREF get_colorbrush() {
		return colorBrush;
	}
	int get_sizeLine() {
		return sizeLine;
	}
	int get_styleLine() {
		return stylePen;
	}
	int get_solid() {
		return Solid;
	}
	int get_hatchbrush() {
		return hacthBrush;
	}

	int typeShape() {
		return 6;
	}
	void SetData(int a, int b, int c, int d, int sizepen, int stylepen) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
		sizeLine = sizepen;
		stylePen = stylepen;
	}
	virtual void SetColor(COLORREF colorline, int hatchbrush, int solid, COLORREF colorbrush) {
		colorLine = colorline;
		colorBrush = colorbrush;
		hacthBrush = hatchbrush;
		Solid = solid;
	}
	void Draw(HDC hdc) {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);
		HBRUSH hBrush = CreateSolidBrush(colorBrush);
		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		SelectObject(hdc, pen);
		SelectObject(hdc, hBrush);
		
		POINT p[7];
		int y = (y2 - y1) * 2 / 8;
		int x = (x2 - x1) * 7 / 10;
		p[0].x = x1;
		p[0].y = y1 + y;
		p[1].x = x1 + x;
		p[1].y = y1 + y;
		p[2].x = x1 + x;
		p[2].y = y1;
		p[3].x = x2;
		p[3].y = (y2 + y1) / 2;
		p[4].x = x1 + x;
		p[4].y = y2;
		p[5].x = x1 + x;
		p[5].y = y2 - y;
		p[6].x = x1;
		p[6].y = y2 - y;
		Polygon(hdc, p, 7);

		DeleteObject(hBrush);
		DeleteObject(pen);
	}
};