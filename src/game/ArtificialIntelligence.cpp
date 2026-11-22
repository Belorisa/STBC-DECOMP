/*
===============================================================================

	ArtificialIntelligence.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "AILog.h"
#include "ArtificialIntelligence.h"
#include "AppConstants.h"
#include "PhysicsObjectClass.h"
#include "PyEmbed.h"
#include "ShipClass.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00471e70 _$E486
// FUNCTION: STBC 0x00471e90 _$E485

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004715b0 _$E32
// FUNCTION: STBC 0x004715c0 _$E29
// FUNCTION: STBC 0x004715d0 _$E31
// FUNCTION: STBC 0x004715e0 _$E30
// FUNCTION: STBC 0x004715f0 _$E38
// FUNCTION: STBC 0x00471600 _$E35
// FUNCTION: STBC 0x00471610 _$E37
// FUNCTION: STBC 0x00471620 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00471630 _$E74
// FUNCTION: STBC 0x00471640 _$E73
// FUNCTION: STBC 0x00471650 _$E296
// FUNCTION: STBC 0x00471660 _$E295
// FUNCTION: STBC 0x00471670 _$E85
// FUNCTION: STBC 0x00471680 _$E84
// FUNCTION: STBC 0x00471690 _$E89
// FUNCTION: STBC 0x004716a0 _$E88
// FUNCTION: STBC 0x004716b0 _$E93
// FUNCTION: STBC 0x004716c0 _$E92
// FUNCTION: STBC 0x004716d0 _$E97
// FUNCTION: STBC 0x004716e0 _$E96
// FUNCTION: STBC 0x004716f0 _$E214
// FUNCTION: STBC 0x00471700 _$E213
// FUNCTION: STBC 0x00471710 _$E156
// FUNCTION: STBC 0x00471720 _$E155
// FUNCTION: STBC 0x00471730 _$E169
// FUNCTION: STBC 0x00471740 _$E168
// FUNCTION: STBC 0x00471750 _$E219
// FUNCTION: STBC 0x00471760 _$E218
// FUNCTION: STBC 0x00471770 _$E301
// FUNCTION: STBC 0x00471780 _$E300
// FUNCTION: STBC 0x00471790 _$E223
// FUNCTION: STBC 0x004717a0 _$E222
// FUNCTION: STBC 0x004717b0 _$E305
// FUNCTION: STBC 0x004717c0 _$E304
// FUNCTION: STBC 0x004717d0 _$E312
// FUNCTION: STBC 0x004717e0 _$E311
// FUNCTION: STBC 0x004717f0 _$E322
// FUNCTION: STBC 0x00471800 _$E321
// FUNCTION: STBC 0x00471810 _$E326
// FUNCTION: STBC 0x00471820 _$E325
// FUNCTION: STBC 0x00471830 _$E114
// FUNCTION: STBC 0x00471840 _$E113
// FUNCTION: STBC 0x00471850 _$E262
// FUNCTION: STBC 0x00471860 _$E261
// FUNCTION: STBC 0x00471870 _$E269
// FUNCTION: STBC 0x00471880 _$E268
// FUNCTION: STBC 0x00471890 _$E273
// FUNCTION: STBC 0x004718a0 _$E272
// FUNCTION: STBC 0x004718b0 _$E277
// FUNCTION: STBC 0x004718c0 _$E276
// FUNCTION: STBC 0x004718d0 _$E281
// FUNCTION: STBC 0x004718e0 _$E280
// FUNCTION: STBC 0x004718f0 _$E287
// FUNCTION: STBC 0x00471900 _$E286
// FUNCTION: STBC 0x00471910 _$E330
// FUNCTION: STBC 0x00471920 _$E329
// FUNCTION: STBC 0x00471930 _$E334
// FUNCTION: STBC 0x00471940 _$E333
// FUNCTION: STBC 0x00471950 _$E338
// FUNCTION: STBC 0x00471960 _$E337
// FUNCTION: STBC 0x00471970 _$E342
// FUNCTION: STBC 0x00471980 _$E341
// FUNCTION: STBC 0x00471990 _$E346
// FUNCTION: STBC 0x004719a0 _$E345
// FUNCTION: STBC 0x004719b0 _$E350
// FUNCTION: STBC 0x004719c0 _$E349
// FUNCTION: STBC 0x004719d0 _$E354
// FUNCTION: STBC 0x004719e0 _$E353
// FUNCTION: STBC 0x004719f0 _$E358
// FUNCTION: STBC 0x00471a00 _$E357
// FUNCTION: STBC 0x00471a10 _$E362
// FUNCTION: STBC 0x00471a20 _$E361
// FUNCTION: STBC 0x00471a30 _$E366
// FUNCTION: STBC 0x00471a40 _$E365
// FUNCTION: STBC 0x00471a50 _$E119
// FUNCTION: STBC 0x00471a60 _$E118
// FUNCTION: STBC 0x00471ac0 _$E370
// FUNCTION: STBC 0x00471ad0 _$E369
// FUNCTION: STBC 0x00471af0 _$E124
// FUNCTION: STBC 0x00471b00 _$E123
// FUNCTION: STBC 0x00471b10 _$E374
// FUNCTION: STBC 0x00471b20 _$E373
// FUNCTION: STBC 0x00471b30 _$E378
// FUNCTION: STBC 0x00471b40 _$E377
// FUNCTION: STBC 0x00471b50 _$E382
// FUNCTION: STBC 0x00471b60 _$E381
// FUNCTION: STBC 0x00471b70 _$E386
// FUNCTION: STBC 0x00471b80 _$E385
// FUNCTION: STBC 0x00471b90 _$E133
// FUNCTION: STBC 0x00471ba0 _$E132
// FUNCTION: STBC 0x00471bf0 _$E390
// FUNCTION: STBC 0x00471c00 _$E389
// FUNCTION: STBC 0x00471c10 _$E394
// FUNCTION: STBC 0x00471c20 _$E393
// FUNCTION: STBC 0x00471c30 _$E398
// FUNCTION: STBC 0x00471c40 _$E397
// FUNCTION: STBC 0x00471c50 _$E139
// FUNCTION: STBC 0x00471c60 _$E138
// FUNCTION: STBC 0x00471c70 _$E144
// FUNCTION: STBC 0x00471c80 _$E143
// FUNCTION: STBC 0x00471c90 _$E174
// FUNCTION: STBC 0x00471ca0 _$E173
// FUNCTION: STBC 0x00471d30 _$E402
// FUNCTION: STBC 0x00471d40 _$E401
// FUNCTION: STBC 0x00471d50 _$E179
// FUNCTION: STBC 0x00471d60 _$E178
// FUNCTION: STBC 0x00471d70 _$E406
// FUNCTION: STBC 0x00471d80 _$E405
// FUNCTION: STBC 0x00471d90 _$E410
// FUNCTION: STBC 0x00471da0 _$E409
// FUNCTION: STBC 0x00471db0 _$E184
// FUNCTION: STBC 0x00471dc0 _$E183
// FUNCTION: STBC 0x00471dd0 _$E414
// FUNCTION: STBC 0x00471de0 _$E413
// FUNCTION: STBC 0x00471df0 _$E149
// FUNCTION: STBC 0x00471e00 _$E148
// FUNCTION: STBC 0x00471e10 _$E418
// FUNCTION: STBC 0x00471e20 _$E417
// FUNCTION: STBC 0x00471e30 _$E422
// FUNCTION: STBC 0x00471e40 _$E421
// FUNCTION: STBC 0x00471e50 _$E189
// FUNCTION: STBC 0x00471e60 _$E188
// FUNCTION: STBC 0x00471ea0 _$E194
// FUNCTION: STBC 0x00471eb0 _$E193
// FUNCTION: STBC 0x00471ec0 _$E426
// FUNCTION: STBC 0x00471ed0 _$E425
// FUNCTION: STBC 0x00471ee0 _$E430
// FUNCTION: STBC 0x00471ef0 _$E429
// FUNCTION: STBC 0x00471f00 _$E434
// FUNCTION: STBC 0x00471f10 _$E433
// FUNCTION: STBC 0x00471f20 _$E438
// FUNCTION: STBC 0x00471f30 _$E437
// FUNCTION: STBC 0x00471f40 _$E442
// FUNCTION: STBC 0x00471f50 _$E441
// FUNCTION: STBC 0x00471f60 _$E446
// FUNCTION: STBC 0x00471f70 _$E445
// FUNCTION: STBC 0x00471f80 _$E450
// FUNCTION: STBC 0x00471f90 _$E449
// FUNCTION: STBC 0x00471fa0 _$E454
// FUNCTION: STBC 0x00471fb0 _$E453
// FUNCTION: STBC 0x00471fc0 _$E199
// FUNCTION: STBC 0x00471fd0 _$E198
// FUNCTION: STBC 0x00471fe0 _$E458
// FUNCTION: STBC 0x00471ff0 _$E457
// FUNCTION: STBC 0x00472000 _$E204
// FUNCTION: STBC 0x00472010 _$E203
// FUNCTION: STBC 0x00472020 _$E209
// FUNCTION: STBC 0x00472030 _$E208
// FUNCTION: STBC 0x00472040 _$E462
// FUNCTION: STBC 0x00472050 _$E461
// FUNCTION: STBC 0x00472060 _$E467
// FUNCTION: STBC 0x00472070 _$E466
// FUNCTION: STBC 0x00472080 _$E471
// FUNCTION: STBC 0x00472090 _$E470

// The map's dynamic initialiser and its exit-time destructor.
// FUNCTION: STBC 0x00470430 _$E477
// FUNCTION: STBC 0x00470440 _$E473
// FUNCTION: STBC 0x00470480 _$E476
// FUNCTION: STBC 0x00470490 _$E474
// DATA: STBC 0x009816a0
NiTMap<long, ArtificialIntelligence *> ArtificialIntelligence::aiMap;

// DATA: STBC 0x008db134
long ArtificialIntelligence::nextID = 1;

// DATA: STBC 0x00981724
AICreateFunction *ArtificialIntelligence::createFunctions;

/*
================
ArtificialIntelligence::LogAITree
================
*/
// FUNCTION: STBC 0x00470410
bool ArtificialIntelligence::LogAITree( char *fileName, bool unknown ) {
	return AILog::Open( fileName, unknown );
}

