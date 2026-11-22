/*
===============================================================================

	ConditionalAI.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>

#include "AILog.h"
#include "ConditionalAI.h"
#include "ConditionScript.h"
#include "PyEmbed.h"
#include "TGCondition.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0047ac50 _$E460
// FUNCTION: STBC 0x0047ac70 _$E459

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0047a200 _$E32
// FUNCTION: STBC 0x0047a210 _$E29
// FUNCTION: STBC 0x0047a220 _$E31
// FUNCTION: STBC 0x0047a230 _$E30
// FUNCTION: STBC 0x0047a240 _$E38
// FUNCTION: STBC 0x0047a250 _$E35
// FUNCTION: STBC 0x0047a260 _$E37
// FUNCTION: STBC 0x0047a270 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0047a280 _$E81
// FUNCTION: STBC 0x0047a290 _$E80
// FUNCTION: STBC 0x0047a2a0 _$E85
// FUNCTION: STBC 0x0047a2b0 _$E84
// FUNCTION: STBC 0x0047a2c0 _$E96
// FUNCTION: STBC 0x0047a2d0 _$E95
// FUNCTION: STBC 0x0047a2e0 _$E100
// FUNCTION: STBC 0x0047a2f0 _$E99
// FUNCTION: STBC 0x0047a300 _$E104
// FUNCTION: STBC 0x0047a310 _$E103
// FUNCTION: STBC 0x0047a320 _$E108
// FUNCTION: STBC 0x0047a330 _$E107
// FUNCTION: STBC 0x0047a340 _$E232
// FUNCTION: STBC 0x0047a350 _$E231
// FUNCTION: STBC 0x0047a360 _$E174
// FUNCTION: STBC 0x0047a370 _$E173
// FUNCTION: STBC 0x0047a380 _$E187
// FUNCTION: STBC 0x0047a390 _$E186
// FUNCTION: STBC 0x0047a3a0 _$E237
// FUNCTION: STBC 0x0047a3b0 _$E236
// FUNCTION: STBC 0x0047a3c0 _$E242
// FUNCTION: STBC 0x0047a3d0 _$E241
// FUNCTION: STBC 0x0047a3e0 _$E246
// FUNCTION: STBC 0x0047a3f0 _$E245
// FUNCTION: STBC 0x0047a400 _$E250
// FUNCTION: STBC 0x0047a410 _$E249
// FUNCTION: STBC 0x0047a420 _$E257
// FUNCTION: STBC 0x0047a430 _$E256
// FUNCTION: STBC 0x0047a440 _$E282
// FUNCTION: STBC 0x0047a450 _$E281
// FUNCTION: STBC 0x0047a460 _$E286
// FUNCTION: STBC 0x0047a470 _$E285
// FUNCTION: STBC 0x0047a480 _$E132
// FUNCTION: STBC 0x0047a490 _$E131
// FUNCTION: STBC 0x0047a4a0 _$E268
// FUNCTION: STBC 0x0047a4b0 _$E267
// FUNCTION: STBC 0x0047a510 _$E275
// FUNCTION: STBC 0x0047a520 _$E274
// FUNCTION: STBC 0x0047a6d0 _$E298
// FUNCTION: STBC 0x0047a6e0 _$E297
// FUNCTION: STBC 0x0047a6f0 _$E302
// FUNCTION: STBC 0x0047a700 _$E301
// FUNCTION: STBC 0x0047a710 _$E342
// FUNCTION: STBC 0x0047a720 _$E341
// FUNCTION: STBC 0x0047a730 _$E348
// FUNCTION: STBC 0x0047a740 _$E347
// FUNCTION: STBC 0x0047a750 _$E290
// FUNCTION: STBC 0x0047a760 _$E289
// FUNCTION: STBC 0x0047a770 _$E294
// FUNCTION: STBC 0x0047a780 _$E293
// FUNCTION: STBC 0x0047a790 _$E306
// FUNCTION: STBC 0x0047a7a0 _$E305
// FUNCTION: STBC 0x0047a7b0 _$E310
// FUNCTION: STBC 0x0047a7c0 _$E309
// FUNCTION: STBC 0x0047a7d0 _$E314
// FUNCTION: STBC 0x0047a7e0 _$E313
// FUNCTION: STBC 0x0047a7f0 _$E318
// FUNCTION: STBC 0x0047a800 _$E317
// FUNCTION: STBC 0x0047a810 _$E322
// FUNCTION: STBC 0x0047a820 _$E321
// FUNCTION: STBC 0x0047a830 _$E326
// FUNCTION: STBC 0x0047a840 _$E325
// FUNCTION: STBC 0x0047a850 _$E330
// FUNCTION: STBC 0x0047a860 _$E329
// FUNCTION: STBC 0x0047a910 _$E334
// FUNCTION: STBC 0x0047a920 _$E333
// FUNCTION: STBC 0x0047a930 _$E137
// FUNCTION: STBC 0x0047a940 _$E136
// FUNCTION: STBC 0x0047a950 _$E338
// FUNCTION: STBC 0x0047a960 _$E337
// FUNCTION: STBC 0x0047a970 _$E142
// FUNCTION: STBC 0x0047a980 _$E141
// FUNCTION: STBC 0x0047a990 _$E352
// FUNCTION: STBC 0x0047a9a0 _$E351
// FUNCTION: STBC 0x0047a9b0 _$E356
// FUNCTION: STBC 0x0047a9c0 _$E355
// FUNCTION: STBC 0x0047a9d0 _$E360
// FUNCTION: STBC 0x0047a9e0 _$E359
// FUNCTION: STBC 0x0047a9f0 _$E364
// FUNCTION: STBC 0x0047aa00 _$E363
// FUNCTION: STBC 0x0047aa10 _$E151
// FUNCTION: STBC 0x0047aa20 _$E150
// FUNCTION: STBC 0x0047aa30 _$E368
// FUNCTION: STBC 0x0047aa40 _$E367
// FUNCTION: STBC 0x0047aa50 _$E372
// FUNCTION: STBC 0x0047aa60 _$E371
// FUNCTION: STBC 0x0047aa70 _$E376
// FUNCTION: STBC 0x0047aa80 _$E375
// FUNCTION: STBC 0x0047aa90 _$E157
// FUNCTION: STBC 0x0047aaa0 _$E156
// FUNCTION: STBC 0x0047aab0 _$E162
// FUNCTION: STBC 0x0047aac0 _$E161
// FUNCTION: STBC 0x0047aad0 _$E192
// FUNCTION: STBC 0x0047aae0 _$E191
// FUNCTION: STBC 0x0047acb0 _$E380
// FUNCTION: STBC 0x0047acc0 _$E379
// FUNCTION: STBC 0x0047acd0 _$E197
// FUNCTION: STBC 0x0047ace0 _$E196
// FUNCTION: STBC 0x0047ad30 _$E384
// FUNCTION: STBC 0x0047ad40 _$E383
// FUNCTION: STBC 0x0047ad50 _$E388
// FUNCTION: STBC 0x0047ad60 _$E387
// FUNCTION: STBC 0x0047ad70 _$E202
// FUNCTION: STBC 0x0047ad80 _$E201
// FUNCTION: STBC 0x0047ad90 _$E392
// FUNCTION: STBC 0x0047ada0 _$E391
// FUNCTION: STBC 0x0047adb0 _$E167
// FUNCTION: STBC 0x0047adc0 _$E166
// FUNCTION: STBC 0x0047add0 _$E396
// FUNCTION: STBC 0x0047ade0 _$E395
// FUNCTION: STBC 0x0047adf0 _$E400
// FUNCTION: STBC 0x0047ae00 _$E399
// FUNCTION: STBC 0x0047ae10 _$E207
// FUNCTION: STBC 0x0047ae20 _$E206
// FUNCTION: STBC 0x0047ae30 _$E212
// FUNCTION: STBC 0x0047ae40 _$E211
// FUNCTION: STBC 0x0047ae50 _$E404
// FUNCTION: STBC 0x0047ae60 _$E403
// FUNCTION: STBC 0x0047ae70 _$E408
// FUNCTION: STBC 0x0047ae80 _$E407
// FUNCTION: STBC 0x0047ae90 _$E412
// FUNCTION: STBC 0x0047aea0 _$E411
// FUNCTION: STBC 0x0047aeb0 _$E416
// FUNCTION: STBC 0x0047aec0 _$E415
// FUNCTION: STBC 0x0047aed0 _$E420
// FUNCTION: STBC 0x0047aee0 _$E419
// FUNCTION: STBC 0x0047aef0 _$E424
// FUNCTION: STBC 0x0047af00 _$E423
// FUNCTION: STBC 0x0047af10 _$E428
// FUNCTION: STBC 0x0047af20 _$E427
// FUNCTION: STBC 0x0047af30 _$E432
// FUNCTION: STBC 0x0047af40 _$E431
// FUNCTION: STBC 0x0047af50 _$E217
// FUNCTION: STBC 0x0047af60 _$E216
// FUNCTION: STBC 0x0047af70 _$E436
// FUNCTION: STBC 0x0047af80 _$E435
// FUNCTION: STBC 0x0047af90 _$E222
// FUNCTION: STBC 0x0047afa0 _$E221
// FUNCTION: STBC 0x0047afb0 _$E227
// FUNCTION: STBC 0x0047afc0 _$E226
// FUNCTION: STBC 0x0047afd0 _$E440
// FUNCTION: STBC 0x0047afe0 _$E439
// FUNCTION: STBC 0x0047aff0 _$E445
// FUNCTION: STBC 0x0047b000 _$E444
// FUNCTION: STBC 0x0047b010 _$E449
// FUNCTION: STBC 0x0047b020 _$E448

// FUNCTION: STBC 0x004789f0 _$E452
// FUNCTION: STBC 0x00478a00 _$E451
static AICreateFunctionRegistration conditionalAIRegistration( AI_TYPE_CONDITIONAL, ConditionalAI::Create );

/*
================
ConditionalAI::ConditionalAI
================
*/
// FUNCTION: STBC 0x00478a50 ??0ConditionalAI@@QAE@PAVPhysicsObjectClass@@PAD@Z
ConditionalAI::ConditionalAI( PhysicsObjectClass *object, char *name_ ) : ArtificialIntelligence( object, name_ ),
	status( US_INVALID ), numTableConditions( 0 ), table( NULL ), containedAI( NULL ) {
}

