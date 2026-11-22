/*
===============================================================================

	PlainAI.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "AILog.h"
#include "PlainAI.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0048e000 _$E472
// FUNCTION: STBC 0x0048e020 _$E471

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0048d880 _$E32
// FUNCTION: STBC 0x0048d890 _$E29
// FUNCTION: STBC 0x0048d8a0 _$E31
// FUNCTION: STBC 0x0048d8b0 _$E30
// FUNCTION: STBC 0x0048d8c0 _$E38
// FUNCTION: STBC 0x0048d8d0 _$E35
// FUNCTION: STBC 0x0048d8e0 _$E37
// FUNCTION: STBC 0x0048d8f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0048d900 _$E89
// FUNCTION: STBC 0x0048d910 _$E88
// FUNCTION: STBC 0x0048d920 _$E245
// FUNCTION: STBC 0x0048d930 _$E244
// FUNCTION: STBC 0x0048d940 _$E100
// FUNCTION: STBC 0x0048d950 _$E99
// FUNCTION: STBC 0x0048d960 _$E104
// FUNCTION: STBC 0x0048d970 _$E103
// FUNCTION: STBC 0x0048d980 _$E108
// FUNCTION: STBC 0x0048d990 _$E107
// FUNCTION: STBC 0x0048d9a0 _$E112
// FUNCTION: STBC 0x0048d9b0 _$E111
// FUNCTION: STBC 0x0048d9c0 _$E232
// FUNCTION: STBC 0x0048d9d0 _$E231
// FUNCTION: STBC 0x0048d9e0 _$E174
// FUNCTION: STBC 0x0048d9f0 _$E173
// FUNCTION: STBC 0x0048da00 _$E187
// FUNCTION: STBC 0x0048da10 _$E186
// FUNCTION: STBC 0x0048da20 _$E237
// FUNCTION: STBC 0x0048da30 _$E236
// FUNCTION: STBC 0x0048da40 _$E250
// FUNCTION: STBC 0x0048da50 _$E249
// FUNCTION: STBC 0x0048da60 _$E241
// FUNCTION: STBC 0x0048da70 _$E240
// FUNCTION: STBC 0x0048da80 _$E254
// FUNCTION: STBC 0x0048da90 _$E253
// FUNCTION: STBC 0x0048daa0 _$E261
// FUNCTION: STBC 0x0048dab0 _$E260
// FUNCTION: STBC 0x0048dac0 _$E294
// FUNCTION: STBC 0x0048dad0 _$E293
// FUNCTION: STBC 0x0048dae0 _$E298
// FUNCTION: STBC 0x0048daf0 _$E297
// FUNCTION: STBC 0x0048db00 _$E136
// FUNCTION: STBC 0x0048db10 _$E135
// FUNCTION: STBC 0x0048db20 _$E280
// FUNCTION: STBC 0x0048db30 _$E279
// FUNCTION: STBC 0x0048db40 _$E287
// FUNCTION: STBC 0x0048db50 _$E286
// FUNCTION: STBC 0x0048db60 _$E310
// FUNCTION: STBC 0x0048db70 _$E309
// FUNCTION: STBC 0x0048db80 _$E314
// FUNCTION: STBC 0x0048db90 _$E313
// FUNCTION: STBC 0x0048dba0 _$E354
// FUNCTION: STBC 0x0048dbb0 _$E353
// FUNCTION: STBC 0x0048dbc0 _$E360
// FUNCTION: STBC 0x0048dbd0 _$E359
// FUNCTION: STBC 0x0048dbe0 _$E302
// FUNCTION: STBC 0x0048dbf0 _$E301
// FUNCTION: STBC 0x0048dc00 _$E306
// FUNCTION: STBC 0x0048dc10 _$E305
// FUNCTION: STBC 0x0048dc20 _$E318
// FUNCTION: STBC 0x0048dc30 _$E317
// FUNCTION: STBC 0x0048dc40 _$E322
// FUNCTION: STBC 0x0048dc50 _$E321
// FUNCTION: STBC 0x0048dc60 _$E326
// FUNCTION: STBC 0x0048dc70 _$E325
// FUNCTION: STBC 0x0048dc80 _$E330
// FUNCTION: STBC 0x0048dc90 _$E329
// FUNCTION: STBC 0x0048dca0 _$E334
// FUNCTION: STBC 0x0048dcb0 _$E333
// FUNCTION: STBC 0x0048dcc0 _$E338
// FUNCTION: STBC 0x0048dcd0 _$E337
// FUNCTION: STBC 0x0048dce0 _$E342
// FUNCTION: STBC 0x0048dcf0 _$E341
// FUNCTION: STBC 0x0048dd00 _$E346
// FUNCTION: STBC 0x0048dd10 _$E345
// FUNCTION: STBC 0x0048dd20 _$E141
// FUNCTION: STBC 0x0048dd30 _$E140
// FUNCTION: STBC 0x0048dd40 _$E350
// FUNCTION: STBC 0x0048dd50 _$E349
// FUNCTION: STBC 0x0048dd60 _$E146
// FUNCTION: STBC 0x0048dd70 _$E145
// FUNCTION: STBC 0x0048dd80 _$E364
// FUNCTION: STBC 0x0048dd90 _$E363
// FUNCTION: STBC 0x0048dda0 _$E368
// FUNCTION: STBC 0x0048ddb0 _$E367
// FUNCTION: STBC 0x0048ddc0 _$E372
// FUNCTION: STBC 0x0048ddd0 _$E371
// FUNCTION: STBC 0x0048dde0 _$E376
// FUNCTION: STBC 0x0048ddf0 _$E375
// FUNCTION: STBC 0x0048de00 _$E151
// FUNCTION: STBC 0x0048de10 _$E150
// FUNCTION: STBC 0x0048de20 _$E380
// FUNCTION: STBC 0x0048de30 _$E379
// FUNCTION: STBC 0x0048de40 _$E384
// FUNCTION: STBC 0x0048de50 _$E383
// FUNCTION: STBC 0x0048de60 _$E388
// FUNCTION: STBC 0x0048de70 _$E387
// FUNCTION: STBC 0x0048de80 _$E157
// FUNCTION: STBC 0x0048de90 _$E156
// FUNCTION: STBC 0x0048dea0 _$E162
// FUNCTION: STBC 0x0048deb0 _$E161
// FUNCTION: STBC 0x0048dec0 _$E192
// FUNCTION: STBC 0x0048ded0 _$E191
// FUNCTION: STBC 0x0048dee0 _$E392
// FUNCTION: STBC 0x0048def0 _$E391
// FUNCTION: STBC 0x0048df00 _$E197
// FUNCTION: STBC 0x0048df10 _$E196
// FUNCTION: STBC 0x0048df20 _$E396
// FUNCTION: STBC 0x0048df30 _$E395
// FUNCTION: STBC 0x0048df40 _$E400
// FUNCTION: STBC 0x0048df50 _$E399
// FUNCTION: STBC 0x0048df60 _$E202
// FUNCTION: STBC 0x0048df70 _$E201
// FUNCTION: STBC 0x0048df80 _$E404
// FUNCTION: STBC 0x0048df90 _$E403
// FUNCTION: STBC 0x0048dfa0 _$E167
// FUNCTION: STBC 0x0048dfb0 _$E166
// FUNCTION: STBC 0x0048dfc0 _$E408
// FUNCTION: STBC 0x0048dfd0 _$E407
// FUNCTION: STBC 0x0048dfe0 _$E412
// FUNCTION: STBC 0x0048dff0 _$E411
// FUNCTION: STBC 0x0048e030 _$E207
// FUNCTION: STBC 0x0048e040 _$E206
// FUNCTION: STBC 0x0048e050 _$E212
// FUNCTION: STBC 0x0048e060 _$E211
// FUNCTION: STBC 0x0048e070 _$E416
// FUNCTION: STBC 0x0048e080 _$E415
// FUNCTION: STBC 0x0048e090 _$E420
// FUNCTION: STBC 0x0048e0a0 _$E419
// FUNCTION: STBC 0x0048e0b0 _$E424
// FUNCTION: STBC 0x0048e0c0 _$E423
// FUNCTION: STBC 0x0048e0d0 _$E428
// FUNCTION: STBC 0x0048e0e0 _$E427
// FUNCTION: STBC 0x0048e0f0 _$E432
// FUNCTION: STBC 0x0048e100 _$E431
// FUNCTION: STBC 0x0048e110 _$E436
// FUNCTION: STBC 0x0048e120 _$E435
// FUNCTION: STBC 0x0048e130 _$E440
// FUNCTION: STBC 0x0048e140 _$E439
// FUNCTION: STBC 0x0048e150 _$E444
// FUNCTION: STBC 0x0048e160 _$E443
// FUNCTION: STBC 0x0048e170 _$E217
// FUNCTION: STBC 0x0048e180 _$E216
// FUNCTION: STBC 0x0048e190 _$E448
// FUNCTION: STBC 0x0048e1a0 _$E447
// FUNCTION: STBC 0x0048e1b0 _$E222
// FUNCTION: STBC 0x0048e1c0 _$E221
// FUNCTION: STBC 0x0048e1d0 _$E227
// FUNCTION: STBC 0x0048e1e0 _$E226
// FUNCTION: STBC 0x0048e1f0 _$E452
// FUNCTION: STBC 0x0048e200 _$E451
// FUNCTION: STBC 0x0048e210 _$E457
// FUNCTION: STBC 0x0048e220 _$E456
// FUNCTION: STBC 0x0048e230 _$E461
// FUNCTION: STBC 0x0048e240 _$E460

// DATA: STBC 0x008dba58
char *PlainAI::updateMethod = "Update";

// DATA: STBC 0x008dba5c
char *PlainAI::getNextUpdateTimeMethod = "GetNextUpdateTime";

// DATA: STBC 0x008dba60
char *PlainAI::activateMethod = "Activate";

// DATA: STBC 0x008dba64
char *PlainAI::deactivateMethod = "Deactivate";

// DATA: STBC 0x008dba68
char *PlainAI::resetMethod = "Reset";

// DATA: STBC 0x008dba6c
char *PlainAI::gotFocusMethod = "GotFocus";

// DATA: STBC 0x008dba70
char *PlainAI::lostFocusMethod = "LostFocus";

// DATA: STBC 0x008dba74
char *PlainAI::registerExternalFunctionsMethod = "RegisterExternalFunctions";

// DATA: STBC 0x008dba78
char *PlainAI::getStatusInfoMethod = "GetStatusInfo";

// FUNCTION: STBC 0x0048cbe0 _$E464
// FUNCTION: STBC 0x0048cbf0 _$E463
static AICreateFunctionRegistration plainAIRegistration( AI_TYPE_PLAIN, PlainAI::Create );

/*
================
PlainAI::PlainAI
================
*/
// FUNCTION: STBC 0x0048cc40 ??0PlainAI@@QAE@PAVPhysicsObjectClass@@PAD@Z
PlainAI::PlainAI( PhysicsObjectClass *object, char *name_ ) : ArtificialIntelligence( object, name_ ),
	scriptInstance( NULL ), scriptModule( NULL ), status( US_DONE ), nextUpdateTime( 0.0f ) {
}

