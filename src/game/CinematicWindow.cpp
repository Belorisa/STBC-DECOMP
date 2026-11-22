/*
===============================================================================

	CinematicWindow.cpp

===============================================================================
*/

#include <iostream>
#include "CinematicWindow.h"
#include "Game.h"
#include "PyEmbed.h"
#include "ReticleManagerWindow.h"
#include "SetManager.h"
#include "ShipClass.h"
#include "SubtitleWindow.h"
#include "TGEventHandlerTable.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TopWindow.h"
#include "WarpEngineSubsystem.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004fe270 _$E528
// FUNCTION: STBC 0x004fe290 _$E527

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004fd540 _$E32
// FUNCTION: STBC 0x004fd550 _$E29
// FUNCTION: STBC 0x004fd560 _$E31
// FUNCTION: STBC 0x004fd570 _$E30
// FUNCTION: STBC 0x004fd580 _$E38
// FUNCTION: STBC 0x004fd590 _$E35
// FUNCTION: STBC 0x004fd5a0 _$E37
// FUNCTION: STBC 0x004fd5b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004fd5c0 _$E57
// FUNCTION: STBC 0x004fd5d0 _$E56
// FUNCTION: STBC 0x004fd5e0 _$E61
// FUNCTION: STBC 0x004fd5f0 _$E60
// FUNCTION: STBC 0x004fd600 _$E72
// FUNCTION: STBC 0x004fd610 _$E71
// FUNCTION: STBC 0x004fd620 _$E76
// FUNCTION: STBC 0x004fd630 _$E75
// FUNCTION: STBC 0x004fd640 _$E80
// FUNCTION: STBC 0x004fd650 _$E79
// FUNCTION: STBC 0x004fd660 _$E84
// FUNCTION: STBC 0x004fd670 _$E83
// FUNCTION: STBC 0x004fd680 _$E208
// FUNCTION: STBC 0x004fd690 _$E207
// FUNCTION: STBC 0x004fda90 _$E150
// FUNCTION: STBC 0x004fdaa0 _$E149
// FUNCTION: STBC 0x004fdab0 _$E163
// FUNCTION: STBC 0x004fdac0 _$E162
// FUNCTION: STBC 0x004fdad0 _$E213
// FUNCTION: STBC 0x004fdae0 _$E212
// FUNCTION: STBC 0x004fdaf0 _$E218
// FUNCTION: STBC 0x004fdb00 _$E217
// FUNCTION: STBC 0x004fdb10 _$E222
// FUNCTION: STBC 0x004fdb20 _$E221
// FUNCTION: STBC 0x004fdb30 _$E226
// FUNCTION: STBC 0x004fdb40 _$E225
// FUNCTION: STBC 0x004fdb50 _$E233
// FUNCTION: STBC 0x004fdb60 _$E232
// FUNCTION: STBC 0x004fdb70 _$E364
// FUNCTION: STBC 0x004fdb80 _$E363
// FUNCTION: STBC 0x004fdb90 _$E368
// FUNCTION: STBC 0x004fdba0 _$E367
// FUNCTION: STBC 0x004fdbb0 _$E108
// FUNCTION: STBC 0x004fdbc0 _$E107
// FUNCTION: STBC 0x004fdbd0 _$E292
// FUNCTION: STBC 0x004fdbe0 _$E291
// FUNCTION: STBC 0x004fdbf0 _$E299
// FUNCTION: STBC 0x004fdc00 _$E298
// FUNCTION: STBC 0x004fdc10 _$E303
// FUNCTION: STBC 0x004fdc20 _$E302
// FUNCTION: STBC 0x004fdc30 _$E307
// FUNCTION: STBC 0x004fdc40 _$E306
// FUNCTION: STBC 0x004fdc50 _$E311
// FUNCTION: STBC 0x004fdc60 _$E310
// FUNCTION: STBC 0x004fdc70 _$E317
// FUNCTION: STBC 0x004fdc80 _$E316
// FUNCTION: STBC 0x004fdc90 _$E372
// FUNCTION: STBC 0x004fdca0 _$E371
// FUNCTION: STBC 0x004fdcb0 _$E376
// FUNCTION: STBC 0x004fdcc0 _$E375
// FUNCTION: STBC 0x004fdcd0 _$E380
// FUNCTION: STBC 0x004fdce0 _$E379
// FUNCTION: STBC 0x004fdcf0 _$E384
// FUNCTION: STBC 0x004fdd00 _$E383
// FUNCTION: STBC 0x004fdd10 _$E388
// FUNCTION: STBC 0x004fdd20 _$E387
// FUNCTION: STBC 0x004fdd30 _$E392
// FUNCTION: STBC 0x004fdd40 _$E391
// FUNCTION: STBC 0x004fdd50 _$E396
// FUNCTION: STBC 0x004fdd60 _$E395
// FUNCTION: STBC 0x004fdd70 _$E400
// FUNCTION: STBC 0x004fdd80 _$E399
// FUNCTION: STBC 0x004fdd90 _$E404
// FUNCTION: STBC 0x004fdda0 _$E403
// FUNCTION: STBC 0x004fddb0 _$E408
// FUNCTION: STBC 0x004fddc0 _$E407
// FUNCTION: STBC 0x004fddd0 _$E113
// FUNCTION: STBC 0x004fdde0 _$E112
// FUNCTION: STBC 0x004fddf0 _$E412
// FUNCTION: STBC 0x004fde00 _$E411
// FUNCTION: STBC 0x004fde10 _$E118
// FUNCTION: STBC 0x004fde20 _$E117
// FUNCTION: STBC 0x004fde30 _$E416
// FUNCTION: STBC 0x004fde40 _$E415
// FUNCTION: STBC 0x004fde50 _$E420
// FUNCTION: STBC 0x004fde60 _$E419
// FUNCTION: STBC 0x004fde70 _$E424
// FUNCTION: STBC 0x004fde80 _$E423
// FUNCTION: STBC 0x004fde90 _$E428
// FUNCTION: STBC 0x004fdea0 _$E427
// FUNCTION: STBC 0x004fdeb0 _$E127
// FUNCTION: STBC 0x004fdec0 _$E126
// FUNCTION: STBC 0x004fded0 _$E432
// FUNCTION: STBC 0x004fdee0 _$E431
// FUNCTION: STBC 0x004fdef0 _$E436
// FUNCTION: STBC 0x004fdf00 _$E435
// FUNCTION: STBC 0x004fdf10 _$E440
// FUNCTION: STBC 0x004fdf20 _$E439
// FUNCTION: STBC 0x004fdf30 _$E133
// FUNCTION: STBC 0x004fdf40 _$E132
// FUNCTION: STBC 0x004fdf50 _$E138
// FUNCTION: STBC 0x004fdf60 _$E137
// FUNCTION: STBC 0x004fdf70 _$E168
// FUNCTION: STBC 0x004fdf80 _$E167
// FUNCTION: STBC 0x004fdf90 _$E444
// FUNCTION: STBC 0x004fdfa0 _$E443
// FUNCTION: STBC 0x004fdfb0 _$E173
// FUNCTION: STBC 0x004fdfc0 _$E172
// FUNCTION: STBC 0x004fdfd0 _$E448
// FUNCTION: STBC 0x004fdfe0 _$E447
// FUNCTION: STBC 0x004fdff0 _$E452
// FUNCTION: STBC 0x004fe000 _$E451
// FUNCTION: STBC 0x004fe010 _$E178
// FUNCTION: STBC 0x004fe020 _$E177
// FUNCTION: STBC 0x004fe030 _$E456
// FUNCTION: STBC 0x004fe040 _$E455
// FUNCTION: STBC 0x004fe050 _$E143
// FUNCTION: STBC 0x004fe060 _$E142
// FUNCTION: STBC 0x004fe070 _$E460
// FUNCTION: STBC 0x004fe080 _$E459
// FUNCTION: STBC 0x004fe090 _$E464
// FUNCTION: STBC 0x004fe0a0 _$E463
// FUNCTION: STBC 0x004fe0b0 _$E183
// FUNCTION: STBC 0x004fe0c0 _$E182
// FUNCTION: STBC 0x004fe0d0 _$E188
// FUNCTION: STBC 0x004fe0e0 _$E187
// FUNCTION: STBC 0x004fe0f0 _$E468
// FUNCTION: STBC 0x004fe100 _$E467
// FUNCTION: STBC 0x004fe110 _$E472
// FUNCTION: STBC 0x004fe120 _$E471
// FUNCTION: STBC 0x004fe130 _$E476
// FUNCTION: STBC 0x004fe140 _$E475
// FUNCTION: STBC 0x004fe150 _$E480
// FUNCTION: STBC 0x004fe160 _$E479
// FUNCTION: STBC 0x004fe170 _$E484
// FUNCTION: STBC 0x004fe180 _$E483
// FUNCTION: STBC 0x004fe190 _$E488
// FUNCTION: STBC 0x004fe1a0 _$E487
// FUNCTION: STBC 0x004fe1b0 _$E492
// FUNCTION: STBC 0x004fe1c0 _$E491
// FUNCTION: STBC 0x004fe1d0 _$E496
// FUNCTION: STBC 0x004fe1e0 _$E495
// FUNCTION: STBC 0x004fe1f0 _$E193
// FUNCTION: STBC 0x004fe200 _$E192
// FUNCTION: STBC 0x004fe210 _$E500
// FUNCTION: STBC 0x004fe220 _$E499
// FUNCTION: STBC 0x004fe230 _$E198
// FUNCTION: STBC 0x004fe240 _$E197
// FUNCTION: STBC 0x004fe250 _$E203
// FUNCTION: STBC 0x004fe260 _$E202
// FUNCTION: STBC 0x004fe2a0 _$E504
// FUNCTION: STBC 0x004fe2b0 _$E503
// FUNCTION: STBC 0x004fe2c0 _$E509
// FUNCTION: STBC 0x004fe2d0 _$E508
// FUNCTION: STBC 0x004fe2e0 _$E513
// FUNCTION: STBC 0x004fe2f0 _$E512

