// MyPaint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CShape.h"
#include "ShapeFactory.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst, ghInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//Các handle trong chương trình
HWND hRebar, hToolBar, hSizeLine, hTabControl, hCurrentTab, hToolBarShape, hStylePen, hToolBarBrush, hExport, hToolBarColor, hCursor;

//Khởi tạo biến về đường vẽ
int sizepen = 1;
int StylePen = 0;
//Khởi tại biến màu tô


int hatchbrush = -1;
int solid = 1;
int idbutton = 0;
COLORREF color;
COLORREF colorLine;
COLORREF colorBrush;
HBITMAP selectBMP;


//Vector chứa các đối tượng đã vẽ
vector<CShape*> shapes;
vector<CShape*> redo;
//Đối tượng đang vẽ
CShape* currentShape;
bool SelectedShape = false, isCursorbutton = false, isHoding = false, isText = false;
int sle_x1, sle_x2, sle_y1, sle_y2;

//Đối tượng đang được chọn bởi anchor
CShape* tempShape;
CShape* selectedShapeRect;
ShapeFactory* shapefactory;
// loại hình đang vẽ
int selectedShape = -1;
// có đang vẽ
bool isDrawing = FALSE;
// toạ độ khi nhấn chuột và hiện tại
int firstX;
int firstY;
int lastX;
int lastY;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool OnCreate(HWND , LPCREATESTRUCT );
void OnGetMinMaxInfo(HWND , LPMINMAXINFO );
void DrawAnchor(HDC , int , int , int , int );
int ShowAnchorCursor(HWND , int , int );
void UpdateClientArea(HWND );

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//Lưu file bmp
void SaveBMPFile(HANDLE hFile, PBITMAPINFO pBI, HBITMAP hBitmap, HDC hDC)
{
	BITMAPFILEHEADER hdr;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD Total;
	DWORD countByte;
	BYTE *hp;
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pBI;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	GetDIBits(hDC, hBitmap, 0, (WORD)pbih->biHeight, lpBits, pBI,
		DIB_RGB_COLORS);

	hdr.bfType = 0x4d42;
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;


	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);

	// Copy  BITMAPFILEHEADER vào .BMP file.  
	WriteFile(hFile, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL);

	//Copy BITMAPINFOHEADER và RGBQUAD  vào file
	WriteFile(hFile, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL));

	//Copy mảng màu sang file
	Total = countByte = pbih->biSizeImage;
	hp = lpBits;
	WriteFile(hFile, (LPSTR)hp, (int)countByte, (LPDWORD)&dwTmp, NULL);
	CloseHandle(hFile);
	GlobalFree((HGLOBAL)lpBits);
}
//Chuyển sang hình ảnh dạng bitmap để lưu
HBITMAP CaptureAnImage(HWND hWnd, int x1, int y1, int x2, int y2)
{
	POINT a, b;
	a.x = x1;
	a.y = y1;
	b.x = x2;
	b.y = y2;

	// copy screen to bitmap
	HDC     hScreen = GetDC(hWnd);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY);

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	return hBitmap;
}
//Tạo file bitmap
PBITMAPINFO CreateBitmapInfo(HBITMAP hBitmap)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    Bits;
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);
	Bits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (Bits == 1)
		Bits = 1;
	else if (Bits <= 4)
		Bits = 4;
	else if (Bits <= 8)
		Bits = 8;
	else if (Bits <= 16)
		Bits = 16;
	else if (Bits <= 24)
		Bits = 24;
	else Bits = 32;
	if (Bits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << Bits));
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (Bits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << Bits);
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * Bits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	CShape *shape;

    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_GETMINMAXINFO, OnGetMinMaxInfo);
    case WM_COMMAND:
        {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.nMaxFile = 260;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			if (LOWORD(wParam) == IDC_COMBOBOXSIZE) {
				int idx = SendMessage(hSizeLine, CB_GETCURSEL, 0, 0); //Độ dày của đường vẽ
				switch (idx) {
				case 0:
					sizepen = 1;
					break;
				case 1:
					sizepen = 3;
					break;
				case 2:
					sizepen = 5;
					break;
				case 3:
					sizepen = 8;
					break;
				case 4:
					sizepen = 10;
					break;
				}
				SetFocus(hWnd);
			}
			if (LOWORD(wParam) == IDC_COMBOXSTYLE_PEN) {
				int idx = SendMessage(hStylePen, CB_GETCURSEL, 0, 0); //Loại đường vẽ
				switch (idx) {
				case 0:
					StylePen = PS_SOLID;
					break;
				case 1:
					StylePen = PS_DASH;
					break;
				case 2:
					StylePen = PS_DOT;
					break;
				case 3:
					StylePen = PS_DASHDOT;
					break;
				case 4:
					StylePen = PS_DASHDOTDOT;
					break;
				}
				SetFocus(hWnd);
			}
		}
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDC_TOOLBAR_OPEN: {
				TCHAR fileName[260] = L"\0";
				ofn.lpstrFile = fileName;
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::fstream f;
					f.open(ofn.lpstrFile, std::ios::in);
					int numshapes;
					f >> numshapes;
					shapes.resize(0);
					
					for (int i = 0; i < numshapes; i++) {
						CShape* temp = NULL;
						int typeshape;
						f >> typeshape;
						switch (typeshape)
						{
						case LINE:
							temp = shapefactory->createShape(F_Line);
							break;
						case RECTANGLE:
							temp = shapefactory->createShape(F_Rectangle);
							break;
						case ELIP:
							temp = shapefactory->createShape(F_Elip);
							break;
						case TRIANGLE:
							temp = shapefactory->createShape(F_Triangle);
							break;
						case PENTAGON:
							temp = shapefactory->createShape(F_Pentagon);
							break;
						case STAR:
							temp = shapefactory->createShape(F_Star);
						case ARROW:
							temp = shapefactory->createShape(F_Arrow);
							break;
						}
						int x1, x2, y1, y2, _size, _style, _solid, _hatchbrush;
						COLORREF _line, _brush;
						f >> x1 >> y1 >> x2 >> y2 >> _size >> _style >> _line >> _solid >> _hatchbrush >> _brush;
						temp->SetData(x1, y1, x2, y2, _size, _style);
						temp->SetColor(_line, _hatchbrush, _solid, _brush);
						shapes.push_back(temp);
					}
					f.close();
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
			}
				break;
			case IDC_TOOLBAR_SAVE: {
				TCHAR fileName[260] = L"\0";
				ofn.lpstrFile = fileName;
				ofn.lpstrFilter = _T("Text Files\0*.txt");
				if (GetSaveFileName(&ofn) == TRUE)
				{
					if (StrStr(fileName, L".txt") == NULL) {
						StrCat(fileName, L".txt");
					}
					std::fstream f;
					f.open(fileName, std::ios::out);
					f << shapes.size() << endl;
					for (int i = 0; i < shapes.size(); i++) {
						f << shapes[i]->typeShape() << " " << shapes[i]->coordinates().left << " " << shapes[i]->coordinates().top
							<< " " << shapes[i]->coordinates().right << " " << shapes[i]->coordinates().bottom
							<< " " << shapes[i]->get_sizeLine() << " " << shapes[i]->get_styleLine()
							<< " " << shapes[i]->get_colorLine() << " " << shapes[i]->get_solid()
							<< " " << shapes[i]->get_hatchbrush() << " " << shapes[i]->get_colorbrush() << endl;
					}
					f.close();
				}
			}
				break;
			case IDC_TOOLBAR_UNDO:
				if (shapes.size() > 0) {
					redo.push_back(shapes.back());
					shapes.pop_back();
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				break;
			case IDC_TOOLBAR_REDO:
				if (redo.size() > 0) {
					shapes.push_back(redo.back());
					redo.pop_back();
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				break;
			case IDC_TOOLBAR_COPY: {
				if (selectBMP != NULL)
				{
					OpenClipboard(NULL);
					EmptyClipboard();
					SetClipboardData(CF_BITMAP, selectBMP);
					CloseClipboard();
				}
			}
				break;
			case IDC_TOOLBAR_DELETE: {
				if (tempShape != NULL) {
					isCursorbutton = false;
					SelectedShape = false;
					tempShape = NULL;
					UpdateClientArea(hWnd);
				}
			}
				break;
			case IDC_TOOLBAR_TEXT: {
			}
				break;
			case IDC_TOOLBAR_LINE: selectedShape = 0; break;
			case IDC_TOOLBAR_REC: selectedShape = 1; break;
			case IDC_TOOLBAR_ELIP: selectedShape = 2; break;
			case IDC_TOOLBAR_TRI: selectedShape = 3; break;
			case IDC_TOOLBAR_PENTA: selectedShape = 4; break;
			case IDC_TOOLBAR_STAR: selectedShape = 5; break;
			case IDC_TOOLBAR_ARROW: selectedShape = 6; break;
			case IDC_TOOLBAR_SELECT: idbutton = IDC_TOOLBAR_SELECT; break;
			case IDC_CURSOR: {
				isCursorbutton = !isCursorbutton;
				if (isCursorbutton) {
					SetCursor(LoadCursor(NULL, IDC_CROSS));
				}
				else {
					shapes.push_back(tempShape);
					tempShape = NULL;
					UpdateWindow(hWnd);
				}
			}
				break;
			case IDC_TOOLBAR_VER:
				solid = 0;
				colorBrush = color;
				hatchbrush = HS_VERTICAL;
				break;
			case IDC_TOOLBAR_HOR:
				solid = 0;
				colorBrush = color;
				hatchbrush = HS_HORIZONTAL;
				break;
			case IDC_TOOLBAR_CROSS:
				solid = 0;
				colorBrush = color;
				hatchbrush = HS_CROSS;
				break;
			//Chọn màu 
			case IDC_TOOLBAR_DO: color = RGB(214, 10, 46); break;
			case IDC_TOOLBAR_DEN: color = RGB(0, 0, 0); break;
			case IDC_TOOLBAR_TRANG: color = RGB(255, 255, 255); break;
			case IDC_TOOLBAR_VANG: color = RGB(254, 217, 83); break;
			case IDC_TOOLBAR_TIM: color = RGB(146, 68, 194); break;
			case IDC_TOOLBAR_LAM: color = RGB(19, 113, 236); break;
			case IDC_TOOLBAR_LUC: color = RGB(147, 218, 98); break;
			case IDC_TOOLBAR_CAM: color = RGB(253, 165, 41); break;
			case IDC_TOOLBAR_CHAM: color = RGB(45, 19, 160); break;
			case IDC_TOOLBAR_HONG: color = RGB(252, 24, 149); break;
			case IDC_TOOLBAR_XANH: color = RGB(24, 166, 70); break;
			case IDC_TOOLBAR_NAU: color = RGB(137, 74, 15); break;
			case IDC_TOOLBAR_PEN: colorLine = color; break;
			case IDC_TOOLBAR_PAINTER: colorBrush = color; hatchbrush = -1; break;
			
			case IDC_EXPORT: {
				TCHAR fileName[260] = L"\0";
				ofn.lpstrFile = fileName;
				ofn.lpstrFilter = _T("24 Color Bitmap\0*.bmp");
				if (GetSaveFileName(&ofn) == TRUE)
				{
					if (StrStr(fileName, L".bmp") == NULL) {
						StrCat(fileName, L".bmp");
					}
					RECT rect;
					GetClientRect(hWnd, &rect);
					HBITMAP saveBMP = CaptureAnImage(hWnd, rect.left, rect.top + 118, rect.right, rect.bottom);
					HDC hdc = GetDC(hWnd);
					HDC hTargetDC = CreateCompatibleDC(hdc);
					PrintWindow(hWnd, hTargetDC, PW_CLIENTONLY);
					PBITMAPINFO pBI = CreateBitmapInfo(saveBMP);
					HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					SaveBMPFile(hFile, pBI, saveBMP, hTargetDC);
				}
			}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_LBUTTONDOWN: {
		//Nếu án vào chức năng chọn hình
		if (isCursorbutton) {
			SelectedShape = false;
			HDC hdc;
			PAINTSTRUCT ps;
			POINT pts;
			BOOL result;

			//Chống hiện tượng giật (Một vùng double buffer)
			RECT Rect;
			GetWindowRect(hWnd, &Rect);
			MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);

			hdc = GetDC(hWnd);//BeginPaint(hwnd, &ps);
			result = GetCursorPos(&pts);
			//Xác định vị trí của con trỏ chuột có chọn được hình nào hay không?
			if (result) 
			{
				POINT cursor_pos;
				bool leave = false;
				cursor_pos.x = pts.x - Rect.left - 10;
				cursor_pos.y = pts.y - Rect.top - 55;
				for (int i = shapes.size() - 1; i >= 0; i--) {
					if (shapes[i]->coordinates().left <= cursor_pos.x && shapes[i]->coordinates().right >= cursor_pos.x
						&& shapes[i]->coordinates().top <= cursor_pos.y && shapes[i]->coordinates().bottom >= cursor_pos.y)
					{
						if (tempShape != NULL)
							shapes.push_back(tempShape);
						selectedShapeRect->SetData(shapes[i]->coordinates().left, shapes[i]->coordinates().top, shapes[i]->coordinates().right, shapes[i]->coordinates().bottom, 1, 2);
						//Khởi tạo loại hình tương ứng
						/*ShapeFactory* shapefactory;
						CShape *shape;

						shapefactory = new Factory;*/
						switch (shapes[i]->typeShape())
						{
						case LINE: tempShape = shapefactory->createShape(F_Line); break;
						case RECTANGLE: tempShape = shapefactory->createShape(F_Rectangle); break;
						case ELIP: tempShape = shapefactory->createShape(F_Elip); break;
						case TRIANGLE: tempShape = shapefactory->createShape(F_Triangle); break;
						case PENTAGON: tempShape = shapefactory->createShape(F_Pentagon); break;
						case STAR: tempShape = shapefactory->createShape(F_Star); break;
						case ARROW: tempShape = shapefactory->createShape(F_Arrow); break;
						}
						//Lưu hình vào biến tạm để có thể thao tác thay đổi kích thước
						tempShape->SetData(shapes[i]->coordinates().left, shapes[i]->coordinates().top, shapes[i]->coordinates().right, shapes[i]->coordinates().bottom, shapes[i]->get_sizeLine(), shapes[i]->get_styleLine());
						tempShape->SetColor(shapes[i]->get_colorLine(), shapes[i]->get_hatchbrush(), shapes[i]->get_solid(), shapes[i]->get_colorbrush());

						shapes.erase(shapes.begin() + i);
						
						SelectedShape = true;

						//Vẽ lại màn hình
						RECT rec;
						GetClientRect(hWnd, &rec);
						rec.top = rec.top + 118;
						InvalidateRect(hWnd, &rec, true);
						UpdateWindow(hWnd);
						leave = true;
					}
					if (leave)
						break;
				}
				
			}
			ReleaseDC(hWnd, hdc);//EndPaint(hwnd, &ps)

			lastX = GET_X_LPARAM(lParam);
			lastY = GET_Y_LPARAM(lParam);

			if (ShowAnchorCursor(hWnd, lastX, lastY) != -1)
				isHoding = true;
			else
				isHoding = false;
		}
		else {
			redo.clear();
		}



		//Vẽ vùng chọn trong chức năng Select
		if (idbutton == IDC_TOOLBAR_SELECT ) {
			sle_x1 = GET_X_LPARAM(lParam);
			sle_y1 = GET_Y_LPARAM(lParam);
			RECT rec;
			GetClientRect(hWnd, &rec);
			rec.top = rec.top + 118;
			InvalidateRect(hWnd, &rec, true);
			UpdateWindow(hWnd);
		}
		else
			//Vẽ hình hiện tại
			if (!isDrawing && selectedShape >= 0 && !SelectedShape && !isCursorbutton) {
				isDrawing = TRUE;
				firstX = GET_X_LPARAM(lParam);
				firstY = GET_Y_LPARAM(lParam);
				/*ShapeFactory* shapefactory;
				CShape *shape;

				shapefactory = new Factory;*/
				switch (selectedShape)
				{
				case LINE: currentShape = shapefactory->createShape(F_Line) ; break;
				case RECTANGLE: currentShape = shapefactory->createShape(F_Rectangle); break;
				case ELIP: currentShape = shapefactory->createShape(F_Elip); break;
				case TRIANGLE: currentShape = shapefactory->createShape(F_Triangle); break;
				case PENTAGON: currentShape = shapefactory->createShape(F_Pentagon); break;
				case STAR: currentShape = shapefactory->createShape(F_Star); break;
				case ARROW: currentShape = shapefactory->createShape(F_Arrow); break;
				}
				currentShape->SetData(firstX, firstY, firstX, firstY, sizepen, StylePen);
				currentShape->SetColor(colorLine, hatchbrush, solid, colorBrush);
			}
	}
		break;

	case WM_MOUSEMOVE: {
		//Vị trí hiện tại của con trỏ
		lastX = GET_X_LPARAM(lParam);
		lastY = GET_Y_LPARAM(lParam);

		int anchorType = ShowAnchorCursor(hWnd, lastX, lastY); //Xác định con trỏ chuột đang chỉ anchor nào
		//Nếu đang giữ con trỏ chuột để thay đổi kích thước
		if (isHoding) {
			switch (anchorType)
			{
			case ANCHOR1: {
				selectedShapeRect->SetData(lastX, lastY, selectedShapeRect->coordinates().right, selectedShapeRect->coordinates().bottom, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(lastX, lastY, tempShape->coordinates().right, tempShape->coordinates().bottom, tempShape->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			case ANCHOR2: {
				selectedShapeRect->SetData(selectedShapeRect->coordinates().left, lastY, selectedShapeRect->coordinates().right, selectedShapeRect->coordinates().bottom, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(tempShape->coordinates().left, lastY, tempShape->coordinates().right, tempShape->coordinates().bottom, tempShape->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			case ANCHOR3: {
				selectedShapeRect->SetData(selectedShapeRect->coordinates().left, lastY, lastX, selectedShapeRect->coordinates().bottom, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(tempShape->coordinates().left, lastY, lastX, tempShape->coordinates().bottom, tempShape->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			case ANCHOR4: {
				selectedShapeRect->SetData(selectedShapeRect->coordinates().left, selectedShapeRect->coordinates().top, lastX, selectedShapeRect->coordinates().bottom, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(tempShape->coordinates().left, tempShape->coordinates().top, lastX, tempShape->coordinates().bottom, selectedShapeRect->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			case ANCHOR5: {
				selectedShapeRect->SetData(selectedShapeRect->coordinates().left, selectedShapeRect->coordinates().top, lastX, lastY, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(tempShape->coordinates().left, tempShape->coordinates().top, lastX, lastY, tempShape->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			case ANCHOR6: {
				selectedShapeRect->SetData(selectedShapeRect->coordinates().left, selectedShapeRect->coordinates().top, selectedShapeRect->coordinates().right, lastY, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(tempShape->coordinates().left, tempShape->coordinates().top, tempShape->coordinates().right, lastY, tempShape->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			case ANCHOR7: {
				selectedShapeRect->SetData(lastX, selectedShapeRect->coordinates().top, selectedShapeRect->coordinates().right, lastY, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(lastX, tempShape->coordinates().top, tempShape->coordinates().right, lastY, tempShape->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			case ANCHOR8: {
				selectedShapeRect->SetData(lastX, selectedShapeRect->coordinates().top, selectedShapeRect->coordinates().bottom, selectedShapeRect->coordinates().right, selectedShapeRect->get_sizeLine(), selectedShapeRect->get_styleLine());
				tempShape->SetData(lastX, tempShape->coordinates().top, tempShape->coordinates().bottom, tempShape->coordinates().right, tempShape->get_sizeLine(), tempShape->get_styleLine());
				UpdateClientArea(hWnd);
				break; }
			}
		}

		if (idbutton == IDC_TOOLBAR_SELECT) {
			sle_x2 = GET_X_LPARAM(lParam);
			sle_y2 = GET_Y_LPARAM(lParam);
			RECT rec;
			GetClientRect(hWnd, &rec);
			rec.top = rec.top + 118;
			InvalidateRect(hWnd, &rec, true);
			UpdateWindow(hWnd);
		}
		else
			//đang vẽ hình hiện tại
			if (isDrawing && selectedShape >= 0) {
				lastX = GET_X_LPARAM(lParam);
				lastY = GET_Y_LPARAM(lParam);
				if (GetAsyncKeyState(VK_SHIFT) < 0)
				{
					/*Special Shape*/
					if (selectedShape == RECTANGLE || selectedShape == ELIP)
					{
						int width = abs(lastX - firstX);
						int height = abs(lastY - firstY);
						int lengthX, lengthY;
						for (int i = 0; i <= width; i++)
						{
							for (int j = 0; j <= height; j++)
								if (i == j)
								{
									lengthX = i;
									lengthY = j;
								}
						}
						if (lastX < firstX)
							lastX = firstX - lengthX;
						else
							lastX = firstX + lengthX;
						if (lastY < firstY)
							lastY = firstY - lengthY;
						else
							lastY = firstY + lengthY;
					}
				}
				currentShape->SetData(firstX, firstY, lastX, lastY, sizepen, StylePen);
				currentShape->SetColor(colorLine, hatchbrush, solid, colorBrush);
				RECT rec;
				GetClientRect(hWnd, &rec);
				rec.top = rec.top + 118;
				InvalidateRect(hWnd, &rec, true);
				UpdateWindow(hWnd);
			}
	}
	break;

	case WM_LBUTTONUP:
		if (isHoding)
			isHoding = false;
		if (idbutton == IDC_TOOLBAR_SELECT) {
			idbutton = 0;
			selectBMP = CaptureAnImage(hWnd, sle_x1, sle_y1, sle_x2, sle_y2);
			sle_x1 = sle_x2 = sle_y1 = sle_y2 = 0;
		}
		else
		if (isDrawing && selectedShape >= 0) {
			shapes.push_back(currentShape);
			isDrawing = FALSE;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	case WM_ERASEBKGND:
		return TRUE;
		break;

    case WM_PAINT:
        {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT main;
		GetClientRect(hWnd, &main);
		int width = main.right - main.left;
		int height = main.bottom - main.top;
	
		// Create an off-screen DC for double-buffering
		HDC memHDC = CreateCompatibleDC(hdc); 
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height); 
		SelectObject(memHDC, hBitmap); 
		FillRect(memHDC, &main, CreateSolidBrush(RGB(255, 255, 255)));
		
		if (isCursorbutton) {
			if (tempShape != NULL) {
				DrawAnchor(memHDC, tempShape->coordinates().left, tempShape->coordinates().top, tempShape->coordinates().right, tempShape->coordinates().bottom);
				selectedShapeRect->SetColor(RGB(0, 120, 255), -1, -1, -1);
				selectedShapeRect->Draw(memHDC);
				tempShape->Draw(memHDC);
			}
		}

			// vẽ lại danh sách hình trước đó
			for (int i = 0; i < shapes.size(); i++) {
				if(shapes[i] != NULL)
				shapes[i]->Draw(memHDC);
			}
			// vẽ lại hình hiện tại
			if (isDrawing) {
				currentShape->Draw(memHDC);
			}

			if (sle_x1 != 0) {
				HPEN pen = CreatePen(2, 1, RGB(0, 0, 0));
				SelectObject(memHDC, pen);
				//SelectObject(memHDC, GetStockObject(NULL_BRUSH));
				//SetBkMode(memHDC, TRANSPARENT);

				MoveToEx(memHDC, sle_x1, sle_y1, NULL);
				LineTo(memHDC, sle_x1, sle_y2);
				MoveToEx(memHDC, sle_x1, sle_y2, NULL);
				LineTo(memHDC, sle_x2, sle_y2);
				MoveToEx(memHDC, sle_x2, sle_y2, NULL);
				LineTo(memHDC, sle_x2, sle_y1);
				MoveToEx(memHDC, sle_x2, sle_y1, NULL);
				LineTo(memHDC, sle_x1, sle_y1);
				//SetBkMode(hdc, TRANSPARENT);
				//Rectangle(hdc, sle_x1, sle_y1, sle_x2, sle_y2);
				DeleteObject(pen);
			}

			BitBlt(hdc, 0, 0, width, height, memHDC, 0, 0, SRCCOPY);//chuyển lại content về cho DC màn hình chính
			DeleteDC(memHDC);
			DeleteObject(hBitmap);
			ReleaseDC(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
//Thêm control vào trong rebar
void AddBand(HWND hControl, int height, int width, int min_height, int min_width, LPWSTR name, bool isbreak) {
	// Initialize band info used by both bands.
	REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };
	rbBand.fMask =
		RBBIM_BACKGROUND 
		| RBBIM_STYLE       // fStyle is valid.
		| RBBIM_TEXT        // lpText is valid.
		| RBBIM_CHILD       // hwndChild is valid.
		| RBBIM_CHILDSIZE   // child size members are valid.
		| RBBIM_SIZE;       // cx is valid
	if (isbreak)
		rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS | RBBS_BREAK ;
	else
	rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;

	// Set values unique to the band with the toolbar.
	rbBand.lpText = name;
	rbBand.hwndChild = hControl;
	rbBand.cch = sizeof(name);
	rbBand.cx = width;
	rbBand.cyChild = height;
	rbBand.cxMinChild = min_width;
	rbBand.cyMinChild = min_height;

	// Add the band that has the toolbar.
	SendMessage(hRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
}

bool OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{	
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(14, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// Initialize common controls.
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES | ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);

	//tạo đối tượng singleton ShapeFactory
	shapefactory = ShapeFactory::getInstance();
	selectedShapeRect = shapefactory->createShape(F_Rectangle);

	hRebar = CreateWindowEx(WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE ,
		0, 0, 0, 0, hWnd, NULL, hInst, NULL);
	
	TBBUTTON tbButtons_fun[11] =
	{
		{ 0, IDC_TOOLBAR_OPEN, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Open") },
		{ 1, IDC_TOOLBAR_SAVE, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Save") },
		{ 2, IDC_TOOLBAR_UNDO, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Undo") },
		{ 3, IDC_TOOLBAR_REDO, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0,  (LPARAM)_T("Redo") },
		{ I_IMAGENONE, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0, 0, NULL },
		{ 4, IDC_TOOLBAR_COPY, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Copy") },
		{ 5, IDC_TOOLBAR_DELETE, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Delete") },
		{ I_IMAGENONE, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0, 0, NULL },
		{ 6, IDC_TOOLBAR_PEN, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Pen") },
		{ 7, IDC_TOOLBAR_PAINTER, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Painter") },
		{ 9, IDC_TOOLBAR_SELECT, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Select") },
	};

	// Tạo toolbar chức năng
	hToolBar = CreateToolbarEx(hRebar, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER,
		IDC_TOOLBAR, 9, hInst, IDB_TOOLBAR, (LPCTBBUTTON)&tbButtons_fun, 11, 32, 32, 0, 0, sizeof(TBBUTTON));

	TBBUTTON tbButtons_brush[3] =
	{
		{ 0, IDC_TOOLBAR_VER, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Vertical") },
		{ 1, IDC_TOOLBAR_HOR, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Horizontal") },
		{ 2, IDC_TOOLBAR_CROSS, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Cross") },
	};
	//Tạo Toolbar Shape
	hToolBarBrush = CreateToolbarEx(hRebar, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER,
		IDC_TOOLBARSHAPE, 3, hInst, IDB_BRUSH, (LPCTBBUTTON)&tbButtons_brush, 3, 32, 32, 24, 24, sizeof(TBBUTTON));
	// Thay đổi kích thước toolbar
	SendMessage(hToolBarBrush, TB_AUTOSIZE, 0, (LPARAM)0);

	//Tạo Combobox chọn size Pen
	const TCHAR *itemsSize[] = { TEXT("1 px"), TEXT("3 px"), TEXT("5 px"), TEXT("8 px") , TEXT("10 px")};
	hSizeLine = CreateWindow(WC_COMBOBOX, TEXT("combobox"),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		0, 0, 20, 0, hRebar, (HMENU)IDC_COMBOBOXSIZE, hInst, NULL);
	for (int k = 0; k < 5; k++)
		SendMessage(hSizeLine, CB_ADDSTRING, 0, (LPARAM)itemsSize[k]);

	 TBBUTTON tbButtons_shape[8] =
	 {
		 { 0, IDC_TOOLBAR_LINE, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Line") },
		 { 1, IDC_TOOLBAR_REC, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Rectangle") },
		 { 2, IDC_TOOLBAR_ELIP, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Ellipse") },
		 { 3, IDC_TOOLBAR_TRI, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Triangle") },
		 { 4, IDC_TOOLBAR_PENTA, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Pentagon") },
		 { I_IMAGENONE, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0, 0, NULL },
		 { 5, IDC_TOOLBAR_STAR, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Star") },
		 { 6, IDC_TOOLBAR_ARROW, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Arrow") },
	 };
	 //Tạo Toolbar Shape
	 hToolBarShape = CreateToolbarEx(hRebar, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER,
		 IDC_TOOLBARSHAPE, 7, hInst, IDB_TOOLBARSHAPE, (LPCTBBUTTON)&tbButtons_shape, 8, 32, 32, 24, 24, sizeof(TBBUTTON));
	 // Thay đổi kích thước toolbar
	 SendMessage(hToolBarShape, TB_AUTOSIZE, 0, (LPARAM)0);

	 //Tạo Combobox chọn size Pen
	 const TCHAR *itemsStyle[] = { TEXT("Solid"), TEXT("Dash"), TEXT("Dot"), TEXT("Dash Dot"), TEXT("Dash Dot Dot")};
	 hStylePen = CreateWindow(WC_COMBOBOX, TEXT("combobox"),
		 CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		 0, 0, 20, 180, hRebar, (HMENU)IDC_COMBOXSTYLE_PEN, hInst, NULL);
	 for (int k = 0; k < 5; k++)
		 SendMessage(hStylePen, CB_ADDSTRING, 0, (LPARAM)itemsStyle[k]);

	 TBBUTTON tbExport[2] =
	 {
		 { 0, IDC_EXPORT, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Export") },
		 { 1, IDC_CURSOR, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, (LPARAM)_T("Cursor") },
	 };
	 //Tạo Toolbar Export
	 hExport = CreateToolbarEx(hRebar, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER,
		 IDC_TOOLBAR_EXPORT, 2, hInst, IDB_LASTTB, (LPCTBBUTTON)&tbExport, 2, 32, 32, 24, 24, sizeof(TBBUTTON));

	 // Thay đổi kích thước toolbar
	 SendMessage(hExport, TB_AUTOSIZE, 0, (LPARAM)0);

	 TBBUTTON tbButtons_color[12] =
	 {
		 { 0, IDC_TOOLBAR_DEN, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0},
		 { 1, IDC_TOOLBAR_TRANG, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0},
		 { 2, IDC_TOOLBAR_DO, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0},
		 { 3, IDC_TOOLBAR_VANG, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0},
		 { 4, IDC_TOOLBAR_TIM, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0},
		 { 5, IDC_TOOLBAR_LAM, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0},
		 { 6, IDC_TOOLBAR_LUC, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0},
		 { 7, IDC_TOOLBAR_CAM, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0 },
		 { 8, IDC_TOOLBAR_CHAM, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0 },
		 { 9, IDC_TOOLBAR_HONG, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0 },
		 { 10, IDC_TOOLBAR_XANH, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0 },
		 { 11, IDC_TOOLBAR_NAU, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0, 0, 0 },
	 };
	 //Tạo Toolbar Color
	 hToolBarColor = CreateToolbarEx(hRebar, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER,
		 IDC_TOOLBARSHAPE, 12, hInst, IDB_COLOR, (LPCTBBUTTON)&tbButtons_color, 12, 16, 16, 16, 16, sizeof(TBBUTTON));
	 // Thay đổi kích thước toolbar
	 SendMessage(hToolBarColor, TB_AUTOSIZE, 0, (LPARAM)0);


	//Thêm toolbar vào rebar
	AddBand( hToolBar, 50, 0, 50, 450, TEXT(""), false);
	AddBand(hToolBarColor, 0, 0, 16, 275, TEXT(""), false);
	AddBand(hToolBarBrush, 0, 0, 0, 190, TEXT(""), false);
	AddBand(hToolBarShape, 0, 0, 55, 420, TEXT(""), true);
	AddBand(hSizeLine, 0, 155, 20, 150, TEXT("Size"), false);
	AddBand(hStylePen, 0, 160, 20, 165, TEXT("Style Pen"), false);
	AddBand(hExport, 0, 0, 0, 89, TEXT(""), false);

	return true;
}
//Giới hạn kích thước cửa sổ chương trình
void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo) {
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lpMinMaxInfo;
	lpMMI->ptMinTrackSize.x = 980;
	lpMMI->ptMinTrackSize.y = 600;
}
//Xử lý anchor
void DrawAnchor(HDC hdc, int x1, int y1, int x2, int y2)
{
	int midX = (x2 + x1) / 2;
	int midY = (y2 + y1) / 2;

	SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));

	Rectangle(hdc, x1 - 3, y1 + 3, x1 + 3, y1 - 3); // ve anchor o top-left
	Rectangle(hdc, x2 - 3, y1 + 3, x2 + 3, y1 - 3); //ve anchor o top-right
	Rectangle(hdc, x1 - 3, y2 + 3, x1 + 3, y2 - 3); // ve anchor o bottom-left
	Rectangle(hdc, x2 - 3, y2 + 3, x2 + 3, y2 - 3); // ve anchor o bottom-right
	Rectangle(hdc, midX - 3, y1 + 3, midX + 3, y1 - 3); // ve anchor o giua tren
	Rectangle(hdc, midX - 3, y2 + 3, midX + 3, y2 - 3); // ve anchor o giua duoi
	Rectangle(hdc, x1 - 3, midY + 3, x1 + 3, midY - 3); // ve anchor o giua trai
	Rectangle(hdc, x2 - 3, midY + 3, x2 + 3, midY - 3); // ve anchor o giua phai
}
int ShowAnchorCursor(HWND hWnd, int X, int Y)
{
	int left = selectedShapeRect->coordinates().left;
	int top = selectedShapeRect->coordinates().top;
	int right = selectedShapeRect->coordinates().right;
	int bottom = selectedShapeRect->coordinates().bottom;
	if (X >= left - 20 && X <= left + 20 && Y >= top - 20 && Y <= top + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		return ANCHOR1;
	}
	else if (X >= (left + right) / 2 - 20 && X <= (left + right) / 2 + 20 && Y >= top - 20 && Y <= bottom + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
		return ANCHOR2;
	}
	else if (X >= right - 20 && X <= right + 20 && Y >= top - 20 && Y <= top + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		return ANCHOR3;
	}
	else if (X >= right - 20 && X <= right + 20 && Y >= (top + bottom) / 2 - 20 && Y <= (top + bottom) / 2 + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		return ANCHOR4;
	}
	else if (X >= right - 20 && X <= right + 20 && Y >= bottom - 20 && Y <= bottom + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		return ANCHOR5;
	}
	else if (X >= (left + right) / 2 - 20 && X <= (left + right) / 2 + 20 && Y >= bottom - 20 && Y <= bottom + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
		return ANCHOR6;
	}
	else if (X >= left - 20 && X <= left + 20 && Y >= bottom - 20 && Y <= bottom + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		return ANCHOR7;
	}
	else if (X >= left - 20 && X <= left + 20 && Y >= (top + bottom) / 2 - 20 && Y <= (top + bottom) / 2 + 20)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		return ANCHOR8;
	}
	return -1;
}
//Cập nhật khu vực vẽ hình
void UpdateClientArea(HWND hWnd) {
	RECT rec;
	GetClientRect(hWnd, &rec);
	rec.top = rec.top + 118;
	InvalidateRect(hWnd, &rec, true);
	UpdateWindow(hWnd);
}