/*
================
PlainAI::VirtualSlot15
================
*/
// FUNCTION: STBC 0x0048cc80
PyObject *PlainAI::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "PlainAI" );
}

/*
================
PlainAI::GetAIType
================
*/
// FUNCTION: STBC 0x0048cc90
int PlainAI::GetAIType( void ) {
	return AI_TYPE_PLAIN;
}

/*
================
PlainAI::VirtualSlot20
================
*/
// FUNCTION: STBC 0x0048cca0
bool PlainAI::VirtualSlot20( int type ) {
	return type == AI_TYPE_PLAIN ? true : false;
}

/*
================
PlainAI::~PlainAI
================
*/
// FUNCTION: STBC 0x0048ccd0
// FUNCTION: STBC 0x0048ccb0 ??_GPlainAI@@UAEPAXI@Z
PlainAI::~PlainAI( void ) {
	Py_XDECREF( scriptInstance );
	if ( scriptModule != NULL ) {
		delete [] scriptModule;
	}
}

/*
================
PlainAI::SetScriptModule

The instance is made by calling the module's class of the same name
with the AI's shadow instance.
================
*/
// FUNCTION: STBC 0x0048cd50
void PlainAI::SetScriptModule( char *module ) {
	if ( module == NULL ) {
		return;
	}
	scriptModule = new char[strlen( module ) + strlen( "AI.PlainAI." ) + 1];
	strcpy( scriptModule, "AI.PlainAI." );
	strcat( scriptModule, module );
	if ( PyEmbed::GetModule( scriptModule, false ) == NULL ) {
		PyErr_Print();
	}

	PyObject *instance = NULL;
	PyObject *self = PyEmbed::Unknown6F9920( this, "PlainAI" );
	int result = PyEmbed::CallFunction( scriptModule, module, "O", &instance, "(O)", self );
	Py_DECREF( self );
	if ( result == -1 ) {
		PyErr_Print();
	}
	scriptInstance = instance;
	FindScriptMethods();
}

