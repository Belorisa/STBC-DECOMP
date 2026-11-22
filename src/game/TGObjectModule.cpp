/*
===============================================================================

	TGObjectModule.cpp

===============================================================================
*/

#include <iostream>
#include "TGAttrObject.h"
#include "TGObjectModule.h"
#include "TGPointerMap.h"
#include "TGString.h"
#include "TGTemplatedAttrObject.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006eea40 _$E32
// FUNCTION: STBC 0x006eea50 _$E29
// FUNCTION: STBC 0x006eea60 _$E31
// FUNCTION: STBC 0x006eea70 _$E30
// FUNCTION: STBC 0x006eea80 _$E38
// FUNCTION: STBC 0x006eea90 _$E35
// FUNCTION: STBC 0x006eeaa0 _$E37
// FUNCTION: STBC 0x006eeab0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006eeac0 _$E75
// FUNCTION: STBC 0x006eead0 _$E74
// FUNCTION: STBC 0x006eeae0 _$E79
// FUNCTION: STBC 0x006eeaf0 _$E78
// FUNCTION: STBC 0x006eeb00 _$E90
// FUNCTION: STBC 0x006eeb10 _$E89
// FUNCTION: STBC 0x006eeb20 _$E94
// FUNCTION: STBC 0x006eeb30 _$E93
// FUNCTION: STBC 0x006eeb40 _$E98
// FUNCTION: STBC 0x006eeb50 _$E97
// FUNCTION: STBC 0x006eeb60 _$E102
// FUNCTION: STBC 0x006eeb70 _$E101
// FUNCTION: STBC 0x006eeb80 _$E226
// FUNCTION: STBC 0x006eeb90 _$E225
// FUNCTION: STBC 0x006eeba0 _$E168
// FUNCTION: STBC 0x006eebb0 _$E167
// FUNCTION: STBC 0x006eebc0 _$E181
// FUNCTION: STBC 0x006eebd0 _$E180
// FUNCTION: STBC 0x006eebe0 _$E231
// FUNCTION: STBC 0x006eebf0 _$E230
// FUNCTION: STBC 0x006eec00 _$E236
// FUNCTION: STBC 0x006eec10 _$E235
// FUNCTION: STBC 0x006eec20 _$E240
// FUNCTION: STBC 0x006eec30 _$E239
// FUNCTION: STBC 0x006eec40 _$E244
// FUNCTION: STBC 0x006eec50 _$E243
// FUNCTION: STBC 0x006eec60 _$E251
// FUNCTION: STBC 0x006eec70 _$E250
// FUNCTION: STBC 0x006eec80 _$E276
// FUNCTION: STBC 0x006eec90 _$E275
// FUNCTION: STBC 0x006eeca0 _$E280
// FUNCTION: STBC 0x006eecb0 _$E279
// FUNCTION: STBC 0x006eecc0 _$E126
// FUNCTION: STBC 0x006eecd0 _$E125
// FUNCTION: STBC 0x006eece0 _$E262
// FUNCTION: STBC 0x006eecf0 _$E261
// FUNCTION: STBC 0x006eed00 _$E269
// FUNCTION: STBC 0x006eed10 _$E268
// FUNCTION: STBC 0x006eed20 _$E292
// FUNCTION: STBC 0x006eed30 _$E291
// FUNCTION: STBC 0x006eed40 _$E296
// FUNCTION: STBC 0x006eed50 _$E295
// FUNCTION: STBC 0x006eed60 _$E336
// FUNCTION: STBC 0x006eed70 _$E335
// FUNCTION: STBC 0x006eed80 _$E342
// FUNCTION: STBC 0x006eed90 _$E341
// FUNCTION: STBC 0x006eeda0 _$E284
// FUNCTION: STBC 0x006eedb0 _$E283
// FUNCTION: STBC 0x006eedc0 _$E288
// FUNCTION: STBC 0x006eedd0 _$E287
// FUNCTION: STBC 0x006eede0 _$E300
// FUNCTION: STBC 0x006eedf0 _$E299
// FUNCTION: STBC 0x006eee00 _$E304
// FUNCTION: STBC 0x006eee10 _$E303
// FUNCTION: STBC 0x006eee20 _$E308
// FUNCTION: STBC 0x006eee30 _$E307
// FUNCTION: STBC 0x006eee40 _$E312
// FUNCTION: STBC 0x006eee50 _$E311
// FUNCTION: STBC 0x006eee60 _$E316
// FUNCTION: STBC 0x006eee70 _$E315
// FUNCTION: STBC 0x006eee80 _$E320
// FUNCTION: STBC 0x006eee90 _$E319
// FUNCTION: STBC 0x006eeea0 _$E324
// FUNCTION: STBC 0x006eeeb0 _$E323
// FUNCTION: STBC 0x006eeec0 _$E328
// FUNCTION: STBC 0x006eeed0 _$E327
// FUNCTION: STBC 0x006eeee0 _$E131
// FUNCTION: STBC 0x006eeef0 _$E130
// FUNCTION: STBC 0x006eef00 _$E332
// FUNCTION: STBC 0x006eef10 _$E331
// FUNCTION: STBC 0x006eef20 _$E136
// FUNCTION: STBC 0x006eef30 _$E135
// FUNCTION: STBC 0x006eef40 _$E346
// FUNCTION: STBC 0x006eef50 _$E345
// FUNCTION: STBC 0x006eef60 _$E350
// FUNCTION: STBC 0x006eef70 _$E349
// FUNCTION: STBC 0x006eef80 _$E354
// FUNCTION: STBC 0x006eef90 _$E353
// FUNCTION: STBC 0x006eefa0 _$E358
// FUNCTION: STBC 0x006eefb0 _$E357
// FUNCTION: STBC 0x006eefc0 _$E145
// FUNCTION: STBC 0x006eefd0 _$E144
// FUNCTION: STBC 0x006eefe0 _$E362
// FUNCTION: STBC 0x006eeff0 _$E361
// FUNCTION: STBC 0x006ef000 _$E366
// FUNCTION: STBC 0x006ef010 _$E365
// FUNCTION: STBC 0x006ef020 _$E370
// FUNCTION: STBC 0x006ef030 _$E369
// FUNCTION: STBC 0x006ef040 _$E151
// FUNCTION: STBC 0x006ef050 _$E150
// FUNCTION: STBC 0x006ef060 _$E156
// FUNCTION: STBC 0x006ef070 _$E155
// FUNCTION: STBC 0x006ef080 _$E186
// FUNCTION: STBC 0x006ef090 _$E185
// FUNCTION: STBC 0x006ef0a0 _$E374
// FUNCTION: STBC 0x006ef0b0 _$E373
// FUNCTION: STBC 0x006ef0c0 _$E191
// FUNCTION: STBC 0x006ef0d0 _$E190
// FUNCTION: STBC 0x006ef0e0 _$E378
// FUNCTION: STBC 0x006ef0f0 _$E377
// FUNCTION: STBC 0x006ef100 _$E382
// FUNCTION: STBC 0x006ef110 _$E381
// FUNCTION: STBC 0x006ef120 _$E196
// FUNCTION: STBC 0x006ef130 _$E195
// FUNCTION: STBC 0x006ef140 _$E386
// FUNCTION: STBC 0x006ef150 _$E385
// FUNCTION: STBC 0x006ef160 _$E161
// FUNCTION: STBC 0x006ef170 _$E160
// FUNCTION: STBC 0x006ef180 _$E390
// FUNCTION: STBC 0x006ef190 _$E389
// FUNCTION: STBC 0x006ef1a0 _$E394
// FUNCTION: STBC 0x006ef1b0 _$E393
// FUNCTION: STBC 0x006ef1c0 _$E201
// FUNCTION: STBC 0x006ef1d0 _$E200
// FUNCTION: STBC 0x006ef1e0 _$E206
// FUNCTION: STBC 0x006ef1f0 _$E205
// FUNCTION: STBC 0x006ef200 _$E398
// FUNCTION: STBC 0x006ef210 _$E397
// FUNCTION: STBC 0x006ef220 _$E402
// FUNCTION: STBC 0x006ef230 _$E401
// FUNCTION: STBC 0x006ef240 _$E406
// FUNCTION: STBC 0x006ef250 _$E405
// FUNCTION: STBC 0x006ef260 _$E410
// FUNCTION: STBC 0x006ef270 _$E409
// FUNCTION: STBC 0x006ef280 _$E414
// FUNCTION: STBC 0x006ef290 _$E413
// FUNCTION: STBC 0x006ef2a0 _$E418
// FUNCTION: STBC 0x006ef2b0 _$E417
// FUNCTION: STBC 0x006ef2c0 _$E422
// FUNCTION: STBC 0x006ef2d0 _$E421
// FUNCTION: STBC 0x006ef2e0 _$E426
// FUNCTION: STBC 0x006ef2f0 _$E425
// FUNCTION: STBC 0x006ef300 _$E211
// FUNCTION: STBC 0x006ef310 _$E210
// FUNCTION: STBC 0x006ef320 _$E430
// FUNCTION: STBC 0x006ef330 _$E429
// FUNCTION: STBC 0x006ef340 _$E216
// FUNCTION: STBC 0x006ef350 _$E215
// FUNCTION: STBC 0x006ef360 _$E221
// FUNCTION: STBC 0x006ef370 _$E220

