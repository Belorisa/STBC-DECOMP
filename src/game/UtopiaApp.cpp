/*
===============================================================================

	UtopiaApp.cpp

	The game's application: the display options, the renderer and the
	command line's game options.

===============================================================================
*/

#include "UtopiaApp.h"
#include "GraphicsMenu.h"
#include "GraphicsModeInfo.h"
#include "GameSpy.h"
#include "MainWindow.h"
#include "MultiplayerWindow.h"
#include "NiAlphaAccumulator.h"
#include "PyEmbed.h"
#include "NetFile.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGString.h"
#include "TGSystemWrapperClass.h"
#include "TGTimerManager.h"
#include "TopWindow.h"
#include "VarManagerClass.h"
#include "NiAppWindow.h"
#include "NiCommand.h"
#include "NiRendererDialog.h"
#include "TGConfigMapping.h"
#include "TGModule.h"
#include "TGMovieManager.h"
#include "TGSoundManager.h"
#include "UtopiaModule.h"
#include "renderer/NiD3DRender.h"
#include "renderer/NiImage.h"
#include <mmsystem.h>
#include <stdio.h>
#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00439010 _$E25
// FUNCTION: STBC 0x00439020 _$E24
// FUNCTION: STBC 0x00439030 _$E181
// FUNCTION: STBC 0x00439040 _$E180
// FUNCTION: STBC 0x00439050 _$E36
// FUNCTION: STBC 0x00439060 _$E35
// FUNCTION: STBC 0x00439070 _$E40
// FUNCTION: STBC 0x00439080 _$E39
// FUNCTION: STBC 0x00439090 _$E44
// FUNCTION: STBC 0x004390a0 _$E43
// FUNCTION: STBC 0x004390b0 _$E48
// FUNCTION: STBC 0x004390c0 _$E47
// FUNCTION: STBC 0x004390d0 _$E168
// FUNCTION: STBC 0x004390e0 _$E167
// FUNCTION: STBC 0x004390f0 _$E110
// FUNCTION: STBC 0x00439100 _$E109
// FUNCTION: STBC 0x00439110 _$E123
// FUNCTION: STBC 0x00439120 _$E122
// FUNCTION: STBC 0x00439130 _$E173
// FUNCTION: STBC 0x00439140 _$E172
// FUNCTION: STBC 0x00439170 _$E186
// FUNCTION: STBC 0x00439180 _$E185
// FUNCTION: STBC 0x00439190 _$E177
// FUNCTION: STBC 0x004391a0 _$E176
// FUNCTION: STBC 0x004391b0 _$E190
// FUNCTION: STBC 0x004391c0 _$E189
// FUNCTION: STBC 0x004391d0 _$E197
// FUNCTION: STBC 0x004391e0 _$E196
// FUNCTION: STBC 0x004391f0 _$E259
// FUNCTION: STBC 0x00439200 _$E258
// FUNCTION: STBC 0x00439250 _$E263
// FUNCTION: STBC 0x00439260 _$E262
// FUNCTION: STBC 0x00439270 _$E72
// FUNCTION: STBC 0x00439280 _$E71
// FUNCTION: STBC 0x00439340 _$E245
// FUNCTION: STBC 0x00439350 _$E244
// FUNCTION: STBC 0x00439360 _$E252
// FUNCTION: STBC 0x00439370 _$E251
// FUNCTION: STBC 0x00439380 _$E342
// FUNCTION: STBC 0x00439390 _$E341
// FUNCTION: STBC 0x004393a0 _$E346
// FUNCTION: STBC 0x004393b0 _$E345
// FUNCTION: STBC 0x004393c0 _$E386
// FUNCTION: STBC 0x004393d0 _$E385
// FUNCTION: STBC 0x004393e0 _$E392
// FUNCTION: STBC 0x004393f0 _$E391
// FUNCTION: STBC 0x00439400 _$E332
// FUNCTION: STBC 0x00439410 _$E331
// FUNCTION: STBC 0x00439420 _$E338
// FUNCTION: STBC 0x00439430 _$E337
// FUNCTION: STBC 0x00439440 _$E350
// FUNCTION: STBC 0x00439450 _$E349
// FUNCTION: STBC 0x00439460 _$E354
// FUNCTION: STBC 0x00439470 _$E353
// FUNCTION: STBC 0x00439480 _$E358
// FUNCTION: STBC 0x00439490 _$E357
// FUNCTION: STBC 0x004394a0 _$E362
// FUNCTION: STBC 0x004394b0 _$E361
// FUNCTION: STBC 0x004394c0 _$E366
// FUNCTION: STBC 0x004394d0 _$E365
// FUNCTION: STBC 0x004394e0 _$E370
// FUNCTION: STBC 0x004394f0 _$E369
// FUNCTION: STBC 0x00439500 _$E374
// FUNCTION: STBC 0x00439510 _$E373
// FUNCTION: STBC 0x00439520 _$E378
// FUNCTION: STBC 0x00439530 _$E377
// FUNCTION: STBC 0x00439540 _$E77
// FUNCTION: STBC 0x00439550 _$E76
// FUNCTION: STBC 0x00439560 _$E382
// FUNCTION: STBC 0x00439570 _$E381
// FUNCTION: STBC 0x00439580 _$E82
// FUNCTION: STBC 0x00439590 _$E81
// FUNCTION: STBC 0x004395a0 _$E396
// FUNCTION: STBC 0x004395b0 _$E395
// FUNCTION: STBC 0x004395c0 _$E400
// FUNCTION: STBC 0x004395d0 _$E399
// FUNCTION: STBC 0x004395e0 _$E404
// FUNCTION: STBC 0x004395f0 _$E403
// FUNCTION: STBC 0x00439600 _$E408
// FUNCTION: STBC 0x00439610 _$E407
// FUNCTION: STBC 0x00439620 _$E87
// FUNCTION: STBC 0x00439630 _$E86
// FUNCTION: STBC 0x00439640 _$E412
// FUNCTION: STBC 0x00439650 _$E411
// FUNCTION: STBC 0x00439660 _$E416
// FUNCTION: STBC 0x00439670 _$E415
// FUNCTION: STBC 0x00439680 _$E420
// FUNCTION: STBC 0x00439690 _$E419
// FUNCTION: STBC 0x004396a0 _$E93
// FUNCTION: STBC 0x004396b0 _$E92
// FUNCTION: STBC 0x004396e0 _$E98
// FUNCTION: STBC 0x004396f0 _$E97
// FUNCTION: STBC 0x00439700 _$E128
// FUNCTION: STBC 0x00439710 _$E127
// FUNCTION: STBC 0x004397e0 _$E424
// FUNCTION: STBC 0x004397f0 _$E423
// FUNCTION: STBC 0x00439800 _$E133
// FUNCTION: STBC 0x00439810 _$E132
// FUNCTION: STBC 0x00439820 _$E428
// FUNCTION: STBC 0x00439830 _$E427
// FUNCTION: STBC 0x00439840 _$E432
// FUNCTION: STBC 0x00439850 _$E431
// FUNCTION: STBC 0x00439860 _$E138
// FUNCTION: STBC 0x00439870 _$E137
// FUNCTION: STBC 0x00439880 _$E436
// FUNCTION: STBC 0x00439890 _$E435
// FUNCTION: STBC 0x004398a0 _$E103
// FUNCTION: STBC 0x004398b0 _$E102
// FUNCTION: STBC 0x004398c0 _$E440
// FUNCTION: STBC 0x004398d0 _$E439
// FUNCTION: STBC 0x004398e0 _$E444
// FUNCTION: STBC 0x004398f0 _$E443
// FUNCTION: STBC 0x00439900 _$E143
// FUNCTION: STBC 0x00439910 _$E142
// FUNCTION: STBC 0x00439920 _$E148
// FUNCTION: STBC 0x00439930 _$E147
// FUNCTION: STBC 0x00439970 _$E448
// FUNCTION: STBC 0x00439980 _$E447
// FUNCTION: STBC 0x00439990 _$E452
// FUNCTION: STBC 0x004399a0 _$E451
// FUNCTION: STBC 0x004399b0 _$E456
// FUNCTION: STBC 0x004399c0 _$E455
// FUNCTION: STBC 0x004399d0 _$E460
// FUNCTION: STBC 0x004399e0 _$E459
// FUNCTION: STBC 0x004399f0 _$E464
// FUNCTION: STBC 0x00439a00 _$E463
// FUNCTION: STBC 0x00439a10 _$E468
// FUNCTION: STBC 0x00439a20 _$E467
// FUNCTION: STBC 0x00439a30 _$E472
// FUNCTION: STBC 0x00439a40 _$E471
// FUNCTION: STBC 0x00439a50 _$E476
// FUNCTION: STBC 0x00439a60 _$E475
// FUNCTION: STBC 0x00439a70 _$E153
// FUNCTION: STBC 0x00439a80 _$E152
// FUNCTION: STBC 0x00439a90 _$E480
// FUNCTION: STBC 0x00439aa0 _$E479
// FUNCTION: STBC 0x00439ab0 _$E158
// FUNCTION: STBC 0x00439ac0 _$E157
// FUNCTION: STBC 0x00439ad0 _$E163
// FUNCTION: STBC 0x00439ae0 _$E162
// FUNCTION: STBC 0x00439b00 _$E484
// FUNCTION: STBC 0x00439b10 _$E483
// FUNCTION: STBC 0x00439b20 _$E489
// FUNCTION: STBC 0x00439b30 _$E488
// FUNCTION: STBC 0x00439b40 _$E493
// FUNCTION: STBC 0x00439b50 _$E492

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale id pair.
// FUNCTION: STBC 0x00438820 _$E307
// FUNCTION: STBC 0x00438830 _$E304
// FUNCTION: STBC 0x00438840 _$E306
// FUNCTION: STBC 0x00438850 _$E305
// FUNCTION: STBC 0x00438860 _$E313
// FUNCTION: STBC 0x00438870 _$E310
// FUNCTION: STBC 0x00438880 _$E312
// FUNCTION: STBC 0x00438890 _$E311
// FUNCTION: STBC 0x00439940 _$E511
// FUNCTION: STBC 0x00439960 _$E510


