/*
===============================================================================

	TGHandler.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "TGHandler.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006e0070 _$E440
// FUNCTION: STBC 0x006e0090 _$E439
// FUNCTION: STBC 0x006e00a0 _$E32
// FUNCTION: STBC 0x006e00b0 _$E29
// FUNCTION: STBC 0x006e00c0 _$E31
// FUNCTION: STBC 0x006e00d0 _$E30
// FUNCTION: STBC 0x006e00e0 _$E38
// FUNCTION: STBC 0x006e00f0 _$E35
// FUNCTION: STBC 0x006e0100 _$E37
// FUNCTION: STBC 0x006e0110 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006e0120 _$E77
// FUNCTION: STBC 0x006e0130 _$E76
// FUNCTION: STBC 0x006e0140 _$E81
// FUNCTION: STBC 0x006e0150 _$E80
// FUNCTION: STBC 0x006e0160 _$E92
// FUNCTION: STBC 0x006e0170 _$E91
// FUNCTION: STBC 0x006e0180 _$E96
// FUNCTION: STBC 0x006e0190 _$E95
// FUNCTION: STBC 0x006e01a0 _$E100
// FUNCTION: STBC 0x006e01b0 _$E99
// FUNCTION: STBC 0x006e01c0 _$E104
// FUNCTION: STBC 0x006e01d0 _$E103
// FUNCTION: STBC 0x006e01e0 _$E228
// FUNCTION: STBC 0x006e01f0 _$E227
// FUNCTION: STBC 0x006e0200 _$E170
// FUNCTION: STBC 0x006e0210 _$E169
// FUNCTION: STBC 0x006e0220 _$E183
// FUNCTION: STBC 0x006e0230 _$E182
// FUNCTION: STBC 0x006e0240 _$E233
// FUNCTION: STBC 0x006e0250 _$E232
// FUNCTION: STBC 0x006e0260 _$E238
// FUNCTION: STBC 0x006e0270 _$E237
// FUNCTION: STBC 0x006e0280 _$E242
// FUNCTION: STBC 0x006e0290 _$E241
// FUNCTION: STBC 0x006e02a0 _$E246
// FUNCTION: STBC 0x006e02b0 _$E245
// FUNCTION: STBC 0x006e02c0 _$E253
// FUNCTION: STBC 0x006e02d0 _$E252
// FUNCTION: STBC 0x006e02e0 _$E278
// FUNCTION: STBC 0x006e02f0 _$E277
// FUNCTION: STBC 0x006e0300 _$E282
// FUNCTION: STBC 0x006e0310 _$E281
// FUNCTION: STBC 0x006e0320 _$E128
// FUNCTION: STBC 0x006e0330 _$E127
// FUNCTION: STBC 0x006e0340 _$E264
// FUNCTION: STBC 0x006e0350 _$E263
// FUNCTION: STBC 0x006e0360 _$E271
// FUNCTION: STBC 0x006e0370 _$E270
// FUNCTION: STBC 0x006e0380 _$E294
// FUNCTION: STBC 0x006e0390 _$E293
// FUNCTION: STBC 0x006e03a0 _$E298
// FUNCTION: STBC 0x006e03b0 _$E297
// FUNCTION: STBC 0x006e03c0 _$E338
// FUNCTION: STBC 0x006e03d0 _$E337
// FUNCTION: STBC 0x006e03e0 _$E344
// FUNCTION: STBC 0x006e03f0 _$E343
// FUNCTION: STBC 0x006e0400 _$E286
// FUNCTION: STBC 0x006e0410 _$E285
// FUNCTION: STBC 0x006e0420 _$E290
// FUNCTION: STBC 0x006e0430 _$E289
// FUNCTION: STBC 0x006e0440 _$E302
// FUNCTION: STBC 0x006e0450 _$E301
// FUNCTION: STBC 0x006e0460 _$E306
// FUNCTION: STBC 0x006e0470 _$E305
// FUNCTION: STBC 0x006e0480 _$E310
// FUNCTION: STBC 0x006e0490 _$E309
// FUNCTION: STBC 0x006e04a0 _$E314
// FUNCTION: STBC 0x006e04b0 _$E313
// FUNCTION: STBC 0x006e04c0 _$E318
// FUNCTION: STBC 0x006e04d0 _$E317
// FUNCTION: STBC 0x006e04e0 _$E322
// FUNCTION: STBC 0x006e04f0 _$E321
// FUNCTION: STBC 0x006e0500 _$E326
// FUNCTION: STBC 0x006e0510 _$E325
// FUNCTION: STBC 0x006e0520 _$E330
// FUNCTION: STBC 0x006e0530 _$E329
// FUNCTION: STBC 0x006e0540 _$E133
// FUNCTION: STBC 0x006e0550 _$E132
// FUNCTION: STBC 0x006e0560 _$E334
// FUNCTION: STBC 0x006e0570 _$E333
// FUNCTION: STBC 0x006e0580 _$E138
// FUNCTION: STBC 0x006e0590 _$E137
// FUNCTION: STBC 0x006e05a0 _$E348
// FUNCTION: STBC 0x006e05b0 _$E347
// FUNCTION: STBC 0x006e05c0 _$E352
// FUNCTION: STBC 0x006e05d0 _$E351
// FUNCTION: STBC 0x006e05e0 _$E356
// FUNCTION: STBC 0x006e05f0 _$E355
// FUNCTION: STBC 0x006e0600 _$E360
// FUNCTION: STBC 0x006e0610 _$E359
// FUNCTION: STBC 0x006e0620 _$E147
// FUNCTION: STBC 0x006e0630 _$E146
// FUNCTION: STBC 0x006e0640 _$E364
// FUNCTION: STBC 0x006e0650 _$E363
// FUNCTION: STBC 0x006e0660 _$E368
// FUNCTION: STBC 0x006e0670 _$E367
// FUNCTION: STBC 0x006e0680 _$E372
// FUNCTION: STBC 0x006e0690 _$E371
// FUNCTION: STBC 0x006e06a0 _$E153
// FUNCTION: STBC 0x006e06b0 _$E152
// FUNCTION: STBC 0x006e06c0 _$E158
// FUNCTION: STBC 0x006e06d0 _$E157
// FUNCTION: STBC 0x006e06e0 _$E188
// FUNCTION: STBC 0x006e06f0 _$E187
// FUNCTION: STBC 0x006e0700 _$E376
// FUNCTION: STBC 0x006e0710 _$E375
// FUNCTION: STBC 0x006e0720 _$E193
// FUNCTION: STBC 0x006e0730 _$E192
// FUNCTION: STBC 0x006e0740 _$E380
// FUNCTION: STBC 0x006e0750 _$E379
// FUNCTION: STBC 0x006e0760 _$E384
// FUNCTION: STBC 0x006e0770 _$E383
// FUNCTION: STBC 0x006e0780 _$E198
// FUNCTION: STBC 0x006e0790 _$E197
// FUNCTION: STBC 0x006e07a0 _$E388
// FUNCTION: STBC 0x006e07b0 _$E387
// FUNCTION: STBC 0x006e07c0 _$E163
// FUNCTION: STBC 0x006e07d0 _$E162
// FUNCTION: STBC 0x006e07e0 _$E392
// FUNCTION: STBC 0x006e07f0 _$E391
// FUNCTION: STBC 0x006e0800 _$E396
// FUNCTION: STBC 0x006e0810 _$E395
// FUNCTION: STBC 0x006e0820 _$E203
// FUNCTION: STBC 0x006e0830 _$E202
// FUNCTION: STBC 0x006e0840 _$E208
// FUNCTION: STBC 0x006e0850 _$E207
// FUNCTION: STBC 0x006e0860 _$E400
// FUNCTION: STBC 0x006e0870 _$E399
// FUNCTION: STBC 0x006e0880 _$E404
// FUNCTION: STBC 0x006e0890 _$E403
// FUNCTION: STBC 0x006e08a0 _$E408
// FUNCTION: STBC 0x006e08b0 _$E407
// FUNCTION: STBC 0x006e08c0 _$E412
// FUNCTION: STBC 0x006e08d0 _$E411
// FUNCTION: STBC 0x006e08e0 _$E416
// FUNCTION: STBC 0x006e08f0 _$E415
// FUNCTION: STBC 0x006e0900 _$E420
// FUNCTION: STBC 0x006e0910 _$E419
// FUNCTION: STBC 0x006e0920 _$E424
// FUNCTION: STBC 0x006e0930 _$E423
// FUNCTION: STBC 0x006e0940 _$E428
// FUNCTION: STBC 0x006e0950 _$E427
// FUNCTION: STBC 0x006e0960 _$E213
// FUNCTION: STBC 0x006e0970 _$E212
// FUNCTION: STBC 0x006e0980 _$E432
// FUNCTION: STBC 0x006e0990 _$E431
// FUNCTION: STBC 0x006e09a0 _$E218
// FUNCTION: STBC 0x006e09b0 _$E217
// FUNCTION: STBC 0x006e09c0 _$E223
// FUNCTION: STBC 0x006e09d0 _$E222

/*
================
TGHandler::TGHandler
================
*/
// FUNCTION: STBC 0x006e09e0
TGHandler::TGHandler( void ) {
	flags = 0;
	next = NULL;
	target = ANY_TARGET;
	function = NULL;
}