// DATA: STBC 0x0099a418
TGPointerMap pointerMap;

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006ef570 _$E440
// FUNCTION: STBC 0x006ef580 _$E437
// FUNCTION: STBC 0x006ef590 _$E439
// FUNCTION: STBC 0x006ef5a0 _$E438
// DATA: STBC 0x0099a4e8
TGObjectModule objectModule;		// invented name

/*
================
TGObjectModule::VirtualSlot00

The startup: the creators of the attribute objects, the Python state
functions and the Python method of TGString.
================
*/
// FUNCTION: STBC 0x006ef5e0
void TGObjectModule::VirtualSlot00( void ) {
	TGObject::RegisterCreator( TGATTROBJECT_TYPE, CreateTGAttrObject );
	TGObject::RegisterCreator( TGTEMPLATEDATTROBJECT_TYPE, CreateTGTemplatedAttrObject );
	RegisterStateFunctions();
	PyEmbed::AddClassMethod( TGString_GetCString, "TGString", "GetCString" );
}

/*
================
TGObjectModule::CreateTGAttrObject
================
*/
// FUNCTION: STBC 0x006ef760
TGObject *TGObjectModule::CreateTGAttrObject( long id ) {
	return new TGAttrObject( id );
}

/*
================
TGObjectModule::CreateTGTemplatedAttrObject
================
*/
// FUNCTION: STBC 0x006ef790
TGObject *TGObjectModule::CreateTGTemplatedAttrObject( long id ) {
	return new TGTemplatedAttrObject( id );
}

