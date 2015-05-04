// application specific functionality
//
#include "stdafx.h"
//	---------- actions ----------

void doPaint(HDC hDC)
{
	HPEN	hPen, hPenOld;
	HBRUSH	hBrush, hBrushOld;

	hPen	= CreatePen(PS_SOLID, 10, RGB(255,0,0) );
	hBrush	= CreateSolidBrush(RGB(0,255,0));
	//Select the pen and brush  while saving the old values
	hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);

	//Draw and fill an ellipse
	Ellipse(hDC, 10, 125, 110, 225);

	//Draw a triangle
	hPenOld	= (HPEN)SelectObject(hDC, hPen);
	MoveToEx(hDC, 40, 200, NULL);
	LineTo(hDC, 20, 240);
	LineTo(hDC, 60, 240);
	LineTo(hDC, 40, 200);

	//Release the resources
	SelectObject(hDC,hPenOld);
	SelectObject(hDC, hBrushOld);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

BOOL doCommand(HWND hDlg, UINT wParam)
{
	switch(wParam)
	{
	case IDOK:  	    	fsxConnect(hDlg);   	return TRUE;
	case IDCANCEL:	    	fsxDisconnect(hDlg);	return TRUE;
	case ID_DO_EVENT1:  	doEvent1(hDlg); 		return TRUE;
	case ID_DO_EVENT2:  	doEvent2(hDlg);	    	return TRUE;
	case ID_REQUEST_DATA:	requestData(hDlg);  	return TRUE;
	}
	return FALSE;
}