/*
================
PlainAI::SetScriptInstance
================
*/
// FUNCTION: STBC 0x0048ce60
void PlainAI::SetScriptInstance( PyObject *instance ) {
	scriptInstance = instance;
	Py_INCREF( instance );
	FindScriptMethods();
}

/*
================
PlainAI::FindScriptMethods
================
*/
// FUNCTION: STBC 0x0048ce80
void PlainAI::FindScriptMethods( void ) {
	hasActivate = PyObject_HasAttrString( scriptInstance, activateMethod ) != 0;
	hasDeactivate = PyObject_HasAttrString( scriptInstance, deactivateMethod ) != 0;
	hasGotFocus = PyObject_HasAttrString( scriptInstance, gotFocusMethod ) != 0;
	hasLostFocus = PyObject_HasAttrString( scriptInstance, lostFocusMethod ) != 0;
	hasReset = PyObject_HasAttrString( scriptInstance, resetMethod ) != 0;
	hasGetStatusInfo = PyObject_HasAttrString( scriptInstance, getStatusInfoMethod ) != 0;
}

/*
================
PlainAI_SetScriptInstance

The AI's shadow instance and the script instance.
================
*/
// FUNCTION: STBC 0x0048cf20
PyObject *PlainAI_SetScriptInstance( PyObject *self, PyObject *args ) {
	PyObject *instance;
	PyObject *scriptInstance = NULL;
	PlainAI *ai;
	// PyArg_ParseTuple returns 0 on failure, never -1, and the AI is not
	// checked: a bad call goes on with whatever the arguments held.
	if ( PyArg_ParseTuple( args, "OO", &instance, &scriptInstance ) != -1 ) {
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "PlainAI *" );
		ai->SetScriptInstance( scriptInstance );
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
PlainAI::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x0048cf90
void PlainAI::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais ) {
	ais.AddTail( this );
}

