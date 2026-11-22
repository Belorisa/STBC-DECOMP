/*
===============================================================================

	TGModelPropertyModule.cpp

	The model property module, and the model property manager's instance.
	The unit's name is invented, after its invented class: retail's unit
	runs from the module's static quad at 0x6E8910 through the module, its
	<iostream> quads (0x6E8B40) and NetImmerse registrations
	(0x6E8BC0-0x6E94AF), the creators and the manager's static quad to the
	manager's destructor copy at 0x6E9570. Its .bss holds the module
	(0x999CC0), the registrations' stores, the filter's guard (0x999DC4) and
	then the manager (0x999DE8), ahead of the manager's own unit.

===============================================================================
*/

#include <iostream>

#include "TGModelPropertyModule.h"
#include "TGModelPropertyManager.h"
#include "TGModelProperty.h"
#include "TGModelPropertySet.h"
#include "TGObject.h"
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006e8bc0 _$E55
// FUNCTION: STBC 0x006e8bd0 _$E54
// FUNCTION: STBC 0x006e8be0 _$E59
// FUNCTION: STBC 0x006e8bf0 _$E58
// FUNCTION: STBC 0x006e8c00 _$E148
// FUNCTION: STBC 0x006e8c10 _$E147
// FUNCTION: STBC 0x006e8c20 _$E161
// FUNCTION: STBC 0x006e8c30 _$E160
// FUNCTION: STBC 0x006e8c40 _$E70
// FUNCTION: STBC 0x006e8c50 _$E69
// FUNCTION: STBC 0x006e8c60 _$E74
// FUNCTION: STBC 0x006e8c70 _$E73
// FUNCTION: STBC 0x006e8cb0 _$E78
// FUNCTION: STBC 0x006e8cc0 _$E77
// FUNCTION: STBC 0x006e8cd0 _$E82
// FUNCTION: STBC 0x006e8ce0 _$E81
// FUNCTION: STBC 0x006e8cf0 _$E206
// FUNCTION: STBC 0x006e8d00 _$E205
// FUNCTION: STBC 0x006e8d10 _$E211
// FUNCTION: STBC 0x006e8d20 _$E210
// FUNCTION: STBC 0x006e8d30 _$E216
// FUNCTION: STBC 0x006e8d40 _$E215
// FUNCTION: STBC 0x006e8d50 _$E220
// FUNCTION: STBC 0x006e8d60 _$E219
// FUNCTION: STBC 0x006e8d70 _$E224
// FUNCTION: STBC 0x006e8d80 _$E223
// FUNCTION: STBC 0x006e8d90 _$E231
// FUNCTION: STBC 0x006e8da0 _$E230
// FUNCTION: STBC 0x006e8db0 _$E256
// FUNCTION: STBC 0x006e8dc0 _$E255
// FUNCTION: STBC 0x006e8dd0 _$E260
// FUNCTION: STBC 0x006e8de0 _$E259
// FUNCTION: STBC 0x006e8df0 _$E106
// FUNCTION: STBC 0x006e8e00 _$E105
// FUNCTION: STBC 0x006e8e10 _$E242
// FUNCTION: STBC 0x006e8e20 _$E241
// FUNCTION: STBC 0x006e8e30 _$E249
// FUNCTION: STBC 0x006e8e40 _$E248
// FUNCTION: STBC 0x006e8e50 _$E272
// FUNCTION: STBC 0x006e8e60 _$E271
// FUNCTION: STBC 0x006e8e70 _$E276
// FUNCTION: STBC 0x006e8e80 _$E275
// FUNCTION: STBC 0x006e8e90 _$E316
// FUNCTION: STBC 0x006e8ea0 _$E315
// FUNCTION: STBC 0x006e8eb0 _$E322
// FUNCTION: STBC 0x006e8ec0 _$E321
// FUNCTION: STBC 0x006e8ed0 _$E264
// FUNCTION: STBC 0x006e8ee0 _$E263
// FUNCTION: STBC 0x006e8ef0 _$E268
// FUNCTION: STBC 0x006e8f00 _$E267
// FUNCTION: STBC 0x006e8f10 _$E280
// FUNCTION: STBC 0x006e8f20 _$E279
// FUNCTION: STBC 0x006e8f30 _$E284
// FUNCTION: STBC 0x006e8f40 _$E283
// FUNCTION: STBC 0x006e8f50 _$E288
// FUNCTION: STBC 0x006e8f60 _$E287
// FUNCTION: STBC 0x006e8f70 _$E292
// FUNCTION: STBC 0x006e8f80 _$E291
// FUNCTION: STBC 0x006e8f90 _$E296
// FUNCTION: STBC 0x006e8fa0 _$E295
// FUNCTION: STBC 0x006e8fb0 _$E300
// FUNCTION: STBC 0x006e8fc0 _$E299
// FUNCTION: STBC 0x006e8fd0 _$E304
// FUNCTION: STBC 0x006e8fe0 _$E303
// FUNCTION: STBC 0x006e8ff0 _$E308
// FUNCTION: STBC 0x006e9000 _$E307
// FUNCTION: STBC 0x006e9010 _$E111
// FUNCTION: STBC 0x006e9020 _$E110
// FUNCTION: STBC 0x006e9030 _$E312
// FUNCTION: STBC 0x006e9040 _$E311
// FUNCTION: STBC 0x006e9050 _$E116
// FUNCTION: STBC 0x006e9060 _$E115
// FUNCTION: STBC 0x006e9070 _$E326
// FUNCTION: STBC 0x006e9080 _$E325
// FUNCTION: STBC 0x006e9090 _$E330
// FUNCTION: STBC 0x006e90a0 _$E329
// FUNCTION: STBC 0x006e90b0 _$E334
// FUNCTION: STBC 0x006e90c0 _$E333
// FUNCTION: STBC 0x006e90d0 _$E338
// FUNCTION: STBC 0x006e90e0 _$E337
// FUNCTION: STBC 0x006e90f0 _$E125
// FUNCTION: STBC 0x006e9100 _$E124
// FUNCTION: STBC 0x006e9110 _$E342
// FUNCTION: STBC 0x006e9120 _$E341
// FUNCTION: STBC 0x006e9130 _$E346
// FUNCTION: STBC 0x006e9140 _$E345
// FUNCTION: STBC 0x006e9150 _$E350
// FUNCTION: STBC 0x006e9160 _$E349
// FUNCTION: STBC 0x006e9170 _$E131
// FUNCTION: STBC 0x006e9180 _$E130
// FUNCTION: STBC 0x006e9190 _$E136
// FUNCTION: STBC 0x006e91a0 _$E135
// FUNCTION: STBC 0x006e91b0 _$E166
// FUNCTION: STBC 0x006e91c0 _$E165
// FUNCTION: STBC 0x006e91d0 _$E354
// FUNCTION: STBC 0x006e91e0 _$E353
// FUNCTION: STBC 0x006e91f0 _$E171
// FUNCTION: STBC 0x006e9200 _$E170
// FUNCTION: STBC 0x006e9210 _$E358
// FUNCTION: STBC 0x006e9220 _$E357
// FUNCTION: STBC 0x006e9230 _$E362
// FUNCTION: STBC 0x006e9240 _$E361
// FUNCTION: STBC 0x006e9250 _$E176
// FUNCTION: STBC 0x006e9260 _$E175
// FUNCTION: STBC 0x006e9270 _$E366
// FUNCTION: STBC 0x006e9280 _$E365
// FUNCTION: STBC 0x006e9290 _$E141
// FUNCTION: STBC 0x006e92a0 _$E140
// FUNCTION: STBC 0x006e92b0 _$E370
// FUNCTION: STBC 0x006e92c0 _$E369
// FUNCTION: STBC 0x006e92d0 _$E374
// FUNCTION: STBC 0x006e92e0 _$E373
// FUNCTION: STBC 0x006e92f0 _$E181
// FUNCTION: STBC 0x006e9300 _$E180
// FUNCTION: STBC 0x006e9310 _$E186
// FUNCTION: STBC 0x006e9320 _$E185
// FUNCTION: STBC 0x006e9330 _$E378
// FUNCTION: STBC 0x006e9340 _$E377
// FUNCTION: STBC 0x006e9350 _$E382
// FUNCTION: STBC 0x006e9360 _$E381
// FUNCTION: STBC 0x006e9370 _$E386
// FUNCTION: STBC 0x006e9380 _$E385
// FUNCTION: STBC 0x006e9390 _$E390
// FUNCTION: STBC 0x006e93a0 _$E389
// FUNCTION: STBC 0x006e93b0 _$E394
// FUNCTION: STBC 0x006e93c0 _$E393
// FUNCTION: STBC 0x006e93d0 _$E398
// FUNCTION: STBC 0x006e93e0 _$E397
// FUNCTION: STBC 0x006e93f0 _$E402
// FUNCTION: STBC 0x006e9400 _$E401
// FUNCTION: STBC 0x006e9410 _$E406
// FUNCTION: STBC 0x006e9420 _$E405
// FUNCTION: STBC 0x006e9430 _$E191
// FUNCTION: STBC 0x006e9440 _$E190
// FUNCTION: STBC 0x006e9450 _$E410
// FUNCTION: STBC 0x006e9460 _$E409
// FUNCTION: STBC 0x006e9470 _$E196
// FUNCTION: STBC 0x006e9480 _$E195
// FUNCTION: STBC 0x006e9490 _$E201
// FUNCTION: STBC 0x006e94a0 _$E200

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006e8c80 _$E428
// FUNCTION: STBC 0x006e8ca0 _$E427

