/*
===============================================================================

	VarManagerClass.cpp

===============================================================================
*/

#include <iostream>
#include "VarManagerClass.h"
#include "SetFloatVarEvent.h"
#include "TGEventManager.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0044b780 _$E32
// FUNCTION: STBC 0x0044b790 _$E29
// FUNCTION: STBC 0x0044b7a0 _$E31
// FUNCTION: STBC 0x0044b7b0 _$E30
// FUNCTION: STBC 0x0044b7c0 _$E38
// FUNCTION: STBC 0x0044b7d0 _$E35
// FUNCTION: STBC 0x0044b7e0 _$E37
// FUNCTION: STBC 0x0044b7f0 _$E36

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0044b800 _$E60
// FUNCTION: STBC 0x0044b810 _$E59
// FUNCTION: STBC 0x0044b820 _$E64
// FUNCTION: STBC 0x0044b830 _$E63
// FUNCTION: STBC 0x0044b840 _$E75
// FUNCTION: STBC 0x0044b850 _$E74
// FUNCTION: STBC 0x0044b860 _$E79
// FUNCTION: STBC 0x0044b870 _$E78
// FUNCTION: STBC 0x0044b880 _$E83
// FUNCTION: STBC 0x0044b890 _$E82
// FUNCTION: STBC 0x0044b8a0 _$E87
// FUNCTION: STBC 0x0044b8b0 _$E86
// FUNCTION: STBC 0x0044b8c0 _$E211
// FUNCTION: STBC 0x0044b8d0 _$E210
// FUNCTION: STBC 0x0044b8e0 _$E153
// FUNCTION: STBC 0x0044b8f0 _$E152
// FUNCTION: STBC 0x0044b900 _$E166
// FUNCTION: STBC 0x0044b910 _$E165
// FUNCTION: STBC 0x0044b920 _$E216
// FUNCTION: STBC 0x0044b930 _$E215
// FUNCTION: STBC 0x0044b940 _$E221
// FUNCTION: STBC 0x0044b950 _$E220
// FUNCTION: STBC 0x0044b960 _$E225
// FUNCTION: STBC 0x0044b970 _$E224
// FUNCTION: STBC 0x0044b9d0 _$E229
// FUNCTION: STBC 0x0044b9e0 _$E228
// FUNCTION: STBC 0x0044b9f0 _$E236
// FUNCTION: STBC 0x0044ba00 _$E235
// FUNCTION: STBC 0x0044ba10 _$E261
// FUNCTION: STBC 0x0044ba20 _$E260
// FUNCTION: STBC 0x0044ba30 _$E265
// FUNCTION: STBC 0x0044ba40 _$E264
// FUNCTION: STBC 0x0044ba50 _$E111
// FUNCTION: STBC 0x0044ba60 _$E110
// FUNCTION: STBC 0x0044ba70 _$E247
// FUNCTION: STBC 0x0044ba80 _$E246
// FUNCTION: STBC 0x0044ba90 _$E254
// FUNCTION: STBC 0x0044baa0 _$E253
// FUNCTION: STBC 0x0044bab0 _$E277
// FUNCTION: STBC 0x0044bac0 _$E276
// FUNCTION: STBC 0x0044bad0 _$E281
// FUNCTION: STBC 0x0044bae0 _$E280
// FUNCTION: STBC 0x0044baf0 _$E321
// FUNCTION: STBC 0x0044bb00 _$E320
// FUNCTION: STBC 0x0044bb10 _$E327
// FUNCTION: STBC 0x0044bb20 _$E326
// FUNCTION: STBC 0x0044bb30 _$E269
// FUNCTION: STBC 0x0044bb40 _$E268
// FUNCTION: STBC 0x0044bb50 _$E273
// FUNCTION: STBC 0x0044bb60 _$E272
// FUNCTION: STBC 0x0044bb70 _$E285
// FUNCTION: STBC 0x0044bb80 _$E284
// FUNCTION: STBC 0x0044bb90 _$E289
// FUNCTION: STBC 0x0044bba0 _$E288
// FUNCTION: STBC 0x0044bbb0 _$E293
// FUNCTION: STBC 0x0044bbc0 _$E292
// FUNCTION: STBC 0x0044bbd0 _$E297
// FUNCTION: STBC 0x0044bbe0 _$E296
// FUNCTION: STBC 0x0044bbf0 _$E301
// FUNCTION: STBC 0x0044bc00 _$E300
// FUNCTION: STBC 0x0044bc10 _$E305
// FUNCTION: STBC 0x0044bc20 _$E304
// FUNCTION: STBC 0x0044bc30 _$E309
// FUNCTION: STBC 0x0044bc40 _$E308
// FUNCTION: STBC 0x0044bc80 _$E313
// FUNCTION: STBC 0x0044bc90 _$E312
// FUNCTION: STBC 0x0044bca0 _$E116
// FUNCTION: STBC 0x0044bcb0 _$E115
// FUNCTION: STBC 0x0044bcc0 _$E317
// FUNCTION: STBC 0x0044bcd0 _$E316
// FUNCTION: STBC 0x0044bce0 _$E121
// FUNCTION: STBC 0x0044bcf0 _$E120
// FUNCTION: STBC 0x0044bd00 _$E331
// FUNCTION: STBC 0x0044bd10 _$E330
// FUNCTION: STBC 0x0044bd20 _$E335
// FUNCTION: STBC 0x0044bd30 _$E334
// FUNCTION: STBC 0x0044bd40 _$E339
// FUNCTION: STBC 0x0044bd50 _$E338
// FUNCTION: STBC 0x0044bd60 _$E343
// FUNCTION: STBC 0x0044bd70 _$E342
// FUNCTION: STBC 0x0044bd80 _$E130
// FUNCTION: STBC 0x0044bd90 _$E129
// FUNCTION: STBC 0x0044bda0 _$E347
// FUNCTION: STBC 0x0044bdb0 _$E346
// FUNCTION: STBC 0x0044bdc0 _$E351
// FUNCTION: STBC 0x0044bdd0 _$E350
// FUNCTION: STBC 0x0044bde0 _$E355
// FUNCTION: STBC 0x0044bdf0 _$E354
// FUNCTION: STBC 0x0044be00 _$E136
// FUNCTION: STBC 0x0044be10 _$E135
// FUNCTION: STBC 0x0044be20 _$E141
// FUNCTION: STBC 0x0044be30 _$E140
// FUNCTION: STBC 0x0044be40 _$E171
// FUNCTION: STBC 0x0044be50 _$E170
// FUNCTION: STBC 0x0044be60 _$E359
// FUNCTION: STBC 0x0044be70 _$E358
// FUNCTION: STBC 0x0044be80 _$E176
// FUNCTION: STBC 0x0044be90 _$E175
// FUNCTION: STBC 0x0044bea0 _$E363
// FUNCTION: STBC 0x0044beb0 _$E362
// FUNCTION: STBC 0x0044bec0 _$E367
// FUNCTION: STBC 0x0044bed0 _$E366
// FUNCTION: STBC 0x0044bee0 _$E181
// FUNCTION: STBC 0x0044bef0 _$E180
// FUNCTION: STBC 0x0044bf00 _$E371
// FUNCTION: STBC 0x0044bf10 _$E370
// FUNCTION: STBC 0x0044bf20 _$E146
// FUNCTION: STBC 0x0044bf30 _$E145
// FUNCTION: STBC 0x0044bf40 _$E375
// FUNCTION: STBC 0x0044bf50 _$E374
// FUNCTION: STBC 0x0044bf60 _$E379
// FUNCTION: STBC 0x0044bf70 _$E378
// FUNCTION: STBC 0x0044bf80 _$E186
// FUNCTION: STBC 0x0044bf90 _$E185
// FUNCTION: STBC 0x0044bfa0 _$E191
// FUNCTION: STBC 0x0044bfb0 _$E190
// FUNCTION: STBC 0x0044bfc0 _$E383
// FUNCTION: STBC 0x0044bfd0 _$E382
// FUNCTION: STBC 0x0044bfe0 _$E387
// FUNCTION: STBC 0x0044bff0 _$E386
// FUNCTION: STBC 0x0044c000 _$E391
// FUNCTION: STBC 0x0044c010 _$E390
// FUNCTION: STBC 0x0044c020 _$E395
// FUNCTION: STBC 0x0044c030 _$E394
// FUNCTION: STBC 0x0044c040 _$E399
// FUNCTION: STBC 0x0044c050 _$E398
// FUNCTION: STBC 0x0044c060 _$E403
// FUNCTION: STBC 0x0044c070 _$E402
// FUNCTION: STBC 0x0044c080 _$E407
// FUNCTION: STBC 0x0044c090 _$E406
// FUNCTION: STBC 0x0044c0a0 _$E411
// FUNCTION: STBC 0x0044c0b0 _$E410
// FUNCTION: STBC 0x0044c0c0 _$E196
// FUNCTION: STBC 0x0044c0d0 _$E195
// FUNCTION: STBC 0x0044c0e0 _$E415
// FUNCTION: STBC 0x0044c0f0 _$E414
// FUNCTION: STBC 0x0044c100 _$E201
// FUNCTION: STBC 0x0044c110 _$E200
// FUNCTION: STBC 0x0044c120 _$E206
// FUNCTION: STBC 0x0044c130 _$E205
// FUNCTION: STBC 0x0044c140 _$E419
// FUNCTION: STBC 0x0044c150 _$E418
// FUNCTION: STBC 0x0044c160 _$E424
// FUNCTION: STBC 0x0044c170 _$E423
// FUNCTION: STBC 0x0044c180 _$E428
// FUNCTION: STBC 0x0044c190 _$E427

