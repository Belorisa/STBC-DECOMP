/*
===============================================================================

	SetFloatVarEvent.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include "SetFloatVarEvent.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00448f70 _$E55
// FUNCTION: STBC 0x00448f80 _$E54
// FUNCTION: STBC 0x00448f90 _$E59
// FUNCTION: STBC 0x00448fa0 _$E58
// FUNCTION: STBC 0x00448fb0 _$E70
// FUNCTION: STBC 0x00448fc0 _$E69
// FUNCTION: STBC 0x00448fd0 _$E74
// FUNCTION: STBC 0x00448fe0 _$E73
// FUNCTION: STBC 0x00448ff0 _$E78
// FUNCTION: STBC 0x00449000 _$E77
// FUNCTION: STBC 0x00449010 _$E82
// FUNCTION: STBC 0x00449020 _$E81
// FUNCTION: STBC 0x00449030 _$E206
// FUNCTION: STBC 0x00449040 _$E205
// FUNCTION: STBC 0x00449050 _$E148
// FUNCTION: STBC 0x00449060 _$E147
// FUNCTION: STBC 0x00449070 _$E161
// FUNCTION: STBC 0x00449080 _$E160
// FUNCTION: STBC 0x00449090 _$E211
// FUNCTION: STBC 0x004490a0 _$E210
// FUNCTION: STBC 0x004490b0 _$E216
// FUNCTION: STBC 0x004490c0 _$E215
// FUNCTION: STBC 0x004490d0 _$E220
// FUNCTION: STBC 0x004490e0 _$E219
// FUNCTION: STBC 0x004490f0 _$E224
// FUNCTION: STBC 0x00449100 _$E223
// FUNCTION: STBC 0x00449110 _$E231
// FUNCTION: STBC 0x00449120 _$E230
// FUNCTION: STBC 0x00449130 _$E256
// FUNCTION: STBC 0x00449140 _$E255
// FUNCTION: STBC 0x00449150 _$E260
// FUNCTION: STBC 0x00449160 _$E259
// FUNCTION: STBC 0x00449170 _$E106
// FUNCTION: STBC 0x00449180 _$E105
// FUNCTION: STBC 0x00449190 _$E242
// FUNCTION: STBC 0x004491a0 _$E241
// FUNCTION: STBC 0x004491b0 _$E249
// FUNCTION: STBC 0x004491c0 _$E248
// FUNCTION: STBC 0x004491d0 _$E272
// FUNCTION: STBC 0x004491e0 _$E271
// FUNCTION: STBC 0x004491f0 _$E276
// FUNCTION: STBC 0x00449200 _$E275
// FUNCTION: STBC 0x00449210 _$E316
// FUNCTION: STBC 0x00449220 _$E315
// FUNCTION: STBC 0x00449230 _$E322
// FUNCTION: STBC 0x00449240 _$E321
// FUNCTION: STBC 0x00449250 _$E264
// FUNCTION: STBC 0x00449260 _$E263
// FUNCTION: STBC 0x00449270 _$E268
// FUNCTION: STBC 0x00449280 _$E267
// FUNCTION: STBC 0x00449290 _$E280
// FUNCTION: STBC 0x004492a0 _$E279
// FUNCTION: STBC 0x004492e0 _$E284
// FUNCTION: STBC 0x004492f0 _$E283
// FUNCTION: STBC 0x00449300 _$E288
// FUNCTION: STBC 0x00449310 _$E287
// FUNCTION: STBC 0x00449320 _$E292
// FUNCTION: STBC 0x00449330 _$E291
// FUNCTION: STBC 0x00449340 _$E296
// FUNCTION: STBC 0x00449350 _$E295
// FUNCTION: STBC 0x00449360 _$E300
// FUNCTION: STBC 0x00449370 _$E299
// FUNCTION: STBC 0x00449380 _$E304
// FUNCTION: STBC 0x00449390 _$E303
// FUNCTION: STBC 0x004493a0 _$E308
// FUNCTION: STBC 0x004493b0 _$E307
// FUNCTION: STBC 0x004493c0 _$E111
// FUNCTION: STBC 0x004493d0 _$E110
// FUNCTION: STBC 0x004493e0 _$E312
// FUNCTION: STBC 0x004493f0 _$E311
// FUNCTION: STBC 0x00449400 _$E116
// FUNCTION: STBC 0x00449410 _$E115
// FUNCTION: STBC 0x00449420 _$E326
// FUNCTION: STBC 0x00449430 _$E325
// FUNCTION: STBC 0x00449440 _$E330
// FUNCTION: STBC 0x00449450 _$E329
// FUNCTION: STBC 0x00449460 _$E334
// FUNCTION: STBC 0x00449470 _$E333
// FUNCTION: STBC 0x00449480 _$E338
// FUNCTION: STBC 0x00449490 _$E337
// FUNCTION: STBC 0x004494a0 _$E125
// FUNCTION: STBC 0x004494b0 _$E124
// FUNCTION: STBC 0x004494c0 _$E342
// FUNCTION: STBC 0x004494d0 _$E341
// FUNCTION: STBC 0x004494e0 _$E346
// FUNCTION: STBC 0x004494f0 _$E345
// FUNCTION: STBC 0x00449500 _$E350
// FUNCTION: STBC 0x00449510 _$E349
// FUNCTION: STBC 0x00449520 _$E131
// FUNCTION: STBC 0x00449530 _$E130
// FUNCTION: STBC 0x00449540 _$E136
// FUNCTION: STBC 0x00449550 _$E135
// FUNCTION: STBC 0x00449560 _$E166
// FUNCTION: STBC 0x00449570 _$E165
// FUNCTION: STBC 0x00449580 _$E354
// FUNCTION: STBC 0x00449590 _$E353
// FUNCTION: STBC 0x004495a0 _$E171
// FUNCTION: STBC 0x004495b0 _$E170
// FUNCTION: STBC 0x004495c0 _$E358
// FUNCTION: STBC 0x004495d0 _$E357
// FUNCTION: STBC 0x004495e0 _$E362
// FUNCTION: STBC 0x004495f0 _$E361
// FUNCTION: STBC 0x00449600 _$E176
// FUNCTION: STBC 0x00449610 _$E175
// FUNCTION: STBC 0x00449620 _$E366
// FUNCTION: STBC 0x00449630 _$E365
// FUNCTION: STBC 0x00449640 _$E141
// FUNCTION: STBC 0x00449650 _$E140
// FUNCTION: STBC 0x00449660 _$E370
// FUNCTION: STBC 0x00449670 _$E369
// FUNCTION: STBC 0x00449680 _$E374
// FUNCTION: STBC 0x00449690 _$E373
// FUNCTION: STBC 0x004496a0 _$E181
// FUNCTION: STBC 0x004496b0 _$E180
// FUNCTION: STBC 0x004496c0 _$E186
// FUNCTION: STBC 0x004496d0 _$E185
// FUNCTION: STBC 0x004496e0 _$E378
// FUNCTION: STBC 0x004496f0 _$E377
// FUNCTION: STBC 0x00449700 _$E382
// FUNCTION: STBC 0x00449710 _$E381
// FUNCTION: STBC 0x00449720 _$E386
// FUNCTION: STBC 0x00449730 _$E385
// FUNCTION: STBC 0x00449740 _$E390
// FUNCTION: STBC 0x00449750 _$E389
// FUNCTION: STBC 0x00449760 _$E394
// FUNCTION: STBC 0x00449770 _$E393
// FUNCTION: STBC 0x00449780 _$E398
// FUNCTION: STBC 0x00449790 _$E397
// FUNCTION: STBC 0x004497a0 _$E402
// FUNCTION: STBC 0x004497b0 _$E401
// FUNCTION: STBC 0x004497c0 _$E406
// FUNCTION: STBC 0x004497d0 _$E405
// FUNCTION: STBC 0x004497e0 _$E191
// FUNCTION: STBC 0x004497f0 _$E190
// FUNCTION: STBC 0x00449800 _$E410
// FUNCTION: STBC 0x00449810 _$E409
// FUNCTION: STBC 0x00449820 _$E196
// FUNCTION: STBC 0x00449830 _$E195
// FUNCTION: STBC 0x00449840 _$E201
// FUNCTION: STBC 0x00449850 _$E200
// FUNCTION: STBC 0x00449860 _$E414
// FUNCTION: STBC 0x00449870 _$E413
// FUNCTION: STBC 0x00449880 _$E419
// FUNCTION: STBC 0x00449890 _$E418
// FUNCTION: STBC 0x004498a0 _$E423
// FUNCTION: STBC 0x004498b0 _$E422

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale id pair.
// FUNCTION: STBC 0x00448ef0 _$E32
// FUNCTION: STBC 0x00448f00 _$E29
// FUNCTION: STBC 0x00448f10 _$E31
// FUNCTION: STBC 0x00448f20 _$E30
// FUNCTION: STBC 0x00448f30 _$E38
// FUNCTION: STBC 0x00448f40 _$E35
// FUNCTION: STBC 0x00448f50 _$E37
// FUNCTION: STBC 0x00448f60 _$E36
// FUNCTION: STBC 0x004492b0 _$E431
// FUNCTION: STBC 0x004492d0 _$E430

/*
================
SetFloatVarEvent::SetFloatVarEvent
================
*/
// FUNCTION: STBC 0x00448c90 ??0SetFloatVarEvent@@QAE@J@Z
SetFloatVarEvent::SetFloatVarEvent( long id ) : TGEvent( id ) {
	scope = NULL;
	name = NULL;
	value = 0.0f;
}