// The standard streams' initialisers and exit-time destructors (<iostream>):
// retail lays them out after the module's code.
// FUNCTION: STBC 0x006e8b40 _$E32
// FUNCTION: STBC 0x006e8b50 _$E29
// FUNCTION: STBC 0x006e8b60 _$E31
// FUNCTION: STBC 0x006e8b70 _$E30
// FUNCTION: STBC 0x006e8b80 _$E38
// FUNCTION: STBC 0x006e8b90 _$E35
// FUNCTION: STBC 0x006e8ba0 _$E37
// FUNCTION: STBC 0x006e8bb0 _$E36

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006e8910 _$E415
// FUNCTION: STBC 0x006e8920 _$E412
// FUNCTION: STBC 0x006e8930 _$E414
// FUNCTION: STBC 0x006e8940 _$E413
// DATA: STBC 0x00999cc0
TGModelPropertyModule modelPropertyModule;		// invented name

/*
================
TGModelPropertyModule::VirtualSlot00

The startup: the creators of the property classes, and the generic filter
the first time.
================
*/
// FUNCTION: STBC 0x006e8980
void TGModelPropertyModule::VirtualSlot00( void ) {
	TGObject::RegisterCreator( TGMODELPROPERTY_TYPE, CreateTGModelProperty );
	TGObject::RegisterCreator( TGMODELPROPERTYSET_TYPE, CreateTGModelPropertySet );

	static bool filterRegistered = false;		// invented name
	if ( !filterRegistered ) {
		filterRegistered = true;
		g_kModelPropertyManager.RegisterFilter( GenericPropertyFilter, "Generic Property" );
	}
}

