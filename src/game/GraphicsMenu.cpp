/*
===============================================================================

	GraphicsMenu.cpp

===============================================================================
*/

#include "GraphicsMenu.h"
#include "Game.h"
#include "InterfaceModule.h"
#include "TGIcon.h"
#include "TGIconManager.h"
#include "TGRootPane.h"
#include "UtopiaModule.h"
#include "PyEmbed.h"
#include "STButton.h"
#include "STMenu.h"
#include "STSubPane.h"
#include "STToggle.h"
#include "ModalDialogWindow.h"
#include "TopWindow.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "UtopiaApp.h"
#include "NiApplication.h"
#include "NiAppWindow.h"
#include "renderer/NiD3DRender.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x005550f0 _$E523
// FUNCTION: STBC 0x00555110 _$E522

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005549f0 _$E33
// FUNCTION: STBC 0x00554a00 _$E32
// FUNCTION: STBC 0x00554a10 _$E37
// FUNCTION: STBC 0x00554a20 _$E36
// FUNCTION: STBC 0x00554a30 _$E48
// FUNCTION: STBC 0x00554a40 _$E47
// FUNCTION: STBC 0x00554a50 _$E52
// FUNCTION: STBC 0x00554a60 _$E51
// FUNCTION: STBC 0x00554a70 _$E56
// FUNCTION: STBC 0x00554a80 _$E55
// FUNCTION: STBC 0x00554a90 _$E60
// FUNCTION: STBC 0x00554aa0 _$E59
// FUNCTION: STBC 0x00554ab0 _$E184
// FUNCTION: STBC 0x00554ac0 _$E183
// FUNCTION: STBC 0x00554ad0 _$E126
// FUNCTION: STBC 0x00554ae0 _$E125
// FUNCTION: STBC 0x00554af0 _$E139
// FUNCTION: STBC 0x00554b00 _$E138
// FUNCTION: STBC 0x00554b10 _$E189
// FUNCTION: STBC 0x00554b20 _$E188
// FUNCTION: STBC 0x00554b30 _$E194
// FUNCTION: STBC 0x00554b40 _$E193
// FUNCTION: STBC 0x00554b50 _$E198
// FUNCTION: STBC 0x00554b60 _$E197
// FUNCTION: STBC 0x00554b70 _$E202
// FUNCTION: STBC 0x00554b80 _$E201
// FUNCTION: STBC 0x00554b90 _$E209
// FUNCTION: STBC 0x00554ba0 _$E208
// FUNCTION: STBC 0x00554bb0 _$E343
// FUNCTION: STBC 0x00554bc0 _$E342
// FUNCTION: STBC 0x00554bd0 _$E347
// FUNCTION: STBC 0x00554be0 _$E346
// FUNCTION: STBC 0x00554bf0 _$E84
// FUNCTION: STBC 0x00554c00 _$E83
// FUNCTION: STBC 0x00554c10 _$E329
// FUNCTION: STBC 0x00554c20 _$E328
// FUNCTION: STBC 0x00554c30 _$E336
// FUNCTION: STBC 0x00554c40 _$E335
// FUNCTION: STBC 0x00554c50 _$E355
// FUNCTION: STBC 0x00554c60 _$E354
// FUNCTION: STBC 0x00554c70 _$E359
// FUNCTION: STBC 0x00554c80 _$E358
// FUNCTION: STBC 0x00554c90 _$E399
// FUNCTION: STBC 0x00554ca0 _$E398
// FUNCTION: STBC 0x00554cb0 _$E405
// FUNCTION: STBC 0x00554cc0 _$E404
// FUNCTION: STBC 0x00554cd0 _$E319
// FUNCTION: STBC 0x00554ce0 _$E318
// FUNCTION: STBC 0x00554cf0 _$E351
// FUNCTION: STBC 0x00554d00 _$E350
// FUNCTION: STBC 0x00554d10 _$E363
// FUNCTION: STBC 0x00554d20 _$E362
// FUNCTION: STBC 0x00554d30 _$E367
// FUNCTION: STBC 0x00554d40 _$E366
// FUNCTION: STBC 0x00554d50 _$E371
// FUNCTION: STBC 0x00554d60 _$E370
// FUNCTION: STBC 0x00554d70 _$E375
// FUNCTION: STBC 0x00554d80 _$E374
// FUNCTION: STBC 0x00554d90 _$E379
// FUNCTION: STBC 0x00554da0 _$E378
// FUNCTION: STBC 0x00554db0 _$E383
// FUNCTION: STBC 0x00554dc0 _$E382
// FUNCTION: STBC 0x00554dd0 _$E387
// FUNCTION: STBC 0x00554de0 _$E386
// FUNCTION: STBC 0x00554df0 _$E391
// FUNCTION: STBC 0x00554e00 _$E390
// FUNCTION: STBC 0x00554e10 _$E89
// FUNCTION: STBC 0x00554e20 _$E88
// FUNCTION: STBC 0x00554e30 _$E395
// FUNCTION: STBC 0x00554e40 _$E394
// FUNCTION: STBC 0x00554e50 _$E94
// FUNCTION: STBC 0x00554e60 _$E93
// FUNCTION: STBC 0x00554e70 _$E409
// FUNCTION: STBC 0x00554e80 _$E408
// FUNCTION: STBC 0x00554e90 _$E413
// FUNCTION: STBC 0x00554ea0 _$E412
// FUNCTION: STBC 0x00554eb0 _$E417
// FUNCTION: STBC 0x00554ec0 _$E416
// FUNCTION: STBC 0x00554ed0 _$E421
// FUNCTION: STBC 0x00554ee0 _$E420
// FUNCTION: STBC 0x00554ef0 _$E103
// FUNCTION: STBC 0x00554f00 _$E102
// FUNCTION: STBC 0x00554f10 _$E425
// FUNCTION: STBC 0x00554f20 _$E424
// FUNCTION: STBC 0x00554f30 _$E429
// FUNCTION: STBC 0x00554f40 _$E428
// FUNCTION: STBC 0x00554f50 _$E433
// FUNCTION: STBC 0x00554f60 _$E432
// FUNCTION: STBC 0x00554f70 _$E109
// FUNCTION: STBC 0x00554f80 _$E108
// FUNCTION: STBC 0x00554f90 _$E114
// FUNCTION: STBC 0x00554fa0 _$E113
// FUNCTION: STBC 0x00554fb0 _$E144
// FUNCTION: STBC 0x00554fc0 _$E143
// FUNCTION: STBC 0x00555010 _$E437
// FUNCTION: STBC 0x00555020 _$E436
// FUNCTION: STBC 0x00555030 _$E149
// FUNCTION: STBC 0x00555040 _$E148
// FUNCTION: STBC 0x00555050 _$E441
// FUNCTION: STBC 0x00555060 _$E440
// FUNCTION: STBC 0x00555070 _$E445
// FUNCTION: STBC 0x00555080 _$E444
// FUNCTION: STBC 0x00555090 _$E154
// FUNCTION: STBC 0x005550a0 _$E153
// FUNCTION: STBC 0x005550b0 _$E449
// FUNCTION: STBC 0x005550c0 _$E448
// FUNCTION: STBC 0x005550d0 _$E119
// FUNCTION: STBC 0x005550e0 _$E118
// FUNCTION: STBC 0x00555120 _$E453
// FUNCTION: STBC 0x00555130 _$E452
// FUNCTION: STBC 0x00555140 _$E457
// FUNCTION: STBC 0x00555150 _$E456
// FUNCTION: STBC 0x00555160 _$E159
// FUNCTION: STBC 0x00555170 _$E158
// FUNCTION: STBC 0x00555180 _$E164
// FUNCTION: STBC 0x00555190 _$E163
// FUNCTION: STBC 0x005551a0 _$E461
// FUNCTION: STBC 0x005551b0 _$E460
// FUNCTION: STBC 0x005551c0 _$E465
// FUNCTION: STBC 0x005551d0 _$E464
// FUNCTION: STBC 0x005551e0 _$E469
// FUNCTION: STBC 0x005551f0 _$E468
// FUNCTION: STBC 0x00555200 _$E473
// FUNCTION: STBC 0x00555210 _$E472
// FUNCTION: STBC 0x00555220 _$E477
// FUNCTION: STBC 0x00555230 _$E476
// FUNCTION: STBC 0x00555240 _$E481
// FUNCTION: STBC 0x00555250 _$E480
// FUNCTION: STBC 0x00555260 _$E485
// FUNCTION: STBC 0x00555270 _$E484
// FUNCTION: STBC 0x00555280 _$E489
// FUNCTION: STBC 0x00555290 _$E488
// FUNCTION: STBC 0x005552a0 _$E169
// FUNCTION: STBC 0x005552b0 _$E168
// FUNCTION: STBC 0x005552c0 _$E493
// FUNCTION: STBC 0x005552d0 _$E492
// FUNCTION: STBC 0x005552e0 _$E174
// FUNCTION: STBC 0x005552f0 _$E173
// FUNCTION: STBC 0x00555300 _$E179
// FUNCTION: STBC 0x00555310 _$E178
// FUNCTION: STBC 0x00555320 _$E497
// FUNCTION: STBC 0x00555330 _$E496
// FUNCTION: STBC 0x00555340 _$E502
// FUNCTION: STBC 0x00555350 _$E501
// FUNCTION: STBC 0x00555360 _$E506
// FUNCTION: STBC 0x00555370 _$E505

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00553bf0 _$E294
// FUNCTION: STBC 0x00553c00 _$E291
// FUNCTION: STBC 0x00553c10 _$E293
// FUNCTION: STBC 0x00553c20 _$E292
// FUNCTION: STBC 0x00553c30 _$E300
// FUNCTION: STBC 0x00553c40 _$E297
// FUNCTION: STBC 0x00553c50 _$E299
// FUNCTION: STBC 0x00553c60 _$E298

