// NiApplication.cpp -- the application framework

#include "NiRTLib.h"
#include "NiApplication.h"
#include "NiAppWindow.h"
#include "NiCommand.h"
#include "NiSystem.h"
#include "NiMessageLoop.h"
#include "NiKeyboard.h"
#include "NiMouse.h"
#include "NiGamePad.h"
#include "NiFrameRate.h"
#include <commctrl.h>
#include "NiRendererDialog.h"
#include "renderer/NiD3DRender.h"
#include "renderer/NiImage.h"
#include <stdio.h>
#include <string.h>
#include "NiMain.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007b7ff0 _$E420
// FUNCTION: STBC 0x007b7f20 _$E17
// FUNCTION: STBC 0x007b7f40 _$E15
// FUNCTION: STBC 0x007b7f50 _$E23
// FUNCTION: STBC 0x007b7f70 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b7f80 _$E34
// FUNCTION: STBC 0x007b7f90 _$E190
// FUNCTION: STBC 0x007b7fa0 _$E45
// FUNCTION: STBC 0x007b7fb0 _$E49
// FUNCTION: STBC 0x007b7fc0 _$E53
// FUNCTION: STBC 0x007b7fd0 _$E57
// FUNCTION: STBC 0x007b7fe0 _$E177
// FUNCTION: STBC 0x007b8010 _$E119
// FUNCTION: STBC 0x007b8020 _$E132
// FUNCTION: STBC 0x007b8030 _$E182
// FUNCTION: STBC 0x007b8040 _$E195
// FUNCTION: STBC 0x007b8050 _$E186
// FUNCTION: STBC 0x007b8060 _$E199
// FUNCTION: STBC 0x007b8070 _$E206
// FUNCTION: STBC 0x007b80a0 _$E81
// FUNCTION: STBC 0x007b8110 _$E235
// FUNCTION: STBC 0x007b81b0 _$E86
// FUNCTION: STBC 0x007b81d0 _$E91
// FUNCTION: STBC 0x007b8220 _$E96
// FUNCTION: STBC 0x007b8260 _$E102
// FUNCTION: STBC 0x007b8270 _$E107
// FUNCTION: STBC 0x007b8280 _$E137
// FUNCTION: STBC 0x007b82a0 _$E142
// FUNCTION: STBC 0x007b82d0 _$E147
// FUNCTION: STBC 0x007b82f0 _$E112
// FUNCTION: STBC 0x007b8320 _$E152
// FUNCTION: STBC 0x007b8330 _$E157
// FUNCTION: STBC 0x007b83c0 _$E162
// FUNCTION: STBC 0x007b83e0 _$E167
// FUNCTION: STBC 0x007b83f0 _$E172
// FUNCTION: STBC 0x007b8080 _$E262
// FUNCTION: STBC 0x007b8090 _$E266
// FUNCTION: STBC 0x007b80b0 _$E248
// FUNCTION: STBC 0x007b80c0 _$E255
// FUNCTION: STBC 0x007b80d0 _$E274
// FUNCTION: STBC 0x007b80e0 _$E278
// FUNCTION: STBC 0x007b80f0 _$E318
// FUNCTION: STBC 0x007b8100 _$E324
// FUNCTION: STBC 0x007b8120 _$E270
// FUNCTION: STBC 0x007b8130 _$E282
// FUNCTION: STBC 0x007b8140 _$E286
// FUNCTION: STBC 0x007b8150 _$E290
// FUNCTION: STBC 0x007b8160 _$E294
// FUNCTION: STBC 0x007b8170 _$E298
// FUNCTION: STBC 0x007b8180 _$E302
// FUNCTION: STBC 0x007b8190 _$E306
// FUNCTION: STBC 0x007b81a0 _$E310
// FUNCTION: STBC 0x007b81c0 _$E314
// FUNCTION: STBC 0x007b81e0 _$E328
// FUNCTION: STBC 0x007b81f0 _$E332
// FUNCTION: STBC 0x007b8200 _$E336
// FUNCTION: STBC 0x007b8210 _$E340
// FUNCTION: STBC 0x007b8230 _$E344
// FUNCTION: STBC 0x007b8240 _$E348
// FUNCTION: STBC 0x007b8250 _$E352
// FUNCTION: STBC 0x007b8290 _$E356
// FUNCTION: STBC 0x007b82b0 _$E360
// FUNCTION: STBC 0x007b82c0 _$E364
// FUNCTION: STBC 0x007b82e0 _$E368
// FUNCTION: STBC 0x007b8300 _$E372
// FUNCTION: STBC 0x007b8310 _$E376
// FUNCTION: STBC 0x007b8340 _$E380
// FUNCTION: STBC 0x007b8350 _$E384
// FUNCTION: STBC 0x007b8360 _$E388
// FUNCTION: STBC 0x007b8370 _$E392
// FUNCTION: STBC 0x007b8380 _$E396
// FUNCTION: STBC 0x007b8390 _$E400
// FUNCTION: STBC 0x007b83a0 _$E404
// FUNCTION: STBC 0x007b83b0 _$E408
// FUNCTION: STBC 0x007b83d0 _$E412

