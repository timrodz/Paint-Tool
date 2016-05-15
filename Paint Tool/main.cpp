//#define WIN32_LEAN_AND_MEAN

#include <windows.h> // Include all the windows headers.
#include <windowsx.h> // Include useful macros.
#include <commctrl.h>
#include <strstream>
#include <string>

// Paint Tool library
#include "h\canvas.h"
#include "h\line.h"
#include "h\rectangle.h"
#include "h\ellipse.h"

// Others
#include "util.h"
#include "resource.h"

// Macros
#define ID_BLUE 1
#define ID_YELLOW 2
#define ID_ORANGE 3

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;

// The two main windows
#define WINDOW_CLASS_NAME     L"WINCLASS1" // Toolbar, menu
#define WINDOW_CLASS_PANEL    L"WINCLASS2" // Drawing canvas
#define WINDOW_CLASS_PROPERTY L"WINCLASS3" // Pop up brush property menu

HWND g_hwnd;
HINSTANCE g_hInstance;
CCanvas* g_canvas;
IShape* currentShape;
EShape g_shape;

// Drawing properties


// Buttons //
static HWND button_new;
static HWND button_open;
static HWND button_save;
static HWND button_color;
static HWND button_properties;

// Brushes //
static HWND brush_line;
static HWND brush_rect;
static HWND brush_ellipse;

// Brush properties
bool g_bClearCanvas;
COLORREF g_fillColor = RGB(0, 0, 0);
COLORREF g_penColor = RGB(0, 0, 0);
EBRUSHSTYLE g_brushStyle;
int g_hatchStyle;
int g_penStyle;
int g_width;

// Function declarations //
void RegisterPanel();
void RegisterPropertyMenu();
LRESULT CALLBACK WindowProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);
LRESULT CALLBACK PanelProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);
LRESULT CALLBACK PropertyProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);
COLORREF ShowColorDialog(HWND hwnd);
void CreateMenubar(HWND);
void OpenDialog(HWND);

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

	// Create the menu
	HMENU _hMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDR_MENU));

	// Centering the window based on the screen's resolution
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	// Center it based on the screens resolution minus the window's dimensions
	int centerX = (screenX / 2) - (WIDTH / 2);
	int centerY = (screenY / 2) - (HEIGHT / 2);

	// Creating the window
	hwnd = CreateWindow(WINDOW_CLASS_NAME,
		L"Paint Tool by Juan Rodriguez",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		centerX, centerY, WIDTH, HEIGHT,
		NULL, _hMenu, _hInstance, NULL);

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


		// Main game processing goes here.
		//GameLoop(); //One frame of game logic occurs here...

	}

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
void RegisterPanel() {

	WNDCLASSW rwc = { 0 };
	rwc.lpszClassName = WINDOW_CLASS_PANEL;
	rwc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	rwc.lpfnWndProc = PanelProc;
	RegisterClassW(&rwc);

}

void RegisterPropertyMenu() {

	WNDCLASSW rwc = { 0 };
	rwc.lpszClassName = WINDOW_CLASS_PROPERTY;
	rwc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	rwc.lpfnWndProc = PropertyProc;
	RegisterClassW(&rwc);

}

