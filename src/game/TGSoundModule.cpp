/*
===============================================================================

	TGSoundModule.cpp

===============================================================================
*/

#include "NiApplication.h"
#include "NiAppWindow.h"

// windows.h renames GetClassName, a TGObject member, to GetClassNameA.
#undef GetClassName

#include "TGMusic.h"
#include "TGMusicFadeEvent.h"
#include "TGEventHandlerTable.h"
#include "TGObject.h"
#include "TGRedbookClass.h"
#include "TGSoundManager.h"
#include "TGSoundModule.h"
#include "TGSoundRegion.h"
#include <iostream>

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).
// FUNCTION: STBC 0x00709a40 _$E237
// FUNCTION: STBC 0x00709a50 _$E234
// FUNCTION: STBC 0x00709a60 _$E236
// FUNCTION: STBC 0x00709a70 _$E235
// FUNCTION: STBC 0x00709a80 _$E243
// FUNCTION: STBC 0x00709a90 _$E240
// FUNCTION: STBC 0x00709aa0 _$E242
// FUNCTION: STBC 0x00709ab0 _$E241

// The sound module's global: its dynamic initialiser and exit-time destructor, after the locale id's guard and registration.
// FUNCTION: STBC 0x00709640 _$E448
// FUNCTION: STBC 0x00709660 _$E447
// FUNCTION: STBC 0x00709670 _$E440
// FUNCTION: STBC 0x00709680 _$E437
// FUNCTION: STBC 0x00709690 _$E439
// FUNCTION: STBC 0x007096a0 _$E438
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00709ac0 _$E25
// FUNCTION: STBC 0x00709ad0 _$E24
// FUNCTION: STBC 0x00709ae0 _$E181
// FUNCTION: STBC 0x00709af0 _$E180
// FUNCTION: STBC 0x00709b00 _$E36
// FUNCTION: STBC 0x00709b10 _$E35
// FUNCTION: STBC 0x00709b20 _$E40
// FUNCTION: STBC 0x00709b30 _$E39
// FUNCTION: STBC 0x00709b40 _$E44
// FUNCTION: STBC 0x00709b50 _$E43
// FUNCTION: STBC 0x00709b60 _$E48
// FUNCTION: STBC 0x00709b70 _$E47
// FUNCTION: STBC 0x00709b80 _$E168
// FUNCTION: STBC 0x00709b90 _$E167
// FUNCTION: STBC 0x00709ba0 _$E110
// FUNCTION: STBC 0x00709bb0 _$E109
// FUNCTION: STBC 0x00709bc0 _$E123
// FUNCTION: STBC 0x00709bd0 _$E122
// FUNCTION: STBC 0x00709be0 _$E173
// FUNCTION: STBC 0x00709bf0 _$E172
// FUNCTION: STBC 0x00709c30 _$E186
// FUNCTION: STBC 0x00709c40 _$E185
// FUNCTION: STBC 0x00709c50 _$E177
// FUNCTION: STBC 0x00709c60 _$E176
// FUNCTION: STBC 0x00709c70 _$E190
// FUNCTION: STBC 0x00709c80 _$E189
// FUNCTION: STBC 0x00709c90 _$E197
// FUNCTION: STBC 0x00709ca0 _$E196
// FUNCTION: STBC 0x00709cb0 _$E268
// FUNCTION: STBC 0x00709cc0 _$E267
// FUNCTION: STBC 0x00709cd0 _$E272
// FUNCTION: STBC 0x00709ce0 _$E271
// FUNCTION: STBC 0x00709cf0 _$E72
// FUNCTION: STBC 0x00709d00 _$E71
// FUNCTION: STBC 0x00709d10 _$E254
// FUNCTION: STBC 0x00709d20 _$E253
// FUNCTION: STBC 0x00709d30 _$E261
// FUNCTION: STBC 0x00709d40 _$E260
// FUNCTION: STBC 0x00709d50 _$E284
// FUNCTION: STBC 0x00709d60 _$E283
// FUNCTION: STBC 0x00709d70 _$E288
// FUNCTION: STBC 0x00709d80 _$E287
// FUNCTION: STBC 0x00709d90 _$E328
// FUNCTION: STBC 0x00709da0 _$E327
// FUNCTION: STBC 0x00709db0 _$E334
// FUNCTION: STBC 0x00709dc0 _$E333
// FUNCTION: STBC 0x00709dd0 _$E276
// FUNCTION: STBC 0x00709de0 _$E275
// FUNCTION: STBC 0x00709df0 _$E280
// FUNCTION: STBC 0x00709e00 _$E279
// FUNCTION: STBC 0x00709e10 _$E292
// FUNCTION: STBC 0x00709e20 _$E291
// FUNCTION: STBC 0x00709e30 _$E296
// FUNCTION: STBC 0x00709e40 _$E295
// FUNCTION: STBC 0x00709e50 _$E300
// FUNCTION: STBC 0x00709e60 _$E299
// FUNCTION: STBC 0x00709e70 _$E304
// FUNCTION: STBC 0x00709e80 _$E303
// FUNCTION: STBC 0x00709e90 _$E308
// FUNCTION: STBC 0x00709ea0 _$E307
// FUNCTION: STBC 0x00709eb0 _$E312
// FUNCTION: STBC 0x00709ec0 _$E311
// FUNCTION: STBC 0x00709ed0 _$E316
// FUNCTION: STBC 0x00709ee0 _$E315
// FUNCTION: STBC 0x00709ef0 _$E320
// FUNCTION: STBC 0x00709f00 _$E319
// FUNCTION: STBC 0x00709f10 _$E77
// FUNCTION: STBC 0x00709f20 _$E76
// FUNCTION: STBC 0x00709f30 _$E324
// FUNCTION: STBC 0x00709f40 _$E323
// FUNCTION: STBC 0x00709f50 _$E82
// FUNCTION: STBC 0x00709f60 _$E81
// FUNCTION: STBC 0x00709f70 _$E338
// FUNCTION: STBC 0x00709f80 _$E337
// FUNCTION: STBC 0x00709f90 _$E342
// FUNCTION: STBC 0x00709fa0 _$E341
// FUNCTION: STBC 0x00709fb0 _$E346
// FUNCTION: STBC 0x00709fc0 _$E345
// FUNCTION: STBC 0x00709fd0 _$E350
// FUNCTION: STBC 0x00709fe0 _$E349
// FUNCTION: STBC 0x00709ff0 _$E87
// FUNCTION: STBC 0x0070a000 _$E86
// FUNCTION: STBC 0x0070a010 _$E354
// FUNCTION: STBC 0x0070a020 _$E353
// FUNCTION: STBC 0x0070a030 _$E358
// FUNCTION: STBC 0x0070a040 _$E357
// FUNCTION: STBC 0x0070a050 _$E362
// FUNCTION: STBC 0x0070a060 _$E361
// FUNCTION: STBC 0x0070a070 _$E93
// FUNCTION: STBC 0x0070a080 _$E92
// FUNCTION: STBC 0x0070a090 _$E98
// FUNCTION: STBC 0x0070a0a0 _$E97
// FUNCTION: STBC 0x0070a0b0 _$E128
// FUNCTION: STBC 0x0070a0c0 _$E127
// FUNCTION: STBC 0x0070a0d0 _$E366
// FUNCTION: STBC 0x0070a0e0 _$E365
// FUNCTION: STBC 0x0070a0f0 _$E133
// FUNCTION: STBC 0x0070a100 _$E132
// FUNCTION: STBC 0x0070a110 _$E370
// FUNCTION: STBC 0x0070a120 _$E369
// FUNCTION: STBC 0x0070a130 _$E374
// FUNCTION: STBC 0x0070a140 _$E373
// FUNCTION: STBC 0x0070a150 _$E138
// FUNCTION: STBC 0x0070a160 _$E137
// FUNCTION: STBC 0x0070a170 _$E378
// FUNCTION: STBC 0x0070a180 _$E377
// FUNCTION: STBC 0x0070a190 _$E103
// FUNCTION: STBC 0x0070a1a0 _$E102
// FUNCTION: STBC 0x0070a1b0 _$E382
// FUNCTION: STBC 0x0070a1c0 _$E381
// FUNCTION: STBC 0x0070a1d0 _$E386
// FUNCTION: STBC 0x0070a1e0 _$E385
// FUNCTION: STBC 0x0070a1f0 _$E143
// FUNCTION: STBC 0x0070a200 _$E142
// FUNCTION: STBC 0x0070a210 _$E148
// FUNCTION: STBC 0x0070a220 _$E147
// FUNCTION: STBC 0x0070a230 _$E390
// FUNCTION: STBC 0x0070a240 _$E389
// FUNCTION: STBC 0x0070a250 _$E394
// FUNCTION: STBC 0x0070a260 _$E393
// FUNCTION: STBC 0x0070a270 _$E398
// FUNCTION: STBC 0x0070a280 _$E397
// FUNCTION: STBC 0x0070a290 _$E402
// FUNCTION: STBC 0x0070a2a0 _$E401
// FUNCTION: STBC 0x0070a2b0 _$E406
// FUNCTION: STBC 0x0070a2c0 _$E405
// FUNCTION: STBC 0x0070a2d0 _$E410
// FUNCTION: STBC 0x0070a2e0 _$E409
// FUNCTION: STBC 0x0070a2f0 _$E414
// FUNCTION: STBC 0x0070a300 _$E413
// FUNCTION: STBC 0x0070a310 _$E418
// FUNCTION: STBC 0x0070a320 _$E417
// FUNCTION: STBC 0x0070a330 _$E153
// FUNCTION: STBC 0x0070a340 _$E152
// FUNCTION: STBC 0x0070a350 _$E422
// FUNCTION: STBC 0x0070a360 _$E421
// FUNCTION: STBC 0x0070a370 _$E158
// FUNCTION: STBC 0x0070a380 _$E157
// FUNCTION: STBC 0x0070a390 _$E163
// FUNCTION: STBC 0x0070a3a0 _$E162
// FUNCTION: STBC 0x0070a3b0 _$E426
// FUNCTION: STBC 0x0070a3c0 _$E425
// FUNCTION: STBC 0x0070a3d0 _$E431
// FUNCTION: STBC 0x0070a3e0 _$E430
// FUNCTION: STBC 0x0070a3f0 _$E435
// FUNCTION: STBC 0x0070a400 _$E434