// The handler table, its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00552940 _$E512
// FUNCTION: STBC 0x00552950 _$E508
// FUNCTION: STBC 0x00552960 _$E511
// FUNCTION: STBC 0x00552970 _$E509
// DATA: STBC 0x0098b228
TGEventHandlerTable GraphicsMenu::classHandlers;

/*
================
GraphicsMenu::Cast

The object as a GraphicsMenu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00552990
GraphicsMenu *GraphicsMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( GRAPHICSMENU_TYPE ) ) {
		return (GraphicsMenu *)object;
	}
	return NULL;
}

// The graphics options' state and its dynamic initialiser.
// FUNCTION: STBC 0x005529c0 _$E515
// FUNCTION: STBC 0x005529d0 _$E514
// DATA: STBC 0x0098b220
GraphicsMenuInfo *graphicsMenuInfo = new GraphicsMenuInfo;

/*
================
GraphicsMenu::GraphicsMenu

The title is not used.
================
*/
// FUNCTION: STBC 0x00552aa0 ??0GraphicsMenu@@QAE@PAVTGString@@@Z
GraphicsMenu::GraphicsMenu( TGString *title ) {
	Init();
}

/*
================
GraphicsMenu::Init
================
*/
// FUNCTION: STBC 0x00552be0
void GraphicsMenu::Init( void ) {
	graphicsMenuInfo->graphicsMenu = this;
	graphicsMenuInfo->database = g_kLocalizationManager.Load( "data/TGL/Options.tgl", true );
}