// DATA: STBC 0x9A09CC
bool NiApplication::unknown9A09CC;

// DATA: STBC 0x9A09D0
NiApplication *NiApplication::application;

// DATA: STBC 0x9A09D4 ?instance@NiApplication@@2PAUHINSTANCE__@@A
HINSTANCE NiApplication::instance;

// DATA: STBC 0x9A09D8
HACCEL NiApplication::accelerator;

// DATA: STBC 0x9A09DC
NiCommand *NiApplication::command;

/*
================
NiApplication::NiApplication

Creates the main window (a caption, a system menu and a minimize box), the
message loop, the keyboard and the mouse, and becomes the one instance. A
bit depth asks for the full screen.

Quirk: the Windows version is queried and never looked at.
================
*/
// FUNCTION: STBC 0x7B7180
NiApplication::NiApplication( const char *caption, unsigned int width, unsigned int height,
							  unsigned int menuID, unsigned int numStatusPanes, unsigned int bitDepth ) {
	appWindow = new NiAppWindow( caption, width, height, numStatusPanes, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX );
	messageLoop = new NiMessageLoop;
	this->menuID = menuID;
	this->bitDepth = bitDepth;

	keyboard = 0;
	mouse = 0;
	gamePad = 0;
	keyboard = new NiKeyboard;
	mouse = new NiMouse;

	lastTime = -1.0f;
	accumTime = 0.0f;
	currentTime = 0.0f;
	frameTime = 0.0f;
	clicks = 0;
	timer = 1;
	maxTimer = 30;
	frameRate = 0.0f;
	lastFrameRateTime = 0.0f;
	lastFrameRateClicks = 0;
	minFramePeriod = 0.01f;
	lastFrame = 0.0f;

	if ( this->bitDepth == 0 ) {
		fullscreen = false;
	} else {
		fullscreen = true;
	}
	unknown2D = true;
	unknown2E = false;
	unknown2F = false;
	unknown30 = false;
	unknown31 = true;
	backgroundColor = NiColor( 0.5f, 0.5f, 0.5f );
	frameRateDisplay = 0;
	frameRateEnabled = false;

	OSVERSIONINFO versionInfo;
	versionInfo.dwOSVersionInfoSize = sizeof( versionInfo );
	GetVersionEx( &versionInfo );

	rendererType = RENDERER_DIRECT3D;
	application = this;
}

/*
================
NiApplication::~NiApplication
================
*/
// FUNCTION: STBC 0x7B7390
// FUNCTION: STBC 0x7B7370 ??_GNiApplication@@UAEPAXI@Z
NiApplication::~NiApplication() {
	delete command;
	delete keyboard;
	delete mouse;
	delete gamePad;
	delete appWindow;
	delete messageLoop;
	application = 0;
	command = 0;
}

/*
================
NiApplication::SetCommandLine

Parses a non-empty command line for the option queries.
================
*/
// FUNCTION: STBC 0x7B74D0
void NiApplication::SetCommandLine( const char *commandLine ) {
	if ( commandLine && strlen( commandLine ) ) {
		command = new NiCommand( commandLine );
	}
}

