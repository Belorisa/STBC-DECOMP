/*
===============================================================================

	TGActionModule.cpp

===============================================================================
*/

#include <iostream>
#include "TGAction.h"
#include "TGAnimAction.h"
#include "TGActionManager.h"
#include "TGAnimPosition.h"
#include "TGActionModule.h"
#include "TGConditionAction.h"
#include "TGEventHandlerTable.h"
#include "TGOverlayAction.h"
#include "TGPhonemeAction.h"
#include "TGPhonemeSequence.h"
#include "TGScriptAction.h"
#include "TGSequence.h"
#include "TGSequenceEvent.h"
#include "TGSoundAction.h"
#include "TGTimedAction.h"
#include "PyEmbed.h"
#include "UtopiaApp.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006fc200 _$E32
// FUNCTION: STBC 0x006fc210 _$E29
// FUNCTION: STBC 0x006fc220 _$E31
// FUNCTION: STBC 0x006fc230 _$E30
// FUNCTION: STBC 0x006fc240 _$E38
// FUNCTION: STBC 0x006fc250 _$E35
// FUNCTION: STBC 0x006fc260 _$E37
// FUNCTION: STBC 0x006fc270 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006fc280 _$E52
// FUNCTION: STBC 0x006fc290 _$E51
// FUNCTION: STBC 0x006fc2a0 _$E63
// FUNCTION: STBC 0x006fc2b0 _$E62
// FUNCTION: STBC 0x006fc2c0 _$E67
// FUNCTION: STBC 0x006fc2d0 _$E66
// FUNCTION: STBC 0x006fc2e0 _$E71
// FUNCTION: STBC 0x006fc2f0 _$E70
// FUNCTION: STBC 0x006fc300 _$E75
// FUNCTION: STBC 0x006fc310 _$E74
// FUNCTION: STBC 0x006fc320 _$E199
// FUNCTION: STBC 0x006fc330 _$E198
// FUNCTION: STBC 0x006fc340 _$E204
// FUNCTION: STBC 0x006fc350 _$E203
// FUNCTION: STBC 0x006fc360 _$E208
// FUNCTION: STBC 0x006fc370 _$E207
// FUNCTION: STBC 0x006fc380 _$E141
// FUNCTION: STBC 0x006fc390 _$E140
// FUNCTION: STBC 0x006fc3a0 _$E154
// FUNCTION: STBC 0x006fc3b0 _$E153
// FUNCTION: STBC 0x006fc3c0 _$E174
// FUNCTION: STBC 0x006fc3d0 _$E173
// FUNCTION: STBC 0x006fc3e0 _$E244
// FUNCTION: STBC 0x006fc3f0 _$E243
// FUNCTION: STBC 0x006fc400 _$E249
// FUNCTION: STBC 0x006fc410 _$E248
// FUNCTION: STBC 0x006fc420 _$E253
// FUNCTION: STBC 0x006fc430 _$E252
// FUNCTION: STBC 0x006fc440 _$E260
// FUNCTION: STBC 0x006fc450 _$E259
// FUNCTION: STBC 0x006fc460 _$E290
// FUNCTION: STBC 0x006fc470 _$E289
// FUNCTION: STBC 0x006fc480 _$E294
// FUNCTION: STBC 0x006fc490 _$E293
// FUNCTION: STBC 0x006fc4a0 _$E99
// FUNCTION: STBC 0x006fc4b0 _$E98
// FUNCTION: STBC 0x006fc4c0 _$E276
// FUNCTION: STBC 0x006fc4d0 _$E275
// FUNCTION: STBC 0x006fc4e0 _$E283
// FUNCTION: STBC 0x006fc4f0 _$E282
// FUNCTION: STBC 0x006fc500 _$E306
// FUNCTION: STBC 0x006fc510 _$E305
// FUNCTION: STBC 0x006fc520 _$E310
// FUNCTION: STBC 0x006fc530 _$E309
// FUNCTION: STBC 0x006fc540 _$E350
// FUNCTION: STBC 0x006fc550 _$E349
// FUNCTION: STBC 0x006fc560 _$E356
// FUNCTION: STBC 0x006fc570 _$E355
// FUNCTION: STBC 0x006fc580 _$E298
// FUNCTION: STBC 0x006fc590 _$E297
// FUNCTION: STBC 0x006fc5a0 _$E302
// FUNCTION: STBC 0x006fc5b0 _$E301
// FUNCTION: STBC 0x006fc5c0 _$E314
// FUNCTION: STBC 0x006fc5d0 _$E313
// FUNCTION: STBC 0x006fc5e0 _$E318
// FUNCTION: STBC 0x006fc5f0 _$E317
// FUNCTION: STBC 0x006fc600 _$E322
// FUNCTION: STBC 0x006fc610 _$E321
// FUNCTION: STBC 0x006fc620 _$E326
// FUNCTION: STBC 0x006fc630 _$E325
// FUNCTION: STBC 0x006fc640 _$E330
// FUNCTION: STBC 0x006fc650 _$E329
// FUNCTION: STBC 0x006fc660 _$E334
// FUNCTION: STBC 0x006fc670 _$E333
// FUNCTION: STBC 0x006fc680 _$E338
// FUNCTION: STBC 0x006fc690 _$E337
// FUNCTION: STBC 0x006fc6a0 _$E342
// FUNCTION: STBC 0x006fc6b0 _$E341
// FUNCTION: STBC 0x006fc6c0 _$E104
// FUNCTION: STBC 0x006fc6d0 _$E103
// FUNCTION: STBC 0x006fc6e0 _$E346
// FUNCTION: STBC 0x006fc6f0 _$E345
// FUNCTION: STBC 0x006fc700 _$E109
// FUNCTION: STBC 0x006fc710 _$E108
// FUNCTION: STBC 0x006fc720 _$E360
// FUNCTION: STBC 0x006fc730 _$E359
// FUNCTION: STBC 0x006fc740 _$E364
// FUNCTION: STBC 0x006fc750 _$E363
// FUNCTION: STBC 0x006fc760 _$E368
// FUNCTION: STBC 0x006fc770 _$E367
// FUNCTION: STBC 0x006fc780 _$E372
// FUNCTION: STBC 0x006fc790 _$E371
// FUNCTION: STBC 0x006fc7a0 _$E118
// FUNCTION: STBC 0x006fc7b0 _$E117
// FUNCTION: STBC 0x006fc7c0 _$E376
// FUNCTION: STBC 0x006fc7d0 _$E375
// FUNCTION: STBC 0x006fc7e0 _$E380
// FUNCTION: STBC 0x006fc7f0 _$E379
// FUNCTION: STBC 0x006fc800 _$E384
// FUNCTION: STBC 0x006fc810 _$E383
// FUNCTION: STBC 0x006fc820 _$E124
// FUNCTION: STBC 0x006fc830 _$E123
// FUNCTION: STBC 0x006fc840 _$E129
// FUNCTION: STBC 0x006fc850 _$E128
// FUNCTION: STBC 0x006fc860 _$E159
// FUNCTION: STBC 0x006fc870 _$E158
// FUNCTION: STBC 0x006fc880 _$E388
// FUNCTION: STBC 0x006fc890 _$E387
// FUNCTION: STBC 0x006fc8a0 _$E164
// FUNCTION: STBC 0x006fc8b0 _$E163
// FUNCTION: STBC 0x006fc8c0 _$E392
// FUNCTION: STBC 0x006fc8d0 _$E391
// FUNCTION: STBC 0x006fc8e0 _$E396
// FUNCTION: STBC 0x006fc8f0 _$E395
// FUNCTION: STBC 0x006fc900 _$E169
// FUNCTION: STBC 0x006fc910 _$E168
// FUNCTION: STBC 0x006fc920 _$E400
// FUNCTION: STBC 0x006fc930 _$E399
// FUNCTION: STBC 0x006fc940 _$E134
// FUNCTION: STBC 0x006fc950 _$E133
// FUNCTION: STBC 0x006fc960 _$E404
// FUNCTION: STBC 0x006fc970 _$E403
// FUNCTION: STBC 0x006fc980 _$E408
// FUNCTION: STBC 0x006fc990 _$E407
// FUNCTION: STBC 0x006fc9a0 _$E179
// FUNCTION: STBC 0x006fc9b0 _$E178
// FUNCTION: STBC 0x006fc9c0 _$E412
// FUNCTION: STBC 0x006fc9d0 _$E411
// FUNCTION: STBC 0x006fc9e0 _$E416
// FUNCTION: STBC 0x006fc9f0 _$E415
// FUNCTION: STBC 0x006fca00 _$E420
// FUNCTION: STBC 0x006fca10 _$E419
// FUNCTION: STBC 0x006fca20 _$E424
// FUNCTION: STBC 0x006fca30 _$E423
// FUNCTION: STBC 0x006fca40 _$E428
// FUNCTION: STBC 0x006fca50 _$E427
// FUNCTION: STBC 0x006fca60 _$E432
// FUNCTION: STBC 0x006fca70 _$E431
// FUNCTION: STBC 0x006fca80 _$E436
// FUNCTION: STBC 0x006fca90 _$E435
// FUNCTION: STBC 0x006fcaa0 _$E440
// FUNCTION: STBC 0x006fcab0 _$E439
// FUNCTION: STBC 0x006fcac0 _$E184
// FUNCTION: STBC 0x006fcad0 _$E183
// FUNCTION: STBC 0x006fcae0 _$E444
// FUNCTION: STBC 0x006fcaf0 _$E443
// FUNCTION: STBC 0x006fcb00 _$E189
// FUNCTION: STBC 0x006fcb10 _$E188
// FUNCTION: STBC 0x006fcb20 _$E194
// FUNCTION: STBC 0x006fcb30 _$E193