/*
================
TGHandler::~TGHandler
================
*/
// FUNCTION: STBC 0x006e0a00
TGHandler::~TGHandler( void ) {
	Clear();
}

/*
================
TGHandler::Clear

Frees a Python function name.
================
*/
// FUNCTION: STBC 0x006e0a10
void TGHandler::Clear( void ) {
	if ( IsPython() && pythonName != NULL ) {
		delete [] pythonName;
		pythonName = NULL;
	}
}

/*
================
TGHandler::WriteToStream

The flags; the Python name (length, then the characters with the
terminator) or the registered handler's name hash; the target's ID, -1 for
ANY_TARGET. Fails for a registered handler that is not in the registry.
================
*/
// FUNCTION: STBC 0x006e0a40
bool TGHandler::WriteToStream( TGStream *stream ) {
	char message[256];
	bool result = true;

	stream->WriteLong( flags );
	if ( IsPython() ) {
		short length = strlen( pythonName ) + 1;
		stream->WriteShort( length );
		stream->Write( pythonName, length );
	} else {
		int nameHash = handlerRegistry.FindNameHash( GetFunction() );
		if ( nameHash != 0 ) {
			stream->WriteID( nameHash );
		} else {
			result = false;
		}
	}

	if ( target != NULL ) {
		if ( target == ANY_TARGET ) {
			sprintf( message, "Saving TGHandler to ANY_TARGET\n" );
			TGObject::Log( message );
			stream->WriteID( -1 );
		} else {
			stream->WriteID( target->GetObjID() );
		}
	} else {
		stream->WriteID( 0 );
	}
	return result;
}

