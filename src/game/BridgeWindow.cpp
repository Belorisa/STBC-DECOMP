/*
===============================================================================

	BridgeWindow.cpp

===============================================================================
*/

#include <iostream>
#include "BridgeWindow.h"
#include "PyEmbed.h"
#include "SetManager.h"
#include "SubtitleWindow.h"
#include "TGEventHandlerTable.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TopWindow.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004fc120 _$E497
// FUNCTION: STBC 0x004fc140 _$E496

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004fba60 _$E32
// FUNCTION: STBC 0x004fba70 _$E29
// FUNCTION: STBC 0x004fba80 _$E31
// FUNCTION: STBC 0x004fba90 _$E30
// FUNCTION: STBC 0x004fbaa0 _$E38
// FUNCTION: STBC 0x004fbab0 _$E35
// FUNCTION: STBC 0x004fbac0 _$E37
// FUNCTION: STBC 0x004fbad0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004fbae0 _$E57
// FUNCTION: STBC 0x004fbaf0 _$E56
// FUNCTION: STBC 0x004fbb00 _$E61
// FUNCTION: STBC 0x004fbb10 _$E60
// FUNCTION: STBC 0x004fbb20 _$E72
// FUNCTION: STBC 0x004fbb30 _$E71
// FUNCTION: STBC 0x004fbb40 _$E76
// FUNCTION: STBC 0x004fbb50 _$E75
// FUNCTION: STBC 0x004fbb60 _$E80
// FUNCTION: STBC 0x004fbb70 _$E79
// FUNCTION: STBC 0x004fbb80 _$E84
// FUNCTION: STBC 0x004fbb90 _$E83
// FUNCTION: STBC 0x004fbba0 _$E208
// FUNCTION: STBC 0x004fbbb0 _$E207
// FUNCTION: STBC 0x004fbbc0 _$E150
// FUNCTION: STBC 0x004fbbd0 _$E149
// FUNCTION: STBC 0x004fbbe0 _$E163
// FUNCTION: STBC 0x004fbbf0 _$E162
// FUNCTION: STBC 0x004fbc00 _$E213
// FUNCTION: STBC 0x004fbc10 _$E212
// FUNCTION: STBC 0x004fbc20 _$E218
// FUNCTION: STBC 0x004fbc30 _$E217
// FUNCTION: STBC 0x004fbc40 _$E222
// FUNCTION: STBC 0x004fbc50 _$E221
// FUNCTION: STBC 0x004fbc60 _$E226
// FUNCTION: STBC 0x004fbc70 _$E225
// FUNCTION: STBC 0x004fbc80 _$E233
// FUNCTION: STBC 0x004fbc90 _$E232
// FUNCTION: STBC 0x004fbca0 _$E316
// FUNCTION: STBC 0x004fbcb0 _$E315
// FUNCTION: STBC 0x004fbcc0 _$E320
// FUNCTION: STBC 0x004fbcd0 _$E319
// FUNCTION: STBC 0x004fbce0 _$E108
// FUNCTION: STBC 0x004fbcf0 _$E107
// FUNCTION: STBC 0x004fbd00 _$E302
// FUNCTION: STBC 0x004fbd10 _$E301
// FUNCTION: STBC 0x004fbd20 _$E309
// FUNCTION: STBC 0x004fbd30 _$E308
// FUNCTION: STBC 0x004fbd40 _$E332
// FUNCTION: STBC 0x004fbd50 _$E331
// FUNCTION: STBC 0x004fbd60 _$E336
// FUNCTION: STBC 0x004fbd70 _$E335
// FUNCTION: STBC 0x004fbd80 _$E376
// FUNCTION: STBC 0x004fbd90 _$E375
// FUNCTION: STBC 0x004fbda0 _$E382
// FUNCTION: STBC 0x004fbdb0 _$E381
// FUNCTION: STBC 0x004fbdc0 _$E324
// FUNCTION: STBC 0x004fbdd0 _$E323
// FUNCTION: STBC 0x004fbde0 _$E328
// FUNCTION: STBC 0x004fbdf0 _$E327
// FUNCTION: STBC 0x004fbe00 _$E340
// FUNCTION: STBC 0x004fbe10 _$E339
// FUNCTION: STBC 0x004fbe20 _$E344
// FUNCTION: STBC 0x004fbe30 _$E343
// FUNCTION: STBC 0x004fbe40 _$E348
// FUNCTION: STBC 0x004fbe50 _$E347
// FUNCTION: STBC 0x004fbe60 _$E352
// FUNCTION: STBC 0x004fbe70 _$E351
// FUNCTION: STBC 0x004fbe80 _$E356
// FUNCTION: STBC 0x004fbe90 _$E355
// FUNCTION: STBC 0x004fbea0 _$E360
// FUNCTION: STBC 0x004fbeb0 _$E359
// FUNCTION: STBC 0x004fbec0 _$E364
// FUNCTION: STBC 0x004fbed0 _$E363
// FUNCTION: STBC 0x004fbee0 _$E368
// FUNCTION: STBC 0x004fbef0 _$E367
// FUNCTION: STBC 0x004fbf00 _$E113
// FUNCTION: STBC 0x004fbf10 _$E112
// FUNCTION: STBC 0x004fbf20 _$E372
// FUNCTION: STBC 0x004fbf30 _$E371
// FUNCTION: STBC 0x004fbf40 _$E118
// FUNCTION: STBC 0x004fbf50 _$E117
// FUNCTION: STBC 0x004fbf60 _$E386
// FUNCTION: STBC 0x004fbf70 _$E385
// FUNCTION: STBC 0x004fbf80 _$E390
// FUNCTION: STBC 0x004fbf90 _$E389
// FUNCTION: STBC 0x004fbfa0 _$E394
// FUNCTION: STBC 0x004fbfb0 _$E393
// FUNCTION: STBC 0x004fbfc0 _$E398
// FUNCTION: STBC 0x004fbfd0 _$E397
// FUNCTION: STBC 0x004fbfe0 _$E127
// FUNCTION: STBC 0x004fbff0 _$E126
// FUNCTION: STBC 0x004fc000 _$E402
// FUNCTION: STBC 0x004fc010 _$E401
// FUNCTION: STBC 0x004fc020 _$E406
// FUNCTION: STBC 0x004fc030 _$E405
// FUNCTION: STBC 0x004fc040 _$E410
// FUNCTION: STBC 0x004fc050 _$E409
// FUNCTION: STBC 0x004fc060 _$E133
// FUNCTION: STBC 0x004fc070 _$E132
// FUNCTION: STBC 0x004fc080 _$E138
// FUNCTION: STBC 0x004fc090 _$E137
// FUNCTION: STBC 0x004fc0a0 _$E168
// FUNCTION: STBC 0x004fc0b0 _$E167
// FUNCTION: STBC 0x004fc0c0 _$E414
// FUNCTION: STBC 0x004fc0d0 _$E413
// FUNCTION: STBC 0x004fc0e0 _$E173
// FUNCTION: STBC 0x004fc0f0 _$E172
// FUNCTION: STBC 0x004fc100 _$E418
// FUNCTION: STBC 0x004fc110 _$E417
// FUNCTION: STBC 0x004fc150 _$E422
// FUNCTION: STBC 0x004fc160 _$E421
// FUNCTION: STBC 0x004fc170 _$E178
// FUNCTION: STBC 0x004fc180 _$E177
// FUNCTION: STBC 0x004fc190 _$E426
// FUNCTION: STBC 0x004fc1a0 _$E425
// FUNCTION: STBC 0x004fc1b0 _$E143
// FUNCTION: STBC 0x004fc1c0 _$E142
// FUNCTION: STBC 0x004fc1d0 _$E430
// FUNCTION: STBC 0x004fc1e0 _$E429
// FUNCTION: STBC 0x004fc1f0 _$E434
// FUNCTION: STBC 0x004fc200 _$E433
// FUNCTION: STBC 0x004fc210 _$E183
// FUNCTION: STBC 0x004fc220 _$E182
// FUNCTION: STBC 0x004fc230 _$E188
// FUNCTION: STBC 0x004fc240 _$E187
// FUNCTION: STBC 0x004fc250 _$E438
// FUNCTION: STBC 0x004fc260 _$E437
// FUNCTION: STBC 0x004fc270 _$E442
// FUNCTION: STBC 0x004fc280 _$E441
// FUNCTION: STBC 0x004fc290 _$E446
// FUNCTION: STBC 0x004fc2a0 _$E445
// FUNCTION: STBC 0x004fc2b0 _$E450
// FUNCTION: STBC 0x004fc2c0 _$E449
// FUNCTION: STBC 0x004fc2d0 _$E454
// FUNCTION: STBC 0x004fc2e0 _$E453
// FUNCTION: STBC 0x004fc2f0 _$E458
// FUNCTION: STBC 0x004fc300 _$E457
// FUNCTION: STBC 0x004fc310 _$E462
// FUNCTION: STBC 0x004fc320 _$E461
// FUNCTION: STBC 0x004fc330 _$E466
// FUNCTION: STBC 0x004fc340 _$E465
// FUNCTION: STBC 0x004fc350 _$E193
// FUNCTION: STBC 0x004fc360 _$E192
// FUNCTION: STBC 0x004fc370 _$E470
// FUNCTION: STBC 0x004fc380 _$E469
// FUNCTION: STBC 0x004fc390 _$E198
// FUNCTION: STBC 0x004fc3a0 _$E197
// FUNCTION: STBC 0x004fc3b0 _$E203
// FUNCTION: STBC 0x004fc3c0 _$E202
// FUNCTION: STBC 0x004fc3d0 _$E474
// FUNCTION: STBC 0x004fc3e0 _$E473
// FUNCTION: STBC 0x004fc3f0 _$E479
// FUNCTION: STBC 0x004fc400 _$E478
// FUNCTION: STBC 0x004fc410 _$E483
// FUNCTION: STBC 0x004fc420 _$E482