/*
================
GraphicsMenu::~GraphicsMenu

The open menu unloads the options strings.
================
*/
// FUNCTION: STBC 0x00552c10
GraphicsMenu::~GraphicsMenu( void ) {
	if ( graphicsMenuInfo->graphicsMenu == this ) {
		graphicsMenuInfo->graphicsMenu = NULL;
		if ( graphicsMenuInfo->database != NULL ) {
			g_kLocalizationManager.Unload( graphicsMenuInfo->database );
			graphicsMenuInfo->database = NULL;
		}
	}
}

/*
================
GraphicsMenu::GetDevicesMenu

A menu of the hardware devices, titled with the current mode's device;
each sends its index among them with ET_GRAPHICS_DEVICE_CHANGE. The names
drop "Direct3D " and "HAL on ". The renderer's device has the focus; with
detection on, it is the current mode's device and its button is disabled.
================
*/
// FUNCTION: STBC 0x00552c90 ?GetDevicesMenu@GraphicsMenu@@QAEPAVSTMenu@@XZ
// PARKED: 89.1, complete; same calls, branches and frame, register allocation only: retail reuses the mode register for its device name and keeps the list position in eax where this build uses ecx and edx
STMenu *GraphicsMenu::GetDevicesMenu( void ) {
	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	if ( renderer == NULL ) {
		return NULL;
	}

	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	Ni3DDeviceDesc *current = renderer->Unknown7C2EE0();
	if ( graphicsMenuInfo->detect ) {
		mode->SetDevice( current->name );
	}

	char buffer[256];
	{
		TGString deviceName( mode->GetDevice() );
		int position = deviceName.FindC( "Direct3D " );
		if ( position >= 0 ) {
			deviceName.Unknown6F55C0( position, strlen( "Direct3D " ) );
		}
		position = deviceName.FindC( "HAL on " );
		if ( position >= 0 ) {
			deviceName.Unknown6F55C0( position, strlen( "HAL on " ) );
		}
		char *name = deviceName.Unknown6F56F0();
		sprintf( buffer, " [%s]", name );
		delete name;
	}
	TGString title( buffer );
	STMenu *menu = new STMenu( &( *graphicsMenuInfo->database->GetString( "Display Device" ) + title ), STMF_DEFAULTS, NULL, NULL );

	NiTList<Ni3DDeviceDesc *> *devices = NiD3DRender::GetDisplayDevices();
	if ( devices != NULL ) {
		NiTListIterator pos = devices->GetHeadPos();
		unsigned int index = 0;
		while ( pos ) {
			Ni3DDeviceDesc *device = devices->GetNext( pos );
			if ( device->IsHardware() ) {
				TGIntEvent *event = new TGIntEvent;
				event->SetEventType( ET_GRAPHICS_DEVICE_CHANGE );
				event->SetInt( index );
				event->SetDestination( this );
				STButton *button;
				{
					TGString deviceName( device->name );
					int position = deviceName.FindC( "Direct3D " );
					if ( position >= 0 ) {
						deviceName.Unknown6F55C0( position, strlen( "Direct3D " ) );
					}
					position = deviceName.FindC( "HAL on " );
					if ( position >= 0 ) {
						deviceName.Unknown6F55C0( position, strlen( "HAL on " ) );
					}
					char *name = deviceName.Unknown6F56F0();
					button = new STButton( name, event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 );
					delete name;
				}
				menu->AddChild( button );
				if ( graphicsMenuInfo->detect ) {
					if ( strcmp( device->GetName(), current->GetName() ) == 0 ) {
						button->SetDisabled();
						menu->SetFocus( button );
					}
				} else if ( strcmp( device->GetName(), current->GetName() ) == 0 ) {
					menu->SetFocus( button );
				}
				index++;
			}
		}
		if ( index < 1 ) {
			menu->SetDisabled();
		}
	}
	return menu;
}

