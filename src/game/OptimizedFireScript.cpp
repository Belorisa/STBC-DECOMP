/*
===============================================================================

	OptimizedFireScript.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "OptimizedFireScript.h"
#include "CloakingSubsystem.h"
#include "DamageableObject.h"
#include "Episode.h"
#include "FuzzyLogic.h"
#include "Game.h"
#include "HullClass.h"
#include "ImpulseEngineSubsystem.h"
#include "Mission.h"
#include "ObjectClass.h"
#include "ObjectGroup.h"
#include "PhaserBank.h"
#include "PhaserProperty.h"
#include "PhaserSystem.h"
#include "PhysicsObjectClass.h"
#include "ProximityManager.h"
#include "PulseWeapon.h"
#include "PulseWeaponProperty.h"
#include "PyEmbed.h"
#include "SetClass.h"
#include "ShieldClass.h"
#include "ShipClass.h"
#include "ShipSubsystem.h"
#include "TGMatrix3.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "Torpedo.h"
#include "TorpedoAmmoType.h"
#include "TorpedoSystem.h"
#include "TorpedoSystemProperty.h"
#include "TorpedoTube.h"
#include "TractorBeamSystem.h"
#include "UtopiaModule.h"
#include "Weapon.h"
#include "WeaponSystem.h"
#include "NiMain.h"
#include "NiMath.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00481880 _$E32
// FUNCTION: STBC 0x00481890 _$E29
// FUNCTION: STBC 0x004818a0 _$E31
// FUNCTION: STBC 0x004818b0 _$E30
// FUNCTION: STBC 0x004818c0 _$E38
// FUNCTION: STBC 0x004818d0 _$E35
// FUNCTION: STBC 0x004818e0 _$E37
// FUNCTION: STBC 0x004818f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00481900 _$E104
// FUNCTION: STBC 0x00481910 _$E103
// FUNCTION: STBC 0x00481920 _$E362
// FUNCTION: STBC 0x00481930 _$E361
// FUNCTION: STBC 0x00481940 _$E115
// FUNCTION: STBC 0x00481950 _$E114
// FUNCTION: STBC 0x00481960 _$E119
// FUNCTION: STBC 0x00481970 _$E118
// FUNCTION: STBC 0x00481980 _$E123
// FUNCTION: STBC 0x00481990 _$E122
// FUNCTION: STBC 0x004819a0 _$E127
// FUNCTION: STBC 0x004819b0 _$E126
// FUNCTION: STBC 0x004819c0 _$E242
// FUNCTION: STBC 0x004819d0 _$E241
// FUNCTION: STBC 0x004819e0 _$E184
// FUNCTION: STBC 0x004819f0 _$E183
// FUNCTION: STBC 0x00481a00 _$E197
// FUNCTION: STBC 0x00481a10 _$E196
// FUNCTION: STBC 0x00481a20 _$E247
// FUNCTION: STBC 0x00481a30 _$E246
// FUNCTION: STBC 0x00481a40 _$E367
// FUNCTION: STBC 0x00481a50 _$E366
// FUNCTION: STBC 0x00481a60 _$E262
// FUNCTION: STBC 0x00481a70 _$E261
// FUNCTION: STBC 0x00481a80 _$E371
// FUNCTION: STBC 0x00481a90 _$E370
// FUNCTION: STBC 0x00481aa0 _$E378
// FUNCTION: STBC 0x00481ab0 _$E377
// FUNCTION: STBC 0x00482260 _$E396
// FUNCTION: STBC 0x00482270 _$E395
// FUNCTION: STBC 0x00482280 _$E400
// FUNCTION: STBC 0x00482290 _$E399
// FUNCTION: STBC 0x004822a0 _$E146
// FUNCTION: STBC 0x004822b0 _$E145
// FUNCTION: STBC 0x00482760 _$E278
// FUNCTION: STBC 0x00482770 _$E277
// FUNCTION: STBC 0x00483070 _$E285
// FUNCTION: STBC 0x00483080 _$E284
// FUNCTION: STBC 0x004834c0 _$E289
// FUNCTION: STBC 0x004834d0 _$E288
// FUNCTION: STBC 0x004834e0 _$E293
// FUNCTION: STBC 0x004834f0 _$E292
// FUNCTION: STBC 0x004836b0 _$E297
// FUNCTION: STBC 0x004836c0 _$E296
// FUNCTION: STBC 0x00483920 _$E303
// FUNCTION: STBC 0x00483930 _$E302
// FUNCTION: STBC 0x00483e10 _$E251
// FUNCTION: STBC 0x00483e20 _$E250
// FUNCTION: STBC 0x00483e30 _$E255
// FUNCTION: STBC 0x00483e40 _$E254
// FUNCTION: STBC 0x004844f0 _$E404
// FUNCTION: STBC 0x00484500 _$E403
// FUNCTION: STBC 0x00484510 _$E408
// FUNCTION: STBC 0x00484520 _$E407
// FUNCTION: STBC 0x00484530 _$E412
// FUNCTION: STBC 0x00484540 _$E411
// FUNCTION: STBC 0x00484550 _$E416
// FUNCTION: STBC 0x00484560 _$E415
// FUNCTION: STBC 0x00484a50 _$E326
// FUNCTION: STBC 0x00484a60 _$E325
// FUNCTION: STBC 0x00484a70 _$E420
// FUNCTION: STBC 0x00484a80 _$E419
// FUNCTION: STBC 0x00484d20 _$E424
// FUNCTION: STBC 0x00484d30 _$E423
// FUNCTION: STBC 0x00485210 _$E428
// FUNCTION: STBC 0x00485220 _$E427
// FUNCTION: STBC 0x00485340 _$E151
// FUNCTION: STBC 0x00485350 _$E150
// FUNCTION: STBC 0x004856c0 _$E432
// FUNCTION: STBC 0x004856d0 _$E431
// FUNCTION: STBC 0x00485c00 _$E156
// FUNCTION: STBC 0x00485c10 _$E155
// FUNCTION: STBC 0x00485c20 _$E436
// FUNCTION: STBC 0x00485c30 _$E435
// FUNCTION: STBC 0x00485c40 _$E440
// FUNCTION: STBC 0x00485c50 _$E439
// FUNCTION: STBC 0x00485e20 _$E444
// FUNCTION: STBC 0x00485e30 _$E443
// FUNCTION: STBC 0x00485fc0 _$E448
// FUNCTION: STBC 0x00485fd0 _$E447
// FUNCTION: STBC 0x00485fe0 _$E161
// FUNCTION: STBC 0x00485ff0 _$E160
// FUNCTION: STBC 0x00486000 _$E452
// FUNCTION: STBC 0x00486010 _$E451
// FUNCTION: STBC 0x00486020 _$E456
// FUNCTION: STBC 0x00486030 _$E455
// FUNCTION: STBC 0x00486040 _$E460
// FUNCTION: STBC 0x00486050 _$E459
// FUNCTION: STBC 0x00486060 _$E167
// FUNCTION: STBC 0x00486070 _$E166
// FUNCTION: STBC 0x00486080 _$E172
// FUNCTION: STBC 0x00486090 _$E171
// FUNCTION: STBC 0x004860a0 _$E202
// FUNCTION: STBC 0x004860b0 _$E201
// FUNCTION: STBC 0x004860c0 _$E464
// FUNCTION: STBC 0x004860d0 _$E463
// FUNCTION: STBC 0x004860e0 _$E207
// FUNCTION: STBC 0x004860f0 _$E206
// FUNCTION: STBC 0x00486100 _$E468
// FUNCTION: STBC 0x00486110 _$E467
// FUNCTION: STBC 0x00486120 _$E472
// FUNCTION: STBC 0x00486130 _$E471
// FUNCTION: STBC 0x00486140 _$E212
// FUNCTION: STBC 0x00486150 _$E211
// FUNCTION: STBC 0x00486160 _$E476
// FUNCTION: STBC 0x00486170 _$E475
// FUNCTION: STBC 0x00486180 _$E177
// FUNCTION: STBC 0x00486190 _$E176
// FUNCTION: STBC 0x004861a0 _$E480
// FUNCTION: STBC 0x004861b0 _$E479
// FUNCTION: STBC 0x004861c0 _$E484
// FUNCTION: STBC 0x004861d0 _$E483
// FUNCTION: STBC 0x004861e0 _$E217
// FUNCTION: STBC 0x004861f0 _$E216
// FUNCTION: STBC 0x00486200 _$E222
// FUNCTION: STBC 0x00486210 _$E221
// FUNCTION: STBC 0x00486220 _$E488
// FUNCTION: STBC 0x00486230 _$E487
// FUNCTION: STBC 0x00486240 _$E492
// FUNCTION: STBC 0x00486250 _$E491
// FUNCTION: STBC 0x00486260 _$E496
// FUNCTION: STBC 0x00486270 _$E495
// FUNCTION: STBC 0x00486280 _$E500
// FUNCTION: STBC 0x00486290 _$E499
// FUNCTION: STBC 0x004862a0 _$E504
// FUNCTION: STBC 0x004862b0 _$E503
// FUNCTION: STBC 0x004862c0 _$E508
// FUNCTION: STBC 0x004862d0 _$E507
// FUNCTION: STBC 0x004862e0 _$E512
// FUNCTION: STBC 0x004862f0 _$E511
// FUNCTION: STBC 0x00486300 _$E516
// FUNCTION: STBC 0x00486310 _$E515
// FUNCTION: STBC 0x00486320 _$E227
// FUNCTION: STBC 0x00486330 _$E226
// FUNCTION: STBC 0x00486340 _$E520
// FUNCTION: STBC 0x00486350 _$E519
// FUNCTION: STBC 0x00486360 _$E232
// FUNCTION: STBC 0x00486370 _$E231
// FUNCTION: STBC 0x00486380 _$E237
// FUNCTION: STBC 0x00486390 _$E236
// FUNCTION: STBC 0x00486940 _$E524
// FUNCTION: STBC 0x00486950 _$E523
// FUNCTION: STBC 0x00486960 _$E529
// FUNCTION: STBC 0x00486970 _$E528
// FUNCTION: STBC 0x00486980 _$E533
// FUNCTION: STBC 0x00486990 _$E532

// A quarter turn, in radians.
static const float HALF_PI = 1.5707964f;

// The seconds between two steps (the script's GetNextUpdateTime says 0.2).
static const float UPDATE_INTERVAL = 0.1f;

// How fast phaserUse rises while the phasers fire, per second, and how fast
// it falls while they rest.
static const float PHASER_USE_RISE = 0.05f;
// FUNCTION: STBC 0x00481ac0 _$E536
// FUNCTION: STBC 0x00481ad0 _$E535
static const float PHASER_USE_FALL = PHASER_USE_RISE * 0.5f;

// What the shield, phaser use and distance factors must pass together for the
// phasers to fire.
static const float PHASER_FIRE_THRESHOLD = 1.75f;

// How many subsystems ChooseRatedSubsystem rates in a call (the script's
// iNumTargetSubsystemsToUpdate is 4).
#define NUM_SUBSYSTEMS_TO_UPDATE	2

// The phaser power settings and tractor beam modes used here (the SWIG
// constants PhaserSystem.PP_* and TractorBeamSystem.TBS_*).
#define PP_LOW				0
#define PP_HIGH				2
#define TBS_DOCK_STAGE_1	4
#define TBS_DOCK_STAGE_2	5

// FUNCTION: STBC 0x00481af0 _$E539
// FUNCTION: STBC 0x00481b00 _$E538
static AICreateFunctionRegistration fireScriptRegistration( AI_TYPE_FIRE_SCRIPT, OptimizedFireScript::Create );

// FUNCTION: STBC 0x00481b50 _$E542
// FUNCTION: STBC 0x00481b60 _$E541
static OptimizedVersionRegistration fireScriptVersion( "FireScript", OptimizedFireScript::CreateFromScript );

// The module's construction and exit-time destruction.
// FUNCTION: STBC 0x00481c20 _$E547
// FUNCTION: STBC 0x00481c30 _$E544
// FUNCTION: STBC 0x00481c90 _$E546
// FUNCTION: STBC 0x00481ca0 _$E545
OptimizedFireScriptModule optimizedFireScriptModule;		// invented name

/*
================
OptimizedFireScriptModule::VirtualSlot00
================
*/
// FUNCTION: STBC 0x00481c60
void OptimizedFireScriptModule::VirtualSlot00( void ) {
	PyEmbed::AddClassMethod( OptimizedFireScript_GetWeapons, "OptimizedFireScript", "GetWeapons" );
}