/*
================
ConditionalAI::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00478b10
PyObject *ConditionalAI::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "ConditionalAI" );
}

/*
================
ConditionalAI::GetAIType
================
*/
// FUNCTION: STBC 0x00478b20
int ConditionalAI::GetAIType( void ) {
	return AI_TYPE_CONDITIONAL;
}

/*
================
ConditionalAI::VirtualSlot20
================
*/
// FUNCTION: STBC 0x00478b30
bool ConditionalAI::VirtualSlot20( int type ) {
	return type == AI_TYPE_CONDITIONAL ? true : false;
}

/*
================
ConditionalAI::~ConditionalAI

Removes the conditions, deletes the contained AI, and releases the
external functions.
================
*/
// FUNCTION: STBC 0x00478b60
ConditionalAI::~ConditionalAI( void ) {
	while ( numConditions != 0 ) {
		RemoveCondition( conditions[0] );
	}
	if ( table != NULL ) {
		delete [] table;
	}
	delete containedAI;

	NiTMapIterator pos = functionLists.GetFirstPos();
	while ( pos != NULL ) {
		const char *functionName;
		NiTList<PyObject *> *functions;
		functionLists.GetNext( pos, functionName, functions );
		if ( functions != NULL ) {
			while ( !functions->IsEmpty() ) {
				// Py_XDECREF evaluates its argument up to four times: each
				// evaluation removes another function, and the reference
				// released may not be the one tested.
				Py_XDECREF( functions->RemoveHead() );
			}
			delete functions;
		}
	}
	functionLists.RemoveAll();
}