// The shortest time between two idle turns while GameSpy runs: other
// programs get the rest (milliseconds).
#define GAMESPY_IDLE_PERIOD		50

// The direct start's role (invented names).
enum hostMode_t {
	HOST_MODE_NONE,				// "Client": joins the game at "IP"
	HOST_MODE_LISTENER,			// "Listener": hosts and plays
	HOST_MODE_DEDICATED			// "Dedicated": hosts without playing
};

// TODO identity: a static setter of the unit after TGDirectory's (it stores
// the flag at 0x99C8B9, which nothing reads by address); Initialize hands it
// the fullscreen flag.
void Unknown71E790( bool fullscreen );

// The framework's event manager (the SWIG variable g_kEventManager).
// FUNCTION: STBC 0x00437eb0 _$E498
// FUNCTION: STBC 0x00437ec0 _$E495
// FUNCTION: STBC 0x00437ed0 _$E497
// FUNCTION: STBC 0x00437ee0 _$E496
// DATA: STBC 0x0097f838
TGEventManager	g_kEventManager;

// The timer managers of game time and of real time (the SWIG variables
// g_kTimerManager and g_kRealtimeTimerManager), constructed after the event
// manager their timers' events go to. Declared together: one initialiser
// constructs both (0x437F00) and one exit-time function destroys both.
// FUNCTION: STBC 0x00437ef0 _$E503
// FUNCTION: STBC 0x00437f00 _$E500
// FUNCTION: STBC 0x00437f20 _$E502
// FUNCTION: STBC 0x00437f30 _$E501
// DATA: STBC 0x0097f898 ?g_kTimerManager@@3VTGTimerManager@@A
// DATA: STBC 0x0097f810 ?g_kRealtimeTimerManager@@3VTGTimerManager@@A
TGTimerManager	g_kTimerManager, g_kRealtimeTimerManager;

