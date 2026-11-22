/*
===============================================================================

	TGScriptAction.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGScriptAction.h"
#include "PyEmbed.h"
#include "TGStream.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ff660 _$E32
// FUNCTION: STBC 0x006ff670 _$E29
// FUNCTION: STBC 0x006ff680 _$E31
// FUNCTION: STBC 0x006ff690 _$E30
// FUNCTION: STBC 0x006ff6a0 _$E38
// FUNCTION: STBC 0x006ff6b0 _$E35
// FUNCTION: STBC 0x006ff6c0 _$E37
// FUNCTION: STBC 0x006ff6d0 _$E36

/*
================
TGScriptAction::TGScriptAction
================
*/
// FUNCTION: STBC 0x006ff6e0
TGScriptAction::TGScriptAction( char *module, char *function ) {
	this->module = new char[strlen( module ) + 1];
	strcpy( this->module, module );
	this->function = new char[strlen( function ) + 1];
	strcpy( this->function, function );
	arguments = PyList_New( 0 );
}

/*
================
TGScriptAction::~TGScriptAction
================
*/
// FUNCTION: STBC 0x006ff7b0
TGScriptAction::~TGScriptAction( void ) {
	delete[] module;
	delete[] function;
	if ( arguments != NULL ) {
		Py_DECREF( arguments );
		arguments = NULL;
	}
}

/*
================
TGScriptAction::Play

Calls module.function( action, arguments... ). A result of 0 means the
function is done; anything else leaves the action playing until something
calls its Completed. A failed call is reported and ends the action.
================
*/
// FUNCTION: STBC 0x006ff840
void TGScriptAction::Play( void ) {
	char message[256];
	int done;

	PyObject *callable = PyEmbed::GetFunction( module, function );
	if ( callable == NULL ) {
		return;
	}

	// The shadow stays at the front of the list: an action played twice
	// passes itself twice.
	PyObject *shadow = VirtualSlot08();
	PyList_Insert( arguments, 0, shadow );
	Py_DECREF( shadow );

	PyObject *args = PyList_AsTuple( arguments );
	int status = PyEmbed::CallObject( callable, "i", &done, args );
	Py_DECREF( callable );
	Py_DECREF( args );

	PyObject *error = PyErr_Occurred();
	if ( status != -1 && error == NULL ) {
		if ( done == 0 ) {
			Completed();
		}
		return;
	}

	PyErr_Print();
	PyErr_Clear();
	sprintf( message, "TGScriptAction faulted attempting to play %s.%s\n", module, function );
	OutputDebugString( message );
	Completed();
}

/*
================
TGScriptAction::AddArgument

The list takes its own reference as well: the caller's reference is the
one this function adds, and the callers never release theirs.
================
*/
// FUNCTION: STBC 0x006ff940
int TGScriptAction::AddArgument( PyObject *argument ) {
	Py_INCREF( argument );
	int result = PyList_Append( arguments, argument );
	PyObject *error = PyErr_Occurred();
	if ( result != 0 || error != NULL ) {
		PyErr_Print();
		PyErr_Clear();
	}
	return result;
}

/*
================
TGScriptAction_Create

App.TGScriptAction_Create( module, function, arguments... ). The module
name is a file path: '/' becomes '\\', in the Python string itself.
================
*/
// FUNCTION: STBC 0x006ff980
extern "C" PyObject *_wrap_TGScriptAction_Create( PyObject *self, PyObject *args ) {
	if ( PyTuple_Size( args ) < 2 ) {
		Py_INCREF( Py_None );
		return Py_None;
	}

	PyObject *moduleObject = PyTuple_GetItem( args, 0 );
	PyObject *functionObject = PyTuple_GetItem( args, 1 );
	if ( PyErr_Occurred() != NULL ) {
		PyErr_Print();
		PyErr_Clear();
		Py_INCREF( Py_None );
		return Py_None;
	}

	char *module = PyString_AsString( moduleObject );
	char *function = PyString_AsString( functionObject );
	if ( PyErr_Occurred() != NULL ) {
		PyErr_Print();
		PyErr_Clear();
		Py_INCREF( Py_None );
		return Py_None;
	}

	int length = strlen( module );
	for ( int i = 0; i < length; i++ ) {
		if ( module[i] == '/' ) {
			module[i] = '\\';
		}
	}

	TGScriptAction *action = new TGScriptAction( module, function );
	for ( int argument = 2; argument < PyTuple_Size( args ); argument++ ) {
		action->AddArgument( PyTuple_GetItem( args, argument ) );
	}

	// The shadow is a new reference already: this one leaks.
	PyObject *shadow = PyEmbed::Unknown6F9920( action, "TGScriptAction" );
	Py_INCREF( shadow );
	return shadow;
}

/*
================
TGScriptAction::WriteToStream
================
*/
// FUNCTION: STBC 0x006ffa90
bool TGScriptAction::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}

	stream->VirtualSlot11( module );
	stream->VirtualSlot11( function );
	if ( arguments != NULL ) {
		stream->WriteBool( true );
		PyEmbed::SerializePython( arguments, stream, 0 );
	} else {
		stream->WriteBool( false );
	}
	return true;
}

/*
================
TGScriptAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x006ffb00
bool TGScriptAction::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}

	module = stream->VirtualSlot10( -1 );
	function = stream->VirtualSlot10( -1 );
	if ( stream->ReadBool() == true ) {
		arguments = PyEmbed::UnserializePython( stream, 0 );
	}
	return true;
}
