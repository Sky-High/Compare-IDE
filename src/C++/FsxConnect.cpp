//------------------------------------------------------------------------------
//
// FsxConnect class methods interface with the (unmanaged) SimConnect library
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "FsxConnect.h"

// (file) global variables
static TCHAR    	sText[512];

// --------------------------------------------------------
// class fsxConnect
//
fsxConnect::fsxConnect() {
	hSimConnect	= NULL;
	nDispatch	= 0;		// SimConnect dispatch sequence number
}

fsxConnect::~fsxConnect() {
}

HRESULT fsxConnect::fsxOpen(System::IntPtr Handle) {
	RequestID	= 0;	// request sequence number
	HWND hWnd	= static_cast<HWND>(Handle.ToPointer());

	HRESULT hr	= SimConnect_Open(&hSimConnect, "mijn Test App", hWnd, WM_USER_SIMCONNECT, NULL, 0);

	nsFsx::displayTextF(L"connect hr=%x hSC=%lx", (int)hr, (long)hSimConnect );
	return hr;
}

HRESULT fsxConnect::fsxClose() {
	nsFsx::displayTextF(L"close");

	// Dispose serves the same purpose as SimConnect_Close()
	SimConnect_Close(hSimConnect);
	hSimConnect 	= NULL; 		// SimConnect handle
	return 0;
}

void fsxConnect::AddToDataDefinition(SIMCONNECT_DATA_DEFINITION_ID DefineID, const char * DatumName,
				const char * UnitsName, SIMCONNECT_DATATYPE DatumType) {
	SimConnect_AddToDataDefinition(hSimConnect, DefineID, DatumName, UnitsName, DatumType);
}

void fsxConnect::RequestDataOnSimObject(SIMCONNECT_DATA_DEFINITION_ID DefineID, SIMCONNECT_OBJECT_ID ObjectID) {
	nsFsx::displayTextF( L"transMit(%d) defID=%d objID=%d",(RequestID+1), DefineID, ObjectID );
	SimConnect_RequestDataOnSimObject(hSimConnect, (++RequestID), DefineID, ObjectID, SIMCONNECT_PERIOD_ONCE);
}

void fsxConnect::SubscribeToSystemEvent(SIMCONNECT_CLIENT_EVENT_ID EventID, const char * SystemEventName) {
	SimConnect_SubscribeToSystemEvent(hSimConnect, EventID, SystemEventName);
}

void fsxConnect::MapClientEventToSimEvent(SIMCONNECT_CLIENT_EVENT_ID EventID, const char * EventName) {
	SimConnect_MapClientEventToSimEvent(hSimConnect, EventID, EventName);
}

void fsxConnect::TransmitClientEvent(SIMCONNECT_OBJECT_ID ObjectID, SIMCONNECT_CLIENT_EVENT_ID EventID, DWORD dwData, SIMCONNECT_NOTIFICATION_GROUP_ID GroupID, SIMCONNECT_EVENT_FLAG Flags) {
	SimConnect_TransmitClientEvent(hSimConnect, ObjectID, EventID, dwData, GroupID, Flags);
}

void fsxConnect::AddClientEventToNotificationGroup(SIMCONNECT_NOTIFICATION_GROUP_ID GroupID, SIMCONNECT_CLIENT_EVENT_ID EventID, BOOL bMaskable) {
	SimConnect_AddClientEventToNotificationGroup(hSimConnect, GroupID, EventID, bMaskable);
}

void fsxConnect::SetNotificationGroupPriority(SIMCONNECT_NOTIFICATION_GROUP_ID GroupID, DWORD uPriority) {
	SimConnect_SetNotificationGroupPriority(hSimConnect, GroupID, uPriority);
}

//void fsxConnect::SetSystemEventState(SIMCONNECT_CLIENT_EVENT_ID EventID, SIMCONNECT_STATE dwState);
//void fsxConnect::RemoveClientEvent(SIMCONNECT_NOTIFICATION_GROUP_ID GroupID, SIMCONNECT_CLIENT_EVENT_ID EventID);

