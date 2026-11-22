/*
===============================================================================

	HullClass.cpp

===============================================================================
*/

#include <iostream>
#include "HullClass.h"
#include "TGEventHandlerTable.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00560950 _$E448
// FUNCTION: STBC 0x00560970 _$E447

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00560570 _$E32
// FUNCTION: STBC 0x00560580 _$E29
// FUNCTION: STBC 0x00560590 _$E31
// FUNCTION: STBC 0x005605a0 _$E30
// FUNCTION: STBC 0x005605b0 _$E38
// FUNCTION: STBC 0x005605c0 _$E35
// FUNCTION: STBC 0x005605d0 _$E37
// FUNCTION: STBC 0x005605e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005605f0 _$E71
// FUNCTION: STBC 0x00560600 _$E70
// FUNCTION: STBC 0x00560610 _$E75
// FUNCTION: STBC 0x00560620 _$E74
// FUNCTION: STBC 0x00560630 _$E86
// FUNCTION: STBC 0x00560640 _$E85
// FUNCTION: STBC 0x00560650 _$E90
// FUNCTION: STBC 0x00560660 _$E89
// FUNCTION: STBC 0x00560670 _$E94
// FUNCTION: STBC 0x00560680 _$E93
// FUNCTION: STBC 0x00560690 _$E98
// FUNCTION: STBC 0x005606a0 _$E97
// FUNCTION: STBC 0x005606b0 _$E217
// FUNCTION: STBC 0x005606c0 _$E216
// FUNCTION: STBC 0x005606d0 _$E159
// FUNCTION: STBC 0x005606e0 _$E158
// FUNCTION: STBC 0x005606f0 _$E172
// FUNCTION: STBC 0x00560700 _$E171
// FUNCTION: STBC 0x00560710 _$E222
// FUNCTION: STBC 0x00560720 _$E221
// FUNCTION: STBC 0x00560730 _$E227
// FUNCTION: STBC 0x00560740 _$E226
// FUNCTION: STBC 0x00560750 _$E231
// FUNCTION: STBC 0x00560760 _$E230
// FUNCTION: STBC 0x00560770 _$E235
// FUNCTION: STBC 0x00560780 _$E234
// FUNCTION: STBC 0x00560790 _$E242
// FUNCTION: STBC 0x005607a0 _$E241
// FUNCTION: STBC 0x005607b0 _$E267
// FUNCTION: STBC 0x005607c0 _$E266
// FUNCTION: STBC 0x005607d0 _$E271
// FUNCTION: STBC 0x005607e0 _$E270
// FUNCTION: STBC 0x005607f0 _$E117
// FUNCTION: STBC 0x00560800 _$E116
// FUNCTION: STBC 0x00560810 _$E253
// FUNCTION: STBC 0x00560820 _$E252
// FUNCTION: STBC 0x00560830 _$E260
// FUNCTION: STBC 0x00560840 _$E259
// FUNCTION: STBC 0x00560850 _$E283
// FUNCTION: STBC 0x00560860 _$E282
// FUNCTION: STBC 0x00560870 _$E287
// FUNCTION: STBC 0x00560880 _$E286
// FUNCTION: STBC 0x00560890 _$E327
// FUNCTION: STBC 0x005608a0 _$E326
// FUNCTION: STBC 0x005608b0 _$E333
// FUNCTION: STBC 0x005608c0 _$E332
// FUNCTION: STBC 0x005608d0 _$E275
// FUNCTION: STBC 0x005608e0 _$E274
// FUNCTION: STBC 0x005608f0 _$E279
// FUNCTION: STBC 0x00560900 _$E278
// FUNCTION: STBC 0x00560910 _$E291
// FUNCTION: STBC 0x00560920 _$E290
// FUNCTION: STBC 0x00560930 _$E295
// FUNCTION: STBC 0x00560940 _$E294
// FUNCTION: STBC 0x00560980 _$E299
// FUNCTION: STBC 0x00560990 _$E298
// FUNCTION: STBC 0x005609a0 _$E303
// FUNCTION: STBC 0x005609b0 _$E302
// FUNCTION: STBC 0x005609c0 _$E307
// FUNCTION: STBC 0x005609d0 _$E306
// FUNCTION: STBC 0x005609e0 _$E311
// FUNCTION: STBC 0x005609f0 _$E310
// FUNCTION: STBC 0x00560a00 _$E315
// FUNCTION: STBC 0x00560a10 _$E314
// FUNCTION: STBC 0x00560a20 _$E319
// FUNCTION: STBC 0x00560a30 _$E318
// FUNCTION: STBC 0x00560a40 _$E122
// FUNCTION: STBC 0x00560a50 _$E121
// FUNCTION: STBC 0x00560a60 _$E323
// FUNCTION: STBC 0x00560a70 _$E322
// FUNCTION: STBC 0x00560a80 _$E127
// FUNCTION: STBC 0x00560a90 _$E126
// FUNCTION: STBC 0x00560aa0 _$E337
// FUNCTION: STBC 0x00560ab0 _$E336
// FUNCTION: STBC 0x00560ac0 _$E341
// FUNCTION: STBC 0x00560ad0 _$E340
// FUNCTION: STBC 0x00560ae0 _$E345
// FUNCTION: STBC 0x00560af0 _$E344
// FUNCTION: STBC 0x00560b00 _$E349
// FUNCTION: STBC 0x00560b10 _$E348
// FUNCTION: STBC 0x00560b20 _$E136
// FUNCTION: STBC 0x00560b30 _$E135
// FUNCTION: STBC 0x00560b40 _$E353
// FUNCTION: STBC 0x00560b50 _$E352
// FUNCTION: STBC 0x00560b60 _$E357
// FUNCTION: STBC 0x00560b70 _$E356
// FUNCTION: STBC 0x00560b80 _$E361
// FUNCTION: STBC 0x00560b90 _$E360
// FUNCTION: STBC 0x00560ba0 _$E142
// FUNCTION: STBC 0x00560bb0 _$E141
// FUNCTION: STBC 0x00560bc0 _$E147
// FUNCTION: STBC 0x00560bd0 _$E146
// FUNCTION: STBC 0x00560be0 _$E177
// FUNCTION: STBC 0x00560bf0 _$E176
// FUNCTION: STBC 0x00560c00 _$E365
// FUNCTION: STBC 0x00560c10 _$E364
// FUNCTION: STBC 0x00560c20 _$E182
// FUNCTION: STBC 0x00560c30 _$E181
// FUNCTION: STBC 0x00560c40 _$E369
// FUNCTION: STBC 0x00560c50 _$E368
// FUNCTION: STBC 0x00560c60 _$E373
// FUNCTION: STBC 0x00560c70 _$E372
// FUNCTION: STBC 0x00560c80 _$E187
// FUNCTION: STBC 0x00560c90 _$E186
// FUNCTION: STBC 0x00560ca0 _$E377
// FUNCTION: STBC 0x00560cb0 _$E376
// FUNCTION: STBC 0x00560cc0 _$E152
// FUNCTION: STBC 0x00560cd0 _$E151
// FUNCTION: STBC 0x00560ce0 _$E381
// FUNCTION: STBC 0x00560cf0 _$E380
// FUNCTION: STBC 0x00560d00 _$E385
// FUNCTION: STBC 0x00560d10 _$E384
// FUNCTION: STBC 0x00560d20 _$E192
// FUNCTION: STBC 0x00560d30 _$E191
// FUNCTION: STBC 0x00560d40 _$E197
// FUNCTION: STBC 0x00560d50 _$E196
// FUNCTION: STBC 0x00560d60 _$E389
// FUNCTION: STBC 0x00560d70 _$E388
// FUNCTION: STBC 0x00560d80 _$E393
// FUNCTION: STBC 0x00560d90 _$E392
// FUNCTION: STBC 0x00560da0 _$E397
// FUNCTION: STBC 0x00560db0 _$E396
// FUNCTION: STBC 0x00560dc0 _$E401
// FUNCTION: STBC 0x00560dd0 _$E400
// FUNCTION: STBC 0x00560de0 _$E405
// FUNCTION: STBC 0x00560df0 _$E404
// FUNCTION: STBC 0x00560e00 _$E409
// FUNCTION: STBC 0x00560e10 _$E408
// FUNCTION: STBC 0x00560e20 _$E413
// FUNCTION: STBC 0x00560e30 _$E412
// FUNCTION: STBC 0x00560e40 _$E417
// FUNCTION: STBC 0x00560e50 _$E416
// FUNCTION: STBC 0x00560e60 _$E202
// FUNCTION: STBC 0x00560e70 _$E201
// FUNCTION: STBC 0x00560e80 _$E421
// FUNCTION: STBC 0x00560e90 _$E420
// FUNCTION: STBC 0x00560ea0 _$E207
// FUNCTION: STBC 0x00560eb0 _$E206
// FUNCTION: STBC 0x00560ec0 _$E212
// FUNCTION: STBC 0x00560ed0 _$E211
// FUNCTION: STBC 0x00560ee0 _$E425
// FUNCTION: STBC 0x00560ef0 _$E424
// FUNCTION: STBC 0x00560f00 _$E430
// FUNCTION: STBC 0x00560f10 _$E429
// FUNCTION: STBC 0x00560f20 _$E434
// FUNCTION: STBC 0x00560f30 _$E433