/*
================
ConditionalAI::SetEvaluationFunction

The table is made for the conditions held now, and the status is
evaluated at once.
================
*/
// FUNCTION: STBC 0x00478dc0
void ConditionalAI::SetEvaluationFunction( PyObject *function ) {
	if ( table != NULL ) {
		delete [] table;
	}
	numTableConditions = numConditions;
	table = new char[1 << numTableConditions];
	if ( table == NULL ) {
		numTableConditions = 0;
	} else {
		PyObject *statuses = PyTuple_New( numTableConditions );
		for ( int combination = ( 1 << numTableConditions ) - 1; combination >= 0; combination-- ) {
			// Condition i's status is bit i of the combination.
			int bit = 1;
			for ( int i = 0; i < numTableConditions; i++, bit <<= 1 ) {
				PyTuple_SetItem( statuses, i, PyInt_FromLong( ( combination & bit ) != 0 ) );
			}
			// The call releases the reference it is given.
			Py_INCREF( statuses );
			int result = US_ACTIVE;
			if ( PyEmbed::CallObject( function, "i", &result, statuses ) == -1 ) {
				result = US_INVALID;
				PyErr_Clear();
			}
			if ( result < US_ACTIVE || result >= US_NUM_STATUSES ) {
				result = US_INVALID;
			}
			table[combination] = result;
		}
		Py_DECREF( statuses );
	}
	status = EvaluateConditions();
}