/*
================
OptimizedFireScriptModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x00481c80
void OptimizedFireScriptModule::VirtualSlot05( void ) {
}

/*
================
OptimizedFireScript::OptimizedFireScript
================
*/
// FUNCTION: STBC 0x00481cc0 ??0OptimizedFireScript@@QAE@PAVPreprocessingAI@@@Z
// PARKED: register: base/index operand order in the inlined NiTListBlock constructors (eax + ecx against retail's ecx + eax, differing even between two addresses of one loop in retail); the rest agrees
OptimizedFireScript::OptimizedFireScript( PreprocessingAI *ai ) : PreprocessingAI( ai->GetObject(), ai->name ),
		targetSubsystems( 2 ), ignoreSubsystemTargets( false ), weapons( 2 ), torpsTooDangerous( false ),
		targetedSubsystemID( 0 ), lastUpdate( -2 ), targetSubsystemUpdateNum( 0 ), targetVisible( false ) {
	phaserUse = 0.0f;
	lastPhaserTime = g_kUtopiaModule.GetGameTime();
	containedAI = ai->containedAI;
	ai->containedAI = NULL;
	containedAI->SetParentID( GetID() );
	interruptable = ai->interruptable;

	PyObject *instance = ai->preprocessingInstance;
	target = NULL;
	PyEmbed::GetStringAttribute( instance, "sTarget", &target );
	PyEmbed::GetFloatAttribute( instance, "fMaxFiringRange", &maxFiringRange );
	PyEmbed::GetBoolAttribute( instance, "bHighPower", &highPower );
	PyEmbed::GetBoolAttribute( instance, "bChangePowerSetting", &changePowerSetting );
	PyEmbed::GetBoolAttribute( instance, "bHullSelectedChooseAlternate", &hullSelectedChooseAlternate );

	// BUG: the list is released even when the instance has no such attribute.
	PyObject *list = PyObject_GetAttrString( instance, "lTargetSubsystems" );
	if ( list != NULL && PyList_Check( list ) ) {
		for ( int i = 0; i < PyList_Size( list ); i++ ) {
			PyObject *item = PyList_GetItem( list, i );
			if ( item != NULL && PyTuple_Check( item ) ) {
				FireTargetSubsystem *targetSubsystem = new FireTargetSubsystem;
				targetSubsystem->type = PyInt_AsLong( PyTuple_GetItem( item, 0 ) );
				targetSubsystem->priority = PyFloat_AsDouble( PyTuple_GetItem( item, 1 ) );
				targetSubsystems.AddTail( targetSubsystem );
			}
		}
	}
	Py_DECREF( list );

	PyEmbed::GetBoolAttribute( instance, "bChooseSubsystemTargets", &chooseSubsystemTargets );
	PyEmbed::GetBoolAttribute( instance, "bChooseTorpsWisely", &chooseTorpsWisely );
	PyEmbed::GetBoolAttribute( instance, "bSmartPhasers", &smartPhasers );

	// BUG: the list is released even when the instance has no such attribute.
	PyObject *weaponList = PyObject_GetAttrString( instance, "lWeapons" );
	if ( weaponList != NULL && PyList_Check( weaponList ) ) {
		for ( int i = 0; i < PyList_Size( weaponList ); i++ ) {
			PyObject *item = PyList_GetItem( weaponList, i );
			if ( item != NULL && PyInstance_Check( item ) ) {
				WeaponSystem *system = NULL;
				PyEmbed::Unknown6F98E0( item, (void **)&system, "WeaponSystem" );
				PyErr_Clear();
				if ( system != NULL ) {
					weapons.AddTail( system );
				}
			}
		}
	}
	Py_DECREF( weaponList );

	PyEmbed::GetBoolAttribute( instance, "bEnabled", &enabled );
	PyEmbed::GetBoolAttribute( instance, "bAttackDisabledSubsystems", &attackDisabledSubsystems );
	PyEmbed::GetBoolAttribute( instance, "bAttackIfNoSubsystemsLeft", &attackIfNoSubsystemsLeft );
	PyEmbed::GetBoolAttribute( instance, "bCallUsingWeaponTypeFunc", &callUsingWeaponTypeFunc );
	PyEmbed::GetBoolAttribute( instance, "bDumbFireTorps", &dumbFireTorps );
	PyEmbed::GetBoolAttribute( instance, "bDisableFirst", &disableFirst );
	PyEmbed::GetFloatAttribute( instance, "fFireDotThreshold", &fireDotThreshold );
	if ( !PyEmbed::GetIntAttribute( instance, "eTractorBeamMode", &tractorBeamMode ) ) {
		tractorBeamMode = 0;
	}
	PyErr_Print();
}