void fsxConnect::fsxDispatch() {
	// fsx has sent a message to hWndMain as notification that a response message is available.
	// Call the Dispatch method to receive this message and handle it.

	// SimConnect dispatch sequence number
	// convert this to a faked pointer so that we can abuse the pContext parameter of
	// SimConnect_CallDispatch to pass this sequence number to fsxDispatchProc
	void *	pContext	= (void *)(++nDispatch);	
	nsFsx::displayTextF(L"** hWnd trigger for dispatch(%ld)", (long)pContext );

	SimConnect_CallDispatch(hSimConnect, &fsxDispatchProc, pContext );
	/* ***************** * /
	Sleep(5);	// wait 5 msec and check if another msg waiting
	pContext	= (void *)(++nDispatch);	
	nsFsx::displayTextF(L"** msg redodsp(%ld)", (long)pContext );
	SimConnect_CallDispatch(hSimConnect, &fsxDispatchProc, pContext );
	/* ***************** */
	nsFsx::displayTextF(L"** hWnd completed dispatch(%ld)", (long)pContext );

}

void fsxConnect::parseOpenMsg( unsigned int n, SIMCONNECT_RECV *pData ) {
	// parse the message received from fsx after opening the connection
	SIMCONNECT_RECV_OPEN* pS	= (SIMCONNECT_RECV_OPEN*)pData;

	nsFsx::displayTextF( L"rcv(%d) open v%ld:%ld:%ld:%ld app=<%S>",		// note %S iso %s !
			n, pS->dwSimConnectVersionMajor, pS->dwSimConnectVersionMinor,
			pS->dwSimConnectBuildMajor, pS->dwSimConnectBuildMinor,
			pS->szApplicationName );
}

void fsxConnect::parseEvents( unsigned int n, SIMCONNECT_RECV *pData ) {
	// parse the event message received from fsx
	UINT	    	    	uEventID = ((SIMCONNECT_RECV_EVENT*)pData)->uEventID;

	switch(uEventID)
	{
		case EVENT_SIM_START:
			nsFsx::displayTextF(        L"rcv(%d) fsx start event %d", n, uEventID );
			break;
		case EVENT_SIM_STOP:
			nsFsx::displayTextF(         L"rcv(%d) fsx stop event %d", n, uEventID );
			break;
		case EVENT_PAUSE:
			nsFsx::displayTextF(        L"rcv(%d) fsx pause event %d", n, uEventID );
			break;
		case EVENT_PAUSE_TOGGLE:
			nsFsx::displayTextF( L"rcv(%d) fsx pause_toggle event %d", n, uEventID );
			break;
		case EVENT_BRAKES:
			nsFsx::displayTextF(       L"rcv(%d) fsx brakes event %d", n, uEventID );
			break;
		default:
			nsFsx::displayTextF(        L"rcv(%d) fsx other event %d", n, uEventID );
		break;
	}
}

// --------------------------------------------------------
// dispatch callback
// SimConnect notifies the hWndMain message loop that fsx data is available.
// The hWndMain message loop calls AppForm::DefWndProc which calls fsxDispatch
// which calls this fsxDispatchProc to process the fsx data.
//
void CALLBACK fsxConnect::fsxDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
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

			nsFsx::displayTextF( L"rcv(%d) req=%d obj=%d def=%d t=<%S> Lat=%.4f Lon=%.4f Alt=%.1f",	// note %S iso %s !
				dispatchNr, pObjData->dwRequestID, pObjData->dwObjectID, pObjData->dwDefineID,
				pS->title, pS->latitude, pS->longitude, pS->altitude );
			}
			break;

		case SIMCONNECT_RECV_ID_OPEN:
			parseOpenMsg(dispatchNr, pData);
			break;

		case SIMCONNECT_RECV_ID_QUIT:
			nsFsx::displayTextF( L"rcv(%d) quit", dispatchNr );
			break;

		default:
			nsFsx::displayTextF( L"rcv(%d) ID=%d", dispatchNr, pData->dwID );
			break;
	}
}
// --------------------------------------------------------

// Unsubscribe from all the system events
//SimConnect_UnsubscribeFromSystemEvent(hSimConnect, SIMSTOP);
// Request an event when the simulation starts
//hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