/*
================
ArtificialIntelligence::ArtificialIntelligence

The AI takes the next ID and joins the AI map under it.
================
*/
// FUNCTION: STBC 0x00470520 ??0ArtificialIntelligence@@QAE@PAVPhysicsObjectClass@@PAD@Z
ArtificialIntelligence::ArtificialIntelligence( PhysicsObjectClass *object, char *name_ ) : objectID( 0 ), parentID( 0 ), name( NULL ), active( false ), focus( false ), paused( false ), interruptable( true ) {
	if ( object != NULL ) {
		objectID = object->GetObjID();
	}
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
	id = nextID++;
	aiMap.SetAt( id, this );
}

/*
================
ArtificialIntelligence::IsInterruptable
================
*/
// FUNCTION: STBC 0x004706c0
bool ArtificialIntelligence::IsInterruptable( void ) {
	return interruptable;
}

/*
================
ArtificialIntelligence::Reset
================
*/
// FUNCTION: STBC 0x004706d0
void ArtificialIntelligence::Reset( void ) {
}

/*
================
ArtificialIntelligence::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x004706e0
void ArtificialIntelligence::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais ) {
}

/*
================
ArtificialIntelligence::GetFocusAIs
================
*/
// FUNCTION: STBC 0x004706f0
void ArtificialIntelligence::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais ) {
}