/*
================
NiApplication::CommandSelectRenderer

"-full" asks for the full screen; "-direct3d", "-opengl" and "-software"
select the renderer, the first one given in that order winning.
================
*/
// FUNCTION: STBC 0x7B7540
void NiApplication::CommandSelectRenderer() {
	if ( !command ) {
		return;
	}
	if ( command->Boolean( "full" ) ) {
		application->fullscreen = true;
	}
	if ( command->Boolean( "direct3d" ) ) {
		application->rendererType = RENDERER_DIRECT3D;
	} else if ( command->Boolean( "opengl" ) ) {
		application->rendererType = RENDERER_OPENGL;
	} else if ( command->Boolean( "software" ) ) {
		application->rendererType = RENDERER_SOFTWARE;
	}
}

/*
================
NiApplication::CreateScene

The scene starts as an empty node.
================
*/
// FUNCTION: STBC 0x7B75D0
bool NiApplication::CreateScene() {
	scene = new NiNode;
	return true;
}

/*
================
NiApplication::CreateCamera

A camera with a 4:3 frustum a quarter unit wide at the near distance 1,
reaching to 1000, over the whole screen.
================
*/
// FUNCTION: STBC 0x7B7660
bool NiApplication::CreateCamera() {
	camera = new NiCamera;
	NiFrustum frustum( -0.25f, 0.25f, 0.1875f, -0.1875f, 1.0f, 1000.0f );
	NiRect<float> port( 0.0f, 1.0f, 1.0f, 0.0f );
	camera->SetViewFrustum( frustum );
	camera->SetViewPort( port );
	return true;
}

/*
================
NiApplication::CreateRenderer

Creates the renderer the command line selects: Direct3D through the
display-settings dialog, or directly on the device and in the mode the
options give. The other renderers are not built in this binary. A failure
is reported in a message box.
================
*/
// FUNCTION: STBC 0x7B77B0
// PARKED: inline budget and tail merge: retail expands all four renderer = 0 releases and shares one tail among them; this build calls NiObject::DecRefCount (0x40CFE0) out of line in the last (the invalid-type branch), so the three before keep a tail each
bool NiApplication::CreateRenderer() {
	HWND window = fullscreen ? appWindow->GetWindow() : appWindow->GetRenderWindow();
	char errorText[512];

	NiImage::defaultMipmap = true;
	if ( rendererType == RENDERER_DIRECT3D ) {
		unknown9A09CC = true;
		if ( unknown31 && bitDepth == 0 ) {
			NiPointer<NiD3DRender> selected = NiSelectRenderer( appWindow->GetWindow(), appWindow->GetRenderWindow(), unknown2E, fullscreen );
			renderer = selected;
			if ( !renderer ) {
				return false;
			}
		} else {
			unsigned int flags = 1;
			if ( unknown2E ) {
				flags = 5;
			}
			Ni3DDeviceDesc *device;
			if ( unknown2D ) {
				device = fullscreen ? NiD3DRender::GetFullscreenDevice() : NiD3DRender::GetWindowedDevice();
			} else {
				device = NiD3DRender::GetSoftwareDevice();
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
				renderer = NiD3DRender::Create( 1, appWindow->GetWidth(), appWindow->GetHeight(), flags, window, 0, device );
			} else if ( fullscreen ) {
				renderer = NiD3DRender::Create( 1, 640, 480, flags, window, 0, device );
			} else {
				renderer = NiD3DRender::Create( 0, 0, 0, flags, window, 0, device );
			}
			if ( renderer == NULL ) {
				sprintf( errorText, "D3D Render Creation Error\nError string from renderer was:\n\"%s\"\n", NiD3DRender::errorString );
			}
		}
		if ( renderer ) {
			if ( renderer->VirtualSlot13() & 8 ) {
				unknown2D = true;
			} else {
				unknown2D = false;
			}
		}
	} else if ( rendererType == RENDERER_OPENGL ) {
		strcpy( errorText, "_USE_OPENGL must be defined to use an OpenGL renderer" );
		renderer = 0;
	} else if ( rendererType == RENDERER_SOFTWARE ) {
		strcpy( errorText, "_USE_SOFTWARE must be defined to use a software renderer" );
		renderer = 0;
	} else if ( rendererType == RENDERER_PSX2 ) {
		strcpy( errorText, "_USE_PSX2 must be defined to use a PSX2 renderer" );
		renderer = 0;
	} else {
		sprintf( errorText, "Invalid renderer m_eType: %#x", rendererType );
		renderer = 0;
	}

	if ( !renderer ) {
		NiMessageBox( errorText, "Renderer Creation Failure" );
		return false;
	}
	NiImage::AddRenderer( renderer );
	renderer->SetBackgroundColor( backgroundColor );
	return true;
}

