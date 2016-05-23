#include <windows.h> // Include all the windows headers.
#include <windowsx.h> // Include useful macros.
#include <commctrl.h>
#include <strstream>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

// Paint Tool library
#include "h\canvas.h"
#include "h\line.h"
#include "h\rectangle.h"
#include "h\ellipse.h"
#include "h\polygon.h"
#include "h\stamp.h"

// Others
#include "util.h"
#include "resource.h"

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;

// The two main windows
#define WINDOW_CLASS_NAME  L"WINCLASS1" // Toolbar, menu
#define WINDOW_CLASS_PANEL L"WINCLASS2" // Drawing canvas

HWND g_hwnd;
HMENU hMenu;
HWND hwndPanel;
HINSTANCE g_hInstance;
CCanvas* g_canvas;
IShape* currentShape;
EShape g_shape;

// Buttons //
HICON hIcon;
HWND button_new;
HWND button_open;
HWND button_save;
HWND button_properties;
HWND button_fillColor;
HWND button_penColor;

// Brushes //
//HWND brush_pen;
HWND brush_line;
HWND brush_rect;
HWND brush_ellipse;
HWND brush_polygon;
HWND brush_stamp;

// Trackbar //
HWND hTrack;
HWND hlbl;

// Brush properties //
COLORREF g_fillColor = RGB(0, 0, 0);
COLORREF g_penColor = RGB(0, 0, 0);
EBRUSHSTYLE g_brushStyle = SOLID;
int g_hatchStyle = 0;
int g_penStyle = 0;
int g_penWidth = 0;

// Temporary shapes //
CPolygon* tempPolygon = NULL;
HBITMAP tempBitmap = NULL;
CStamp* tempStamp = NULL;
POINT tempStartPoint;
POINT tempEndPoint;

// Function declarations //
LRESULT CALLBACK WindowProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);
LRESULT CALLBACK PanelProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);
COLORREF ShowColorDialog(HWND hwnd);
void RegisterPanel(HWND _hwnd);
void CreateButtons(HWND _hwnd);
void CreateMenubar(HWND);
HBITMAP OpenBitmapImage(HWND);
void UpdateLabel();

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //

//////////////////////////////////////////////////////////
//				~~	MAIN FUNCTION	~~					//
//////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE _hInstance,
	HINSTANCE _hPrevInstance,
	LPSTR _lpCmdLine,
	int _nCmdShow)
{

	WNDCLASSEX wndMenu;  // This will hold the class we create.
	HWND hwnd;			 // Generic window handle.
	MSG msg;			 // Generic message.

						 // Extra GDI utility
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	//// First fill in the window class structure ////
	wndMenu.cbSize = sizeof(WNDCLASSEX);
	wndMenu.style = CS_DBLCLKS | CS_OWNDC | CS_VREDRAW;
	wndMenu.lpfnWndProc = WindowProc;
	wndMenu.cbClsExtra = 0;
	wndMenu.cbWndExtra = 0;
	wndMenu.hInstance = _hInstance;
	wndMenu.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
	wndMenu.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndMenu.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wndMenu.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wndMenu.lpszClassName = WINDOW_CLASS_NAME;
	wndMenu.hIconSm = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ICON_APP));

	// Register the window class
	if (!RegisterClassEx(&wndMenu)) {
		return (0);
	}

	// Initialize GDI+
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Create the menu
	hMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDR_MENU));

	// Centering the window based on the screen's resolution
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	// Center it based on the screens resolution minus the window's dimensions
	int centerX = (screenX / 2) - (WIDTH / 2);
	int centerY = (screenY / 2) - (HEIGHT / 2);

	// Creating the window
	hwnd = CreateWindow(WINDOW_CLASS_NAME,
		L"Paint Tool v1.0.0 by Juan Rodriguez",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		centerX, centerY, WIDTH, HEIGHT,
		NULL, hMenu, _hInstance, NULL);

	if (!(hwnd)) {
		return (0);
	}

	g_hwnd = hwnd;
	g_hInstance = _hInstance;

	//// Main event loop ////
	while (true) {

		// Test if there is a message in queue, if so get it.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			// Test if this is a quit.
			if (msg.message == WM_QUIT) {
				break;
			}

			// Translate any accelerator keys.
			TranslateMessage(&msg);
			// Send the message to the window proc.
			DispatchMessage(&msg);

		}

	}

	// Shutdown the GDI+ token
	GdiplusShutdown(gdiplusToken);

	// Return to Windows like this...
	return (static_cast<int>(msg.wParam));

}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //

