/*
===============================================================================

	TacticalWindow.cpp

===============================================================================
*/

#include "TacticalWindow.h"
#include "CameraObjectClass.h"
#include "Game.h"
#include "NiMatrix3.h"
#include "PyEmbed.h"
#include "ReticleManagerWindow.h"
#include "SetClass.h"
#include "SetManager.h"
#include "SubtitleWindow.h"
#include "TacticalControlWindow.h"
#include "TGEventHandlerTable.h"
#include "TGInputManager.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TopWindow.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0050b7b0 _$E33
// FUNCTION: STBC 0x0050b7c0 _$E32
// FUNCTION: STBC 0x0050b7d0 _$E37
// FUNCTION: STBC 0x0050b7e0 _$E36
// FUNCTION: STBC 0x0050b7f0 _$E48
// FUNCTION: STBC 0x0050b800 _$E47
// FUNCTION: STBC 0x0050b810 _$E52
// FUNCTION: STBC 0x0050b820 _$E51
// FUNCTION: STBC 0x0050b830 _$E56
// FUNCTION: STBC 0x0050b840 _$E55
// FUNCTION: STBC 0x0050b850 _$E60
// FUNCTION: STBC 0x0050b860 _$E59
// FUNCTION: STBC 0x0050b870 _$E184
// FUNCTION: STBC 0x0050b880 _$E183
// FUNCTION: STBC 0x0050b890 _$E126
// FUNCTION: STBC 0x0050b8a0 _$E125
// FUNCTION: STBC 0x0050b8b0 _$E139
// FUNCTION: STBC 0x0050b8c0 _$E138
// FUNCTION: STBC 0x0050b8d0 _$E189
// FUNCTION: STBC 0x0050b8e0 _$E188
// FUNCTION: STBC 0x0050b8f0 _$E194
// FUNCTION: STBC 0x0050b900 _$E193
// FUNCTION: STBC 0x0050b910 _$E198
// FUNCTION: STBC 0x0050b920 _$E197
// FUNCTION: STBC 0x0050b930 _$E202
// FUNCTION: STBC 0x0050b940 _$E201
// FUNCTION: STBC 0x0050b950 _$E209
// FUNCTION: STBC 0x0050b960 _$E208
// FUNCTION: STBC 0x0050b970 _$E329
// FUNCTION: STBC 0x0050b980 _$E328
// FUNCTION: STBC 0x0050b990 _$E333
// FUNCTION: STBC 0x0050b9a0 _$E332
// FUNCTION: STBC 0x0050b9b0 _$E84
// FUNCTION: STBC 0x0050b9c0 _$E83
// FUNCTION: STBC 0x0050b9d0 _$E315
// FUNCTION: STBC 0x0050b9e0 _$E314
// FUNCTION: STBC 0x0050b9f0 _$E322
// FUNCTION: STBC 0x0050ba00 _$E321
// FUNCTION: STBC 0x0050ba10 _$E345
// FUNCTION: STBC 0x0050ba20 _$E344
// FUNCTION: STBC 0x0050ba30 _$E349
// FUNCTION: STBC 0x0050ba40 _$E348
// FUNCTION: STBC 0x0050ba50 _$E389
// FUNCTION: STBC 0x0050ba60 _$E388
// FUNCTION: STBC 0x0050ba70 _$E395
// FUNCTION: STBC 0x0050ba80 _$E394
// FUNCTION: STBC 0x0050ba90 _$E337
// FUNCTION: STBC 0x0050baa0 _$E336
// FUNCTION: STBC 0x0050bab0 _$E341
// FUNCTION: STBC 0x0050bac0 _$E340
// FUNCTION: STBC 0x0050bad0 _$E353
// FUNCTION: STBC 0x0050bae0 _$E352
// FUNCTION: STBC 0x0050baf0 _$E357
// FUNCTION: STBC 0x0050bb00 _$E356
// FUNCTION: STBC 0x0050bb10 _$E361
// FUNCTION: STBC 0x0050bb20 _$E360
// FUNCTION: STBC 0x0050bb30 _$E365
// FUNCTION: STBC 0x0050bb40 _$E364
// FUNCTION: STBC 0x0050bb50 _$E369
// FUNCTION: STBC 0x0050bb60 _$E368
// FUNCTION: STBC 0x0050bb70 _$E373
// FUNCTION: STBC 0x0050bb80 _$E372
// FUNCTION: STBC 0x0050bb90 _$E377
// FUNCTION: STBC 0x0050bba0 _$E376
// FUNCTION: STBC 0x0050bbb0 _$E381
// FUNCTION: STBC 0x0050bbc0 _$E380
// FUNCTION: STBC 0x0050bbd0 _$E89
// FUNCTION: STBC 0x0050bbe0 _$E88
// FUNCTION: STBC 0x0050bbf0 _$E385
// FUNCTION: STBC 0x0050bc00 _$E384
// FUNCTION: STBC 0x0050bc10 _$E94
// FUNCTION: STBC 0x0050bc20 _$E93
// FUNCTION: STBC 0x0050bc30 _$E399
// FUNCTION: STBC 0x0050bc40 _$E398
// FUNCTION: STBC 0x0050bc50 _$E403
// FUNCTION: STBC 0x0050bc60 _$E402
// FUNCTION: STBC 0x0050bc70 _$E407
// FUNCTION: STBC 0x0050bc80 _$E406
// FUNCTION: STBC 0x0050bc90 _$E411
// FUNCTION: STBC 0x0050bca0 _$E410
// FUNCTION: STBC 0x0050bcb0 _$E103
// FUNCTION: STBC 0x0050bcc0 _$E102
// FUNCTION: STBC 0x0050bcd0 _$E415
// FUNCTION: STBC 0x0050bce0 _$E414
// FUNCTION: STBC 0x0050bcf0 _$E419
// FUNCTION: STBC 0x0050bd00 _$E418
// FUNCTION: STBC 0x0050bd10 _$E423
// FUNCTION: STBC 0x0050bd20 _$E422
// FUNCTION: STBC 0x0050bd30 _$E109
// FUNCTION: STBC 0x0050bd40 _$E108
// FUNCTION: STBC 0x0050bd50 _$E114
// FUNCTION: STBC 0x0050bd60 _$E113
// FUNCTION: STBC 0x0050bd70 _$E144
// FUNCTION: STBC 0x0050bd80 _$E143
// FUNCTION: STBC 0x0050bd90 _$E427
// FUNCTION: STBC 0x0050bda0 _$E426
// FUNCTION: STBC 0x0050bdb0 _$E149
// FUNCTION: STBC 0x0050bdc0 _$E148
// FUNCTION: STBC 0x0050bdd0 _$E431
// FUNCTION: STBC 0x0050bde0 _$E430
// FUNCTION: STBC 0x0050bdf0 _$E435
// FUNCTION: STBC 0x0050be00 _$E434
// FUNCTION: STBC 0x0050be10 _$E154
// FUNCTION: STBC 0x0050be20 _$E153
// FUNCTION: STBC 0x0050be30 _$E439
// FUNCTION: STBC 0x0050be40 _$E438
// FUNCTION: STBC 0x0050be50 _$E119
// FUNCTION: STBC 0x0050be60 _$E118
// FUNCTION: STBC 0x0050bea0 _$E443
// FUNCTION: STBC 0x0050beb0 _$E442
// FUNCTION: STBC 0x0050bec0 _$E447
// FUNCTION: STBC 0x0050bed0 _$E446
// FUNCTION: STBC 0x0050bee0 _$E159
// FUNCTION: STBC 0x0050bef0 _$E158
// FUNCTION: STBC 0x0050bf00 _$E164
// FUNCTION: STBC 0x0050bf10 _$E163
// FUNCTION: STBC 0x0050bf20 _$E451
// FUNCTION: STBC 0x0050bf30 _$E450
// FUNCTION: STBC 0x0050bf40 _$E455
// FUNCTION: STBC 0x0050bf50 _$E454
// FUNCTION: STBC 0x0050bf60 _$E459
// FUNCTION: STBC 0x0050bf70 _$E458
// FUNCTION: STBC 0x0050bf80 _$E463
// FUNCTION: STBC 0x0050bf90 _$E462
// FUNCTION: STBC 0x0050bfa0 _$E467
// FUNCTION: STBC 0x0050bfb0 _$E466
// FUNCTION: STBC 0x0050bfc0 _$E471
// FUNCTION: STBC 0x0050bfd0 _$E470
// FUNCTION: STBC 0x0050bfe0 _$E475
// FUNCTION: STBC 0x0050bff0 _$E474
// FUNCTION: STBC 0x0050c000 _$E479
// FUNCTION: STBC 0x0050c010 _$E478
// FUNCTION: STBC 0x0050c020 _$E169
// FUNCTION: STBC 0x0050c030 _$E168
// FUNCTION: STBC 0x0050c040 _$E483
// FUNCTION: STBC 0x0050c050 _$E482
// FUNCTION: STBC 0x0050c060 _$E174
// FUNCTION: STBC 0x0050c070 _$E173
// FUNCTION: STBC 0x0050c080 _$E179
// FUNCTION: STBC 0x0050c090 _$E178
// FUNCTION: STBC 0x0050c0a0 _$E487
// FUNCTION: STBC 0x0050c0b0 _$E486
// FUNCTION: STBC 0x0050c0c0 _$E492
// FUNCTION: STBC 0x0050c0d0 _$E491
// FUNCTION: STBC 0x0050c0e0 _$E496
// FUNCTION: STBC 0x0050c0f0 _$E495