/*
================
ConditionalAI::AddCondition

A condition script is asked for its external functions, which it
registers with this AI ("RegisterExternalFunctions").
================
*/
// FUNCTION: STBC 0x00478f00
void ConditionalAI::AddCondition( TGCondition *condition ) {
	TGConditionHandler::AddCondition( condition );
	ConditionScript *script = ConditionScript::Cast( condition );
	if ( script != NULL ) {
		PyObject *instance = script->GetScriptInstance();
		if ( instance != NULL && PyObject_HasAttrString( instance, "RegisterExternalFunctions" ) ) {
			PyObject *self = VirtualSlot15();
			PyEmbed::CallMethod( instance, "RegisterExternalFunctions", "", NULL, "(O)", self );
			Py_DECREF( self );
			PyErr_Print();
		}
	}
}

/*
================
ConditionalAI::RemoveCondition

A condition script's external functions go with it.
================
*/
// FUNCTION: STBC 0x00478f80
void ConditionalAI::RemoveCondition( TGCondition *condition ) {
	ConditionScript *script = ConditionScript::Cast( condition );
	if ( script != NULL ) {
		RemoveConditionFunctions( script->GetObjID() );
	}
	TGConditionHandler::RemoveCondition( condition );
}

/*
================
ConditionalAI::RemoveConditionFunctions

Releases the external functions the condition script of the ID
registered, and drops the names left without functions. A function entry
whose "CodeID" is missing or not an integer clears the lookup's error.
================
*/
// FUNCTION: STBC 0x00478fb0
void ConditionalAI::RemoveConditionFunctions( long scriptID ) {
	NiTMapIterator pos = functionLists.GetFirstPos();
	while ( pos != NULL ) {
		const char *functionName;
		NiTList<PyObject *> *functions;
		functionLists.GetNext( pos, functionName, functions );

		NiTListIterator functionPos = functions->GetHeadPos();
		while ( functionPos != NULL ) {
			NiTListIterator current = functionPos;
			PyObject *function = functions->GetNext( functionPos );
			if ( PyDict_Check( function ) ) {
				PyObject *codeID = PyDict_GetItemString( function, "CodeID" );
				if ( codeID != NULL && PyInt_Check( codeID ) ) {
					if ( scriptID == PyInt_AsLong( codeID ) ) {
						functions->RemovePos( current );
						Py_DECREF( function );
					}
				} else {
					PyErr_Clear();
				}
			}
		}

		if ( functions->IsEmpty() ) {
			functionLists.RemoveAt( functionName );
			delete functions;
		}
	}
}

/*
================
ConditionalAI::SetContainedAI
================
*/
// FUNCTION: STBC 0x004791e0
void ConditionalAI::SetContainedAI( ArtificialIntelligence *ai ) {
	delete containedAI;
	containedAI = ai->VirtualSlot13();
	containedAI->SetParentID( id );
}

/*
================
ConditionalAI::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x00479210
void ConditionalAI::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais ) {
	ais.AddTail( this );
	if ( containedAI != NULL ) {
		containedAI->GetAllAIsInTree( ais );
	}
}

/*
================
ConditionalAI::GetFocusAIs
================
*/
// FUNCTION: STBC 0x00479280
void ConditionalAI::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais ) {
	if ( focus ) {
		ais.AddTail( this );
		if ( containedAI != NULL && containedAI->HasFocus() ) {
			containedAI->GetFocusAIs( ais );
		}
	}
}

