#define WIN32_LEAN_AND_MEAN

#include <windows.h> // Include all the windows headers.
#include <windowsx.h> // Include useful macros.
#include <commctrl.h>

// Paint Tool library
#include "h\canvas.h"
#include "h\line.h"
#include "resource.h"

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;

// Code
#define WINDOW_CLASS_NAME L"WINCLASS1"

HWND g_hwnd;
HINSTANCE g_hInstance;
CCanvas* g_canvas;

void GameLoop() {
	//One frame of game logic occurs here...
}

LRESULT CALLBACK WindowProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{

	// This is the main message handler of the system.
	//HINSTANCE _hInstance;
	PAINTSTRUCT ps; // Used in WM_PAINT.
	HDC hdc; // Handle to a device context.

	// What is the message?
	switch (_msg) {

	case WM_CREATE:
	{
		g_hwnd = _hwnd;
		//g_canvas = new CCanvas();

		//// ICONS ////
		// ----------------------------- New
		HWND button_new = CreateWindow(
			L"BUTTON", L"",
			WS_CHILD | WS_VISIBLE | BS_ICON,
			0, 0, 44, 44, 
			_hwnd,
			(HMENU)IDM_FILE_NEW, GetModuleHandle(NULL), NULL
		);
		HICON icon_new = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_NEW));
		SendMessage(button_new, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon_new);

		// ----------------------------- Open
		HWND button_open = CreateWindow(
			L"BUTTON", L"",
			WS_CHILD | WS_VISIBLE | BS_ICON,
			44, 0, 44, 44,
			_hwnd,
			(HMENU)IDM_FILE_OPEN, GetModuleHandle(NULL), NULL
		);
		HICON icon_open = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_OPEN));
		SendMessage(button_open, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon_open);

		// ----------------------------- Save
		HWND button_save = CreateWindow(
			L"BUTTON", L"",
			WS_CHILD | WS_VISIBLE | BS_ICON,
			88, 0, 44, 44,
			_hwnd,
			(HMENU)IDM_FILE_SAVE, GetModuleHandle(NULL), NULL
		);
		HICON icon_save = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_SAVE));
		SendMessage(button_save, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon_save);

		// ----------------------------- Brush
		HWND button_brush = CreateWindow(
			L"BUTTON", L"",
			WS_CHILD | WS_VISIBLE | BS_ICON,
			176, 0, 44, 44,
			_hwnd,
			(HMENU)IDI_ICON_BRUSH, GetModuleHandle(NULL), NULL
		);
		HICON icon_brush = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_BRUSH));
		SendMessage(button_brush, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon_brush);

		// Return Success.
		return (0);
	}
	break;
	case WM_PAINT:
	{

		hdc = BeginPaint(_hwnd, &ps);

		/*hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);

		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);*/

		EndPaint(_hwnd, &ps);

		return (0);
	}
	break;

	//////////////////////////////////////////////////////////
	//						LISTENERS						//
	//////////////////////////////////////////////////////////

	case WM_COMMAND:
	{

		switch (LOWORD(_wparam)) {

		// File Menu
		case IDM_FILE_NEW:
			MessageBox(_hwnd, L"Create a new project?", L"New", MB_OKCANCEL);
			break;
		case IDM_FILE_OPEN:

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
		// Drawing buttons
		case IDI_ICON_BRUSH:
			
			//MessageBox(_hwnd, L"BRUSH SELECTED", L"BRUSH", MB_OK);
			//g_canvas->AddShape();

			switch (LOWORD(_wparam)) {

			case WM_RBUTTONDBLCLK:

				break;

			}

			break;
		default: break;

		}

		return (0);
	}
	break;

	// Moving the mouse
	case WM_MOUSEMOVE:
	{

		int mouseX = static_cast<int>(LOWORD(_lparam));
		int mouseY = static_cast<int>(HIWORD(_lparam));

		return (0);
	}
	break;

	// Key combo listener
	case WM_KEYDOWN:
	{
		// SAVE


		// UNDO


		// REDO


		return (0);
	}
	break;

	//////////////////////////////////////////////////////////
	//							OTHER						//
	//////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////
//				~~	MAIN FUNCTION	~~					//
//////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE _hInstance,
	HINSTANCE _hPrevInstance,
	LPSTR _lpCmdLine,
	int _nCmdShow)
{

	WNDCLASSEX winClass; // This will hold the class we create.
	HWND hwnd;			 // Generic window handle.
	MSG msg;			 // Generic message.

	//// First fill in the window class structure ////
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WindowProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = _hInstance;
	winClass.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	winClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	winClass.lpszClassName = WINDOW_CLASS_NAME;
	winClass.hIconSm = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ICON_APP));

	//// Register the window class ////
	if (!RegisterClassEx(&winClass)) {
		return (0);
	}

	HMENU _hMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDR_MENU));

	//// Centering the window ////

	// Get the screen's resolution
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	// Center it based on the screens resolution minus the window's dimensions
	int centerX = (screenX / 2) - (WIDTH / 2);
	int centerY = (screenY / 2) - (HEIGHT / 2);

	// Creating the window
	hwnd = CreateWindowEx(NULL,				// Extended style.
		WINDOW_CLASS_NAME,					// Class.
		L"Paint Tool by Juan Rodriguez",	// Title.
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// Window properties
		centerX, centerY,					// Initial x,y.
		WIDTH, HEIGHT,						// Initial width, height.
		NULL,								// Handle to parent.
		_hMenu,								// Handle to menu.
		_hInstance,							// Instance of this application.
		NULL);								// Extra creation parameters.

	if (!(hwnd)) {
		g_hInstance = _hInstance;
		return (0);
		
	}

	// TOOLBAR //



	//// Enter main event loop ////
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
		GameLoop(); //One frame of game logic occurs here...

	}

	// Return to Windows like this...
	return (static_cast<int>(msg.wParam));

}