// The "SkipChecksum" command-line switch (NetFile.h).
// DATA: STBC 0x0097f94c
bool skipChecksum;

// When the last idle turn ran, while GameSpy runs (timeGetTime).
// DATA: STBC 0x0097f950
static DWORD lastIdleTime;

/*
================
NiApplication::Create

The framework's main function creates the application through it.
================
*/
// FUNCTION: STBC 0x00437f50
NiApplication *NiApplication::Create() {
	return new UtopiaApp;
}

/*
================
UtopiaApp::UtopiaApp

Reads the display options, and lets the command line force the window
mode. The width is one of the supported modes, each with its height.
================
*/
// FUNCTION: STBC 0x00437fb0
UtopiaApp::UtopiaApp( void ) : TGApplication( "Bridge Commander", 800, 600, 0, 0, 0 ) {
	deviceDesc = NULL;
	unknown31 = false;
	directStart = false;
	testMenuState = 0;
	unknown30 = true;
	backgroundColor = NiColor::BLACK;
	minFramePeriod = 1.0f / 60.0f;
	timeRate = 0.25f;
	fullscreen = true;
	unknown2D = true;
	width = 800;
	height = 600;
	bitDepth = 16;

	if ( g_kConfigMapping.LoadConfigFile( "Options.cfg" ) ) {
		if ( g_kConfigMapping.HasValue( "Graphics Options", "Fullscreen Mode" ) ) {
			fullscreen = g_kConfigMapping.GetIntValue( "Graphics Options", "Fullscreen Mode" ) != 0;
		}
		if ( g_kConfigMapping.HasValue( "Graphics Options", "Display Width" ) ) {
			width = g_kConfigMapping.GetIntValue( "Graphics Options", "Display Width" );
		}
		if ( g_kConfigMapping.HasValue( "Graphics Options", "Display Height" ) ) {
			height = g_kConfigMapping.GetIntValue( "Graphics Options", "Display Height" );
		}
		if ( g_kConfigMapping.HasValue( "Graphics Options", "Display Depth" ) ) {
			bitDepth = g_kConfigMapping.GetIntValue( "Graphics Options", "Display Depth" );
		}
	}

	NiCommand *commandLine = command;
	if ( commandLine->Boolean( "ForceWindowed" ) ) {
		fullscreen = false;
	}
	if ( commandLine->Boolean( "ForceFullscreen" ) ) {
		fullscreen = true;
	}

	GetAppWindow()->SetWindowStyle( WS_POPUP );
	SnapToMode();

	if ( fullscreen ) {
		GetAppWindow()->SetHeight( GetSystemMetrics( SM_CYFULLSCREEN ) );
		GetAppWindow()->SetWidth( GetSystemMetrics( SM_CXFULLSCREEN ) );
	} else {
		GetAppWindow()->SetHeight( height );
		GetAppWindow()->SetWidth( width );
	}
}