/*
================
NiApplication::MeasureTime

Advances the clock: the time since the last measurement and the time since
the first one.
================
*/
// FUNCTION: STBC 0x7B7B90
void NiApplication::MeasureTime() {
	if ( lastTime == -1.0f ) {
		lastTime = NiGetCurrentTimeInSec();
		accumTime = 0.0f;
		clicks = 0;
	}
	currentTime = NiGetCurrentTimeInSec();
	frameTime = currentTime - lastTime;
	lastTime = currentTime;
	accumTime += frameTime;
}

/*
================
NiApplication::DrawFrameRate

Every maxTimer frames, shows the frame rate since the last update in the
first status pane.
================
*/
// FUNCTION: STBC 0x7B7BD0
void NiApplication::DrawFrameRate() {
	if ( --timer == 0 ) {
		if ( accumTime > 0.0f ) {
			if ( accumTime > lastFrameRateTime ) {
				frameRate = ( clicks - lastFrameRateClicks ) / ( accumTime - lastFrameRateTime );
			}
		} else {
			frameRate = 0.0f;
		}
		char message[32];
		sprintf( message, "fps: %.1f", frameRate );
		appWindow->SetStatusPaneText( 0, message );
		timer = maxTimer;
		lastFrameRateTime = accumTime;
		lastFrameRateClicks = clicks;
	}
}

/*
================
NiApplication::Initialize

Creates the camera, the renderer and the scene, hands the camera the other
two, brings the scene up to date and sets up the frame-rate overlay, shown
in red.
================
*/
// FUNCTION: STBC 0x7B7C70
bool NiApplication::Initialize() {
	if ( !CreateCamera() ) {
		return false;
	}
	if ( !CreateRenderer() ) {
		return false;
	}
	if ( !CreateScene() ) {
		return false;
	}

	camera->SetRenderer( renderer );
	camera->SetScene( scene );
	scene->Update( 0.0f );
	scene->UpdateProperties();
	scene->UpdateEffects();
	camera->Update( 0.0f );

	frameRateDisplay = new NiFrameRate;
	if ( frameRateDisplay ) {
		frameRateDisplay->SetColor( NiColorA( 1.0f, 0.0f, 0.0f, 1.0f ) );
		frameRateDisplay->Init( true, 0 );
		if ( frameRateEnabled ) {
			frameRateDisplay->AddToCamera( camera );
		}
	}
	return true;
}

/*
================
NiApplication::Terminate

Releases the frame-rate overlay, the camera's scene and renderer, and the
scene, the camera and the renderer.

Quirk: the deleted overlay stays in frameRateDisplay.
================
*/
// FUNCTION: STBC 0x7B7DD0
// PARKED: inline-state wall: retail releases the camera's scene through the out-of-line NiObject::DecRefCount (0x40CFE0); this unit inlines it
void NiApplication::Terminate() {
	if ( frameRateDisplay ) {
		frameRateDisplay->Shutdown();
		delete frameRateDisplay;
	}
	if ( camera ) {
		camera->SetScene( 0 );
		camera->SetRenderer( 0 );
	}
	scene = 0;
	camera = 0;
	renderer = 0;
}

/*
================
NiApplication::CreateMenuBar
================
*/
// FUNCTION: STBC 0x7B7EA0
void NiApplication::CreateMenuBar() {
}

/*
================
NiApplication::EnableFrameRate

Attaches the overlay to the camera, or detaches it, when the setting
changes.
================
*/
// FUNCTION: STBC 0x7B7EB0
void NiApplication::EnableFrameRate( bool enable ) {
	if ( frameRateDisplay ) {
		if ( frameRateEnabled != enable ) {
			if ( enable ) {
				frameRateDisplay->AddToCamera( camera );
			} else {
				frameRateDisplay->RemoveFromCamera( camera );
			}
		}
	}
	frameRateEnabled = enable;
}