// The handler table's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004fd6a0 _$E519
// FUNCTION: STBC 0x004fd6b0 _$E515
// FUNCTION: STBC 0x004fd6c0 _$E518
// FUNCTION: STBC 0x004fd6d0 _$E516
// DATA: STBC 0x00986e30
TGEventHandlerTable CinematicWindow::classHandlers;

/*
================
CinematicWindow::CinematicWindow

Hidden and interactive at first.
================
*/
// FUNCTION: STBC 0x004fd6f0
CinematicWindow::CinematicWindow( void ) : MainWindow( MWT_CINEMATIC, 1.0f, 1.0f ) {
	SetNotVisible();
	SetInteractive( true );
}

/*
================
CinematicWindow::GetObjType
================
*/
// FUNCTION: STBC 0x004fd750
int CinematicWindow::GetObjType( void ) {
	return CINEMATICWINDOW_TYPE;
}

/*
================
CinematicWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x004fd760
bool CinematicWindow::IsTypeOf( int type ) {
	if ( type == CINEMATICWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
CinematicWindow::GetClassName
================
*/
// FUNCTION: STBC 0x004fd7c0
char *CinematicWindow::GetClassName( void ) {
	return "CinematicWindow";
}

/*
================
CinematicWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004fd7d0
char *CinematicWindow::GetSwigTypeName( void ) {
	return "_p_CinematicWindow";
}

/*
================
CinematicWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004fd7e0
char *CinematicWindow::GetPythonClassName( void ) {
	return "CinematicWindowPtr";
}

/*
================
CinematicWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004fd7f0
TGEventHandlerTable *CinematicWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
CinematicWindow::SetInteractive

Sets or clears the window's interactive state flag.
================
*/
// FUNCTION: STBC 0x004fd840
void CinematicWindow::SetInteractive( bool interactive ) {
	if ( interactive ) {
		flags |= CINEMATICWINDOW_INTERACTIVE;
	} else {
		flags &= ~CINEMATICWINDOW_INTERACTIVE;
	}
}

