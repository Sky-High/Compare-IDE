//------------------------------------------------------------------------------
//
// SimApp01Dlg.cpp :	Defines the entry point for the application.
//
// Description:
//	Test events and request the lat/lon/alt of the user aircraft
//
// See also:
//	mSystemEvent / Sample: Data Request.cs
//	msdn.microsoft.com/en-us/library/cc730340.aspx
//
//------------------------------------------------------------------------------

#include <stdarg.h>
#include "stdafx.h"
#include <malloc.h>
#include <memory.h>
#include "XPstyleCtrl.h"
//
// Global Variables
// 'static' means: file-global
#define LEN_DISPLAY_BUF 4096

HWND	    	hDialog;    	    	    	// handle to the Dialog Window
HWND	    	hEdit;	    	    	    	// handle to the Edit box
static TCHAR	sDisplayBuf[LEN_DISPLAY_BUF];	// text buffer
static int  	nLine = -1;  	    	    	// line number
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
static void onClose(HWND hDlg)
{
	fsxDisconnect(hDlg);		// always disconnect
	//if(MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"), MB_ICONQUESTION | MB_YESNO) == IDYES) {}
	DestroyWindow(hDlg);
}
//
//	---------- the action message callback ----------
// Windows sends its (event) messages to this procedure
//
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:	return doCommand(hDlg, LOWORD(wParam));
	case WM_CLOSE:		onClose(hDlg);		return TRUE;
	case WM_DESTROY:	PostQuitMessage(0);	return TRUE;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hDlg, &ps);
			doPaint(hDC);
			EndPaint(hDlg, &ps);
		}
		return true;
	case WM_USER_SIMCONNECT:	fsxDispatch(hDlg);	return TRUE;
	// default: is not caught here so will be handled by DefWndProc
	}

	// indicate that message should (additionally) be processed by DefWndProc
	return FALSE;
}
//
// ---------- Main entrypoint ----------
// initializes the application and enters the message loop to wait for
// messages from Windows and dispatch those to DialogProc
//
int APIENTRY _tWinMain(	HINSTANCE	hInstance,
			HINSTANCE	hPrevInstance,
			LPTSTR		lpCmdLine,
			int 		nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG 	    	    	msg;
	BOOL	    	    	ret;
	HICON	    	    	hIcon;
	INITCOMMONCONTROLSEX	icex;

	//	setup for new XP style controls
	icex.dwSize	= sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC	= ICC_STANDARD_CLASSES | ICC_COOL_CLASSES;
	InitCommonControlsEx(&icex);

	//	start the dialog as main window
	hDialog	= CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG), 0, DialogProc, 0);
	ShowWindow(hDialog, nCmdShow);

	// handle to the editbox
	hEdit	= GetDlgItem(hDialog, IDEDIT);

	// Attach icon to main dialog
	hIcon	= LoadIcon (hInstance, MAKEINTRESOURCE (IDI_MWINAPP));
	SendMessage (hDialog, WM_SETICON, WPARAM (ICON_BIG), LPARAM (hIcon));
	hIcon	= LoadIcon (hInstance, MAKEINTRESOURCE (IDI_SMALL));
	SendMessage (hDialog, WM_SETICON, WPARAM (ICON_SMALL), LPARAM (hIcon));

	// Other initializations...
	initialize();

	// process actions in message loop
	while((ret = GetMessage(&msg, 0, 0, 0)) != 0 && ret != -1) {
		if(!IsDialogMessage(hDialog, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}
