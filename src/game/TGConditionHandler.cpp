/*
===============================================================================

	TGConditionHandler.cpp

===============================================================================
*/

#include "TGCondition.h"
#include "TGConditionHandler.h"
#include "TGStream.h"
#include <iostream>

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).
// FUNCTION: STBC 0x00701500 _$E39
// FUNCTION: STBC 0x00701510 _$E36
// FUNCTION: STBC 0x00701520 _$E38
// FUNCTION: STBC 0x00701530 _$E37
// FUNCTION: STBC 0x00701540 _$E45
// FUNCTION: STBC 0x00701550 _$E42
// FUNCTION: STBC 0x00701560 _$E44
// FUNCTION: STBC 0x00701570 _$E43
// FUNCTION: STBC 0x007018b0 _$E52
// FUNCTION: STBC 0x007018d0 _$E51

/*
================
TGConditionHandler::TGConditionHandler
================
*/
// FUNCTION: STBC 0x00701580
TGConditionHandler::TGConditionHandler( void ) {
	numConditions = 0;
	maxConditions = 1;
	conditions = new TGCondition *[maxConditions];
}

/*
================
TGConditionHandler::~TGConditionHandler

Leaves every condition's handlers; the conditions this was the last
handler of are deleted.
================
*/
// FUNCTION: STBC 0x007015e0
// FUNCTION: STBC 0x007015c0 ??_GTGConditionHandler@@UAEPAXI@Z
TGConditionHandler::~TGConditionHandler( void ) {
	if ( numConditions != 0 ) {
		for ( int i = 0; i < numConditions; i++ ) {
			conditions[i]->RemoveHandler( this );
		}
	}
	delete [] conditions;
}

/*
================
TGConditionHandler::AddCondition
================
*/
// FUNCTION: STBC 0x00701620
void TGConditionHandler::AddCondition( TGCondition *condition ) {
	if ( numConditions + 1 > maxConditions ) {
		TGCondition **newConditions = new TGCondition *[maxConditions + 2];
		for ( int i = 0; i < numConditions; i++ ) {
			newConditions[i] = conditions[i];
		}
		delete [] conditions;
		conditions = newConditions;
		maxConditions += 2;
	}
	conditions[numConditions] = condition;
	numConditions++;
	condition->AddHandler( this );
}

/*
================
TGConditionHandler::RemoveCondition

The entry after a removed one is not checked: the loop moves on past it.
================
*/
// FUNCTION: STBC 0x007016a0
void TGConditionHandler::RemoveCondition( TGCondition *condition ) {
	for ( int i = 0; i < numConditions; i++ ) {
		if ( conditions[i] == condition ) {
			for ( int j = i + 1; j < numConditions; j++ ) {
				conditions[j - 1] = conditions[j];
			}
			numConditions--;
			condition->RemoveHandler( this );
		}
	}
}

/*
================
TGConditionHandler::WriteToStream
================
*/
// FUNCTION: STBC 0x00701700
bool TGConditionHandler::WriteToStream( TGStream *stream ) {
	stream->WriteInt( numConditions );
	for ( int i = 0; i < numConditions; i++ ) {
		stream->WriteID( conditions[i]->GetObjID() );
	}
	return true;
}

/*
================
TGConditionHandler::ReadFromStream

The conditions held before are removed; the array then holds the IDs read
until PostFixUp.
================
*/
// FUNCTION: STBC 0x00701750
bool TGConditionHandler::ReadFromStream( TGStream *stream ) {
	while ( numConditions > 0 ) {
		RemoveCondition( conditions[0] );
	}
	numConditions = stream->ReadInt();
	if ( maxConditions < numConditions ) {
		delete [] conditions;
		maxConditions = numConditions;
		conditions = new TGCondition *[maxConditions];
	}
	for ( int i = 0; i < numConditions; i++ ) {
		conditions[i] = (TGCondition *)stream->ReadID();
	}
	return true;
}

/*
================
TGConditionHandler::FixUpPointers
================
*/
// FUNCTION: STBC 0x007017e0
bool TGConditionHandler::FixUpPointers( void ) {
	for ( int i = 0; i < numConditions; i++ ) {
		if ( TGObject::GetTGObjectPtr( (long)conditions[i] ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
TGConditionHandler::PostFixUp

The IDs are taken out, and each condition still loaded is added again.
================
*/
// FUNCTION: STBC 0x00701820
void TGConditionHandler::PostFixUp( void ) {
	int count = numConditions;
	long *ids = new long[count];
	int i;
	for ( i = 0; i < count; i++ ) {
		ids[i] = (long)conditions[i];
	}
	numConditions = 0;
	for ( i = 0; i < count; i++ ) {
		TGObject *object = TGObject::GetTGObjectPtr( ids[i] );
		if ( object != NULL && object->IsTypeOf( TGCONDITION_TYPE ) ) {
			AddCondition( (TGCondition *)object );
		}
	}
	delete [] ids;
}
