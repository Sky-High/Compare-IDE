//------------------------------------------------------------------------------
//
//  SimConnect Data Request Sample
//
//  Description:
//	Test events and request the lat/lon/alt of the user aircraft
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>

// File-global variables and structures
// unprefixed definitions are global
// static definitions are static and file-private
// exern declaration refer to a (global) definition in another (.cpp) file

static HANDLE   	hSimConnect = NULL;
static long	    	lineNr = 0;
static int	    	RequestID	= 0;	// request sequence number
static unsigned int	nDispatch	= 0;	// SimConnect dispatch sequence number

#define REQ_OBJECT_ID0		0
#define REQ_FLAGS   		0
#define REQ_ORIGIN  		0
#define REQ_INTERVAL		0
#define REQ_LIMIT   		0

struct Struct1
{
	char	title[256];
	double	altitude;
	double	longitude;
	double	latitude;
};
static enum EVENT_ID{
	EVENT_SIM_START,
	EVENT_PAUSE,
	EVENT_PAUSE_TOGGLE,
	EVENT_BRAKES,
	EVENT_4S_TIMER,
};
static enum DATA_DEFINE_ID {
	DEFINITION_1
};
static enum GROUP_ID {
	GROUP_0,
	GROUP_1,
};
// --------------------------------------------------------
void parseOpenMsg( unsigned int n, SIMCONNECT_RECV *pData ) {
	// parse the message received from fsx after opening the connection
	SIMCONNECT_RECV_OPEN* pS	= (SIMCONNECT_RECV_OPEN*)pData;

	displayTextF( L"rcv(%d) open v%ld:%ld:%ld:%ld app=<%S>",		// note %S iso %s !
			n, pS->dwSimConnectVersionMajor, pS->dwSimConnectVersionMinor,
			pS->dwSimConnectBuildMajor, pS->dwSimConnectBuildMinor,
			pS->szApplicationName );
}

void parseEvents( unsigned int n, SIMCONNECT_RECV *pData ) {
	// parse the event message received from fsx
	UINT	    	    	uEventID = ((SIMCONNECT_RECV_EVENT*)pData)->uEventID;

	switch(uEventID)
	{
		case EVENT_SIM_START:
			displayTextF(        L"rcv(%d) fsx start event %d", n, uEventID );
			break;
		case EVENT_PAUSE:
			displayTextF(        L"rcv(%d) fsx pause event %d", n, uEventID );
			break;
		case EVENT_PAUSE_TOGGLE:
			displayTextF( L"rcv(%d) fsx pause_toggle event %d", n, uEventID );
			break;
		case EVENT_BRAKES:
			displayTextF(       L"rcv(%d) fsx brakes event %d", n, uEventID );
			break;
		default:
			displayTextF(        L"rcv(%d) fsx other event %d", n, uEventID );
			break;
	}
}

// --------------------------------------------------------
// dispatch callback
// SimConnect notifies the hWndMain message loop that fsx data is available.
// The hWndMain message loop calls fsxDispatch
// which calls this fsxDispatchProc to process the fsx data.
//
void CALLBACK fsxDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	unsigned int dispatchNr = (long)pContext;	//sequence number passed from fsxDispatch as faked pointer

	switch(pData->dwID)		// packet command type
	{
		case SIMCONNECT_RECV_ID_EVENT:
			parseEvents(dispatchNr, pData);
			break;

		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
			{
			SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
			Struct1	*pS	    	= (Struct1*)&pObjData->dwData;

			displayTextF( L"rcv(%d) req=%d obj=%d def=%d t=<%S> Lat=%.4f Lon=%.4f Alt=%.1f",	// note %S iso %s !
				dispatchNr, pObjData->dwRequestID, pObjData->dwObjectID, pObjData->dwDefineID,
				pS->title, pS->latitude, pS->longitude, pS->altitude );
			}
			break;

		case SIMCONNECT_RECV_ID_OPEN:
			parseOpenMsg(dispatchNr, pData);
			break;

		case SIMCONNECT_RECV_ID_QUIT:
			displayTextF( L"rcv(%d) quit", dispatchNr );
			break;

		default:
			displayTextF( L"rcv(%d) ID=%d", dispatchNr, pData->dwID );
			break;
	}
}
// --------------------------------------------------------
void fsxDispatch(HWND hWnd)
{
	// fsx has sent a message to hWndMain as notification that a response message is available.
	// Call the Dispatch method to receive this message and handle it.

	// SimConnect dispatch sequence number
	// convert this to a faked pointer so that we can abuse the pContext parameter of
	// SimConnect_CallDispatch to pass this sequence number to fsxDispatchProc
	void *	pContext	= (void *)(++nDispatch);	
	displayTextF(L"** hWnd notification: dispatch(%ld)", (long)pContext );

	SimConnect_CallDispatch(hSimConnect, fsxDispatchProc, pContext );
	/* ***************** * /
	Sleep(5);	// wait 5 msec and check if another msg waiting
	pContext	= (void *)(++nDispatch);	
	nsFsx::displayTextF(L"** msg redodsp(%ld)", (long)pContext );
	SimConnect_CallDispatch(hSimConnect, fsxDispatchProc, pContext );
	/* ***************** */
	displayTextF(L"** hWnd msgs dispatched(%ld)", (long)pContext );
}
// --------------------------------------------------------
void fsxConnect(HWND hWnd)
{
	// open SimConnect connection
	// open while specifying hWnd's message handler to receive notifications
	// of SimConnect when a message has been received. The message type in
	// the hWnd message handler will be WM_USER_SIMCONNECT .
	// With this Sim_Open function we can just wait for a message to arrive
	// in the hWnd handler, avoiding the need for continuous polling.
	HRESULT hr;
	nDispatch	= 0;		// SimConnect dispatch sequence number

	if ( !SUCCEEDED(SimConnect_Open(&hSimConnect, "my Test App", hWnd, WM_USER_SIMCONNECT, NULL, 0)))
	{
		displayTextF( L"SimConnect_Open error?" );
		return;
	}

	// Set up the data definition, but do not yet do anything with it
	hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
	hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Altitude", "feet");
	hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Longitude", "degrees");
	hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Latitude", "degrees");

	// Request an event when the simulation starts
	hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
	hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_PAUSE, "Pause");

	hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_BRAKES, "brakes");
	hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_1, EVENT_BRAKES);
	hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_PAUSE_TOGGLE, "pause_toggle");
	hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_1, EVENT_PAUSE_TOGGLE);
	hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_1, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

	displayTextF( L"open done" );
}
// --------------------------------------------------------
void fsxDisconnect(HWND hWnd) {
	displayTextF(L"disconnect");

	// Dispose serves the same purpose as SimConnect_Close()
	SimConnect_Close(hSimConnect);
	hSimConnect 	= NULL; 		// SimConnect handle
}
// --------------------------------------------------------
void requestData(HWND hWnd) {
	// send a request for aircraft data to fsx.

	displayTextF(L"send requestData");

	SimConnect_RequestDataOnSimObject(hSimConnect, (++RequestID), DEFINITION_1, REQ_OBJECT_ID0,
		SIMCONNECT_PERIOD_ONCE, REQ_FLAGS, REQ_ORIGIN, REQ_INTERVAL, REQ_LIMIT);
}

void doEvent1(HWND hWnd)
{
	displayTextF(L"transmit brakes");
	SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_BRAKES, 0,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
}

void doEvent2(HWND hWnd)
{
	displayTextF(L"transmit pause toggle");
	SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_PAUSE_TOGGLE, 0,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
}