/*
================
CinematicWindow::GotFocus

Renders the player's set, unless the ship is warping or the options or
console window has the focus; switches the player's camera to the
cinematic view, updates the reticles, places the subtitles for the
cinematic and shows the window.
================
*/
// FUNCTION: STBC 0x004fd870
void CinematicWindow::GotFocus( bool layout ) {
	TopWindow *top = TopWindow::GetTopWindow();
	TGUIObject::GotFocus( false );

	if ( Game::GetCurrentGame() != NULL ) {
		ShipClass *player = Game::GetCurrentGame()->GetPlayer();
		if ( player != NULL ) {
			SetClass *set = player->GetContainingSet();
			WarpEngineSubsystem *warpEngines = player->GetWarpEngineSubsystem();
			if ( set != NULL && ( warpEngines == NULL || warpEngines->GetWarpState() == WarpEngineSubsystem::WES_NOT_WARPING ) ) {
				MainWindow *options = top->FindMainWindow( MWT_OPTIONS );
				MainWindow *console = top->FindMainWindow( MWT_CONSOLE );
				if ( ( options == NULL || !options->IsInTrueFocusPath() ) && ( console == NULL || !console->IsInTrueFocusPath() ) ) {
					g_kSetManager.MakeRenderedSet( set );
				}
			}
		}
	}
	top->SetLastRenderedSet( NULL );

	PyEmbed::CallFunction( "Camera", "PlayerCameraAsCinematic", "", NULL, "()" );
	PyErr_Print();

	ReticleManagerWindow::GetReticleManagerWindow()->UpdateVisibility();

	SubtitleWindow *subtitles = (SubtitleWindow *)top->FindMainWindow( MWT_SUBTITLE );
	MoveToFront( subtitles, false );
	subtitles->SetPositionForMode( SM_CINEMATIC, false );
	SetVisible( layout );
}

