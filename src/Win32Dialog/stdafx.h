// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <tchar.h>

// global function and variable declarations
//
#include "Resource.h"
#include <SimConnect.h>

// own parameters higher than WM_USER=0x0400, so ...
//#define WM_USER_SIMCONNECT (WM_USER+2)
#define WM_USER_SIMCONNECT 0x0402

void	doPaint(HDC hDC);
void	displayTextF(TCHAR *sFmt, ...);
BOOL	doCommand(HWND hDlg, UINT wParam);

void	fsxConnect(HWND hWnd);
void	fsxDisconnect(HWND hWnd);
void	fsxDispatch(HWND hWnd);
void	requestData(HWND hWnd);
void	doEvent1(HWND hWnd);
void	doEvent2(HWND hWnd);