// The handler table's dynamic initialiser and exit-time destructor, compiled
// favouring size: retail packs the four without alignment and tests the guard
// with `test byte ptr` (0x560400-0x560437, the only such quad in retail), which
// only size optimisation at the definition gives. The pragma is reconstructed
// from those bytes alone; the table's guard and storage lie among this unit's
// own registration stores (0x98B92C, 0x98B940), so it is this unit's.
// FUNCTION: STBC 0x00560400 _$E440
// FUNCTION: STBC 0x0056040a _$E436
// FUNCTION: STBC 0x00560414 _$E439
// FUNCTION: STBC 0x00560420 _$E437
// MATCH-PENDING: R17, the size-optimisation pragma is reconstructed from these bytes alone; no evidence shows the original had it (user's ruling, 2026-11-22)
#pragma optimize( "s", on )
// DATA: STBC 0x0098b940
TGEventHandlerTable HullClass::classHandlers;
#pragma optimize( "", on )

/*
================
HullClass::Cast

The object as a HullClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00560440
HullClass *HullClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( HULLCLASS_TYPE ) ) {
		return (HullClass *)object;
	}
	return NULL;
}

/*
================
HullClass::HullClass
================
*/
// FUNCTION: STBC 0x00560470
HullClass::HullClass( long id ) : ShipSubsystem( id ) {
}

/*
================
HullClass::GetObjType
================
*/
// FUNCTION: STBC 0x00560490
int HullClass::GetObjType( void ) {
	return HULLCLASS_TYPE;
}

/*
================
HullClass::GetClassName
================
*/
// FUNCTION: STBC 0x005604f0
char *HullClass::GetClassName( void ) {
	return "HullClass";
}

/*
================
HullClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00560500
char *HullClass::GetSwigTypeName( void ) {
	return "_p_HullClass";
}

/*
================
HullClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00560510
char *HullClass::GetPythonClassName( void ) {
	return "HullClassPtr";
}

/*
================
HullClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00560520
TGEventHandlerTable *HullClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
HullClass::~HullClass
================
*/
// FUNCTION: STBC 0x00560560
// FUNCTION: STBC 0x00560530 ??_GHullClass@@UAEPAXI@Z
HullClass::~HullClass( void ) {
}