/*
================
SetFloatVarEvent::GetObjType
================
*/
// FUNCTION: STBC 0x00448cc0
int SetFloatVarEvent::GetObjType( void ) {
	return SETFLOATVAREVENT_TYPE;
}

/*
================
SetFloatVarEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00448cd0
bool SetFloatVarEvent::IsTypeOf( int type ) {
	if ( type == SETFLOATVAREVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
SetFloatVarEvent::GetClassName
================
*/
// FUNCTION: STBC 0x00448d00
char *SetFloatVarEvent::GetClassName( void ) {
	return "SetFloatVarEvent";
}

/*
================
SetFloatVarEvent::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00448d10
char *SetFloatVarEvent::GetSwigTypeName( void ) {
	return "_p_SetFloatVarEvent";
}

/*
================
SetFloatVarEvent::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00448d20
char *SetFloatVarEvent::GetPythonClassName( void ) {
	return "SetFloatVarEventPtr";
}

/*
================
SetFloatVarEvent::SetFloatVarEvent
================
*/
// FUNCTION: STBC 0x00448d60 ??0SetFloatVarEvent@@QAE@XZ
SetFloatVarEvent::SetFloatVarEvent( void ) {
	scope = NULL;
	name = NULL;
	value = 0.0f;
}

/*
================
SetFloatVarEvent::~SetFloatVarEvent
================
*/
// FUNCTION: STBC 0x00448d90
SetFloatVarEvent::~SetFloatVarEvent( void ) {
	if ( scope != NULL ) {
		delete [] scope;
		scope = NULL;
	}
	if ( name != NULL ) {
		delete [] name;
		name = NULL;
	}
}

