/*
===============================================================================

	ScriptObject.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "ScriptObject.h"
#include "PyEmbed.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0040a9f0 _$E466
// FUNCTION: STBC 0x0040aa10 _$E465

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0040a570 _$E32
// FUNCTION: STBC 0x0040a580 _$E29
// FUNCTION: STBC 0x0040a590 _$E31
// FUNCTION: STBC 0x0040a5a0 _$E30
// FUNCTION: STBC 0x0040a5b0 _$E38
// FUNCTION: STBC 0x0040a5c0 _$E35
// FUNCTION: STBC 0x0040a5d0 _$E37
// FUNCTION: STBC 0x0040a5e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0040a5f0 _$E88
// FUNCTION: STBC 0x0040a600 _$E87
// FUNCTION: STBC 0x0040a610 _$E248
// FUNCTION: STBC 0x0040a620 _$E247
// FUNCTION: STBC 0x0040a630 _$E99
// FUNCTION: STBC 0x0040a640 _$E98
// FUNCTION: STBC 0x0040a650 _$E103
// FUNCTION: STBC 0x0040a660 _$E102
// FUNCTION: STBC 0x0040a670 _$E107
// FUNCTION: STBC 0x0040a680 _$E106
// FUNCTION: STBC 0x0040a690 _$E111
// FUNCTION: STBC 0x0040a6a0 _$E110
// FUNCTION: STBC 0x0040a6b0 _$E228
// FUNCTION: STBC 0x0040a6c0 _$E227
// FUNCTION: STBC 0x0040a6d0 _$E170
// FUNCTION: STBC 0x0040a6e0 _$E169
// FUNCTION: STBC 0x0040a6f0 _$E183
// FUNCTION: STBC 0x0040a700 _$E182
// FUNCTION: STBC 0x0040a710 _$E233
// FUNCTION: STBC 0x0040a720 _$E232
// FUNCTION: STBC 0x0040a730 _$E253
// FUNCTION: STBC 0x0040a740 _$E252
// FUNCTION: STBC 0x0040a750 _$E237
// FUNCTION: STBC 0x0040a760 _$E236
// FUNCTION: STBC 0x0040a770 _$E257
// FUNCTION: STBC 0x0040a780 _$E256
// FUNCTION: STBC 0x0040a790 _$E264
// FUNCTION: STBC 0x0040a7a0 _$E263
// FUNCTION: STBC 0x0040a7b0 _$E289
// FUNCTION: STBC 0x0040a7c0 _$E288
// FUNCTION: STBC 0x0040a7d0 _$E293
// FUNCTION: STBC 0x0040a7e0 _$E292
// FUNCTION: STBC 0x0040a7f0 _$E128
// FUNCTION: STBC 0x0040a800 _$E127
// FUNCTION: STBC 0x0040a810 _$E275
// FUNCTION: STBC 0x0040a820 _$E274
// FUNCTION: STBC 0x0040a830 _$E282
// FUNCTION: STBC 0x0040a840 _$E281
// FUNCTION: STBC 0x0040a850 _$E305
// FUNCTION: STBC 0x0040a860 _$E304
// FUNCTION: STBC 0x0040a870 _$E309
// FUNCTION: STBC 0x0040a880 _$E308
// FUNCTION: STBC 0x0040a890 _$E349
// FUNCTION: STBC 0x0040a8a0 _$E348
// FUNCTION: STBC 0x0040a8b0 _$E355
// FUNCTION: STBC 0x0040a8c0 _$E354
// FUNCTION: STBC 0x0040a8d0 _$E297
// FUNCTION: STBC 0x0040a8e0 _$E296
// FUNCTION: STBC 0x0040a8f0 _$E301
// FUNCTION: STBC 0x0040a900 _$E300
// FUNCTION: STBC 0x0040a910 _$E313
// FUNCTION: STBC 0x0040a920 _$E312
// FUNCTION: STBC 0x0040a930 _$E317
// FUNCTION: STBC 0x0040a940 _$E316
// FUNCTION: STBC 0x0040a950 _$E321
// FUNCTION: STBC 0x0040a960 _$E320
// FUNCTION: STBC 0x0040a970 _$E325
// FUNCTION: STBC 0x0040a980 _$E324
// FUNCTION: STBC 0x0040a990 _$E329
// FUNCTION: STBC 0x0040a9a0 _$E328
// FUNCTION: STBC 0x0040a9b0 _$E333
// FUNCTION: STBC 0x0040a9c0 _$E332
// FUNCTION: STBC 0x0040a9d0 _$E337
// FUNCTION: STBC 0x0040a9e0 _$E336
// FUNCTION: STBC 0x0040aa20 _$E341
// FUNCTION: STBC 0x0040aa30 _$E340
// FUNCTION: STBC 0x0040aa40 _$E133
// FUNCTION: STBC 0x0040aa50 _$E132
// FUNCTION: STBC 0x0040aa60 _$E345
// FUNCTION: STBC 0x0040aa70 _$E344
// FUNCTION: STBC 0x0040aa80 _$E138
// FUNCTION: STBC 0x0040aa90 _$E137
// FUNCTION: STBC 0x0040aaa0 _$E359
// FUNCTION: STBC 0x0040aab0 _$E358
// FUNCTION: STBC 0x0040aac0 _$E363
// FUNCTION: STBC 0x0040aad0 _$E362
// FUNCTION: STBC 0x0040aae0 _$E367
// FUNCTION: STBC 0x0040aaf0 _$E366
// FUNCTION: STBC 0x0040ab00 _$E371
// FUNCTION: STBC 0x0040ab10 _$E370
// FUNCTION: STBC 0x0040ab20 _$E147
// FUNCTION: STBC 0x0040ab30 _$E146
// FUNCTION: STBC 0x0040ab40 _$E375
// FUNCTION: STBC 0x0040ab50 _$E374
// FUNCTION: STBC 0x0040ab60 _$E379
// FUNCTION: STBC 0x0040ab70 _$E378
// FUNCTION: STBC 0x0040ab80 _$E383
// FUNCTION: STBC 0x0040ab90 _$E382
// FUNCTION: STBC 0x0040aba0 _$E153
// FUNCTION: STBC 0x0040abb0 _$E152
// FUNCTION: STBC 0x0040abc0 _$E158
// FUNCTION: STBC 0x0040abd0 _$E157
// FUNCTION: STBC 0x0040abe0 _$E188
// FUNCTION: STBC 0x0040abf0 _$E187
// FUNCTION: STBC 0x0040ac00 _$E387
// FUNCTION: STBC 0x0040ac10 _$E386
// FUNCTION: STBC 0x0040ac20 _$E193
// FUNCTION: STBC 0x0040ac30 _$E192
// FUNCTION: STBC 0x0040ac40 _$E391
// FUNCTION: STBC 0x0040ac50 _$E390
// FUNCTION: STBC 0x0040ac60 _$E395
// FUNCTION: STBC 0x0040ac70 _$E394
// FUNCTION: STBC 0x0040ac80 _$E198
// FUNCTION: STBC 0x0040ac90 _$E197
// FUNCTION: STBC 0x0040aca0 _$E399
// FUNCTION: STBC 0x0040acb0 _$E398
// FUNCTION: STBC 0x0040acc0 _$E163
// FUNCTION: STBC 0x0040acd0 _$E162
// FUNCTION: STBC 0x0040ace0 _$E403
// FUNCTION: STBC 0x0040acf0 _$E402
// FUNCTION: STBC 0x0040ad00 _$E407
// FUNCTION: STBC 0x0040ad10 _$E406
// FUNCTION: STBC 0x0040ad20 _$E203
// FUNCTION: STBC 0x0040ad30 _$E202
// FUNCTION: STBC 0x0040ad40 _$E208
// FUNCTION: STBC 0x0040ad50 _$E207
// FUNCTION: STBC 0x0040ad60 _$E411
// FUNCTION: STBC 0x0040ad70 _$E410
// FUNCTION: STBC 0x0040ad80 _$E415
// FUNCTION: STBC 0x0040ad90 _$E414
// FUNCTION: STBC 0x0040ada0 _$E419
// FUNCTION: STBC 0x0040adb0 _$E418
// FUNCTION: STBC 0x0040adc0 _$E423
// FUNCTION: STBC 0x0040add0 _$E422
// FUNCTION: STBC 0x0040ade0 _$E427
// FUNCTION: STBC 0x0040adf0 _$E426
// FUNCTION: STBC 0x0040ae00 _$E431
// FUNCTION: STBC 0x0040ae10 _$E430
// FUNCTION: STBC 0x0040ae20 _$E435
// FUNCTION: STBC 0x0040ae30 _$E434
// FUNCTION: STBC 0x0040ae40 _$E439
// FUNCTION: STBC 0x0040ae50 _$E438
// FUNCTION: STBC 0x0040ae60 _$E213
// FUNCTION: STBC 0x0040ae70 _$E212
// FUNCTION: STBC 0x0040ae80 _$E443
// FUNCTION: STBC 0x0040ae90 _$E442
// FUNCTION: STBC 0x0040aea0 _$E218
// FUNCTION: STBC 0x0040aeb0 _$E217
// FUNCTION: STBC 0x0040aec0 _$E223
// FUNCTION: STBC 0x0040aed0 _$E222
// FUNCTION: STBC 0x0040aee0 _$E447
// FUNCTION: STBC 0x0040aef0 _$E446
// FUNCTION: STBC 0x0040af00 _$E452
// FUNCTION: STBC 0x0040af10 _$E451
// FUNCTION: STBC 0x0040af20 _$E241
// FUNCTION: STBC 0x0040af30 _$E240

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00409cd0 _$E458
// FUNCTION: STBC 0x00409ce0 _$E454
// FUNCTION: STBC 0x00409cf0 _$E457
// FUNCTION: STBC 0x00409d00 _$E455
// DATA: STBC 0x0097e450
TGEventHandlerTable ScriptObject::classHandlers;

/*
================
ScriptObject::ScriptObject

The modules loaded when the object is made stay loaded when it goes.
================
*/
// FUNCTION: STBC 0x00409d20 ??0ScriptObject@@QAE@PADH@Z
ScriptObject::ScriptObject( char *script_, int firstEventType ) {
	script = NULL;
	onIdleFunc = NULL;
	terminateEvent = NULL;
	initializeEvent = NULL;
	numPersistentModules = 0;
	persistentModules = NULL;
	database = NULL;
	assetsLoaded = false;
	if ( script_ != NULL ) {
		script = new char[strlen( script_ ) + 1];
		strcpy( script, script_ );
	}
	eventCount = firstEventType;
	RecordPersistentModules();
}

