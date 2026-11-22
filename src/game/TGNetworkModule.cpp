/*
===============================================================================

	TGNetworkModule.cpp

===============================================================================
*/

#include <iostream>
#include "TGEventHandlerTable.h"
#include "TGGameSpyEvent.h"
#include "TGMessageEvent.h"
#include "TGNetwork.h"
#include "TGNetworkModule.h"
#include "TGObject.h"
#include "TGPlayerEvent.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006b2ad0 _$E431
// FUNCTION: STBC 0x006b2af0 _$E430

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006b2950 _$E32
// FUNCTION: STBC 0x006b2960 _$E29
// FUNCTION: STBC 0x006b2970 _$E31
// FUNCTION: STBC 0x006b2980 _$E30
// FUNCTION: STBC 0x006b2990 _$E38
// FUNCTION: STBC 0x006b29a0 _$E35
// FUNCTION: STBC 0x006b29b0 _$E37
// FUNCTION: STBC 0x006b29c0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006b29d0 _$E63
// FUNCTION: STBC 0x006b29e0 _$E62
// FUNCTION: STBC 0x006b29f0 _$E67
// FUNCTION: STBC 0x006b2a00 _$E66
// FUNCTION: STBC 0x006b2a10 _$E78
// FUNCTION: STBC 0x006b2a20 _$E77
// FUNCTION: STBC 0x006b2a30 _$E82
// FUNCTION: STBC 0x006b2a40 _$E81
// FUNCTION: STBC 0x006b2a50 _$E86
// FUNCTION: STBC 0x006b2a60 _$E85
// FUNCTION: STBC 0x006b2a70 _$E90
// FUNCTION: STBC 0x006b2a80 _$E89
// FUNCTION: STBC 0x006b2a90 _$E214
// FUNCTION: STBC 0x006b2aa0 _$E213
// FUNCTION: STBC 0x006b2ab0 _$E156
// FUNCTION: STBC 0x006b2ac0 _$E155
// FUNCTION: STBC 0x006b2b00 _$E169
// FUNCTION: STBC 0x006b2b10 _$E168
// FUNCTION: STBC 0x006b2b20 _$E219
// FUNCTION: STBC 0x006b2b30 _$E218
// FUNCTION: STBC 0x006b2b40 _$E224
// FUNCTION: STBC 0x006b2b50 _$E223
// FUNCTION: STBC 0x006b2b60 _$E228
// FUNCTION: STBC 0x006b2b70 _$E227
// FUNCTION: STBC 0x006b2b80 _$E232
// FUNCTION: STBC 0x006b2b90 _$E231
// FUNCTION: STBC 0x006b2ba0 _$E239
// FUNCTION: STBC 0x006b2bb0 _$E238
// FUNCTION: STBC 0x006b2bc0 _$E264
// FUNCTION: STBC 0x006b2bd0 _$E263
// FUNCTION: STBC 0x006b2be0 _$E268
// FUNCTION: STBC 0x006b2bf0 _$E267
// FUNCTION: STBC 0x006b2c00 _$E114
// FUNCTION: STBC 0x006b2c10 _$E113
// FUNCTION: STBC 0x006b2c20 _$E250
// FUNCTION: STBC 0x006b2c30 _$E249
// FUNCTION: STBC 0x006b2c40 _$E257
// FUNCTION: STBC 0x006b2c50 _$E256
// FUNCTION: STBC 0x006b2c60 _$E280
// FUNCTION: STBC 0x006b2c70 _$E279
// FUNCTION: STBC 0x006b2c80 _$E284
// FUNCTION: STBC 0x006b2c90 _$E283
// FUNCTION: STBC 0x006b2ca0 _$E324
// FUNCTION: STBC 0x006b2cb0 _$E323
// FUNCTION: STBC 0x006b2cc0 _$E330
// FUNCTION: STBC 0x006b2cd0 _$E329
// FUNCTION: STBC 0x006b2ce0 _$E272
// FUNCTION: STBC 0x006b2cf0 _$E271
// FUNCTION: STBC 0x006b2d00 _$E276
// FUNCTION: STBC 0x006b2d10 _$E275
// FUNCTION: STBC 0x006b2d20 _$E288
// FUNCTION: STBC 0x006b2d30 _$E287
// FUNCTION: STBC 0x006b2d40 _$E292
// FUNCTION: STBC 0x006b2d50 _$E291
// FUNCTION: STBC 0x006b2d60 _$E296
// FUNCTION: STBC 0x006b2d70 _$E295
// FUNCTION: STBC 0x006b2d80 _$E300
// FUNCTION: STBC 0x006b2d90 _$E299
// FUNCTION: STBC 0x006b2da0 _$E304
// FUNCTION: STBC 0x006b2db0 _$E303
// FUNCTION: STBC 0x006b2dc0 _$E308
// FUNCTION: STBC 0x006b2dd0 _$E307
// FUNCTION: STBC 0x006b2de0 _$E312
// FUNCTION: STBC 0x006b2df0 _$E311
// FUNCTION: STBC 0x006b2e00 _$E316
// FUNCTION: STBC 0x006b2e10 _$E315
// FUNCTION: STBC 0x006b2e20 _$E119
// FUNCTION: STBC 0x006b2e30 _$E118
// FUNCTION: STBC 0x006b2e40 _$E320
// FUNCTION: STBC 0x006b2e50 _$E319
// FUNCTION: STBC 0x006b2e60 _$E124
// FUNCTION: STBC 0x006b2e70 _$E123
// FUNCTION: STBC 0x006b2e80 _$E334
// FUNCTION: STBC 0x006b2e90 _$E333
// FUNCTION: STBC 0x006b2ea0 _$E338
// FUNCTION: STBC 0x006b2eb0 _$E337
// FUNCTION: STBC 0x006b2ec0 _$E342
// FUNCTION: STBC 0x006b2ed0 _$E341
// FUNCTION: STBC 0x006b2ee0 _$E346
// FUNCTION: STBC 0x006b2ef0 _$E345
// FUNCTION: STBC 0x006b2f00 _$E133
// FUNCTION: STBC 0x006b2f10 _$E132
// FUNCTION: STBC 0x006b2f20 _$E350
// FUNCTION: STBC 0x006b2f30 _$E349
// FUNCTION: STBC 0x006b2f40 _$E354
// FUNCTION: STBC 0x006b2f50 _$E353
// FUNCTION: STBC 0x006b2f60 _$E358
// FUNCTION: STBC 0x006b2f70 _$E357
// FUNCTION: STBC 0x006b2f80 _$E139
// FUNCTION: STBC 0x006b2f90 _$E138
// FUNCTION: STBC 0x006b2fa0 _$E144
// FUNCTION: STBC 0x006b2fb0 _$E143
// FUNCTION: STBC 0x006b2fc0 _$E174
// FUNCTION: STBC 0x006b2fd0 _$E173
// FUNCTION: STBC 0x006b2fe0 _$E362
// FUNCTION: STBC 0x006b2ff0 _$E361
// FUNCTION: STBC 0x006b3000 _$E179
// FUNCTION: STBC 0x006b3010 _$E178
// FUNCTION: STBC 0x006b3020 _$E366
// FUNCTION: STBC 0x006b3030 _$E365
// FUNCTION: STBC 0x006b3040 _$E370
// FUNCTION: STBC 0x006b3050 _$E369
// FUNCTION: STBC 0x006b3060 _$E184
// FUNCTION: STBC 0x006b3070 _$E183
// FUNCTION: STBC 0x006b3080 _$E374
// FUNCTION: STBC 0x006b3090 _$E373
// FUNCTION: STBC 0x006b30a0 _$E149
// FUNCTION: STBC 0x006b30b0 _$E148
// FUNCTION: STBC 0x006b30c0 _$E378
// FUNCTION: STBC 0x006b30d0 _$E377
// FUNCTION: STBC 0x006b30e0 _$E382
// FUNCTION: STBC 0x006b30f0 _$E381
// FUNCTION: STBC 0x006b3100 _$E189
// FUNCTION: STBC 0x006b3110 _$E188
// FUNCTION: STBC 0x006b3120 _$E194
// FUNCTION: STBC 0x006b3130 _$E193
// FUNCTION: STBC 0x006b3140 _$E386
// FUNCTION: STBC 0x006b3150 _$E385
// FUNCTION: STBC 0x006b3160 _$E390
// FUNCTION: STBC 0x006b3170 _$E389
// FUNCTION: STBC 0x006b3180 _$E394
// FUNCTION: STBC 0x006b3190 _$E393
// FUNCTION: STBC 0x006b31a0 _$E398
// FUNCTION: STBC 0x006b31b0 _$E397
// FUNCTION: STBC 0x006b31c0 _$E402
// FUNCTION: STBC 0x006b31d0 _$E401
// FUNCTION: STBC 0x006b31e0 _$E406
// FUNCTION: STBC 0x006b31f0 _$E405
// FUNCTION: STBC 0x006b3200 _$E410
// FUNCTION: STBC 0x006b3210 _$E409
// FUNCTION: STBC 0x006b3220 _$E414
// FUNCTION: STBC 0x006b3230 _$E413
// FUNCTION: STBC 0x006b3240 _$E199
// FUNCTION: STBC 0x006b3250 _$E198
// FUNCTION: STBC 0x006b3260 _$E418
// FUNCTION: STBC 0x006b3270 _$E417
// FUNCTION: STBC 0x006b3280 _$E204
// FUNCTION: STBC 0x006b3290 _$E203
// FUNCTION: STBC 0x006b32a0 _$E209
// FUNCTION: STBC 0x006b32b0 _$E208

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006b2520 _$E423
// FUNCTION: STBC 0x006b2530 _$E420
// FUNCTION: STBC 0x006b2540 _$E422
// FUNCTION: STBC 0x006b2550 _$E421
// DATA: STBC 0x00995d68
TGNetworkModule networkModule;		// invented name