/*
================
TGObjectModule::VirtualSlot01

Forgets the saved addresses of the last load.
================
*/
// FUNCTION: STBC 0x006ef7c0
void TGObjectModule::VirtualSlot01( void ) {
	pointerMap.RemoveAll();
}

/*
================
TGObjectModule::VirtualSlot04
================
*/
// FUNCTION: STBC 0x006ef830
void TGObjectModule::VirtualSlot04( void ) {
}

/*
================
TGObjectModule::VirtualSlot05

The end: the table of the objects by ID goes with its last object.
================
*/
// FUNCTION: STBC 0x006ef840
void TGObjectModule::VirtualSlot05( void ) {
	TGObject::ReleaseObjectTable();
}

/*
================
TGObjectModule::RegisterStateFunctions

TGObject shadow instances save their object's ID and are pointed at the
loaded object again; NiAVObject shadow instances come back invalid.
================
*/
// FUNCTION: STBC 0x006ef850
void TGObjectModule::RegisterStateFunctions( void ) {
	PyEmbed::AddGetStateFunction( GetTGObjectState, "TGObject" );
	PyEmbed::AddSetStateFunction( SetTGObjectState, "TGObject" );
	PyEmbed::AddSetStateFunction( SetNiAVObjectState, "NiAVObject" );
}

