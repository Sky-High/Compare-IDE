// (un)Managed Form sample
//
// fsx SimConnect test
//
// Note that static variables and methods are only visible in this file.

#include "stdafx.h"
#include <msclr\gcroot.h>
#include "AppForm.h"
#include "FsxConnect.h"

namespace SimApp01Form {

// variables
static ::fsxConnect *	    	sc;
static msclr::gcroot<AppForm^>	pNativForm;			// smart native pointer (set to 'this' in ::AppForm)

// --------------------------------------------------------
/// <summary>
/// Summary for AppForm
/// </summary>
AppForm::AppForm(void) {
	response    	= 1;	    	    	// starting line number
	output	    	= gcnew String( L"\n\n\n\n\n\n\n\n\n\n\n\n");
	pNativForm  	= this;	    	    	//native pointer to managed class
	sc  	    	= new ::fsxConnect();

	InitializeComponent();	    	    	// initialize form
	setButtons(true, true, false);
}
/// <summary>
/// Clean up any resources being used.
/// </summary>
AppForm::~AppForm()
{
	if (components)	delete components;
}
//
// --------------------------------------------------------
// SimConnect client will send a win32 message when there is 
// a packet to process. SimConnect_CallDispatch must be called to
// handle the events.
//
// Form.DefWndProc Method . This member overrides Control.DefWndProc.
//
void AppForm::DefWndProc(Message% m)
{
	if (m.Msg == WM_USER_SIMCONNECT)
	{
		// dispatch SimConnect messages
		sc->fsxDispatch();
	}
	else
	{
		// handle non-SimConnect messages in the default way.
		// use __super in MSVC although the fully qualified name (FQN)
		//	System::Windows::Forms::Form::DefWndProc(m)
		// is more "correct" in view of the multiple inheritance in C++
		__super::DefWndProc(m);
	}
}
//
// --------------------------------------------------------
//
void AppForm::setButtons(bool bConnect, bool bGet, bool bDisconnect)
{
	this->btnConnect->Enabled   	= bConnect;
	this->btnEvent1->Enabled    	= bGet;
	this->btnEvent2->Enabled    	= bGet;
	this->btnReqData->Enabled   	= bGet;
	this->btnDisconnect->Enabled	= bDisconnect;
}
// --------------------------------------------------------
System::Void AppForm::btnConnect_Click(System::Object^ sender, System::EventArgs^ e)
{
	HRESULT	hr	= sc->fsxOpen(this->Handle);	// Forms::Handle is hWndMain, window handle of main Form

	if ( !SUCCEEDED(hr) )	{
		displayText("openFsx error hWnd="+ this->Handle.ToString() );
		sc->fsxClose();
		setButtons(true, true, false);
		return;
	}

	// Set up the data definition, but do not yet do anything with it
	sc->AddToDataDefinition(DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
	sc->AddToDataDefinition(DEFINITION_1, "Plane Altitude", "feet");
	sc->AddToDataDefinition(DEFINITION_1, "Plane Longitude", "degrees");
	sc->AddToDataDefinition(DEFINITION_1, "Plane Latitude", "degrees");

	// Request an event when the simulation starts
	sc->SubscribeToSystemEvent(EVENT_SIM_START, "SimStart");
	sc->SubscribeToSystemEvent(EVENT_SIM_STOP, "SimStop");
	sc->SubscribeToSystemEvent(EVENT_PAUSE, "Pause");

	sc->MapClientEventToSimEvent(EVENT_BRAKES, "brakes");
	sc->AddClientEventToNotificationGroup(GROUP_1, EVENT_BRAKES);
	sc->MapClientEventToSimEvent(EVENT_PAUSE_TOGGLE, "pause_toggle");
	sc->AddClientEventToNotificationGroup(GROUP_1, EVENT_PAUSE_TOGGLE);
	sc->SetNotificationGroupPriority(GROUP_1, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

	setButtons(false, true, true);
	displayText( L"open done" );
}
// --------------------------------------------------------
System::Void AppForm::btnDisconnect_Click(System::Object^ sender, System::EventArgs^ e) {
	sc->fsxClose();
	setButtons(true, true, false);
}
// --------------------------------------------------------
System::Void AppForm::btnEvent1_Click(System::Object^ sender, System::EventArgs^ e) {
	displayText(L"transmit brakes");
	sc->TransmitClientEvent(SIMCONNECT_OBJECT_ID_USER, EVENT_BRAKES);
}
// --------------------------------------------------------
System::Void AppForm::btnEvent2_Click(System::Object^ sender, System::EventArgs^ e) {
	displayText(L"transmit pause toggle");
	sc->TransmitClientEvent(SIMCONNECT_OBJECT_ID_USER, EVENT_PAUSE_TOGGLE);
}
// --------------------------------------------------------
System::Void AppForm::btnReqData_Click(System::Object^ sender, System::EventArgs^ e) {
	sc->RequestDataOnSimObject(DEFINITION_1, REQ_OBJECT_ID0);
}
// --------------------------------------------------------
System::Void AppForm::FormClosedEventHandler(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
	displayText("FormClosedEventHandler");
}
// --------------------------------------------------------
void AppForm::displayText(System::String ^ s) {
	output		= output->Substring(output->IndexOf("\n") + 1);
	output		+= "\n" + (response++) + ": " + s;
	this->richResponse->Text	= output;
}
}	// ***** end namespace SimApp01Form *****

// --------------------------------------------------------

namespace nsFsx {

// Show text for testing.
// varargs is not supported in managed mode (/clr), so we have to stick
// to StringCbPrintf combined with displayText, or use a pragma directive.
// see
// - msdn.microsoft.com/en-us/library/ffkc918h.aspx
// - msdn.microsoft.com/en-us/library/0adb9zxe.aspx
void displayText0(TCHAR* s)
{
	SimApp01Form::pNativForm->displayText(gcnew System::String(s) );
}

// ! unmanaged native code follows !
#pragma managed(push, off)
#include <stdarg.h>
TCHAR	sDisplayBuffer[512];

void displayTextF(TCHAR *sFmt, ...)
{
	va_list	pArg;
	va_start(pArg, sFmt);
	_vsntprintf_s(sDisplayBuffer, sizeof(sDisplayBuffer), _TRUNCATE, sFmt, pArg);
	va_end(pArg);
	displayText0(sDisplayBuffer);
}
#pragma managed(pop)

}