LRESULT CALLBACK WindowProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{

	static HWND hwndPanel;
	static HWND hwndProperty;

	switch (_msg) {

	case WM_CREATE:
	{

		HICON hIcon;

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
		button_color = CreateWindow(
			L"BUTTON", L"",
			WS_CHILD | WS_VISIBLE | BS_ICON,
			132, 0, 44, 44,
			_hwnd,
			(HMENU)IDI_ICON_COLOR, GetModuleHandle(NULL), NULL
		);
		hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_COLOR));
		SendMessage(button_color, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

		// ----------------------------- Brush Properties
		button_properties = CreateWindow(
			L"BUTTON", L"",
			WS_CHILD | WS_VISIBLE | BS_ICON,
			352, 0, 44, 44,
			_hwnd,
			(HMENU)IDI_ICON_PROPERTY, GetModuleHandle(NULL), NULL
		);
		hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_PROPERTY));
		SendMessage(button_properties, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

		//// BRUSHES ////
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

		//// Registering the canvas and creating the drawing panel ////

		g_canvas = new CCanvas();

		RegisterPanel();

		// Drawing panel
		hwndPanel = CreateWindowW(WINDOW_CLASS_PANEL, NULL,
			WS_CHILD | WS_VISIBLE,
			0, 44,
			GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), // Basically the size of the current window
			_hwnd, (HMENU)1, NULL, NULL);

		// Register the hotkeys
		RegisterHotKey(_hwnd, 1, MOD_CONTROL | MOD_NOREPEAT, 'N'); // CTRL + N
		RegisterHotKey(_hwnd, 2, MOD_CONTROL | MOD_NOREPEAT, 'O'); // CTRL + N
		RegisterHotKey(_hwnd, 3, MOD_CONTROL | MOD_NOREPEAT, 'S'); // CTRL + S
		RegisterHotKey(_hwnd, 4, MOD_SHIFT | MOD_CONTROL | MOD_NOREPEAT, 'N'); // SHIFT + CTRL + S
		RegisterHotKey(_hwnd, 5, MOD_CONTROL | MOD_NOREPEAT, 'Q'); // CTRL + Q
		RegisterHotKey(_hwnd, 6, MOD_CONTROL | MOD_NOREPEAT, 'Z'); // CTRL + Z
		RegisterHotKey(_hwnd, 7, MOD_CONTROL | MOD_NOREPEAT, 'Y'); // CTRL + Y

		g_canvas->Initialize(hwndPanel, WIDTH, HEIGHT);

		return (0);

	}
	break;

	case WM_COMMAND:
	{

		switch (LOWORD(_wparam)) {

			// File Menu
		case IDM_FILE_NEW:

			break;
		case IDM_FILE_OPEN:
			OpenDialog(_hwnd);
			break;
		case IDM_FILE_SAVE:

			break;
		case IDM_FILE_SAVE_AS:

			break;
		case IDM_FILE_EXIT:
			PostQuitMessage(0);
			break;
			// Help menu
		case IDM_HELP_ABOUT:
			MessageBox(_hwnd, L"Paint Tool v1\nUniversity: Media Design School\nAuthor: Juan Rodriguez\nContact: trodz24@gmail.com\n", L"About", MB_OK);
			break;
		case IDI_ICON_COLOR:
			g_fillColor = ShowColorDialog(_hwnd);
			break;
			// Drawing buttons
			// Line
		case IDI_ICON_BRUSH:
			g_shape = LINE;
			break;
			// Rectangle
		case IDI_ICON_RECTANGLE:
			g_shape = BOX;
			break;
			// Ellipse
		case IDI_ICON_ELLIPSE:
			g_shape = ELLIPSE;
			break;
		case IDI_ICON_PROPERTY:
		{
			// Position it according to where the window is at
			RegisterPropertyMenu();

			// Drawing panel
			hwndProperty = CreateWindowEx(WS_EX_TOOLWINDOW, WINDOW_CLASS_PROPERTY, L"Properties",
				WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE | WS_CHILD,
				200, 120, 400, 320,
				_hwnd, NULL, g_hInstance, NULL);

		}
		default: break;

		}

		return (0);

	}
	break;

	// Key combo listener
	case WM_HOTKEY:
	{
	case 'N':
		MessageBoxA(_hwnd, "CTRL + N", "IMPORTANT", MB_OK);
		SendMessage(button_new, BM_CLICK, 0, 0);
		break;
	case 'O':
		MessageBoxA(_hwnd, "CTRL + O", "IMPORTANT", MB_OK);
		SendMessage(button_new, BM_CLICK, 0, 0);
		break;
	case 'S':
		MessageBoxA(_hwnd, "CTRL + S", "IMPORTANT", MB_OK);
		SendMessage(button_new, BM_CLICK, 0, 0);
		break;
	case 'Q':
		MessageBoxA(_hwnd, "CTRL + Q", "IMPORTANT", MB_OK);
		PostQuitMessage(0);
		break;
	case 'Z':
		g_canvas->UndoShape();
		break;
	case 'Y':
		g_canvas->RedoShape();
		break;

		return (0);

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
	static int endX, endY;

	switch (_msg) {

	case WM_PAINT:
	{

		hdc = BeginPaint(_hwnd, &ps);

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
			currentShape = new CLine(g_penStyle, g_width, g_fillColor);
			brushType = Button_GetState(brush_line);
			break;
		case BOX:
			currentShape = new CRectangle(g_brushStyle, g_hatchStyle, g_fillColor, g_penStyle, g_penColor);
			brushType = Button_GetState(brush_rect);
			break;
		case ELLIPSE:
			currentShape = new CEllipse(g_fillColor);
			brushType = Button_GetState(brush_ellipse);
			break;
		default:
			break;
		}

		// We can start drawing
		if (brushType != 0) {

			g_canvas->AddShape(currentShape);

			currentShape->SetStartX(static_cast<int>(LOWORD(_lparam)));
			currentShape->SetStartY(static_cast<int>(HIWORD(_lparam)));

			bIsDrawing = true;

		}

		return (0);

	}
	break;

	case WM_MOUSEMOVE:
	{

		if (bIsDrawing == true) {

			currentShape->SetEndX(static_cast<int>(LOWORD(_lparam)));
			currentShape->SetEndY(static_cast<int>(HIWORD(_lparam)));

			InvalidateRect(_hwnd, NULL, true);

		}

		return (0);

	}
	break;

	case WM_LBUTTONUP:
	{

		if (bIsDrawing == true) {

			currentShape->SetEndX(static_cast<int>(LOWORD(_lparam)));
			currentShape->SetEndY(static_cast<int>(HIWORD(_lparam)));

			InvalidateRect(_hwnd, NULL, true);

			bIsDrawing = false;

		}

		return (0);

	}
	break;

	}

	// Process any messages that we did not take care of...
	return (DefWindowProc(_hwnd, _msg, _wparam, _lparam));

}

