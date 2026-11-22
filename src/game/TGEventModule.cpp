/*
===============================================================================

	TGEventModule.cpp

===============================================================================
*/

#include <iostream>
#include "TGEventModule.h"
#include "NiMain.h"
#include "PyEmbed.h"
#include "TGBoolEvent.h"
#include "TGCharEvent.h"
#include "TGEvent.h"
#include "TGEventHandlerObject.h"
#include "TGEventManager.h"
#include "TGFloatEvent.h"
#include "TGIntEvent.h"
#include "TGObjPtrEvent.h"
#include "TGPythonInstanceWrapper.h"
#include "TGShortEvent.h"
#include "TGStringEvent.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "TGVoidPtrEvent.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006d3a60 _$E453
// FUNCTION: STBC 0x006d3a80 _$E452
// FUNCTION: STBC 0x006d3bb0 _$E32
// FUNCTION: STBC 0x006d3bc0 _$E29
// FUNCTION: STBC 0x006d3bd0 _$E31
// FUNCTION: STBC 0x006d3be0 _$E30
// FUNCTION: STBC 0x006d3bf0 _$E38
// FUNCTION: STBC 0x006d3c00 _$E35
// FUNCTION: STBC 0x006d3c10 _$E37
// FUNCTION: STBC 0x006d3c20 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006d3c30 _$E47
// FUNCTION: STBC 0x006d3c40 _$E46
// FUNCTION: STBC 0x006d3c50 _$E51
// FUNCTION: STBC 0x006d3c60 _$E50
// FUNCTION: STBC 0x006d3c70 _$E62
// FUNCTION: STBC 0x006d3c80 _$E61
// FUNCTION: STBC 0x006d3c90 _$E66
// FUNCTION: STBC 0x006d3ca0 _$E65
// FUNCTION: STBC 0x006d3cb0 _$E70
// FUNCTION: STBC 0x006d3cc0 _$E69
// FUNCTION: STBC 0x006d3cd0 _$E74
// FUNCTION: STBC 0x006d3ce0 _$E73
// FUNCTION: STBC 0x006d3cf0 _$E198
// FUNCTION: STBC 0x006d3d00 _$E197
// FUNCTION: STBC 0x006d3d10 _$E140
// FUNCTION: STBC 0x006d3d20 _$E139
// FUNCTION: STBC 0x006d3d30 _$E153
// FUNCTION: STBC 0x006d3d40 _$E152
// FUNCTION: STBC 0x006d3d50 _$E203
// FUNCTION: STBC 0x006d3d60 _$E202
// FUNCTION: STBC 0x006d3d70 _$E208
// FUNCTION: STBC 0x006d3d80 _$E207
// FUNCTION: STBC 0x006d3d90 _$E212
// FUNCTION: STBC 0x006d3da0 _$E211
// FUNCTION: STBC 0x006d3db0 _$E216
// FUNCTION: STBC 0x006d3dc0 _$E215
// FUNCTION: STBC 0x006d3dd0 _$E223
// FUNCTION: STBC 0x006d3de0 _$E222
// FUNCTION: STBC 0x006d3df0 _$E248
// FUNCTION: STBC 0x006d3e00 _$E247
// FUNCTION: STBC 0x006d3e10 _$E252
// FUNCTION: STBC 0x006d3e20 _$E251
// FUNCTION: STBC 0x006d3e30 _$E98
// FUNCTION: STBC 0x006d3e40 _$E97
// FUNCTION: STBC 0x006d3e50 _$E234
// FUNCTION: STBC 0x006d3e60 _$E233
// FUNCTION: STBC 0x006d3e70 _$E241
// FUNCTION: STBC 0x006d3e80 _$E240
// FUNCTION: STBC 0x006d3e90 _$E264
// FUNCTION: STBC 0x006d3ea0 _$E263
// FUNCTION: STBC 0x006d3eb0 _$E268
// FUNCTION: STBC 0x006d3ec0 _$E267
// FUNCTION: STBC 0x006d3ed0 _$E308
// FUNCTION: STBC 0x006d3ee0 _$E307
// FUNCTION: STBC 0x006d3ef0 _$E314
// FUNCTION: STBC 0x006d3f00 _$E313
// FUNCTION: STBC 0x006d3f10 _$E256
// FUNCTION: STBC 0x006d3f20 _$E255
// FUNCTION: STBC 0x006d3f30 _$E260
// FUNCTION: STBC 0x006d3f40 _$E259
// FUNCTION: STBC 0x006d3f50 _$E272
// FUNCTION: STBC 0x006d3f60 _$E271
// FUNCTION: STBC 0x006d3f70 _$E276
// FUNCTION: STBC 0x006d3f80 _$E275
// FUNCTION: STBC 0x006d3f90 _$E280
// FUNCTION: STBC 0x006d3fa0 _$E279
// FUNCTION: STBC 0x006d3fb0 _$E284
// FUNCTION: STBC 0x006d3fc0 _$E283
// FUNCTION: STBC 0x006d3fd0 _$E288
// FUNCTION: STBC 0x006d3fe0 _$E287
// FUNCTION: STBC 0x006d3ff0 _$E292
// FUNCTION: STBC 0x006d4000 _$E291
// FUNCTION: STBC 0x006d4010 _$E296
// FUNCTION: STBC 0x006d4020 _$E295
// FUNCTION: STBC 0x006d4030 _$E300
// FUNCTION: STBC 0x006d4040 _$E299
// FUNCTION: STBC 0x006d4050 _$E103
// FUNCTION: STBC 0x006d4060 _$E102
// FUNCTION: STBC 0x006d4070 _$E304
// FUNCTION: STBC 0x006d4080 _$E303
// FUNCTION: STBC 0x006d4090 _$E108
// FUNCTION: STBC 0x006d40a0 _$E107
// FUNCTION: STBC 0x006d40b0 _$E318
// FUNCTION: STBC 0x006d40c0 _$E317
// FUNCTION: STBC 0x006d40d0 _$E322
// FUNCTION: STBC 0x006d40e0 _$E321
// FUNCTION: STBC 0x006d40f0 _$E326
// FUNCTION: STBC 0x006d4100 _$E325
// FUNCTION: STBC 0x006d4110 _$E330
// FUNCTION: STBC 0x006d4120 _$E329
// FUNCTION: STBC 0x006d4130 _$E117
// FUNCTION: STBC 0x006d4140 _$E116
// FUNCTION: STBC 0x006d4150 _$E334
// FUNCTION: STBC 0x006d4160 _$E333
// FUNCTION: STBC 0x006d4170 _$E338
// FUNCTION: STBC 0x006d4180 _$E337
// FUNCTION: STBC 0x006d4190 _$E342
// FUNCTION: STBC 0x006d41a0 _$E341
// FUNCTION: STBC 0x006d41b0 _$E123
// FUNCTION: STBC 0x006d41c0 _$E122
// FUNCTION: STBC 0x006d41d0 _$E128
// FUNCTION: STBC 0x006d41e0 _$E127
// FUNCTION: STBC 0x006d41f0 _$E158
// FUNCTION: STBC 0x006d4200 _$E157
// FUNCTION: STBC 0x006d4210 _$E346
// FUNCTION: STBC 0x006d4220 _$E345
// FUNCTION: STBC 0x006d4230 _$E163
// FUNCTION: STBC 0x006d4240 _$E162
// FUNCTION: STBC 0x006d4250 _$E350
// FUNCTION: STBC 0x006d4260 _$E349
// FUNCTION: STBC 0x006d4270 _$E354
// FUNCTION: STBC 0x006d4280 _$E353
// FUNCTION: STBC 0x006d4290 _$E168
// FUNCTION: STBC 0x006d42a0 _$E167
// FUNCTION: STBC 0x006d42b0 _$E358
// FUNCTION: STBC 0x006d42c0 _$E357
// FUNCTION: STBC 0x006d42d0 _$E133
// FUNCTION: STBC 0x006d42e0 _$E132
// FUNCTION: STBC 0x006d42f0 _$E362
// FUNCTION: STBC 0x006d4300 _$E361
// FUNCTION: STBC 0x006d4310 _$E366
// FUNCTION: STBC 0x006d4320 _$E365
// FUNCTION: STBC 0x006d4330 _$E173
// FUNCTION: STBC 0x006d4340 _$E172
// FUNCTION: STBC 0x006d4350 _$E178
// FUNCTION: STBC 0x006d4360 _$E177
// FUNCTION: STBC 0x006d4370 _$E370
// FUNCTION: STBC 0x006d4380 _$E369
// FUNCTION: STBC 0x006d4390 _$E374
// FUNCTION: STBC 0x006d43a0 _$E373
// FUNCTION: STBC 0x006d43b0 _$E378
// FUNCTION: STBC 0x006d43c0 _$E377
// FUNCTION: STBC 0x006d43d0 _$E382
// FUNCTION: STBC 0x006d43e0 _$E381
// FUNCTION: STBC 0x006d43f0 _$E386
// FUNCTION: STBC 0x006d4400 _$E385
// FUNCTION: STBC 0x006d4410 _$E390
// FUNCTION: STBC 0x006d4420 _$E389
// FUNCTION: STBC 0x006d4430 _$E394
// FUNCTION: STBC 0x006d4440 _$E393
// FUNCTION: STBC 0x006d4450 _$E398
// FUNCTION: STBC 0x006d4460 _$E397
// FUNCTION: STBC 0x006d4470 _$E183
// FUNCTION: STBC 0x006d4480 _$E182
// FUNCTION: STBC 0x006d4490 _$E402
// FUNCTION: STBC 0x006d44a0 _$E401
// FUNCTION: STBC 0x006d44b0 _$E188
// FUNCTION: STBC 0x006d44c0 _$E187
// FUNCTION: STBC 0x006d44d0 _$E193
// FUNCTION: STBC 0x006d44e0 _$E192

