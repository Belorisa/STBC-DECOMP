/*
===============================================================================

	TGObject.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "TGObject.h"
#include "TGStream.h"
#include "TGEvent.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006f1c20 _$E459
// FUNCTION: STBC 0x006f1c40 _$E458

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006efde0 _$E32
// FUNCTION: STBC 0x006efdf0 _$E29
// FUNCTION: STBC 0x006efe00 _$E31
// FUNCTION: STBC 0x006efe10 _$E30
// FUNCTION: STBC 0x006efe20 _$E38
// FUNCTION: STBC 0x006efe30 _$E35
// FUNCTION: STBC 0x006efe40 _$E37
// FUNCTION: STBC 0x006efe50 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006efe60 _$E75
// FUNCTION: STBC 0x006efe70 _$E74
// FUNCTION: STBC 0x006efe80 _$E79
// FUNCTION: STBC 0x006efe90 _$E78
// FUNCTION: STBC 0x006efea0 _$E90
// FUNCTION: STBC 0x006efeb0 _$E89
// FUNCTION: STBC 0x006efec0 _$E94
// FUNCTION: STBC 0x006efed0 _$E93
// FUNCTION: STBC 0x006efee0 _$E98
// FUNCTION: STBC 0x006efef0 _$E97
// FUNCTION: STBC 0x006eff00 _$E102
// FUNCTION: STBC 0x006eff10 _$E101
// FUNCTION: STBC 0x006eff20 _$E226
// FUNCTION: STBC 0x006eff30 _$E225
// FUNCTION: STBC 0x006eff40 _$E168
// FUNCTION: STBC 0x006eff50 _$E167
// FUNCTION: STBC 0x006eff60 _$E181
// FUNCTION: STBC 0x006eff70 _$E180
// FUNCTION: STBC 0x006eff80 _$E231
// FUNCTION: STBC 0x006eff90 _$E230
// FUNCTION: STBC 0x006effa0 _$E236
// FUNCTION: STBC 0x006effb0 _$E235
// FUNCTION: STBC 0x006effc0 _$E240
// FUNCTION: STBC 0x006effd0 _$E239
// FUNCTION: STBC 0x006effe0 _$E244
// FUNCTION: STBC 0x006efff0 _$E243
// FUNCTION: STBC 0x006f0000 _$E251
// FUNCTION: STBC 0x006f0010 _$E250
// FUNCTION: STBC 0x006f0020 _$E276
// FUNCTION: STBC 0x006f0030 _$E275
// FUNCTION: STBC 0x006f0040 _$E280
// FUNCTION: STBC 0x006f0050 _$E279
// FUNCTION: STBC 0x006f0060 _$E126
// FUNCTION: STBC 0x006f0070 _$E125
// FUNCTION: STBC 0x006f0080 _$E262
// FUNCTION: STBC 0x006f0090 _$E261
// FUNCTION: STBC 0x006f00a0 _$E269
// FUNCTION: STBC 0x006f00b0 _$E268
// FUNCTION: STBC 0x006f00c0 _$E292
// FUNCTION: STBC 0x006f00d0 _$E291
// FUNCTION: STBC 0x006f00e0 _$E296
// FUNCTION: STBC 0x006f00f0 _$E295
// FUNCTION: STBC 0x006f0100 _$E336
// FUNCTION: STBC 0x006f0110 _$E335
// FUNCTION: STBC 0x006f0120 _$E342
// FUNCTION: STBC 0x006f0130 _$E341
// FUNCTION: STBC 0x006f0140 _$E284
// FUNCTION: STBC 0x006f0150 _$E283
// FUNCTION: STBC 0x006f0160 _$E288
// FUNCTION: STBC 0x006f0170 _$E287
// FUNCTION: STBC 0x006f0180 _$E300
// FUNCTION: STBC 0x006f0190 _$E299
// FUNCTION: STBC 0x006f01a0 _$E304
// FUNCTION: STBC 0x006f01b0 _$E303
// FUNCTION: STBC 0x006f01c0 _$E308
// FUNCTION: STBC 0x006f01d0 _$E307
// FUNCTION: STBC 0x006f01e0 _$E312
// FUNCTION: STBC 0x006f01f0 _$E311
// FUNCTION: STBC 0x006f0200 _$E316
// FUNCTION: STBC 0x006f0210 _$E315
// FUNCTION: STBC 0x006f0220 _$E320
// FUNCTION: STBC 0x006f0230 _$E319
// FUNCTION: STBC 0x006f0240 _$E324
// FUNCTION: STBC 0x006f0250 _$E323
// FUNCTION: STBC 0x006f0260 _$E328
// FUNCTION: STBC 0x006f0270 _$E327
// FUNCTION: STBC 0x006f0280 _$E131
// FUNCTION: STBC 0x006f0290 _$E130
// FUNCTION: STBC 0x006f02a0 _$E332
// FUNCTION: STBC 0x006f02b0 _$E331
// FUNCTION: STBC 0x006f02c0 _$E136
// FUNCTION: STBC 0x006f02d0 _$E135
// FUNCTION: STBC 0x006f02e0 _$E346
// FUNCTION: STBC 0x006f02f0 _$E345
// FUNCTION: STBC 0x006f0300 _$E350
// FUNCTION: STBC 0x006f0310 _$E349
// FUNCTION: STBC 0x006f0320 _$E354
// FUNCTION: STBC 0x006f0330 _$E353
// FUNCTION: STBC 0x006f0340 _$E358
// FUNCTION: STBC 0x006f0350 _$E357
// FUNCTION: STBC 0x006f0360 _$E145
// FUNCTION: STBC 0x006f0370 _$E144
// FUNCTION: STBC 0x006f0380 _$E362
// FUNCTION: STBC 0x006f0390 _$E361
// FUNCTION: STBC 0x006f03a0 _$E366
// FUNCTION: STBC 0x006f03b0 _$E365
// FUNCTION: STBC 0x006f03c0 _$E370
// FUNCTION: STBC 0x006f03d0 _$E369
// FUNCTION: STBC 0x006f03e0 _$E151
// FUNCTION: STBC 0x006f03f0 _$E150
// FUNCTION: STBC 0x006f0400 _$E156
// FUNCTION: STBC 0x006f0410 _$E155
// FUNCTION: STBC 0x006f0420 _$E186
// FUNCTION: STBC 0x006f0430 _$E185
// FUNCTION: STBC 0x006f0440 _$E374
// FUNCTION: STBC 0x006f0450 _$E373
// FUNCTION: STBC 0x006f0460 _$E191
// FUNCTION: STBC 0x006f0470 _$E190
// FUNCTION: STBC 0x006f0480 _$E378
// FUNCTION: STBC 0x006f0490 _$E377
// FUNCTION: STBC 0x006f04a0 _$E382
// FUNCTION: STBC 0x006f04b0 _$E381
// FUNCTION: STBC 0x006f04c0 _$E196
// FUNCTION: STBC 0x006f04d0 _$E195
// FUNCTION: STBC 0x006f04e0 _$E386
// FUNCTION: STBC 0x006f04f0 _$E385
// FUNCTION: STBC 0x006f0500 _$E161
// FUNCTION: STBC 0x006f0510 _$E160
// FUNCTION: STBC 0x006f0520 _$E390
// FUNCTION: STBC 0x006f0530 _$E389
// FUNCTION: STBC 0x006f0540 _$E394
// FUNCTION: STBC 0x006f0550 _$E393
// FUNCTION: STBC 0x006f0560 _$E201
// FUNCTION: STBC 0x006f0570 _$E200
// FUNCTION: STBC 0x006f0580 _$E206
// FUNCTION: STBC 0x006f0590 _$E205
// FUNCTION: STBC 0x006f05a0 _$E398
// FUNCTION: STBC 0x006f05b0 _$E397
// FUNCTION: STBC 0x006f05c0 _$E402
// FUNCTION: STBC 0x006f05d0 _$E401
// FUNCTION: STBC 0x006f05e0 _$E406
// FUNCTION: STBC 0x006f05f0 _$E405
// FUNCTION: STBC 0x006f0600 _$E410
// FUNCTION: STBC 0x006f0610 _$E409
// FUNCTION: STBC 0x006f0620 _$E414
// FUNCTION: STBC 0x006f0630 _$E413
// FUNCTION: STBC 0x006f0640 _$E418
// FUNCTION: STBC 0x006f0650 _$E417
// FUNCTION: STBC 0x006f0660 _$E422
// FUNCTION: STBC 0x006f0670 _$E421
// FUNCTION: STBC 0x006f0680 _$E426
// FUNCTION: STBC 0x006f0690 _$E425
// FUNCTION: STBC 0x006f06a0 _$E211
// FUNCTION: STBC 0x006f06b0 _$E210
// FUNCTION: STBC 0x006f06c0 _$E430
// FUNCTION: STBC 0x006f06d0 _$E429
// FUNCTION: STBC 0x006f06e0 _$E216
// FUNCTION: STBC 0x006f06f0 _$E215
// FUNCTION: STBC 0x006f0700 _$E221
// FUNCTION: STBC 0x006f0710 _$E220

// The object's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006f0720 _$E436
// FUNCTION: STBC 0x006f0730 _$E432
// FUNCTION: STBC 0x006f0760 _$E435
// FUNCTION: STBC 0x006f0770 _$E433
// DATA: STBC 0x0099a5d0
NiTList<TGObject *> TGObject::pendingObjects( LOAD_LIST_BLOCK );

// The object's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006f07e0 _$E441
// FUNCTION: STBC 0x006f07f0 _$E438
// FUNCTION: STBC 0x006f0820 _$E440
// FUNCTION: STBC 0x006f0830 _$E439
// DATA: STBC 0x0099a538
NiTList<TGFixUpCallback *> TGObject::fixUpCallbacks( LOAD_LIST_BLOCK );

// The object's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006f08a0 _$E446
// FUNCTION: STBC 0x006f08b0 _$E443
// FUNCTION: STBC 0x006f08f0 _$E445
// FUNCTION: STBC 0x006f0900 _$E444
// DATA: STBC 0x0099a578
NiTMap<int, TGObjectCreator> TGObject::creators;

// The sizes of the objects loaded, by GetObjType's value. A file-scope static,
// not a class member: its exit-time destructor runs unguarded, where the class
// statics' above test a guard bit first.
// The object's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006f0990 _$E451
// FUNCTION: STBC 0x006f09a0 _$E448
// FUNCTION: STBC 0x006f09e0 _$E450
// FUNCTION: STBC 0x006f09f0 _$E449
// DATA: STBC 0x0099a598
static NiTMap<int, TGLoadSizes> loadSizes;					// invented name

// DATA: STBC 0x0099a67c
NiTMap<long, TGObject *> *TGObject::objectTable;

// DATA: STBC 0x0095b078
long TGObject::nextID = 1;

// DATA: STBC 0x0095b07c
bool TGObject::keepObjectTable = true;

// DATA: STBC 0x0095b07d
bool TGObject::incrementID = true;

// The digits FormatAddress writes.
// DATA: STBC 0x0095b080
static char hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

// What GetIndentString returns the tail of.
// DATA: STBC 0x0095b090
static char indentSpaces[] =
	"                                                                "
	"                                                                "
	"                                                                "
	"                                                                ";

// How many objects WriteToStream has saved.
// DATA: STBC 0x0099a684
static int numSaved;

// How many objects LoadObject has read.
// DATA: STBC 0x0099a688
static int numLoaded;

// The file Log writes to (OpenLogFile).
// DATA: STBC 0x0099a680
static FILE *logFile;

/*
================
TGObject::TGObject

An ID of 0 takes the next free one; an explicit ID is kept, and moves the
next free one past it while incrementID is set.
================
*/
// FUNCTION: STBC 0x006f0a70
TGObject::TGObject( long id ) {
	if ( id == 0 ) {
		this->id = nextID;
		nextID++;
	} else {
		this->id = id;
		if ( incrementID && nextID <= id ) {
			nextID = id + 1;
		}
	}

	if ( objectTable == NULL ) {
		objectTable = new NiTMap<long, TGObject *>( OBJECT_TABLE_SIZE );
	}
	AddObject( this );
}

