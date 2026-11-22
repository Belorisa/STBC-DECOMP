// NiAppMain.cpp -- the application framework's WinMain
//
// Gamebryo 1.1.2: AppFrameworks/NiAppMain.cpp holds WinMain. Retail's unit
// after NiMemStream's holds the game's WinMain (0x86EFF0), its <iostream>
// quads (0x86F250), the locale pair and the 70-stub NiMain run.

#include "NiRTLib.h"
#include "NiApplication.h"
#include "NiAppWindow.h"
#include "NiMessageLoop.h"
#include "NiFile.h"
#include "NiSystem.h"
#include "NiMain.h"
#include <stdio.h>

/*
================
WinMain

The program's entry: reads the command line from ./cmdline.txt when there
is one, creates the application, registers the window class, creates the
main window and runs the message loop, then reports leaked NiObjects to the
debugger. Gamebryo 1.1.2's NiAppMain.cpp is the same function; this
version has no NiInit/NiShutdown and runs the application's message loop.
================
*/
// FUNCTION: STBC 0x0086eff0
int WINAPI WinMain( HINSTANCE hI, HINSTANCE hPI, LPSTR cmdline, int iWinMode ) {
	// for determining if there are 'object leaks'
	unsigned int initialCount = NiObject::GetTotalObjectCount();

	if ( NiApplication::application != NULL ) {
		// only one instance of application allowed
		NiMessageBox( "Application already active", "NetImmerse Application" );
		return 0;
	}

	// If cmdline.txt file exists in the work directory, read it and
	// set it as the command line
	NiFile *cmdlineFile = new NiFile( "./cmdline.txt", NiFile::READ_ONLY, 0x2800 );
	if ( cmdlineFile && *cmdlineFile ) {
		// artificial 1024-character limit
		char *commandLine = new char[1024];
		int length = 0;
		char c;
		// Read until limit is reached or end of file or end of string
		while ( length < 1023 ) {
			if ( !( cmdlineFile->Read( &c, 1 ) || c == '\0' ) ) {
				break;
			}

			// Factor out newlines and carriage returns
			// Replace with ' ' in case newline seperates valid parameters
			if ( c == '\n' || c == '\r' ) {
				c = ' ';
			}
			commandLine[length++] = c;
		}
		commandLine[length] = '\0';

		// let the application know about the command line
		NiApplication::SetCommandLine( commandLine );

		delete [] commandLine;
	} else {
		// let the application know about the command line
		NiApplication::SetCommandLine( cmdline );
	}

	delete cmdlineFile;

	// create the application
	NiApplication *theApp = NiApplication::Create();
	if ( !theApp ) {
		NiMessageBox( "Unable to create application", "NetImmerse Application" );
		return 0;
	}

	// Set reference
	NiApplication::SetInstanceReference( hI );

	// process the command line for renderer selection
	NiApplication::CommandSelectRenderer();

	// Now, we are ready to initialize the window and the main loop.

	// register the window class
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = NiMessageLoop::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hI;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszClassName = "NetImmerse Application";

	// register menu (if requested)
	if ( theApp->GetMenuID() ) {
		wc.lpszMenuName = MAKEINTRESOURCE( theApp->GetMenuID() );
	} else {
		wc.lpszMenuName = 0;
	}

	RegisterClass( &wc );

	// create the application window
	theApp->GetAppWindow()->CreateMainWindow( iWinMode );

	// allow the application to initialize before starting the message pump
	long result;	// BUG: left unset when Initialize fails
	if ( theApp->Initialize() ) {
		result = theApp->GetMessageLoop()->Run();
	}

	theApp->Terminate();

	delete theApp;

	// for determining if there are 'object leaks'
	unsigned int finalCount = NiObject::GetTotalObjectCount();
	char message[256];
	sprintf( message, "\nNetImmerse NiObject counts:  initial = %u, final = %u. ", initialCount, finalCount );
	NiOutputDebugString( message );
	if ( finalCount > initialCount ) {
		unsigned int difference = finalCount - initialCount;
		sprintf( message, "Application is leaking %u objects\n\n", difference );
		NiOutputDebugString( message );
	} else {
		NiOutputDebugString( "Application has no object leaks.\n\n" );
	}
	return result;
}

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0086f250 _$E17
// FUNCTION: STBC 0x0086f270 _$E15
// FUNCTION: STBC 0x0086f280 _$E23
// FUNCTION: STBC 0x0086f2a0 _$E21
// FUNCTION: STBC 0x0086f2d0 _$E401

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0086f2b0 _$E34
// FUNCTION: STBC 0x0086f2c0 _$E190
// FUNCTION: STBC 0x0086f2f0 _$E45
// FUNCTION: STBC 0x0086f300 _$E49
// FUNCTION: STBC 0x0086f310 _$E53
// FUNCTION: STBC 0x0086f320 _$E57
// FUNCTION: STBC 0x0086f330 _$E177
// FUNCTION: STBC 0x0086f340 _$E119
// FUNCTION: STBC 0x0086f350 _$E132
// FUNCTION: STBC 0x0086f360 _$E182
// FUNCTION: STBC 0x0086f370 _$E195
// FUNCTION: STBC 0x0086f380 _$E186
// FUNCTION: STBC 0x0086f390 _$E199
// FUNCTION: STBC 0x0086f3a0 _$E206
// FUNCTION: STBC 0x0086f3b0 _$E239
// FUNCTION: STBC 0x0086f3c0 _$E243
// FUNCTION: STBC 0x0086f3d0 _$E81
// FUNCTION: STBC 0x0086f3e0 _$E225
// FUNCTION: STBC 0x0086f3f0 _$E232
// FUNCTION: STBC 0x0086f400 _$E255
// FUNCTION: STBC 0x0086f410 _$E259
// FUNCTION: STBC 0x0086f420 _$E299
// FUNCTION: STBC 0x0086f430 _$E305
// FUNCTION: STBC 0x0086f440 _$E247
// FUNCTION: STBC 0x0086f450 _$E251
// FUNCTION: STBC 0x0086f460 _$E263
// FUNCTION: STBC 0x0086f470 _$E267
// FUNCTION: STBC 0x0086f480 _$E271
// FUNCTION: STBC 0x0086f490 _$E275
// FUNCTION: STBC 0x0086f4a0 _$E279
// FUNCTION: STBC 0x0086f4b0 _$E283
// FUNCTION: STBC 0x0086f4c0 _$E287
// FUNCTION: STBC 0x0086f4d0 _$E291
// FUNCTION: STBC 0x0086f4e0 _$E86
// FUNCTION: STBC 0x0086f4f0 _$E295
// FUNCTION: STBC 0x0086f500 _$E91
// FUNCTION: STBC 0x0086f510 _$E309
// FUNCTION: STBC 0x0086f520 _$E313
// FUNCTION: STBC 0x0086f530 _$E317
// FUNCTION: STBC 0x0086f540 _$E321
// FUNCTION: STBC 0x0086f550 _$E96
// FUNCTION: STBC 0x0086f560 _$E325
// FUNCTION: STBC 0x0086f570 _$E329
// FUNCTION: STBC 0x0086f580 _$E333
// FUNCTION: STBC 0x0086f590 _$E102
// FUNCTION: STBC 0x0086f5a0 _$E107
// FUNCTION: STBC 0x0086f5b0 _$E137
// FUNCTION: STBC 0x0086f5c0 _$E337
// FUNCTION: STBC 0x0086f5d0 _$E142
// FUNCTION: STBC 0x0086f5e0 _$E341
// FUNCTION: STBC 0x0086f5f0 _$E345
// FUNCTION: STBC 0x0086f600 _$E147
// FUNCTION: STBC 0x0086f610 _$E349
// FUNCTION: STBC 0x0086f620 _$E112
// FUNCTION: STBC 0x0086f630 _$E353
// FUNCTION: STBC 0x0086f640 _$E357
// FUNCTION: STBC 0x0086f650 _$E152
// FUNCTION: STBC 0x0086f660 _$E157
// FUNCTION: STBC 0x0086f670 _$E361
// FUNCTION: STBC 0x0086f680 _$E365
// FUNCTION: STBC 0x0086f690 _$E369
// FUNCTION: STBC 0x0086f6a0 _$E373
// FUNCTION: STBC 0x0086f6b0 _$E377
// FUNCTION: STBC 0x0086f6c0 _$E381
// FUNCTION: STBC 0x0086f6d0 _$E385
// FUNCTION: STBC 0x0086f6e0 _$E389
// FUNCTION: STBC 0x0086f6f0 _$E162
// FUNCTION: STBC 0x0086f700 _$E393
// FUNCTION: STBC 0x0086f710 _$E167
// FUNCTION: STBC 0x0086f720 _$E172