/*
================
PlainAI::GetFocusAIs
================
*/
// FUNCTION: STBC 0x0048d000
void PlainAI::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais ) {
	if ( focus ) {
		ais.AddTail( this );
	}
}

/*
================
PlainAI::CallExternalFunction
================
*/
// FUNCTION: STBC 0x0048d070
bool PlainAI::CallExternalFunction( char *functionName, PyObject *arguments ) {
	PyObject *function;
	if ( externalFunctions.GetAt( functionName, function ) ) {
		PyObject *nameObject = PyDict_GetItemString( function, "Name" );
		if ( nameObject != NULL ) {
			char *method = PyString_AsString( nameObject );
			if ( PyEmbed::CallMethod( scriptInstance, method, "", NULL, "(O)", arguments ) != -1 ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
PlainAI::Update

The script is updated when its time has come; the status it returned
stands in between. While the AI tree log is open, the script's status
text is logged too.
================
*/
// FUNCTION: STBC 0x0048d100
int PlainAI::Update( double time ) {
	AILog log( "PlainAI(%s)::Update", name );
	int newStatus = status;
	if ( g_kUtopiaModule.GetGameTime() >= nextUpdateTime ) {
		newStatus = CallScriptUpdate();
		if ( newStatus != US_DONE ) {
			nextUpdateTime = g_kUtopiaModule.GetGameTime() + GetNextUpdateTime();
		}
	}

	if ( hasGetStatusInfo && AILog::IsOpen() ) {
		PyObject *statusInfo = NULL;
		if ( PyEmbed::CallMethod( scriptInstance, getStatusInfoMethod, "O", &statusInfo, "()" ) != -1 && PyString_Check( statusInfo ) ) {
			AILog( "%s", PyString_AsString( statusInfo ) );
		} else {
			hasGetStatusInfo = false;
			PyErr_Print();
		}
		Py_XDECREF( statusInfo );
	}

	status = newStatus;
	return newStatus;
}

/*
================
PlainAI::Reset
================
*/
// FUNCTION: STBC 0x0048d250
void PlainAI::Reset( void ) {
	if ( hasReset ) {
		if ( PyEmbed::CallMethod( scriptInstance, resetMethod, "", NULL, "()" ) == -1 ) {
			PyErr_Print();
			hasReset = false;
		}
	}
	nextUpdateTime = 0.0f;
}

/*
================
PlainAI::IsDormant
================
*/
// FUNCTION: STBC 0x0048d290
bool PlainAI::IsDormant( void ) {
	AILog log( "PlainAI(%s)::IsDormant, (false)", name );
	return false;
}

/*
================
PlainAI::CallScriptUpdate

The script's string form is made and dropped after a failure.
================
*/
// FUNCTION: STBC 0x0048d2c0
int PlainAI::CallScriptUpdate( void ) {
	if ( scriptInstance != NULL ) {
		int result;
		if ( PyEmbed::CallMethod( scriptInstance, updateMethod, "i", &result, "()" ) == 0 ) {
			return result;
		}
		PyErr_Print();
		PyObject *description = PyObject_Str( scriptInstance );
		Py_DECREF( description );
	}
	return US_DONE;
}

/*
================
PlainAI::GetNextUpdateTime
================
*/
// FUNCTION: STBC 0x0048d320
float PlainAI::GetNextUpdateTime( void ) {
	float delay;
	if ( PyEmbed::CallMethod( scriptInstance, getNextUpdateTimeMethod, "f", &delay, "()" ) == -1 ) {
		PyErr_Print();
		return 0.0f;
	}
	return delay;
}

/*
================
PlainAI::SetActive
================
*/
// FUNCTION: STBC 0x0048d360
void PlainAI::SetActive( void ) {
	AILog log( "PlainAI(%s)::SetActive", name );
	active = true;
	if ( scriptInstance != NULL && hasActivate ) {
		if ( PyEmbed::CallMethod( scriptInstance, activateMethod, NULL, NULL, "()" ) == -1 ) {
			PyErr_Print();
			hasActivate = false;
		}
	}
}

/*
================
PlainAI::SetInactive
================
*/
// FUNCTION: STBC 0x0048d400
void PlainAI::SetInactive( void ) {
	AILog log( "PlainAI(%s)::SetInactive", name );
	active = false;
	if ( scriptInstance != NULL && hasDeactivate ) {
		if ( PyEmbed::CallMethod( scriptInstance, deactivateMethod, NULL, NULL, "()" ) == -1 ) {
			PyErr_Print();
			hasDeactivate = false;
		}
	}
}

/*
================
PlainAI::GotFocus

The script is updated at once.
================
*/
// FUNCTION: STBC 0x0048d4a0
void PlainAI::GotFocus( void ) {
	AILog log( "PlainAI(%s)::GotFocus", name );
	focus = true;
	nextUpdateTime = g_kUtopiaModule.GetGameTime();
	if ( scriptInstance != NULL && hasGotFocus ) {
		if ( PyEmbed::CallMethod( scriptInstance, gotFocusMethod, NULL, NULL, "()" ) == -1 ) {
			PyErr_Print();
			hasGotFocus = false;
		}
	}
}

/*
================
PlainAI::LostFocus
================
*/
// FUNCTION: STBC 0x0048d540
void PlainAI::LostFocus( void ) {
	AILog log( "PlainAI(%s)::LostFocus", name );
	focus = false;
	if ( scriptInstance != NULL && hasLostFocus ) {
		if ( PyEmbed::CallMethod( scriptInstance, lostFocusMethod, NULL, NULL, "()" ) == -1 ) {
			PyErr_Print();
			hasLostFocus = false;
		}
	}
}

/*
================
PlainAI_GetScriptInstance

The script instance of the AI, or None.
================
*/
// FUNCTION: STBC 0x0048d5e0
PyObject *PlainAI_GetScriptInstance( PyObject *self, PyObject *args ) {
	PyObject *result = Py_None;
	PyObject *instance = NULL;
	PyArg_ParseTuple( args, "O", &instance );
	PlainAI *ai = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&ai, "PlainAI *" );
	if ( ai != NULL && ai->GetScriptInstance() != NULL ) {
		result = ai->GetScriptInstance();
	}
	Py_INCREF( result );
	return result;
}

/*
================
PlainAI::WriteToStream
================
*/
// FUNCTION: STBC 0x0048d650
bool PlainAI::WriteToStream( TGStream *stream ) {
	if ( !ArtificialIntelligence::WriteToStream( stream ) ) {
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
	stream->VirtualSlot11( scriptModule );
	stream->WriteLong( status );
	stream->WriteBool( hasActivate );
	stream->WriteBool( hasDeactivate );
	stream->WriteBool( hasGotFocus );
	stream->WriteBool( hasLostFocus );
	stream->WriteBool( hasReset );
	stream->WriteBool( hasGetStatusInfo );
	stream->WriteFloat( nextUpdateTime );
	return true;
}

/*
================
PlainAI::PlainAI
================
*/
// FUNCTION: STBC 0x0048d710 ??0PlainAI@@QAE@PAVTGStream@@PA_N@Z
PlainAI::PlainAI( TGStream *stream, bool *ok ) : ArtificialIntelligence( stream, ok ),
	scriptInstance( NULL ), scriptModule( NULL ), status( US_DONE ), nextUpdateTime( 0.0f ) {
	if ( *ok ) {
		if ( stream->ReadBool() ) {
			scriptInstance = PyEmbed::UnserializePython( stream, 0 );
			*ok &= scriptInstance != NULL;
			if ( !*ok ) {
				return;
			}
		}
		scriptModule = stream->VirtualSlot10( -1 );
		status = stream->ReadLong();
		hasActivate = stream->ReadBool();
		hasDeactivate = stream->ReadBool();
		hasGotFocus = stream->ReadBool();
		hasLostFocus = stream->ReadBool();
		hasReset = stream->ReadBool();
		hasGetStatusInfo = stream->ReadBool();
		nextUpdateTime = stream->ReadFloat();
	}
}

/*
================
PlainAI::Create
================
*/
// FUNCTION: STBC 0x0048d800
ArtificialIntelligence *PlainAI::Create( TGStream *stream ) {
	bool ok = true;
	PlainAI *ai = new PlainAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}
