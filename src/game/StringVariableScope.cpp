/*
===============================================================================

	StringVariableScope.cpp

	The string variables of one VarManagerClass scope (invented unit name).

===============================================================================
*/

#include <iostream>
#include <string.h>
#include "ScriptVariables.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0044a7e0 _$E45
// FUNCTION: STBC 0x0044a7f0 _$E44
// FUNCTION: STBC 0x0044a800 _$E49
// FUNCTION: STBC 0x0044a810 _$E48
// FUNCTION: STBC 0x0044a820 _$E60
// FUNCTION: STBC 0x0044a830 _$E59
// FUNCTION: STBC 0x0044a840 _$E64
// FUNCTION: STBC 0x0044a850 _$E63
// FUNCTION: STBC 0x0044a860 _$E68
// FUNCTION: STBC 0x0044a870 _$E67
// FUNCTION: STBC 0x0044a880 _$E72
// FUNCTION: STBC 0x0044a890 _$E71
// FUNCTION: STBC 0x0044a8a0 _$E196
// FUNCTION: STBC 0x0044a8b0 _$E195
// FUNCTION: STBC 0x0044a8c0 _$E138
// FUNCTION: STBC 0x0044a8d0 _$E137
// FUNCTION: STBC 0x0044a8e0 _$E151
// FUNCTION: STBC 0x0044a8f0 _$E150
// FUNCTION: STBC 0x0044a900 _$E201
// FUNCTION: STBC 0x0044a910 _$E200
// FUNCTION: STBC 0x0044a920 _$E206
// FUNCTION: STBC 0x0044a930 _$E205
// FUNCTION: STBC 0x0044a940 _$E210
// FUNCTION: STBC 0x0044a950 _$E209
// FUNCTION: STBC 0x0044a960 _$E214
// FUNCTION: STBC 0x0044a970 _$E213
// FUNCTION: STBC 0x0044a980 _$E221
// FUNCTION: STBC 0x0044a990 _$E220
// FUNCTION: STBC 0x0044a9a0 _$E246
// FUNCTION: STBC 0x0044a9b0 _$E245
// FUNCTION: STBC 0x0044a9c0 _$E250
// FUNCTION: STBC 0x0044a9d0 _$E249
// FUNCTION: STBC 0x0044a9e0 _$E96
// FUNCTION: STBC 0x0044a9f0 _$E95
// FUNCTION: STBC 0x0044aa00 _$E232
// FUNCTION: STBC 0x0044aa10 _$E231
// FUNCTION: STBC 0x0044aa20 _$E239
// FUNCTION: STBC 0x0044aa30 _$E238
// FUNCTION: STBC 0x0044aa40 _$E262
// FUNCTION: STBC 0x0044aa50 _$E261
// FUNCTION: STBC 0x0044aa60 _$E266
// FUNCTION: STBC 0x0044aa70 _$E265
// FUNCTION: STBC 0x0044aa80 _$E306
// FUNCTION: STBC 0x0044aa90 _$E305
// FUNCTION: STBC 0x0044aaa0 _$E312
// FUNCTION: STBC 0x0044aab0 _$E311
// FUNCTION: STBC 0x0044aac0 _$E254
// FUNCTION: STBC 0x0044aad0 _$E253
// FUNCTION: STBC 0x0044aae0 _$E258
// FUNCTION: STBC 0x0044aaf0 _$E257
// FUNCTION: STBC 0x0044ab00 _$E270
// FUNCTION: STBC 0x0044ab10 _$E269
// FUNCTION: STBC 0x0044ab20 _$E274
// FUNCTION: STBC 0x0044ab30 _$E273
// FUNCTION: STBC 0x0044ab70 _$E278
// FUNCTION: STBC 0x0044ab80 _$E277
// FUNCTION: STBC 0x0044ab90 _$E282
// FUNCTION: STBC 0x0044aba0 _$E281
// FUNCTION: STBC 0x0044abb0 _$E286
// FUNCTION: STBC 0x0044abc0 _$E285
// FUNCTION: STBC 0x0044abd0 _$E290
// FUNCTION: STBC 0x0044abe0 _$E289
// FUNCTION: STBC 0x0044abf0 _$E294
// FUNCTION: STBC 0x0044ac00 _$E293
// FUNCTION: STBC 0x0044ac10 _$E298
// FUNCTION: STBC 0x0044ac20 _$E297
// FUNCTION: STBC 0x0044ac30 _$E101
// FUNCTION: STBC 0x0044ac40 _$E100
// FUNCTION: STBC 0x0044ac50 _$E302
// FUNCTION: STBC 0x0044ac60 _$E301
// FUNCTION: STBC 0x0044ac70 _$E106
// FUNCTION: STBC 0x0044ac80 _$E105
// FUNCTION: STBC 0x0044ac90 _$E316
// FUNCTION: STBC 0x0044aca0 _$E315
// FUNCTION: STBC 0x0044acb0 _$E320
// FUNCTION: STBC 0x0044acc0 _$E319
// FUNCTION: STBC 0x0044acd0 _$E324
// FUNCTION: STBC 0x0044ace0 _$E323
// FUNCTION: STBC 0x0044acf0 _$E328
// FUNCTION: STBC 0x0044ad00 _$E327
// FUNCTION: STBC 0x0044ad10 _$E115
// FUNCTION: STBC 0x0044ad20 _$E114
// FUNCTION: STBC 0x0044ad30 _$E332
// FUNCTION: STBC 0x0044ad40 _$E331
// FUNCTION: STBC 0x0044ad50 _$E336
// FUNCTION: STBC 0x0044ad60 _$E335
// FUNCTION: STBC 0x0044ad70 _$E340
// FUNCTION: STBC 0x0044ad80 _$E339
// FUNCTION: STBC 0x0044ad90 _$E121
// FUNCTION: STBC 0x0044ada0 _$E120
// FUNCTION: STBC 0x0044adb0 _$E126
// FUNCTION: STBC 0x0044adc0 _$E125
// FUNCTION: STBC 0x0044add0 _$E156
// FUNCTION: STBC 0x0044ade0 _$E155
// FUNCTION: STBC 0x0044adf0 _$E344
// FUNCTION: STBC 0x0044ae00 _$E343
// FUNCTION: STBC 0x0044ae10 _$E161
// FUNCTION: STBC 0x0044ae20 _$E160
// FUNCTION: STBC 0x0044ae30 _$E348
// FUNCTION: STBC 0x0044ae40 _$E347
// FUNCTION: STBC 0x0044ae50 _$E352
// FUNCTION: STBC 0x0044ae60 _$E351
// FUNCTION: STBC 0x0044ae70 _$E166
// FUNCTION: STBC 0x0044ae80 _$E165
// FUNCTION: STBC 0x0044ae90 _$E356
// FUNCTION: STBC 0x0044aea0 _$E355
// FUNCTION: STBC 0x0044aeb0 _$E131
// FUNCTION: STBC 0x0044aec0 _$E130
// FUNCTION: STBC 0x0044aed0 _$E360
// FUNCTION: STBC 0x0044aee0 _$E359
// FUNCTION: STBC 0x0044aef0 _$E364
// FUNCTION: STBC 0x0044af00 _$E363
// FUNCTION: STBC 0x0044af10 _$E171
// FUNCTION: STBC 0x0044af20 _$E170
// FUNCTION: STBC 0x0044af30 _$E176
// FUNCTION: STBC 0x0044af40 _$E175
// FUNCTION: STBC 0x0044af50 _$E368
// FUNCTION: STBC 0x0044af60 _$E367
// FUNCTION: STBC 0x0044af70 _$E372
// FUNCTION: STBC 0x0044af80 _$E371
// FUNCTION: STBC 0x0044af90 _$E376
// FUNCTION: STBC 0x0044afa0 _$E375
// FUNCTION: STBC 0x0044afb0 _$E380
// FUNCTION: STBC 0x0044afc0 _$E379
// FUNCTION: STBC 0x0044afd0 _$E384
// FUNCTION: STBC 0x0044afe0 _$E383
// FUNCTION: STBC 0x0044aff0 _$E388
// FUNCTION: STBC 0x0044b000 _$E387
// FUNCTION: STBC 0x0044b010 _$E392
// FUNCTION: STBC 0x0044b020 _$E391
// FUNCTION: STBC 0x0044b030 _$E396
// FUNCTION: STBC 0x0044b040 _$E395
// FUNCTION: STBC 0x0044b050 _$E181
// FUNCTION: STBC 0x0044b060 _$E180
// FUNCTION: STBC 0x0044b070 _$E400
// FUNCTION: STBC 0x0044b080 _$E399
// FUNCTION: STBC 0x0044b090 _$E186
// FUNCTION: STBC 0x0044b0a0 _$E185
// FUNCTION: STBC 0x0044b0b0 _$E191
// FUNCTION: STBC 0x0044b0c0 _$E190
// FUNCTION: STBC 0x0044b0d0 _$E404
// FUNCTION: STBC 0x0044b0e0 _$E403
// FUNCTION: STBC 0x0044b0f0 _$E409
// FUNCTION: STBC 0x0044b100 _$E408
// FUNCTION: STBC 0x0044b110 _$E413
// FUNCTION: STBC 0x0044b120 _$E412

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale id pair.
// FUNCTION: STBC 0x0044a760 _$E32
// FUNCTION: STBC 0x0044a770 _$E29
// FUNCTION: STBC 0x0044a780 _$E31
// FUNCTION: STBC 0x0044a790 _$E30
// FUNCTION: STBC 0x0044a7a0 _$E38
// FUNCTION: STBC 0x0044a7b0 _$E35
// FUNCTION: STBC 0x0044a7c0 _$E37
// FUNCTION: STBC 0x0044a7d0 _$E36
// FUNCTION: STBC 0x0044ab40 _$E421
// FUNCTION: STBC 0x0044ab60 _$E420