/*
================
TGObject::GetObjType
================
*/
// FUNCTION: STBC 0x006f0b60
int TGObject::GetObjType( void ) {
	return TGOBJECT_TYPE;
}

/*
================
TGObject::~TGObject
================
*/
// FUNCTION: STBC 0x006f0ba0
TGObject::~TGObject( void ) {
	TGEvent::RemoveObjectReferences( this );
	RemoveObject( this );
}

/*
================
FormatSaving

Prints WriteToStream's log line (invented name; the helper itself is shown by
the bytes). Retail evaluates the arguments, applies numSaved's increment after
GetClassName returns and only then calls sprintf: the sequence point of a call
between them, which a single sprintf( ..., numSaved++ ) doesn't have.
================
*/
static inline void FormatSaving( char *text, long id, const char *name, int type, int number ) {
	sprintf( text, "ID %d Saving %s (%d) number %d\n", id, name, type, number );
}

/*
================
FormatLoading

Prints LoadObject's log line (invented name), as FormatSaving: retail applies
numLoaded's increment after GetClassName returns, before sprintf.
================
*/
static inline void FormatLoading( char *text, long id, const char *name, int type, int number, int size ) {
	sprintf( text, "ID %d Loading %s (%d) number %d of size %d\n", id, name, type, number, size );
}