/*
================
TGObjectModule::GetTGObjectState

Adds the object's ID to the instance's dictionary as "_ID_". False when
the instance has no dictionary or no object, or when its object is no
longer the live object of its ID.
================
*/
// FUNCTION: STBC 0x006efa70
bool TGObjectModule::GetTGObjectState( PyObject *instance, PyObject **state ) {
	PyObject *dict;
	if ( PyEmbed::GetAttribute( instance, "__dict__", "O", &dict ) == -1 ) {
		return false;
	}

	TGObject *object = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&object, "TGObject" );
	if ( object == NULL ) {
		return false;
	}

	long id = object->GetObjID();
	if ( TGObject::GetTGObjectPtr( id ) != object ) {
		// The pointer string is fetched for a report the release build
		// does not make.
		PyObject *pointer = PyDict_GetItemString( dict, "this" );
		if ( pointer != NULL && PyString_Check( pointer ) ) {
			PyString_AsString( pointer );
		}
		return false;
	}

	PyObject *idObject = PyInt_FromLong( id );
	PyDict_SetItemString( dict, "_ID_", idObject );
	Py_DECREF( idObject );
	return true;
}

/*
================
TGObjectModule::SetTGObjectState

Restores the instance's dictionary; when it holds an ID, the instance is
pointed at the object of that ID at the end of the load.
================
*/
// FUNCTION: STBC 0x006efb40
bool TGObjectModule::SetTGObjectState( PyObject *instance, PyObject *state ) {
	if ( PyObject_SetAttrString( instance, "__dict__", state ) != -1 ) {
		PyObject *idObject = PyDict_GetItemString( state, "_ID_" );
		if ( idObject != NULL ) {
			TGObject::AddFixUpCallback( FixUpInstance, instance, (void *)PyInt_AsLong( idObject ) );
		}
	}
	return true;
}

/*
================
TGObjectModule::FixUpInstance

Gives the instance a pointer string to the loaded object of the ID. An ID
without an object gets a NULL pointer, and the call fails.

The reference to the old pointer string is never released.
================
*/
// FUNCTION: STBC 0x006efb90
bool TGObjectModule::FixUpInstance( void *instance, void *id ) {
	bool fixed = true;

	PyObject *pointer = PyObject_GetAttrString( (PyObject *)instance, "this" );
	if ( pointer != NULL ) {
		TGObject *object = TGObject::GetTGObjectPtr( (long)id );
		PyObject *newPointer;
		if ( object != NULL ) {
			newPointer = PyEmbed::Unknown6F9A90( object, pointer );
		} else {
			newPointer = PyEmbed::Unknown6F9A90( NULL, pointer );
			fixed = false;
		}

		if ( PyObject_SetAttrString( (PyObject *)instance, "this", newPointer ) == -1 ) {
			fixed = false;
		}
	} else {
		fixed = false;
	}
	return fixed;
}

/*
================
TGObjectModule::SetNiAVObjectState

Restores the instance's dictionary, but not its object: the instance
loses its pointer and is marked invalid and not owning.

The references to the two new integers are never released.
================
*/
// FUNCTION: STBC 0x006efc00
bool TGObjectModule::SetNiAVObjectState( PyObject *instance, PyObject *state ) {
	if ( PyObject_SetAttrString( instance, "__dict__", state ) == -1 ) {
		return false;
	}
	PyObject_SetAttrString( instance, "this", NULL );
	PyObject_SetAttrString( instance, "INVALID", PyInt_FromLong( 1 ) );
	PyObject_SetAttrString( instance, "thisown", PyInt_FromLong( 0 ) );
	return true;
}
