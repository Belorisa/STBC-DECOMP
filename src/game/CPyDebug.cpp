/*
===============================================================================

	CPyDebug.cpp

===============================================================================
*/

#include <iostream>
#include "CPyDebug.h"

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "PyEmbed.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00437880 _$E441
// FUNCTION: STBC 0x004378a0 _$E440

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004374e0 _$E32
// FUNCTION: STBC 0x004374f0 _$E29
// FUNCTION: STBC 0x00437500 _$E31
// FUNCTION: STBC 0x00437510 _$E30
// FUNCTION: STBC 0x00437520 _$E38
// FUNCTION: STBC 0x00437530 _$E35
// FUNCTION: STBC 0x00437540 _$E37
// FUNCTION: STBC 0x00437550 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00437560 _$E45
// FUNCTION: STBC 0x00437570 _$E44
// FUNCTION: STBC 0x00437580 _$E49
// FUNCTION: STBC 0x00437590 _$E48
// FUNCTION: STBC 0x004375a0 _$E60
// FUNCTION: STBC 0x004375b0 _$E59
// FUNCTION: STBC 0x004375c0 _$E64
// FUNCTION: STBC 0x004375d0 _$E63
// FUNCTION: STBC 0x004375e0 _$E68
// FUNCTION: STBC 0x004375f0 _$E67
// FUNCTION: STBC 0x00437600 _$E72
// FUNCTION: STBC 0x00437610 _$E71
// FUNCTION: STBC 0x00437620 _$E196
// FUNCTION: STBC 0x00437630 _$E195
// FUNCTION: STBC 0x00437640 _$E138
// FUNCTION: STBC 0x00437650 _$E137
// FUNCTION: STBC 0x00437660 _$E151
// FUNCTION: STBC 0x00437670 _$E150
// FUNCTION: STBC 0x00437680 _$E201
// FUNCTION: STBC 0x00437690 _$E200
// FUNCTION: STBC 0x004376a0 _$E206
// FUNCTION: STBC 0x004376b0 _$E205
// FUNCTION: STBC 0x004376c0 _$E210
// FUNCTION: STBC 0x004376d0 _$E209
// FUNCTION: STBC 0x004376e0 _$E214
// FUNCTION: STBC 0x004376f0 _$E213
// FUNCTION: STBC 0x00437700 _$E221
// FUNCTION: STBC 0x00437710 _$E220
// FUNCTION: STBC 0x00437720 _$E246
// FUNCTION: STBC 0x00437730 _$E245
// FUNCTION: STBC 0x00437740 _$E250
// FUNCTION: STBC 0x00437750 _$E249
// FUNCTION: STBC 0x00437760 _$E96
// FUNCTION: STBC 0x00437770 _$E95
// FUNCTION: STBC 0x00437780 _$E232
// FUNCTION: STBC 0x00437790 _$E231
// FUNCTION: STBC 0x004377a0 _$E239
// FUNCTION: STBC 0x004377b0 _$E238
// FUNCTION: STBC 0x004377c0 _$E262
// FUNCTION: STBC 0x004377d0 _$E261
// FUNCTION: STBC 0x004377e0 _$E266
// FUNCTION: STBC 0x004377f0 _$E265
// FUNCTION: STBC 0x00437800 _$E306
// FUNCTION: STBC 0x00437810 _$E305
// FUNCTION: STBC 0x00437820 _$E312
// FUNCTION: STBC 0x00437830 _$E311
// FUNCTION: STBC 0x00437840 _$E254
// FUNCTION: STBC 0x00437850 _$E253
// FUNCTION: STBC 0x00437860 _$E258
// FUNCTION: STBC 0x00437870 _$E257
// FUNCTION: STBC 0x004378b0 _$E270
// FUNCTION: STBC 0x004378c0 _$E269
// FUNCTION: STBC 0x004378d0 _$E274
// FUNCTION: STBC 0x004378e0 _$E273
// FUNCTION: STBC 0x004378f0 _$E278
// FUNCTION: STBC 0x00437900 _$E277
// FUNCTION: STBC 0x00437910 _$E282
// FUNCTION: STBC 0x00437920 _$E281
// FUNCTION: STBC 0x00437930 _$E286
// FUNCTION: STBC 0x00437940 _$E285
// FUNCTION: STBC 0x00437950 _$E290
// FUNCTION: STBC 0x00437960 _$E289
// FUNCTION: STBC 0x00437970 _$E294
// FUNCTION: STBC 0x00437980 _$E293
// FUNCTION: STBC 0x00437990 _$E298
// FUNCTION: STBC 0x004379a0 _$E297
// FUNCTION: STBC 0x004379b0 _$E101
// FUNCTION: STBC 0x004379c0 _$E100
// FUNCTION: STBC 0x004379d0 _$E302
// FUNCTION: STBC 0x004379e0 _$E301
// FUNCTION: STBC 0x004379f0 _$E106
// FUNCTION: STBC 0x00437a00 _$E105
// FUNCTION: STBC 0x00437a10 _$E316
// FUNCTION: STBC 0x00437a20 _$E315
// FUNCTION: STBC 0x00437a30 _$E320
// FUNCTION: STBC 0x00437a40 _$E319
// FUNCTION: STBC 0x00437a50 _$E324
// FUNCTION: STBC 0x00437a60 _$E323
// FUNCTION: STBC 0x00437a70 _$E328
// FUNCTION: STBC 0x00437a80 _$E327
// FUNCTION: STBC 0x00437a90 _$E115
// FUNCTION: STBC 0x00437aa0 _$E114
// FUNCTION: STBC 0x00437ab0 _$E332
// FUNCTION: STBC 0x00437ac0 _$E331
// FUNCTION: STBC 0x00437ad0 _$E336
// FUNCTION: STBC 0x00437ae0 _$E335
// FUNCTION: STBC 0x00437af0 _$E340
// FUNCTION: STBC 0x00437b00 _$E339
// FUNCTION: STBC 0x00437b10 _$E121
// FUNCTION: STBC 0x00437b20 _$E120
// FUNCTION: STBC 0x00437b30 _$E126
// FUNCTION: STBC 0x00437b40 _$E125
// FUNCTION: STBC 0x00437b50 _$E156
// FUNCTION: STBC 0x00437b60 _$E155
// FUNCTION: STBC 0x00437b70 _$E344
// FUNCTION: STBC 0x00437b80 _$E343
// FUNCTION: STBC 0x00437b90 _$E161
// FUNCTION: STBC 0x00437ba0 _$E160
// FUNCTION: STBC 0x00437bb0 _$E348
// FUNCTION: STBC 0x00437bc0 _$E347
// FUNCTION: STBC 0x00437bd0 _$E352
// FUNCTION: STBC 0x00437be0 _$E351
// FUNCTION: STBC 0x00437bf0 _$E166
// FUNCTION: STBC 0x00437c00 _$E165
// FUNCTION: STBC 0x00437c10 _$E356
// FUNCTION: STBC 0x00437c20 _$E355
// FUNCTION: STBC 0x00437c30 _$E131
// FUNCTION: STBC 0x00437c40 _$E130
// FUNCTION: STBC 0x00437c50 _$E360
// FUNCTION: STBC 0x00437c60 _$E359
// FUNCTION: STBC 0x00437c70 _$E364
// FUNCTION: STBC 0x00437c80 _$E363
// FUNCTION: STBC 0x00437c90 _$E171
// FUNCTION: STBC 0x00437ca0 _$E170
// FUNCTION: STBC 0x00437cb0 _$E176
// FUNCTION: STBC 0x00437cc0 _$E175
// FUNCTION: STBC 0x00437cd0 _$E368
// FUNCTION: STBC 0x00437ce0 _$E367
// FUNCTION: STBC 0x00437cf0 _$E372
// FUNCTION: STBC 0x00437d00 _$E371
// FUNCTION: STBC 0x00437d10 _$E376
// FUNCTION: STBC 0x00437d20 _$E375
// FUNCTION: STBC 0x00437d30 _$E380
// FUNCTION: STBC 0x00437d40 _$E379
// FUNCTION: STBC 0x00437d50 _$E384
// FUNCTION: STBC 0x00437d60 _$E383
// FUNCTION: STBC 0x00437d70 _$E388
// FUNCTION: STBC 0x00437d80 _$E387
// FUNCTION: STBC 0x00437d90 _$E392
// FUNCTION: STBC 0x00437da0 _$E391
// FUNCTION: STBC 0x00437db0 _$E396
// FUNCTION: STBC 0x00437dc0 _$E395
// FUNCTION: STBC 0x00437dd0 _$E181
// FUNCTION: STBC 0x00437de0 _$E180
// FUNCTION: STBC 0x00437df0 _$E400
// FUNCTION: STBC 0x00437e00 _$E399
// FUNCTION: STBC 0x00437e10 _$E186
// FUNCTION: STBC 0x00437e20 _$E185
// FUNCTION: STBC 0x00437e30 _$E191
// FUNCTION: STBC 0x00437e40 _$E190
// FUNCTION: STBC 0x00437e50 _$E404
// FUNCTION: STBC 0x00437e60 _$E403
// FUNCTION: STBC 0x00437e70 _$E409
// FUNCTION: STBC 0x00437e80 _$E408
// FUNCTION: STBC 0x00437e90 _$E413
// FUNCTION: STBC 0x00437ea0 _$E412

