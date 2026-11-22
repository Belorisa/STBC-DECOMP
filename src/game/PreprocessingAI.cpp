/*
===============================================================================

	PreprocessingAI.cpp

===============================================================================
*/

#include <string.h>

#include "AILog.h"
#include "PreprocessingAI.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0048f300 _$E70
// FUNCTION: STBC 0x0048f310 _$E69
// FUNCTION: STBC 0x0048f320 _$E226
// FUNCTION: STBC 0x0048f330 _$E225
// FUNCTION: STBC 0x0048f340 _$E81
// FUNCTION: STBC 0x0048f350 _$E80
// FUNCTION: STBC 0x0048f360 _$E85
// FUNCTION: STBC 0x0048f370 _$E84
// FUNCTION: STBC 0x0048f380 _$E89
// FUNCTION: STBC 0x0048f390 _$E88
// FUNCTION: STBC 0x0048f3a0 _$E93
// FUNCTION: STBC 0x0048f3b0 _$E92
// FUNCTION: STBC 0x0048f3c0 _$E213
// FUNCTION: STBC 0x0048f3d0 _$E212
// FUNCTION: STBC 0x0048f3e0 _$E155
// FUNCTION: STBC 0x0048f3f0 _$E154
// FUNCTION: STBC 0x0048f400 _$E168
// FUNCTION: STBC 0x0048f410 _$E167
// FUNCTION: STBC 0x0048f440 _$E231
// FUNCTION: STBC 0x0048f450 _$E230
// FUNCTION: STBC 0x0048f460 _$E222
// FUNCTION: STBC 0x0048f470 _$E221
// FUNCTION: STBC 0x0048f480 _$E235
// FUNCTION: STBC 0x0048f490 _$E234
// FUNCTION: STBC 0x0048f4a0 _$E242
// FUNCTION: STBC 0x0048f4b0 _$E241
// FUNCTION: STBC 0x0048f500 _$E117
// FUNCTION: STBC 0x0048f510 _$E116
// FUNCTION: STBC 0x0048f720 _$E122
// FUNCTION: STBC 0x0048f730 _$E121
// FUNCTION: STBC 0x0048f760 _$E127
// FUNCTION: STBC 0x0048f770 _$E126
// FUNCTION: STBC 0x0048f800 _$E132
// FUNCTION: STBC 0x0048f810 _$E131
// FUNCTION: STBC 0x0048f880 _$E138
// FUNCTION: STBC 0x0048f890 _$E137
// FUNCTION: STBC 0x0048f8a0 _$E143
// FUNCTION: STBC 0x0048f8b0 _$E142
// FUNCTION: STBC 0x0048f8c0 _$E173
// FUNCTION: STBC 0x0048f8d0 _$E172
// FUNCTION: STBC 0x0048f930 _$E178
// FUNCTION: STBC 0x0048f940 _$E177
// FUNCTION: STBC 0x0048f990 _$E183
// FUNCTION: STBC 0x0048f9a0 _$E182
// FUNCTION: STBC 0x0048f9d0 _$E148
// FUNCTION: STBC 0x0048f9e0 _$E147
// FUNCTION: STBC 0x0048fa30 _$E188
// FUNCTION: STBC 0x0048fa40 _$E187
// FUNCTION: STBC 0x0048fa50 _$E193
// FUNCTION: STBC 0x0048fa60 _$E192
// FUNCTION: STBC 0x0048fb70 _$E198
// FUNCTION: STBC 0x0048fb80 _$E197
// FUNCTION: STBC 0x0048fbb0 _$E203
// FUNCTION: STBC 0x0048fbc0 _$E202
// FUNCTION: STBC 0x0048fbd0 _$E208
// FUNCTION: STBC 0x0048fbe0 _$E207
// FUNCTION: STBC 0x0048fbf0 _$E254
// FUNCTION: STBC 0x0048fc00 _$E253
// FUNCTION: STBC 0x0048fc10 _$E259
// FUNCTION: STBC 0x0048fc20 _$E258
// FUNCTION: STBC 0x0048fc30 _$E263
// FUNCTION: STBC 0x0048fc40 _$E262

