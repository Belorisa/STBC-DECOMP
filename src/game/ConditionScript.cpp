/*
===============================================================================

	ConditionScript.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "ConditionScript.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0047c780 _$E449
// FUNCTION: STBC 0x0047c7a0 _$E448

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0047c340 _$E32
// FUNCTION: STBC 0x0047c350 _$E29
// FUNCTION: STBC 0x0047c360 _$E31
// FUNCTION: STBC 0x0047c370 _$E30
// FUNCTION: STBC 0x0047c380 _$E38
// FUNCTION: STBC 0x0047c390 _$E35
// FUNCTION: STBC 0x0047c3a0 _$E37
// FUNCTION: STBC 0x0047c3b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0047c3c0 _$E73
// FUNCTION: STBC 0x0047c3d0 _$E72
// FUNCTION: STBC 0x0047c3e0 _$E77
// FUNCTION: STBC 0x0047c3f0 _$E76
// FUNCTION: STBC 0x0047c400 _$E88
// FUNCTION: STBC 0x0047c410 _$E87
// FUNCTION: STBC 0x0047c420 _$E92
// FUNCTION: STBC 0x0047c430 _$E91
// FUNCTION: STBC 0x0047c440 _$E96
// FUNCTION: STBC 0x0047c450 _$E95
// FUNCTION: STBC 0x0047c460 _$E100
// FUNCTION: STBC 0x0047c470 _$E99
// FUNCTION: STBC 0x0047c480 _$E224
// FUNCTION: STBC 0x0047c490 _$E223
// FUNCTION: STBC 0x0047c4a0 _$E166
// FUNCTION: STBC 0x0047c4b0 _$E165
// FUNCTION: STBC 0x0047c4c0 _$E179
// FUNCTION: STBC 0x0047c4d0 _$E178
// FUNCTION: STBC 0x0047c4e0 _$E229
// FUNCTION: STBC 0x0047c4f0 _$E228
// FUNCTION: STBC 0x0047c500 _$E234
// FUNCTION: STBC 0x0047c510 _$E233
// FUNCTION: STBC 0x0047c520 _$E238
// FUNCTION: STBC 0x0047c530 _$E237
// FUNCTION: STBC 0x0047c540 _$E242
// FUNCTION: STBC 0x0047c550 _$E241
// FUNCTION: STBC 0x0047c560 _$E249
// FUNCTION: STBC 0x0047c570 _$E248
// FUNCTION: STBC 0x0047c580 _$E274
// FUNCTION: STBC 0x0047c590 _$E273
// FUNCTION: STBC 0x0047c5a0 _$E278
// FUNCTION: STBC 0x0047c5b0 _$E277
// FUNCTION: STBC 0x0047c5c0 _$E124
// FUNCTION: STBC 0x0047c5d0 _$E123
// FUNCTION: STBC 0x0047c5e0 _$E260
// FUNCTION: STBC 0x0047c5f0 _$E259
// FUNCTION: STBC 0x0047c600 _$E267
// FUNCTION: STBC 0x0047c610 _$E266
// FUNCTION: STBC 0x0047c620 _$E290
// FUNCTION: STBC 0x0047c630 _$E289
// FUNCTION: STBC 0x0047c640 _$E294
// FUNCTION: STBC 0x0047c650 _$E293
// FUNCTION: STBC 0x0047c660 _$E334
// FUNCTION: STBC 0x0047c670 _$E333
// FUNCTION: STBC 0x0047c680 _$E340
// FUNCTION: STBC 0x0047c690 _$E339
// FUNCTION: STBC 0x0047c6a0 _$E282
// FUNCTION: STBC 0x0047c6b0 _$E281
// FUNCTION: STBC 0x0047c6c0 _$E286
// FUNCTION: STBC 0x0047c6d0 _$E285
// FUNCTION: STBC 0x0047c6e0 _$E298
// FUNCTION: STBC 0x0047c6f0 _$E297
// FUNCTION: STBC 0x0047c700 _$E302
// FUNCTION: STBC 0x0047c710 _$E301
// FUNCTION: STBC 0x0047c720 _$E306
// FUNCTION: STBC 0x0047c730 _$E305
// FUNCTION: STBC 0x0047c740 _$E310
// FUNCTION: STBC 0x0047c750 _$E309
// FUNCTION: STBC 0x0047c760 _$E314
// FUNCTION: STBC 0x0047c770 _$E313
// FUNCTION: STBC 0x0047c7b0 _$E318
// FUNCTION: STBC 0x0047c7c0 _$E317
// FUNCTION: STBC 0x0047c7d0 _$E322
// FUNCTION: STBC 0x0047c7e0 _$E321
// FUNCTION: STBC 0x0047c7f0 _$E326
// FUNCTION: STBC 0x0047c800 _$E325
// FUNCTION: STBC 0x0047c810 _$E129
// FUNCTION: STBC 0x0047c820 _$E128
// FUNCTION: STBC 0x0047c830 _$E330
// FUNCTION: STBC 0x0047c840 _$E329
// FUNCTION: STBC 0x0047c850 _$E134
// FUNCTION: STBC 0x0047c860 _$E133
// FUNCTION: STBC 0x0047c870 _$E344
// FUNCTION: STBC 0x0047c880 _$E343
// FUNCTION: STBC 0x0047c890 _$E348
// FUNCTION: STBC 0x0047c8a0 _$E347
// FUNCTION: STBC 0x0047c8b0 _$E352
// FUNCTION: STBC 0x0047c8c0 _$E351
// FUNCTION: STBC 0x0047c8d0 _$E356
// FUNCTION: STBC 0x0047c8e0 _$E355
// FUNCTION: STBC 0x0047c8f0 _$E143
// FUNCTION: STBC 0x0047c900 _$E142
// FUNCTION: STBC 0x0047c910 _$E360
// FUNCTION: STBC 0x0047c920 _$E359
// FUNCTION: STBC 0x0047c930 _$E364
// FUNCTION: STBC 0x0047c940 _$E363
// FUNCTION: STBC 0x0047c950 _$E368
// FUNCTION: STBC 0x0047c960 _$E367
// FUNCTION: STBC 0x0047c970 _$E149
// FUNCTION: STBC 0x0047c980 _$E148
// FUNCTION: STBC 0x0047c990 _$E154
// FUNCTION: STBC 0x0047c9a0 _$E153
// FUNCTION: STBC 0x0047c9b0 _$E184
// FUNCTION: STBC 0x0047c9c0 _$E183
// FUNCTION: STBC 0x0047c9d0 _$E372
// FUNCTION: STBC 0x0047c9e0 _$E371
// FUNCTION: STBC 0x0047c9f0 _$E189
// FUNCTION: STBC 0x0047ca00 _$E188
// FUNCTION: STBC 0x0047ca10 _$E376
// FUNCTION: STBC 0x0047ca20 _$E375
// FUNCTION: STBC 0x0047ca30 _$E380
// FUNCTION: STBC 0x0047ca40 _$E379
// FUNCTION: STBC 0x0047ca50 _$E194
// FUNCTION: STBC 0x0047ca60 _$E193
// FUNCTION: STBC 0x0047ca70 _$E384
// FUNCTION: STBC 0x0047ca80 _$E383
// FUNCTION: STBC 0x0047ca90 _$E159
// FUNCTION: STBC 0x0047caa0 _$E158
// FUNCTION: STBC 0x0047cab0 _$E388
// FUNCTION: STBC 0x0047cac0 _$E387
// FUNCTION: STBC 0x0047cad0 _$E392
// FUNCTION: STBC 0x0047cae0 _$E391
// FUNCTION: STBC 0x0047caf0 _$E199
// FUNCTION: STBC 0x0047cb00 _$E198
// FUNCTION: STBC 0x0047cb10 _$E204
// FUNCTION: STBC 0x0047cb20 _$E203
// FUNCTION: STBC 0x0047cb30 _$E396
// FUNCTION: STBC 0x0047cb40 _$E395
// FUNCTION: STBC 0x0047cb50 _$E400
// FUNCTION: STBC 0x0047cb60 _$E399
// FUNCTION: STBC 0x0047cb70 _$E404
// FUNCTION: STBC 0x0047cb80 _$E403
// FUNCTION: STBC 0x0047cb90 _$E408
// FUNCTION: STBC 0x0047cba0 _$E407
// FUNCTION: STBC 0x0047cbb0 _$E412
// FUNCTION: STBC 0x0047cbc0 _$E411
// FUNCTION: STBC 0x0047cbd0 _$E416
// FUNCTION: STBC 0x0047cbe0 _$E415
// FUNCTION: STBC 0x0047cbf0 _$E420
// FUNCTION: STBC 0x0047cc00 _$E419
// FUNCTION: STBC 0x0047cc10 _$E424
// FUNCTION: STBC 0x0047cc20 _$E423
// FUNCTION: STBC 0x0047cc30 _$E209
// FUNCTION: STBC 0x0047cc40 _$E208
// FUNCTION: STBC 0x0047cc50 _$E428
// FUNCTION: STBC 0x0047cc60 _$E427
// FUNCTION: STBC 0x0047cc70 _$E214
// FUNCTION: STBC 0x0047cc80 _$E213
// FUNCTION: STBC 0x0047cc90 _$E219
// FUNCTION: STBC 0x0047cca0 _$E218
// FUNCTION: STBC 0x0047ccb0 _$E432
// FUNCTION: STBC 0x0047ccc0 _$E431
// FUNCTION: STBC 0x0047ccd0 _$E437
// FUNCTION: STBC 0x0047cce0 _$E436
// FUNCTION: STBC 0x0047ccf0 _$E441
// FUNCTION: STBC 0x0047cd00 _$E440

// DATA: STBC 0x00981f94
ConditionScriptCreator *ConditionScript::creators;

/*
================
ConditionScript::Cast
================
*/
// FUNCTION: STBC 0x0047bd50
ConditionScript *ConditionScript::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( CONDITIONSCRIPT_TYPE ) ) {
		return (ConditionScript *)object;
	}
	return NULL;
}