// The locale-id pair.
// FUNCTION: STBC 0x0044bc50 _$E447
// FUNCTION: STBC 0x0044bc70 _$E446

// The handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0044b130 _$E434
// FUNCTION: STBC 0x0044b140 _$E430
// FUNCTION: STBC 0x0044b150 _$E433
// FUNCTION: STBC 0x0044b160 _$E431
// DATA: STBC 0x00980120
TGEventHandlerTable VarManagerClass::classHandlers;

// The variable manager's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0044b180 _$E439
// FUNCTION: STBC 0x0044b190 _$E436
// FUNCTION: STBC 0x0044b1a0 _$E438
// FUNCTION: STBC 0x0044b1b0 _$E437
// DATA: STBC 0x00980228
VarManagerClass	g_kVarManager;

/*
================
VarManagerClass::VarManagerClass
================
*/
// FUNCTION: STBC 0x0044b1c0
VarManagerClass::VarManagerClass( void ) {
}

/*
================
VarManagerClass::~VarManagerClass
================
*/
// FUNCTION: STBC 0x0044b1f0
VarManagerClass::~VarManagerClass( void ) {
}

/*
================
VarManagerClass::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0044b2d0
void VarManagerClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&VarManagerClass::SetFloatVarHandler, "VarManagerClass :: SetFloatVarHandler" );
}

/*
================
VarManagerClass::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0044b2f0
void VarManagerClass::RegisterHandlers( void ) {
	TGEventHandlerObject::AddMethodHandlerForClass( &classHandlers, ET_SET_FLOAT_VAR, "VarManagerClass :: SetFloatVarHandler" );
}

/*
================
VarManagerClass::SetFloatVariable

Sets the float variable, creating its scope when there is none of the name.
================
*/
// FUNCTION: STBC 0x0044b310
void VarManagerClass::SetFloatVariable( char *scopeName, char *name, float value ) {
	FloatVariableScope *scope = floatScopes.Get( scopeName );
	if ( scope == NULL ) {
		scope = new FloatVariableScope;
		scope->SetName( scopeName );
		floatScopes.Insert( scope );
	}
	scope->SetVariable( name, value );
}