// DATA: STBC 0x00982a1c
OptimizedVersion *PreprocessingAI::optimizedVersions;

// FUNCTION: STBC 0x0048e250 _$E266
// FUNCTION: STBC 0x0048e260 _$E265
static AICreateFunctionRegistration preprocessingAIRegistration( AI_TYPE_PREPROCESSING, PreprocessingAI::Create );

/*
================
PreprocessingAI::PreprocessingAI
================
*/
// FUNCTION: STBC 0x0048e2b0 ??0PreprocessingAI@@QAE@PAVPhysicsObjectClass@@PAD@Z
PreprocessingAI::PreprocessingAI( PhysicsObjectClass *object, char *name_ ) : ArtificialIntelligence( object, name_ ) {
	preprocessingInstance = NULL;
	preprocessingMethod = NULL;
	hasActivate = false;
	hasDeactivate = false;
	hasGetNextUpdateTime = false;
	hasReset = false;
	hasGotFocus = false;
	hasLostFocus = false;
	hasGetStatusInfo = false;
	nextUpdateTime = 0.0f;
	forceUpdate = false;
	forcedDormantStatus = FDS_NORMAL;
	preprocessStatus = PS_NORMAL;
	containedAI = NULL;
}

/*
================
PreprocessingAI::GetAIType
================
*/
// FUNCTION: STBC 0x0048e300
int PreprocessingAI::GetAIType( void ) {
	return AI_TYPE_PREPROCESSING;
}

/*
================
PreprocessingAI::~PreprocessingAI

Deletes the contained AI.
================
*/
// FUNCTION: STBC 0x0048e340
// FUNCTION: STBC 0x0048e320 ??_GPreprocessingAI@@UAEPAXI@Z
PreprocessingAI::~PreprocessingAI( void ) {
	if ( preprocessingInstance != NULL ) {
		Py_DECREF( preprocessingInstance );
		preprocessingInstance = NULL;
	}
	if ( preprocessingMethod != NULL ) {
		delete [] preprocessingMethod;
		preprocessingMethod = NULL;
	}
	delete containedAI;
}

/*
================
PreprocessingAI::Cast
================
*/
// FUNCTION: STBC 0x0048e3e0
PreprocessingAI *PreprocessingAI::Cast( ArtificialIntelligence *ai ) {
	if ( ai != NULL && ai->VirtualSlot20( AI_TYPE_PREPROCESSING ) ) {
		return static_cast<PreprocessingAI *>( ai );
	}
	return NULL;
}

/*
================
PreprocessingAI::SetPreprocessingMethod

The script instance gets the AI's shadow instance as "pCodeAI", and its
CodeAISet is called when it has one.
================
*/
// FUNCTION: STBC 0x0048e400
void PreprocessingAI::SetPreprocessingMethod( PyObject *instance, char *method ) {
	if ( preprocessingInstance != NULL ) {
		Py_DECREF( preprocessingInstance );
		preprocessingInstance = NULL;
	}
	preprocessingInstance = instance;
	Py_INCREF( instance );
	if ( preprocessingMethod != NULL ) {
		delete [] preprocessingMethod;
		preprocessingMethod = NULL;
	}
	preprocessingMethod = new char[strlen( method ) + 1];
	strcpy( preprocessingMethod, method );

	hasActivate = PyEmbed::HasAttribute( preprocessingInstance, "Activate" );
	hasDeactivate = PyEmbed::HasAttribute( preprocessingInstance, "Deactivate" );
	hasGetNextUpdateTime = PyEmbed::HasAttribute( preprocessingInstance, "GetNextUpdateTime" );
	hasReset = PyEmbed::HasAttribute( preprocessingInstance, "Reset" );
	hasGotFocus = PyEmbed::HasAttribute( preprocessingInstance, "GotFocus" );
	hasLostFocus = PyEmbed::HasAttribute( preprocessingInstance, "LostFocus" );
	hasGetStatusInfo = PyEmbed::HasAttribute( preprocessingInstance, "GetStatusInfo" );

	if ( PyEmbed::SetAttribute( preprocessingInstance, "pCodeAI", "O", PyEmbed::Unknown6F9920( this, "PreprocessingAI" ) ) == -1 ) {
		PyErr_Print();
	}
	if ( PyEmbed::HasAttribute( preprocessingInstance, "CodeAISet" ) ) {
		if ( PyEmbed::CallMethod( preprocessingInstance, "CodeAISet", NULL, NULL, "()" ) == -1 ) {
			PyErr_Print();
		}
	}
}