/*
================
ConditionalAI::RegisterExternalFunction

Each function is an entry of the condition script that registered it: a
dictionary of its "CodeID" (the condition's ID) and its "FunctionName".
================
*/
// FUNCTION: STBC 0x00479300
void ConditionalAI::RegisterExternalFunction( char *functionName, PyObject *function ) {
	NiTList<PyObject *> *functions;
	if ( !functionLists.GetAt( functionName, functions ) ) {
		functions = new NiTList<PyObject *>( 2 );
		functionLists.SetAt( functionName, functions );
	}
	Py_INCREF( function );
	functions->AddTail( function );
}

/*
================
ConditionalAI::CallExternalFunction

True when no function is registered under the name, else when every
call succeeded.
================
*/
// FUNCTION: STBC 0x004794d0
bool ConditionalAI::CallExternalFunction( char *functionName, PyObject *arguments ) {
	bool result = true;
	NiTList<PyObject *> *functions;
	if ( functionLists.GetAt( functionName, functions ) ) {
		result = true;
		NiTListIterator pos = functions->GetHeadPos();
		while ( pos != NULL ) {
			result &= CallConditionFunction( functionName, functions->GetNext( pos ), arguments );
		}
	}
	return result;
}

/*
================
ConditionalAI::CallConditionFunction

Calls the entry's method on the instance of the condition script whose
ID it carries. False when no such condition is held, or the call fails.
================
*/
// FUNCTION: STBC 0x00479550
bool ConditionalAI::CallConditionFunction( char *functionName, PyObject *function, PyObject *arguments ) {
	bool called = false;
	PyObject *codeID = PyDict_GetItemString( function, "CodeID" );
	if ( codeID == NULL || !PyInt_Check( codeID ) ) {
		return false;
	}
	long scriptID = PyInt_AsLong( codeID );
	PyObject *methodName = PyDict_GetItemString( function, "FunctionName" );
	if ( methodName == NULL || !PyString_Check( methodName ) ) {
		return false;
	}
	char *method = PyString_AsString( methodName );

	for ( int i = 0; i < numConditions; i++ ) {
		ConditionScript *script = ConditionScript::Cast( conditions[i] );
		if ( script != NULL ) {
			PyObject *instance = script->GetScriptInstance();
			if ( instance != NULL && script->GetObjID() == scriptID ) {
				called = true;
				if ( PyEmbed::CallMethod( instance, method, "", NULL, "(O)", arguments ) == -1 ) {
					PyErr_Print();
					PyObject *text = PyObject_Str( instance );
					Py_DECREF( text );
					called = false;
				}
			}
		}
	}
	return called;
}

/*
================
ConditionalAI::SetActive

The contained AI is activated only while the status is Active.
================
*/
// FUNCTION: STBC 0x00479670
void ConditionalAI::SetActive( void ) {
	AILog log( "ConditionalAI(%s)::SetActive", name );
	active = true;
	if ( containedAI != NULL && status == US_ACTIVE && !containedAI->IsActive() ) {
		containedAI->SetActive();
	}
	for ( int i = 0; i < numConditions; i++ ) {
		conditions[i]->SetActive();
	}
}

/*
================
ConditionalAI::SetInactive
================
*/
// FUNCTION: STBC 0x00479710
void ConditionalAI::SetInactive( void ) {
	AILog log( "ConditionalAI(%s)::SetInactive", name );
	active = false;
	if ( containedAI != NULL && containedAI->IsActive() ) {
		containedAI->SetInactive();
	}
	for ( int i = 0; i < numConditions; i++ ) {
		conditions[i]->SetInactive();
	}
}

/*
================
ConditionalAI::GotFocus
================
*/
// FUNCTION: STBC 0x004797a0
void ConditionalAI::GotFocus( void ) {
	AILog log( "ConditionalAI(%s)::GotFocus", name );
	focus = true;
}

/*
================
ConditionalAI::LostFocus
================
*/
// FUNCTION: STBC 0x004797d0
void ConditionalAI::LostFocus( void ) {
	AILog log( "ConditionalAI(%s)::LostFocus", name );
	focus = false;
	if ( containedAI != NULL && containedAI->HasFocus() ) {
		containedAI->LostFocus();
	}
}