// DATA: STBC 0x00998260
TGEventModule eventModule;		// invented name

/*
================
TGEventModule::VirtualSlot00

The startup: the creators of the event classes, the class handler tables,
the handler names, the wrapper's state functions and two Python methods.
================
*/
// FUNCTION: STBC 0x006d4560
void TGEventModule::VirtualSlot00( void ) {
	TGObject::RegisterCreator( TGEVENT_TYPE, CreateTGEvent );
	TGObject::RegisterCreator( TGEVENTHANDLEROBJECT_TYPE, CreateTGEventHandlerObject );
	TGObject::RegisterCreator( TGTIMER_TYPE, CreateTGTimer );
	TGObject::RegisterCreator( TGBOOLEVENT_TYPE, CreateTGBoolEvent );
	TGObject::RegisterCreator( TGCHAREVENT_TYPE, CreateTGCharEvent );
	TGObject::RegisterCreator( TGSHORTEVENT_TYPE, CreateTGShortEvent );
	TGObject::RegisterCreator( TGINTEVENT_TYPE, CreateTGIntEvent );
	TGObject::RegisterCreator( TGFLOATEVENT_TYPE, CreateTGFloatEvent );
	TGObject::RegisterCreator( TGSTRINGEVENT_TYPE, CreateTGStringEvent );
	TGObject::RegisterCreator( TGVOIDPTREVENT_TYPE, CreateTGVoidPtrEvent );
	TGObject::RegisterCreator( TGPYTHONINSTANCEWRAPPER_TYPE, CreateTGPythonInstanceWrapper );
	TGObject::RegisterCreator( TGOBJPTREVENT_TYPE, CreateTGObjPtrEvent );

	TGEventHandlerObject::classTables.Add( TGEventHandlerObject::GetClassTable() );
	TGPythonInstanceWrapper::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGPythonInstanceWrapper::GetClassTable() );

	TGEventHandlerObject::RegisterHandlerNames();
	RegisterStateFunctions();

	PyEmbed::AddClassMethod( TGStringEvent_GetCString, "TGStringEvent", "GetCString" );
	PyEmbed::AddClassMethod( TGPythonInstanceWrapper_SetPyWrapper, "TGPythonInstanceWrapper", "SetPyWrapper" );
}