/*
================
GraphicsMenu::GetResolutionMenu

A menu of the device's modes at the current colour depth that have an
LCARS layout, up to the card's maximum width (MainMenu.CardSpecificSetup's
GetMaxRez, 1600 when it gives none); each sends its index with
ET_GRAPHICS_RESOLUTION_CHANGE. Modes as big as the desktop are disabled
in a window. The current mode's button gets the focus; with detection on,
the renderer's size is the current one, its button is disabled and the
current mode takes the size.
================
*/
// FUNCTION: STBC 0x005530e0 ?GetResolutionMenu@GraphicsMenu@@QAEPAVSTMenu@@XZ
// PARKED: frame and registers: retail's slots run index, maxWidth, renderer, pos (pos sharing a slot with a renderer value) where this build sorts pos before maxWidth; control flow and saves agree
STMenu *GraphicsMenu::GetResolutionMenu( void ) {
	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	if ( renderer == NULL ) {
		return NULL;
	}

	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	Ni3DDeviceDesc *device = renderer->Unknown7C2EE0();
	int maxWidth = 1600;
	PyEmbed::CallFunction( "MainMenu.CardSpecificSetup", "GetMaxRez", "i", &maxWidth, "(s)", mode->GetDevice() );
	if ( maxWidth == 0 ) {
		maxWidth = 1600;
	}
	int desktopWidth = GetSystemMetrics( SM_CXFULLSCREEN );
	int desktopHeight = GetSystemMetrics( SM_CYFULLSCREEN );

	char buffer[256];
	if ( graphicsMenuInfo->detect ) {
		sprintf( buffer, " [%u x %u]", renderer->VirtualSlot19(), renderer->VirtualSlot20() );
	} else {
		sprintf( buffer, " [%u x %u]", mode->width, mode->GetHeight() );
	}
	TGString size( buffer );
	STMenu *menu = new STMenu( &( *graphicsMenuInfo->database->GetString( "Resolution" ) + size ), STMF_DEFAULTS, NULL, NULL );

	if ( device == NULL || device->modes == NULL ) {
		return menu;
	}

	unsigned int index = 0;
	NiTListIterator pos = device->modes->GetHeadPos();
	while ( pos ) {
		NiD3DDisplayMode *displayMode = device->modes->GetNext( pos );
		if ( displayMode->bitDepth == (unsigned int)mode->GetColorDepth() && displayMode->width >= 640 && displayMode->height >= 480
			 && ( ( displayMode->width == 640 && displayMode->height == 480 ) || ( displayMode->width == 800 && displayMode->height == 600 )
				  || ( displayMode->width == 1024 && displayMode->height == 768 ) || ( displayMode->width == 1280 && displayMode->height == 1024 )
				  || ( displayMode->width == 1600 && displayMode->height == 1200 ) )
			 && (int)displayMode->width <= maxWidth ) {
			TGIntEvent *event = new TGIntEvent;
			event->SetInt( index );
			event->SetEventType( ET_GRAPHICS_RESOLUTION_CHANGE );
			event->SetDestination( this );
			char name[256];
			sprintf( name, "%u x %u", displayMode->width, displayMode->height );
			STButton *button = new STButton( name, event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 );
			menu->AddChild( button, 0.0f, 0.0f, true );
			if ( mode->IsWindowed() ) {
				if ( displayMode->width >= (unsigned int)desktopWidth || displayMode->height >= (unsigned int)desktopHeight ) {
					button->SetDisabled();
				}
			}
			if ( graphicsMenuInfo->detect ) {
				if ( renderer->VirtualSlot19() == displayMode->width && renderer->VirtualSlot20() == displayMode->height ) {
					button->SetDisabled();
					menu->SetFocus( button, true );
					mode->width = renderer->VirtualSlot19();
					mode->height = renderer->VirtualSlot20();
				}
			} else if ( mode->width == displayMode->width && (unsigned int)mode->GetHeight() == displayMode->height ) {
				menu->SetFocus( button, true );
			}
		}
		index++;
	}
	if ( index < 1 ) {
		menu->SetDisabled();
	}
	return menu;
}