// The locale's ctype id initialiser and its exit-time destructor (<string>).
// FUNCTION: STBC 0x0050b1a0 _$E509
// FUNCTION: STBC 0x0050b180 _$E510

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0050b5d0 _$E298
// FUNCTION: STBC 0x0050b5f0 _$E299
// FUNCTION: STBC 0x0050b5e0 _$E300
// FUNCTION: STBC 0x0050b5c0 _$E301
// FUNCTION: STBC 0x0050b680 _$E304
// FUNCTION: STBC 0x0050b6a0 _$E305
// FUNCTION: STBC 0x0050b690 _$E306
// FUNCTION: STBC 0x0050b670 _$E307

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0050b220 _$E498
// FUNCTION: STBC 0x0050b240 _$E499
// FUNCTION: STBC 0x0050b230 _$E501
// FUNCTION: STBC 0x0050b210 _$E502

// DATA: STBC 0x00987688
TGEventHandlerTable TacticalWindow::classHandlers;

/*
================
TacticalWindow::Cast

The object as a TacticalWindow; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0050b260
TacticalWindow *TacticalWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TACTICALWINDOW_TYPE ) ) {
		return (TacticalWindow *)object;
	}
	return NULL;
}

/*
================
TacticalWindow::TacticalWindow
================
*/
// FUNCTION: STBC 0x0050b290
TacticalWindow::TacticalWindow( void ) : MainWindow( MWT_TACTICAL, 1.0f, 1.0f ) {
}