/*
================
PreprocessingAI::SetContainedAI
================
*/
// FUNCTION: STBC 0x0048e570
void PreprocessingAI::SetContainedAI( ArtificialIntelligence *ai ) {
	delete containedAI;
	containedAI = ai->VirtualSlot13();
	containedAI->SetParentID( id );
}

/*
================
PreprocessingAI::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x0048e5a0
void PreprocessingAI::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais ) {
	ais.AddTail( this );
	if ( containedAI != NULL ) {
		containedAI->GetAllAIsInTree( ais );
	}
}

/*
================
PreprocessingAI::GetFocusAIs
================
*/
// FUNCTION: STBC 0x0048e610
void PreprocessingAI::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais ) {
	if ( focus ) {
		ais.AddTail( this );
		if ( containedAI->HasFocus() ) {
			containedAI->GetFocusAIs( ais );
		}
	}
}

/*
================
PreprocessingAI::CallExternalFunction

True unless the script method fails.
================
*/
// FUNCTION: STBC 0x0048e680
bool PreprocessingAI::CallExternalFunction( char *functionName, PyObject *arguments ) {
	if ( preprocessingInstance != NULL ) {
		PyObject *function;
		if ( externalFunctions.GetAt( functionName, function ) ) {
			PyObject *nameObject = PyDict_GetItemString( function, "Name" );
			if ( nameObject != NULL ) {
				char *method = PyString_AsString( nameObject );
				if ( PyEmbed::CallMethod( preprocessingInstance, method, "", NULL, "(O)", arguments ) == -1 ) {
					return false;
				}
			}
		}
	}
	return true;
}

/*
================
PreprocessingAI::SetActive
================
*/
// FUNCTION: STBC 0x0048e710
void PreprocessingAI::SetActive( void ) {
	AILog log( "PreprocessingAI(%s)::SetActive", name );
	active = true;
	if ( hasActivate ) {
		if ( PyEmbed::CallMethod( preprocessingInstance, "Activate", "", NULL, "()" ) == -1 ) {
			hasActivate = false;
			PyErr_Print();
			PyObject *description = PyObject_Str( preprocessingInstance );
			Py_DECREF( description );
		}
	}
	if ( containedAI != NULL && !containedAI->IsActive() ) {
		containedAI->SetActive();
	}
}

/*
================
PreprocessingAI::SetInactive
================
*/
// FUNCTION: STBC 0x0048e7d0
void PreprocessingAI::SetInactive( void ) {
	AILog log( "PreprocessingAI(%s)::SetInactive", name );
	active = false;
	if ( hasDeactivate ) {
		if ( PyEmbed::CallMethod( preprocessingInstance, "Deactivate", "", NULL, "()" ) == -1 ) {
			hasDeactivate = false;
			PyErr_Print();
			PyObject *description = PyObject_Str( preprocessingInstance );
			Py_DECREF( description );
		}
	}
	nextUpdateTime = 0.0f;
	if ( containedAI != NULL && containedAI->IsActive() ) {
		containedAI->SetInactive();
	}
}