/*
================
ConditionScript::ConditionScript
================
*/
// FUNCTION: STBC 0x0047bd80
ConditionScript::ConditionScript( void ) : scriptInstance( NULL ), hasActivate( false ), hasDeactivate( false ) {
}

/*
================
ConditionScript::~ConditionScript
================
*/
// FUNCTION: STBC 0x0047bdb0
ConditionScript::~ConditionScript( void ) {
	if ( scriptInstance != NULL ) {
		Py_DECREF( scriptInstance );
		scriptInstance = NULL;
	}
}

/*
================
ConditionScript::SetActive

A failed Activate is printed and not called again; the string of the
instance is still made for a report the release build does not make.
================
*/
// FUNCTION: STBC 0x0047be20
void ConditionScript::SetActive( void ) {
	TGCondition::SetActive();
	if ( hasActivate ) {
		if ( PyEmbed::CallMethod( scriptInstance, "Activate", "", NULL, "()" ) == -1 ) {
			hasActivate = false;
			PyErr_Print();
			PyObject *text = PyObject_Str( scriptInstance );
			Py_DECREF( text );
		}
	}
}

/*
================
ConditionScript::SetInactive
================
*/
// FUNCTION: STBC 0x0047be80
void ConditionScript::SetInactive( void ) {
	TGCondition::SetInactive();
	if ( hasDeactivate ) {
		if ( PyEmbed::CallMethod( scriptInstance, "Deactivate", "", NULL, "()" ) == -1 ) {
			hasDeactivate = false;
			PyErr_Print();
			PyObject *text = PyObject_Str( scriptInstance );
			Py_DECREF( text );
		}
	}
}