/*
================
TGObject::WriteToStream

Writes the type and the ID; derived classes write their members after.
================
*/
// FUNCTION: STBC 0x006f0bc0
bool TGObject::WriteToStream( TGStream *stream ) {
	char text[256];
	// MATCH-PENDING: R17, exact only through the invented inline FormatSaving wrapping sprintf; the bytes show a call boundary before sprintf but nothing shows the helper (user's ruling, 2026-11-22)

	int type = GetObjType();
	FormatSaving( text, id, GetClassName(), type, numSaved++ );
	Log( text );

	stream->WriteInt( type );
	stream->WriteID( id );
	return true;
}

/*
================
TGObject::LoadObject

Reads an object WriteToStream wrote, with its ID moved by idOffset: the
object registered under that ID, else a new one of the type. Records its
size for CloseLogFile and queues it for the fix-up pass. NULL for a type
no class registered.
================
*/
// FUNCTION: STBC 0x006f0c40
TGObject *TGObject::LoadObject( TGStream *stream, long idOffset ) {
	char text[256];
	// MATCH-PENDING: R17, exact only through the invented inline FormatLoading wrapping sprintf; the bytes show a call boundary before sprintf but nothing shows the helper (user's ruling, 2026-11-22)

	int start = stream->GetPos();
	int type = stream->ReadInt();
	long id = stream->ReadID() + idOffset;

	TGObject *object = GetTGObjectPtr( id );
	if ( object != NULL ) {
		object->GetObjType();
	} else {
		object = CreateObject( type, id );
		if ( object == NULL ) {
			return NULL;
		}
	}

	object->ReadFromStream( stream );
	int size = stream->GetPos() - start;
	FormatLoading( text, object->id, object->GetClassName(), type, numLoaded++, size );
	Log( text );

	TGLoadSizes sizes;
	if ( loadSizes.GetAt( object->GetObjType(), sizes ) ) {
		sizes.totalSize += size;
		if ( sizes.largestSize < size ) {
			sizes.largestID = object->id;
			sizes.largestSize = size;
		}
	} else {
		sizes.largestID = object->id;
		sizes.largestSize = size;
		sizes.totalSize = size;
	}
	loadSizes.SetAt( object->GetObjType(), sizes );

	// an object already pending is not added twice
	NiTListIterator pos = pendingObjects.GetHeadPos();
	while ( pos ) {
		if ( pendingObjects.GetNext( pos ) == object ) {
			return object;
		}
	}
	pendingObjects.AddHead( object );
	return object;
}