/*
================
SetFloatVarEvent::Copy

Copies the scope, name and value of the other event, which must be a
SetFloatVarEvent too.
================
*/
// FUNCTION: STBC 0x00448dd0
bool SetFloatVarEvent::Copy( TGEvent *event ) {
	TGEvent::Copy( event );
	SetFloatVarEvent *other = (SetFloatVarEvent *)event;
	SetScope( other->scope );
	SetName( other->name );
	SetValue( other->GetValue() );
	return true;
}

/*
================
SetFloatVarEvent::SetScope

Keeps a copy of the scope's text.
================
*/
// FUNCTION: STBC 0x00448e10
void SetFloatVarEvent::SetScope( char *scope_ ) {
	if ( scope != NULL ) {
		delete [] scope;
		scope = NULL;
	}
	if ( scope_ != NULL ) {
		scope = new char[strlen( scope_ ) + 1];
		strcpy( scope, scope_ );
	}
}

/*
================
SetFloatVarEvent::SetName

Keeps a copy of the variable's name, unless it is the copy already held.
================
*/
// FUNCTION: STBC 0x00448e80
// PARKED: register allocation; retail keeps this in ebx and the argument in esi, this build the reverse
void SetFloatVarEvent::SetName( char *name_ ) {
	if ( name_ != name ) {
		if ( name != NULL ) {
			delete [] name;
			name = NULL;
		}
		if ( name_ != NULL ) {
			name = new char[strlen( name_ ) + 1];
			strcpy( name, name_ );
		}
	}
}
