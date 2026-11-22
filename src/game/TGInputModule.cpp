/*
===============================================================================

	TGInputModule.cpp

===============================================================================
*/

#include <iostream>
#include "TGGamepadEvent.h"
#include "TGIEvent.h"
#include "TGInputModule.h"
#include "TGInputManager.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGObject.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006e2fe0 _$E32
// FUNCTION: STBC 0x006e2ff0 _$E29
// FUNCTION: STBC 0x006e3000 _$E31
// FUNCTION: STBC 0x006e3010 _$E30
// FUNCTION: STBC 0x006e3020 _$E38
// FUNCTION: STBC 0x006e3030 _$E35
// FUNCTION: STBC 0x006e3040 _$E37
// FUNCTION: STBC 0x006e3050 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006e3060 _$E61
// FUNCTION: STBC 0x006e3070 _$E60
// FUNCTION: STBC 0x006e3080 _$E76
// FUNCTION: STBC 0x006e3090 _$E75
// FUNCTION: STBC 0x006e30a0 _$E80
// FUNCTION: STBC 0x006e30b0 _$E79
// FUNCTION: STBC 0x006e30c0 _$E84
// FUNCTION: STBC 0x006e30d0 _$E83
// FUNCTION: STBC 0x006e30e0 _$E88
// FUNCTION: STBC 0x006e30f0 _$E87
// FUNCTION: STBC 0x006e3100 _$E212
// FUNCTION: STBC 0x006e3110 _$E211
// FUNCTION: STBC 0x006e3120 _$E65
// FUNCTION: STBC 0x006e3130 _$E64
// FUNCTION: STBC 0x006e3140 _$E154
// FUNCTION: STBC 0x006e3150 _$E153
// FUNCTION: STBC 0x006e3160 _$E167
// FUNCTION: STBC 0x006e3170 _$E166
// FUNCTION: STBC 0x006e3180 _$E217
// FUNCTION: STBC 0x006e3190 _$E216
// FUNCTION: STBC 0x006e31a0 _$E222
// FUNCTION: STBC 0x006e31b0 _$E221
// FUNCTION: STBC 0x006e31c0 _$E226
// FUNCTION: STBC 0x006e31d0 _$E225
// FUNCTION: STBC 0x006e31e0 _$E230
// FUNCTION: STBC 0x006e31f0 _$E229
// FUNCTION: STBC 0x006e3200 _$E237
// FUNCTION: STBC 0x006e3210 _$E236
// FUNCTION: STBC 0x006e3220 _$E259
// FUNCTION: STBC 0x006e3230 _$E258
// FUNCTION: STBC 0x006e3240 _$E263
// FUNCTION: STBC 0x006e3250 _$E262
// FUNCTION: STBC 0x006e3260 _$E112
// FUNCTION: STBC 0x006e3270 _$E111
// FUNCTION: STBC 0x006e3280 _$E245
// FUNCTION: STBC 0x006e3290 _$E244
// FUNCTION: STBC 0x006e32a0 _$E252
// FUNCTION: STBC 0x006e32b0 _$E251
// FUNCTION: STBC 0x006e32c0 _$E275
// FUNCTION: STBC 0x006e32d0 _$E274
// FUNCTION: STBC 0x006e32e0 _$E279
// FUNCTION: STBC 0x006e32f0 _$E278
// FUNCTION: STBC 0x006e3300 _$E319
// FUNCTION: STBC 0x006e3310 _$E318
// FUNCTION: STBC 0x006e3320 _$E325
// FUNCTION: STBC 0x006e3330 _$E324
// FUNCTION: STBC 0x006e3340 _$E267
// FUNCTION: STBC 0x006e3350 _$E266
// FUNCTION: STBC 0x006e3360 _$E271
// FUNCTION: STBC 0x006e3370 _$E270
// FUNCTION: STBC 0x006e3380 _$E283
// FUNCTION: STBC 0x006e3390 _$E282
// FUNCTION: STBC 0x006e33a0 _$E287
// FUNCTION: STBC 0x006e33b0 _$E286
// FUNCTION: STBC 0x006e33c0 _$E291
// FUNCTION: STBC 0x006e33d0 _$E290
// FUNCTION: STBC 0x006e33e0 _$E295
// FUNCTION: STBC 0x006e33f0 _$E294
// FUNCTION: STBC 0x006e3400 _$E299
// FUNCTION: STBC 0x006e3410 _$E298
// FUNCTION: STBC 0x006e3420 _$E303
// FUNCTION: STBC 0x006e3430 _$E302
// FUNCTION: STBC 0x006e3440 _$E307
// FUNCTION: STBC 0x006e3450 _$E306
// FUNCTION: STBC 0x006e3460 _$E311
// FUNCTION: STBC 0x006e3470 _$E310
// FUNCTION: STBC 0x006e3480 _$E117
// FUNCTION: STBC 0x006e3490 _$E116
// FUNCTION: STBC 0x006e34a0 _$E315
// FUNCTION: STBC 0x006e34b0 _$E314
// FUNCTION: STBC 0x006e34c0 _$E122
// FUNCTION: STBC 0x006e34d0 _$E121
// FUNCTION: STBC 0x006e34e0 _$E329
// FUNCTION: STBC 0x006e34f0 _$E328
// FUNCTION: STBC 0x006e3500 _$E333
// FUNCTION: STBC 0x006e3510 _$E332
// FUNCTION: STBC 0x006e3520 _$E337
// FUNCTION: STBC 0x006e3530 _$E336
// FUNCTION: STBC 0x006e3540 _$E341
// FUNCTION: STBC 0x006e3550 _$E340
// FUNCTION: STBC 0x006e3560 _$E131
// FUNCTION: STBC 0x006e3570 _$E130
// FUNCTION: STBC 0x006e3580 _$E345
// FUNCTION: STBC 0x006e3590 _$E344
// FUNCTION: STBC 0x006e35a0 _$E349
// FUNCTION: STBC 0x006e35b0 _$E348
// FUNCTION: STBC 0x006e35c0 _$E353
// FUNCTION: STBC 0x006e35d0 _$E352
// FUNCTION: STBC 0x006e35e0 _$E137
// FUNCTION: STBC 0x006e35f0 _$E136
// FUNCTION: STBC 0x006e3600 _$E142
// FUNCTION: STBC 0x006e3610 _$E141
// FUNCTION: STBC 0x006e3620 _$E172
// FUNCTION: STBC 0x006e3630 _$E171
// FUNCTION: STBC 0x006e3640 _$E357
// FUNCTION: STBC 0x006e3650 _$E356
// FUNCTION: STBC 0x006e3660 _$E177
// FUNCTION: STBC 0x006e3670 _$E176
// FUNCTION: STBC 0x006e3680 _$E361
// FUNCTION: STBC 0x006e3690 _$E360
// FUNCTION: STBC 0x006e36a0 _$E365
// FUNCTION: STBC 0x006e36b0 _$E364
// FUNCTION: STBC 0x006e36c0 _$E182
// FUNCTION: STBC 0x006e36d0 _$E181
// FUNCTION: STBC 0x006e36e0 _$E369
// FUNCTION: STBC 0x006e36f0 _$E368
// FUNCTION: STBC 0x006e3700 _$E147
// FUNCTION: STBC 0x006e3710 _$E146
// FUNCTION: STBC 0x006e3720 _$E373
// FUNCTION: STBC 0x006e3730 _$E372
// FUNCTION: STBC 0x006e3740 _$E377
// FUNCTION: STBC 0x006e3750 _$E376
// FUNCTION: STBC 0x006e3760 _$E187
// FUNCTION: STBC 0x006e3770 _$E186
// FUNCTION: STBC 0x006e3780 _$E192
// FUNCTION: STBC 0x006e3790 _$E191
// FUNCTION: STBC 0x006e37a0 _$E381
// FUNCTION: STBC 0x006e37b0 _$E380
// FUNCTION: STBC 0x006e37c0 _$E385
// FUNCTION: STBC 0x006e37d0 _$E384
// FUNCTION: STBC 0x006e37e0 _$E389
// FUNCTION: STBC 0x006e37f0 _$E388
// FUNCTION: STBC 0x006e3800 _$E393
// FUNCTION: STBC 0x006e3810 _$E392
// FUNCTION: STBC 0x006e3820 _$E397
// FUNCTION: STBC 0x006e3830 _$E396
// FUNCTION: STBC 0x006e3840 _$E401
// FUNCTION: STBC 0x006e3850 _$E400
// FUNCTION: STBC 0x006e3860 _$E405
// FUNCTION: STBC 0x006e3870 _$E404
// FUNCTION: STBC 0x006e3880 _$E409
// FUNCTION: STBC 0x006e3890 _$E408
// FUNCTION: STBC 0x006e38a0 _$E197
// FUNCTION: STBC 0x006e38b0 _$E196
// FUNCTION: STBC 0x006e38c0 _$E413
// FUNCTION: STBC 0x006e38d0 _$E412
// FUNCTION: STBC 0x006e38e0 _$E202
// FUNCTION: STBC 0x006e38f0 _$E201
// FUNCTION: STBC 0x006e3900 _$E207
// FUNCTION: STBC 0x006e3910 _$E206

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006e3920 _$E418
// FUNCTION: STBC 0x006e3930 _$E415
// FUNCTION: STBC 0x006e3940 _$E417
// FUNCTION: STBC 0x006e3950 _$E416
// DATA: STBC 0x009991d0
TGInputModule inputModule;		// invented name

