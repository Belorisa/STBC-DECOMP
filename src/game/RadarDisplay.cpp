/*
===============================================================================

	RadarDisplay.cpp

===============================================================================
*/

#include "RadarDisplay.h"
#include "Game.h"
#include "InterfaceModule.h"
#include "PyEmbed.h"
#include "RadarScope.h"
#include "STColors.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGMouseEvent.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00542da0 _$E479
// FUNCTION: STBC 0x00542dc0 _$E478

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005424e0 _$E33
// FUNCTION: STBC 0x005424f0 _$E32
// FUNCTION: STBC 0x005425c0 _$E37
// FUNCTION: STBC 0x005425d0 _$E36
// FUNCTION: STBC 0x00542660 _$E48
// FUNCTION: STBC 0x00542670 _$E47
// FUNCTION: STBC 0x00542680 _$E52
// FUNCTION: STBC 0x00542690 _$E51
// FUNCTION: STBC 0x005426a0 _$E56
// FUNCTION: STBC 0x005426b0 _$E55
// FUNCTION: STBC 0x005426c0 _$E60
// FUNCTION: STBC 0x005426d0 _$E59
// FUNCTION: STBC 0x005426e0 _$E184
// FUNCTION: STBC 0x005426f0 _$E183
// FUNCTION: STBC 0x00542700 _$E126
// FUNCTION: STBC 0x00542710 _$E125
// FUNCTION: STBC 0x00542720 _$E139
// FUNCTION: STBC 0x00542730 _$E138
// FUNCTION: STBC 0x00542740 _$E189
// FUNCTION: STBC 0x00542750 _$E188
// FUNCTION: STBC 0x00542760 _$E194
// FUNCTION: STBC 0x00542770 _$E193
// FUNCTION: STBC 0x00542780 _$E198
// FUNCTION: STBC 0x00542790 _$E197
// FUNCTION: STBC 0x005427a0 _$E202
// FUNCTION: STBC 0x005427b0 _$E201
// FUNCTION: STBC 0x005427c0 _$E209
// FUNCTION: STBC 0x005427d0 _$E208
// FUNCTION: STBC 0x005427e0 _$E298
// FUNCTION: STBC 0x005427f0 _$E297
// FUNCTION: STBC 0x00542800 _$E302
// FUNCTION: STBC 0x00542810 _$E301
// FUNCTION: STBC 0x00542820 _$E84
// FUNCTION: STBC 0x00542830 _$E83
// FUNCTION: STBC 0x00542840 _$E284
// FUNCTION: STBC 0x00542850 _$E283
// FUNCTION: STBC 0x00542860 _$E291
// FUNCTION: STBC 0x00542870 _$E290
// FUNCTION: STBC 0x00542880 _$E314
// FUNCTION: STBC 0x00542890 _$E313
// FUNCTION: STBC 0x005428a0 _$E318
// FUNCTION: STBC 0x005428b0 _$E317
// FUNCTION: STBC 0x005428c0 _$E358
// FUNCTION: STBC 0x005428d0 _$E357
// FUNCTION: STBC 0x005428e0 _$E364
// FUNCTION: STBC 0x005428f0 _$E363
// FUNCTION: STBC 0x00542900 _$E306
// FUNCTION: STBC 0x00542910 _$E305
// FUNCTION: STBC 0x00542920 _$E310
// FUNCTION: STBC 0x00542930 _$E309
// FUNCTION: STBC 0x00542940 _$E322
// FUNCTION: STBC 0x00542950 _$E321
// FUNCTION: STBC 0x00542960 _$E326
// FUNCTION: STBC 0x00542970 _$E325
// FUNCTION: STBC 0x00542980 _$E330
// FUNCTION: STBC 0x00542990 _$E329
// FUNCTION: STBC 0x005429a0 _$E334
// FUNCTION: STBC 0x005429b0 _$E333
// FUNCTION: STBC 0x005429c0 _$E338
// FUNCTION: STBC 0x005429d0 _$E337
// FUNCTION: STBC 0x005429e0 _$E342
// FUNCTION: STBC 0x005429f0 _$E341
// FUNCTION: STBC 0x00542a00 _$E346
// FUNCTION: STBC 0x00542a10 _$E345
// FUNCTION: STBC 0x00542a20 _$E350
// FUNCTION: STBC 0x00542a30 _$E349
// FUNCTION: STBC 0x00542a40 _$E89
// FUNCTION: STBC 0x00542a50 _$E88
// FUNCTION: STBC 0x00542a60 _$E354
// FUNCTION: STBC 0x00542a70 _$E353
// FUNCTION: STBC 0x00542a80 _$E94
// FUNCTION: STBC 0x00542a90 _$E93
// FUNCTION: STBC 0x00542aa0 _$E368
// FUNCTION: STBC 0x00542ab0 _$E367
// FUNCTION: STBC 0x00542ac0 _$E372
// FUNCTION: STBC 0x00542ad0 _$E371
// FUNCTION: STBC 0x00542ae0 _$E376
// FUNCTION: STBC 0x00542af0 _$E375
// FUNCTION: STBC 0x00542b00 _$E380
// FUNCTION: STBC 0x00542b10 _$E379
// FUNCTION: STBC 0x00542b20 _$E103
// FUNCTION: STBC 0x00542b30 _$E102
// FUNCTION: STBC 0x00542b40 _$E384
// FUNCTION: STBC 0x00542b50 _$E383
// FUNCTION: STBC 0x00542b60 _$E388
// FUNCTION: STBC 0x00542b70 _$E387
// FUNCTION: STBC 0x00542b80 _$E392
// FUNCTION: STBC 0x00542b90 _$E391
// FUNCTION: STBC 0x00542ba0 _$E109
// FUNCTION: STBC 0x00542bb0 _$E108
// FUNCTION: STBC 0x00542bc0 _$E114
// FUNCTION: STBC 0x00542bd0 _$E113
// FUNCTION: STBC 0x00542be0 _$E144
// FUNCTION: STBC 0x00542bf0 _$E143
// FUNCTION: STBC 0x00542c00 _$E396
// FUNCTION: STBC 0x00542c10 _$E395
// FUNCTION: STBC 0x00542c20 _$E149
// FUNCTION: STBC 0x00542c30 _$E148
// FUNCTION: STBC 0x00542c40 _$E400
// FUNCTION: STBC 0x00542c50 _$E399
// FUNCTION: STBC 0x00542c60 _$E404
// FUNCTION: STBC 0x00542c70 _$E403
// FUNCTION: STBC 0x00542c80 _$E154
// FUNCTION: STBC 0x00542c90 _$E153
// FUNCTION: STBC 0x00542ca0 _$E408
// FUNCTION: STBC 0x00542cb0 _$E407
// FUNCTION: STBC 0x00542cc0 _$E119
// FUNCTION: STBC 0x00542cd0 _$E118
// FUNCTION: STBC 0x00542ce0 _$E412
// FUNCTION: STBC 0x00542cf0 _$E411
// FUNCTION: STBC 0x00542d00 _$E416
// FUNCTION: STBC 0x00542d10 _$E415
// FUNCTION: STBC 0x00542d20 _$E159
// FUNCTION: STBC 0x00542d30 _$E158
// FUNCTION: STBC 0x00542d40 _$E164
// FUNCTION: STBC 0x00542d50 _$E163
// FUNCTION: STBC 0x00542d60 _$E420
// FUNCTION: STBC 0x00542d70 _$E419
// FUNCTION: STBC 0x00542d80 _$E424
// FUNCTION: STBC 0x00542d90 _$E423
// FUNCTION: STBC 0x00542dd0 _$E428
// FUNCTION: STBC 0x00542de0 _$E427
// FUNCTION: STBC 0x00542df0 _$E432
// FUNCTION: STBC 0x00542e00 _$E431
// FUNCTION: STBC 0x00542e10 _$E436
// FUNCTION: STBC 0x00542e20 _$E435
// FUNCTION: STBC 0x00542e30 _$E440
// FUNCTION: STBC 0x00542e40 _$E439
// FUNCTION: STBC 0x00542e50 _$E444
// FUNCTION: STBC 0x00542e60 _$E443
// FUNCTION: STBC 0x00542e70 _$E448
// FUNCTION: STBC 0x00542e80 _$E447
// FUNCTION: STBC 0x00542e90 _$E169
// FUNCTION: STBC 0x00542ea0 _$E168
// FUNCTION: STBC 0x00542eb0 _$E452
// FUNCTION: STBC 0x00542ec0 _$E451
// FUNCTION: STBC 0x00542ed0 _$E174
// FUNCTION: STBC 0x00542ee0 _$E173
// FUNCTION: STBC 0x00542ef0 _$E179
// FUNCTION: STBC 0x00542f00 _$E178
// FUNCTION: STBC 0x00542f10 _$E456
// FUNCTION: STBC 0x00542f20 _$E455
// FUNCTION: STBC 0x00542f30 _$E461
// FUNCTION: STBC 0x00542f40 _$E460
// FUNCTION: STBC 0x00542f50 _$E465
// FUNCTION: STBC 0x00542f60 _$E464

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00542210 _$E270
// FUNCTION: STBC 0x00542220 _$E267
// FUNCTION: STBC 0x00542230 _$E269
// FUNCTION: STBC 0x00542240 _$E268
// FUNCTION: STBC 0x00542250 _$E276
// FUNCTION: STBC 0x00542260 _$E273
// FUNCTION: STBC 0x00542270 _$E275
// FUNCTION: STBC 0x00542280 _$E274

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00542290 _$E471
// FUNCTION: STBC 0x005422a0 _$E467
// FUNCTION: STBC 0x005422b0 _$E470
// FUNCTION: STBC 0x005422c0 _$E468
// DATA: STBC 0x0098a608
TGEventHandlerTable RadarDisplay::classHandlers;

