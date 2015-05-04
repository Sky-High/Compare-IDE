// FsxConnect.h

// User-defined win32 event = WM_USER+2
//const static int	WM_USER_SIMCONNECT = 0x0402;
#define	WM_USER_SIMCONNECT	0x0402

namespace nsFsx {
	void	displayText0(TCHAR* s);
	void	displayTextF(TCHAR *sFmt, ...);
}
//
/////////////////////////////////////////////////////////////////////////////
// SimConnect API wrapper
//
class fsxConnect {
public:
	fsxConnect();
	~fsxConnect();

	HRESULT fsxOpen(System::IntPtr Handle);
	HRESULT fsxClose();
	void	fsxDispatch();
	void	AddToDataDefinition(SIMCONNECT_DATA_DEFINITION_ID DefineID, const char * DatumName,
				const char * UnitsName, SIMCONNECT_DATATYPE DatumType = SIMCONNECT_DATATYPE_FLOAT64);
	void	RequestDataOnSimObject(SIMCONNECT_DATA_DEFINITION_ID DefineID, SIMCONNECT_OBJECT_ID ObjectID);
	void	SubscribeToSystemEvent(SIMCONNECT_CLIENT_EVENT_ID EventID, const char * SystemEventName);
	void	MapClientEventToSimEvent(SIMCONNECT_CLIENT_EVENT_ID EventID, const char * EventName = "");
	void	TransmitClientEvent(SIMCONNECT_OBJECT_ID ObjectID, SIMCONNECT_CLIENT_EVENT_ID EventID, DWORD dwData = 0,
				SIMCONNECT_NOTIFICATION_GROUP_ID GroupID = SIMCONNECT_GROUP_PRIORITY_HIGHEST,
				SIMCONNECT_EVENT_FLAG Flags = SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	void	AddClientEventToNotificationGroup(SIMCONNECT_NOTIFICATION_GROUP_ID GroupID,
				SIMCONNECT_CLIENT_EVENT_ID EventID, BOOL bMaskable = FALSE);
	void	SetNotificationGroupPriority(SIMCONNECT_NOTIFICATION_GROUP_ID GroupID, DWORD uPriority);
//	void	SetSystemEventState(SIMCONNECT_CLIENT_EVENT_ID EventID, SIMCONNECT_STATE dwState);
//	void	RemoveClientEvent(SIMCONNECT_NOTIFICATION_GROUP_ID GroupID, SIMCONNECT_CLIENT_EVENT_ID EventID);

	// internal functions
private:
	static void CALLBACK	fsxDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
	static void 	    	parseOpenMsg(unsigned int n, SIMCONNECT_RECV *pData);
	static void 	    	parseEvents (unsigned int n, SIMCONNECT_RECV *pData);

	// SimConnect object
private:
	HANDLE  	    	    	hSimConnect;
	SIMCONNECT_DATA_REQUEST_ID	RequestID;
	unsigned int    	    	nDispatch;		// SimConnect dispatch sequence number
};

/////////////////////////////////////////////////////////////////////////////
//
// SimConnect definitions
//
const static int	REQ_OBJECT_ID0	= 0;
const static int	REQ_FLAGS   	= 0;
const static int	REQ_ORIGIN   	= 0;
const static int	REQ_INTERVAL   	= 0;
const static int	REQ_LIMIT   	= 0;

struct Struct1 {
	char	title[256];
	double	altitude;
	double	longitude;
	double	latitude;
};
static enum EVENT_ID {
	EVENT_SIM_START,
	EVENT_SIM_STOP,
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