// DATA: STBC 0x0099aeb0
TGActionModule actionModule;		// invented name

/*
================
TGActionModule::VirtualSlot00

The startup: the classes, the handler names and the Python method of
TGConditionAction.
================
*/
// FUNCTION: STBC 0x006fcbb0
void TGActionModule::VirtualSlot00( void ) {
	RegisterClasses();
	RegisterHandlerNames();
	PyEmbed::AddClassMethod( TGConditionAction_SetEvaluationFunction, "TGConditionAction", "SetEvaluationFunction" );
	g_kTGActionManager.Unknown6FEBA0();
}

/*
================
TGActionModule::RegisterClasses
================
*/
// FUNCTION: STBC 0x006fcbf0
void TGActionModule::RegisterClasses( void ) {
	TGObject::RegisterCreator( TGACTIONMANAGER_TYPE, CreateTGActionManager );
	TGObject::RegisterCreator( TGACTION_TYPE, CreateTGAction );
	TGObject::RegisterCreator( TGSEQUENCE_TYPE, CreateTGSequence );
	TGObject::RegisterCreator( TGTIMEDACTION_TYPE, CreateTGTimedAction );
	TGObject::RegisterCreator( TGANIMACTION_TYPE, CreateTGAnimAction );
	TGObject::RegisterCreator( TGSCRIPTACTION_TYPE, CreateTGScriptAction );
	TGObject::RegisterCreator( TGANIMPOSITION_TYPE, CreateTGAnimPosition );
	TGObject::RegisterCreator( TGCONDITIONACTION_TYPE, CreateTGConditionAction );
	TGObject::RegisterCreator( TGPHONEMEACTION_TYPE, CreateTGPhonemeAction );
	TGObject::RegisterCreator( TGOVERLAYACTION_TYPE, CreateTGOverlayAction );
	TGObject::RegisterCreator( TGPHONEMESEQUENCE_TYPE, CreateTGPhonemeSequence );
	TGObject::RegisterCreator( TGSOUNDACTION_TYPE, CreateTGSoundAction );

	TGActionManager::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGActionManager::GetClassTable() );

	TGObject::RegisterCreator( TGSEQUENCEEVENT_TYPE, CreateTGSequenceEvent );
}