/*
================
ArtificialIntelligence::SetActive
================
*/
// FUNCTION: STBC 0x00470700
void ArtificialIntelligence::SetActive( void ) {
	active = true;
}

/*
================
ArtificialIntelligence::SetInactive
================
*/
// FUNCTION: STBC 0x00470710
void ArtificialIntelligence::SetInactive( void ) {
	active = false;
}

/*
================
ArtificialIntelligence::GotFocus
================
*/
// FUNCTION: STBC 0x00470720
void ArtificialIntelligence::GotFocus( void ) {
	focus = true;
}

/*
================
ArtificialIntelligence::LostFocus
================
*/
// FUNCTION: STBC 0x00470730
void ArtificialIntelligence::LostFocus( void ) {
	focus = false;
}

/*
================
ArtificialIntelligence::Update
================
*/
// FUNCTION: STBC 0x00470740
int ArtificialIntelligence::Update( double time ) {
	return US_INVALID;
}

/*
================
ArtificialIntelligence::VirtualSlot13
================
*/
// FUNCTION: STBC 0x00470750
ArtificialIntelligence *ArtificialIntelligence::VirtualSlot13( void ) {
	return this;
}

/*
================
ArtificialIntelligence::IsDormant
================
*/
// FUNCTION: STBC 0x00470760
bool ArtificialIntelligence::IsDormant( void ) {
	return false;
}