/*
================
TGObject::GetTGObjectPtr

The object with this ID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x006f0ee0
TGObject *TGObject::GetTGObjectPtr( long id ) {
	TGObject *object;
	if ( objectTable->GetAt( id, object ) ) {
		return object;
	}
	return NULL;
}

/*
================
TGObject::AddObject

Registers the object under its ID, replacing any object registered there.
================
*/
// FUNCTION: STBC 0x006f0f30
bool TGObject::AddObject( TGObject *object ) {
	objectTable->SetAt( object->id, object );
	return true;
}

/*
================
TGObject::RemoveObject

Unregisters the object; false when it was not registered. Once shutdown has
begun (ReleaseObjectTable), the last removal deletes the table.

The table pointer is not cleared after the delete.
================
*/
// FUNCTION: STBC 0x006f0fc0
bool TGObject::RemoveObject( TGObject *object ) {
	bool removed = objectTable->RemoveAt( object->id );
	if ( !keepObjectTable && objectTable != NULL && objectTable->GetCount() == 0 ) {
		delete objectTable;
	}
	return removed;
}

/*
================
TGObject::ReleaseObjectTable

Shutdown: the table goes now if it is empty, else with its last object.

The table pointer is not cleared after the delete.
================
*/
// FUNCTION: STBC 0x006f1080
void TGObject::ReleaseObjectTable( void ) {
	keepObjectTable = false;
	if ( objectTable != NULL && objectTable->GetCount() == 0 ) {
		delete objectTable;
	}
}

/*
================
TGObject::RemoveAllObjects

Forgets every object without deleting any.
================
*/
// FUNCTION: STBC 0x006f10b0
void TGObject::RemoveAllObjects( void ) {
	objectTable->RemoveAll();
}