/*
================
ScriptObject::GetObjType
================
*/
// FUNCTION: STBC 0x00409dd0
int ScriptObject::GetObjType( void ) {
	return SCRIPTOBJECT_TYPE;
}

/*
================
ScriptObject::GetClassName
================
*/
// FUNCTION: STBC 0x00409e20
char *ScriptObject::GetClassName( void ) {
	return "ScriptObject";
}

/*
================
ScriptObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00409e30
char *ScriptObject::GetSwigTypeName( void ) {
	return "_p_ScriptObject";
}

/*
================
ScriptObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00409e40
char *ScriptObject::GetPythonClassName( void ) {
	return "ScriptObjectPtr";
}

/*
================
ScriptObject::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00409e50
TGEventHandlerTable *ScriptObject::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ScriptObject::ScriptObject

The object a stream loads.
================
*/
// FUNCTION: STBC 0x00409e90 ??0ScriptObject@@QAE@J@Z
ScriptObject::ScriptObject( long id ) : TGEventHandlerObject( id ) {
	script = NULL;
	onIdleFunc = NULL;
	eventCount = 0;
	terminateEvent = NULL;
	initializeEvent = NULL;
	assetsLoaded = false;
	numPersistentModules = 0;
	persistentModules = NULL;
	database = NULL;
}