/*
================
TGEventModule::CreateTGEventHandlerObject
================
*/
// FUNCTION: STBC 0x006d47e0
TGObject *TGEventModule::CreateTGEventHandlerObject( long id ) {
	return new TGEventHandlerObject( id );
}

/*
================
TGEventModule::CreateTGEvent
================
*/
// FUNCTION: STBC 0x006d4810
TGObject *TGEventModule::CreateTGEvent( long id ) {
	return new TGEvent( id );
}

/*
================
TGEventModule::CreateTGBoolEvent
================
*/
// FUNCTION: STBC 0x006d4840
TGObject *TGEventModule::CreateTGBoolEvent( long id ) {
	return new TGBoolEvent( id );
}

/*
================
TGEventModule::CreateTGCharEvent
================
*/
// FUNCTION: STBC 0x006d4870
TGObject *TGEventModule::CreateTGCharEvent( long id ) {
	return new TGCharEvent( id );
}

/*
================
TGEventModule::CreateTGShortEvent
================
*/
// FUNCTION: STBC 0x006d48a0
TGObject *TGEventModule::CreateTGShortEvent( long id ) {
	return new TGShortEvent( id );
}

/*
================
TGEventModule::CreateTGIntEvent
================
*/
// FUNCTION: STBC 0x006d48d0
TGObject *TGEventModule::CreateTGIntEvent( long id ) {
	return new TGIntEvent( id );
}