/*
================
RadarDisplay::Cast

The object as a RadarDisplay; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005422E0
RadarDisplay *RadarDisplay::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( RADARDISPLAY_TYPE ) ) {
		return (RadarDisplay *)object;
	}
	return NULL;
}

/*
================
RadarDisplay::RadarDisplay

A radar window of the given size; the scripts build the scope in it.
================
*/
// FUNCTION: STBC 0x00542310 ??0RadarDisplay@@QAE@MM@Z
RadarDisplay::RadarDisplay( float width, float height ) : STStylizedWindow( "StylizedWindow", "Radar", NULL, 0.0f, 0.0f, NULL, false, 0.0f, 0.0f, &g_kInterfaceBorderColor ) {
	Resize( width, height, false );
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.RadarDisplay", "Create", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
RadarDisplay::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005424c0
void RadarDisplay::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RadarDisplay::MouseHandler, "RadarDisplay::MouseHandler" );
}

/*
================
RadarDisplay::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00542500
void RadarDisplay::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "RadarDisplay::MouseHandler" );
}

/*
================
RadarDisplay::ResizeUI

Has the scripts redo the display's sizes.
================
*/
// FUNCTION: STBC 0x00542520
void RadarDisplay::ResizeUI( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.RadarDisplay", "ResizeUI", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
RadarDisplay::RepositionUI

Has the scripts redo the display's layout.
================
*/
// FUNCTION: STBC 0x00542560
void RadarDisplay::RepositionUI( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.RadarDisplay", "RepositionUI", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
RadarDisplay::MouseHandler

Lets the mouse events through to the scope's handlers, and takes them as
handled.
================
*/
// FUNCTION: STBC 0x005425a0
void RadarDisplay::MouseHandler( TGMouseEvent *event ) {
	CallNextHandler( event );
	event->SetHandled();
}

/*
================
RadarDisplay::Update

The scope, the first child, redraws.
================
*/
// FUNCTION: STBC 0x005425e0
void RadarDisplay::Update( void ) {
	( (RadarScope *)GetNthChild( 0 ) )->Update();
}

/*
================
RadarDisplay::SetHighlighted
================
*/
// FUNCTION: STBC 0x00542600
void RadarDisplay::SetHighlighted( bool layout ) {
	if ( !( flags & TGUIO_HIGHLIGHTED ) ) {
		STStylizedWindow::SetHighlighted( layout );
		SetColorBasedOnFlags();
		g_kInterfaceModule.playClickSound = true;
	}
}

/*
================
RadarDisplay::SetNotHighlighted
================
*/
// FUNCTION: STBC 0x00542630
void RadarDisplay::SetNotHighlighted( bool layout ) {
	if ( flags & TGUIO_HIGHLIGHTED ) {
		STStylizedWindow::SetNotHighlighted( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
RadarDisplay::SetColorBasedOnFlags

Empty in retail.
================
*/
// FUNCTION: STBC 0x00542650
void RadarDisplay::SetColorBasedOnFlags( void ) {
}