/*
================
CPyDebug::CPyDebug
================
*/
// FUNCTION: STBC 0x004372e0
CPyDebug::CPyDebug( char *name_ ) {
	name = NULL;
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
}

/*
================
CPyDebug::~CPyDebug
================
*/
// FUNCTION: STBC 0x00437340
CPyDebug::~CPyDebug( void ) {
	if ( name != NULL ) {
		delete [] name;
	}
}

/*
================
CPyDebug::Print

The release build prints nothing.
================
*/
// FUNCTION: STBC 0x00437350
void CPyDebug::Print( char *text ) {
}

/*
================
CPyDebug_SaveInstance

The printer's name goes into the instance as DebugType, or None.
================
*/
// FUNCTION: STBC 0x00437360
bool CPyDebug_SaveInstance( PyObject *instance, PyObject **state ) {
	CPyDebug *debug = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&debug, "CPyDebug" );
	if ( debug == NULL ) {
		return false;
	}
	if ( debug->GetName() != NULL ) {
		// BUG: the new string is never released
		PyObject_SetAttrString( instance, "DebugType", PyString_FromString( debug->GetName() ) );
	} else {
		// BUG: None is stored without a new reference
		PyObject_SetAttrString( instance, "DebugType", Py_None );
	}
	return true;
}

/*
================
CPyDebug_LoadInstance

Makes a new printer from the state's DebugType and gives the instance the
SWIG pointer to it.
================
*/
// FUNCTION: STBC 0x004373d0
bool CPyDebug_LoadInstance( PyObject *instance, PyObject *state ) {
	PyObject *debugType = PyDict_GetItemString( state, "DebugType" );
	CPyDebug *debug;
	if ( debugType == Py_None ) {
		debug = new CPyDebug( NULL );
	} else {
		debug = new CPyDebug( PyString_AsString( debugType ) );
	}
	PyObject *pointer = PyEmbed::NewPointerString( (unsigned int)debug, "CPyDebug" );
	if ( pointer == NULL ) {
		return false;
	}
	// BUG: the new integer is never released
	if ( PyObject_SetAttrString( instance, "thisown", PyInt_FromLong( 1 ) ) == -1 ) {
		return false;
	}
	return PyObject_SetAttrString( instance, "this", pointer ) == -1 ? false : true;
}