//////////////////////////////////////////////////////////
//					MAIN PROCESS						//
//////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{

	switch (_msg) {

	case WM_CREATE:
	{

		CreateButtons(_hwnd);
		RegisterPanel(_hwnd);

		return (0);

	}
	break;

	case WM_COMMAND:
	{

		switch (LOWORD(_wparam)) {

			//// File Menu ////
		case IDM_FILE_NEW:
			tempBitmap = NULL;
			RegisterPanel(_hwnd);
			break;
		case IDM_FILE_OPEN:
			tempBitmap = OpenBitmapImage(_hwnd);
			RegisterPanel(_hwnd);
			break;
		case IDM_FILE_SAVE:
			g_canvas->Save(hwndPanel);
			break;
		case IDM_FILE_EXIT:
			PostQuitMessage(0);
			break;
			// Help menu
		case IDM_HELP_ABOUT:
			MessageBoxA(_hwnd, "Paint Tool v1\nUniversity: Media Design School\nAuthor: Juan Rodriguez\nContact: trodz24@gmail.com\n", "About", MB_OK);
			break;
		case IDM_HELP_CONTROLS:
			MessageBoxA(_hwnd,
				"POLYGONS\nTo create a polygon, click and hold the LEFT mouse button while you click the RIGHT mouse button to add a point. Once satisfied, release the LEFT mouse to successfully add the shape to your canvas.\n\n"
				"STAMPS\nTo add a stamp, click the stamp button, load a .bmp file and clik the RIGHT mouse button on the canvas to add it.\n\n"
				"To undo any shape you've made\tCTRL + Z\nTo redo any shape you've undoed\tCTRL + Y\n", "Controls", MB_OK);
			break;
			//// Pen styles ////
			{
		case ID_PEN_SOLID:
		{
			g_penStyle = PS_SOLID;
			CheckMenuItem(hMenu, ID_PEN_SOLID, MF_CHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DOTTED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOTDOT, MF_UNCHECKED);
		}
		break;
		case ID_PEN_DASHED:
		{
			g_penStyle = PS_DASH;
			CheckMenuItem(hMenu, ID_PEN_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHED, MF_CHECKED);
			CheckMenuItem(hMenu, ID_PEN_DOTTED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOTDOT, MF_UNCHECKED);
		}
		break;
		case ID_PEN_DOTTED:
		{
			g_penStyle = PS_DOT;
			CheckMenuItem(hMenu, ID_PEN_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DOTTED, MF_CHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOTDOT, MF_UNCHECKED);
		}
		break;
		case ID_PEN_DASHDOT:
		{
			g_penStyle = PS_DASHDOT;
			CheckMenuItem(hMenu, ID_PEN_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DOTTED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOT, MF_CHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOTDOT, MF_UNCHECKED);
		}
		break;
		case ID_PEN_DASHDOTDOT:
		{
			g_penStyle = PS_DASHDOTDOT;
			CheckMenuItem(hMenu, ID_PEN_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DOTTED, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_PEN_DASHDOTDOT, MF_CHECKED);
		}
		break;
			}

			//// Brush styles ////
			{
		case ID_BRUSH_SOLID:
		{
			g_brushStyle = SOLID;
			CheckMenuItem(hMenu, ID_BRUSH_SOLID, MF_CHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_DIAGONAL, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_STRAIGHT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_UP, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_DOWN, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_HOR, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_VER, MF_UNCHECKED);
		}
		break;
		case ID_HATCH_CROSS_DIAGONAL:
		{
			g_brushStyle = HATCH;
			g_hatchStyle = HS_BDIAGONAL;
			CheckMenuItem(hMenu, ID_BRUSH_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_DIAGONAL, MF_CHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_STRAIGHT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_UP, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_DOWN, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_HOR, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_VER, MF_UNCHECKED);
		}
		break;
		case ID_HATCH_CROSS_STRAIGHT:
		{
			g_brushStyle = HATCH;
			g_hatchStyle = HS_CROSS;
			CheckMenuItem(hMenu, ID_BRUSH_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_DIAGONAL, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_STRAIGHT, MF_CHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_UP, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_DOWN, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_HOR, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_VER, MF_UNCHECKED);
		}
		break;

		case ID_HATCH_DIAGONAL_UP:
		{
			g_brushStyle = HATCH;
			g_hatchStyle = HS_DIAGCROSS;
			CheckMenuItem(hMenu, ID_BRUSH_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_DIAGONAL, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_STRAIGHT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_UP, MF_CHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_DOWN, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_HOR, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_VER, MF_UNCHECKED);
		}
		break;

		case ID_HATCH_DIAGONAL_DOWN:
		{
			g_brushStyle = HATCH;
			g_hatchStyle = HS_FDIAGONAL;
			CheckMenuItem(hMenu, ID_BRUSH_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_DIAGONAL, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_STRAIGHT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_UP, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_DOWN, MF_CHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_HOR, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_VER, MF_UNCHECKED);
		}
		break;

		case ID_HATCH_LINE_HOR:
		{
			g_brushStyle = HATCH;
			g_hatchStyle = HS_HORIZONTAL;
			CheckMenuItem(hMenu, ID_BRUSH_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_DIAGONAL, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_STRAIGHT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_UP, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_DOWN, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_HOR, MF_CHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_VER, MF_UNCHECKED);
		}
		break;

		case ID_HATCH_LINE_VER:
		{
			g_brushStyle = HATCH;
			g_hatchStyle = HS_VERTICAL;
			CheckMenuItem(hMenu, ID_BRUSH_SOLID, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_DIAGONAL, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_CROSS_STRAIGHT, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_UP, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_DIAGONAL_DOWN, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_HOR, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_HATCH_LINE_VER, MF_CHECKED);
		}
		break;
			}

			//// Toolbar ////
			// Line
		case IDI_ICON_BRUSH:
			g_shape = LINE;
			Button_SetState(brush_line, 8);
			Button_SetState(brush_rect, 0);
			Button_SetState(brush_ellipse, 0);
			Button_SetState(brush_polygon, 0);
			Button_SetState(brush_stamp, 0);
			break;
			// Rectangle
		case IDI_ICON_RECTANGLE:
			g_shape = RECTANGLE;
			Button_SetState(brush_line, 0);
			Button_SetState(brush_rect, 8);
			Button_SetState(brush_ellipse, 0);
			Button_SetState(brush_polygon, 0);
			Button_SetState(brush_stamp, 0);
			break;
			// Ellipse
		case IDI_ICON_ELLIPSE:
			g_shape = ELLIPSE;
			Button_SetState(brush_line, 0);
			Button_SetState(brush_rect, 0);
			Button_SetState(brush_ellipse, 8);
			Button_SetState(brush_polygon, 0);
			Button_SetState(brush_stamp, 0);
			break;
		case IDI_ICON_POLYGON:
			g_shape = POLYGON;
			Button_SetState(brush_line, 0);
			Button_SetState(brush_rect, 0);
			Button_SetState(brush_ellipse, 0);
			Button_SetState(brush_polygon, 8);
			Button_SetState(brush_stamp, 0);
			break;
		case IDI_ICON_STAMP:
			tempBitmap = OpenBitmapImage(_hwnd);
			g_shape = STAMP;
			Button_SetState(brush_line, 0);
			Button_SetState(brush_rect, 0);
			Button_SetState(brush_ellipse, 0);
			Button_SetState(brush_polygon, 0);
			Button_SetState(brush_stamp, 8);
			break;
		case IDI_ICON_COLOR:
			g_fillColor = ShowColorDialog(_hwnd);
			break;
		case IDI_ICON_PENCOLOR:
			g_penColor = ShowColorDialog(_hwnd);
			break;
		default: break;

		}

		return (0);

	}
	break;

	case WM_HSCROLL:
	{
		UpdateLabel();
	}
	break;
	// Key combo listener
	case WM_HOTKEY:
	{

		switch (_wparam)
		{
			// CTRL + N
		case 1:
			if (MessageBoxA(_hwnd, "Do you wish to create a new canvas from scratch?", "Confirm", MB_OKCANCEL | MB_ICONEXCLAMATION) == 1) // Ok = 1
				RegisterPanel(_hwnd);
			break;
			// CTRL + O
		case 2:
			//OpenBitmapImage(_hwnd);
			if (MessageBoxA(_hwnd, "Do you wish to open a new image and set it as your current canvas?", "Confirm", MB_OKCANCEL | MB_ICONEXCLAMATION) == 1) { // Ok = 1

				tempBitmap = OpenBitmapImage(_hwnd);
				RegisterPanel(_hwnd);

			}
			break;
			// CTRL + S
		case 3:
			if (MessageBoxA(_hwnd, "Do you wish to save your canvas?", "Confirm", MB_OKCANCEL | MB_ICONEXCLAMATION) == 1) // Ok = 1
				g_canvas->Save(hwndPanel);
			break;
			// CTRL + Q
		case 4:
			if (MessageBoxA(_hwnd, "Do you wish to exit?", "Confirm", MB_OKCANCEL | MB_ICONEXCLAMATION) == 1) // Ok = 1
				PostQuitMessage(0);
			break;
			// CTRL + Z
		case 5:
			g_canvas->Undo();
			break;
			// CTRL + Y
		case 6:
			g_canvas->Redo();
			break;
		}

	}
	break;

	case WM_DESTROY:
	{
		// Kill the application, this sends a WM_QUIT message.
		PostQuitMessage(0);
		// Return success.
		return (0);

	}
	break;

	default: break;

	} // End switch.

	  // Process any messages that we did not take care of...
	return (DefWindowProc(_hwnd, _msg, _wparam, _lparam));

}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //

//////////////////////////////////////////////////////////
//					PANEL PROCESS						//
//////////////////////////////////////////////////////////
LRESULT CALLBACK PanelProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{

	PAINTSTRUCT ps; // Used in WM_PAINT.
	HDC hdc; // Handle to a device context.

			 // Positions for drawing
	static bool bIsDrawing = false;
	static int startX, startY;

	switch (_msg) {

	case WM_PAINT:
	{

		hdc = BeginPaint(_hwnd, &ps);

		if (g_canvas != NULL)
			g_canvas->Draw(hdc);

		EndPaint(_hwnd, &ps);

		return (0);

	}
	break;
	// Moving the mouse
	case WM_LBUTTONDOWN:
	{

		int brushType;

		switch (g_shape) {

		case LINE:
			currentShape = new CLine(g_penStyle, g_penWidth, g_penColor);
			brushType = Button_GetState(brush_line);
			break;
		case RECTANGLE:
			currentShape = new CRectangle(g_brushStyle, g_hatchStyle, g_fillColor, g_penStyle, g_penWidth, g_penColor);
			brushType = Button_GetState(brush_rect);
			break;
		case ELLIPSE:
			currentShape = new CEllipse(g_brushStyle, g_hatchStyle, g_fillColor, g_penStyle, g_penWidth, g_penColor);
			brushType = Button_GetState(brush_ellipse);
			break;
		case POLYGON:
			tempPolygon = new CPolygon(g_brushStyle, g_hatchStyle, g_fillColor, g_penStyle, g_penWidth, g_penColor);
			brushType = Button_GetState(brush_polygon);
			break;
		case STAMP:
			brushType = Button_GetState(button_open);
		default:break;

		}

		// We can start drawing
		if (brushType != 0) {

			if (g_shape != POLYGON && g_shape != STAMP) {

				currentShape->SetStartX(static_cast<int>(LOWORD(_lparam)));
				currentShape->SetStartY(static_cast<int>(HIWORD(_lparam)));
				g_canvas->AddShape(currentShape);
				bIsDrawing = true;

			}
			else if (g_shape == POLYGON) {

				tempStartPoint.x = static_cast<int>(LOWORD(_lparam));
				tempStartPoint.y = static_cast<int>(HIWORD(_lparam));
				tempPolygon->AddPoint(tempStartPoint);
				bIsDrawing = true;

			}

		}

		return (0);

	}
	break;

	case WM_MOUSEMOVE:
	{

		if (bIsDrawing == true && g_shape != POLYGON) {

			currentShape->SetEndX(static_cast<int>(LOWORD(_lparam)));
			currentShape->SetEndY(static_cast<int>(HIWORD(_lparam)));
			InvalidateRect(_hwnd, NULL, false);

		}

		return (0);

	}
	break;

	case WM_LBUTTONUP:
	{

		if (bIsDrawing == true) {

			if (g_shape != POLYGON) {

				currentShape->SetEndX(static_cast<int>(LOWORD(_lparam)));
				currentShape->SetEndY(static_cast<int>(HIWORD(_lparam)));
				bIsDrawing = false;

			}
			else {

				if (tempPolygon->GetPoints() >= 2) {

					g_canvas->AddShape(tempPolygon);
					bIsDrawing = false;

				}

			}

			InvalidateRect(_hwnd, NULL, false);

		}

		return (0);

	}
	break;

	case WM_RBUTTONUP:
	{

		if (bIsDrawing == true && g_shape == POLYGON) {

			tempEndPoint.x = static_cast<int>(LOWORD(_lparam));
			tempEndPoint.y = static_cast<int>(HIWORD(_lparam));
			tempPolygon->AddPoint(tempEndPoint);

		}

		if (g_shape == STAMP) {

			startX = static_cast<int>(LOWORD(_lparam));
			startY = static_cast<int>(HIWORD(_lparam));

			tempStamp = new CStamp(tempBitmap, startX, startY);
			g_canvas->AddShape(tempStamp);
			Button_SetState(brush_stamp, 0);
			InvalidateRect(_hwnd, NULL, false);
			g_shape = NONE;

		}

		return (0);
	}
	break;

	}

	// Process any messages that we did not take care of...
	return (DefWindowProc(_hwnd, _msg, _wparam, _lparam));

}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //

