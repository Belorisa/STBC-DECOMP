/*
===============================================================================

	STCharacterMenu.cpp

===============================================================================
*/

#include "STCharacterMenu.h"
#include "STButton.h"
#include "TopWindow.h"
#include "TGBoolEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0051bb90 _$E457
// FUNCTION: STBC 0x0051bbb0 _$E456

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0051b710 _$E33
// FUNCTION: STBC 0x0051b720 _$E32
// FUNCTION: STBC 0x0051b730 _$E37
// FUNCTION: STBC 0x0051b740 _$E36
// FUNCTION: STBC 0x0051b750 _$E48
// FUNCTION: STBC 0x0051b760 _$E47
// FUNCTION: STBC 0x0051b770 _$E52
// FUNCTION: STBC 0x0051b780 _$E51
// FUNCTION: STBC 0x0051b790 _$E56
// FUNCTION: STBC 0x0051b7a0 _$E55
// FUNCTION: STBC 0x0051b7b0 _$E60
// FUNCTION: STBC 0x0051b7c0 _$E59
// FUNCTION: STBC 0x0051b7d0 _$E184
// FUNCTION: STBC 0x0051b7e0 _$E183
// FUNCTION: STBC 0x0051b7f0 _$E126
// FUNCTION: STBC 0x0051b800 _$E125
// FUNCTION: STBC 0x0051b810 _$E139
// FUNCTION: STBC 0x0051b820 _$E138
// FUNCTION: STBC 0x0051b830 _$E189
// FUNCTION: STBC 0x0051b840 _$E188
// FUNCTION: STBC 0x0051b850 _$E194
// FUNCTION: STBC 0x0051b860 _$E193
// FUNCTION: STBC 0x0051b870 _$E198
// FUNCTION: STBC 0x0051b880 _$E197
// FUNCTION: STBC 0x0051b890 _$E202
// FUNCTION: STBC 0x0051b8a0 _$E201
// FUNCTION: STBC 0x0051b8b0 _$E209
// FUNCTION: STBC 0x0051b8c0 _$E208
// FUNCTION: STBC 0x0051b8d0 _$E289
// FUNCTION: STBC 0x0051b8e0 _$E288
// FUNCTION: STBC 0x0051b8f0 _$E293
// FUNCTION: STBC 0x0051b900 _$E292
// FUNCTION: STBC 0x0051b910 _$E84
// FUNCTION: STBC 0x0051b920 _$E83
// FUNCTION: STBC 0x0051b930 _$E275
// FUNCTION: STBC 0x0051b940 _$E274
// FUNCTION: STBC 0x0051b950 _$E282
// FUNCTION: STBC 0x0051b960 _$E281
// FUNCTION: STBC 0x0051b970 _$E305
// FUNCTION: STBC 0x0051b980 _$E304
// FUNCTION: STBC 0x0051b990 _$E309
// FUNCTION: STBC 0x0051b9a0 _$E308
// FUNCTION: STBC 0x0051b9b0 _$E349
// FUNCTION: STBC 0x0051b9c0 _$E348
// FUNCTION: STBC 0x0051b9d0 _$E355
// FUNCTION: STBC 0x0051b9e0 _$E354
// FUNCTION: STBC 0x0051b9f0 _$E297
// FUNCTION: STBC 0x0051ba00 _$E296
// FUNCTION: STBC 0x0051ba10 _$E301
// FUNCTION: STBC 0x0051ba20 _$E300
// FUNCTION: STBC 0x0051ba30 _$E313
// FUNCTION: STBC 0x0051ba40 _$E312
// FUNCTION: STBC 0x0051ba50 _$E317
// FUNCTION: STBC 0x0051ba60 _$E316
// FUNCTION: STBC 0x0051ba70 _$E321
// FUNCTION: STBC 0x0051ba80 _$E320
// FUNCTION: STBC 0x0051ba90 _$E325
// FUNCTION: STBC 0x0051baa0 _$E324
// FUNCTION: STBC 0x0051bab0 _$E329
// FUNCTION: STBC 0x0051bac0 _$E328
// FUNCTION: STBC 0x0051bad0 _$E333
// FUNCTION: STBC 0x0051bae0 _$E332
// FUNCTION: STBC 0x0051baf0 _$E337
// FUNCTION: STBC 0x0051bb00 _$E336
// FUNCTION: STBC 0x0051bb10 _$E341
// FUNCTION: STBC 0x0051bb20 _$E340
// FUNCTION: STBC 0x0051bb30 _$E89
// FUNCTION: STBC 0x0051bb40 _$E88
// FUNCTION: STBC 0x0051bb50 _$E345
// FUNCTION: STBC 0x0051bb60 _$E344
// FUNCTION: STBC 0x0051bb70 _$E94
// FUNCTION: STBC 0x0051bb80 _$E93
// FUNCTION: STBC 0x0051bbc0 _$E359
// FUNCTION: STBC 0x0051bbd0 _$E358
// FUNCTION: STBC 0x0051bbe0 _$E363
// FUNCTION: STBC 0x0051bbf0 _$E362
// FUNCTION: STBC 0x0051bc00 _$E367
// FUNCTION: STBC 0x0051bc10 _$E366
// FUNCTION: STBC 0x0051bc20 _$E371
// FUNCTION: STBC 0x0051bc30 _$E370
// FUNCTION: STBC 0x0051bc40 _$E103
// FUNCTION: STBC 0x0051bc50 _$E102
// FUNCTION: STBC 0x0051bc60 _$E375
// FUNCTION: STBC 0x0051bc70 _$E374
// FUNCTION: STBC 0x0051bc80 _$E379
// FUNCTION: STBC 0x0051bc90 _$E378
// FUNCTION: STBC 0x0051bca0 _$E383
// FUNCTION: STBC 0x0051bcb0 _$E382
// FUNCTION: STBC 0x0051bcc0 _$E109
// FUNCTION: STBC 0x0051bcd0 _$E108
// FUNCTION: STBC 0x0051bce0 _$E114
// FUNCTION: STBC 0x0051bcf0 _$E113
// FUNCTION: STBC 0x0051bd00 _$E144
// FUNCTION: STBC 0x0051bd10 _$E143
// FUNCTION: STBC 0x0051bd20 _$E387
// FUNCTION: STBC 0x0051bd30 _$E386
// FUNCTION: STBC 0x0051bd40 _$E149
// FUNCTION: STBC 0x0051bd50 _$E148
// FUNCTION: STBC 0x0051bd60 _$E391
// FUNCTION: STBC 0x0051bd70 _$E390
// FUNCTION: STBC 0x0051bd80 _$E395
// FUNCTION: STBC 0x0051bd90 _$E394
// FUNCTION: STBC 0x0051bda0 _$E154
// FUNCTION: STBC 0x0051bdb0 _$E153
// FUNCTION: STBC 0x0051bdc0 _$E399
// FUNCTION: STBC 0x0051bdd0 _$E398
// FUNCTION: STBC 0x0051bde0 _$E119
// FUNCTION: STBC 0x0051bdf0 _$E118
// FUNCTION: STBC 0x0051be00 _$E403
// FUNCTION: STBC 0x0051be10 _$E402
// FUNCTION: STBC 0x0051be20 _$E407
// FUNCTION: STBC 0x0051be30 _$E406
// FUNCTION: STBC 0x0051be40 _$E159
// FUNCTION: STBC 0x0051be50 _$E158
// FUNCTION: STBC 0x0051be60 _$E164
// FUNCTION: STBC 0x0051be70 _$E163
// FUNCTION: STBC 0x0051be80 _$E411
// FUNCTION: STBC 0x0051be90 _$E410
// FUNCTION: STBC 0x0051bea0 _$E415
// FUNCTION: STBC 0x0051beb0 _$E414
// FUNCTION: STBC 0x0051bec0 _$E419
// FUNCTION: STBC 0x0051bed0 _$E418
// FUNCTION: STBC 0x0051bee0 _$E423
// FUNCTION: STBC 0x0051bef0 _$E422
// FUNCTION: STBC 0x0051bf00 _$E427
// FUNCTION: STBC 0x0051bf10 _$E426
// FUNCTION: STBC 0x0051bf20 _$E431
// FUNCTION: STBC 0x0051bf30 _$E430
// FUNCTION: STBC 0x0051bf40 _$E435
// FUNCTION: STBC 0x0051bf50 _$E434
// FUNCTION: STBC 0x0051bf60 _$E439
// FUNCTION: STBC 0x0051bf70 _$E438
// FUNCTION: STBC 0x0051bf80 _$E169
// FUNCTION: STBC 0x0051bf90 _$E168
// FUNCTION: STBC 0x0051bfa0 _$E443
// FUNCTION: STBC 0x0051bfb0 _$E442
// FUNCTION: STBC 0x0051bfc0 _$E174
// FUNCTION: STBC 0x0051bfd0 _$E173
// FUNCTION: STBC 0x0051bfe0 _$E179
// FUNCTION: STBC 0x0051bff0 _$E178
// FUNCTION: STBC 0x0051c000 _$E247
// FUNCTION: STBC 0x0051c010 _$E246
// FUNCTION: STBC 0x0051c020 _$E252
// FUNCTION: STBC 0x0051c030 _$E251
// FUNCTION: STBC 0x0051c040 _$E256
// FUNCTION: STBC 0x0051c050 _$E255

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0051b690 _$E261
// FUNCTION: STBC 0x0051b6a0 _$E258
// FUNCTION: STBC 0x0051b6b0 _$E260
// FUNCTION: STBC 0x0051b6c0 _$E259
// FUNCTION: STBC 0x0051b6d0 _$E267
// FUNCTION: STBC 0x0051b6e0 _$E264
// FUNCTION: STBC 0x0051b6f0 _$E266
// FUNCTION: STBC 0x0051b700 _$E265

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0051b450 _$E449
// FUNCTION: STBC 0x0051b460 _$E445
// FUNCTION: STBC 0x0051b470 _$E448
// FUNCTION: STBC 0x0051b480 _$E446
// DATA: STBC 0x00988198
TGEventHandlerTable STCharacterMenu::classHandlers;