/*
================
OptimizedFireScript::GetPreprocessingInstance

The optimized version has no script instance: it hands out a new shadow
instance of itself.
================
*/
// FUNCTION: STBC 0x00482210
PyObject *OptimizedFireScript::GetPreprocessingInstance( void ) {
	return VirtualSlot15();
}

/*
================
OptimizedFireScript::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00482220
PyObject *OptimizedFireScript::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "OptimizedFireScript" );
}

/*
================
OptimizedFireScript::GetAIType
================
*/
// FUNCTION: STBC 0x00482230
int OptimizedFireScript::GetAIType( void ) {
	return AI_TYPE_FIRE_SCRIPT;
}

/*
================
OptimizedFireScript::VirtualSlot20
================
*/
// FUNCTION: STBC 0x00482240
bool OptimizedFireScript::VirtualSlot20( int type ) {
	if ( type == AI_TYPE_FIRE_SCRIPT ) {
		return true;
	}
	return PreprocessingAI::VirtualSlot20( type );
}

/*
================
OptimizedFireScript::~OptimizedFireScript
================
*/
// FUNCTION: STBC 0x004822e0
// DRAFT: register allocation in the inlined map and list clean-up: retail tests against a fresh edi where this build keeps a zero register
// FUNCTION: STBC 0x004822c0 ??_GOptimizedFireScript@@UAEPAXI@Z
OptimizedFireScript::~OptimizedFireScript( void ) {
	delete [] target;
	while ( targetSubsystems.GetSize() != 0 ) {
		delete targetSubsystems.RemoveHead();
	}
	NiTMapIterator pos = targetSubsystemRating.GetFirstPos();
	while ( pos != NULL ) {
		long id;
		FireSubsystemRating *rating;
		targetSubsystemRating.GetNext( pos, id, rating );
		delete rating;
	}
	targetSubsystemRating.RemoveAll();
}

/*
================
OptimizedFireScript::CreateFromScript
================
*/
// FUNCTION: STBC 0x00482510
ArtificialIntelligence *OptimizedFireScript::CreateFromScript( PreprocessingAI *ai ) {
	ArtificialIntelligence *optimized = new OptimizedFireScript( ai );
	delete ai;
	return optimized;
}

/*
================
OptimizedFireScript::IsKindListed

Whether a listed system is of the system's kind (invented name).
================
*/
inline bool OptimizedFireScript::IsKindListed( WeaponSystem *system ) {
	NiTListIterator pos = weapons.GetHeadPos();
	while ( pos != NULL ) {
		WeaponSystem *listed = weapons.GetNext( pos );
		if ( system->IsTypeOf( listed->GetObjType() ) ) {
			return true;
		}
	}
	return false;
}

/*
================
OptimizedFireScript::AddWeaponSystem

The system is listed even when one of its kind already is; the AIs of the
tree are told only when none was.
================
*/
// FUNCTION: STBC 0x00482580
void OptimizedFireScript::AddWeaponSystem( WeaponSystem *system ) {
	if ( !IsKindListed( system ) ) {
		callUsingWeaponTypeFunc = true;
	}
	weapons.AddTail( system );
}

/*
================
OptimizedFireScript::RemoveAllWeaponSystems
================
*/
// FUNCTION: STBC 0x00482620
void OptimizedFireScript::RemoveAllWeaponSystems( void ) {
	StopFiring();
	if ( weapons.GetSize() != 0 ) {
		callUsingWeaponTypeFunc = true;
	}
	weapons.RemoveAll();
}

/*
================
OptimizedFireScript::SetEnabled
================
*/
// FUNCTION: STBC 0x00482680
void OptimizedFireScript::SetEnabled( bool enabled_ ) {
	if ( enabled != enabled_ ) {
		if ( enabled ) {
			StopFiring();
		}
		enabled = enabled_;
	}
}

/*
================
OptimizedFireScript::HasSubsystemTargets
================
*/
// FUNCTION: STBC 0x004826b0
bool OptimizedFireScript::HasSubsystemTargets( void ) {
	return !ignoreSubsystemTargets && targetSubsystems.GetSize() != 0;
}

/*
================
OptimizedFireScript::IgnoreSubsystemTargets
================
*/
// FUNCTION: STBC 0x004826d0
void OptimizedFireScript::IgnoreSubsystemTargets( void ) {
	ignoreSubsystemTargets = true;
}

/*
================
OptimizedFireScript::RestoreSubsystemTargets
================
*/
// FUNCTION: STBC 0x004826e0
void OptimizedFireScript::RestoreSubsystemTargets( void ) {
	ignoreSubsystemTargets = false;
}

/*
================
OptimizedFireScript::SetTarget
================
*/
// FUNCTION: STBC 0x004826f0
void OptimizedFireScript::SetTarget( char *name ) {
	StopFiring();
	targetedSubsystemID = 0;
	delete [] target;
	target = NULL;
	if ( name != NULL ) {
		target = new char[strlen( name ) + 1];
		strcpy( target, name );
	}
}

/*
================
OptimizedFireScript::LostFocus
================
*/
// FUNCTION: STBC 0x00482780
void OptimizedFireScript::LostFocus( void ) {
	PreprocessingAI::LostFocus();
	StopFiring();
}

/*
================
OptimizedFireScript::CallExternalFunction
================
*/
// FUNCTION: STBC 0x004827a0
bool OptimizedFireScript::CallExternalFunction( char *functionName, PyObject *arguments ) {
	if ( strcmp( functionName, "SetTarget" ) == 0 ) {
		if ( arguments == Py_None ) {
			SetTarget( NULL );
			return true;
		}
		if ( PyString_Check( arguments ) ) {
			SetTarget( PyString_AsString( arguments ) );
		}
		return true;
	}
	return false;
}

/*
================
OptimizedFireScript_GetWeapons

The Python method GetWeapons of the shadow class: a tuple of the weapon
systems' shadow instances.
================
*/
// FUNCTION: STBC 0x00482830
PyObject *OptimizedFireScript_GetWeapons( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		OptimizedFireScript *ai = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&ai, "OptimizedFireScript" );
		if ( ai != NULL ) {
			result = PyTuple_New( ai->GetWeapons().GetSize() );
			int index = 0;
			NiTListIterator pos = ai->GetWeapons().GetHeadPos();
			while ( pos != NULL ) {
				WeaponSystem *system = ai->GetWeapons().GetNext( pos );
				PyTuple_SetItem( result, index++, system->VirtualSlot08() );
			}
		}
	}
	return result;
}