/*
================
TGHandler::ReadFromStream
================
*/
// FUNCTION: STBC 0x006e0b20
bool TGHandler::ReadFromStream( TGStream *stream ) {
	char message[256];
	bool result = true;
	flags = 0;
	pythonName = NULL;

	flags = stream->ReadLong();
	if ( IsPython() ) {
		short length = stream->ReadShort();
		pythonName = new char[length];
		if ( pythonName != NULL ) {
			stream->Read( pythonName, length );
		} else {
			result = false;
		}
	} else {
		TGHandlerFunction registered = handlerRegistry.Find( stream->ReadID() );
		if ( registered == NULL ) {
			result = false;
		}
		function = registered;
	}

	target = (TGEventHandlerObject *)stream->ReadID();
	if ( target == (TGEventHandlerObject *)-1 ) {
		sprintf( message, "Loading TGHandler to ANY_TARGET\n" );
		TGObject::Log( message );
		target = ANY_TARGET;
	}
	return result;
}

/*
================
TGHandler::FixUpPointers

False when the target's ID names no object.
================
*/
// FUNCTION: STBC 0x006e0be0
bool TGHandler::FixUpPointers( void ) {
	if ( target != ANY_TARGET && target != NULL && TGObject::GetTGObjectPtr( (long)target ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGHandler::PostFixUp

Turns the target's ID back into the object.
================
*/
// FUNCTION: STBC 0x006e0c10
void TGHandler::PostFixUp( void ) {
	if ( target != NULL && target != ANY_TARGET ) {
		target = (TGEventHandlerObject *)TGObject::GetTGObjectPtr( (long)target );
	}
}

/*
================
TGHandler::Call

Hands the event to the handler: a Python function gets the object and the
event, a Python method is called on the object with the event, and a
registered handler likewise. A handler removed while it ran is deleted
when it returns.
================
*/
// FUNCTION: STBC 0x006e0c30
void TGHandler::Call( TGEvent *event, TGEventHandlerObject *object ) {
	flags |= HANDLER_CALLING;
	if ( function == NULL ) {
		// the original returns with HANDLER_CALLING still set
		return;
	}

	if ( IsPython() ) {
		PyObject *pyEvent = event->VirtualSlot08();
		PyObject *pyObject = object->VirtualSlot08();
		if ( IsMethod() && object != NULL ) {
			PyEmbed::CallMethod( pyObject, pythonName, NULL, NULL, "(O)", pyEvent );
			if ( PyErr_Occurred() ) {
				PyErr_Print();
				PyErr_Clear();
				PyObject *text = PyObject_Str( pyObject );
				Py_DECREF( text );
			}
		} else {
			CallPython( pyEvent, pyObject );
		}
		Py_DECREF( pyEvent );
		Py_DECREF( pyObject );
	} else if ( IsMethod() && object != NULL ) {
		( object->*method )( event );
	} else {
		function( object, event );
	}

	flags &= ~HANDLER_CALLING;
	if ( IsDeletePending() == true ) {
		delete this;
	}
}

/*
================
TGHandler::CallPython

Calls the Python function "module.function" (a bare name is looked up in
__main__) with the object and the event.
================
*/
// FUNCTION: STBC 0x006e0d40
void TGHandler::CallPython( PyObject *event, PyObject *object ) {
	char *module = pythonName;
	char *name = NULL;
	for ( int i = strlen( pythonName ) - 1; i >= 0; i-- ) {
		if ( pythonName[i] == '.' ) {
			name = &pythonName[i];
			break;
		}
	}
	if ( name == NULL ) {
		module = NULL;
		name = pythonName;
	} else {
		*name = '\0';
		name++;
	}

	PyObject *callable = PyEmbed::GetFunction( module, name );
	if ( PyErr_Occurred() == NULL ) {
		PyObject *args = Py_BuildValue( "(OO)", object, event );
		PyObject *error = PyErr_Occurred();
		if ( error == NULL ) {
			// the original puts the dot back only on this path
			if ( module != NULL ) {
				name[-1] = '.';
			}
			PyEmbed::CallObject( callable, NULL, NULL, args );
			error = PyErr_Occurred();
		}
		Py_DECREF( callable );
		if ( error == NULL ) {
			return;
		}
	}
	PyErr_Print();
	PyErr_Clear();
}

/*
================
TGHandler::SetPythonFunction
================
*/
// FUNCTION: STBC 0x006e0e00
bool TGHandler::SetPythonFunction( char *name, bool method ) {
	Clear();
	SetPython( true );
	SetMethod( method );
	pythonName = new char[strlen( name ) + 1];
	if ( pythonName != NULL ) {
		strcpy( pythonName, name );
		return true;
	}
	return false;
}

/*
================
TGHandler::SetFunction

Takes the registered handler of the name; false when there is none.
================
*/
// FUNCTION: STBC 0x006e0e70
bool TGHandler::SetFunction( char *name, bool method ) {
	Clear();
	TGHandlerFunction registered = handlerRegistry.Find( TGHashString( name ) );
	if ( registered == NULL ) {
		return false;
	}
	SetPython( false );
	SetMethod( method );
	function = registered;
	return true;
}

/*
================
TGHandler::SetPython
================
*/
// FUNCTION: STBC 0x006e0ec0
void TGHandler::SetPython( bool python ) {
	if ( python ) {
		flags |= HANDLER_PYTHON;
	} else {
		flags &= ~HANDLER_PYTHON;
	}
}

/*
================
TGHandler::SetMethod
================
*/
// FUNCTION: STBC 0x006e0ee0
void TGHandler::SetMethod( bool method ) {
	if ( method ) {
		flags |= HANDLER_METHOD;
	} else {
		flags &= ~HANDLER_METHOD;
	}
}
