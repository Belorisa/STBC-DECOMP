/*
===============================================================================

	ObjectGroup.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "ObjectGroup.h"
#include "PyEmbed.h"
#include "ObjectClass.h"
#include "PhysicsObjectClass.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "TGObjPtrEvent.h"
#include "SetClass.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGSystemWrapperClass.h"
#include "TGEvent.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0059e4a0 _$E477
// FUNCTION: STBC 0x0059e4c0 _$E476

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0059de20 _$E32
// FUNCTION: STBC 0x0059de30 _$E29
// FUNCTION: STBC 0x0059de40 _$E31
// FUNCTION: STBC 0x0059de50 _$E30
// FUNCTION: STBC 0x0059de60 _$E38
// FUNCTION: STBC 0x0059de70 _$E35
// FUNCTION: STBC 0x0059de80 _$E37
// FUNCTION: STBC 0x0059de90 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0059dea0 _$E87
// FUNCTION: STBC 0x0059deb0 _$E86
// FUNCTION: STBC 0x0059dec0 _$E258
// FUNCTION: STBC 0x0059ded0 _$E257
// FUNCTION: STBC 0x0059dee0 _$E98
// FUNCTION: STBC 0x0059def0 _$E97
// FUNCTION: STBC 0x0059df00 _$E102
// FUNCTION: STBC 0x0059df10 _$E101
// FUNCTION: STBC 0x0059df20 _$E106
// FUNCTION: STBC 0x0059df30 _$E105
// FUNCTION: STBC 0x0059df40 _$E110
// FUNCTION: STBC 0x0059df50 _$E109
// FUNCTION: STBC 0x0059df60 _$E227
// FUNCTION: STBC 0x0059df70 _$E226
// FUNCTION: STBC 0x0059df80 _$E169
// FUNCTION: STBC 0x0059df90 _$E168
// FUNCTION: STBC 0x0059dfa0 _$E182
// FUNCTION: STBC 0x0059dfb0 _$E181
// FUNCTION: STBC 0x0059dfc0 _$E232
// FUNCTION: STBC 0x0059dfd0 _$E231
// FUNCTION: STBC 0x0059dfe0 _$E263
// FUNCTION: STBC 0x0059dff0 _$E262
// FUNCTION: STBC 0x0059e000 _$E236
// FUNCTION: STBC 0x0059e010 _$E235
// FUNCTION: STBC 0x0059e020 _$E267
// FUNCTION: STBC 0x0059e030 _$E266
// FUNCTION: STBC 0x0059e040 _$E274
// FUNCTION: STBC 0x0059e050 _$E273
// FUNCTION: STBC 0x0059e060 _$E296
// FUNCTION: STBC 0x0059e070 _$E295
// FUNCTION: STBC 0x0059e080 _$E300
// FUNCTION: STBC 0x0059e090 _$E299
// FUNCTION: STBC 0x0059e0a0 _$E127
// FUNCTION: STBC 0x0059e0b0 _$E126
// FUNCTION: STBC 0x0059e0c0 _$E282
// FUNCTION: STBC 0x0059e0d0 _$E281
// FUNCTION: STBC 0x0059e0e0 _$E289
// FUNCTION: STBC 0x0059e0f0 _$E288
// FUNCTION: STBC 0x0059e120 _$E312
// FUNCTION: STBC 0x0059e130 _$E311
// FUNCTION: STBC 0x0059e140 _$E316
// FUNCTION: STBC 0x0059e150 _$E315
// FUNCTION: STBC 0x0059e160 _$E356
// FUNCTION: STBC 0x0059e170 _$E355
// FUNCTION: STBC 0x0059e180 _$E362
// FUNCTION: STBC 0x0059e190 _$E361
// FUNCTION: STBC 0x0059e1a0 _$E304
// FUNCTION: STBC 0x0059e1b0 _$E303
// FUNCTION: STBC 0x0059e1c0 _$E308
// FUNCTION: STBC 0x0059e1d0 _$E307
// FUNCTION: STBC 0x0059e1e0 _$E320
// FUNCTION: STBC 0x0059e1f0 _$E319
// FUNCTION: STBC 0x0059e200 _$E324
// FUNCTION: STBC 0x0059e210 _$E323
// FUNCTION: STBC 0x0059e220 _$E328
// FUNCTION: STBC 0x0059e230 _$E327
// FUNCTION: STBC 0x0059e240 _$E332
// FUNCTION: STBC 0x0059e250 _$E331
// FUNCTION: STBC 0x0059e260 _$E336
// FUNCTION: STBC 0x0059e270 _$E335
// FUNCTION: STBC 0x0059e280 _$E340
// FUNCTION: STBC 0x0059e290 _$E339
// FUNCTION: STBC 0x0059e2a0 _$E344
// FUNCTION: STBC 0x0059e2b0 _$E343
// FUNCTION: STBC 0x0059e2c0 _$E348
// FUNCTION: STBC 0x0059e2d0 _$E347
// FUNCTION: STBC 0x0059e2e0 _$E132
// FUNCTION: STBC 0x0059e2f0 _$E131
// FUNCTION: STBC 0x0059e300 _$E352
// FUNCTION: STBC 0x0059e310 _$E351
// FUNCTION: STBC 0x0059e320 _$E137
// FUNCTION: STBC 0x0059e330 _$E136
// FUNCTION: STBC 0x0059e340 _$E366
// FUNCTION: STBC 0x0059e350 _$E365
// FUNCTION: STBC 0x0059e360 _$E370
// FUNCTION: STBC 0x0059e370 _$E369
// FUNCTION: STBC 0x0059e380 _$E374
// FUNCTION: STBC 0x0059e390 _$E373
// FUNCTION: STBC 0x0059e3a0 _$E378
// FUNCTION: STBC 0x0059e3b0 _$E377
// FUNCTION: STBC 0x0059e3c0 _$E146
// FUNCTION: STBC 0x0059e3d0 _$E145
// FUNCTION: STBC 0x0059e3e0 _$E382
// FUNCTION: STBC 0x0059e3f0 _$E381
// FUNCTION: STBC 0x0059e400 _$E386
// FUNCTION: STBC 0x0059e410 _$E385
// FUNCTION: STBC 0x0059e420 _$E390
// FUNCTION: STBC 0x0059e430 _$E389
// FUNCTION: STBC 0x0059e440 _$E152
// FUNCTION: STBC 0x0059e450 _$E151
// FUNCTION: STBC 0x0059e460 _$E157
// FUNCTION: STBC 0x0059e470 _$E156
// FUNCTION: STBC 0x0059e480 _$E187
// FUNCTION: STBC 0x0059e490 _$E186
// FUNCTION: STBC 0x0059e4d0 _$E394
// FUNCTION: STBC 0x0059e4e0 _$E393
// FUNCTION: STBC 0x0059e4f0 _$E192
// FUNCTION: STBC 0x0059e500 _$E191
// FUNCTION: STBC 0x0059e510 _$E398
// FUNCTION: STBC 0x0059e520 _$E397
// FUNCTION: STBC 0x0059e530 _$E402
// FUNCTION: STBC 0x0059e540 _$E401
// FUNCTION: STBC 0x0059e550 _$E197
// FUNCTION: STBC 0x0059e560 _$E196
// FUNCTION: STBC 0x0059e570 _$E406
// FUNCTION: STBC 0x0059e580 _$E405
// FUNCTION: STBC 0x0059e590 _$E162
// FUNCTION: STBC 0x0059e5a0 _$E161
// FUNCTION: STBC 0x0059e5b0 _$E410
// FUNCTION: STBC 0x0059e5c0 _$E409
// FUNCTION: STBC 0x0059e5d0 _$E414
// FUNCTION: STBC 0x0059e5e0 _$E413
// FUNCTION: STBC 0x0059e5f0 _$E202
// FUNCTION: STBC 0x0059e600 _$E201
// FUNCTION: STBC 0x0059e610 _$E207
// FUNCTION: STBC 0x0059e620 _$E206
// FUNCTION: STBC 0x0059e630 _$E418
// FUNCTION: STBC 0x0059e640 _$E417
// FUNCTION: STBC 0x0059e650 _$E422
// FUNCTION: STBC 0x0059e660 _$E421
// FUNCTION: STBC 0x0059e670 _$E426
// FUNCTION: STBC 0x0059e680 _$E425
// FUNCTION: STBC 0x0059e690 _$E430
// FUNCTION: STBC 0x0059e6a0 _$E429
// FUNCTION: STBC 0x0059e6b0 _$E434
// FUNCTION: STBC 0x0059e6c0 _$E433
// FUNCTION: STBC 0x0059e6d0 _$E438
// FUNCTION: STBC 0x0059e6e0 _$E437
// FUNCTION: STBC 0x0059e6f0 _$E442
// FUNCTION: STBC 0x0059e700 _$E441
// FUNCTION: STBC 0x0059e710 _$E446
// FUNCTION: STBC 0x0059e720 _$E445
// FUNCTION: STBC 0x0059e730 _$E212
// FUNCTION: STBC 0x0059e740 _$E211
// FUNCTION: STBC 0x0059e750 _$E450
// FUNCTION: STBC 0x0059e760 _$E449
// FUNCTION: STBC 0x0059e770 _$E217
// FUNCTION: STBC 0x0059e780 _$E216
// FUNCTION: STBC 0x0059e790 _$E222
// FUNCTION: STBC 0x0059e7a0 _$E221
// FUNCTION: STBC 0x0059e7b0 _$E454
// FUNCTION: STBC 0x0059e7c0 _$E453
// FUNCTION: STBC 0x0059e7d0 _$E459
// FUNCTION: STBC 0x0059e7e0 _$E458
// FUNCTION: STBC 0x0059e7f0 _$E463
// FUNCTION: STBC 0x0059e800 _$E462

// The flag that has the group tell its handlers of its objects entering a set.
#define EVENTFLAG_ENTERED_SET		2

// The flag whose handler watches for objects leaving a set.
#define EVENTFLAG_EXITED_SET		4

// The flag that has the group tell its handlers of its objects being destroyed.
#define EVENTFLAG_DESTROYED			8

// The bit of the event flags that has the group tell its handlers about a
// change of its count.
#define EVENTFLAG_COUNT_CHANGED		1

// The handler table's dynamic initialiser and its exit-time destructor.
// FUNCTION: STBC 0x0059c760 _$E469
// FUNCTION: STBC 0x0059c770 _$E465
// FUNCTION: STBC 0x0059c780 _$E468
// FUNCTION: STBC 0x0059c790 _$E466
// DATA: STBC 0x0098dfa0
TGEventHandlerTable ObjectGroup::classHandlers;

/*
================
ObjectGroup::ObjectGroup

An empty group; it hears of objects entering a set and of objects being
destroyed.
================
*/
// FUNCTION: STBC 0x0059c7b0 ??0ObjectGroup@@QAE@XZ
ObjectGroup::ObjectGroup( void ) {
	unknown44 = -1;
	eventFlags = 0;
	g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "ObjectGroup::EnteredSet" );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_DESTROYED, this, "ObjectGroup::ObjectDestroyed" );
}