/*
================
UtopiaApp::CreateRenderer

Creates the Direct3D renderer on the device the options name (the first
fullscreen device, else), at the options' size when fullscreen, and a
camera that clears the screen once.
================
*/
// FUNCTION: STBC 0x00438290
// DRAFT: register: retail keeps zero in ebx for the NiPointer tests and the device-name local in memory; the window-mode block's loads are scheduled differently
bool UtopiaApp::CreateRenderer() {
	char *deviceName = NULL;
	Ni3DDeviceDesc *device = NULL;
	if ( g_kConfigMapping.HasValue( "Graphics Options", "Display Device" ) ) {
		deviceName = g_kConfigMapping.GetStringValue( "Graphics Options", "Display Device" );
	}

	HWND window = fullscreen ? appWindow->GetWindow() : appWindow->GetRenderWindow();
	char errorText[512];

	NiImage::defaultMipmap = true;
	unknown9A09CC = true;

	if ( fullscreen ) {
		if ( g_kConfigMapping.HasValue( "Graphics Options", "Display Width" ) &&
			 g_kConfigMapping.HasValue( "Graphics Options", "Display Height" ) ) {
			width = g_kConfigMapping.GetIntValue( "Graphics Options", "Display Width" );
			height = g_kConfigMapping.GetIntValue( "Graphics Options", "Display Height" );
		} else {
			width = 800;
			height = 600;
		}
	}

	if ( bitDepth != 0 ) {
		unsigned int flags = 1;
		if ( bitDepth == 32 ) {
			flags = 2;
		}
		if ( unknown2E ) {
			flags |= 4;
		}
		if ( unknown2D ) {
			if ( deviceName == NULL ) {
				if ( fullscreen ) {
					device = NiD3DRender::GetFullscreenDevice();
				} else {
					device = NiD3DRender::GetWindowedDevice();
				}
			} else {
				NiTList<Ni3DDeviceDesc *> *devices = NiD3DRender::GetDisplayDevices();
				NiTListIterator pos = devices->GetHeadPos();
				while ( pos != NULL && device == NULL ) {
					Ni3DDeviceDesc *candidate = devices->GetNext( pos );
					if ( candidate != NULL && candidate->name != NULL && strcmp( candidate->name, deviceName ) == 0 ) {
						device = candidate;
					}
				}
				if ( device == NULL ) {
					device = NiD3DRender::GetFullscreenDevice();
				}
			}
		} else {
			device = NiD3DRender::GetSoftwareDevice();
		}

		if ( device != NULL ) {
			if ( device->GetName() != NULL ) {
				deviceDesc = new char[strlen( device->GetName() ) + 1];
				strcpy( deviceDesc, device->GetName() );
			}
			if ( !unknown30 ) {
				flags |= 0x40;
			}
			if ( bitDepth == 16 || bitDepth == 32 ) {
				if ( bitDepth == 16 ) {
					flags |= 8;
				} else {
					flags |= 0x10;
				}
			}
			if ( fullscreen ) {
				renderer = NiD3DRender::Create( 1, width, height, flags, window, 0, device );
			} else {
				// the window's client area takes the display size
				RECT rect;
				rect.left = 0;
				rect.top = 0;
				rect.right = width;
				rect.bottom = height;
				DWORD style = GetWindowLong( application->GetAppWindow()->GetWindow(), GWL_STYLE );
				AdjustWindowRectEx( &rect, style, FALSE, GetWindowLong( application->GetAppWindow()->GetWindow(), GWL_EXSTYLE ) );
				MoveWindow( application->GetAppWindow()->GetWindow(), 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE );
				renderer = NiD3DRender::Create( 0, width, height, flags, window, 0, device );
			}
		} else {
			sprintf( errorText, "D3D Render Creation Error\nCan't find a D3D Device\n" );
		}
		if ( !renderer ) {
			sprintf( errorText, "D3D Render Creation Error\nError string from renderer was:\n\"%s\"\n", NiD3DRender::errorString );
		}
	} else {
		NiPointer<NiD3DRender> selected = NiSelectRenderer( appWindow->GetWindow(), appWindow->GetRenderWindow(), unknown2E, fullscreen );
		renderer = selected;
		if ( !renderer ) {
			return false;
		}
	}
	if ( renderer ) {
		if ( renderer->VirtualSlot13() & 8 ) {
			unknown2D = true;
		} else {
			unknown2D = false;
		}
		NiImage::AddRenderer( renderer );
		renderer->SetBackgroundColor( NiColor::BLACK );

		NiPointer<NiCamera> clearCamera = new NiCamera;
		clearCamera->SetRenderer( renderer );
		clearCamera->Clear( 1 );
		clearCamera->Click();
		clearCamera->Unknown7E81B0();
		return true;
	}
	NiMessageBox( errorText, "Renderer Creation Failure" );
	return false;
}