/*
================
TGObject::GetNumObjects
================
*/
// FUNCTION: STBC 0x006f1110
int TGObject::GetNumObjects( void ) {
	return objectTable->GetCount();
}

/*
================
TGObject::AddFixUpCallback

Queues function( argument1, argument2 ) for the end of the fix-up pass.
================
*/
// FUNCTION: STBC 0x006f1120
bool TGObject::AddFixUpCallback( bool ( *function )( void *, void * ), void *argument1, void *argument2 ) {
	TGFixUpCallback *callback = new TGFixUpCallback;
	callback->function = function;
	callback->argument1 = argument1;
	callback->argument2 = argument2;
	fixUpCallbacks.AddTail( callback );
	return true;
}

/*
================
TGObject::FixUpLoadedObjects

The end of a load: fixes up every object LoadObject read, then makes the
calls AddFixUpCallback queued. False when something failed; the objects
that failed stay queued.
================
*/
// FUNCTION: STBC 0x006f11b0
bool TGObject::FixUpLoadedObjects( void ) {
	char text[256];
	bool fixed = true;

	NiTListIterator pos = pendingObjects.GetHeadPos();
	while ( pos != NULL ) {
		NiTListIterator objectPos = pos;
		TGObject *object = pendingObjects.GetNext( pos );

		char *className = object->GetClassName();
		long id = object->GetObjID();
		sprintf( text, "ID %d fixing up %s (%d)... ", id, className, object->GetObjType() );
		Log( text );

		if ( object->FixUpPointers() ) {
			object->PostFixUp();
			pendingObjects.RemovePos( objectPos );
			Log( "okay\n" );
		} else {
			Log( "FAILED\n" );
			fixed = false;
		}
		if ( !fixed ) {
			Log( "\nAARGH - FAILED\n\n" );
		}
	}

	Log( "\nFinished checks, now onto the functions\n\n" );
	if ( !fixed ) {
		Log( "\nAARGH\n\n" );
	}

	while ( !fixUpCallbacks.IsEmpty() ) {
		TGFixUpCallback *callback = fixUpCallbacks.RemoveHead();
		if ( callback != NULL ) {
			fixed &= callback->function( callback->argument1, callback->argument2 );
			if ( !fixed ) {
				Log( "\nFAILED - Unknown why\n\n" );
			}
		}
		delete callback;
	}
	return fixed;
}

/*
================
TGObject::FixUpObject

Turns an object's IDs back into pointers; false while one of them names
no object yet.
================
*/
// FUNCTION: STBC 0x006f13c0
bool TGObject::FixUpObject( TGObject *object ) {
	bool fixed = object->FixUpPointers();
	object->PostFixUp();
	return fixed;
}

/*
================
TGObject::CreateObject

A new object of the type from its class's creator; NULL for a type no
class registered.
================
*/
// FUNCTION: STBC 0x006f13e0
TGObject *TGObject::CreateObject( int type, long id ) {
	TGObjectCreator creator;
	if ( creators.GetAt( type, creator ) ) {
		return creator( id );
	}
	return NULL;
}

/*
================
TGObject::ResetNextID

Moves the next free ID just past the highest ID in use.
================
*/
// FUNCTION: STBC 0x006f1440
// PARKED: same calls and control flow; retail keeps nextID in EAX across the walk (stored from it, reloaded only after the rehash path), this build reloads it at every compare; reordering and an int nextID change nothing
void TGObject::ResetNextID( void ) {
	NiTMapIterator pos = objectTable->GetFirstPos();
	nextID = 1;
	while ( pos ) {
		long key;
		TGObject *object;
		objectTable->GetNext( pos, key, object );
		if ( object != NULL && object->id >= nextID ) {
			nextID = object->id + 1;
		}
	}
}

/*
================
TGObject::FormatAddress

The object's address in lower-case hex without leading zeros; an empty
string for address 0.
================
*/
// FUNCTION: STBC 0x006f14e0
void TGObject::FormatAddress( char *buffer ) {
	unsigned long address = (unsigned long)this;

	int nibble = 0;
	while ( ( address & 0xF0000000 ) == 0 && nibble < 7 ) {
		address <<= 4;
		nibble++;
	}
	for ( ; nibble < 8; nibble++ ) {
		*buffer++ = hexDigits[address >> 28];
		address <<= 4;
	}
	*buffer = '\0';
}

/*
================
TGObject::GetClassName
================
*/
// FUNCTION: STBC 0x006f1540
char *TGObject::GetClassName( void ) {
	return "TGObject";
}