// The handler table's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004fb6d0 _$E489
// FUNCTION: STBC 0x004fb6e0 _$E485
// FUNCTION: STBC 0x004fb6f0 _$E488
// FUNCTION: STBC 0x004fb700 _$E486
// DATA: STBC 0x00986c38
TGEventHandlerTable BridgeWindow::classHandlers;

/*
================
BridgeWindow::Cast

The object as a BridgeWindow; NULL when it is none.
================
*/
// FUNCTION: STBC 0x004fb720
BridgeWindow *BridgeWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( BRIDGEWINDOW_TYPE ) ) {
		return (BridgeWindow *)object;
	}
	return NULL;
}

/*
================
BridgeWindow::BridgeWindow
================
*/
// FUNCTION: STBC 0x004fb750
BridgeWindow::BridgeWindow( void ) : MainWindow( MWT_BRIDGE, 1.0f, 1.0f ) {
}

/*
================
BridgeWindow::GetObjType
================
*/
// FUNCTION: STBC 0x004fb780
int BridgeWindow::GetObjType( void ) {
	return BRIDGEWINDOW_TYPE;
}

/*
================
BridgeWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x004fb790
bool BridgeWindow::IsTypeOf( int type ) {
	if ( type == BRIDGEWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
BridgeWindow::GetClassName
================
*/
// FUNCTION: STBC 0x004fb7f0
char *BridgeWindow::GetClassName( void ) {
	return "BridgeWindow";
}