/*
================
TacticalWindow::GetObjType
================
*/
// FUNCTION: STBC 0x0050b2b0
int TacticalWindow::GetObjType( void ) {
	return TACTICALWINDOW_TYPE;
}

/*
================
TacticalWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x0050b2c0
bool TacticalWindow::IsTypeOf( int type ) {
	if ( type == TACTICALWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
TacticalWindow::GetClassName
================
*/
// FUNCTION: STBC 0x0050b320
char *TacticalWindow::GetClassName( void ) {
	return "TacticalWindow";
}

/*
================
TacticalWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0050b330
char *TacticalWindow::GetSwigTypeName( void ) {
	return "_p_TacticalWindow";
}

/*
================
TacticalWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0050b340
char *TacticalWindow::GetPythonClassName( void ) {
	return "TacticalWindowPtr";
}

/*
================
TacticalWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0050b350
TGEventHandlerTable *TacticalWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TacticalWindow::Initialize

Lets the scripts set up the tactical interface.
================
*/
// FUNCTION: STBC 0x0050b3a0
void TacticalWindow::Initialize( void ) {
	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "TacticalInterfaceHandlers", "Initialize", "", NULL, "(O)", self );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}
}

/*
================
TacticalWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0050b3f0
void TacticalWindow::RegisterHandlerNames( void ) {
}

/*
================
TacticalWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0050b400
void TacticalWindow::RegisterHandlers( void ) {
	AddPythonFuncHandlerForClass( &classHandlers, ET_MOUSE, "TacticalInterfaceHandlers.HandleMouse" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_KEYBOARD, "TacticalInterfaceHandlers.HandleKeyboard" );
}

/*
================
TacticalWindow::GotFocus

Renders the set the top window remembered (the player's set when there is
none), places the subtitles for the tactical view, shows the window, tells
the scripts, and takes the tactical control window in as the focused
child: shown, with its first child, unless a cutscene is playing.
================
*/
// FUNCTION: STBC 0x0050b430
void TacticalWindow::GotFocus( bool layout ) {
	TopWindow *top = TopWindow::GetTopWindow();
	TGUIObject::GotFocus( false );

	if ( Game::GetCurrentGame() != NULL ) {
		SetClass *playerSet = Game::GetCurrentGame()->GetPlayerSet();
		SetClass *set = top->GetLastRenderedSet();
		if ( set != NULL ) {
			g_kSetManager.MakeRenderedSet( set );
		} else if ( playerSet != NULL ) {
			g_kSetManager.MakeRenderedSet( playerSet );
		}
		top->SetLastRenderedSet( NULL );
	}

	SubtitleWindow *subtitles = (SubtitleWindow *)top->FindMainWindow( MWT_SUBTITLE );
	if ( subtitles == NULL ) {
		return;
	}
	MoveToFront( subtitles, false );
	subtitles->SetPositionForMode( SM_TACTICAL, false );
	SetVisible( layout );

	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "TacticalInterfaceHandlers", "GotFocus", "", NULL, "(Oi)", self, layout );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}

	ReticleManagerWindow *reticleManagerWindow = ReticleManagerWindow::GetReticleManagerWindow();
	if ( reticleManagerWindow == NULL ) {
		return;
	}
	reticleManagerWindow->Unknown514350();

	TacticalControlWindow *control = TacticalControlWindow::GetTacticalControlWindow();
	if ( control == NULL ) {
		return;
	}
	TGPane *controlParent = control->GetParent();
	if ( controlParent != NULL && controlParent != this ) {
		controlParent->RemoveChild( control, false );
	}
	if ( controlParent != this ) {
		AddChild( control, 0.0f, 0.0f, false );
	}
	if ( !top->IsCutsceneMode() ) {
		control->SetVisible( false );
		if ( control->GetFirstChild() != NULL ) {
			control->GetFirstChild()->SetVisible( false );
		}
	}
	if ( layout == true ) {
		control->Layout();
	}
	SetFocus( control, layout );
}

