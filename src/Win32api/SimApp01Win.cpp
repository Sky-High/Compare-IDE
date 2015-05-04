// SimApp01Win.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#define TEXT_LEN		256
#define LEN_DISPLAY_BUF 4096
#define WINDOW_WIDTH	540
#define WINDOW_HEIGHT	330
#define BUTTON_WIDTH	120
#define BUTTON_HEIGHT	25
#define BUTTON_X		10
#define BUTTON_Y		30
#define EDIT_WIDTH		350
#define EDIT_HEIGHT		250
#define EDIT_X			150
#define EDIT_Y			10
//
// Global Variables
// 'static' means: file-global

static HINSTANCE hInst;	    	    	    	// current instance
static HWND 	hWndMain;    	    	    	// handle to the main Window
static HWND 	hEdit;	    	    	    	// handle to the Edit box
static int  	nLine = -1;  	    	    	// line number
static TCHAR	sDisplayBuf[LEN_DISPLAY_BUF];	// text buffer
static TCHAR	szTitle[TEXT_LEN];  	    	// The title bar text
static TCHAR	szWindowClass[TEXT_LEN];    	// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG		msg;
	HACCEL	hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, TEXT_LEN);
	LoadString(hInstance, IDC_SIMAPP01WIN, szWindowClass, TEXT_LEN);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if( !InitInstance (hInstance, nCmdShow) )	return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMAPP01WIN));

	// Main message loop:
	while( GetMessage(&msg, NULL, 0, 0) )
	{
		if( !TranslateAccelerator(msg.hwnd, hAccelTable, &msg) )
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
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX  	    	wcex;
	wcex.cbSize   		= sizeof(WNDCLASSEX);
	wcex.style    		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon    		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMAPP01WIN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SIMAPP01WIN);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
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
	hInst	= hInstance; // Store instance handle in our global variable
	hWndMain = CreateWindow(szWindowClass, szTitle,
				WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
				WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	if (!hWndMain)	return FALSE;

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);
	return TRUE;
}

//
// Message handler for about box.
//
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
//
//	---------- general functions ----------
//
// display a line of text in a list
//
static void displayTextBuffer(TCHAR *txt)
{
	// display a line of text in a list
	TCHAR*	ptr;
	TCHAR	sNum[20];

	// find ptr to second line
	ptr	= _tcschr(sDisplayBuf,'\n') + 1;
	if( ptr == NULL ) ptr = TEXT("-- no line? --");

	// remove first line from tBuffer
	// (slightly illegal as copying in the same buffer ...)
	_tcsncpy_s(sDisplayBuf, _countof(sDisplayBuf), ptr, _TRUNCATE);

	// append new line with linenumber
	// line separator in editbox is cr-lf, so \r\n
	_ultot_s(++nLine, sNum, 20, 10);
	_tcsncat_s(sDisplayBuf, _countof(sDisplayBuf), TEXT("\r\n"), _TRUNCATE);
	_tcsncat_s(sDisplayBuf, _countof(sDisplayBuf), sNum, _TRUNCATE);
	_tcsncat_s(sDisplayBuf, _countof(sDisplayBuf), TEXT(": "), _TRUNCATE);

	// append text and display new set of lines in Edit Box
	_tcsncat_s(sDisplayBuf, _countof(sDisplayBuf), txt, _TRUNCATE);
	SetWindowText(hEdit, sDisplayBuf);
}

static void initialize()
{
	// prepare sBuffer with dummy lines
	_tcsncpy_s(sDisplayBuf, sizeof(sDisplayBuf),
		TEXT("00\r\n01\r\n02\r\n03\r\n04\r\n05\r\n06\r\n07\r\n08\r\n09\r\n10\r\n11\r\n12"),
		_TRUNCATE);
	displayTextBuffer(TEXT("-- running --"));
}

void displayTextF(TCHAR *sFmt, ...)
{
	TCHAR	sBuffer[512];		// text buffer for formatting
	va_list	pArg;
	va_start(pArg, sFmt);
	_vsntprintf_s(sBuffer, _countof(sBuffer), _TRUNCATE, sFmt, pArg);
	va_end(pArg);
	displayTextBuffer(sBuffer);
}

//
// close application
//
static void onClose(HWND hWnd)
{
	fsxDisconnect(hWnd);		// always disconnect
	//if(MessageBox(hWnd, TEXT("Close the program?"), TEXT("Close"), MB_ICONQUESTION | MB_YESNO) == IDYES) {}
	DestroyWindow(hWnd);
}

//
//	Create a button
//
HWND CreateButton(HWND hWnd, int iy, int id, LPCWSTR szText) {
	HWND	hButton;
	hButton = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), szText, WS_CHILD | WS_VISIBLE ,
		BUTTON_X, iy, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);
	if(hButton == NULL)	MessageBox(hWnd, _T("Could not create button."), _T("Error"), MB_OK | MB_ICONERROR);
	return hButton;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process widget, menu, or other event
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:		// execute command for widget, menu, or else
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return doCommand(hWnd, wmId);
		}
		break;
    case WM_CREATE:
		{
			int 	iy;

			// create buttons
			CreateButton(hWnd, iy = EDIT_Y   , ID_CONNECT,  	L"Connect");
			CreateButton(hWnd, iy += BUTTON_Y, ID_DO_EVENT1,	L"do Event1");
			CreateButton(hWnd, iy += BUTTON_Y, ID_DO_EVENT2,	L"do Event2");
			CreateButton(hWnd, iy += BUTTON_Y, ID_REQUEST_DATA,	L"Request Data");
			CreateButton(hWnd, iy += BUTTON_Y, ID_DISCONNECT,	L"Disconnect");

			// create editbox
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("This is an edit text box"),
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL ,
				EDIT_X, EDIT_Y, EDIT_WIDTH, EDIT_HEIGHT, hWnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
			if(hEdit == NULL)
				MessageBox(hWnd, _T("Could not create edit box."), _T("Error"), MB_OK | MB_ICONERROR);
			
			initialize();
		}
		break;
	case WM_PAINT:
		{
			// C++ rule for declare and initialize in one statement.
			// The issue is the scope. Put the whole block in braces.  
			PAINTSTRUCT	ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			doPaint(hDC);
			EndPaint(hWnd, &ps);
		}
		return true;
	case WM_CLOSE:		onClose(hWnd);		return TRUE;
	case WM_DESTROY:	PostQuitMessage(0);	return TRUE;
	case WM_USER_SIMCONNECT:	fsxDispatch(hWnd);	return TRUE;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
