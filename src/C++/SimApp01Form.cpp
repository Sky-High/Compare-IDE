// SimApp01Form.cpp : main project file.
#include "stdafx.h"

// Define the functions that do the work
#include "AppForm.h"

using namespace SimApp01Form;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew AppForm());
	return 0;
}