/*
================
ScriptObject::~ScriptObject

Sends the event Terminate was given, and unloads the modules loaded
since the object was made.
================
*/
// FUNCTION: STBC 0x00409ed0
// FUNCTION: STBC 0x00409e60 ??_GScriptObject@@UAEPAXI@Z
ScriptObject::~ScriptObject( void ) {
	if ( database != NULL ) {
		g_kLocalizationManager.Unload( database );
	}
	database = NULL;
	if ( terminateEvent != NULL ) {
		g_kEventManager.AddEvent( terminateEvent );
		terminateEvent->DecRefCount();
		terminateEvent = NULL;
	}
	if ( script != NULL ) {
		delete script;
	}
	if ( onIdleFunc != NULL ) {
		delete onIdleFunc;
	}
	if ( persistentModules != NULL ) {
		PyEmbed::UnloadOtherModules( numPersistentModules, persistentModules );
		for ( int i = 0; i < numPersistentModules; i++ ) {
			delete persistentModules[i];
		}
		delete persistentModules;
		persistentModules = NULL;
		numPersistentModules = 0;
	}
}

/*
================
ScriptObject::SetOnIdleFunc
================
*/
// FUNCTION: STBC 0x00409fc0
void ScriptObject::SetOnIdleFunc( char *function ) {
	if ( onIdleFunc != NULL ) {
		delete onIdleFunc;
	}
	if ( function != NULL ) {
		onIdleFunc = new char[strlen( function ) + 1];
		strcpy( onIdleFunc, function );
	} else {
		onIdleFunc = NULL;
	}
}