/*
================
ArtificialIntelligence::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00470770
PyObject *ArtificialIntelligence::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "ArtificialIntelligence" );
}

/*
================
ArtificialIntelligence::FixUpPointers
================
*/
// FUNCTION: STBC 0x00470780
bool ArtificialIntelligence::FixUpPointers( void ) {
	return true;
}

/*
================
ArtificialIntelligence::PostFixUp
================
*/
// FUNCTION: STBC 0x00470790
void ArtificialIntelligence::PostFixUp( void ) {
}

/*
================
ArtificialIntelligence::VirtualSlot20
================
*/
// FUNCTION: STBC 0x004707a0
bool ArtificialIntelligence::VirtualSlot20( int ) {
	return false;
}

/*
================
ArtificialIntelligence::~ArtificialIntelligence

Leaves the AI map and releases the external functions.
================
*/
// FUNCTION: STBC 0x004707d0
// FUNCTION: STBC 0x004707b0 ??_GArtificialIntelligence@@UAEPAXI@Z
ArtificialIntelligence::~ArtificialIntelligence( void ) {
	aiMap.RemoveAt( id );
	objectID = 0;
	delete [] name;
	name = NULL;

	NiTMapIterator pos = externalFunctions.GetFirstPos();
	while ( pos != NULL ) {
		const char *functionName;
		PyObject *function;
		externalFunctions.GetNext( pos, functionName, function );
		Py_DECREF( function );
	}
	externalFunctions.RemoveAll();
}

/*
================
ArtificialIntelligence::GetObject
================
*/
// FUNCTION: STBC 0x00470a30
PhysicsObjectClass *ArtificialIntelligence::GetObject( void ) {
	return PhysicsObjectClass::GetObjectByID( NULL, objectID );
}

/*
================
ArtificialIntelligence::GetShip
================
*/
// FUNCTION: STBC 0x00470a40
ShipClass *ArtificialIntelligence::GetShip( void ) {
	return ShipClass::GetObjectByID( NULL, objectID );
}

/*
================
ArtificialIntelligence::GetAIByID
================
*/
// FUNCTION: STBC 0x00470a50
ArtificialIntelligence *ArtificialIntelligence::GetAIByID( long id ) {
	ArtificialIntelligence *ai = NULL;
	aiMap.GetAt( id, ai );
	return ai;
}

/*
================
ArtificialIntelligence::RegisterExternalFunction
================
*/
// FUNCTION: STBC 0x00470aa0
void ArtificialIntelligence::RegisterExternalFunction( char *functionName, PyObject *function ) {
	Py_INCREF( function );
	externalFunctions.SetAt( functionName, function );
}

/*
================
ArtificialIntelligence_RegisterExternalFunction

The Python side of RegisterExternalFunction: the AI's shadow instance,
the function's name and the function.
================
*/
// FUNCTION: STBC 0x00470b40
PyObject *ArtificialIntelligence_RegisterExternalFunction( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	PyObject *function = NULL;
	char *functionName;
	// PyArg_ParseTuple returns 0 on failure, never -1: a bad call goes on
	// with whatever the arguments held.
	if ( PyArg_ParseTuple( args, "OsO", &instance, &functionName, &function ) != -1 ) {
		ArtificialIntelligence *ai = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "ArtificialIntelligence" );
		if ( ai != NULL ) {
			ai->RegisterExternalFunction( functionName, function );
			Py_INCREF( Py_None );
			return Py_None;
		}
	}
	return NULL;
}

/*
================
ArtificialIntelligence::UnregisterExternalFunction

The reference the AI held is not released.
================
*/
// FUNCTION: STBC 0x00470bd0
void ArtificialIntelligence::UnregisterExternalFunction( char *functionName, PyObject *function ) {
	PyObject *registered;
	if ( externalFunctions.GetAt( functionName, registered ) ) {
		externalFunctions.RemoveAt( functionName );
	}
}