/*
================
TGModelPropertyModule::VirtualSlot04

The end: the local and global templates go.
================
*/
// FUNCTION: STBC 0x006e8b10
void TGModelPropertyModule::VirtualSlot04( void ) {
	g_kModelPropertyManager.ClearLocalTemplates();
	g_kModelPropertyManager.ClearGlobalTemplates();
}

/*
================
TGModelPropertyModule::VirtualSlot05

The shutdown: the registered filters go.
================
*/
// FUNCTION: STBC 0x006e8b30
void TGModelPropertyModule::VirtualSlot05( void ) {
	g_kModelPropertyManager.ClearRegisteredFilters();
}

/*
================
TGModelPropertyModule::CreateTGModelProperty
================
*/
// FUNCTION: STBC 0x006e94b0
TGObject *TGModelPropertyModule::CreateTGModelProperty( long id ) {
	return new TGModelProperty( id );
}

/*
================
TGModelPropertyModule::GenericPropertyFilter
================
*/
// FUNCTION: STBC 0x006e94e0
bool TGModelPropertyModule::GenericPropertyFilter( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( TGMODELPROPERTY_TYPE ) ) {
		return true;
	}
	return false;
}

/*
================
TGModelPropertyModule::CreateTGModelPropertySet
================
*/
// FUNCTION: STBC 0x006e9500
TGObject *TGModelPropertyModule::CreateTGModelPropertySet( long id ) {
	return new TGModelPropertySet( id );
}

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006e9530 _$E420
// FUNCTION: STBC 0x006e9540 _$E417
// FUNCTION: STBC 0x006e9550 _$E419
// FUNCTION: STBC 0x006e9560 _$E418
// DATA: STBC 0x00999de8
TGModelPropertyManager g_kModelPropertyManager;