/*
================
ObjectGroup::~ObjectGroup
================
*/
// FUNCTION: STBC 0x0059c890 ??1ObjectGroup@@UAE@XZ
ObjectGroup::~ObjectGroup( void ) {
	while ( names.GetSize() != 0 ) {
		delete names.RemoveEnd();
	}
}

/*
================
ObjectGroup::RemoveName

Takes the name out of the group, and the object of that name out of those
present. True when the group had the name.
================
*/
// FUNCTION: STBC 0x0059cb10 ?RemoveName@ObjectGroup@@UAE_NPAD@Z
bool ObjectGroup::RemoveName( char *name ) {
	if ( name == NULL ) {
		return false;
	}

	bool removed = false;
	for ( unsigned int i = 0; i < names.GetSize(); i++ ) {
		char *current = names.GetAt( i );
		if ( strcmp( name, current ) == 0 ) {
			delete current;
			for ( unsigned int j = i + 1; j < names.GetSize(); j++ ) {
				names.SetAt( j - 1, names.GetAt( j ) );
			}
			names.RemoveEnd();
			removed = true;
		}
	}

	for ( unsigned int k = 0; k < activeObjects.GetSize(); k++ ) {
		if ( strcmp( activeObjects.GetAt( k )->GetName(), name ) == 0 ) {
			activeObjects.SetAt( k, activeObjects.GetAt( activeObjects.GetSize() - 1 ) );
			activeObjects.RemoveEnd();
			break;
		}
	}

	PostCountChanged();
	return removed;
}