/*
================
ScriptObject::Initialize

Only a script module that loads is initialized; the event is sent once
it has been.
================
*/
// FUNCTION: STBC 0x0040a030
void ScriptObject::Initialize( TGEvent *event ) {
	if ( PyEmbed::GetModule( script, false ) != NULL ) {
		CallScriptFunction( "Initialize" );
		initializeEvent = event;
		if ( event != NULL ) {
			event->IncRefCount();
		}
		if ( initializeEvent != NULL ) {
			g_kEventManager.AddEvent( initializeEvent );
			initializeEvent->DecRefCount();
			initializeEvent = NULL;
		}
	}
}

/*
================
ScriptObject::Terminate

The script's sounds go, and the object deletes itself in two stages;
the destructor sends the event.
================
*/
// FUNCTION: STBC 0x0040a090
void ScriptObject::Terminate( TGEvent *event ) {
	terminateEvent = event;
	if ( event != NULL ) {
		event->IncRefCount();
	}
	CallScriptFunction( "Terminate" );
	g_kSoundManager.DeleteAllSoundsInGroup( script );
	TGEvent *deleteEvent = new TGEvent;
	deleteEvent->SetEventType( ET_DELETE_ME_STAGE_1 );
	deleteEvent->SetDestination( this );
	g_kEventManager.AddEvent( deleteEvent );
}

/*
================
ScriptObject::CallScriptFunction
================
*/
// FUNCTION: STBC 0x0040a110
void ScriptObject::CallScriptFunction( char *function ) {
	PyObject *shadow = VirtualSlot08();
	int result = PyEmbed::CallFunction( script, function, NULL, NULL, "(O)", shadow );
	Py_DECREF( shadow );
	if ( result == -1 ) {
		PyErr_Print();
	}
}

/*
================
ScriptObject::RecordPersistentModules
================
*/
// FUNCTION: STBC 0x0040a160
void ScriptObject::RecordPersistentModules( void ) {
	if ( persistentModules != NULL ) {
		for ( int i = 0; i < numPersistentModules; i++ ) {
			delete persistentModules[i];
		}
		delete persistentModules;
		persistentModules = NULL;
	}
	PyEmbed::GetLoadedModuleNames( &numPersistentModules, &persistentModules );
}