void CreateButtons(HWND _hwnd) {

	//// TOOLS ////
	// ----------------------------- New
	button_new = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		0, 0, 44, 44,
		_hwnd,
		(HMENU)IDM_FILE_NEW, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_NEW));
	SendMessage(button_new, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Open
	button_open = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		44, 0, 44, 44,
		_hwnd,
		(HMENU)IDM_FILE_OPEN, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_OPEN));
	SendMessage(button_open, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Save
	button_save = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		88, 0, 44, 44,
		_hwnd,
		(HMENU)IDM_FILE_SAVE, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_SAVE));
	SendMessage(button_save, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Color picker
	button_fillColor = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		396, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_COLOR, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_COLOR));
	SendMessage(button_fillColor, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Color picker
	button_penColor = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		440, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_PENCOLOR, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_PENCOLOR));
	SendMessage(button_penColor, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	//// BRUSHES ////
	// ----------------------------- Pen
	/*brush_pen = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		132, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_PEN, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_PEN));
	SendMessage(brush_pen, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);*/

	// ----------------------------- Line
	brush_line = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		176, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_BRUSH, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_BRUSH));
	SendMessage(brush_line, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Rectangle
	brush_rect = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		220, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_RECTANGLE, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_RECTANGLE));
	SendMessage(brush_rect, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Ellipse
	brush_ellipse = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		264, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_ELLIPSE, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_ELLIPSE));
	SendMessage(brush_ellipse, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Polygon
	brush_polygon = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		308, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_POLYGON, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_POLYGON));
	SendMessage(brush_polygon, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	// ----------------------------- Polygon
	brush_stamp = CreateWindow(
		L"BUTTON", L"",
		WS_CHILD | WS_VISIBLE | BS_ICON,
		352, 0, 44, 44,
		_hwnd,
		(HMENU)IDI_ICON_STAMP, GetModuleHandle(NULL), NULL
	);
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_STAMP));
	SendMessage(brush_stamp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

	//// Width trackbar ////
	HWND hLeftLabel = CreateWindowW(L"Static", L"1",
		WS_CHILD | WS_VISIBLE, 0, 0, 10, 15, _hwnd, (HMENU)1, NULL, NULL);

	HWND hRightLabel = CreateWindowW(L"Static", L"10",
		WS_CHILD | WS_VISIBLE, 0, 0, 30, 15, _hwnd, (HMENU)2, NULL, NULL);

	hlbl = CreateWindowW(L"Static", L"Width: 1", WS_CHILD | WS_VISIBLE,
		500, 13, 65, 30, _hwnd, (HMENU)3, NULL, NULL);

	INITCOMMONCONTROLSEX icex;

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	hTrack = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control",
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
		590, 7, 70, 30, _hwnd, (HMENU)3, NULL, NULL);

	SendMessageW(hTrack, TBM_SETRANGE, TRUE, MAKELONG(1, 10));
	SendMessageW(hTrack, TBM_SETPAGESIZE, 0, 10);
	SendMessageW(hTrack, TBM_SETTICFREQ, 10, 0);
	SendMessageW(hTrack, TBM_SETPOS, FALSE, 0);
	SendMessageW(hTrack, TBM_SETBUDDY, TRUE, (LPARAM)hLeftLabel);
	SendMessageW(hTrack, TBM_SETBUDDY, FALSE, (LPARAM)hRightLabel);

}