/*
================
PreprocessingAI::GotFocus

The preprocessing method is called at the next update.
================
*/
// FUNCTION: STBC 0x0048e890
void PreprocessingAI::GotFocus( void ) {
	AILog log( "PreprocessingAI(%s)::GotFocus", name );
	focus = true;
	nextUpdateTime = 0.0f;
	if ( hasGotFocus ) {
		if ( PyEmbed::CallMethod( preprocessingInstance, "GotFocus", "", NULL, "()" ) == -1 ) {
			hasGotFocus = false;
			PyErr_Print();
			PyObject *description = PyObject_Str( preprocessingInstance );
			Py_DECREF( description );
		}
	}
}

/*
================
PreprocessingAI::LostFocus
================
*/
// FUNCTION: STBC 0x0048e950
void PreprocessingAI::LostFocus( void ) {
	AILog log( "PreprocessingAI(%s)::LostFocus", name );
	if ( containedAI != NULL && containedAI->HasFocus() ) {
		containedAI->LostFocus();
	}
	focus = false;
	if ( hasLostFocus ) {
		if ( PyEmbed::CallMethod( preprocessingInstance, "LostFocus", "", NULL, "()" ) == -1 ) {
			hasLostFocus = false;
			PyErr_Print();
			PyObject *description = PyObject_Str( preprocessingInstance );
			Py_DECREF( description );
		}
	}
}

/*
================
PreprocessingAI::IsInterruptable
================
*/
// FUNCTION: STBC 0x0048ea10
bool PreprocessingAI::IsInterruptable( void ) {
	bool result = true;
	result &= interruptable;
	if ( result && active ) {
		result &= containedAI->IsInterruptable();
	}
	return result;
}

/*
================
PreprocessingAI::Update

A contained AI that cannot be interrupted keeps running whatever the
preprocessing says.
================
*/
// FUNCTION: STBC 0x0048ea30
int PreprocessingAI::Update( double time ) {
	AILog log( "PreprocessingAI(%s)::Update", name );
	int result = US_ACTIVE;
	int status = CallPreprocessingMethod( time );
	if ( containedAI != NULL && containedAI->IsActive() && !containedAI->IsInterruptable() ) {
		if ( !containedAI->HasFocus() ) {
			containedAI->GotFocus();
		}
		result = containedAI->Update( time );
	} else {
		switch ( status ) {
		case PS_NORMAL:
			if ( containedAI != NULL ) {
				if ( !containedAI->HasFocus() ) {
					containedAI->GotFocus();
				}
				result = containedAI->Update( time );
			}
			break;
		case PS_SKIP_ACTIVE:
			result = US_ACTIVE;
			break;
		case PS_SKIP_DORMANT:
			result = US_DORMANT;
			break;
		default:
			result = US_DONE;
			break;
		}
	}
	return result;
}

/*
================
PreprocessingAI::VirtualSlot13
================
*/
// FUNCTION: STBC 0x0048eb20
ArtificialIntelligence *PreprocessingAI::VirtualSlot13( void ) {
	if ( preprocessingInstance != NULL && PyInstance_Check( preprocessingInstance ) ) {
		char *className = PyString_AsString( ( (PyInstanceObject *)preprocessingInstance )->in_class->cl_name );
		if ( className != NULL ) {
			for ( OptimizedVersion *entry = optimizedVersions; entry != NULL; entry = entry->next ) {
				if ( strcmp( entry->className, className ) == 0 ) {
					if ( entry->create != NULL ) {
						return entry->create( this );
					}
					break;
				}
			}
		}
	}
	return this;
}

/*
================
PreprocessingAI::Reset
================
*/
// FUNCTION: STBC 0x0048ebb0
void PreprocessingAI::Reset( void ) {
	if ( preprocessingInstance != NULL && hasReset ) {
		if ( PyEmbed::CallMethod( preprocessingInstance, "Reset", "", NULL, "()" ) == -1 ) {
			hasReset = false;
			PyErr_Print();
			PyObject *description = PyObject_Str( preprocessingInstance );
			Py_DECREF( description );
		}
	}
	if ( containedAI != NULL ) {
		containedAI->Reset();
	}
}