/*
================
CinematicWindow::LostFocus

Hides the window, updates the reticles and places the subtitles for the
end of the cinematic.
================
*/
// FUNCTION: STBC 0x004fd970
void CinematicWindow::LostFocus( bool layout ) {
	TGUIObject::LostFocus( layout );
	SetNotVisible( layout );
	ReticleManagerWindow::GetReticleManagerWindow()->UpdateVisibility();
	SubtitleWindow *subtitles = (SubtitleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_SUBTITLE );
	subtitles->SetPositionForMode( SM_END_CINEMATIC, false );
}

/*
================
CinematicWindow::Initialize

Lets the scripts set up the cinematic interface.
================
*/
// FUNCTION: STBC 0x004fd9c0
void CinematicWindow::Initialize( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "CinematicInterfaceHandlers", "Initialize", "", NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
CinematicWindow::Update
================
*/
// FUNCTION: STBC 0x004fda00
void CinematicWindow::Update( float time ) {
}

/*
================
CinematicWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x004fda10
void CinematicWindow::RegisterHandlerNames( void ) {
}

/*
================
CinematicWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x004fda20
void CinematicWindow::RegisterHandlers( void ) {
	AddPythonFuncHandlerForClass( &classHandlers, ET_MOUSE, "CinematicInterfaceHandlers.HandleMouse" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_KEYBOARD, "CinematicInterfaceHandlers.HandleKeyboard" );
}

/*
================
CinematicWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x004fda50
bool CinematicWindow::WriteToStream( TGStream *stream ) {
	if ( !MainWindow::WriteToStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
CinematicWindow::ReadFromStream
================
*/
// FUNCTION: STBC 0x004fda70
bool CinematicWindow::ReadFromStream( TGStream *stream ) {
	if ( !MainWindow::ReadFromStream( stream ) ) {
		return false;
	}
	return true;
}