/*
================
TGNetworkModule::VirtualSlot00

The startup: the classes and the handler names.
================
*/
// FUNCTION: STBC 0x006b2590
void TGNetworkModule::VirtualSlot00( void ) {
	RegisterClasses();
	RegisterHandlerNames();
}

/*
================
TGNetworkModule::VirtualSlot01
================
*/
// FUNCTION: STBC 0x006b25b0
void TGNetworkModule::VirtualSlot01( void ) {
	RegisterHandlers();
}

/*
================
TGNetworkModule::VirtualSlot04

The list of networks lets them go; the networks themselves are not
deleted.
================
*/
// FUNCTION: STBC 0x006b25c0
void TGNetworkModule::VirtualSlot04( void ) {
	TGNetwork::GetTGNetworkList()->RemoveAll();
}

/*
================
TGNetworkModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x006b2610
void TGNetworkModule::VirtualSlot05( void ) {
}

/*
================
TGNetworkModule::VirtualSlot02

Each frame every network sends, receives and hands out its messages.
================
*/
// FUNCTION: STBC 0x006b2620
void TGNetworkModule::VirtualSlot02( void ) {
	TGNetworkListType *networks = TGNetwork::GetTGNetworkList();
	for ( TGNetwork *network = networks->GetFirst(); network != NULL; network = networks->GetNext() ) {
		network->Update();
	}
}