/*
================
TGObject::GetSwigTypeName

The type part of a SWIG pointer string.
================
*/
// FUNCTION: STBC 0x006f1550
char *TGObject::GetSwigTypeName( void ) {
	return "_p_TGObject";
}

/*
================
TGObject::GetPythonClassName

The App module's shadow class for this type.
================
*/
// FUNCTION: STBC 0x006f1560
char *TGObject::GetPythonClassName( void ) {
	return "TGObjectPtr";
}

/*
================
TGObject::GetSwigPointer

The SWIG pointer string of this object: "_", the address in hex, then the
type ("_8a3f00_p_TGObject").
================
*/
// FUNCTION: STBC 0x006f1570
void TGObject::GetSwigPointer( char *buffer ) {
	*buffer++ = '_';
	FormatAddress( buffer );
	strcat( buffer, GetSwigTypeName() );
}

/*
================
TGObject::VirtualSlot08

A new instance of the object's Python shadow class (GetPythonClassName in
App) wrapping its SWIG pointer string; None when App has no such class.
================
*/
// FUNCTION: STBC 0x006f15c0
PyObject *TGObject::VirtualSlot08( void ) {
	char pointer[128];
	GetSwigPointer( pointer );
	PyObject *args = Py_BuildValue( "(s)", pointer );

	PyObject *appDict = PyEmbed::appDict;
	PyObject *pythonClass = PyDict_GetItemString( appDict, GetPythonClassName() );
	if ( pythonClass == NULL ) {
		// the original leaks args here
		Py_INCREF( Py_None );
		return Py_None;
	}

	PyObject *instance = PyInstance_New( pythonClass, args, NULL );
	Py_DECREF( args );
	return instance;
}

/*
================
TGObject::Print

The release build prints nothing; only the evaluation of the indent and
the address remains.
================
*/
// FUNCTION: STBC 0x006f1650
void TGObject::Print( int indent, int depth ) {
	char address[12];

	GetIndentString( indent );
	FormatAddress( address );
}

/*
================
TGObject::GetIndentString

A string of INDENT_WIDTH spaces per level.
================
*/
// FUNCTION: STBC 0x006f1680
char *TGObject::GetIndentString( int indent ) {
	if ( indent > MAX_INDENT_LEVEL ) {
		indent = MAX_INDENT_LEVEL;
	}
	return &indentSpaces[( MAX_INDENT_LEVEL - indent ) * INDENT_WIDTH];
}

/*
================
TGObject::SetObjID

Re-registers the object under a new ID.
================
*/
// FUNCTION: STBC 0x006f16a0
void TGObject::SetObjID( long id ) {
	RemoveObject( this );
	this->id = id;
	AddObject( this );
}

/*
================
TGObject::OpenLogFile
================
*/
// FUNCTION: STBC 0x006f16c0
void TGObject::OpenLogFile( const char *fileName ) {
	if ( logFile != NULL ) {
		fclose( logFile );
	}
	logFile = fopen( fileName, "w" );
}

/*
================
TGObject::CloseLogFile

Reports the biggest object of each type loaded, forgets the sizes, and
closes the log.
================
*/
// FUNCTION: STBC 0x006f16f0
void TGObject::CloseLogFile( void ) {
	char text[256];

	if ( loadSizes.GetCount() != 0 ) {
		Log( "\n\nBIG OBJECT LIST\n\n" );

		NiTMapIterator pos = loadSizes.GetFirstPos();
		while ( pos != NULL ) {
			int type;
			TGLoadSizes sizes;
			loadSizes.GetNext( pos, type, sizes );

			TGObject *object = GetTGObjectPtr( sizes.largestID );
			if ( object != NULL ) {
				char *className = object->GetClassName();
				sprintf( text, "ID %d type %s (%d) of size %d, total size %d\n", sizes.largestID, className, type, sizes.largestSize, sizes.totalSize );
				Log( text );
			} else {
				sprintf( text, "ID %d invalid (type %d), size %d, total size %d\n", sizes.largestID, type, sizes.largestSize, sizes.totalSize );
				Log( text );
			}
		}
		loadSizes.RemoveAll();
	}

	if ( logFile != NULL ) {
		fclose( logFile );
	}
	logFile = NULL;
}

/*
================
TGObject::Log

Empty in the release build.
================
*/
// FUNCTION: STBC 0x006f1880
void TGObject::Log( const char *text ) {
}
