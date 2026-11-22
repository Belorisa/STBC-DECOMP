/*
===============================================================================

	GraphicsModeInfo.cpp

	The graphics mode in use and the LCARS icon groups drawn at each resolution.

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "GraphicsModeInfo.h"
#include "PyEmbed.h"
#include "UtopiaApp.h"
#include "NiApplication.h"
#include "renderer/NiD3DRender.h"
#include "renderer/NiD3DDirectDraw.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00462d40 _$E466
// FUNCTION: STBC 0x00462d60 _$E465

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00462a10 _$E32
// FUNCTION: STBC 0x00462a20 _$E29
// FUNCTION: STBC 0x00462a30 _$E31
// FUNCTION: STBC 0x00462a40 _$E30
// FUNCTION: STBC 0x00462a50 _$E38
// FUNCTION: STBC 0x00462a60 _$E35
// FUNCTION: STBC 0x00462a70 _$E37
// FUNCTION: STBC 0x00462a80 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00462c00 _$E72
// FUNCTION: STBC 0x00462c10 _$E71
// FUNCTION: STBC 0x00462c20 _$E228
// FUNCTION: STBC 0x00462c30 _$E227
// FUNCTION: STBC 0x00462d00 _$E157
// FUNCTION: STBC 0x00462d10 _$E156
// FUNCTION: STBC 0x00462d20 _$E170
// FUNCTION: STBC 0x00462d30 _$E169
// FUNCTION: STBC 0x00462d70 _$E83
// FUNCTION: STBC 0x00462d80 _$E82
// FUNCTION: STBC 0x00462d90 _$E87
// FUNCTION: STBC 0x00462da0 _$E86
// FUNCTION: STBC 0x00462db0 _$E91
// FUNCTION: STBC 0x00462dc0 _$E90
// FUNCTION: STBC 0x00462dd0 _$E95
// FUNCTION: STBC 0x00462de0 _$E94
// FUNCTION: STBC 0x00462df0 _$E215
// FUNCTION: STBC 0x00462e00 _$E214
// FUNCTION: STBC 0x00462e10 _$E220
// FUNCTION: STBC 0x00462e20 _$E219
// FUNCTION: STBC 0x00462e30 _$E233
// FUNCTION: STBC 0x00462e40 _$E232
// FUNCTION: STBC 0x00462e50 _$E224
// FUNCTION: STBC 0x00462e60 _$E223
// FUNCTION: STBC 0x00462e70 _$E237
// FUNCTION: STBC 0x00462e80 _$E236
// FUNCTION: STBC 0x00462e90 _$E244
// FUNCTION: STBC 0x00462ea0 _$E243
// FUNCTION: STBC 0x00462eb0 _$E295
// FUNCTION: STBC 0x00462ec0 _$E294
// FUNCTION: STBC 0x00462ed0 _$E299
// FUNCTION: STBC 0x00462ee0 _$E298
// FUNCTION: STBC 0x00462ef0 _$E119
// FUNCTION: STBC 0x00462f00 _$E118
// FUNCTION: STBC 0x00462f10 _$E281
// FUNCTION: STBC 0x00462f20 _$E280
// FUNCTION: STBC 0x00462f30 _$E288
// FUNCTION: STBC 0x00462f40 _$E287
// FUNCTION: STBC 0x00462f50 _$E307
// FUNCTION: STBC 0x00462f60 _$E306
// FUNCTION: STBC 0x00462f70 _$E311
// FUNCTION: STBC 0x00462f80 _$E310
// FUNCTION: STBC 0x00462f90 _$E351
// FUNCTION: STBC 0x00462fa0 _$E350
// FUNCTION: STBC 0x00462fb0 _$E357
// FUNCTION: STBC 0x00462fc0 _$E356
// FUNCTION: STBC 0x00462fd0 _$E268
// FUNCTION: STBC 0x00462fe0 _$E267
// FUNCTION: STBC 0x00462ff0 _$E303
// FUNCTION: STBC 0x00463000 _$E302
// FUNCTION: STBC 0x00463010 _$E315
// FUNCTION: STBC 0x00463020 _$E314
// FUNCTION: STBC 0x00463030 _$E319
// FUNCTION: STBC 0x00463040 _$E318
// FUNCTION: STBC 0x00463050 _$E323
// FUNCTION: STBC 0x00463060 _$E322
// FUNCTION: STBC 0x00463070 _$E327
// FUNCTION: STBC 0x00463080 _$E326
// FUNCTION: STBC 0x00463090 _$E331
// FUNCTION: STBC 0x004630a0 _$E330
// FUNCTION: STBC 0x004630b0 _$E335
// FUNCTION: STBC 0x004630c0 _$E334
// FUNCTION: STBC 0x004630d0 _$E339
// FUNCTION: STBC 0x004630e0 _$E338
// FUNCTION: STBC 0x004630f0 _$E343
// FUNCTION: STBC 0x00463100 _$E342
// FUNCTION: STBC 0x00463110 _$E124
// FUNCTION: STBC 0x00463120 _$E123
// FUNCTION: STBC 0x00463130 _$E347
// FUNCTION: STBC 0x00463140 _$E346
// FUNCTION: STBC 0x00463150 _$E129
// FUNCTION: STBC 0x00463160 _$E128
// FUNCTION: STBC 0x00463170 _$E361
// FUNCTION: STBC 0x00463180 _$E360
// FUNCTION: STBC 0x00463190 _$E365
// FUNCTION: STBC 0x004631a0 _$E364
// FUNCTION: STBC 0x004631b0 _$E369
// FUNCTION: STBC 0x004631c0 _$E368
// FUNCTION: STBC 0x004631d0 _$E373
// FUNCTION: STBC 0x004631e0 _$E372
// FUNCTION: STBC 0x004631f0 _$E134
// FUNCTION: STBC 0x00463200 _$E133
// FUNCTION: STBC 0x00463210 _$E377
// FUNCTION: STBC 0x00463220 _$E376
// FUNCTION: STBC 0x00463230 _$E381
// FUNCTION: STBC 0x00463240 _$E380
// FUNCTION: STBC 0x00463250 _$E385
// FUNCTION: STBC 0x00463260 _$E384
// FUNCTION: STBC 0x00463270 _$E140
// FUNCTION: STBC 0x00463280 _$E139
// FUNCTION: STBC 0x00463290 _$E145
// FUNCTION: STBC 0x004632a0 _$E144
// FUNCTION: STBC 0x004632b0 _$E175
// FUNCTION: STBC 0x004632c0 _$E174
// FUNCTION: STBC 0x004632d0 _$E389
// FUNCTION: STBC 0x004632e0 _$E388
// FUNCTION: STBC 0x004632f0 _$E180
// FUNCTION: STBC 0x00463300 _$E179
// FUNCTION: STBC 0x00463310 _$E393
// FUNCTION: STBC 0x00463320 _$E392
// FUNCTION: STBC 0x00463330 _$E397
// FUNCTION: STBC 0x00463340 _$E396
// FUNCTION: STBC 0x00463350 _$E185
// FUNCTION: STBC 0x00463360 _$E184
// FUNCTION: STBC 0x00463370 _$E401
// FUNCTION: STBC 0x00463380 _$E400
// FUNCTION: STBC 0x00463390 _$E150
// FUNCTION: STBC 0x004633a0 _$E149
// FUNCTION: STBC 0x004633b0 _$E405
// FUNCTION: STBC 0x004633c0 _$E404
// FUNCTION: STBC 0x004633d0 _$E409
// FUNCTION: STBC 0x004633e0 _$E408
// FUNCTION: STBC 0x004633f0 _$E190
// FUNCTION: STBC 0x00463400 _$E189
// FUNCTION: STBC 0x00463410 _$E195
// FUNCTION: STBC 0x00463420 _$E194
// FUNCTION: STBC 0x00463430 _$E413
// FUNCTION: STBC 0x00463440 _$E412
// FUNCTION: STBC 0x00463450 _$E417
// FUNCTION: STBC 0x00463460 _$E416
// FUNCTION: STBC 0x00463470 _$E421
// FUNCTION: STBC 0x00463480 _$E420
// FUNCTION: STBC 0x00463490 _$E425
// FUNCTION: STBC 0x004634a0 _$E424
// FUNCTION: STBC 0x004634b0 _$E429
// FUNCTION: STBC 0x004634c0 _$E428
// FUNCTION: STBC 0x004634d0 _$E433
// FUNCTION: STBC 0x004634e0 _$E432
// FUNCTION: STBC 0x004634f0 _$E437
// FUNCTION: STBC 0x00463500 _$E436
// FUNCTION: STBC 0x00463510 _$E441
// FUNCTION: STBC 0x00463520 _$E440
// FUNCTION: STBC 0x00463530 _$E200
// FUNCTION: STBC 0x00463540 _$E199
// FUNCTION: STBC 0x00463550 _$E445
// FUNCTION: STBC 0x00463560 _$E444
// FUNCTION: STBC 0x00463570 _$E205
// FUNCTION: STBC 0x00463580 _$E204
// FUNCTION: STBC 0x00463590 _$E210
// FUNCTION: STBC 0x004635a0 _$E209

// DATA: STBC 0x0098131c
GraphicsModeInfo *GraphicsModeInfo::currentMode;

// DATA: STBC 0x008db068
char *GraphicsModeInfo::lcarsStrings[] = {
	"LCARS_640",
	"LCARS_800",
	"LCARS_1024",
	"LCARS_1280",
	"LCARS_1600"
};

// DATA: STBC 0x008db07c
char *GraphicsModeInfo::lcarsModules[] = {
	"Icons.LCARS_640",
	"Icons.LCARS_800",
	"Icons.LCARS_1024",
	"Icons.LCARS_1280",
	"Icons.LCARS_1600"
};

/*
================
GraphicsModeInfo::GraphicsModeInfo

No device, full screen at 640 by 480 and 16 bits.
================
*/
// FUNCTION: STBC 0x004621e0
GraphicsModeInfo::GraphicsModeInfo( void ) {
	device = NULL;
	desc = NULL;
	windowed = false;
	width = 640;
	height = 480;
	colorDepth = 16;
	highZBuffer = false;
}