/*
================
UtopiaApp::Initialize

Reads the command line's game options: a direct start into a multiplayer
game ("Client" with "IP" and "Name", or "Listener" or "Dedicated" with
"Mission", each with an optional "Password"), "SkipChecksum" and the test
menu switches. Then hides the cursor, sorts the renderer's geometry by
alpha, starts the modules and the clocks, and for a direct start sets the
multiplayer options and opens the multiplayer pane.
================
*/
// FUNCTION: STBC 0x004388a0
// DRAFT: left: the direct start's dedicated-server arm, which retail lays out after the client arm of the multiplayer flags (listener's tail kept), and the TGString and temporaries' frame slots
bool UtopiaApp::Initialize() {
	char password[256];
	char mission[256];
	char name[256];
	char address[256];
	char missionScript[1024];

	if ( !NiApplication::Initialize() ) {
		return false;
	}
	SetCapture( appWindow->GetWindow() );

	NiCommand *commandLine = command;
	int hostMode = HOST_MODE_NONE;
	if ( commandLine != NULL ) {
		if ( commandLine->Boolean( "Client" ) && commandLine->String( "IP", address ) && commandLine->String( "Name", name ) ) {
			password[0] = '\0';
			commandLine->String( "Password", password );
			directStart = true;
		}
		if ( commandLine->Boolean( "Listener" ) ) {
			hostMode = HOST_MODE_LISTENER;
		} else if ( commandLine->Boolean( "Dedicated" ) ) {
			hostMode = HOST_MODE_DEDICATED;
		}
		if ( hostMode != HOST_MODE_NONE && commandLine->String( "Mission", mission ) ) {
				password[0] = '\0';
				commandLine->String( "Password", password );
				directStart = true;
		}
		if ( commandLine->Boolean( "SkipChecksum" ) ) {
			skipChecksum = true;
		}
		if ( commandLine->Boolean( "TestMode" ) ) {
			testMenuState = 2;
		}
		if ( commandLine->Boolean( "TestMenu" ) ) {
			testMenuState = 0;
		}
		if ( commandLine->Boolean( "TestGurus" ) ) {
			testMenuState = 3;
		}
	}

	while ( ShowCursor( FALSE ) >= 0 ) {
	}

	Unknown71E790( GetFullscreen() );

	GetRenderer()->SetSorter( new NiAlphaAccumulator );

	TGModule::CallVirtualSlot00();
	TGModule::CallVirtualSlot01();

	PyEmbed::unknown99ADD6 = !GetFullscreen();

	// drop the keys pressed while loading
	MSG message;
	while ( PeekMessage( &message, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE ) ) {
	}

	MeasureTime();
	g_kRealtimeTimerManager.RebaseTime( GetCurrentTime() );

	if ( directStart ) {
		TGString value;
		// the player name is cut to 32 characters
		name[32] = '\0';
		if ( hostMode == HOST_MODE_NONE ) {
			g_kConfigMapping.SetIntValue( "Multiplayer Options", "Internet Client", 1 );
			g_kConfigMapping.SetIntValue( "Multiplayer Options", "Internet Host", 0 );
			value.SetString( address );
			g_kConfigMapping.SetTGStringValue( "Multiplayer Options", "Direct Join Address", &value );
			value.SetString( name );
			g_kConfigMapping.SetTGStringValue( "Multiplayer Options", "Player Name", &value );
		} else {
			if ( hostMode == HOST_MODE_LISTENER ) {
				g_kConfigMapping.SetIntValue( "Multiplayer Options", "Internet Client", 0 );
				g_kConfigMapping.SetIntValue( "Multiplayer Options", "Internet Host", 1 );
				g_kConfigMapping.SetIntValue( "Multiplayer Options", "Dedicated Server", 0 );
			} else {
				g_kConfigMapping.SetIntValue( "Multiplayer Options", "Internet Client", 0 );
				g_kConfigMapping.SetIntValue( "Multiplayer Options", "Internet Host", 1 );
				g_kConfigMapping.SetIntValue( "Multiplayer Options", "Dedicated Server", 1 );
			}
			sprintf( missionScript, "Multiplayer.Episode.%s.%s", mission, mission );
			g_kVarManager.SetStringVariable( "Multiplayer", "Mission", missionScript );
		}
		value.SetString( password );
		g_kConfigMapping.SetTGStringValue( "Multiplayer Options", "Password", &value );

		MultiplayerWindow *window = (MultiplayerWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
		if ( window != NULL && hostMode == HOST_MODE_NONE ) {
			window->SetDirectJoin( true );
		}
		PyEmbed::CallFunction( "MainMenu.mainmenu", "BuildInterface", "", NULL, "()" );
		PyEmbed::CallFunction( "MainMenu.mainmenu", "SwitchMiddlePane", "", NULL, "(s)", "Multiplayer" );

		if ( hostMode == HOST_MODE_NONE ) {
			g_kUtopiaModule.SetMultiplayer( true );
			g_kUtopiaModule.SetIsClient( true );
			g_kUtopiaModule.SetIsHost( false );
		} else {
			g_kUtopiaModule.SetMultiplayer( true );
			g_kUtopiaModule.SetIsClient( true );
			if ( hostMode != HOST_MODE_LISTENER ) {
				g_kUtopiaModule.SetIsClient( false );
			}
			g_kUtopiaModule.SetIsHost( true );
		}

		if ( window != NULL ) {
			TGEvent *event = new TGEvent;
			event->SetEventType( ET_START );
			event->SetDestination( window );
			g_kEventManager.AddEvent( event );
		}
	}

	g_kSystemWrapper.SetDefaultScreenShotPath( "Screen Shots" );
	return true;
}

/*
================
UtopiaApp::Terminate
================
*/
// FUNCTION: STBC 0x00438e00
void UtopiaApp::Terminate() {
	TGModule::CallVirtualSlots03And04();
	TGModule::CallVirtualSlot05();
	NiApplication::Terminate();
}

/*
================
UtopiaApp::OnIdle

While GameSpy runs, an idle turn comes at most every 50 milliseconds.
================
*/
// FUNCTION: STBC 0x00438e20
void UtopiaApp::OnIdle() {
	GameSpy *gameSpy = g_kUtopiaModule.GetGameSpy();
	if ( gameSpy != NULL && !gameSpy->GetUnknownEC() ) {
		DWORD now = timeGetTime();
		if ( now - lastIdleTime < GAMESPY_IDLE_PERIOD ) {
			return;
		}
		lastIdleTime = now;
	}
	TGApplication::OnIdle();
	TGModule::CallVirtualSlot02();
}

/*
================
UtopiaApp::Pause

The sounds pause with the game.
================
*/
// FUNCTION: STBC 0x00438e60
void UtopiaApp::Pause( bool pause ) {
	TGApplication::Pause( pause );
	if ( pause ) {
		g_kSoundManager.PauseAllSounds();
	} else {
		g_kSoundManager.UnpauseAllSounds();
	}
}

/*
================
UtopiaApp::OnUnpause

The first frame after the pause advances no game time.
================
*/
// FUNCTION: STBC 0x00438e90
void UtopiaApp::OnUnpause() {
	gameFrameTime = 0.0f;
}

/*
================
UtopiaApp::OnWindowResize

Takes the graphics menu's chosen width (or the window's), snapped to a
supported mode, and the current mode's window state and depth.
================
*/
// FUNCTION: STBC 0x00438ea0
bool UtopiaApp::OnWindowResize( int width_, int height_, unsigned int sizeType, HWND window ) {
	bool result = TGApplication::OnWindowResize( width_, height_, sizeType, window );
	if ( result == true ) {
		if ( graphicsMenuInfo != NULL && graphicsMenuInfo->graphicsMenu != NULL ) {
			width = graphicsMenuInfo->graphicsMenu->GetChosenModeWidth();
		} else {
			width = width_;
		}
		SnapToMode();
		if ( GraphicsModeInfo::GetCurrentMode() != NULL ) {
			fullscreen = !GraphicsModeInfo::GetCurrentMode()->IsWindowed();
			bitDepth = GraphicsModeInfo::GetCurrentMode()->GetColorDepth();
		}
	}
	return result;
}

/*
================
UtopiaApp::OnDefault

The application's activation (WM_ACTIVATEAPP): the window takes the
mouse and the movies play while it is active.
================
*/
// FUNCTION: STBC 0x00438fb0
bool UtopiaApp::OnDefault( NiEventRecord *event ) {
	if ( event->uiMsg != WM_ACTIVATEAPP ) {
		return false;
	}
	if ( event->wParam ) {
		SetCapture( event->hWnd );
		g_kMovieManager.SetPlayMovies( true );
		return true;
	}
	ReleaseCapture();
	g_kMovieManager.SetPlayMovies( false );
	return true;
}