//////////////////////////////////////////////////////////
//					PROPERTY PROCESS					//
//////////////////////////////////////////////////////////

LRESULT CALLBACK PropertyProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{

	static HWND hwndCombo, hwndStatic;
	const wchar_t *items[] = {
		L"Line",		// 0 
		L"Rectangle",	// 1
		L"Ellipse",		// 2
		L"Polygon"		// 3
	};

	switch (_msg) {

	case WM_CREATE:
	{

		hwndCombo = CreateWindowW(L"Combobox", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			10, 10, 120, 110, _hwnd, NULL, g_hInstance, NULL);

		hwndStatic = CreateWindowW(L"Static", L"",
			WS_CHILD | WS_VISIBLE,
			10, 50, 120, 25, _hwnd, NULL, g_hInstance, NULL);

		for (int i = 0; i < 4; i++) {

			SendMessageW(hwndCombo, CB_ADDSTRING, 0, (LPARAM)items[i]);

		}


		return (0);

	}
	break;

	case WM_COMMAND:
	{

		if (HIWORD(_wparam) == BN_CLICKED) {

			SendMessage(hwndCombo, CB_SHOWDROPDOWN, (WPARAM)TRUE, 0);
		}

		if (HIWORD(_wparam) == CBN_SELCHANGE) {

			LRESULT sel = SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);
			//SetWindowTextW(hwndStatic, items[sel]);
			switch (sel) {
			// Line
			case 0:
			{

			}
			break;
			// Rectangle
			case 1:
			{

			}
			break;
			// Ellipse
			case 2:
			{

			}
			break;
			// Polygon
			case 3:
			{

			}
			break;

			}

		}

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

COLORREF ShowColorDialog(HWND hwnd) {

	CHOOSECOLOR cc;
	static COLORREF crCustClr[16];

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)crCustClr;
	cc.rgbResult = RGB(0, 255, 0);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);

	return cc.rgbResult;

}

void OpenDialog(HWND hwnd) {

	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = hwnd;
	//ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFilter = L"Bitmap Files (*.bmp)\0*.bmp\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"bmp";

	// Do something usefull with the filename stored in szFileName 
	if (GetOpenFileName(&ofn)) {



	}

}