/*
================
GraphicsMenu::GetColorDepthToggle

The low / high colour depth toggle; a disabled "low" one when the device
can't render at 32 bits. With detection on, the state is the renderer's
depth, otherwise the current mode's.
================
*/
// FUNCTION: STBC 0x00553520
STToggle *GraphicsMenu::GetColorDepthToggle( void ) {
	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	if ( renderer != NULL ) {
		Ni3DDeviceDesc *device = renderer->Unknown7C2EE0();
		if ( device != NULL && ( device->flags & DEVICE_DESC_RENDERS_32_BIT ) ) {
			int state;
			if ( graphicsMenuInfo->detect ) {
				state = ( renderer->VirtualSlot13() >> 8 ) & 1;
			} else {
				state = mode->GetColorDepth() != 16;
			}
			TGIntEvent *event = new TGIntEvent;
			event->SetEventType( ET_GRAPHICS_BUTTON_CLICKED );
			event->SetInt( 1 );
			event->SetDestination( this );
			graphicsMenuInfo->colorDepthToggle = new STToggle( graphicsMenuInfo->database->GetString( "Color depth:" ), state,
				graphicsMenuInfo->database->GetString( "[LOW] HIGH" ), event,
				graphicsMenuInfo->database->GetString( "LOW [HIGH]" ), event, NULL, NULL, NULL, NULL );
			return graphicsMenuInfo->colorDepthToggle;
		}
		graphicsMenuInfo->colorDepthToggle = new STToggle( graphicsMenuInfo->database->GetString( "Color depth:" ), 0,
			graphicsMenuInfo->database->GetString( "LOW" ), NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		graphicsMenuInfo->colorDepthToggle->SetDisabled();
		return graphicsMenuInfo->colorDepthToggle;
	}
	graphicsMenuInfo->colorDepthToggle = new STToggle( graphicsMenuInfo->database->GetString( "Color depth:" ), 0,
		graphicsMenuInfo->database->GetString( "LOW" ), NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	graphicsMenuInfo->colorDepthToggle->SetDisabled();
	return graphicsMenuInfo->colorDepthToggle;
}

/*
================
GraphicsMenu::GetSizeToggle

The full screen / windowed toggle; a disabled "full screen" one when the
device can't render into a window.
================
*/
// FUNCTION: STBC 0x005537b0
STToggle *GraphicsMenu::GetSizeToggle( void ) {
	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	if ( renderer != NULL ) {
		Ni3DDeviceDesc *device = renderer->Unknown7C2EE0();
		if ( device != NULL && device->windowed ) {
			int state = mode->IsWindowed() == true;
			TGIntEvent *event = new TGIntEvent;
			event->SetEventType( ET_GRAPHICS_BUTTON_CLICKED );
			event->SetInt( 0 );
			event->SetDestination( this );
			graphicsMenuInfo->sizeToggle = new STToggle( graphicsMenuInfo->database->GetString( "Size:" ), state,
				graphicsMenuInfo->database->GetString( "[FULL-SCREEN] WINDOWED" ), event,
				graphicsMenuInfo->database->GetString( "FULL-SCREEN [WINDOWED]" ), event, NULL, NULL, NULL, NULL );
			return graphicsMenuInfo->sizeToggle;
		}
		graphicsMenuInfo->sizeToggle = new STToggle( graphicsMenuInfo->database->GetString( "Size:" ), 0,
			graphicsMenuInfo->database->GetString( "FULL SCREEN" ), NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	} else {
		graphicsMenuInfo->sizeToggle = new STToggle( graphicsMenuInfo->database->GetString( "Size:" ), 0,
			graphicsMenuInfo->database->GetString( "FULL SCREEN" ), NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	}
	graphicsMenuInfo->sizeToggle->SetDisabled();
	return graphicsMenuInfo->sizeToggle;
}

/*
================
GraphicsMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00553a00
void GraphicsMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&GraphicsMenu::HandleButtons, "GraphicsMenu::HandleButtons" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&GraphicsMenu::DeviceChangeHandler, "GraphicsMenu::DeviceChangeHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&GraphicsMenu::ResolutionChangePreHandler, "GraphicsMenu::ResolutionChangePreHandler" );
}

/*
================
GraphicsMenu::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00553a40
void GraphicsMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_BUTTON_CLICKED, "GraphicsMenu::HandleButtons" );
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_DEVICE_CHANGE, "GraphicsMenu::DeviceChangeHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_RESOLUTION_CHANGE, "GraphicsMenu::ResolutionChangePreHandler" );
}

/*
================
GraphicsMenu::HandleButtons

The size toggle (0) switches between full screen and a window, the colour
depth toggle (1) between 16 and 32 bits; either changes the display mode
and reports a failure in a dialog.
================
*/
// FUNCTION: STBC 0x00553a80
void GraphicsMenu::HandleButtons( TGEvent *event ) {
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	TGLocalizationDatabase *database;
	TGString *message;
	bool changed = true;
	switch ( ( (TGIntEvent *)event )->GetInt() ) {
		case 0: {
			if ( graphicsMenuInfo->sizeToggle->GetState() == 0 ) {
				mode->windowed = false;
			} else {
				mode->windowed = true;
			}
			graphicsMenuInfo->detect = false;
			ChangeDisplayMode();
			GraphicsMenu *menu = GetGraphicsMenu();
			if ( menu != NULL ) {
				graphicsMenuInfo->detect = false;
				changed = menu->ChangeDisplayMode();
			}
			UpdateToggles( GetGraphicsMenu() );
			if ( changed ) {
				return;
			}
			database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
			message = database->GetString( "Change Resolution Failed" );
			g_kLocalizationManager.Unload( database );
			break;
		}
		case 1:
			if ( graphicsMenuInfo->colorDepthToggle->GetState() == 0 ) {
				mode->colorDepth = 16;
			} else {
				mode->colorDepth = 32;
			}
			graphicsMenuInfo->detect = false;
			changed = ChangeDisplayMode();
			UpdateToggles( GetGraphicsMenu() );
			if ( changed ) {
				return;
			}
			database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
			message = database->GetString( "Color Depth Failed" );
			g_kLocalizationManager.Unload( database );
			break;
		default:
			return;
	}

	ModalDialogWindow *dialog = (ModalDialogWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MODAL_DIALOG );
	database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
	dialog->Run( NULL, message, database->GetString( "OK" ), NULL, NULL, NULL );
	g_kLocalizationManager.Unload( database );
}

/*
================
GraphicsMenu::DeviceChangeHandler

Makes the chosen device the current mode's and changes the display mode.
The event's integer counts among the hardware devices that can render the
current mode: in a window, only devices that can render into one; at 32
bits, only devices that render 32 bits.
================
*/
// FUNCTION: STBC 0x00553c70
// PARKED: complete; inline budget: retail calls operator='s RemoveHead out of line, which needs operator='s share below 100 (ours 187): 88 to 137 more units spent before it; no evidence-backed site of that cost found
void GraphicsMenu::DeviceChangeHandler( TGEvent *event ) {
	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	if ( renderer == NULL ) {
		return;
	}
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	renderer->Unknown7C2EE0();
	NiTList<Ni3DDeviceDesc *> *devices = NiD3DRender::GetDisplayDevices();
	if ( devices == NULL ) {
		return;
	}
	int index = 0;
	NiTListIterator pos = devices->GetHeadPos();
	while ( pos ) {
		Ni3DDeviceDesc *device = devices->GetNext( pos );
		if ( ( ( mode->IsWindowed() && device->IsWindowed() ) || !mode->IsWindowed() )
			 && ( ( mode->GetColorDepth() == 32 && ( device->GetFlags() & DEVICE_DESC_RENDERS_32_BIT ) ) || mode->GetColorDepth() == 16 )
			 && device->IsHardware() ) {
			if ( index == ( (TGIntEvent *)event )->GetInt() ) {
				Ni3DDeviceDesc *desc = new Ni3DDeviceDesc;
				*desc = *device;
				mode->SetDesc( desc );
				mode->SetDevice( device->name );
				graphicsMenuInfo->detect = false;
				ChangeDisplayMode();
				return;
			}
			index++;
		}
	}
}

/*
================
GraphicsMenu::ResolutionChangePreHandler

Asks for confirmation before a resolution change: the dialog's OK sends
ET_GRAPHICS_RESOLUTION_CHANGE_STAGE2, with the chosen resolution's index,
to the top window.
================
*/
// FUNCTION: STBC 0x00553f60
// PARKED: 68.7, complete; register allocation: retail keeps this in ebp and the event in ebx and spills the top window to the frame, this build keeps the top window in ebx and spills this
void GraphicsMenu::ResolutionChangePreHandler( TGEvent *event ) {
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
	TGString *message = database->GetString( "Change Res Help" );
	TopWindow *topWindow = TopWindow::GetTopWindow();
	ModalDialogWindow *dialog = (ModalDialogWindow *)topWindow->FindMainWindow( MWT_MODAL_DIALOG );

	TGIntEvent *okEvent = new TGIntEvent;
	okEvent->SetEventType( ET_GRAPHICS_RESOLUTION_CHANGE_STAGE2 );
	okEvent->SetInt( ( (TGIntEvent *)event )->GetInt() );
	okEvent->SetDestination( topWindow );

	dialog->Run( NULL, message, database->GetString( "OK" ), okEvent, database->GetString( "Cancel" ), NULL );
	g_kLocalizationManager.Unload( database );
	CallNextHandler( event );
}

/*
================
GraphicsMenu::ChangeDisplayMode

Makes a renderer for the chosen mode. In a window, the resolution steps
down until the window fits the screen. When the renderer fails, a failed
undo quits the game; otherwise a window of the mode is tried, then one of
the other frame buffer format, and when both fail the old mode is
restored by undoing. With the renderer made, the icon groups, the user
interface and the fonts are rebuilt for the new resolution. False when no
renderer of the chosen mode was made.
================
*/
// FUNCTION: STBC 0x00554040
// PARKED: frame-slot order of two spilled locals flips with the declarations the unit sees (any of 1-5 more in GraphicsModeInfo.h, wherever placed)
bool GraphicsMenu::ChangeDisplayMode( void ) {
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	int fullScreen = IsChosenModeFullScreen();
	unsigned int width = GetChosenModeWidth();
	unsigned int height = GetChosenModeHeight();
	int flags = GetChosenModeFlags();
	bool failed = false;
	int screenWidth = GetSystemMetrics( SM_CXFULLSCREEN );
	int screenHeight = GetSystemMetrics( SM_CYFULLSCREEN );

	HWND window = GetChosenModeWindow();
	if ( window == NULL ) {
		return false;
	}
	Ni3DDeviceDesc *desc = GetChosenModeDesc();
	if ( desc == NULL ) {
		return false;
	}

	if ( !fullScreen ) {
		RECT rect;
		for ( ;; ) {
			rect.left = 0;
			rect.top = 0;
			rect.right = width;
			rect.bottom = height;
			DWORD style = GetWindowLongA( NiApplication::application->GetAppWindow()->GetWindow(), GWL_STYLE );
			AdjustWindowRectEx( &rect, style, FALSE, GetWindowLongA( NiApplication::application->GetAppWindow()->GetWindow(), GWL_EXSTYLE ) );
			if ( ( rect.right <= screenWidth && rect.bottom <= screenHeight ) || width == 640 ) {
				break;
			}
			if ( width == 800 ) {
				width = 640;
				height = 480;
			} else if ( width == 1024 ) {
				width = 800;
				height = 600;
			} else if ( width == 1280 ) {
				width = 1024;
				height = 768;
			} else if ( width == 1600 ) {
				width = 1280;
				height = 1024;
			}
		}
		MoveWindow( NiApplication::application->GetAppWindow()->GetWindow(), 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE );
		mode->width = width;
		mode->height = height;
	}

	g_kUtopiaModule.VirtualSlot06();
	NiD3DRender *renderer = NiD3DRender::Create( fullScreen, width, height, flags, window,
		NiApplication::application->GetAppWindow()->GetWindow(), desc );
	graphicsMenuInfo->modeWasChanged = true;
	graphicsMenuInfo->detect = true;
	if ( renderer == NULL ) {
		failed = true;
		if ( graphicsMenuInfo->weAreUndoing ) {
			graphicsMenuInfo->weAreUndoing = false;
			TGIntEvent *quitEvent = new TGIntEvent;
			quitEvent->SetDestination( TopWindow::GetTopWindow() );
			quitEvent->SetInt( 1 );
			quitEvent->SetEventType( ET_QUIT );
			g_kEventManager.AddEvent( quitEvent );
			return false;
		}
		renderer = NiD3DRender::Create( 1, width, height, flags, window, NiApplication::application->GetAppWindow()->GetWindow(), desc );
		if ( renderer == NULL ) {
			renderer = NiD3DRender::Create( 1, width, height, 8, window, NiApplication::application->GetAppWindow()->GetWindow(), desc );
			if ( renderer == NULL ) {
				graphicsMenuInfo->weAreUndoing = true;
				ChangeDisplayMode();
				return false;
			}
		}
	}
	graphicsMenuInfo->weAreUndoing = false;
	g_kUtopiaModule.VirtualSlot07( (int)renderer );
	g_kIconManager.SetScreenWidth( (float)width );
	g_kIconManager.SetScreenHeight( (float)height );

	char moduleName[256];
	sprintf( moduleName, "Icons.%s", graphicsMenuInfo->oldMode.GetLcarsString() );
	PyEmbed::UnloadModule( moduleName );
	sprintf( moduleName, "Icons.%s", mode->GetLcarsString() );
	PyEmbed::GetModule( moduleName, false );
	PyEmbed::CallFunction( moduleName, "Switch", NULL, NULL, NULL );

	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		if ( object->IsTypeOf( TGUIOBJECT_TYPE ) ) {
			( (TGUIObject *)object )->VirtualSlot60();
			( (TGUIObject *)object )->DeleteBatches();
		}
		TGIcon *icon = TGIcon::Cast( object );
		if ( icon != NULL && strstr( icon->GetIconGroupName(), "LCARS_" ) != NULL ) {
			icon->SetIconGroupName( mode->GetLcarsString(), false );
		}
	}

	g_kIconManager.ReloadIconGroups();
	graphicsMenuInfo->undoMode = *mode;
	g_kRootWindow.VirtualSlot60();
	Ni3DDeviceDesc *device = renderer->Unknown7C2EE0();
	if ( device != NULL && device->GetName() != NULL ) {
		PyEmbed::CallFunction( "MainMenu.CardSpecificSetup", "ResChanged", NULL, NULL, "(s)", device->GetName() );
	}
	g_kRootWindow.Layout();

	TGIntEvent *event = new TGIntEvent;
	event->SetDestination( &g_kRootWindow );
	event->SetEventType( ET_UI_SWITCH_ICON_GROUPS );
	event->SetInt( graphicsMenuInfo->oldMode.GetCurrentResolution() );
	g_kEventManager.ProcessEvent( event );
	delete event;

	event = new TGIntEvent;
	event->SetDestination( &g_kRootWindow );
	event->SetEventType( ET_UI_RESIZE );
	event->SetInt( graphicsMenuInfo->oldMode.GetCurrentResolution() );
	g_kEventManager.ProcessEvent( event );
	delete event;

	event = new TGIntEvent;
	event->SetDestination( &g_kRootWindow );
	event->SetEventType( ET_UI_REPOSITION );
	event->SetInt( graphicsMenuInfo->oldMode.GetCurrentResolution() );
	g_kEventManager.ProcessEvent( event );
	delete event;

	PyEmbed::CallFunction( "MainMenu.mainmenu", "SetupMenuFont", NULL, NULL, NULL, NULL );
	return !failed;
}

/*
================
GraphicsMenu::IsChosenModeFullScreen
================
*/
// FUNCTION: STBC 0x00554650
int GraphicsMenu::IsChosenModeFullScreen( void ) {
	if ( graphicsMenuInfo->weAreUndoing ) {
		return !graphicsMenuInfo->undoMode.IsWindowed();
	}
	if ( graphicsMenuInfo->weAreCanceling ) {
		return !graphicsMenuInfo->oldMode.IsWindowed();
	}
	return !GraphicsModeInfo::GetCurrentMode()->IsWindowed();
}

/*
================
GraphicsMenu::GetChosenModeWidth
================
*/
// FUNCTION: STBC 0x00554690
unsigned int GraphicsMenu::GetChosenModeWidth( void ) {
	if ( graphicsMenuInfo->weAreUndoing ) {
		return graphicsMenuInfo->undoMode.width;
	}
	if ( graphicsMenuInfo->weAreCanceling ) {
		return graphicsMenuInfo->oldMode.width;
	}
	return GraphicsModeInfo::GetCurrentMode()->width;
}

/*
================
GraphicsMenu::GetChosenModeHeight
================
*/
// FUNCTION: STBC 0x005546c0
int GraphicsMenu::GetChosenModeHeight( void ) {
	if ( graphicsMenuInfo->weAreUndoing ) {
		return graphicsMenuInfo->undoMode.GetHeight();
	}
	if ( graphicsMenuInfo->weAreCanceling ) {
		return graphicsMenuInfo->oldMode.GetHeight();
	}
	return GraphicsModeInfo::GetCurrentMode()->GetHeight();
}

/*
================
GraphicsMenu::GetChosenModeFlags

The frame buffer flags of the mode's colour depth: 16 bits gives 8 | 1,
anything else 16 | 2 (the flags' names are unknown).
================
*/
// FUNCTION: STBC 0x005546f0
// PARKED: 64.0, complete; retail loads the current mode before the flag tests, reads each depth straight from its mode and repeats the flag tail for the current mode; this build sinks the load and shares the tail
int GraphicsMenu::GetChosenModeFlags( void ) {
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	if ( graphicsMenuInfo->weAreUndoing ) {
		mode = &graphicsMenuInfo->undoMode;
	} else if ( graphicsMenuInfo->weAreCanceling ) {
		mode = &graphicsMenuInfo->oldMode;
	}
	int colorDepth = mode->GetColorDepth();
	int flags = ( colorDepth == 16 ) ? 8 : 16;
	if ( colorDepth == 16 ) {
		return flags | 1;
	}
	return flags | 2;
}

/*
================
GraphicsMenu::GetChosenModeWindow

The window the mode draws into: the render window when windowed, the main
window full screen.
================
*/
// FUNCTION: STBC 0x00554750
HWND GraphicsMenu::GetChosenModeWindow( void ) {
	if ( graphicsMenuInfo->weAreUndoing ) {
		if ( graphicsMenuInfo->undoMode.IsWindowed() ) {
			return NiApplication::application->GetAppWindow()->GetRenderWindow();
		}
		return NiApplication::application->GetAppWindow()->GetWindow();
	}
	if ( graphicsMenuInfo->weAreCanceling ) {
		if ( graphicsMenuInfo->oldMode.IsWindowed() ) {
			return NiApplication::application->GetAppWindow()->GetRenderWindow();
		}
		return NiApplication::application->GetAppWindow()->GetWindow();
	}
	if ( GraphicsModeInfo::GetCurrentMode()->IsWindowed() ) {
		return NiApplication::application->GetAppWindow()->GetRenderWindow();
	}
	return NiApplication::application->GetAppWindow()->GetWindow();
}

/*
================
GraphicsMenu::GetChosenModeDesc
================
*/
// FUNCTION: STBC 0x005547c0
Ni3DDeviceDesc *GraphicsMenu::GetChosenModeDesc( void ) {
	if ( graphicsMenuInfo->weAreUndoing ) {
		return graphicsMenuInfo->undoMode.GetDesc();
	}
	if ( graphicsMenuInfo->weAreCanceling ) {
		return graphicsMenuInfo->oldMode.GetDesc();
	}
	return GraphicsModeInfo::GetCurrentMode()->GetDesc();
}

/*
================
GraphicsMenu::Unknown5547F0

Empties the menu and gives it one sub pane of its own size. Nothing calls it.
================
*/
// FUNCTION: STBC 0x005547f0
void GraphicsMenu::Unknown5547F0( void ) {
	focus = NULL;
	TGPane::SetFocus( NULL, false );
	TGPane::KillChildren();
	float width = GetWidth();
	float height = GetHeight();
	STSubPane *subPane = new STSubPane( DEFAULT_ST_INDENT_VERT, true );
	subPane->Resize( width, height, false );
	TGPane::InsertChild( 0x7FFFFFFF, subPane, 0.0f, 0.0f, false );
}

/*
================
GraphicsMenu::Unknown554880

Makes this the open menu again and reloads the options strings, then
unloads them with the menu detached from its parent. Nothing calls it.
================
*/
// FUNCTION: STBC 0x00554880
void GraphicsMenu::Unknown554880( void ) {
	Init();
	TGPane *savedParent = parent;
	parent = NULL;
	g_kLocalizationManager.Unload( graphicsMenuInfo->database );
	parent = savedParent;
}

/*
================
GraphicsMenu::ResetToggles
================
*/
// FUNCTION: STBC 0x005548b0 ?ResetToggles@GraphicsMenu@@QAEXXZ
void GraphicsMenu::ResetToggles( void ) {
	UpdateToggles( GetGraphicsMenu() );
}

/*
================
GraphicsMenu::UpdateToggles

The size toggle shows windowed or full screen, and the colour depth toggle
is usable only full screen; a device that can't render into a window keeps
full screen, one that can't render at 32 bits disables the colour depth.
================
*/
// FUNCTION: STBC 0x005548d0
// PARKED: 94.4, complete; retail computes the colour depth state before the toggle's NULL test, this build sinks the compare below it (an inline boundary or a statement order not found)
void GraphicsMenu::UpdateToggles( GraphicsMenu *menu ) {
	if ( menu == NULL ) {
		return;
	}
	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	if ( graphicsMenuInfo == NULL ) {
		return;
	}
	STToggle *sizeToggle = graphicsMenuInfo->sizeToggle;
	STToggle *colorDepthToggle = graphicsMenuInfo->colorDepthToggle;
	int state;
	if ( renderer != NULL ) {
		Ni3DDeviceDesc *device = renderer->Unknown7C2EE0();
		if ( device == NULL || !device->windowed ) {
			if ( sizeToggle != NULL ) {
				sizeToggle->SetState( 0, true );
			}
			if ( colorDepthToggle != NULL ) {
				colorDepthToggle->SetEnabled( true );
			}
		} else {
			if ( mode->IsWindowed() == true ) {
				state = 1;
				if ( colorDepthToggle != NULL ) {
					colorDepthToggle->SetDisabled( true );
				}
			} else {
				state = 0;
				if ( colorDepthToggle != NULL ) {
					colorDepthToggle->SetEnabled( true );
				}
			}
			if ( sizeToggle != NULL ) {
				sizeToggle->SetState( state, true );
			}
		}
		if ( device != NULL && ( device->flags & DEVICE_DESC_RENDERS_32_BIT ) ) {
			state = mode->GetColorDepth() != 16;
			if ( colorDepthToggle != NULL ) {
				colorDepthToggle->SetState( state, true );
			}
			return;
		}
	}
	if ( colorDepthToggle != NULL ) {
		colorDepthToggle->SetDisabled( true );
	}
}