/*
================
VarManagerClass::GetFloatVariable

The float variable's value; 0 when there is no such scope or variable.
================
*/
// FUNCTION: STBC 0x0044b490
float VarManagerClass::GetFloatVariable( char *scopeName, char *name ) {
	FloatVariableScope *scope = floatScopes.Get( scopeName );
	if ( scope != NULL ) {
		return scope->GetVariable( name );
	}
	return 0.0f;
}

/*
================
VarManagerClass::SetStringVariable

Sets the string variable, creating its scope when there is none of the name.
================
*/
// FUNCTION: STBC 0x0044b500
void VarManagerClass::SetStringVariable( char *scopeName, char *name, char *value ) {
	StringVariableScope *scope = stringScopes.Get( scopeName );
	if ( scope == NULL ) {
		scope = new StringVariableScope;
		scope->SetName( scopeName );
		stringScopes.Insert( scope );
	}
	scope->SetVariable( name, value );
}

/*
================
VarManagerClass::GetStringVariable

The string variable's value; NULL when there is no such scope or variable.
================
*/
// FUNCTION: STBC 0x0044b680
char *VarManagerClass::GetStringVariable( char *scopeName, char *name ) {
	StringVariableScope *scope = stringScopes.Get( scopeName );
	if ( scope != NULL ) {
		return scope->GetVariable( name );
	}
	return NULL;
}

/*
================
VarManagerClass::DeleteAllVariables

Deletes the float scopes with their variables.
================
*/
// FUNCTION: STBC 0x0044b6f0
void VarManagerClass::DeleteAllVariables( void ) {
	floatScopes.DeleteAll();
}

/*
================
VarManagerClass::DeleteAllScopedVariables

Does nothing.
================
*/
// FUNCTION: STBC 0x0044b750
void VarManagerClass::DeleteAllScopedVariables( char *scope ) {
}

/*
================
VarManagerClass::SetFloatVarHandler

A SetFloatVarEvent sets the float variable it names.
================
*/
// FUNCTION: STBC 0x0044b760
void VarManagerClass::SetFloatVarHandler( SetFloatVarEvent *event ) {
	SetFloatVariable( event->GetScope(), event->GetName(), event->GetValue() );
}
