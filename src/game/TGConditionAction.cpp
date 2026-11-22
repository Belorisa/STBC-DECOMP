/*
===============================================================================

	TGConditionAction.cpp

===============================================================================
*/

#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGConditionAction.h"
#include "TGCondition.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include <iostream>

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).
// FUNCTION: STBC 0x00704a50 _$E59
// FUNCTION: STBC 0x00704a60 _$E56
// FUNCTION: STBC 0x00704a70 _$E58
// FUNCTION: STBC 0x00704a80 _$E57
// FUNCTION: STBC 0x00704a90 _$E65
// FUNCTION: STBC 0x00704aa0 _$E62
// FUNCTION: STBC 0x00704ab0 _$E64
// FUNCTION: STBC 0x00704ac0 _$E63
// FUNCTION: STBC 0x00704f80 _$E72
// FUNCTION: STBC 0x00704fa0 _$E71

/*
================
TGConditionAction::TGConditionAction
================
*/
// FUNCTION: STBC 0x00704ad0
TGConditionAction::TGConditionAction( void ) {
	playing = false;
	numTableConditions = 0;
	table = NULL;
}

/*
================
TGConditionAction::~TGConditionAction
================
*/
// FUNCTION: STBC 0x00704b30
TGConditionAction::~TGConditionAction( void ) {
	if ( table != NULL ) {
		delete [] table;
	}
}

/*
================
TGConditionAction::SetEvaluationFunction

The table is made for the conditions held now. A call that fails, or an
answer out of range, is printed (the print is compiled out, the string
of the arguments is still made) and counts as TGCA_WAIT.
================
*/
// FUNCTION: STBC 0x00704bb0
void TGConditionAction::SetEvaluationFunction( PyObject *function ) {
	if ( table != NULL ) {
		delete [] table;
	}
	numTableConditions = numConditions;
	table = new char[1 << numTableConditions];
	if ( table == NULL ) {
		numTableConditions = 0;
		return;
	}

	PyObject *statuses = PyTuple_New( numTableConditions );
	for ( int combination = ( 1 << numTableConditions ) - 1; combination >= 0; combination-- ) {
		// Condition i's status is bit i of the combination.
		int bit = 1;
		for ( int i = 0; i < numTableConditions; i++, bit <<= 1 ) {
			PyTuple_SetItem( statuses, i, PyInt_FromLong( ( combination & bit ) != 0 ) );
		}
		// The call releases the reference it is given.
		Py_INCREF( statuses );
		int result = TGCA_WAIT;
		if ( PyEmbed::CallObject( function, "i", &result, statuses ) == -1 ) {
			PyObject *text = PyObject_Str( statuses );
			Py_DECREF( text );
			result = TGCA_WAIT;
			PyErr_Clear();
		}
		if ( result != TGCA_WAIT && result != TGCA_COMPLETED ) {
			PyObject *text = PyObject_Str( statuses );
			Py_DECREF( text );
			result = TGCA_WAIT;
		}
		table[combination] = result;
	}
	Py_DECREF( statuses );
}

/*
================
TGConditionAction_SetEvaluationFunction

PyArg_ParseTuple's failure (0) is not caught: the check is for -1.
================
*/
// FUNCTION: STBC 0x00704d10
PyObject *TGConditionAction_SetEvaluationFunction( PyObject *self, PyObject *args ) {
	PyObject *instance;
	PyObject *function = NULL;
	if ( PyArg_ParseTuple( args, "OO", &instance, &function ) != -1 ) {
		TGConditionAction *action;
		PyEmbed::Unknown6F98E0( instance, (void **)&action, "TGConditionAction" );
		action->SetEvaluationFunction( function );
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
TGConditionAction::Play
================
*/
// FUNCTION: STBC 0x00704d80
void TGConditionAction::Play( void ) {
	playing = true;
	CheckConditions();
}

/*
================
TGConditionAction::ConditionChanged
================
*/
// FUNCTION: STBC 0x00704d90
void TGConditionAction::ConditionChanged( TGCondition *condition ) {
	CheckConditions();
}

/*
================
TGConditionAction::CheckConditions

With a table, the conditions must still be the ones it was made for.
================
*/
// FUNCTION: STBC 0x00704da0
void TGConditionAction::CheckConditions( void ) {
	if ( !playing ) {
		return;
	}
	if ( numTableConditions != 0 && table != NULL ) {
		if ( numConditions != numTableConditions ) {
			return;
		}
		int combination = 0;
		for ( int i = 0; i < numConditions; i++ ) {
			combination |= conditions[i]->GetStatus() << i;
		}
		if ( table[combination] == TGCA_COMPLETED ) {
			Completed();
		}
	} else {
		for ( int i = 0; i < numConditions; i++ ) {
			if ( !conditions[i]->GetStatus() ) {
				return;
			}
		}
		Completed();
	}
}

/*
================
TGConditionAction::WriteToStream
================
*/
// FUNCTION: STBC 0x00704e30 ?WriteToStream@TGConditionAction@@UAE_NPAVTGStream@@@Z
bool TGConditionAction::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}
	if ( !TGConditionHandler::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( playing );
	stream->WriteInt( numTableConditions );
	if ( table != NULL ) {
		stream->WriteBool( true );
		stream->Write( table, 1 << numTableConditions );
	} else {
		stream->WriteBool( false );
	}
	return true;
}

/*
================
TGConditionAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x00704eb0 ?ReadFromStream@TGConditionAction@@UAE_NPAVTGStream@@@Z
bool TGConditionAction::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}
	if ( !TGConditionHandler::ReadFromStream( stream ) ) {
		return false;
	}
	playing = stream->ReadBool();
	numTableConditions = stream->ReadInt();
	table = NULL;
	if ( stream->ReadBool() ) {
		table = new char[1 << numTableConditions];
		if ( table == NULL ) {
			return false;
		}
		stream->Read( table, 1 << numTableConditions );
	}
	return true;
}

/*
================
TGConditionAction::FixUpPointers
================
*/
// FUNCTION: STBC 0x00704f40 ?FixUpPointers@TGConditionAction@@UAE_NXZ
bool TGConditionAction::FixUpPointers( void ) {
	if ( !TGAction::FixUpPointers() ) {
		return false;
	}
	return TGConditionHandler::FixUpPointers() ? true : false;
}

/*
================
TGConditionAction::PostFixUp
================
*/
// FUNCTION: STBC 0x00704f60 ?PostFixUp@TGConditionAction@@UAEXXZ
void TGConditionAction::PostFixUp( void ) {
	TGAction::PostFixUp();
	TGConditionHandler::PostFixUp();
}