/*
================
TGInputModule::VirtualSlot00

The startup: the input event classes, then the input manager.
================
*/
// FUNCTION: STBC 0x006e3990
void TGInputModule::VirtualSlot00( void ) {
	RegisterClasses();
	RegisterHandlerNames();
	g_kInputManager.Unknown6E4D80( 7 );
}

/*
================
TGInputModule::VirtualSlot01
================
*/
// FUNCTION: STBC 0x006e39b0
void TGInputModule::VirtualSlot01( void ) {
	RegisterHandlers();
	Unknown6E3E30();
}

/*
================
TGInputModule::VirtualSlot02
================
*/
// FUNCTION: STBC 0x006e39d0
void TGInputModule::VirtualSlot02( void ) {
	g_kInputManager.Unknown6E71C0();
}

/*
================
TGInputModule::VirtualSlot04
================
*/
// FUNCTION: STBC 0x006e39e0
void TGInputModule::VirtualSlot04( void ) {
}

/*
================
TGInputModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x006e39f0
void TGInputModule::VirtualSlot05( void ) {
	g_kInputManager.Unknown6E5090();
}

/*
================
TGInputModule::RegisterClasses
================
*/
// FUNCTION: STBC 0x006e3a00
void TGInputModule::RegisterClasses( void ) {
	TGObject::RegisterCreator( TGIEVENT_TYPE, CreateTGIEvent );
	TGObject::RegisterCreator( TGKEYBOARDEVENT_TYPE, CreateTGKeyboardEvent );
	TGObject::RegisterCreator( TGMOUSEEVENT_TYPE, CreateTGMouseEvent );
	TGObject::RegisterCreator( TGGAMEPADEVENT_TYPE, CreateTGGamepadEvent );
}