/*
================
TGActionModule::CreateTGAction
================
*/
// FUNCTION: STBC 0x006fcec0
TGObject *TGActionModule::CreateTGAction( long id ) {
	return new TGAction( id );
}

/*
================
TGActionModule::CreateTGActionManager
================
*/
// FUNCTION: STBC 0x006fcef0
TGObject *TGActionModule::CreateTGActionManager( long id ) {
	return new TGActionManager( id );
}

/*
================
TGActionModule::CreateTGSequence
================
*/
// FUNCTION: STBC 0x006fd090
TGObject *TGActionModule::CreateTGSequence( long id ) {
	return new TGSequence( id );
}

/*
================
TGActionModule::CreateTGSequenceEvent
================
*/
// FUNCTION: STBC 0x006fd0c0
TGObject *TGActionModule::CreateTGSequenceEvent( long id ) {
	return new TGSequenceEvent( id );
}

/*
================
TGActionModule::CreateTGConditionAction
================
*/
// FUNCTION: STBC 0x006fd110
TGObject *TGActionModule::CreateTGConditionAction( long id ) {
	return new TGConditionAction( id );
}

/*
================
TGActionModule::CreateTGScriptAction
================
*/
// FUNCTION: STBC 0x006fd240
TGObject *TGActionModule::CreateTGScriptAction( long id ) {
	return new TGScriptAction( id );
}