/*
================
TacticalWindow::LostFocus

Hides the window and tells the scripts.
================
*/
// FUNCTION: STBC 0x0050b600
void TacticalWindow::LostFocus( bool layout ) {
	TGUIObject::LostFocus( layout );
	SetNotVisible( layout );

	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "TacticalInterfaceHandlers", "LostFocus", "", NULL, "(Oi)", self, layout );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}
}

/*
================
TacticalWindow::SpaceCameraRotation

Turns the rendered set's active camera with the mouse: sideways about its up
axis, up and down about its right axis.
================
*/
// FUNCTION: STBC 0x0050b6b0
// PARKED: x87 schedule: retail loads the mouse delta (fild) before copying the axis and multiplies after; this build multiplies right after the load
void TacticalWindow::SpaceCameraRotation( void ) {
	int dx = g_kInputManager.GetMouseDeltaX();
	int dy = g_kInputManager.GetMouseDeltaY();
	if ( ( dx != 0 || dy != 0 ) && g_kSetManager.GetRenderedSet() != NULL ) {
		CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
		if ( camera != NULL ) {
			if ( dx != 0 ) {
				NiMatrix3 rotation;
				NiPoint3 axis;
				axis = camera->GetWorldUp();
				rotation.MakeRotation( dx * 0.00075f, axis );
				camera->Rotate( rotation );
			}
			if ( dy != 0 ) {
				NiMatrix3 rotation;
				NiPoint3 axis;
				axis = camera->GetWorldRight();
				rotation.MakeRotation( dy * 0.00075f, axis );
				camera->Rotate( rotation );
			}
		}
	}
}