// DATA: STBC 0x0099b748
TGSoundModule soundModule;		// invented name

// The directory of the Miles files the sound manager starts with.
#define MILES_DIRECTORY		"./data/Miles"

/*
================
TGSoundModule::VirtualSlot00

The startup: the classes, the handler names, then the sound manager on
the application's window (the render window when windowed).
================
*/
// FUNCTION: STBC 0x007096e0
void TGSoundModule::VirtualSlot00( void ) {
	RegisterClasses();
	RegisterHandlerNames();

	NiApplication *application = NiApplication::application;
	HWND window;
	if ( application->GetFullscreen() ) {
		window = application->GetAppWindow()->GetWindow();
	} else {
		window = application->GetAppWindow()->GetRenderWindow();
	}
	g_kSoundManager.Startup( window, MILES_DIRECTORY );
}

/*
================
TGSoundModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x00709730
void TGSoundModule::VirtualSlot05( void ) {
	g_kSoundManager.Shutdown();
}

/*
================
TGSoundModule::VirtualSlot01
================
*/
// FUNCTION: STBC 0x00709740
void TGSoundModule::VirtualSlot01( void ) {
	RegisterHandlers();
}

/*
================
TGSoundModule::VirtualSlot02

The CD player and the music move on each frame.
================
*/
// FUNCTION: STBC 0x00709750
void TGSoundModule::VirtualSlot02( void ) {
	g_kRedbook.UpdateRedbookCD();
	g_kMusicManager.Update();
}