/*
================
TGInputModule::CreateTGIEvent
================
*/
// FUNCTION: STBC 0x006e3cd0
TGObject *TGInputModule::CreateTGIEvent( long id ) {
	return new TGIEvent( id );
}

/*
================
TGInputModule::CreateTGMouseEvent
================
*/
// FUNCTION: STBC 0x006e3d20
TGObject *TGInputModule::CreateTGMouseEvent( long id ) {
	return new TGMouseEvent( id );
}

/*
================
TGInputModule::CreateTGKeyboardEvent
================
*/
// FUNCTION: STBC 0x006e3d70
TGObject *TGInputModule::CreateTGKeyboardEvent( long id ) {
	return new TGKeyboardEvent( id );
}

/*
================
TGInputModule::CreateTGGamepadEvent
================
*/
// FUNCTION: STBC 0x006e3dc0
TGObject *TGInputModule::CreateTGGamepadEvent( long id ) {
	return new TGGamepadEvent( id );
}

/*
================
TGInputModule::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x006e3e10
void TGInputModule::RegisterHandlerNames( void ) {
}

/*
================
TGInputModule::RegisterHandlers
================
*/
// FUNCTION: STBC 0x006e3e20
void TGInputModule::RegisterHandlers( void ) {
}

/*
================
TGInputModule::Unknown6E3E30
================
*/
// FUNCTION: STBC 0x006e3e30
void TGInputModule::Unknown6E3E30( void ) {
}