/*
================
PreprocessingAI::IsDormant
================
*/
// FUNCTION: STBC 0x0048ec20
// PARKED: logic, calls and blocks match; retail loads the contained AI into ecx where this source reuses esi (this) for it
bool PreprocessingAI::IsDormant( void ) {
	bool dormant = ( preprocessStatus == PS_SKIP_DORMANT ) & !forceUpdate;
	AILog log( "PreprocessingAI(%s)::IsDormant (%s)", name, dormant ? "true" : "maybe" );
	if ( forcedDormantStatus != FDS_NORMAL ) {
		dormant = forcedDormantStatus == FDS_TRUE;
	} else if ( dormant ) {
		if ( active && containedAI != NULL && !containedAI->IsInterruptable() ) {
			dormant = false;
		}
	} else if ( containedAI != NULL && containedAI->IsDormant() ) {
		dormant = true;
	}
	return dormant;
}

/*
================
PreprocessingAI::CallPreprocessingMethod

While the AI tree log is open, the script's status text is logged too.
================
*/
// FUNCTION: STBC 0x0048ece0
int PreprocessingAI::CallPreprocessingMethod( double time ) {
	int status = preprocessStatus;
	if ( forceUpdate || g_kUtopiaModule.GetGameTime() >= nextUpdateTime ) {
		if ( preprocessingInstance != NULL && preprocessingMethod != NULL ) {
			int result;
			if ( PyEmbed::CallMethod( preprocessingInstance, preprocessingMethod, "i", &result, "(f)", time ) != -1 ) {
				status = result;
			} else {
				PyErr_Print();
				PyObject *description = PyObject_Str( preprocessingInstance );
				Py_DECREF( description );
			}

			if ( hasGetNextUpdateTime ) {
				float delay = 0.0f;
				if ( PyEmbed::CallMethod( preprocessingInstance, "GetNextUpdateTime", "f", &delay, "()" ) == -1 ) {
					PyErr_Print();
					PyObject *description = PyObject_Str( preprocessingInstance );
					Py_DECREF( description );
					delay = 0.0f;
					hasGetNextUpdateTime = false;
				}
				nextUpdateTime = g_kUtopiaModule.GetGameTime() + delay;
			}
		}
	}

	if ( hasGetStatusInfo && AILog::IsOpen() ) {
		PyObject *statusInfo = NULL;
		if ( PyEmbed::CallMethod( preprocessingInstance, "GetStatusInfo", "O", &statusInfo, "()" ) != -1 && PyString_Check( statusInfo ) ) {
			AILog( "%s", PyString_AsString( statusInfo ) );
		} else {
			hasGetStatusInfo = false;
			PyErr_Print();
		}
		Py_XDECREF( statusInfo );
	}

	preprocessStatus = status;
	forceUpdate = false;
	return status;
}

/*
================
PreprocessingAI_SetPreprocessingMethod

The AI's shadow instance, the script instance and the method's name.
================
*/
// FUNCTION: STBC 0x0048ee90
PyObject *PreprocessingAI_SetPreprocessingMethod( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	char *method = NULL;
	PyObject *aiInstance = NULL;
	PreprocessingAI *ai;
	// PyArg_ParseTuple returns 0 on failure, never -1, and the AI is not
	// checked.
	if ( PyArg_ParseTuple( args, "OOs", &aiInstance, &instance, &method ) != -1 ) {
		PyEmbed::Unknown6F98E0( aiInstance, (void **)&ai, "PreprocessingAI" );
		ai->SetPreprocessingMethod( instance, method );
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
PreprocessingAI_GetPreprocessingInstance

The AI's script instance, or None.
================
*/
// FUNCTION: STBC 0x0048ef10
PyObject *PreprocessingAI_GetPreprocessingInstance( PyObject *self, PyObject *args ) {
	PyObject *result = Py_None;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		PreprocessingAI *ai;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "PreprocessingAI" );
		result = ai->GetPreprocessingInstance();
		if ( result == NULL ) {
			result = Py_None;
		}
	}
	Py_INCREF( result );
	return result;
}

