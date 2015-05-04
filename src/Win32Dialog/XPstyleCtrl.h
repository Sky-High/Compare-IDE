// settings to enable the newer XP style control widgets
// define the manifest, library and include file
// "ComCtl32.lib" for InitCommonControls()
//
#pragma once

#pragma comment(linker, \
"\"/manifestdependency:type='Win32' "\
"name='Microsoft.Windows.Common-Controls' "\
"version='6.0.0.0' "\
"processorArchitecture='*' "\
"publicKeyToken='6595b64144ccf1df' "\
"language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#include <CommCtrl.h>
#include <richedit.h>
