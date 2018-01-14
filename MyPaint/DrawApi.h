#pragma once
#include "resource.h"
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <shlwapi.h>
#include <Commdlg.h>
#include <vector>

class DrawApi {
public:
	virtual void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush)  = 0;
};

class DrawLineApi : public DrawApi {
public:
	void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush) override {
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

};

class DrawRectangleApi : public DrawApi {
public:
	void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush) override {
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

};

class DrawElipApi : public DrawApi {
public:
	void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush) override {
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

};

class DrawTriangleApi : public DrawApi {
public:
	void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush) override {
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

class DrawPentagonApi : public DrawApi {
public:
	void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush) override {
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

class DrawStarApi : public DrawApi {
public:
	void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush) override {
		HPEN pen = CreatePen(stylePen, sizeLine, colorLine);
		HBRUSH hBrush = CreateSolidBrush(colorBrush);
		if (hacthBrush > -1)
			hBrush = CreateHatchBrush(hacthBrush, colorBrush);
		SelectObject(hdc, pen);
		SelectObject(hdc, hBrush);

		POINT p[10];

		int X = x1 > x2 ? x2 + abs(x1 - x2) / 2 : x1 + abs(x1 - x2) / 2;
		int Y = y1 > y2 ? y2 + abs(y1 - y2) / 2 : y1 + abs(y1 - y2) / 2;

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

class DrawArrowApi : public DrawApi {
public:
	void Draw(HDC hdc, int x1, int y1, int x2, int y2, int sizeLine, int stylePen, COLORREF colorLine, int hacthBrush, int Solid, COLORREF colorBrush) override {
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