/*
================
GraphicsModeInfo::~GraphicsModeInfo

Frees the device name and deletes the device description.
================
*/
// FUNCTION: STBC 0x00462210
GraphicsModeInfo::~GraphicsModeInfo( void ) {
	if ( device != NULL ) {
		delete [] device;
		device = NULL;
	}
	if ( desc != NULL ) {
		delete desc;
		desc = NULL;
	}
}

/*
================
GraphicsModeInfo::operator=

Copies the device name, the window flag, the screen size, the colour depth
and the Z buffer depth.
================
*/
// FUNCTION: STBC 0x00462240 ??4GraphicsModeInfo@@QAEAAV0@ABV0@@Z
GraphicsModeInfo &GraphicsModeInfo::operator=( const GraphicsModeInfo &mode ) {
	// MATCH-PENDING: R17, exact only through the invented setters SetWindowed/SetWidth/SetHeight/SetColorDepth/SetHighZBuffer (GraphicsModeInfo.h); no evidence shows them (user's ruling, 2026-11-18)
	SetDevice( mode.GetDevice() );
	// BUG: the local hides the member: the copied description is never
	// stored and leaks, and the mode keeps its old description
	Ni3DDeviceDesc *desc = new Ni3DDeviceDesc;
	*desc = *mode.GetDesc();
	SetWindowed( mode.IsWindowed() );
	SetWidth( mode.GetWidth() );
	SetHeight( mode.GetHeight() );
	SetColorDepth( mode.GetColorDepth() );
	SetHighZBuffer( mode.IsHighZBuffer() );
	return *this;
}

