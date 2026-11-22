/*
===============================================================================

	TGCondition.cpp

===============================================================================
*/

#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGCondition.h"
#include "TGConditionHandler.h"
#include "TGStream.h"
#include "NiTList.h"
#include <iostream>

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).
// FUNCTION: STBC 0x00700f70 _$E39
// FUNCTION: STBC 0x00700f80 _$E36
// FUNCTION: STBC 0x00700f90 _$E38
// FUNCTION: STBC 0x00700fa0 _$E37
// FUNCTION: STBC 0x00700fb0 _$E45
// FUNCTION: STBC 0x00700fc0 _$E42
// FUNCTION: STBC 0x00700fd0 _$E44
// FUNCTION: STBC 0x00700fe0 _$E43
// FUNCTION: STBC 0x007014d0 _$E52
// FUNCTION: STBC 0x007014f0 _$E51

/*
================
TGCondition::Cast
================
*/
// FUNCTION: STBC 0x00700ff0
TGCondition *TGCondition::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGCONDITION_TYPE ) ) {
		return (TGCondition *)object;
	}
	return NULL;
}

/*
================
TGCondition::TGCondition
================
*/
// FUNCTION: STBC 0x00701020
TGCondition::TGCondition( long id ) : TGObject( id ) {
	status = false;
	active = true;
	numHandlers = 0;
	maxHandlers = 1;
	handlers = new TGConditionHandler *[maxHandlers];
}

/*
================
TGCondition::GetObjType
================
*/
// FUNCTION: STBC 0x00701090
int TGCondition::GetObjType( void ) {
	return TGCONDITION_TYPE;
}

/*
================
TGCondition::GetClassName
================
*/
// FUNCTION: STBC 0x007010c0
char *TGCondition::GetClassName( void ) {
	return "TGCondition";
}

/*
================
TGCondition::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x007010d0
char *TGCondition::GetSwigTypeName( void ) {
	return "_p_TGCondition";
}

/*
================
TGCondition::GetPythonClassName
================
*/
// FUNCTION: STBC 0x007010e0
char *TGCondition::GetPythonClassName( void ) {
	return "TGConditionPtr";
}

/*
================
TGCondition::~TGCondition

The handlers are not told: a condition goes when its last handler leaves.
================
*/
// FUNCTION: STBC 0x00701120
// FUNCTION: STBC 0x007010f0 ??_GTGCondition@@UAEPAXI@Z
TGCondition::~TGCondition( void ) {
	delete [] handlers;
}

/*
================
TGCondition::SetStatus
================
*/
// FUNCTION: STBC 0x00701140
void TGCondition::SetStatus( bool status_ ) {
	if ( status != status_ ) {
		status = status_;
		for ( int i = 0; i < numHandlers; i++ ) {
			handlers[i]->ConditionChanged( this );
		}
	}
}

/*
================
TGCondition::AddHandler
================
*/
// FUNCTION: STBC 0x00701180
void TGCondition::AddHandler( TGConditionHandler *handler ) {
	if ( numHandlers + 1 > maxHandlers ) {
		TGConditionHandler **newHandlers = new TGConditionHandler *[maxHandlers * 2];
		for ( int i = 0; i < numHandlers; i++ ) {
			newHandlers[i] = handlers[i];
		}
		delete [] handlers;
		handlers = newHandlers;
		maxHandlers *= 2;
	}
	handlers[numHandlers] = handler;
	numHandlers++;
}

/*
================
TGCondition::RemoveHandler

The entry after a removed one is not checked: the loop moves on past it.
================
*/
// FUNCTION: STBC 0x007011f0
void TGCondition::RemoveHandler( TGConditionHandler *handler ) {
	for ( int i = 0; i < numHandlers; i++ ) {
		if ( handlers[i] == handler ) {
			for ( int j = i + 1; j < numHandlers; j++ ) {
				handlers[j - 1] = handlers[j];
			}
			numHandlers--;
		}
	}
	if ( numHandlers <= 0 ) {
		delete this;
	}
}

/*
================
TGCondition::WriteToStream
================
*/
// FUNCTION: STBC 0x00701260
bool TGCondition::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( status );
	stream->WriteBool( active );
	return true;
}

/*
================
TGCondition::ReadFromStream

The handlers add themselves again when they are fixed up.
================
*/
// FUNCTION: STBC 0x007012a0
bool TGCondition::ReadFromStream( TGStream *stream ) {
	status = stream->ReadBool();
	active = stream->ReadBool();
	numHandlers = 0;
	return true;
}

/*
================
TGCondition::SaveConditions

The conditions are gathered from the object table first, for the count.
================
*/
// FUNCTION: STBC 0x007012d0
bool TGCondition::SaveConditions( TGStream *stream ) {
	bool saved = true;
	NiTList<TGObject *> conditions( LOAD_LIST_BLOCK );

	NiTMap<long, TGObject *> *objects = objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		if ( object->IsTypeOf( TGCONDITION_TYPE ) ) {
			conditions.AddTail( object );
		}
	}

	stream->WriteInt( conditions.GetSize() );
	while ( !conditions.IsEmpty() && saved ) {
		saved &= conditions.RemoveHead()->WriteToStream( stream );
	}
	return saved;
}

/*
================
TGCondition::LoadConditions

Reads what SaveConditions wrote; once one fails, the rest are skipped.
================
*/
// FUNCTION: STBC 0x00701490
bool TGCondition::LoadConditions( TGStream *stream ) {
	bool loaded = true;
	int count = stream->ReadInt();
	for ( int i = count - 1; i >= 0 && loaded; i-- ) {
		if ( LoadObject( stream, 0 ) == NULL ) {
			loaded = false;
		}
	}
	return loaded;
}