/*
================
PreprocessingAI::WriteToStream
================
*/
// FUNCTION: STBC 0x0048ef80
bool PreprocessingAI::WriteToStream( TGStream *stream ) {
	if ( !ArtificialIntelligence::WriteToStream( stream ) ) {
		return false;
	}
	if ( preprocessingInstance != NULL ) {
		stream->WriteBool( true );
		if ( !PyEmbed::SerializePython( preprocessingInstance, stream, 0 ) ) {
			return false;
		}
	} else {
		stream->WriteBool( false );
	}
	stream->VirtualSlot11( preprocessingMethod );
	stream->WriteBool( hasActivate );
	stream->WriteBool( hasDeactivate );
	stream->WriteBool( hasGetNextUpdateTime );
	stream->WriteBool( hasReset );
	stream->WriteBool( hasGotFocus );
	stream->WriteBool( hasLostFocus );
	stream->WriteBool( hasGetStatusInfo );
	stream->WriteFloat( nextUpdateTime );
	stream->WriteBool( forceUpdate );
	stream->WriteInt( forcedDormantStatus );
	stream->WriteLong( preprocessStatus );
	if ( containedAI != NULL ) {
		stream->WriteBool( true );
		if ( !containedAI->WriteToStream( stream ) ) {
			return false;
		}
	} else {
		stream->WriteBool( false );
	}
	return true;
}

/*
================
PreprocessingAI::PreprocessingAI
================
*/
// FUNCTION: STBC 0x0048f080 ??0PreprocessingAI@@QAE@PAVTGStream@@PA_N@Z
PreprocessingAI::PreprocessingAI( TGStream *stream, bool *ok ) : ArtificialIntelligence( stream, ok ) {
	preprocessingInstance = NULL;
	preprocessingMethod = NULL;
	hasActivate = false;
	hasDeactivate = false;
	hasGetNextUpdateTime = false;
	hasReset = false;
	hasGotFocus = false;
	hasLostFocus = false;
	nextUpdateTime = 0.0f;
	preprocessStatus = PS_NORMAL;
	containedAI = NULL;
	if ( *ok ) {
		if ( stream->ReadBool() == true ) {
			preprocessingInstance = PyEmbed::UnserializePython( stream, 0 );
			*ok &= preprocessingInstance != NULL;
			if ( !*ok ) {
				return;
			}
		}
		preprocessingMethod = stream->VirtualSlot10( -1 );
		hasActivate = stream->ReadBool();
		hasDeactivate = stream->ReadBool();
		hasGetNextUpdateTime = stream->ReadBool();
		hasReset = stream->ReadBool();
		hasGotFocus = stream->ReadBool();
		hasLostFocus = stream->ReadBool();
		hasGetStatusInfo = stream->ReadBool();
		nextUpdateTime = stream->ReadFloat();
		forceUpdate = stream->ReadBool();
		forcedDormantStatus = stream->ReadInt();
		preprocessStatus = stream->ReadLong();
		if ( stream->ReadBool() == true ) {
			containedAI = CreateFromStream( stream );
			if ( containedAI == NULL ) {
				*ok = false;
			}
		}
	}
}

/*
================
PreprocessingAI::Create
================
*/
// FUNCTION: STBC 0x0048f1d0
ArtificialIntelligence *PreprocessingAI::Create( TGStream *stream ) {
	bool ok = true;
	PreprocessingAI *ai = new PreprocessingAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}

/*
================
PreprocessingAI::FixUpPointers
================
*/
// FUNCTION: STBC 0x0048f250
bool PreprocessingAI::FixUpPointers( void ) {
	if ( containedAI != NULL && !containedAI->FixUpPointers() ) {
		return false;
	}
	return true;
}

/*
================
PreprocessingAI::PostFixUp
================
*/
// FUNCTION: STBC 0x0048f270
void PreprocessingAI::PostFixUp( void ) {
	if ( containedAI != NULL ) {
		containedAI->PostFixUp();
	}
}