/*
================
ConditionalAI::IsInterruptable
================
*/
// FUNCTION: STBC 0x00479840
bool ConditionalAI::IsInterruptable( void ) {
	bool result = true;
	result &= interruptable;
	if ( result && active && containedAI != NULL ) {
		result &= containedAI->IsInterruptable();
	}
	return result;
}

/*
================
ConditionalAI::Update

Runs the contained AI while the status is Active, or while the contained
AI is busy and cannot be interrupted; the status is returned otherwise.
While the AI tree log is open, the condition scripts' status texts are
logged.
================
*/
// FUNCTION: STBC 0x00479870
int ConditionalAI::Update( double time ) {
	char *statusName;
	switch ( status ) {
	case US_ACTIVE:
		statusName = "Active";
		break;
	case US_DONE:
		statusName = "Done";
		break;
	case US_DORMANT:
		statusName = "Dormant";
		break;
	default:
		statusName = "Invalid";
		break;
	}
	AILog log( "ConditionalAI(%s)::Update, condition %s", name, statusName );

	if ( AILog::IsOpen() ) {
		for ( int i = 0; i < numConditions; i++ ) {
			ConditionScript *script = ConditionScript::Cast( conditions[i] );
			if ( script != NULL ) {
				PyObject *instance = script->GetScriptInstance();
				if ( instance != NULL && PyObject_HasAttrString( instance, "GetStatusInfo" ) ) {
					PyObject *statusInfo = NULL;
					if ( PyEmbed::CallMethod( instance, "GetStatusInfo", "O", &statusInfo, "()" ) == -1 ) {
						PyErr_Print();
					} else {
						AILog( "%s", PyString_AsString( statusInfo ) );
						Py_DECREF( statusInfo );
					}
				}
			}
		}
	}

	if ( status != US_ACTIVE && ( containedAI == NULL || !containedAI->IsActive() || containedAI->IsInterruptable() ) ) {
		return status;
	}
	if ( !containedAI->IsActive() ) {
		containedAI->SetActive();
	}
	if ( !containedAI->HasFocus() ) {
		containedAI->GotFocus();
	}
	return containedAI->Update( time );
}

/*
================
ConditionalAI::Reset
================
*/
// FUNCTION: STBC 0x00479a50
void ConditionalAI::Reset( void ) {
	if ( containedAI != NULL ) {
		containedAI->Reset();
	}
}

/*
================
ConditionalAI::IsDormant

An active contained AI that cannot be interrupted keeps the AI awake;
otherwise a Dormant status is dormant, and an Active one is as dormant as
the contained AI.
================
*/
// FUNCTION: STBC 0x00479a60
bool ConditionalAI::IsDormant( void ) {
	char text[128];
	sprintf( text, "ConditionalAI(%s)::IsDormant, (%s)", name, status == US_DORMANT ? "true" : "false" );
	AILog log( text );
	if ( active && containedAI != NULL && containedAI->IsActive() && !containedAI->IsInterruptable() ) {
		return false;
	}
	if ( status == US_DORMANT ) {
		return true;
	}
	if ( status == US_ACTIVE && containedAI != NULL ) {
		return containedAI->IsDormant();
	}
	return false;
}

/*
================
ConditionalAI::ConditionChanged
================
*/
// FUNCTION: STBC 0x00479b90
void ConditionalAI::ConditionChanged( TGCondition *condition ) {
	status = EvaluateConditions();
}

/*
================
ConditionalAI::EvaluateConditions
================
*/
// FUNCTION: STBC 0x00479bb0
int ConditionalAI::EvaluateConditions( void ) {
	if ( numConditions != numTableConditions ) {
		return US_INVALID;
	}
	int combination = 0;
	for ( int i = 0; i < numConditions; i++ ) {
		combination |= conditions[i]->GetStatus() << i;
	}
	return table[combination];
}