/*
================
ConditionScript::SetScriptInstance

The reference held before is not released.
================
*/
// FUNCTION: STBC 0x0047bee0
void ConditionScript::SetScriptInstance( PyObject *instance ) {
	scriptInstance = instance;
	Py_INCREF( scriptInstance );
	hasActivate = PyEmbed::HasAttribute( scriptInstance, "Activate" );
	hasDeactivate = PyEmbed::HasAttribute( scriptInstance, "Deactivate" );
}

/*
================
ConditionScript::WriteToStream
================
*/
// FUNCTION: STBC 0x0047bf20
bool ConditionScript::WriteToStream( TGStream *stream ) {
	if ( !TGCondition::WriteToStream( stream ) ) {
		return false;
	}
	if ( scriptInstance != NULL ) {
		stream->WriteBool( true );
		if ( !PyEmbed::SerializePython( scriptInstance, stream, 0 ) ) {
			return false;
		}
	} else {
		stream->WriteBool( false );
	}
	stream->WriteBool( hasActivate );
	stream->WriteBool( hasDeactivate );
	return true;
}

/*
================
ConditionScript::ReadFromStream
================
*/
// FUNCTION: STBC 0x0047bf90
bool ConditionScript::ReadFromStream( TGStream *stream ) {
	scriptInstance = NULL;
	if ( !TGCondition::ReadFromStream( stream ) ) {
		return false;
	}
	if ( scriptInstance != NULL ) {
		Py_DECREF( scriptInstance );
		scriptInstance = NULL;
	}
	if ( stream->ReadBool() == true ) {
		scriptInstance = PyEmbed::UnserializePython( stream, 0 );
		if ( scriptInstance == NULL ) {
			return false;
		}
	}
	hasActivate = stream->ReadBool();
	hasDeactivate = stream->ReadBool();
	return true;
}