/*
================
ObjectGroup::RemoveAllNames
================
*/
// FUNCTION: STBC 0x0059cd10 ?RemoveAllNames@ObjectGroup@@UAE_NXZ
bool ObjectGroup::RemoveAllNames( void ) {
	for ( unsigned int i = 0; i < names.GetSize(); i++ ) {
		char *name = names.GetAt( i );
		if ( name != NULL ) {
			delete name;
		}
	}
	names.RemoveAll();
	activeObjects.RemoveAll();
	PostCountChanged();
	return true;
}

/*
================
ObjectGroup::WriteToStream

The names, the IDs of the objects present, the change stamp and the event
flags.
================
*/
// FUNCTION: STBC 0x0059d820
bool ObjectGroup::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}

	int count = names.GetSize();
	stream->WriteLong( count );
	int i;
	for ( i = 0; i < count; i++ ) {
		stream->VirtualSlot11( names.GetAt( i ) );
	}

	count = activeObjects.GetSize();
	stream->WriteLong( count );
	for ( i = 0; i < count; i++ ) {
		stream->WriteID( GetSavedID( (TGObject *)activeObjects.GetAt( i ) ) );
	}

	stream->WriteLong( unknown44 );
	stream->WriteChar( eventFlags );
	return true;
}

/*
================
ObjectGroup::ReadFromStream

Reads what WriteToStream wrote; the objects stay IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x0059d8b0
bool ObjectGroup::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}

	int count;
	for ( count = stream->ReadLong(); count > 0; count-- ) {
		names.Add( stream->VirtualSlot10( -1 ) );
	}
	for ( count = stream->ReadLong(); count > 0; count-- ) {
		activeObjects.Add( (ObjectClass *)stream->ReadID() );
	}

	unknown44 = stream->ReadLong();
	eventFlags = stream->ReadChar();
	return true;
}

/*
================
ObjectGroup::FixUpPointers

Fails until every object present is loaded.
================
*/
// FUNCTION: STBC 0x0059dac0
bool ObjectGroup::FixUpPointers( void ) {
	if ( !TGEventHandlerObject::FixUpPointers() ) {
		return false;
	}
	int count = activeObjects.GetSize();
	for ( int i = 0; i < count; i++ ) {
		ObjectClass *object = activeObjects.GetAt( i );
		if ( object != NULL && TGObject::GetTGObjectPtr( (long)object ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
ObjectGroup::PostFixUp

Turns the saved IDs of the objects present back into the objects.
================
*/
// FUNCTION: STBC 0x0059db00
void ObjectGroup::PostFixUp( void ) {
	TGEventHandlerObject::PostFixUp();
	int count = activeObjects.GetSize();
	for ( int i = 0; i < count; i++ ) {
		activeObjects.SetAt( i, (ObjectClass *)GetSavedObject( (long)activeObjects.GetAt( i ) ) );
	}
}

/*
================
ObjectGroup::SetEventFlag

Sets the flag; the exited-set flag also has the group hear of objects
leaving a set.
================
*/
// FUNCTION: STBC 0x0059c900 ?SetEventFlag@ObjectGroup@@QAEXH@Z
void ObjectGroup::SetEventFlag( int flag ) {
	if ( ( eventFlags & flag ) == 0 ) {
		eventFlags |= flag;
		if ( flag == EVENTFLAG_EXITED_SET ) {
			g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, this, "ObjectGroup::ExitedSet", ANY_TARGET );
		}
	}
}

/*
================
ObjectGroup::ClearEventFlag
================
*/
// FUNCTION: STBC 0x0059c940 ?ClearEventFlag@ObjectGroup@@QAEXH@Z
void ObjectGroup::ClearEventFlag( int flag ) {
	if ( eventFlags & flag ) {
		eventFlags &= ~flag;
		if ( flag == EVENTFLAG_EXITED_SET ) {
			g_kEventManager.RemoveBroadcastHandler( ET_EXITED_SET, this, "ObjectGroup::ExitedSet", ANY_TARGET );
		}
	}
}

/*
================
ObjectGroup::GetNameTuple

The group's names, as a new tuple of Python strings.
================
*/
// FUNCTION: STBC 0x0059cd90
PyObject *ObjectGroup::GetNameTuple( void ) {
	PyObject *tuple = PyTuple_New( names.GetSize() );
	for ( unsigned int i = 0; i < names.GetSize(); i++ ) {
		PyTuple_SET_ITEM( tuple, i, PyString_FromString( names.GetAt( i ) ) );
	}
	return tuple;
}

/*
================
ObjectGroup_GetNameTuple

The Python method ObjectGroup.GetNameTuple (added to App's shadow class by
the objects module).
================
*/
// FUNCTION: STBC 0x0059cde0
PyObject *ObjectGroup_GetNameTuple( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		ObjectGroup *group;
		PyEmbed::Unknown6F98E0( instance, (void **)&group, "ObjectGroup *" );
		return group->GetNameTuple();
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
ObjectGroup::GetActiveObjectTuple

The objects of the group present now, as a new tuple of their Python
objects.
================
*/
// FUNCTION: STBC 0x0059ce40
PyObject *ObjectGroup::GetActiveObjectTuple( void ) {
	PyObject *tuple = PyTuple_New( activeObjects.GetSize() );
	for ( unsigned int i = 0; i < activeObjects.GetSize(); i++ ) {
		PyTuple_SetItem( tuple, i, activeObjects.GetAt( i )->VirtualSlot08() );
	}
	return tuple;
}

/*
================
ObjectGroup_GetActiveObjectTuple

The Python method ObjectGroup.GetActiveObjectTuple.
================
*/
// FUNCTION: STBC 0x0059ce80
PyObject *ObjectGroup_GetActiveObjectTuple( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		ObjectGroup *group;
		PyEmbed::Unknown6F98E0( instance, (void **)&group, "ObjectGroup *" );
		return group->GetActiveObjectTuple();
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
ObjectGroup::GetActiveObjectTupleInSet

The objects of the group present in the set, as a new tuple of their
Python objects; an empty tuple for no set.
================
*/
// FUNCTION: STBC 0x0059cee0
// PARKED: register allocation: retail keeps this in EDI and the set in ESI (reloading it from its slot in the second loop), and the index in ESI there; this build gives this ESI (c2trace --regs: this prio 39 takes esi first)
PyObject *ObjectGroup::GetActiveObjectTupleInSet( SetClass *set ) {
	if ( set == NULL ) {
		return PyTuple_New( 0 );
	}
	unsigned int i;
	int count = 0;
	for ( i = 0; i < activeObjects.GetSize(); i++ ) {
		if ( activeObjects.GetAt( i )->GetContainingSet() == set ) {
			count++;
		}
	}
	PyObject *tuple = PyTuple_New( count );
	int index = 0;
	for ( i = 0; i < activeObjects.GetSize(); i++ ) {
		if ( activeObjects.GetAt( i )->GetContainingSet() == set ) {
			PyTuple_SetItem( tuple, index, activeObjects.GetAt( i )->VirtualSlot08() );
			index++;
		}
	}
	return tuple;
}

/*
================
ObjectGroup_GetActiveObjectTupleInSet

The Python method ObjectGroup.GetActiveObjectTupleInSet.
================
*/
// FUNCTION: STBC 0x0059cf60
PyObject *ObjectGroup_GetActiveObjectTupleInSet( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	PyObject *setInstance = NULL;
	if ( PyArg_ParseTuple( args, "OO", &instance, &setInstance ) != -1 ) {
		SetClass *set = NULL;
		PyEmbed::Unknown6F98E0( setInstance, (void **)&set, "SetClass *" );
		ObjectGroup *group;
		PyEmbed::Unknown6F98E0( instance, (void **)&group, "ObjectGroup *" );
		return group->GetActiveObjectTupleInSet( set );
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
ObjectGroup::IsNameInGroup
================
*/
// FUNCTION: STBC 0x0059cff0 ?IsNameInGroup@ObjectGroup@@QAE_NPAD@Z
bool ObjectGroup::IsNameInGroup( char *name ) {
	if ( name != NULL ) {
		for ( unsigned int i = 0; i < names.GetSize(); i++ ) {
			if ( strcmp( name, names.GetAt( i ) ) == 0 ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
ObjectGroup::AddName

Adds the name to the group, with the object of that name if it exists now.
================
*/
// FUNCTION: STBC 0x0059c990 ?AddName@ObjectGroup@@QAEXPAD@Z
void ObjectGroup::AddName( char *name ) {
	if ( name == NULL ) {
		return;
	}
	for ( unsigned int i = 0; i < names.GetSize(); i++ ) {
		if ( strcmp( name, names.GetAt( i ) ) == 0 ) {
			return;
		}
	}
	char *copy = new char[strlen( name ) + 1];
	strcpy( copy, name );
	names.Add( copy );
	ObjectClass *object = ObjectClass::GetObject( NULL, copy );
	if ( object != NULL ) {
		AddActiveObject( object );
	}
	PostCountChanged();
}

/*
================
ObjectGroup::AddActiveObject

Adds the object to those present, if it is not among them, and has the
group hear when it is deleted.
================
*/
// FUNCTION: STBC 0x0059d4b0 ?AddActiveObject@ObjectGroup@@QAEXPAVObjectClass@@@Z
void ObjectGroup::AddActiveObject( ObjectClass *object ) {
	for ( unsigned int i = 0; i < activeObjects.GetSize(); i++ ) {
		if ( activeObjects.GetAt( i ) == object ) {
			return;
		}
	}
	activeObjects.Add( object );
	g_kEventManager.AddBroadcastMethodHandler( ET_DELETE_ME_STAGE_1, this, "ObjectGroup::ObjectDeleted", object );
}

/*
================
_wrap_ObjectGroup_ForceToGroup

The Python function App.ObjectGroup_ForceToGroup: makes a group of its
argument. A name gives a new group of that name, a group is returned as it
is, a sequence of one is forced through its item, and a longer sequence
gives a new group of its names and of the names of the groups its other
items force to. A new group is returned owned by Python; None when the
argument is none of these.
================
*/
// FUNCTION: STBC 0x0059d5b0 __wrap_ObjectGroup_ForceToGroup
// PARKED: declaration-sensitive frame slots: exact, or with the slots of group and created swapped, depending on the NetImmerse declarations the unit sees; source unchanged
extern "C" PyObject *_wrap_ObjectGroup_ForceToGroup( PyObject *self, PyObject *args ) {
	ObjectGroup *group = NULL;
	bool created = false;

	if ( PyTuple_Size( args ) == 1 ) {
		PyObject *object = PyTuple_GetItem( args, 0 );
		if ( object != NULL ) {
			int length = PyObject_Length( object );
			PyErr_Clear();
			if ( PyString_Check( object ) ) {
				group = new ObjectGroup;
				group->AddName( PyString_AsString( object ) );
				created = true;
			} else if ( length != -1 ) {
				if ( length == 1 ) {
					PyObject *itemArgs = PyTuple_New( 1 );
					// BUG: the index integer is never released
					PyTuple_SetItem( itemArgs, 0, PyObject_GetItem( object, PyInt_FromLong( 0 ) ) );
					PyObject *result = _wrap_ObjectGroup_ForceToGroup( NULL, itemArgs );
					Py_DECREF( itemArgs );
					return result;
				}
				group = new ObjectGroup;
				for ( int i = 0; i < length; i++ ) {
					// BUG: the index integer is never released, nor is a name item
					PyObject *item = PyObject_GetItem( object, PyInt_FromLong( i ) );
					if ( PyString_Check( item ) ) {
						group->AddName( PyString_AsString( item ) );
					} else {
						PyObject *itemArgs = PyTuple_New( 1 );
						PyTuple_SetItem( itemArgs, 0, item );
						PyObject *itemInstance = _wrap_ObjectGroup_ForceToGroup( NULL, itemArgs );
						ObjectGroup *itemGroup = NULL;
						PyEmbed::Unknown6F98E0( itemInstance, (void **)&itemGroup, "ObjectGroup" );
						if ( itemGroup != NULL ) {
							for ( int j = 0; j < itemGroup->GetNumNames(); j++ ) {
								group->AddName( itemGroup->GetNthName( j ) );
							}
						}
						Py_DECREF( itemInstance );
						Py_DECREF( itemArgs );
					}
				}
				created = true;
			} else if ( PyEmbed::IsInstance( object, "ObjectGroup" ) ) {
				Py_INCREF( object );
				return object;
			}
		}
	}

	PyObject *result = Py_None;
	if ( group != NULL ) {
		result = group->VirtualSlot08();
		if ( created ) {
			PyObject_SetAttrString( result, "thisown", PyInt_FromLong( 1 ) );
		}
	} else {
		Py_INCREF( Py_None );
	}
	return result;
}

/*
================
ObjectGroup::FromModule

The group the script module holds under the name; NULL, with the Python
error cleared, when it holds none.
================
*/
// FUNCTION: STBC 0x0059d060 ?FromModule@ObjectGroup@@SAPAV1@PAD0@Z
ObjectGroup *ObjectGroup::FromModule( char *moduleName, char *name ) {
	ObjectGroup *group;
	PyObject *module = PyEmbed::GetModule( moduleName, false );
	if ( module != NULL ) {
		PyObject *object = PyObject_GetAttrString( module, name );
		if ( object != NULL ) {
			group = NULL;
			PyEmbed::Unknown6F98E0( object, (void **)&group, "ObjectGroup" );
			Py_DECREF( object );
			if ( group != NULL ) {
				return group;
			}
		}
	}
	PyErr_Clear();
	return NULL;
}

/*
================
ObjectGroup::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0059d0d0
void ObjectGroup::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ObjectGroup::EnteredSet, "ObjectGroup::EnteredSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ObjectGroup::ExitedSet, "ObjectGroup::ExitedSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ObjectGroup::ObjectDestroyed, "ObjectGroup::ObjectDestroyed" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ObjectGroup::ObjectDeleted, "ObjectGroup::ObjectDeleted" );
}

/*
================
ObjectGroup::RegisterHandlers

The group has no class handlers.
================
*/
// FUNCTION: STBC 0x0059d130
void ObjectGroup::RegisterHandlers( void ) {
}

/*
================
ObjectGroup::EnteredSet

An object of one of the group's names entered a set: it is present, and
the group tells its handlers when it listens for that.
================
*/
// FUNCTION: STBC 0x0059d140
void ObjectGroup::EnteredSet( TGEvent *event ) {
	ObjectClass *object = (ObjectClass *)event->GetDestination();
	if ( object->GetName() != NULL && IsNameInGroup( object->GetName() ) ) {
		AddActiveObject( object );
		if ( eventFlags & EVENTFLAG_ENTERED_SET ) {
			TGObjPtrEvent *notice = new TGObjPtrEvent;
			notice->SetEventType( ET_OBJECT_GROUP_OBJECT_ENTERED_SET );
			notice->SetDestination( this );
			notice->SetObjPtr( object );
			g_kEventManager.AddEvent( notice );
		}
	}
	CallNextHandler( event );
}

/*
================
ObjectGroup::ExitedSet

An object of one of the group's names left a set: the group tells its
handlers.
================
*/
// FUNCTION: STBC 0x0059d1d0
void ObjectGroup::ExitedSet( TGEvent *event ) {
	ObjectClass *object = (ObjectClass *)event->GetDestination();
	if ( object->GetName() != NULL && IsNameInGroup( object->GetName() ) ) {
		TGObjPtrEvent *notice = new TGObjPtrEvent;
		notice->SetEventType( ET_OBJECT_GROUP_OBJECT_EXITED_SET );
		notice->SetDestination( this );
		notice->SetObjPtr( object );
		g_kEventManager.AddEvent( notice );
	}
	CallNextHandler( event );
}

/*
================
ObjectGroup::ObjectDestroyed

An object of one of the group's names was destroyed: it is no longer
present, and the group tells its handlers when it listens for that. The
event goes no further.
================
*/
// FUNCTION: STBC 0x0059d250
void ObjectGroup::ObjectDestroyed( TGEvent *event ) {
	ObjectClass *object = (ObjectClass *)event->GetDestination();
	if ( object->GetName() != NULL && IsNameInGroup( object->GetName() ) ) {
		for ( unsigned int i = 0; i < activeObjects.GetSize(); i++ ) {
			if ( object == activeObjects.GetAt( i ) ) {
				activeObjects.SetAt( i, activeObjects.GetAt( activeObjects.GetSize() - 1 ) );
				activeObjects.RemoveEnd();
				break;
			}
		}
		if ( eventFlags & EVENTFLAG_DESTROYED ) {
			TGObjPtrEvent *notice = new TGObjPtrEvent;
			notice->SetEventType( ET_OBJECT_GROUP_OBJECT_DESTROYED );
			notice->SetDestination( this );
			notice->SetObjPtr( object );
			g_kEventManager.AddEvent( notice );
		}
	}
}

/*
================
ObjectGroup::ObjectDeleted

A present object is being deleted: it is taken out of those present, the
others keeping their order.
================
*/
// FUNCTION: STBC 0x0059d370
void ObjectGroup::ObjectDeleted( TGEvent *event ) {
	for ( unsigned int i = 0; i < activeObjects.GetSize(); i++ ) {
		if ( activeObjects.GetAt( i ) == event->GetDestination() ) {
			for ( unsigned int j = i + 1; j < activeObjects.GetSize(); j++ ) {
				activeObjects.SetAt( j - 1, activeObjects.GetAt( j ) );
			}
			activeObjects.RemoveEnd();
			break;
		}
	}
	CallNextHandler( event );
}

/*
================
ObjectGroup::PostCountChanged

When the group listens for it, posts the count-changed event to itself,
once per update of the game.
================
*/
// FUNCTION: STBC 0x0059d440
void ObjectGroup::PostCountChanged( void ) {
	if ( eventFlags & EVENTFLAG_COUNT_CHANGED ) {
		int updateNumber = g_kSystemWrapper.GetUpdateNumber();
		if ( updateNumber != unknown44 ) {
			TGEvent *event = new TGEvent;
			event->SetEventType( ET_OBJECT_GROUP_CHANGED );
			event->SetDestination( this );
			g_kEventManager.AddEvent( event );
			unknown44 = updateNumber;
		}
	}
}

/*
================
ObjectGroup::SaveGroups

Writes every group there is: the count, then each group. The name is
invented, after CameraMode::SaveModes.
================
*/
// FUNCTION: STBC 0x0059db80
bool ObjectGroup::SaveGroups( TGStream *stream ) {
	bool result = true;
	NiTList<TGObject *> groups( LOAD_LIST_BLOCK );

	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		if ( object->IsTypeOf( OBJECTGROUP_TYPE ) ) {
			groups.AddTail( object );
		}
	}

	stream->WriteInt( groups.GetSize() );
	while ( !groups.IsEmpty() && result ) {
		result &= groups.RemoveHead()->WriteToStream( stream );
	}
	return result;
}

/*
================
ObjectGroup::LoadGroups

Reads the groups SaveGroups wrote; false when one is missing or no group.
The name is invented, after CameraMode::LoadModes.
================
*/
// FUNCTION: STBC 0x0059dd40
bool ObjectGroup::LoadGroups( TGStream *stream ) {
	bool result = true;
	for ( int i = stream->ReadInt() - 1; i >= 0 && result; i-- ) {
		TGObject *object = TGObject::LoadObject( stream, 0 );
		if ( object == NULL || !object->IsTypeOf( OBJECTGROUP_TYPE ) ) {
			result = false;
		}
	}
	return result;
}

/*
================
ObjectGroup::DeleteAllGroups

Deletes every group there is, before a saved game is loaded. The name is
invented, after TGSoundRegion::DeleteAllRegions.
================
*/
// FUNCTION: STBC 0x0059dd90
void ObjectGroup::DeleteAllGroups( void ) {
	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		if ( object->IsTypeOf( OBJECTGROUP_TYPE ) ) {
			delete object;
		}
	}
}