/*
================
GraphicsModeInfo::GetCurrentResolution

The index of the screen width among the LCARS resolutions: 640, 800, 1024,
1280 and 1600 pixels wide are 0 to 4; any other width counts as 640.
================
*/
// FUNCTION: STBC 0x004624a0
int GraphicsModeInfo::GetCurrentResolution( void ) {
	switch ( width ) {
		case 640:
			return 0;
		case 800:
			return 1;
		case 1024:
			return 2;
		case 1280:
			return 3;
		case 1600:
			return 4;
		default:
			return 0;
	}
}

/*
================
GraphicsModeInfo::UpdateCurrentMode

Makes the current mode anew: the renderer's device, colour depth, window
and screen size, or 640 by 480 in 16 bits full screen without a renderer.
Then loads the LCARS icon group of the resolution.
================
*/
// FUNCTION: STBC 0x004624f0
void GraphicsModeInfo::UpdateCurrentMode( void ) {
	// MATCH-PENDING: R17, exact only through the invented setters SetWindowed/SetWidth/SetHeight/SetColorDepth/SetHighZBuffer (GraphicsModeInfo.h); no evidence shows them (user's ruling, 2026-11-18)
	if ( currentMode != NULL ) {
		delete currentMode;
	}
	currentMode = new GraphicsModeInfo;
	currentMode->SetDesc( new Ni3DDeviceDesc );

	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	if ( renderer != NULL ) {
		*currentMode->desc = *renderer->Unknown7C2EE0();
		if ( renderer->VirtualSlot13() & 0x100 ) {
			currentMode->SetColorDepth( 32 );
		} else {
			currentMode->SetColorDepth( 16 );
		}
		if ( NiApplication::application->GetFullscreen() ) {
			currentMode->SetWindowed( false );
		} else {
			currentMode->SetWindowed( true );
		}
		if ( renderer->VirtualSlot13() & 0x100 ) {
			currentMode->SetHighZBuffer( true );
		} else {
			currentMode->SetHighZBuffer( false );
		}
		currentMode->SetWidth( renderer->VirtualSlot19() );
		currentMode->SetHeight( renderer->VirtualSlot20() );
	} else {
		currentMode->SetColorDepth( 16 );
		currentMode->SetWindowed( false );
		currentMode->SetHighZBuffer( false );
		currentMode->SetWidth( 640 );
		currentMode->SetHeight( 480 );
	}
	PyEmbed::GetModule( currentMode->GetLcarsString(), false );
}

/*
================
GraphicsModeInfo::DeleteCurrentMode

Frees the current mode at shutdown; UtopiaModule calls it.
================
*/
// FUNCTION: STBC 0x00462870
void GraphicsModeInfo::DeleteCurrentMode( void ) {
	if ( currentMode != NULL ) {
		delete currentMode;
	}
}

/*
================
GraphicsModeInfo::SetDevice

Frees the device name and stores a copy of the new one; NULL only frees it.
================
*/
// FUNCTION: STBC 0x00462890
void GraphicsModeInfo::SetDevice( char *newDevice ) {
	if ( device != NULL ) {
		delete [] device;
		device = NULL;
	}
	if ( newDevice != NULL ) {
		device = new char[strlen( newDevice ) + 1];
		strcpy( device, newDevice );
	}
}

/*
================
GraphicsModeInfo::SetDesc

Deletes the device description and stores the new one; NULL only deletes it.
================
*/
// FUNCTION: STBC 0x004628f0
void GraphicsModeInfo::SetDesc( Ni3DDeviceDesc *newDesc ) {
	if ( desc != NULL ) {
		delete desc;
		desc = NULL;
	}
	if ( newDesc != NULL ) {
		desc = newDesc;
	}
}