/*
================
ArtificialIntelligence_UnregisterExternalFunction
================
*/
// FUNCTION: STBC 0x00470ca0
PyObject *ArtificialIntelligence_UnregisterExternalFunction( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	PyObject *function = NULL;
	char *functionName;
	if ( PyArg_ParseTuple( args, "OsO", &instance, &functionName, &function ) != -1 ) {
		ArtificialIntelligence *ai = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "ArtificialIntelligence" );
		if ( ai != NULL ) {
			ai->UnregisterExternalFunction( functionName, function );
			Py_INCREF( Py_None );
			return Py_None;
		}
	}
	return NULL;
}

/*
================
ArtificialIntelligence::CallExternalFunction

This AI has no external functions to call.
================
*/
// FUNCTION: STBC 0x00470d30
bool ArtificialIntelligence::CallExternalFunction( char *functionName, PyObject *arguments ) {
	return true;
}

/*
================
ArtificialIntelligence_CallExternalFunction

Returns the call's result as an int.
================
*/
// FUNCTION: STBC 0x00470d40
PyObject *ArtificialIntelligence_CallExternalFunction( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	PyObject *arguments = NULL;
	char *functionName;
	if ( PyArg_ParseTuple( args, "OsO", &instance, &functionName, &arguments ) != -1 ) {
		ArtificialIntelligence *ai = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "ArtificialIntelligence" );
		if ( ai != NULL ) {
			return PyInt_FromLong( ai->CallExternalFunction( functionName, arguments ) );
		}
	}
	return NULL;
}

/*
================
ArtificialIntelligence_GetAIByID

The AI's shadow instance, or None.
================
*/
// FUNCTION: STBC 0x00470dd0
extern "C" PyObject *_wrap_ArtificialIntelligence_GetAIByID( PyObject *self, PyObject *args ) {
	int id;
	if ( PyArg_ParseTuple( args, "i", &id ) != -1 ) {
		ArtificialIntelligence *ai = ArtificialIntelligence::GetAIByID( id );
		if ( ai != NULL ) {
			return ai->VirtualSlot15();
		}
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
ArtificialIntelligence_GetAllAIsInTree

A tuple of the shadow instances of the AIs in the tree.
================
*/
// FUNCTION: STBC 0x00470e20
PyObject *ArtificialIntelligence_GetAllAIsInTree( PyObject *self, PyObject *args ) {
	PyObject *tuple = NULL;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		ArtificialIntelligence *ai = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "ArtificialIntelligence" );
		if ( ai != NULL ) {
			NiTList<ArtificialIntelligence *> ais( 2 );
			ai->GetAllAIsInTree( ais );
			tuple = PyTuple_New( ais.GetSize() );
			int i = 0;
			while ( !ais.IsEmpty() ) {
				PyTuple_SetItem( tuple, i++, ais.RemoveHead()->VirtualSlot15() );
			}
		}
	}
	return tuple;
}

/*
================
ArtificialIntelligence_GetFocusAIs

A tuple of the shadow instances of the AIs in the tree that have the
focus.
================
*/
// FUNCTION: STBC 0x00470f70
PyObject *ArtificialIntelligence_GetFocusAIs( PyObject *self, PyObject *args ) {
	PyObject *tuple = NULL;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		ArtificialIntelligence *ai = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "ArtificialIntelligence" );
		if ( ai != NULL ) {
			NiTList<ArtificialIntelligence *> ais( 2 );
			ai->GetFocusAIs( ais );
			tuple = PyTuple_New( ais.GetSize() );
			int i = 0;
			while ( !ais.IsEmpty() ) {
				PyTuple_SetItem( tuple, i++, ais.RemoveHead()->VirtualSlot15() );
			}
		}
	}
	return tuple;
}