/*
================
TGNetworkModule::RegisterClasses

Each class's creator; the network's handler table takes
TGEventHandlerObject's as its parent and joins the array of the class
tables.
================
*/
// FUNCTION: STBC 0x006b2670
void TGNetworkModule::RegisterClasses( void ) {
	TGObject::RegisterCreator( TGNETWORK_TYPE, CreateTGNetwork );
	TGNetwork::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGNetwork::GetClassTable() );

	TGObject::RegisterCreator( TGMESSAGEEVENT_TYPE, CreateTGMessageEvent );
	TGObject::RegisterCreator( TGPLAYEREVENT_TYPE, CreateTGPlayerEvent );
	TGObject::RegisterCreator( TGGAMESPYEVENT_TYPE, CreateTGGameSpyEvent );
}

/*
================
TGNetworkModule::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x006b2930
void TGNetworkModule::RegisterHandlerNames( void ) {
	TGNetwork::RegisterHandlers();
}

/*
================
TGNetworkModule::RegisterHandlers
================
*/
// FUNCTION: STBC 0x006b2940
void TGNetworkModule::RegisterHandlers( void ) {
	TGNetwork::AddClassHandlers();
}

/*
================
TGNetworkModule::CreateTGNetwork
================
*/
// FUNCTION: STBC 0x006b32c0
TGObject *TGNetworkModule::CreateTGNetwork( long id ) {
	return new TGNetwork( id );
}

/*
================
TGNetworkModule::CreateTGMessageEvent
================
*/
// FUNCTION: STBC 0x006b35c0
TGObject *TGNetworkModule::CreateTGMessageEvent( long id ) {
	return new TGMessageEvent( id );
}

/*
================
TGNetworkModule::CreateTGPlayerEvent
================
*/
// FUNCTION: STBC 0x006b36b0
TGObject *TGNetworkModule::CreateTGPlayerEvent( long id ) {
	return new TGPlayerEvent( id );
}

/*
================
TGNetworkModule::CreateTGGameSpyEvent
================
*/
// FUNCTION: STBC 0x006b37a0
TGObject *TGNetworkModule::CreateTGGameSpyEvent( long id ) {
	return new TGGameSpyEvent( id );
}