/*
================
ConditionalAI_SetEvaluationFunction

PyArg_ParseTuple's failure (0) is not caught: the check is for -1.
================
*/
// FUNCTION: STBC 0x00479c00
PyObject *ConditionalAI_SetEvaluationFunction( PyObject *self, PyObject *args ) {
	PyObject *function = NULL;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "OO", &instance, &function ) != -1 ) {
		ConditionalAI *ai;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "ConditionalAI *" );
		ai->SetEvaluationFunction( function );
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
ConditionalAI::WriteToStream
================
*/
// FUNCTION: STBC 0x00479c70 ?WriteToStream@ConditionalAI@@UAE_NPAVTGStream@@@Z
bool ConditionalAI::WriteToStream( TGStream *stream ) {
	if ( !ArtificialIntelligence::WriteToStream( stream ) ) {
		return false;
	}
	if ( !TGConditionHandler::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( status );
	if ( containedAI != NULL ) {
		stream->WriteBool( true );
		if ( !containedAI->WriteToStream( stream ) ) {
			return false;
		}
	} else {
		stream->WriteBool( false );
	}

	stream->WriteInt( numTableConditions );
	for ( int combination = ( 1 << numTableConditions ) - 1; combination >= 0; combination-- ) {
		stream->WriteChar( table[combination] );
	}

	stream->WriteInt( functionLists.GetCount() );
	NiTMapIterator pos = functionLists.GetFirstPos();
	while ( pos != NULL ) {
		const char *functionName;
		NiTList<PyObject *> *functions;
		functionLists.GetNext( pos, functionName, functions );
		stream->VirtualSlot11( (char *)functionName );
		stream->WriteInt( functions->GetSize() );
		NiTListIterator functionPos = functions->GetHeadPos();
		while ( functionPos != NULL ) {
			if ( !PyEmbed::SerializePython( functions->GetNext( functionPos ), stream, 0 ) ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
ConditionalAI::ConditionalAI

Reads what WriteToStream wrote, after the AI's part; ok is cleared on a
failure.
================
*/
// FUNCTION: STBC 0x00479de0 ??0ConditionalAI@@QAE@PAVTGStream@@PA_N@Z
ConditionalAI::ConditionalAI( TGStream *stream, bool *ok ) : ArtificialIntelligence( stream, ok ),
	status( US_INVALID ), numTableConditions( 0 ), table( NULL ), containedAI( NULL ) {
	*ok &= TGConditionHandler::ReadFromStream( stream );
	if ( !*ok ) {
		return;
	}

	status = stream->ReadLong();
	delete containedAI;
	containedAI = NULL;
	if ( stream->ReadBool() == true ) {
		containedAI = ArtificialIntelligence::CreateFromStream( stream );
		if ( containedAI == NULL ) {
			*ok = false;
		}
	}

	numTableConditions = stream->ReadInt();
	table = new char[1 << numTableConditions];
	if ( table == NULL ) {
		*ok = false;
		return;
	}
	for ( int combination = ( 1 << numTableConditions ) - 1; combination >= 0; combination-- ) {
		table[combination] = stream->ReadChar();
	}

	for ( int i = stream->ReadInt() - 1; i >= 0; i-- ) {
		char *functionName = stream->VirtualSlot10( -1 );
		NiTList<PyObject *> *functions = new NiTList<PyObject *>( 2 );
		for ( int j = stream->ReadInt() - 1; j >= 0; j-- ) {
			PyObject *function = PyEmbed::UnserializePython( stream, 0 );
			if ( function == NULL ) {
				*ok = false;
				return;
			}
			functions->AddTail( function );
		}
		functionLists.SetAt( functionName, functions );
		delete [] functionName;
	}
}

/*
================
ConditionalAI::Create
================
*/
// FUNCTION: STBC 0x0047a130
ArtificialIntelligence *ConditionalAI::Create( TGStream *stream ) {
	bool ok = true;
	ConditionalAI *ai = new ConditionalAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}

/*
================
ConditionalAI::FixUpPointers
================
*/
// FUNCTION: STBC 0x0047a1b0 ?FixUpPointers@ConditionalAI@@UAE_NXZ
bool ConditionalAI::FixUpPointers( void ) {
	if ( !TGConditionHandler::FixUpPointers() ) {
		return false;
	}
	if ( containedAI != NULL && !containedAI->FixUpPointers() ) {
		return false;
	}
	return true;
}

/*
================
ConditionalAI::PostFixUp
================
*/
// FUNCTION: STBC 0x0047a1e0 ?PostFixUp@ConditionalAI@@UAEXXZ
void ConditionalAI::PostFixUp( void ) {
	TGConditionHandler::PostFixUp();
	if ( containedAI != NULL ) {
		containedAI->PostFixUp();
	}
}