/*
================
ArtificialIntelligence::WriteToStream

Each external function goes as its name and its pickle, after a true;
a false ends them. The ID counter is saved with every AI.
================
*/
// FUNCTION: STBC 0x004710c0
bool ArtificialIntelligence::WriteToStream( TGStream *stream ) {
	stream->WriteLong( GetAIType() );
	stream->WriteID( objectID );
	stream->WriteLong( parentID );
	stream->VirtualSlot11( name );

	NiTMapIterator pos = externalFunctions.GetFirstPos();
	while ( pos != NULL ) {
		const char *functionName;
		PyObject *function;
		externalFunctions.GetNext( pos, functionName, function );
		stream->WriteBool( true );
		stream->VirtualSlot11( (char *)functionName );
		if ( !PyEmbed::SerializePython( function, stream, 0 ) ) {
			return false;
		}
	}
	stream->WriteBool( false );

	stream->WriteBool( active );
	stream->WriteBool( focus );
	stream->WriteBool( paused );
	stream->WriteBool( interruptable );
	stream->WriteLong( id );
	stream->WriteLong( nextID );
	return true;
}

/*
================
ArtificialIntelligence::ArtificialIntelligence

The AI joins the AI map under the ID it was saved with, and the ID
counter goes back to its saved value, which must be past the ID.
================
*/
// FUNCTION: STBC 0x004711f0 ??0ArtificialIntelligence@@QAE@PAVTGStream@@PA_N@Z
ArtificialIntelligence::ArtificialIntelligence( TGStream *stream, bool *ok ) {
	objectID = stream->ReadID();
	parentID = stream->ReadLong();
	name = stream->VirtualSlot10( -1 );

	while ( stream->ReadBool() ) {
		char *functionName = stream->VirtualSlot10( -1 );
		PyObject *function = PyEmbed::UnserializePython( stream, 0 );
		if ( function == NULL ) {
			*ok = false;
			return;
		}
		externalFunctions.SetAt( functionName, function );
		delete [] functionName;
	}

	active = stream->ReadBool();
	focus = stream->ReadBool();
	paused = stream->ReadBool();
	interruptable = stream->ReadBool();
	id = stream->ReadLong();
	aiMap.SetAt( id, this );
	nextID = stream->ReadLong();
	if ( nextID <= id ) {
		*ok = false;
	}
}

/*
================
ArtificialIntelligence::CreateFromStream
================
*/
// FUNCTION: STBC 0x00471470
ArtificialIntelligence *ArtificialIntelligence::CreateFromStream( TGStream *stream ) {
	ArtificialIntelligence *ai = NULL;
	long type = stream->ReadLong();
	for ( AICreateFunction *entry = createFunctions; entry != NULL; entry = entry->next ) {
		if ( entry->type == type ) {
			if ( entry->create != NULL ) {
				ai = entry->create( stream );
			}
			break;
		}
	}
	return ai;
}

/*
================
ArtificialIntelligence_SaveInstance
================
*/
// FUNCTION: STBC 0x004714b0
bool ArtificialIntelligence_SaveInstance( PyObject *instance, PyObject **state ) {
	PyObject *dictionary;
	if ( PyEmbed::GetAttribute( instance, "__dict__", "O", &dictionary ) == -1 ) {
		return false;
	}
	ArtificialIntelligence *ai = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&ai, "ArtificialIntelligence" );
	if ( ai == NULL ) {
		return false;
	}
	PyObject *id = PyInt_FromLong( ai->GetID() );
	PyDict_SetItemString( dictionary, "_AIID_", id );
	Py_DECREF( id );
	return true;
}

/*
================
ArtificialIntelligence_LoadInstance
================
*/
// FUNCTION: STBC 0x00471540
bool ArtificialIntelligence_LoadInstance( PyObject *instance, PyObject *state ) {
	PyObject *id = PyDict_GetItemString( state, "_AIID_" );
	if ( id != NULL ) {
		ArtificialIntelligence *ai = ArtificialIntelligence::GetAIByID( PyInt_AsLong( id ) );
		if ( ai != NULL ) {
			PyObject *pointer = PyDict_GetItemString( state, "this" );
			if ( pointer != NULL ) {
				PyDict_SetItemString( state, "this", PyEmbed::Unknown6F9A90( ai, pointer ) );
				PyObject_SetAttrString( instance, "__dict__", state );
				return true;
			}
		}
	}
	return false;
}

// The object types of the torpedoes-ready condition and its event handler.
// DATA: STBC 0x0088bb38
extern const int CT_OPT_TORPS_READY = 0x803B;
// DATA: STBC 0x0088bb3c
extern const int CT_OPT_TR_EVENT_HANDLER = 0x803C;