/*
================
BridgeWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004fb800
char *BridgeWindow::GetSwigTypeName( void ) {
	return "_p_BridgeWindow";
}

/*
================
BridgeWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004fb810
char *BridgeWindow::GetPythonClassName( void ) {
	return "BridgeWindowPtr";
}

/*
================
BridgeWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004fb820
TGEventHandlerTable *BridgeWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
BridgeWindow::Initialize

Lets the scripts build the bridge menus, then set up their handlers. The
error print runs whether or not a call failed.
================
*/
// FUNCTION: STBC 0x004fb870
void BridgeWindow::Initialize( void ) {
	PyObject *self = VirtualSlot08();
	// BUG: self is never released when building the menus fails
	if ( PyEmbed::CallFunction( "Bridge.BridgeMenus", "Initialize", "", NULL, "(O)", self ) != -1 ) {
		PyEmbed::CallFunction( "BridgeHandlers", "Initialize", "", NULL, "(O)", self );
		Py_DECREF( self );
	}
	PyErr_Print();
}

/*
================
BridgeWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x004fb8e0
void BridgeWindow::RegisterHandlerNames( void ) {
}

/*
================
BridgeWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x004fb8f0
void BridgeWindow::RegisterHandlers( void ) {
	AddPythonFuncHandlerForClass( &classHandlers, ET_MOUSE, "BridgeHandlers.HandleMouse" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_KEYBOARD, "BridgeHandlers.HandleKeyboard" );
}

/*
================
BridgeWindow::GotFocus

Renders the set the top window remembered (the bridge when there is
none), brings the subtitles to the front placed for the bridge, shows
the window and tells the scripts.
================
*/
// FUNCTION: STBC 0x004fb920
void BridgeWindow::GotFocus( bool layout ) {
	TopWindow *top = TopWindow::GetTopWindow();
	SetClass *set = top->GetLastRenderedSet();
	if ( set != NULL ) {
		g_kSetManager.MakeRenderedSet( set );
	} else {
		g_kSetManager.MakeRenderedSet( "bridge" );
	}
	top->SetLastRenderedSet( NULL );

	SubtitleWindow *subtitles = (SubtitleWindow *)top->FindMainWindow( MWT_SUBTITLE );
	if ( subtitles != NULL ) {
		MoveToFront( subtitles, true );
		subtitles->SetPositionForMode( SM_BRIDGE, false );
		SetVisible( layout );

		PyObject *self = VirtualSlot08();
		int status = PyEmbed::CallFunction( "BridgeHandlers", "GotFocus", "", NULL, "(Oi)", self, layout );
		Py_DECREF( self );
		if ( status == -1 ) {
			PyErr_Print();
		}
	}
}

/*
================
BridgeWindow::LostFocus

Hides the window and tells the scripts.
================
*/
// FUNCTION: STBC 0x004fb9f0
void BridgeWindow::LostFocus( bool layout ) {
	SetNotVisible( false );

	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "BridgeHandlers", "LostFocus", "", NULL, "(Oi)", self, layout );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}
}