/*
================
OptimizedFireScript::WriteToStream
================
*/
// FUNCTION: STBC 0x004828d0
bool OptimizedFireScript::WriteToStream( TGStream *stream ) {
	if ( !PreprocessingAI::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( target );
	stream->WriteFloat( maxFiringRange );
	stream->WriteBool( highPower );
	stream->WriteBool( changePowerSetting );
	stream->WriteBool( hullSelectedChooseAlternate );

	stream->WriteInt( targetSubsystems.GetSize() );
	NiTListIterator pos = targetSubsystems.GetHeadPos();
	while ( pos != NULL ) {
		FireTargetSubsystem *targetSubsystem = targetSubsystems.GetNext( pos );
		stream->WriteLong( targetSubsystem->type );
		stream->WriteFloat( targetSubsystem->priority );
	}

	stream->WriteBool( ignoreSubsystemTargets );
	stream->WriteBool( chooseSubsystemTargets );
	stream->WriteBool( chooseTorpsWisely );
	stream->WriteBool( smartPhasers );

	stream->WriteInt( weapons.GetSize() );
	pos = weapons.GetHeadPos();
	while ( pos != NULL ) {
		WeaponSystem *system = weapons.GetNext( pos );
		stream->WriteID( system->GetObjID() );
	}

	stream->WriteBool( enabled );
	stream->WriteBool( attackDisabledSubsystems );
	stream->WriteBool( attackIfNoSubsystemsLeft );
	stream->WriteBool( callUsingWeaponTypeFunc );
	stream->WriteFloat( fireDotThreshold );
	stream->WriteFloat( phaserUse );
	stream->WriteFloat( lastPhaserTime );
	stream->WriteBool( dumbFireTorps );
	stream->WriteBool( disableFirst );
	stream->WriteInt( tractorBeamMode );
	stream->WriteBool( torpsTooDangerous );
	stream->WriteID( targetedSubsystemID );
	stream->WriteInt( lastUpdate );

	stream->WriteInt( targetSubsystemRating.GetCount() );
	NiTMapIterator ratingPos = targetSubsystemRating.GetFirstPos();
	while ( ratingPos != NULL ) {
		long id;
		FireSubsystemRating *rating;
		targetSubsystemRating.GetNext( ratingPos, id, rating );
		stream->WriteID( id );
		stream->WriteInt( rating->updateNum );
		stream->WriteFloat( rating->rating );
	}

	stream->WriteInt( targetSubsystemUpdateNum );
	stream->WriteBool( targetVisible );
	return true;
}

/*
================
OptimizedFireScript::OptimizedFireScript

The weapon systems are listed by their IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x00482b50 ??0OptimizedFireScript@@QAE@PAVTGStream@@PA_N@Z
OptimizedFireScript::OptimizedFireScript( TGStream *stream, bool *ok ) : PreprocessingAI( stream, ok ),
		targetSubsystems( 2 ), weapons( 2 ) {
	if ( *ok ) {
		target = stream->VirtualSlot10( -1 );
		maxFiringRange = stream->ReadFloat();
		highPower = stream->ReadBool();
		changePowerSetting = stream->ReadBool();
		hullSelectedChooseAlternate = stream->ReadBool();

		int i;
		for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
			FireTargetSubsystem *targetSubsystem = new FireTargetSubsystem;
			targetSubsystem->type = stream->ReadLong();
			targetSubsystem->priority = stream->ReadFloat();
			targetSubsystems.AddTail( targetSubsystem );
		}

		ignoreSubsystemTargets = stream->ReadBool();
		chooseSubsystemTargets = stream->ReadBool();
		chooseTorpsWisely = stream->ReadBool();
		smartPhasers = stream->ReadBool();

		for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
			weapons.AddTail( (WeaponSystem *)stream->ReadID() );
		}

		enabled = stream->ReadBool();
		attackDisabledSubsystems = stream->ReadBool();
		attackIfNoSubsystemsLeft = stream->ReadBool();
		callUsingWeaponTypeFunc = stream->ReadBool();
		fireDotThreshold = stream->ReadFloat();
		phaserUse = stream->ReadFloat();
		lastPhaserTime = stream->ReadFloat();
		dumbFireTorps = stream->ReadBool();
		disableFirst = stream->ReadBool();
		tractorBeamMode = stream->ReadInt();
		torpsTooDangerous = stream->ReadBool();
		targetedSubsystemID = stream->ReadID();
		lastUpdate = stream->ReadInt();

		for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
			FireSubsystemRating *rating = new FireSubsystemRating;
			long id = stream->ReadID();
			rating->updateNum = stream->ReadInt();
			rating->rating = stream->ReadFloat();
			targetSubsystemRating.SetAt( id, rating );
		}

		targetSubsystemUpdateNum = stream->ReadInt();
		targetVisible = stream->ReadBool();
	}
}

/*
================
OptimizedFireScript::Create
================
*/
// FUNCTION: STBC 0x00483090
ArtificialIntelligence *OptimizedFireScript::Create( TGStream *stream ) {
	bool ok = true;
	OptimizedFireScript *ai = new OptimizedFireScript( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}

/*
================
OptimizedFireScript::FixUpPointers

Fails when a listed weapon system is gone.
================
*/
// FUNCTION: STBC 0x00483110
bool OptimizedFireScript::FixUpPointers( void ) {
	if ( !PreprocessingAI::FixUpPointers() ) {
		return false;
	}
	NiTListIterator pos = weapons.GetHeadPos();
	while ( pos != NULL ) {
		long id = (long)weapons.GetNext( pos );
		if ( id != 0 ) {
			if ( WeaponSystem::Cast( TGObject::GetTGObjectPtr( id ) ) == NULL ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
OptimizedFireScript::PostFixUp

Turns the listed IDs into the weapon systems.
================
*/
// FUNCTION: STBC 0x00483150
void OptimizedFireScript::PostFixUp( void ) {
	PreprocessingAI::PostFixUp();

	NiTListIterator pos = weapons.GetHeadPos();
	NiTList<WeaponSystem *> systems( 2 );
	while ( pos != NULL ) {
		long id = (long)weapons.GetNext( pos );
		systems.AddTail( WeaponSystem::Cast( TGObject::GetTGObjectPtr( id ) ) );
	}
	weapons.RemoveAll();
	pos = systems.GetHeadPos();
	while ( pos != NULL ) {
		weapons.AddTail( systems.GetNext( pos ) );
	}
}

/*
================
OptimizedFireScript::CallPreprocessingMethod

The script's Update: one step when its time has come. The first step with a
target checks that it can be fired at, the next chooses the subsystem to aim
at, and the next fires the weapon systems; then it starts over.
================
*/
// FUNCTION: STBC 0x00483330
int OptimizedFireScript::CallPreprocessingMethod( double time ) {
	if ( !forceUpdate && g_kUtopiaModule.GetGameTime() < nextUpdateTime ) {
		return preprocessStatus;
	}
	nextUpdateTime = g_kUtopiaModule.GetGameTime() + UPDATE_INTERVAL;

	ShipClass *ship = GetShip();
	ObjectClass *target = GetTarget();
	if ( ship == NULL || target == NULL ) {
		return PS_DONE;
	}
	CallUsingWeaponTypeFunc();
	if ( !enabled || weapons.GetSize() == 0 ) {
		return PS_NORMAL;
	}

	if ( !targetVisible || lastUpdate == -2 ) {
		targetVisible = TargetVisible( ship, target );
		if ( !targetVisible ) {
			StopFiring();
			lastUpdate = -2;
		} else {
			lastUpdate = -1;
		}
	}
	if ( lastUpdate == -1 ) {
		ChooseTargetSubsystem( ship, target );
	} else if ( lastUpdate >= 0 ) {
		ShipSubsystem *subsystem = ShipSubsystem::Cast( TGObject::GetTGObjectPtr( targetedSubsystemID ) );
		lastUpdate = -1;
		NiTListIterator pos = weapons.GetHeadPos();
		while ( pos != NULL ) {
			WeaponSystem *system = weapons.GetNext( pos );
			lastUpdate++;
			if ( subsystem == NULL && !attackIfNoSubsystemsLeft ) {
				StopFiring();
			} else {
				FireSystemAtTarget( system, target, subsystem );
			}
		}
	}
	lastUpdate = ( lastUpdate + 3 ) % ( weapons.GetSize() + 2 ) - 2;
	return PS_NORMAL;
}

/*
================
OptimizedFireScript::FireSystemAtTarget

Sets the system up and fires it at the target, or at the target's subsystem;
stops it when it should not fire.
================
*/
// FUNCTION: STBC 0x00483500
void OptimizedFireScript::FireSystemAtTarget( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem ) {
	if ( !ConfigureWeaponSystem( system, target, subsystem ) ) {
		system->StopFiringAtTarget( target );
		return;
	}
	if ( !CheckGoodShot( system, target, subsystem ) ) {
		system->StopFiringAtTarget( target );
		return;
	}
	if ( dumbFireTorps && TorpedoSystem::Cast( system ) != NULL ) {
		NiPoint3 toTarget;
		if ( subsystem != NULL ) {
			toTarget = subsystem->GetWorldLocation();
		} else {
			toTarget = target->GetWorldLocation();
		}
		toTarget -= system->GetWorldLocation();
		for ( int i = 0; i < system->GetNumChildSubsystems(); i++ ) {
			TorpedoTube *tube = TorpedoTube::Cast( system->GetChildSubsystem( i ) );
			if ( tube != NULL ) {
				if ( toTarget.Dot( tube->CalculateRoughDirection() ) > 0.0f ) {
					tube->FireDumb( 0.0f, true );
				}
			}
		}
	} else {
		NiPoint3 offset;
		if ( subsystem != NULL ) {
			offset = subsystem->GetDamagePoint();
		} else {
			offset = NiPoint3::ZERO;
		}
		system->VirtualSlot34( target, &offset );
	}
}

/*
================
OptimizedFireScript::ConfigureWeaponSystem

Whether the system may fire: never while it is too dangerous for the target.
Sets the tractor beam's mode, the phasers' power and the torpedo type.
================
*/
// FUNCTION: STBC 0x004836d0
bool OptimizedFireScript::ConfigureWeaponSystem( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem ) {
	if ( WeaponTooDangerous( system, target, subsystem ) ) {
		if ( system->IsTypeOf( TORPEDOSYSTEM_TYPE ) && !torpsTooDangerous ) {
			torpsTooDangerous = true;
			callUsingWeaponTypeFunc = true;
		}
		return false;
	}
	if ( system->IsTypeOf( TORPEDOSYSTEM_TYPE ) && torpsTooDangerous ) {
		torpsTooDangerous = false;
		callUsingWeaponTypeFunc = true;
	}

	TractorBeamSystem *tractor = TractorBeamSystem::Cast( system );
	if ( tractor != NULL ) {
		if ( tractor->GetMode() != tractorBeamMode ) {
			if ( !( tractor->IsFiring() && tractor->GetMode() == TBS_DOCK_STAGE_2 && tractorBeamMode == TBS_DOCK_STAGE_1 ) ) {
				tractor->SetMode( tractorBeamMode );
			}
		}
		return true;
	}

	PhaserSystem *phasers = PhaserSystem::Cast( system );
	if ( phasers != NULL ) {
		if ( changePowerSetting ) {
			if ( !highPower ) {
				phasers->SetPowerLevel( PP_LOW );
			} else {
				phasers->SetPowerLevel( PP_HIGH );
			}
		}
		if ( smartPhasers ) {
			bool fire = ShouldFirePhasers( system, target, subsystem );
			if ( fire ) {
				phaserUse += ( g_kUtopiaModule.GetGameTime() - lastPhaserTime ) * PHASER_USE_RISE;
				if ( phaserUse > 1.0f ) {
					phaserUse = 1.0f;
				}
			} else {
				phaserUse -= ( g_kUtopiaModule.GetGameTime() - lastPhaserTime ) * PHASER_USE_FALL;
				if ( phaserUse < 0.0f ) {
					phaserUse = 0.0f;
				}
			}
			lastPhaserTime = g_kUtopiaModule.GetGameTime();
			return fire;
		}
		return true;
	}

	TorpedoSystem *torps = TorpedoSystem::Cast( system );
	if ( torps != NULL && chooseTorpsWisely ) {
		NiPoint3 targetLocation;
		if ( subsystem != NULL ) {
			targetLocation = subsystem->GetWorldLocation();
		} else {
			targetLocation = target->GetWorldLocation();
		}
		float speed = 0.0f;
		ShipClass *targetShip = ShipClass::Cast( target );
		if ( targetShip != NULL ) {
			ImpulseEngineSubsystem *impulseEngines = targetShip->GetImpulseEngineSubsystem();
			if ( impulseEngines != NULL ) {
				speed = impulseEngines->GetCurMaxSpeed();
			}
		}
		ChooseTorpType( torps, &targetLocation, speed );
	}
	return true;
}

/*
================
OptimizedFireScript::ShouldFirePhasers

Always within twice the phasers' full-damage distance. Farther out the
distance, the shield facing the ship and the recent use of the phasers decide:
a weak facing shield and rested phasers make it worth firing.
================
*/
// FUNCTION: STBC 0x00483940
// PARKED: register/schedule: the store of the facing shield's strength (fstp) is scheduled after the loop set-up where retail stores it at once; the rest agrees
bool OptimizedFireScript::ShouldFirePhasers( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem ) {
	NiPoint3 targetLocation;
	if ( subsystem != NULL ) {
		targetLocation = subsystem->GetWorldLocation();
	} else {
		targetLocation = target->GetWorldLocation();
	}
	float distance = ( targetLocation - system->GetWorldLocation() ).Length();

	PhaserSystem *phasers = PhaserSystem::Cast( system );
	if ( phasers == NULL ) {
		return false;
	}
	float maxDamageDistance = 1.0f;
	if ( phasers->GetNumChildSubsystems() != 0 ) {
		maxDamageDistance = ( (PhaserBank *)phasers->GetChildSubsystem( 0 ) )->GetProperty()->GetMaxDamageDistance();
		if ( maxDamageDistance * 2.0f > distance ) {
			return true;
		}
	}
	float maxDistance = maxDamageDistance * 6.0f;
	float distanceFactor = ( maxDistance - distance ) / ( maxDistance - maxDamageDistance * 2.0f );
	if ( distanceFactor < 0.0f ) {
		return false;
	}

	float shieldFactor = 1.0f;
	ShipClass *targetShip = ShipClass::Cast( target );
	if ( targetShip != NULL ) {
		ShieldClass *shields = targetShip->GetShields();
		if ( shields != NULL && shields->IsOn() && !shields->IsDisabled() ) {
			int facing = shields->Unknown56A690( &system->GetWorldLocation(), &targetLocation );
			if ( facing != -1 ) {
				float others = 0.0f;
				float shield = shields->GetCurShields( facing );
				for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
					if ( i != facing ) {
						others += shields->GetCurShields( i );
					}
				}
				others /= ShieldClass::NUM_SHIELDS - 1;		// the average of the other facings
				if ( shield > others && shield > 0.0f ) {
					shieldFactor = (float)atan( others / shield ) / HALF_PI;
				}
			}
		}
	}
	if ( shieldFactor + phaserUse + distanceFactor > PHASER_FIRE_THRESHOLD ) {
		return true;
	}
	return false;
}

/*
================
OptimizedFireScript::WeaponTooDangerous

Always for a dying target. When disabled subsystems are not to be attacked,
torpedoes are too dangerous if the ones on their way and the ones the tubes
facing the target hold would destroy the subsystem or a critical one.
================
*/
// FUNCTION: STBC 0x00483b80
bool OptimizedFireScript::WeaponTooDangerous( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem ) {
	DamageableObject *damageable = DamageableObject::Cast( target );
	if ( damageable != NULL ) {
		if ( damageable->IsDead() || damageable->IsDying() ) {
			return true;
		}
	}
	if ( attackDisabledSubsystems ) {
		return false;
	}
	TorpedoSystem *torps = TorpedoSystem::Cast( system );
	if ( torps == NULL ) {
		return false;
	}
	SetClass *set = target->GetContainingSet();
	if ( set != NULL ) {
		long targetID = target->GetObjID();
		float incomingDamage = 0.0f;
		int count = 0;
		Torpedo **torpedoes = Torpedo::GetClassObjects( set, &count );
		int i;
		for ( i = 0; i < count; i++ ) {
			if ( torpedoes[i]->GetTargetID() == targetID ) {
				incomingDamage += torpedoes[i]->GetDamage();
			}
		}

		ShipClass *ship = system->GetParentShip();
		NiPoint3 toTarget;
		if ( subsystem != NULL ) {
			toTarget = subsystem->GetWorldLocation();
		} else {
			toTarget = target->GetWorldLocation();
		}
		toTarget -= ship->GetWorldLocation();

		float torpDamage = 0.0f;
		if ( PyEmbed::CallFunction( torps->GetCurrentAmmoType()->GetTorpedoScript(), "GetDamage", "f", &torpDamage, "()" ) == -1 ) {
			PyErr_Clear();
		}
		count = 0;
		for ( i = 0; i < torps->GetNumChildSubsystems(); i++ ) {
			TorpedoTube *tube = (TorpedoTube *)torps->GetChildSubsystem( i );
			if ( toTarget.Dot( ship->GetWorldRotation() * tube->GetDirection() ) > 0.0f ) {
				count += tube->GetMaxReady();
			}
		}
		torpDamage *= count;

		// BUG: the subsystem is read without a test; the callers pass NULL for a
		// target without subsystems.
		if ( !( subsystem->GetCondition() - ( torpDamage + incomingDamage ) > 0.0f ) ) {
			return true;
		}
		ShipClass *targetShip = ShipClass::Cast( target );
		if ( targetShip != NULL ) {
			NiTListIterator pos = targetShip->GetSubsystemList().GetHeadPos();
			while ( pos != NULL ) {
				ShipSubsystem *other = targetShip->GetSubsystemList().GetNext( pos );
				if ( other->IsCritical() && other->GetCondition() <= torpDamage ) {
					return true;
				}
			}
		}
	}
	return false;
}

/*
================
OptimizedFireScript::ChooseTorpType

Loads the torpedo type that suits the target's distance and speed best. When
disabled subsystems are not to be attacked, the type that does the least
damage.
================
*/
// FUNCTION: STBC 0x00483e50
// PARKED: register/inline: the ammo's launch speed is copied with integer moves where retail uses fld/fstp, and the ratings list's item handling allocates registers differently; calls and branches agree
void OptimizedFireScript::ChooseTorpType( TorpedoSystem *torpSystem, NiPoint3 *targetLocation, float targetSpeed ) {
	NiTList<int> torpTypes( 2 );
	for ( int type = 0; type < torpSystem->GetProperty()->GetNumAmmoTypes(); type++ ) {
		if ( torpSystem->GetNumAvailableTorpsToType( type ) > 0 ) {
			torpTypes.AddTail( type );
		}
	}
	if ( torpTypes.GetSize() == 0 ) {
		return;
	}
	if ( torpTypes.GetSize() == 1 ) {
		if ( torpSystem->GetAmmoTypeNumber() != torpTypes.GetHead() ) {
			torpSystem->SetAmmoType( torpTypes.GetHead(), true );
		}
		return;
	}

	float distance = NiPoint3::VectorLength( *targetLocation - torpSystem->GetWorldLocation() );

	NiTList<FireTorpRating *> ratings( 2 );
	while ( torpTypes.GetSize() != 0 ) {
		int ammoIndex = torpTypes.RemoveHead();
		TorpedoAmmoType *ammo = torpSystem->GetAmmoType( ammoIndex );
		FireTorpRating *rating = new FireTorpRating;
		rating->ammoType = ammoIndex;
		float launchSpeed = ammo->GetLaunchSpeed();
		float powerCost = ammo->GetPowerCost();
		char *script = ammo->GetTorpedoScript();
		rating->damage = 0.0f;
		float guidanceLifetime = 0.0f;
		float turnRate = 0.0f;
		PyEmbed::CallFunction( script, "GetDamage", "f", &rating->damage, "()" );
		PyErr_Clear();
		PyEmbed::CallFunction( script, "GetGuidanceLifetime", "f", &guidanceLifetime, "()" );
		PyErr_Clear();
		PyEmbed::CallFunction( script, "GetMaxAngularAccel", "f", &turnRate, "()" );
		PyErr_Clear();
		rating->idealDistMin = 0.0f;
		rating->idealDistMax = guidanceLifetime * launchSpeed;
		rating->idealTargetSpeedMin = 0.0f;
		rating->idealTargetSpeedMax = 0.0f;
		if ( guidanceLifetime > 0.0f || launchSpeed > 0.0f ) {
			rating->idealTargetSpeedMax = turnRate * 4.0f + launchSpeed;
		}
		rating->rating = rating->damage - powerCost;
		ratings.AddTail( rating );
	}

	float bestScore = -NI_INFINITY;
	int chosenAmmo = 0;
	NiTListIterator pos = ratings.GetHeadPos();
	while ( pos != NULL ) {
		FireTorpRating *rating = ratings.GetNext( pos );
		float score;
		if ( !attackDisabledSubsystems ) {
			score = -rating->damage;
		} else {
			PyObject *boundaries = PyTuple_New( 4 );
			PyTuple_SetItem( boundaries, 0, PyFloat_FromDouble( 0.0 ) );
			PyTuple_SetItem( boundaries, 1, PyFloat_FromDouble( rating->idealDistMin ) );
			PyTuple_SetItem( boundaries, 2, PyFloat_FromDouble( rating->idealDistMax ) );
			PyTuple_SetItem( boundaries, 3, PyFloat_FromDouble( ( rating->idealDistMax - rating->idealDistMin ) * 1.5f ) );
			PyObject *sets = FuzzyLogic::BreakIntoSets( distance, boundaries );
			double highIdeal = PyFloat_AsDouble( PyTuple_GetItem( sets, 2 ) );
			float distScore = ( PyFloat_AsDouble( PyTuple_GetItem( sets, 1 ) ) + highIdeal ) * rating->rating;
			Py_DECREF( boundaries );
			Py_DECREF( sets );

			boundaries = PyTuple_New( 4 );
			PyTuple_SetItem( boundaries, 0, PyFloat_FromDouble( 0.0 ) );
			PyTuple_SetItem( boundaries, 1, PyFloat_FromDouble( rating->idealTargetSpeedMin ) );
			PyTuple_SetItem( boundaries, 2, PyFloat_FromDouble( rating->idealTargetSpeedMax ) );
			PyTuple_SetItem( boundaries, 3, PyFloat_FromDouble( ( rating->idealTargetSpeedMax - rating->idealTargetSpeedMin ) * 1.5f ) );
			sets = FuzzyLogic::BreakIntoSets( targetSpeed, boundaries );
			highIdeal = PyFloat_AsDouble( PyTuple_GetItem( sets, 2 ) );
			float speedScore = ( PyFloat_AsDouble( PyTuple_GetItem( sets, 1 ) ) + highIdeal ) * rating->rating;
			Py_DECREF( boundaries );
			Py_DECREF( sets );

			score = distScore * 0.25f + speedScore;
		}
		if ( score > bestScore ) {
			bestScore = score;
			chosenAmmo = rating->ammoType;
		}
	}
	while ( ratings.GetSize() != 0 ) {
		delete ratings.RemoveHead();
	}

	if ( torpSystem->GetAmmoTypeNumber() != chosenAmmo ) {
		torpSystem->SetAmmoType( chosenAmmo, true );
	}
}

/*
================
OptimizedFireScript::CheckGoodShot

Whether one of the system's torpedo tubes or pulse weapons points at where the
target will be when the shot arrives, within fireDotThreshold. Systems that
are not aimed always have a good shot.
================
*/
// FUNCTION: STBC 0x00484570
bool OptimizedFireScript::CheckGoodShot( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem ) {
	if ( !system->ShouldBeAimed() ) {
		return true;
	}
	ShipClass *ship = GetShip();
	if ( ship == NULL ) {
		return false;
	}

	// the script's GetWeaponInfo
	float speed = 0.0f;
	NiTList<NiPoint3 *> directions( 2 );
	for ( int i = 0; i < system->GetNumChildSubsystems(); i++ ) {
		Weapon *weapon = Weapon::Cast( system->GetChildSubsystem( i ) );
		if ( weapon != NULL ) {
			TorpedoTube *tube = TorpedoTube::Cast( weapon );
			PulseWeapon *pulse = PulseWeapon::Cast( weapon );
			if ( tube != NULL ) {
				directions.AddTail( new NiPoint3( tube->GetDirection() ) );
			} else if ( pulse != NULL ) {
				directions.AddTail( new NiPoint3( pulse->GetProperty()->GetOrientationForward() ) );
			}
			float weaponSpeed = 0.0f;
			TorpedoSystem *torps = TorpedoSystem::Cast( system );
			if ( torps != NULL ) {
				weaponSpeed = torps->GetCurrentAmmoType()->GetLaunchSpeed();
			} else if ( pulse != NULL ) {
				weaponSpeed = pulse->GetLaunchSpeed();
			}
			if ( weaponSpeed > speed ) {
				speed = weaponSpeed;
			}
		}
	}

	bool goodShot = false;
	NiPoint3 targetLocation = PredictTargetLocation( ship, target, subsystem, speed );
	targetLocation = ship->GetWorldRotation().Transpose() * TGPoint3( targetLocation - ship->GetWorldLocation() );
	float distance = targetLocation.Unitize();
	if ( distance < speed * 60.0f ) {
		while ( directions.GetSize() != 0 ) {
			NiPoint3 *direction = directions.RemoveHead();
			if ( !goodShot && direction->Dot( targetLocation ) > fireDotThreshold ) {
				goodShot = true;
			}
			delete direction;
		}
	}
	return goodShot;
}

/*
================
OptimizedFireScript::PredictTargetLocation

Where the target, or its subsystem, will be when a shot of the speed arrives.
================
*/
// FUNCTION: STBC 0x00484a90
// DRAFT: the length now inlines as retail's; retail has an EH frame (a destructed local) and a 0x64 frame, and computes the difference's x, y, z in order where ours starts at z
NiPoint3 OptimizedFireScript::PredictTargetLocation( ShipClass *ship, ObjectClass *target, ShipSubsystem *subsystem, float speed ) {
	NiPoint3 location;
	if ( subsystem != NULL ) {
		location = subsystem->GetWorldLocation();
	} else {
		location = target->GetWorldLocation();
	}
	if ( speed == 0.0f ) {
		return location;
	}
	float time = NiPoint3::VectorLength( location - ship->GetWorldLocation() ) / speed;

	TGPoint3 predicted = location;
	PhysicsObjectClass *physicsTarget = PhysicsObjectClass::Cast( target );
	if ( physicsTarget != NULL ) {
		TGPoint3 position = physicsTarget->GetWorldLocation();
		predicted = physicsTarget->PredictPosition( position, physicsTarget->GetVelocity(), physicsTarget->GetAcceleration(), time );
		if ( subsystem != NULL ) {
			NiPoint3 offset = subsystem->GetDamagePoint();
			predicted += physicsTarget->GetPredictedRotation( time ) * offset;
		}
	}
	return predicted;
}

/*
================
OptimizedFireScript::GetTarget

The object of the target's name in the ship's set; NULL for none.
================
*/
// FUNCTION: STBC 0x00484cf0
ObjectClass *OptimizedFireScript::GetTarget( void ) {
	ShipClass *ship = GetShip();
	if ( ship == NULL ) {
		return NULL;
	}
	SetClass *set = ship->GetContainingSet();
	if ( set == NULL ) {
		return NULL;
	}
	return ObjectClass::GetObject( set, target );
}

/*
================
OptimizedFireScript::StopFiring

Called first by every function that changes the targeting setup.
================
*/
// FUNCTION: STBC 0x00484d40
void OptimizedFireScript::StopFiring( void ) {
	ObjectClass *target = GetTarget();
	if ( target != NULL ) {
		NiTListIterator pos = weapons.GetHeadPos();
		while ( pos != NULL ) {
			weapons.GetNext( pos )->StopFiringAtTarget( target );
		}
	}
}

/*
================
OptimizedFireScript::CallUsingWeaponTypeFunc

Tells the AIs of the tree which weapon systems are in use, when that changed.
================
*/
// FUNCTION: STBC 0x00484d70
void OptimizedFireScript::CallUsingWeaponTypeFunc( void ) {
	if ( !callUsingWeaponTypeFunc ) {
		return;
	}
	NiTList<ArtificialIntelligence *> ais( 2 );
	GetAllAIsInTree( ais );
	if ( ais.GetSize() != 0 ) {
		int numSystems = weapons.GetSize();
		NiTListIterator pos;
		if ( torpsTooDangerous ) {
			numSystems = 0;
			pos = weapons.GetHeadPos();
			while ( pos != NULL ) {
				if ( !weapons.GetNext( pos )->IsTypeOf( TORPEDOSYSTEM_TYPE ) ) {
					numSystems++;
				}
			}
		}
		PyObject *list = PyList_New( numSystems );
		int index = 0;
		pos = weapons.GetHeadPos();
		while ( pos != NULL ) {
			WeaponSystem *system = weapons.GetNext( pos );
			if ( !torpsTooDangerous || !system->IsTypeOf( TORPEDOSYSTEM_TYPE ) ) {
				PyList_SetItem( list, index++, system->VirtualSlot08() );
			}
		}
		while ( ais.GetSize() != 0 ) {
			ais.RemoveHead()->CallExternalFunction( "UsingWeaponType", list );
		}
		Py_DECREF( list );
	}
	callUsingWeaponTypeFunc = false;
}

/*
================
ObjectGroupOf

The ObjectGroup of a Python shadow instance (invented name).
================
*/
inline ObjectGroup *ObjectGroupOf( PyObject *instance ) {
	ObjectGroup *group = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&group, "ObjectGroup" );
	return group;
}

/*
================
OptimizedFireScript::TargetVisible

In range, and no ship of the ship's own side (the mission's friendly or enemy
group, whichever names the ship) in the line of fire.
================
*/
// FUNCTION: STBC 0x00484f10
bool OptimizedFireScript::TargetVisible( ShipClass *ship, ObjectClass *target ) {
	NiPoint3 diff = target->GetWorldLocation() - ship->GetWorldLocation();
	float distance = NiPoint3::VectorLength( diff );
	if ( distance > maxFiringRange ) {
		return false;
	}
	if ( ship->GetName() != NULL && Game::GetCurrentGame() != NULL ) {
		Episode *episode = Game::GetCurrentGame()->GetCurrentEpisode();
		if ( episode != NULL ) {
			Mission *mission = episode->GetCurrentMission();
			if ( mission != NULL ) {
				ObjectGroup *group;
				if ( ObjectGroupOf( mission->GetFriendlyGroup() )->IsNameInGroup( ship->GetName() ) ) {
					group = ObjectGroupOf( mission->GetFriendlyGroup() );
				} else if ( ObjectGroupOf( mission->GetEnemyGroup() )->IsNameInGroup( ship->GetName() ) ) {
					group = ObjectGroupOf( mission->GetEnemyGroup() );
				} else {
					return true;
				}
				if ( group != NULL && IsLineOfFireBlocked( ship, target->GetWorldLocation(), group, target ) ) {
					return false;
				}
			}
		}
	}
	return true;
}

/*
================
OptimizedFireScript::IsLineOfFireBlocked
================
*/
// FUNCTION: STBC 0x004850d0
bool OptimizedFireScript::IsLineOfFireBlocked( ShipClass *ship, const NiPoint3 &point, ObjectGroup *group, ObjectClass *target ) {
	SetClass *set = ship->GetContainingSet();
	if ( set == NULL ) {
		return false;
	}
	ProximityManager *proximity = set->GetProximityManager();
	if ( proximity == NULL ) {
		return false;
	}
	bool blocked = false;
	float radius = ship->GetWorldBound().GetRadius();
	ProximityManager::ObjectIterator *iterator = proximity->GetLineIntersectObjects( (const NiPoint3 &)ship->GetWorldLocation(), point, radius, false );
	while ( true ) {
		ObjectClass *object = proximity->GetNextObject( iterator );
		if ( object == NULL ) {
			break;
		}
		if ( object != ship && object != target && ShipClass::Cast( object ) != NULL && object->GetName() != NULL
				&& group->IsNameInGroup( object->GetName() ) ) {
			// The end point is a temporary: retail destroys it before the test.
			// TODO structure: retail calls the NiPoint3 GetLineIntersectObjects with the
			// ship's location as is, which the TGPoint3 overload in ProximityManager.h
			// makes ambiguous without the cast above.
			// MATCH-PENDING: R17, the address of the TGPoint3( point ) temporary is taken only to reproduce retail's destroyed-before-the-test copy; no other evidence for the spelling yet (user's ruling, 2026-11-18)
			if ( object->LineCollides( (TGPoint3 *)&ship->GetWorldLocation(), &TGPoint3( point ) ) ) {
				blocked = true;
				break;
			}
		}
	}
	proximity->EndObjectIteration( iterator );
	return blocked;
}

/*
================
OptimizedFireScript::ChooseTargetSubsystem

By the listed kinds of subsystems, by the ratings, or the subsystem the ship
targets; the choice goes to targetedSubsystemID.
================
*/
// FUNCTION: STBC 0x00485230
void OptimizedFireScript::ChooseTargetSubsystem( ShipClass *ship, ObjectClass *target ) {
	ShipClass *targetShip = ShipClass::Cast( target );
	if ( targetShip == NULL ) {
		return;
	}
	ShipSubsystem *subsystem;
	if ( !ignoreSubsystemTargets && targetSubsystems.GetSize() != 0 ) {
		subsystem = ChooseListedSubsystem( ship, targetShip );
	} else if ( chooseSubsystemTargets ) {
		subsystem = ChooseRatedSubsystem( ship, targetShip );
	} else {
		subsystem = ship->GetTargetSubsystem();
		if ( hullSelectedChooseAlternate && subsystem != NULL && subsystem->IsTypeOf( HULLCLASS_TYPE ) ) {
			subsystem = ChooseRatedSubsystem( ship, targetShip );
		} else if ( subsystem != NULL && subsystem->IsDisabled() && !attackDisabledSubsystems ) {
			// a sibling that still works
			ShipSubsystem *parent = subsystem->GetParentSubsystem();
			ShipSubsystem *sibling = NULL;
			if ( parent != NULL && parent->GetNumChildSubsystems() > 1 ) {
				for ( int i = 0; i < parent->GetNumChildSubsystems(); i++ ) {
					sibling = parent->GetChildSubsystem( i );
					if ( !sibling->IsDisabled() ) {
						break;
					}
					sibling = NULL;
				}
			}
			subsystem = sibling;
		}
	}
	targetedSubsystemID = subsystem != NULL ? subsystem->GetObjID() : 0;
}

/*
================
OptimizedFireScript::ChooseListedSubsystem

The target's subsystems of the listed kinds, one priority at a time until
there are candidates. The match iterators are not freed.
================
*/
// FUNCTION: STBC 0x00485360
// DRAFT: first write, not diagnosed (list clean-up paths differ)
ShipSubsystem *OptimizedFireScript::ChooseListedSubsystem( ShipClass *ship, ShipClass *targetShip ) {
	float currentPriority = targetSubsystems.GetHead()->priority;
	int numDisabled = 0;
	NiTList<ShipSubsystem *> possibleTargets( 2 );
	NiTListIterator pos = targetSubsystems.GetHeadPos();
	while ( pos != NULL ) {
		FireTargetSubsystem *targetSubsystem = targetSubsystems.GetNext( pos );
		if ( possibleTargets.GetSize() != 0 && targetSubsystem->priority < currentPriority ) {
			if ( !disableFirst || numDisabled < (int)possibleTargets.GetSize() ) {
				break;
			}
		}
		currentPriority = targetSubsystem->priority;
		ShipClass::MatchIterator *iterator = targetShip->StartGetSubsystemMatch( targetSubsystem->type );
		ShipSubsystem *subsystem;
		while ( ( subsystem = targetShip->GetNextSubsystemMatch( iterator ) ) != NULL ) {
			if ( subsystem->GetCondition() > 0.0f && ( attackDisabledSubsystems || !subsystem->IsDisabled() ) ) {
				if ( !subsystem->IsTargetable() ) {
					numDisabled += GetChildTargets( subsystem, &possibleTargets, attackDisabledSubsystems );
				} else {
					possibleTargets.AddTail( subsystem );
					if ( subsystem->IsDisabled() ) {
						numDisabled++;
					}
				}
			}
		}
	}
	if ( possibleTargets.GetSize() == 0 ) {
		return NULL;
	}
	if ( disableFirst ) {
		NiTList<ShipSubsystem *> nonDisabled( 2 );
		pos = possibleTargets.GetHeadPos();
		while ( pos != NULL ) {
			ShipSubsystem *subsystem = possibleTargets.GetNext( pos );
			if ( !subsystem->IsDisabled() ) {
				nonDisabled.AddTail( subsystem );
			}
		}
		if ( nonDisabled.GetSize() != 0 ) {
			return PickTargetSubsystemFromList( &nonDisabled );
		}
	}
	return PickTargetSubsystemFromList( &possibleTargets );
}

/*
================
OptimizedFireScript::ChooseRatedSubsystem

Rates a few of the target's targetable subsystems each call, round after
round, and returns the best rated one.
================
*/
// FUNCTION: STBC 0x004856e0
// DRAFT: the forget loop agrees; retail keeps the rounds loop unrotated, merges the two AddTail tails and calls the last loop's GetAt out of line (budget 132 at [32], needs 81..103)
ShipSubsystem *OptimizedFireScript::ChooseRatedSubsystem( ShipClass *ship, ShipClass *targetShip ) {
	NiTList<ShipSubsystem *> targetable( 2 );
	NiTListIterator pos = targetShip->GetSubsystemList().GetHeadPos();
	while ( pos != NULL ) {
		GetTargetableSubsystems( targetShip->GetSubsystemList().GetNext( pos ), &targetable );
	}
	if ( !attackDisabledSubsystems ) {
		pos = targetable.GetHeadPos();
		while ( pos != NULL ) {
			NiTListIterator current = pos;
			if ( targetable.GetNext( pos )->IsDisabled() ) {
				targetable.RemovePos( current );
			}
		}
	}

	// forget the ratings of the subsystems that are gone
	NiTMapIterator ratingPos = targetSubsystemRating.GetFirstPos();
	while ( ratingPos != NULL ) {
		long id;
		FireSubsystemRating *rating;
		targetSubsystemRating.GetNext( ratingPos, id, rating );
		bool exists = false;
		pos = targetable.GetHeadPos();
		while ( pos != NULL ) {
			if ( targetable.GetNext( pos )->GetObjID() == id ) {
				exists = true;
				break;
			}
		}
		if ( !exists ) {
			targetSubsystemRating.RemoveAt( id );
		}
	}

	// the subsystems not rated in this round; the next round when all are
	FireSubsystemRating *rating;
	NiTList<ShipSubsystem *> needUpdates( 2 );
	while ( targetable.GetSize() != 0 ) {
		pos = targetable.GetHeadPos();
		while ( pos != NULL ) {
			ShipSubsystem *subsystem = targetable.GetNext( pos );
			if ( !targetSubsystemRating.GetAt( subsystem->GetObjID(), rating ) ) {
				needUpdates.AddTail( subsystem );
			} else if ( rating->updateNum != targetSubsystemUpdateNum ) {
				needUpdates.AddTail( subsystem );
			}
		}
		if ( needUpdates.GetSize() != 0 ) {
			break;
		}
		targetSubsystemUpdateNum++;
	}
	while ( needUpdates.GetSize() > NUM_SUBSYSTEMS_TO_UPDATE ) {
		int index = g_kSystemWrapper.GetRandomNumber( needUpdates.GetSize() );
		pos = needUpdates.GetHeadPos();
		while ( pos != NULL ) {
			if ( index-- == 0 ) {
				needUpdates.RemovePos( pos );
				break;
			}
			pos = needUpdates.GetNextPos( pos );
		}
	}
	pos = needUpdates.GetHeadPos();
	while ( pos != NULL ) {
		ShipSubsystem *subsystem = needUpdates.GetNext( pos );
		rating = NULL;
		if ( !targetSubsystemRating.GetAt( subsystem->GetObjID(), rating ) ) {
			rating = new FireSubsystemRating;
		}
		rating->updateNum = targetSubsystemUpdateNum;
		rating->rating = RateSubsystemForTargeting( ship, targetShip, subsystem );
		targetSubsystemRating.SetAt( subsystem->GetObjID(), rating );
	}

	float highestRating = -NI_INFINITY;
	long bestID = 0;
	long id;
	ratingPos = targetSubsystemRating.GetFirstPos();
	while ( ratingPos != NULL ) {
		rating = NULL;
		targetSubsystemRating.GetNext( ratingPos, id, rating );
		if ( rating->rating > highestRating ) {
			highestRating = rating->rating;
			bestID = id;
		}
	}
	return ShipSubsystem::Cast( TGObject::GetTGObjectPtr( bestID ) );
}

/*
================
OptimizedFireScript::GetChildTargets
================
*/
// FUNCTION: STBC 0x00485c60
int OptimizedFireScript::GetChildTargets( ShipSubsystem *subsystem, NiTList<ShipSubsystem *> *targets, bool attackDisabled ) {
	int numDisabled = 0;
	for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
		ShipSubsystem *child = subsystem->GetChildSubsystem( i );
		if ( attackDisabled || !child->IsDisabled() ) {
			if ( child->IsTargetable() ) {
				targets->AddTail( child );
				if ( child->IsDisabled() ) {
					numDisabled++;
				}
			} else {
				numDisabled += GetChildTargets( child, targets, attackDisabled );
			}
		}
	}
	return numDisabled;
}

/*
================
OptimizedFireScript::PickTargetSubsystemFromList

The first one; the script rates them.
================
*/
// FUNCTION: STBC 0x00485d40
ShipSubsystem *OptimizedFireScript::PickTargetSubsystemFromList( NiTList<ShipSubsystem *> *subsystems ) {
	if ( subsystems->GetSize() == 0 ) {
		return NULL;
	}
	return subsystems->GetHead();
}

/*
================
OptimizedFireScript::GetTargetableSubsystems

Adds the subsystem if it can be targeted, else its targetable descendants;
nothing of a destroyed one.
================
*/
// FUNCTION: STBC 0x00485d60
void OptimizedFireScript::GetTargetableSubsystems( ShipSubsystem *subsystem, NiTList<ShipSubsystem *> *targetable ) {
	if ( subsystem->GetCondition() > 0.0f ) {
		if ( subsystem->IsTargetable() ) {
			targetable->AddTail( subsystem );
		} else {
			for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
				ShipSubsystem *child = subsystem->GetChildSubsystem( i );
				GetTargetableSubsystems( child, targetable );
			}
		}
	}
}

/*
================
OptimizedFireScript::RateSubsystemForTargeting
================
*/
// FUNCTION: STBC 0x00485e40
float OptimizedFireScript::RateSubsystemForTargeting( ShipClass *ship, ShipClass *targetShip, ShipSubsystem *subsystem ) {
	float critical = subsystem->IsCritical();
	float damageToDisable = 0.0f;
	float maxCondition = subsystem->GetMaxCondition();
	float disabledCondition = subsystem->GetDisabledPercentage() * maxCondition;
	float condition = subsystem->GetCondition();
	if ( condition > disabledCondition ) {
		damageToDisable = condition - disabledCondition;
	}
	float isDisabled = subsystem->IsDisabled();
	float hittable = subsystem->IsHittableFromLocation( (TGPoint3 *)&ship->GetWorldLocation() );

	float systemRating = 0.0f;
	if ( subsystem->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
		systemRating = 5.0f;
	} else if ( subsystem->IsTypeOf( WEAPON_TYPE ) ) {
		systemRating = 5.0f;
	} else if ( subsystem->IsTypeOf( SHIELDCLASS_TYPE ) ) {
		systemRating = 5.0f;
	} else if ( subsystem->IsTypeOf( CLOAKINGSUBSYSTEM_TYPE ) ) {
		systemRating = 4.0f;
	} else if ( subsystem->IsTypeOf( IMPULSEENGINESUBSYSTEM_TYPE ) ) {
		systemRating = 3.0f;
	} else if ( subsystem->IsTypeOf( HULLCLASS_TYPE ) ) {
		systemRating = -200.0f;
	}

	return critical * 6.0f - ( (float)disableFirst * isDisabled * 3.0f + ( condition + damageToDisable ) * 0.0005f )
		+ hittable + systemRating;
}