/*
================
TGActionModule::CreateTGTimedAction
================
*/
// FUNCTION: STBC 0x006fd2a0
TGObject *TGActionModule::CreateTGTimedAction( long id ) {
	return new TGTimedAction( id );
}

/*
================
TGActionModule::CreateTGAnimAction
================
*/
// FUNCTION: STBC 0x006fd2d0
TGObject *TGActionModule::CreateTGAnimAction( long id ) {
	return new TGAnimAction( id );
}

/*
================
TGActionModule::CreateTGAnimPosition
================
*/
// FUNCTION: STBC 0x006fd3d0
TGObject *TGActionModule::CreateTGAnimPosition( long id ) {
	return new TGAnimPosition( id );
}

/*
================
TGActionModule::CreateTGSoundAction
================
*/
// FUNCTION: STBC 0x006fd4c0
TGObject *TGActionModule::CreateTGSoundAction( long id ) {
	return new TGSoundAction( id );
}

/*
================
TGActionModule::CreateTGPhonemeAction
================
*/
// FUNCTION: STBC 0x006fd5b0
TGObject *TGActionModule::CreateTGPhonemeAction( long id ) {
	return new TGPhonemeAction( id );
}

/*
================
TGActionModule::CreateTGPhonemeSequence
================
*/
// FUNCTION: STBC 0x006fd710
TGObject *TGActionModule::CreateTGPhonemeSequence( long id ) {
	return new TGPhonemeSequence( id );
}

/*
================
TGActionModule::CreateTGOverlayAction
================
*/
// FUNCTION: STBC 0x006fd890
TGObject *TGActionModule::CreateTGOverlayAction( long id ) {
	return new TGOverlayAction( id );
}

/*
================
TGActionModule::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x006fd9f0
void TGActionModule::RegisterHandlerNames( void ) {
	TGActionManager::RegisterHandlerNames();
}

/*
================
TGActionModule::RegisterHandlers
================
*/
// FUNCTION: STBC 0x006fda00
void TGActionModule::RegisterHandlers( void ) {
	TGActionManager::RegisterHandlers();
}

/*
================
TGActionModule::VirtualSlot01
================
*/
// FUNCTION: STBC 0x006fda10
void TGActionModule::VirtualSlot01( void ) {
	RegisterHandlers();
}

/*
================
TGActionModule::VirtualSlot02

The phoneme sequences advance by the frame's game time, at most the
time rate.
================
*/
// FUNCTION: STBC 0x006fda20
void TGActionModule::VirtualSlot02( void ) {
	TGPhonemeSequence::UpdateAll( UtopiaApp::GetApp()->GetGameFrameStep() );
}

/*
================
TGActionModule::VirtualSlot04
================
*/
// FUNCTION: STBC 0x006fda70
void TGActionModule::VirtualSlot04( void ) {
	g_kTGActionManager.DeleteAllActions();
	g_kTGActionManager.Unknown6FEBA0();
}

/*
================
TGActionModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x006fda90
void TGActionModule::VirtualSlot05( void ) {
	g_kTGActionManager.DeleteAllActions();
}
