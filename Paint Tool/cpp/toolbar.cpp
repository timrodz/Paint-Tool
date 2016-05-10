// TOOLBAR //
	g_hImageList = NULL;
	const int ImageListID = 0;
	const int NUMBUTTONS = 8;
	const int bitmapSize = 32;
	const DWORD buttonStyles = BTNS_AUTOSIZE;
	
	HWND hWndToolbar = CreateWindowEx(
		NULL,
		TOOLBARCLASSNAME,
		NULL,
		WS_CHILD | TBSTYLE_WRAPABLE, 
		0, 0, 0, 0,
		hwnd, _hMenu, _hInstance, NULL
	);

	if (hWndToolbar == NULL)
		return NULL;

	// Create the image list.
	
	g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
		ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
		NUMBUTTONS, 0);

	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
		(WPARAM)ImageListID,
		(LPARAM)g_hImageList);

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
		(WPARAM)IDB_STD_LARGE_COLOR,
		(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	/*TBBUTTON tbButtons[NUMBUTTONS] =
	{
		{ MAKELONG(STD_FILENEW,  ImageListID), IDM_FILE_NEW,  TBSTATE_ENABLED, buttonStyles,{ 0 }, 0, (INT_PTR)L"New" },
		{ MAKELONG(STD_FILEOPEN, ImageListID), IDM_FILE_OPEN, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0, (INT_PTR)L"Open" },
		{ MAKELONG(STD_FILESAVE, ImageListID), IDM_FILE_SAVE, 0,               buttonStyles,{ 0 }, 0, (INT_PTR)L"Save" }
	};*/

	TBBUTTON tbButtons[NUMBUTTONS];

	tbButtons[0].iBitmap = 0;
	tbButtons[0].idCommand = IDM_FILE_NEW;
	tbButtons[0].fsState = TBSTATE_ENABLED;
	tbButtons[0].fsStyle = TBSTYLE_BUTTON;
	tbButtons[0].dwData = 0L;
	tbButtons[0].iString = 0;

	tbButtons[1].iBitmap = 1;
	tbButtons[1].idCommand = IDM_FILE_OPEN;
	tbButtons[1].fsState = TBSTATE_ENABLED;
	tbButtons[1].fsStyle = TBSTYLE_BUTTON;
	tbButtons[1].dwData = 0L;
	tbButtons[1].iString = 0;

	tbButtons[2].iBitmap = 2;
	tbButtons[2].idCommand = IDM_FILE_SAVE;
	tbButtons[2].fsState = TBSTATE_ENABLED;
	tbButtons[2].fsStyle = TBSTYLE_SEP;
	tbButtons[2].dwData = 0L;
	tbButtons[2].iString = 0;

	tbButtons[3].iBitmap = 0;
	tbButtons[3].idCommand = 0;
	tbButtons[3].fsState = TBSTATE_ENABLED;
	tbButtons[3].fsStyle = TBSTYLE_BUTTON;
	tbButtons[3].dwData = 0L;
	tbButtons[3].iString = 0;

	tbButtons[4].iBitmap = 3;
	tbButtons[4].idCommand = IDI_ICON_BRUSH;
	tbButtons[4].fsState = TBSTATE_ENABLED;
	tbButtons[4].fsStyle = TBSTYLE_BUTTON;
	tbButtons[4].dwData = 0L;
	tbButtons[4].iString = 0;

	tbButtons[5].iBitmap = 4;
	tbButtons[5].idCommand = IDI_ICON_FILL;
	tbButtons[5].fsState = TBSTATE_ENABLED;
	tbButtons[5].fsStyle = TBSTYLE_BUTTON;
	tbButtons[5].dwData = 0L;
	tbButtons[5].iString = 0;

	tbButtons[6].iBitmap = 5;
	tbButtons[6].idCommand = IDI_ICON_CIRCLE;
	tbButtons[6].fsState = TBSTATE_ENABLED;
	tbButtons[6].fsStyle = TBSTYLE_BUTTON;
	tbButtons[6].dwData = 0L;
	tbButtons[6].iString = 0;

	tbButtons[7].iBitmap = 6;
	tbButtons[7].idCommand = IDI_ICON_POLYGON;
	tbButtons[7].fsState = TBSTATE_ENABLED;
	tbButtons[7].fsStyle = TBSTYLE_BUTTON;
	tbButtons[7].dwData = 0L;
	tbButtons[7].iString = 0;

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)NUMBUTTONS, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);