/*
================
StringVariableScope::StringVariableScope
================
*/
// FUNCTION: STBC 0x0044a400
StringVariableScope::StringVariableScope( void ) {
	name = NULL;
}

/*
================
StringVariableScope::Compare

Compares the scope with another one by name.
================
*/
// FUNCTION: STBC 0x0044a420
int StringVariableScope::Compare( StringVariableScope *other ) {
	return CompareKey( other->GetName() );
}

/*
================
StringVariableScope::CompareKey
================
*/
// FUNCTION: STBC 0x0044a430
int StringVariableScope::CompareKey( char *key ) {
	return strcmp( key, name );
}

/*
================
StringVariableScope::~StringVariableScope

Deletes the name and the variables.
================
*/
// FUNCTION: STBC 0x0044a490
StringVariableScope::~StringVariableScope( void ) {
	delete [] name;
}

/*
================
StringVariableScope::SetName

Keeps a copy of the scope's name.
================
*/
// FUNCTION: STBC 0x0044a510
void StringVariableScope::SetName( char *name_ ) {
	if ( name != NULL ) {
		delete [] name;
		name = NULL;
	}
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
}

/*
================
StringVariableScope::SetVariable
================
*/
// FUNCTION: STBC 0x0044a580
void StringVariableScope::SetVariable( char *variableName, char *variableValue ) {
	StringVariable *variable = variables.Get( variableName );
	if ( variable == NULL ) {
		variable = new StringVariable;
		variable->SetName( variableName );
		variables.Insert( variable );
	}
	variable->SetValue( variableValue );
}

/*
================
StringVariableScope::GetVariable
================
*/
// FUNCTION: STBC 0x0044a700
char *StringVariableScope::GetVariable( char *variableName ) {
	StringVariable *variable = variables.Get( variableName );
	if ( variable != NULL ) {
		return variable->value;
	}
	return NULL;
}