void RegisterPanel(HWND _hwnd) {

	WNDCLASSW rwc = { 0 };
	rwc.lpszClassName = WINDOW_CLASS_PANEL;
	rwc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	rwc.lpfnWndProc = PanelProc;
	RegisterClassW(&rwc);

	if (tempBitmap == NULL) {

		int sizeX = GetSystemMetrics(SM_CXSCREEN);
		int sizeY = GetSystemMetrics(SM_CYSCREEN);

		hwndPanel = CreateWindow(WINDOW_CLASS_PANEL, NULL,
			WS_CHILD | WS_VISIBLE,
			0, 44,
			sizeX, sizeY,
			_hwnd, (HMENU)1, g_hInstance, NULL);

		g_canvas = new CCanvas();
		g_canvas->Initialize(hwndPanel, sizeX, sizeY);

	}
	else {

		BITMAP bitmap;
		GetObject(tempBitmap, sizeof(BITMAP), &bitmap);

		hwndPanel = CreateWindow(WINDOW_CLASS_PANEL, NULL,
			WS_CHILD | WS_VISIBLE,
			0, 44,
			bitmap.bmWidth + 1, bitmap.bmHeight + 2,
			_hwnd, (HMENU)1, g_hInstance, NULL);

		g_canvas = new CCanvas();
		g_canvas->Initialize(hwndPanel, bitmap.bmWidth + 1, bitmap.bmHeight + 2);

		InvalidateRect(_hwnd, NULL, true);

		tempStamp = new CStamp(tempBitmap, 0, 1);

		g_canvas->AddShape(tempStamp);

		tempBitmap = NULL;

	}

	// Register the hotkeys
	RegisterHotKey(_hwnd, 1, MOD_CONTROL | MOD_NOREPEAT, 'N'); // CTRL + N
	RegisterHotKey(_hwnd, 2, MOD_CONTROL | MOD_NOREPEAT, 'O'); // CTRL + N
	RegisterHotKey(_hwnd, 3, MOD_CONTROL | MOD_NOREPEAT, 'S'); // CTRL + S
	RegisterHotKey(_hwnd, 4, MOD_CONTROL | MOD_NOREPEAT, 'Q'); // CTRL + Q
	RegisterHotKey(_hwnd, 5, MOD_CONTROL, 'Z'); // CTRL + Z
	RegisterHotKey(_hwnd, 6, MOD_CONTROL, 'Y'); // CTRL + Y

}

void LoadPanel(HWND _hwnd) {



}

COLORREF ShowColorDialog(HWND hwnd) {

	CHOOSECOLOR cc;
	static COLORREF crCustClr[16];

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)crCustClr;
	cc.rgbResult = g_fillColor;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);

	return cc.rgbResult;

}

HBITMAP OpenBitmapImage(HWND _hwnd) {

	//if (g_shape == STAMP && tempBitmap == NULL) {

	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"Bitmap Files (*.bmp)\0*.bmp\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"bmp";

	// Do something usefull with the filename stored in szFileName 
	if (GetOpenFileName(&ofn)) {

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(g_hInstance, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		BITMAP bitmap;
		GetObject(bmp, sizeof(BITMAP), &bitmap);
		return bmp;

	}

	return NULL;

}

void UpdateLabel() {

	LRESULT pos = SendMessageW(hTrack, TBM_GETPOS, 0, 0);
	wchar_t buf[10];
	wsprintfW(buf, L"Width: %ld", pos);
	g_penWidth = pos;
	SetWindowTextW(hlbl, buf);

}