/*
================
ConditionScript_Create
================
*/
// FUNCTION: STBC 0x0047c020
// PARKED: complete; same calls and branches, retail keeps the arguments in EDI and the new condition in the arguments' stack slot (register allocation)
extern "C" PyObject *_wrap_ConditionScript_Create( PyObject *self, PyObject *args ) {
	if ( PyTuple_Size( args ) < 2 || !PyString_Check( PyTuple_GetItem( args, 0 ) ) || !PyString_Check( PyTuple_GetItem( args, 1 ) ) ) {
		PyErr_SetString( PyExc_TypeError, "Bad arguments to ConditionScript_Create(sModule, sClass, ...)" );
		return NULL;
	}

	conditionCreateFunction_t create = ConditionScript::GetCreateFunction( PyString_AsString( PyTuple_GetItem( args, 0 ) ), PyString_AsString( PyTuple_GetItem( args, 1 ) ) );
	if ( create != NULL ) {
		TGObject *condition = create( args );
		if ( condition != NULL ) {
			return condition->VirtualSlot08();
		}
	}

	ConditionScript *condition = new ConditionScript;
	PyObject *scriptArgs = PyTuple_New( PyTuple_Size( args ) - 1 );
	PyObject *shadow = PyEmbed::Unknown6F9920( condition, "ConditionScript" );
	Py_INCREF( shadow );
	PyTuple_SetItem( scriptArgs, 0, shadow );
	for ( int i = 2; i < PyTuple_Size( args ); i++ ) {
		PyObject *arg = PyTuple_GetItem( args, i );
		Py_INCREF( arg );
		PyTuple_SetItem( scriptArgs, i - 1, arg );
	}

	PyObject *moduleName = PyObject_Str( PyTuple_GetItem( args, 0 ) );
	char *module = PyString_AsString( moduleName );
	PyObject *className = PyObject_Str( PyTuple_GetItem( args, 1 ) );
	PyObject *scriptClass = PyEmbed::GetFunction( module, PyString_AsString( className ) );
	Py_DECREF( moduleName );
	Py_DECREF( className );
	if ( scriptClass == NULL ) {
		delete condition;
		return NULL;
	}
	PyObject *instance = NULL;
	if ( PyEmbed::CallObject( scriptClass, "O", &instance, scriptArgs ) == -1 ) {
		delete condition;
		return NULL;
	}
	condition->SetScriptInstance( instance );
	Py_DECREF( instance );
	return shadow;
}

/*
================
ConditionScript_GetConditionScript
================
*/
// FUNCTION: STBC 0x0047c2d0
PyObject *ConditionScript_GetConditionScript( PyObject *self, PyObject *args ) {
	PyObject *result = Py_None;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		ConditionScript *condition;
		PyEmbed::Unknown6F98E0( instance, (void **)&condition, "ConditionScript *" );
		result = condition->GetScriptInstance();
		if ( result == NULL ) {
			result = Py_None;
		}
	}
	Py_INCREF( result );
	return result;
}