/*
================
STCharacterMenu::STCharacterMenu

A menu with the default flags. The colours are not used.
================
*/
// FUNCTION: STBC 0x0051b4a0 ??0STCharacterMenu@@QAE@PADPAVNiColorA@@1@Z
STCharacterMenu::STCharacterMenu( char *name, NiColorA *color1, NiColorA *color2 )
	: STMenu( name, STMF_DEFAULTS, NULL, NULL ) {
}

/*
================
STCharacterMenu::STCharacterMenu
================
*/
// FUNCTION: STBC 0x0051b5c0 ??0STCharacterMenu@@QAE@PAVTGString@@PAVNiColorA@@1@Z
STCharacterMenu::STCharacterMenu( TGString *name, NiColorA *color1, NiColorA *color2 )
	: STMenu( name, STMF_DEFAULTS, NULL, NULL ) {
}

/*
================
STCharacterMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0051b5f0
void STCharacterMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STCharacterMenu::ButtonClickedHandler, "STCharacterMenu::ButtonClickedHandler" );
}

/*
================
STCharacterMenu::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0051b610
void STCharacterMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_ST_BUTTON_CLICKED, "STCharacterMenu::ButtonClickedHandler" );
}

/*
================
STCharacterMenu::ButtonClickedHandler

Unselects the menu and passes a button's click up its parents, below
the top window, until one handles it; the menu lays itself out when
none did. The click counts as handled.
================
*/
// FUNCTION: STBC 0x0051b630
void STCharacterMenu::ButtonClickedHandler( TGBoolEvent *event ) {
	SetNotSelected( false );
	for ( TGPane *pane = parent; pane != NULL && pane != TopWindow::GetTopWindow(); pane = pane->GetParent() ) {
		event->SetDestination( pane );
		pane->ProcessEvent( event );
		if ( event->GetBool() ) {
			break;
		}
	}
	if ( !event->GetBool() ) {
		Layout();
	}
	event->SetBool( true );
}