/*
================
ScriptObject::LoadSound
================
*/
// FUNCTION: STBC 0x0040a1b0
TGSound *ScriptObject::LoadSound( char *fileName, char *name, long flags ) {
	if ( fileName == NULL ) {
		return NULL;
	}
	TGSound *sound = new TGSound( fileName, name, flags );
	if ( sound != NULL ) {
		sound->SetGroup( script );
	}
	return sound;
}

/*
================
ScriptObject::SetDatabase

The database before is unloaded; NULL loads none.
================
*/
// FUNCTION: STBC 0x0040a250
TGLocalizationDatabase *ScriptObject::SetDatabase( char *name ) {
	if ( database != NULL ) {
		g_kLocalizationManager.Unload( database );
	}
	database = NULL;
	if ( name != NULL ) {
		database = g_kLocalizationManager.Load( name, true );
	}
	return database;
}

/*
================
ScriptObject::AddPersistentModule
================
*/
// FUNCTION: STBC 0x0040a290
void ScriptObject::AddPersistentModule( char *module ) {
	if ( module != NULL ) {
		char **modules = new char *[numPersistentModules + 1];
		if ( persistentModules != NULL ) {
			memcpy( modules, persistentModules, numPersistentModules * sizeof( char * ) );
			delete persistentModules;
		}
		modules[numPersistentModules] = new char[strlen( module ) + 1];
		strcpy( modules[numPersistentModules], module );
		persistentModules = modules;
		numPersistentModules++;
	}
}

/*
================
ScriptObject::WriteToStream
================
*/
// FUNCTION: STBC 0x0040a340
bool ScriptObject::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( script );
	stream->VirtualSlot11( onIdleFunc );
	stream->WriteInt( eventCount );
	stream->WriteID( GetSavedID( terminateEvent ) );
	stream->WriteID( GetSavedID( initializeEvent ) );
	stream->WriteBool( assetsLoaded );
	stream->VirtualSlot11( database == NULL ? NULL : database->GetName() );
	stream->WriteInt( numPersistentModules );
	for ( int i = 0; i < numPersistentModules; i++ ) {
		stream->VirtualSlot11( persistentModules[i] );
	}
	return true;
}

/*
================
ScriptObject::ReadFromStream
================
*/
// FUNCTION: STBC 0x0040a400
bool ScriptObject::ReadFromStream( TGStream *stream ) {
	database = NULL;
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}
	script = stream->VirtualSlot10( -1 );
	onIdleFunc = stream->VirtualSlot10( -1 );
	eventCount = stream->ReadInt();
	terminateEvent = (TGEvent *)stream->ReadID();
	initializeEvent = (TGEvent *)stream->ReadID();
	assetsLoaded = stream->ReadBool();
	char *databaseName = stream->VirtualSlot10( -1 );
	if ( databaseName != NULL ) {
		database = g_kLocalizationManager.Load( databaseName, false );
		delete databaseName;
	}
	numPersistentModules = stream->ReadInt();
	if ( numPersistentModules == 0 ) {
		persistentModules = NULL;
		return true;
	}
	persistentModules = new char *[numPersistentModules];
	for ( int i = 0; i < numPersistentModules; i++ ) {
		persistentModules[i] = stream->VirtualSlot10( -1 );
	}
	return true;
}

/*
================
ScriptObject::FixUpPointers
================
*/
// FUNCTION: STBC 0x0040a4e0
bool ScriptObject::FixUpPointers( void ) {
	if ( !TGEventHandlerObject::FixUpPointers() ) {
		return false;
	}
	if ( terminateEvent != NULL && TGEvent::GetTGEventPtr( (long)terminateEvent ) == NULL ) {
		return false;
	}
	if ( initializeEvent != NULL && TGEvent::GetTGEventPtr( (long)initializeEvent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ScriptObject::PostFixUp
================
*/
// FUNCTION: STBC 0x0040a530
void ScriptObject::PostFixUp( void ) {
	TGEventHandlerObject::PostFixUp();
	terminateEvent = TGEvent::GetSavedEvent( (long)terminateEvent );
	initializeEvent = TGEvent::GetSavedEvent( (long)initializeEvent );
}
