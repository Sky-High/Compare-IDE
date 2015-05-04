//
// C# Managed SimConnect example
//
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
// Add these two statements to all SimConnect clients
using Microsoft.FlightSimulator.SimConnect;
using System.Runtime.InteropServices;

namespace SimApp01CsForm {
public partial class SimApp : Form {

	// User-defined win32 event
	const int WM_USER_SIMCONNECT	= 0x0402;

	// SimConnect object
	SimConnect simconnect	= null;

	bool    FS_state1 = false;

	/////////////////////////////////////////////////////////////////////////////
	//
	// SimConnect definitions
	//
	const  int	REQ_OBJECT_ID0	= 0;
	const  int	REQ_FLAGS   	= 0;
	const  int	REQ_ORIGIN   	= 0;
	const  int	REQ_INTERVAL   	= 0;
	const  int	REQ_LIMIT   	= 0;

	enum DEFINITIONS {
		Struct1,
	}
	// this is how you declare a data structure so that
	// simconnect knows how to fill it/read it.
	[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
	struct Struct1
	{
		// this is how you declare a fixed size string
		[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
		public String   title;
		public double   latitude;
		public double   longitude;
		public double   altitude;
	};
	enum EVENTS {
		EVENT_SIM_START,
		EVENT_SIM_STOP,
		EVENT_PAUSE,
		EVENT_PAUSE_TOGGLE,
		EVENT_BRAKES,
		EVENT_4S_TIMER,
	};
	enum DATA_DEFINE {
		DEFINITION_1
	};
	enum GROUP {
		GROUP_0,
		GROUP_1,
	};
	enum DATA_REQUESTS {
		REQUEST_1,
	};

	/////////////////////////////////////////////////////////////////////////////
	//
	// main form methods
	//
	public SimApp()
	{  	// constructor
		InitializeComponent();
		setButtons(true, false, false);
	}

	// Simconnect client will send a win32 message when there is
	// a packet to process. ReceiveMessage must be called to
	// trigger the events. This model keeps simconnect processing on the main thread.
	//
	protected override void DefWndProc(ref Message m)
	{
		if (m.Msg == WM_USER_SIMCONNECT)
		{
			if (simconnect != null)
			{
				try {
					simconnect.ReceiveMessage();
				} catch (KeyNotFoundException e) {
					displayText("ReceiveMessage KeyNotFound exception");
				} catch (COMException e) {
					displayText("ReceiveMessage COMxception");
					disposeConnection();
				} catch (Exception e) {
					displayText("ReceiveMessage exception: "+e.ToString() );
				}
			}
		}
		else
		{
			base.DefWndProc(ref m);
		}
	}

	private void setButtons(bool bConnect, bool bGet, bool bDisconnect)
	{
		btnConnect.Enabled  	= bConnect;
		btnReqData.Enabled  	= bGet;
		btnEvent1.Enabled   	= bGet;
		btnEvent2.Enabled   	= bGet;
		btnDisconnect.Enabled	= bDisconnect;
	}

	private void disposeConnection()
	{
		if (simconnect != null)
		{
			// Dispose serves the same purpose as SimConnect_Close()
			simconnect.Dispose();
			simconnect = null;
		}
		FS_state1 = false;
		displayText("Connection disposed");
		setButtons(true, false, false);
	}

	private void closeConnection()
	{
		if (simconnect != null)
		{
			// Unsubscribe from all the system events
			simconnect.UnsubscribeFromSystemEvent(EVENTS.EVENT_SIM_START);
			simconnect.UnsubscribeFromSystemEvent(EVENTS.EVENT_SIM_STOP);
			simconnect.UnsubscribeFromSystemEvent(EVENTS.EVENT_PAUSE);

			// Dispose serves the same purpose as SimConnect_Close()
			simconnect.Dispose();
			simconnect  = null;
		}
		displayText("Connection closed");
	}

	// Set up all the SimConnect related event handlers
	private void initSystemEvent()
	{
		FS_state1 = false;

		// listen to connect and quit msgs
		simconnect.OnRecvOpen		+= new SimConnect.RecvOpenEventHandler(simconnect_OnRecvOpen);
		simconnect.OnRecvQuit		+= new SimConnect.RecvQuitEventHandler(simconnect_OnRecvQuit);

		// listen to exceptions
		simconnect.OnRecvException	+= new SimConnect.RecvExceptionEventHandler(simconnect_OnRecvException);

		// listen to events
		simconnect.OnRecvEvent		+= new SimConnect.RecvEventEventHandler(simconnect_OnRecvEvent);

		// catch a simobject data request
		simconnect.OnRecvSimobjectData += new SimConnect.RecvSimobjectDataEventHandler(simconnect_OnRecvSimobjectData);
		simconnect.OnRecvSimobjectDataBytype += new SimConnect.RecvSimobjectDataBytypeEventHandler(simconnect_OnRecvSimobjectDataBytype);

		// define a data structure
		simconnect.AddToDataDefinition(DEFINITIONS.Struct1, "Title", null, SIMCONNECT_DATATYPE.STRING256, 0.0f, SimConnect.SIMCONNECT_UNUSED);
		simconnect.AddToDataDefinition(DEFINITIONS.Struct1, "Plane Latitude", "degrees", SIMCONNECT_DATATYPE.FLOAT64, 0.0f, SimConnect.SIMCONNECT_UNUSED);
		simconnect.AddToDataDefinition(DEFINITIONS.Struct1, "Plane Longitude", "degrees", SIMCONNECT_DATATYPE.FLOAT64, 0.0f, SimConnect.SIMCONNECT_UNUSED);
		simconnect.AddToDataDefinition(DEFINITIONS.Struct1, "Plane Altitude", "feet", SIMCONNECT_DATATYPE.FLOAT64, 0.0f, SimConnect.SIMCONNECT_UNUSED);

		// IMPORTANT: register it with the simconnect managed wrapper marshaller
		// if you skip this step, you will only receive a uint in the .dwData field.
		simconnect.RegisterDataDefineStruct<Struct1>(DEFINITIONS.Struct1);

		// Subscribe to system events
		simconnect.SubscribeToSystemEvent(EVENTS.EVENT_SIM_START, "SimStart");
		simconnect.SubscribeToSystemEvent(EVENTS.EVENT_SIM_STOP, "SimStop");
		simconnect.SubscribeToSystemEvent(EVENTS.EVENT_PAUSE, "Pause");

		// Initially turn off the system events
		//simconnect.SetSystemEventState(EVENTS.EVENT_SIM_START, SIMCONNECT_STATE.OFF);
		//simconnect.SetSystemEventState(EVENTS.EVENT_SIM_STOP, SIMCONNECT_STATE.OFF);
		//simconnect.SetSystemEventState(EVENTS.EVENT_PAUSE, SIMCONNECT_STATE.OFF);

		simconnect.MapClientEventToSimEvent(EVENTS.EVENT_BRAKES,"brakes");
		simconnect.AddClientEventToNotificationGroup(GROUP.GROUP_0, EVENTS.EVENT_BRAKES,false);
		simconnect.MapClientEventToSimEvent(EVENTS.EVENT_PAUSE_TOGGLE, "pause_toggle");
		simconnect.AddClientEventToNotificationGroup(GROUP.GROUP_0, EVENTS.EVENT_PAUSE_TOGGLE, false);
		simconnect.SetNotificationGroupPriority(GROUP.GROUP_0, SimConnect.SIMCONNECT_GROUP_PRIORITY_HIGHEST);
	}

	void simconnect_OnRecvOpen(SimConnect sender, SIMCONNECT_RECV_OPEN data)
	{
		displayText("Connected to FSX");
	}

	// The case where the user closes FSX
	void simconnect_OnRecvQuit(SimConnect sender, SIMCONNECT_RECV data)
	{
		displayText("FSX has exited");
		closeConnection();
	}

	// The case where the user closes the client
	private void SimApp_FormClosed(object sender, FormClosedEventArgs e)
	{
		btnDisconnect_Click(sender, null);
	}

	void simconnect_OnRecvException(SimConnect sender, SIMCONNECT_RECV_EXCEPTION data)
	{
		displayText("Exception received: " + data.dwException);
	}

	void simconnect_OnRecvSimobjectDataBytype(SimConnect sender, SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
	{
		switch ((DATA_REQUESTS)data.dwRequestID)
		{
		case DATA_REQUESTS.REQUEST_1:
			Struct1 s1 = (Struct1)data.dwData[0];

			displayText("ReceiveDataByType");
			displayText("Title: " + s1.title);
			displayText("Lat:   " + s1.latitude);
			displayText("Lon:   " + s1.longitude);
			displayText("Alt:   " + s1.altitude);
			break;

		default:
			displayText("Unknown request ID: " + data.dwRequestID);
			break;
		}
	}

	void simconnect_OnRecvSimobjectData(SimConnect sender, SIMCONNECT_RECV_SIMOBJECT_DATA data)
	{
		switch ((DATA_REQUESTS)data.dwRequestID)
		{
		case DATA_REQUESTS.REQUEST_1:
			Struct1 s1 = (Struct1)data.dwData[0];

			displayText("ReceiveData");
			displayText("Title: " + s1.title);
			displayText("Lat:   " + s1.latitude);
			displayText("Lon:   " + s1.longitude);
			displayText("Alt:   " + s1.altitude);
			break;

		default:
			displayText("Unknown request ID: " + data.dwRequestID);
			break;
		}
	}

	void simconnect_OnRecvEvent(SimConnect sender, SIMCONNECT_RECV_EVENT recEvent)
	{
		switch (recEvent.uEventID)
		{
		case (uint) EVENTS.EVENT_SIM_START:
			displayText("Sim running");
			break;

		case (uint) EVENTS.EVENT_SIM_STOP:
			displayText("Sim stopped");
			break;

		case (uint) EVENTS.EVENT_PAUSE:
			displayText("Pause");
			break;

		case (uint) EVENTS.EVENT_PAUSE_TOGGLE:
			displayText("Pause Toggled");
			break;

		case (uint) EVENTS.EVENT_BRAKES:
			displayText("Brakes");
			break;
		}
	}

	private void btnConnect_Click(object sender, EventArgs e)
	{
		if (simconnect	== null)
		{
			try
			{
				// the constructor is similar to SimConnect_Open in the native API
				simconnect	= new SimConnect("Managed Sim App", this.Handle, WM_USER_SIMCONNECT, null, 0);
				initSystemEvent();
			}
			catch (COMException ex)
			{
				displayText("Unable to connect to FSX " + ex.Message);
			}
			setButtons(false, true, true);
		}
		else
		{
			displayText("Error - try again");
			closeConnection();
			setButtons(true, false, false);
		}
	}

	private void btnDisconnect_Click(object sender, EventArgs e)
	{
		// If they are on, turn off the system event subscriptions
		if (FS_state1)  btnEvent1_Click(sender, null);

		closeConnection();
		setButtons(true, false, false);
	}

	private void btnReqData_Click(object sender, EventArgs e)
	{
		if (simconnect == null) { displayText("no connection"); return; }
		// The following calls return identical information:
		//simconnect.RequestDataOnSimObject(DATA_REQUESTS.REQUEST_1, DEFINITIONS.Struct1, SimConnect.SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD.ONCE, 0, 0, 0, 0);
		simconnect.RequestDataOnSimObjectType(DATA_REQUESTS.REQUEST_1, DEFINITIONS.Struct1, 0, SIMCONNECT_SIMOBJECT_TYPE.USER);
		displayText("Request sent...");
	}

	// example, although currently not used
	private void btnEvent0_Click(object sender, EventArgs e)
	{
		if (simconnect == null) { displayText("no connection"); return; }
		// Toggle switch
		FS_state1 = !FS_state1;

		if (FS_state1)
		{
			simconnect.SetSystemEventState(EVENTS.EVENT_SIM_START, SIMCONNECT_STATE.ON);
			simconnect.SetSystemEventState(EVENTS.EVENT_SIM_STOP, SIMCONNECT_STATE.ON);
			simconnect.SetSystemEventState(EVENTS.EVENT_PAUSE, SIMCONNECT_STATE.ON);
			btnEvent1.Text = "Stop sim events";
		}
		else
		{
			simconnect.SetSystemEventState(EVENTS.EVENT_SIM_START, SIMCONNECT_STATE.OFF);
			simconnect.SetSystemEventState(EVENTS.EVENT_SIM_STOP, SIMCONNECT_STATE.OFF);
			simconnect.SetSystemEventState(EVENTS.EVENT_PAUSE, SIMCONNECT_STATE.OFF);
			btnEvent1.Text = "Request sim events";
		}
	}

	private void btnEvent1_Click(object sender, EventArgs e)
	{
		if (simconnect == null) { displayText("no connection"); return; }
		displayText("transmit brakes");
		simconnect.TransmitClientEvent(SimConnect.SIMCONNECT_OBJECT_ID_USER,
			EVENTS.EVENT_BRAKES, 0, GROUP.GROUP_0,
			SIMCONNECT_EVENT_FLAG.GROUPID_IS_PRIORITY);
	}

	private void btnEvent2_Click(object sender, EventArgs e)
	{
		if (simconnect == null) { displayText("no connection"); return; }
		displayText("transmit pause");
		simconnect.TransmitClientEvent(SimConnect.SIMCONNECT_OBJECT_ID_USER,
			EVENTS.EVENT_PAUSE_TOGGLE, 0, GROUP.GROUP_0,
			SIMCONNECT_EVENT_FLAG.GROUPID_IS_PRIORITY);
	}

	// Response number
	int response	= 1;

	// Output text - display a maximum of 10 lines
	string output	= "\n\n\n\n\n\n\n\n\n\n";

	void displayText(string s)
	{
		// remove first string from output
		output	= output.Substring(output.IndexOf("\n") + 1);

		// add the new string
		output += "\n" + response++ + ":" + s;

		// display it
		richResponse.Text	= output;
	}
} // ***** End of class *****
} // ***** End of namespace *****