/*
================
TGSoundModule::RegisterClasses

Each class's creator; the manager's and the player's handler tables take
TGEventHandlerObject's as their parent and join the array of the class
tables.
================
*/
// FUNCTION: STBC 0x00709770
void TGSoundModule::RegisterClasses( void ) {
	TGObject::RegisterCreator( TGSOUNDMANAGER_TYPE, CreateTGSoundManager );
	TGSoundManager::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGSoundManager::GetClassTable() );

	TGObject::RegisterCreator( TGMUSIC_TYPE, CreateTGMusic );
	TGMusic::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGMusic::GetClassTable() );

	TGObject::RegisterCreator( TGMUSICFADEEVENT_TYPE, CreateTGMusicFadeEvent );
	TGObject::RegisterCreator( TGSOUNDREGION_TYPE, CreateTGSoundRegion );
}

/*
================
TGSoundModule::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00709a10
void TGSoundModule::RegisterHandlerNames( void ) {
	TGSoundManager::RegisterHandlerNames();
	TGMusic::RegisterHandlerNames();
}

/*
================
TGSoundModule::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00709a20
void TGSoundModule::RegisterHandlers( void ) {
	TGSoundManager::RegisterHandlers();
	TGMusic::RegisterHandlers();
}

/*
================
TGSoundModule::VirtualSlot03
================
*/
// FUNCTION: STBC 0x00709a30
void TGSoundModule::VirtualSlot03( void ) {
}

/*
================
TGSoundModule::CreateTGSoundManager
================
*/
// FUNCTION: STBC 0x0070a410
TGObject *TGSoundModule::CreateTGSoundManager( long id ) {
	return new TGSoundManager( id );
}

/*
================
TGSoundModule::CreateTGMusic
================
*/
// FUNCTION: STBC 0x0070aa20
TGObject *TGSoundModule::CreateTGMusic( long id ) {
	return new TGMusic( id );
}

/*
================
TGSoundModule::CreateTGMusicFadeEvent
================
*/
// FUNCTION: STBC 0x0070aea0
TGObject *TGSoundModule::CreateTGMusicFadeEvent( long id ) {
	return new TGMusicFadeEvent( id );
}

/*
================
TGSoundModule::CreateTGSoundRegion
================
*/
// FUNCTION: STBC 0x0070afb0
TGObject *TGSoundModule::CreateTGSoundRegion( long id ) {
	return new TGSoundRegion( id );
}