/*
================
TGEventModule::CreateTGFloatEvent
================
*/
// FUNCTION: STBC 0x006d4900
TGObject *TGEventModule::CreateTGFloatEvent( long id ) {
	return new TGFloatEvent( id );
}

/*
================
TGEventModule::CreateTGStringEvent
================
*/
// FUNCTION: STBC 0x006d4930
TGObject *TGEventModule::CreateTGStringEvent( long id ) {
	return new TGStringEvent( id );
}

/*
================
TGEventModule::CreateTGVoidPtrEvent
================
*/
// FUNCTION: STBC 0x006d4960
TGObject *TGEventModule::CreateTGVoidPtrEvent( long id ) {
	return new TGVoidPtrEvent( id );
}

/*
================
TGEventModule::CreateTGObjPtrEvent
================
*/
// FUNCTION: STBC 0x006d4990
TGObject *TGEventModule::CreateTGObjPtrEvent( long id ) {
	return new TGObjPtrEvent( id );
}

/*
================
TGEventModule::CreateTGTimer
================
*/
// FUNCTION: STBC 0x006d49c0
TGObject *TGEventModule::CreateTGTimer( long id ) {
	return new TGTimer( id );
}

/*
================
TGEventModule::CreateTGPythonInstanceWrapper
================
*/
// FUNCTION: STBC 0x006d49f0
TGObject *TGEventModule::CreateTGPythonInstanceWrapper( long id ) {
	return new TGPythonInstanceWrapper( id );
}

/*
================
TGEventModule::VirtualSlot01

The handlers of the event handler class, and both timer managers start over.
================
*/
// FUNCTION: STBC 0x006d4b20
void TGEventModule::VirtualSlot01( void ) {
	TGEventHandlerObject::RegisterHandlers();
	g_kTimerManager.ResetTime();
	g_kRealtimeTimerManager.ResetTime();
}

/*
================
TGEventModule::VirtualSlot03

Each frame: the queued events, until none is left.
================
*/
// FUNCTION: STBC 0x006d4b40
void TGEventModule::VirtualSlot03( void ) {
	while ( g_kEventManager.GetNumQueuedEvents() != 0 ) {
		g_kEventManager.Unknown6DA2C0();
	}
}

/*
================
TGEventModule::VirtualSlot04

Drops every timer, queued event and handler of the classes and broadcasts.
================
*/
// FUNCTION: STBC 0x006d4b60
void TGEventModule::VirtualSlot04( void ) {
	g_kTimerManager.DeleteAllTimers();
	g_kRealtimeTimerManager.DeleteAllTimers();
	g_kEventManager.ClearLoggedEvents();
	g_kEventManager.ClearEvents();
	g_kEventManager.RemoveAllClassHandlers();
	g_kEventManager.RemoveAllBroadcastHandlers();
}

/*
================
TGEventModule::VirtualSlot05

The end: slot 4's work, then the handler registry and the event and timer
tables go.
================
*/
// FUNCTION: STBC 0x006d4ba0
void TGEventModule::VirtualSlot05( void ) {
	g_kTimerManager.DeleteAllTimers();
	g_kRealtimeTimerManager.DeleteAllTimers();
	g_kEventManager.ClearLoggedEvents();
	g_kEventManager.ClearEvents();
	g_kEventManager.RemoveAllClassHandlers();
	g_kEventManager.RemoveAllBroadcastHandlers();
	handlerRegistry.Clear();
	TGEvent::ReleaseEventTable();
	TGTimer::ReleaseTimerTable();
}

/*
================
TGEventModule::RegisterStateFunctions
================
*/
// FUNCTION: STBC 0x006d4bf0
void TGEventModule::RegisterStateFunctions( void ) {
	PyEmbed::AddGetStateFunction( TGPythonInstanceWrapper::GetState, "TGPythonInstanceWrapper" );
	PyEmbed::AddSetStateFunction( TGPythonInstanceWrapper::SetState, "TGPythonInstanceWrapper" );
}
