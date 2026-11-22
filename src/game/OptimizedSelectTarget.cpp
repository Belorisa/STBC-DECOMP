/*
===============================================================================

	OptimizedSelectTarget.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "OptimizedSelectTarget.h"
#include "CloakingSubsystem.h"
#include "DamageableObject.h"
#include "SensorSubsystem.h"
#include "HullClass.h"
#include "ObjectClass.h"
#include "ObjectGroup.h"
#include "ShieldClass.h"
#include "WeaponSystem.h"
#include "ObjectGroupWithInfo.h"
#include "PhysicsObjectClass.h"
#include "PyEmbed.h"
#include "SetClass.h"
#include "ShipClass.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "UtopiaModule.h"
#include "WeaponHitEvent.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0048b3f0 _$E529
// FUNCTION: STBC 0x0048b410 _$E528

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004881f0 _$E32
// FUNCTION: STBC 0x00488200 _$E29
// FUNCTION: STBC 0x00488210 _$E31
// FUNCTION: STBC 0x00488220 _$E30
// FUNCTION: STBC 0x00488230 _$E38
// FUNCTION: STBC 0x00488240 _$E35
// FUNCTION: STBC 0x00488250 _$E37
// FUNCTION: STBC 0x00488260 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00488270 _$E105
// FUNCTION: STBC 0x00488280 _$E104
// FUNCTION: STBC 0x00488290 _$E328
// FUNCTION: STBC 0x004882a0 _$E327
// FUNCTION: STBC 0x00488880 _$E116
// FUNCTION: STBC 0x00488890 _$E115
// FUNCTION: STBC 0x00488d80 _$E120
// FUNCTION: STBC 0x00488d90 _$E119
// FUNCTION: STBC 0x00489450 _$E124
// FUNCTION: STBC 0x00489460 _$E123
// FUNCTION: STBC 0x00489550 _$E128
// FUNCTION: STBC 0x00489560 _$E127
// FUNCTION: STBC 0x00489bb0 _$E243
// FUNCTION: STBC 0x00489bc0 _$E242
// FUNCTION: STBC 0x0048a930 _$E185
// FUNCTION: STBC 0x0048a940 _$E184
// FUNCTION: STBC 0x0048a950 _$E198
// FUNCTION: STBC 0x0048a960 _$E197
// FUNCTION: STBC 0x0048a970 _$E248
// FUNCTION: STBC 0x0048a980 _$E247
// FUNCTION: STBC 0x0048a990 _$E333
// FUNCTION: STBC 0x0048a9a0 _$E332
// FUNCTION: STBC 0x0048a9b0 _$E263
// FUNCTION: STBC 0x0048a9c0 _$E262
// FUNCTION: STBC 0x0048a9d0 _$E337
// FUNCTION: STBC 0x0048a9e0 _$E336
// FUNCTION: STBC 0x0048a9f0 _$E344
// FUNCTION: STBC 0x0048aa00 _$E343
// FUNCTION: STBC 0x0048aa10 _$E362
// FUNCTION: STBC 0x0048aa20 _$E361
// FUNCTION: STBC 0x0048aa30 _$E366
// FUNCTION: STBC 0x0048aa40 _$E365
// FUNCTION: STBC 0x0048aa50 _$E147
// FUNCTION: STBC 0x0048aa60 _$E146
// FUNCTION: STBC 0x0048aa70 _$E279
// FUNCTION: STBC 0x0048aa80 _$E278
// FUNCTION: STBC 0x0048aa90 _$E286
// FUNCTION: STBC 0x0048aaa0 _$E285
// FUNCTION: STBC 0x0048aab0 _$E290
// FUNCTION: STBC 0x0048aac0 _$E289
// FUNCTION: STBC 0x0048aad0 _$E294
// FUNCTION: STBC 0x0048aae0 _$E293
// FUNCTION: STBC 0x0048aaf0 _$E298
// FUNCTION: STBC 0x0048ab00 _$E297
// FUNCTION: STBC 0x0048ab10 _$E304
// FUNCTION: STBC 0x0048ab20 _$E303
// FUNCTION: STBC 0x0048ab30 _$E252
// FUNCTION: STBC 0x0048ab40 _$E251
// FUNCTION: STBC 0x0048ab50 _$E256
// FUNCTION: STBC 0x0048ab60 _$E255
// FUNCTION: STBC 0x0048ab70 _$E370
// FUNCTION: STBC 0x0048ab80 _$E369
// FUNCTION: STBC 0x0048ab90 _$E374
// FUNCTION: STBC 0x0048aba0 _$E373
// FUNCTION: STBC 0x0048abb0 _$E378
// FUNCTION: STBC 0x0048abc0 _$E377
// FUNCTION: STBC 0x0048abd0 _$E382
// FUNCTION: STBC 0x0048abe0 _$E381
// FUNCTION: STBC 0x0048abf0 _$E386
// FUNCTION: STBC 0x0048ac00 _$E385
// FUNCTION: STBC 0x0048ac10 _$E390
// FUNCTION: STBC 0x0048ac20 _$E389
// FUNCTION: STBC 0x0048ac30 _$E394
// FUNCTION: STBC 0x0048ac40 _$E393
// FUNCTION: STBC 0x0048ac50 _$E398
// FUNCTION: STBC 0x0048ac60 _$E397
// FUNCTION: STBC 0x0048ac70 _$E152
// FUNCTION: STBC 0x0048ac80 _$E151
// FUNCTION: STBC 0x0048ac90 _$E402
// FUNCTION: STBC 0x0048aca0 _$E401
// FUNCTION: STBC 0x0048acb0 _$E157
// FUNCTION: STBC 0x0048acc0 _$E156
// FUNCTION: STBC 0x0048acd0 _$E406
// FUNCTION: STBC 0x0048ace0 _$E405
// FUNCTION: STBC 0x0048acf0 _$E410
// FUNCTION: STBC 0x0048ad00 _$E409
// FUNCTION: STBC 0x0048ad10 _$E414
// FUNCTION: STBC 0x0048ad20 _$E413
// FUNCTION: STBC 0x0048ad30 _$E418
// FUNCTION: STBC 0x0048ad40 _$E417
// FUNCTION: STBC 0x0048ad50 _$E162
// FUNCTION: STBC 0x0048ad60 _$E161
// FUNCTION: STBC 0x0048ad70 _$E422
// FUNCTION: STBC 0x0048ad80 _$E421
// FUNCTION: STBC 0x0048ad90 _$E426
// FUNCTION: STBC 0x0048ada0 _$E425
// FUNCTION: STBC 0x0048adb0 _$E430
// FUNCTION: STBC 0x0048adc0 _$E429
// FUNCTION: STBC 0x0048add0 _$E168
// FUNCTION: STBC 0x0048ade0 _$E167
// FUNCTION: STBC 0x0048adf0 _$E173
// FUNCTION: STBC 0x0048ae00 _$E172
// FUNCTION: STBC 0x0048ae10 _$E203
// FUNCTION: STBC 0x0048ae20 _$E202
// FUNCTION: STBC 0x0048ae30 _$E434
// FUNCTION: STBC 0x0048ae40 _$E433
// FUNCTION: STBC 0x0048ae50 _$E208
// FUNCTION: STBC 0x0048ae60 _$E207
// FUNCTION: STBC 0x0048ae70 _$E438
// FUNCTION: STBC 0x0048ae80 _$E437
// FUNCTION: STBC 0x0048ae90 _$E442
// FUNCTION: STBC 0x0048aea0 _$E441
// FUNCTION: STBC 0x0048aeb0 _$E213
// FUNCTION: STBC 0x0048aec0 _$E212
// FUNCTION: STBC 0x0048aed0 _$E446
// FUNCTION: STBC 0x0048aee0 _$E445
// FUNCTION: STBC 0x0048aef0 _$E178
// FUNCTION: STBC 0x0048af00 _$E177
// FUNCTION: STBC 0x0048af70 _$E450
// FUNCTION: STBC 0x0048af80 _$E449
// FUNCTION: STBC 0x0048b000 _$E454
// FUNCTION: STBC 0x0048b010 _$E453
// FUNCTION: STBC 0x0048b080 _$E218
// FUNCTION: STBC 0x0048b090 _$E217
// FUNCTION: STBC 0x0048b0a0 _$E223
// FUNCTION: STBC 0x0048b0b0 _$E222
// FUNCTION: STBC 0x0048b0c0 _$E458
// FUNCTION: STBC 0x0048b0d0 _$E457
// FUNCTION: STBC 0x0048b0e0 _$E462
// FUNCTION: STBC 0x0048b0f0 _$E461
// FUNCTION: STBC 0x0048b100 _$E466
// FUNCTION: STBC 0x0048b110 _$E465
// FUNCTION: STBC 0x0048b120 _$E470
// FUNCTION: STBC 0x0048b130 _$E469
// FUNCTION: STBC 0x0048b140 _$E474
// FUNCTION: STBC 0x0048b150 _$E473
// FUNCTION: STBC 0x0048b160 _$E478
// FUNCTION: STBC 0x0048b170 _$E477
// FUNCTION: STBC 0x0048b180 _$E482
// FUNCTION: STBC 0x0048b190 _$E481
// FUNCTION: STBC 0x0048b1a0 _$E486
// FUNCTION: STBC 0x0048b1b0 _$E485
// FUNCTION: STBC 0x0048b1c0 _$E228
// FUNCTION: STBC 0x0048b1d0 _$E227
// FUNCTION: STBC 0x0048b1e0 _$E490
// FUNCTION: STBC 0x0048b1f0 _$E489
// FUNCTION: STBC 0x0048b200 _$E233
// FUNCTION: STBC 0x0048b210 _$E232
// FUNCTION: STBC 0x0048b220 _$E238
// FUNCTION: STBC 0x0048b230 _$E237
// FUNCTION: STBC 0x0048b490 _$E494
// FUNCTION: STBC 0x0048b4a0 _$E493
// FUNCTION: STBC 0x0048b4b0 _$E499
// FUNCTION: STBC 0x0048b4c0 _$E498
// FUNCTION: STBC 0x0048b4d0 _$E503
// FUNCTION: STBC 0x0048b4e0 _$E502

// FUNCTION: STBC 0x004882b0 _$E506
// FUNCTION: STBC 0x004882c0 _$E505
static AICreateFunctionRegistration selectTargetRegistration( AI_TYPE_OPTIMIZED_SELECT_TARGET, OptimizedSelectTarget::Create );

// FUNCTION: STBC 0x00488310 _$E509
// FUNCTION: STBC 0x00488320 _$E508
static OptimizedVersionRegistration selectTargetVersion( "SelectTarget", OptimizedSelectTarget::CreateFromScript );

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004883e0 _$E515
// FUNCTION: STBC 0x004883f0 _$E511
// FUNCTION: STBC 0x00488400 _$E514
// FUNCTION: STBC 0x00488410 _$E512
// DATA: STBC 0x00982668
TGEventHandlerTable OSTEventHandler::classHandlers;

// The module's construction and exit-time destruction.
// FUNCTION: STBC 0x00488430 _$E520
// FUNCTION: STBC 0x00488440 _$E517
// FUNCTION: STBC 0x004887b0 _$E519
// FUNCTION: STBC 0x004887c0 _$E518
// DATA: STBC 0x009825f8
OSTEventHandlerModule ostEventHandlerModule;		// invented name

// The distance at which a target's nearness rating reaches 1 (invented name).
static const float RATING_DISTANCE = 30.0f;

// The seconds between two choices of target (the script's
// fNormalUpdateTime), while the target info is being updated, and
// while there is no target.
static const float NORMAL_UPDATE_TIME = 5.0f;
static const float UPDATING_TARGET_INFO_TIME = 0.0f;
static const float NO_TARGET_UPDATE_TIME = 45.0f;


/*
================
OSTEventHandlerModule::VirtualSlot00

The startup: the creator, the handler table and the handler names.
================
*/
// FUNCTION: STBC 0x00488470
void OSTEventHandlerModule::VirtualSlot00( void ) {
	TGObject::RegisterCreator( CT_OST_EVENT_HANDLER, CreateOSTEventHandler );
	OSTEventHandler::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( OSTEventHandler::GetClassTable() );

	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OSTEventHandler::TargetListChanged, "OSTEventHandler::TargetListChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OSTEventHandler::TargetEnteredSet, "OSTEventHandler::TargetEnteredSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OSTEventHandler::ObjectDecloaked, "OSTEventHandler::ObjectDecloaked" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OSTEventHandler::OurShipEnteredSet, "OSTEventHandler::OurShipEnteredSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OSTEventHandler::DamageEvent, "OSTEventHandler::DamageEvent" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OSTEventHandler::TargetGone, "OSTEventHandler::TargetGone" );
}

/*
================
OSTEventHandlerModule::CreateOSTEventHandler
================
*/
// FUNCTION: STBC 0x00488620
TGObject *OSTEventHandlerModule::CreateOSTEventHandler( long id ) {
	return new OSTEventHandler( id );
}

/*
================
OSTEventHandler::TargetListChanged
================
*/
// FUNCTION: STBC 0x00488700
void OSTEventHandler::TargetListChanged( TGEvent *event ) {
	ai->ForceTargetUpdate();
}

/*
================
OSTEventHandler::TargetEnteredSet
================
*/
// FUNCTION: STBC 0x00488710
void OSTEventHandler::TargetEnteredSet( TGEvent *event ) {
	ai->ForceTargetUpdate();
}

/*
================
OSTEventHandler::OurShipEnteredSet
================
*/
// FUNCTION: STBC 0x00488720
void OSTEventHandler::OurShipEnteredSet( TGEvent *event ) {
	ai->ForceTargetUpdate();
}

/*
================
OSTEventHandler::TargetGone
================
*/
// FUNCTION: STBC 0x00488730
void OSTEventHandler::TargetGone( TGEvent *event ) {
	ai->ForceTargetUpdate();
}

/*
================
OSTEventHandler::ObjectDecloaked
================
*/
// FUNCTION: STBC 0x00488740
void OSTEventHandler::ObjectDecloaked( TGEvent *event ) {
	ai->ObjectDecloaked( event );
}

/*
================
OSTEventHandler::DamageEvent
================
*/
// FUNCTION: STBC 0x00488750
void OSTEventHandler::DamageEvent( TGEvent *event ) {
	ai->DamageEvent( (WeaponHitEvent *)event );
}

/*
================
OSTEventHandlerModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x004887a0
void OSTEventHandlerModule::VirtualSlot05( void ) {
}

/*
================
OSTEventHandler::WriteToStream

The AI is written as its ID.
================
*/
// FUNCTION: STBC 0x004887e0
bool OSTEventHandler::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( ai->GetID() );
	return true;
}

/*
================
OSTEventHandler::ReadFromStream

The AI's ID waits in the pointer until PostFixUp.
================
*/
// FUNCTION: STBC 0x00488810
bool OSTEventHandler::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}
	ai = (OptimizedSelectTarget *)stream->ReadID();
	return true;
}

/*
================
OSTEventHandler::FixUpPointers
================
*/
// FUNCTION: STBC 0x00488840
bool OSTEventHandler::FixUpPointers( void ) {
	return ArtificialIntelligence::GetAIByID( (long)ai ) != NULL ? true : false;
}

/*
================
OSTEventHandler::PostFixUp
================
*/
// FUNCTION: STBC 0x00488860
void OSTEventHandler::PostFixUp( void ) {
	ai = (OptimizedSelectTarget *)ArtificialIntelligence::GetAIByID( (long)ai );
}

/*
================
OptimizedSelectTarget::OptimizedSelectTarget
================
*/
// FUNCTION: STBC 0x004888a0 ??0OptimizedSelectTarget@@QAE@PAVPreprocessingAI@@@Z
// PARKED: register allocation: retail keeps the script instance in EBX over the spent scripted AI and the list in EDI; this build swaps them (instance local, top-declared locals and member reads tried); the demo's copy of this code (stbcdemo 0x488740, same 1157 bytes) differs from retail only in where one load is scheduled, so retail's allocation here flips between builds
OptimizedSelectTarget::OptimizedSelectTarget( PreprocessingAI *ai ) : PreprocessingAI( ai->GetObject(), ai->name ), pendingSetTargetAIs( 2 ), damageTaken( 37 ) {
	eventHandler = new OSTEventHandler( this );
	containedAI = ai->containedAI;
	ai->containedAI = NULL;
	containedAI->SetParentID( GetID() );
	interruptable = ai->interruptable;

	PyObject *instance = ai->preprocessingInstance;
	targetGroupInstance = PyObject_GetAttrString( instance, "pTargetGroup" );
	targetGroup = (ObjectGroup *)PyEmbed::GetObjectPointer( targetGroupInstance );
	PyEmbed::GetStringAttribute( instance, "sCurrentTarget", &currentTarget );
	currentTargetID = 0;

	PyObject *trees = PyObject_GetAttrString( instance, "lAdditionalSetTargetAITrees" );
	if ( trees != NULL && PyList_Check( trees ) ) {
		for ( int i = 0; i < PyList_Size( trees ); i++ ) {
			long id = PyInt_AsLong( PyList_GetItem( trees, i ) );
			setTargetTrees.Add( new long( id ) );
		}
	}
	// BUG: the reference is dropped without a NULL test; a script without
	// lAdditionalSetTargetAITrees crashes here
	Py_DECREF( trees );

	PyEmbed::GetFloatAttribute( instance, "fUpdateTime", &updateTime );
	PyEmbed::GetBoolAttribute( instance, "bSetShipTarget", &setShipTarget );
	PyEmbed::GetBoolAttribute( instance, "bCallSetTargetFuncsWithNoTarget", &callSetTargetFuncsWithNoTarget );
	PyEmbed::GetBoolAttribute( instance, "bCallSetTargetFunctions", &callSetTargetFunctions );
	PyEmbed::GetBoolAttribute( instance, "bUpdateInfoOnCodeAISet", &updateInfoOnCodeAISet );
	PyEmbed::GetBoolAttribute( instance, "bUpdatingTargetInfo", &updatingTargetInfo );
	PyEmbed::GetBoolAttribute( instance, "bIgnoreSensors", &ignoreSensors );
	PyEmbed::GetIntAttribute( instance, "eNoTargetPreprocessStatus", &noTargetPreprocessStatus );
	PyEmbed::GetIntAttribute( instance, "eNoTargetNoShipTargetPreprocessStatus", &noTargetNoShipTargetPreprocessStatus );
	PyEmbed::GetFloatAttribute( instance, "fDistanceFactor", &distanceFactor );
	PyEmbed::GetFloatAttribute( instance, "fInFrontFactor", &inFrontFactor );
	PyEmbed::GetFloatAttribute( instance, "fIsTargetFactor", &isTargetFactor );
	PyEmbed::GetFloatAttribute( instance, "fShieldFactor", &shieldFactor );
	PyEmbed::GetFloatAttribute( instance, "fWeaponsFactor", &weaponsFactor );
	PyEmbed::GetFloatAttribute( instance, "fHullFactor", &hullFactor );
	PyEmbed::GetFloatAttribute( instance, "fDamageFactor", &damageFactor );
	PyEmbed::GetFloatAttribute( instance, "fPriorityFactor", &priorityFactor );
	PyEmbed::GetFloatAttribute( instance, "fPopularityFactor", &popularityFactor );

	replacementSetTargetModule = replacementSetTargetFunction = NULL;
	if ( !PyEmbed::GetStringAttribute( instance, "sReplacementSetTargetModule", &replacementSetTargetModule )
		|| !PyEmbed::GetStringAttribute( instance, "sReplacementSetTargetFunction", &replacementSetTargetFunction ) ) {
		delete [] replacementSetTargetModule;
		replacementSetTargetModule = NULL;
		delete [] replacementSetTargetFunction;
		replacementSetTargetFunction = NULL;
	}
	PyErr_Print();
	AddEventHandlers();
}

/*
================
OptimizedSelectTarget::GetPreprocessingInstance

The optimized version has no script instance: it hands out a new shadow
instance of itself.
================
*/
// FUNCTION: STBC 0x00488d30
PyObject *OptimizedSelectTarget::GetPreprocessingInstance( void ) {
	return VirtualSlot15();
}

/*
================
OptimizedSelectTarget::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00488d40
PyObject *OptimizedSelectTarget::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "OptimizedSelectTarget" );
}

/*
================
OptimizedSelectTarget::GetAIType
================
*/
// FUNCTION: STBC 0x00488d50
int OptimizedSelectTarget::GetAIType( void ) {
	return AI_TYPE_OPTIMIZED_SELECT_TARGET;
}

/*
================
OptimizedSelectTarget::VirtualSlot20
================
*/
// FUNCTION: STBC 0x00488d60
bool OptimizedSelectTarget::VirtualSlot20( int type ) {
	if ( type == AI_TYPE_OPTIMIZED_SELECT_TARGET ) {
		return true;
	}
	return PreprocessingAI::VirtualSlot20( type );
}

/*
================
OptimizedSelectTarget::~OptimizedSelectTarget
================
*/
// FUNCTION: STBC 0x00488dc0
// FUNCTION: STBC 0x00488da0 ??_GOptimizedSelectTarget@@UAEPAXI@Z
OptimizedSelectTarget::~OptimizedSelectTarget( void ) {
	delete eventHandler;
	Py_DECREF( targetGroupInstance );
	if ( currentTarget != NULL ) {
		delete [] currentTarget;
	}
	while ( setTargetTrees.GetSize() != 0 ) {
		delete setTargetTrees.RemoveEnd();
	}
	if ( replacementSetTargetModule != NULL ) {
		delete [] replacementSetTargetModule;
	}
	if ( replacementSetTargetFunction != NULL ) {
		delete [] replacementSetTargetFunction;
	}
}

/*
================
OptimizedSelectTarget::CreateFromScript
================
*/
// FUNCTION: STBC 0x00488f60
ArtificialIntelligence *OptimizedSelectTarget::CreateFromScript( PreprocessingAI *ai ) {
	ArtificialIntelligence *optimized = new OptimizedSelectTarget( ai );
	delete ai;
	return optimized;
}

/*
================
OptimizedSelectTarget::AddSetTargetTree
================
*/
// FUNCTION: STBC 0x00488fd0
void OptimizedSelectTarget::AddSetTargetTree( ArtificialIntelligence *ai ) {
	if ( ai != NULL ) {
		setTargetTrees.Add( new long( ai->GetID() ) );
	}
}

/*
================
OptimizedSelectTarget::AddEventHandlers

The group's changes, the decloaks, the ship's set changes and the hits on
the ship; then the target's handlers, without calling the SetTarget
functions.
================
*/
// FUNCTION: STBC 0x004890e0
void OptimizedSelectTarget::AddEventHandlers( void ) {
	targetGroup->SetEventFlag( 1 );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_GROUP_CHANGED, eventHandler, "OSTEventHandler::TargetListChanged", targetGroup );
	targetGroup->SetEventFlag( 2 );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_GROUP_OBJECT_ENTERED_SET, eventHandler, "OSTEventHandler::TargetEnteredSet", targetGroup );
	g_kEventManager.AddBroadcastMethodHandler( ET_DECLOAK_BEGINNING, eventHandler, "OSTEventHandler::ObjectDecloaked", ANY_TARGET );
	g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, eventHandler, "OSTEventHandler::OurShipEnteredSet", GetShip() );
	g_kEventManager.AddBroadcastMethodHandler( ET_WEAPON_HIT, eventHandler, "OSTEventHandler::DamageEvent", GetShip() );
	if ( updateInfoOnCodeAISet ) {
		bool callFunctions = callSetTargetFunctions;
		callSetTargetFunctions = false;
		UpdateTargetInfo( g_kSystemWrapper.GetTimeSinceFrameStart() + 1.0 );
		callSetTargetFunctions = callFunctions;
		updateInfoOnCodeAISet = false;
	}
}

/*
================
OptimizedSelectTarget::CallPreprocessingMethod

Chooses the target when the time has come; the contained AI runs unless
the target info is being updated.
================
*/
// FUNCTION: STBC 0x00489200
int OptimizedSelectTarget::CallPreprocessingMethod( double time ) {
	if ( !forceUpdate && g_kUtopiaModule.GetGameTime() < nextUpdateTime ) {
		return preprocessStatus;
	}
	forceUpdate = false;
	preprocessStatus = PS_NORMAL;
	if ( updatingTargetInfo ) {
		UpdateTargetInfo( time );
		nextUpdateTime = g_kUtopiaModule.GetGameTime() + UPDATING_TARGET_INFO_TIME;
		preprocessStatus = PS_SKIP_ACTIVE;
		return PS_SKIP_ACTIVE;
	}

	updateTime = NORMAL_UPDATE_TIME;
	char *target = FindGoodTarget();
	if ( setShipTarget ) {
		ShipClass *ship = GetShip();
		if ( ship != NULL ) {
			ObjectClass *shipTarget = ship->GetTarget();
			if ( target == NULL ) {
				ship->SetTarget( NULL, NULL );
			} else if ( shipTarget == NULL || strcmp( shipTarget->GetName(), target ) != 0 ) {
				ship->SetTarget( target );
			}
		}
	}

	if ( ( target == NULL && currentTarget != NULL ) || ( target != NULL && ( currentTarget == NULL || strcmp( target, currentTarget ) != 0 ) ) ) {
		if ( currentTarget != NULL ) {
			delete [] currentTarget;
			currentTarget = NULL;
		}
		if ( target != NULL ) {
			currentTarget = new char[strlen( target ) + 1];
			strcpy( currentTarget, target );
		}
		UpdateTargetInfo( time );
		if ( updatingTargetInfo ) {
			preprocessStatus = PS_SKIP_ACTIVE;
			nextUpdateTime = g_kUtopiaModule.GetGameTime() + updateTime;
			return PS_SKIP_ACTIVE;
		}
	}

	nextUpdateTime = g_kUtopiaModule.GetGameTime() + updateTime;
	if ( currentTarget == NULL ) {
		ShipClass *ship = GetShip();
		if ( ship != NULL && ship->GetTarget() != NULL ) {
			preprocessStatus = noTargetPreprocessStatus;
		} else {
			preprocessStatus = noTargetNoShipTargetPreprocessStatus;
		}
		nextUpdateTime = g_kUtopiaModule.GetGameTime() + NO_TARGET_UPDATE_TIME;
	}
	return preprocessStatus;
}

/*
================
OptimizedSelectTarget::UpdateTargetInfo

Moves the target handlers to the current target, then calls the SetTarget
functions if the settings ask for it.
================
*/
// FUNCTION: STBC 0x00489470
void OptimizedSelectTarget::UpdateTargetInfo( double endTime ) {
	if ( !updatingTargetInfo ) {
		RemoveOldTargetHandlers();
		if ( currentTarget != NULL ) {
			ShipClass *ship = GetShip();
			if ( ship != NULL ) {
				ObjectClass *target = ObjectClass::GetObject( ship->GetContainingSet(), currentTarget );
				if ( target != NULL ) {
					currentTargetID = target->GetObjID();
					g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, eventHandler, "OSTEventHandler::TargetGone", target );
					g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_DESTROYED, eventHandler, "OSTEventHandler::TargetGone", target );
					g_kEventManager.AddBroadcastMethodHandler( ET_CLOAK_COMPLETED, eventHandler, "OSTEventHandler::TargetGone", target );
				}
			}
		}
	}
	if ( callSetTargetFunctions && ( currentTarget != NULL || callSetTargetFuncsWithNoTarget ) ) {
		CallSetTargetFunctions( endTime );
	}
}

/*
================
OptimizedSelectTarget::RemoveOldTargetHandlers
================
*/
// FUNCTION: STBC 0x00489570
void OptimizedSelectTarget::RemoveOldTargetHandlers( void ) {
	ShipClass *ship = GetShip();
	if ( ship != NULL ) {
		ObjectClass *target = ObjectClass::GetObjectByID( ship->GetContainingSet(), currentTargetID );
		if ( target != NULL ) {
			// BUG: the handlers were added as "OSTEventHandler::TargetGone",
			// so these removals find nothing and the old target's handlers stay
			g_kEventManager.RemoveBroadcastHandler( ET_EXITED_SET, eventHandler, "OptimizedSelectTarget::TargetGone", target );
			g_kEventManager.RemoveBroadcastHandler( ET_OBJECT_DESTROYED, eventHandler, "OptimizedSelectTarget::TargetGone", target );
			g_kEventManager.RemoveBroadcastHandler( ET_CLOAK_COMPLETED, eventHandler, "OptimizedSelectTarget::TargetGone", target );
		}
	}
	currentTargetID = 0;
}

/*
================
OptimizedSelectTarget::CallSetTargetFunctions

Calls the replacement function if the script names one. Otherwise, at
the start of a round, lists the AIs under this one and under the added
trees, then calls "SetTarget" on them with the current target until the
time runs out (but at least ten of them); the AI counts as dormant until
the list is done.
================
*/
// FUNCTION: STBC 0x00489610
void OptimizedSelectTarget::CallSetTargetFunctions( double endTime ) {
	if ( replacementSetTargetModule != NULL && replacementSetTargetFunction != NULL ) {
		PyEmbed::CallFunction( replacementSetTargetModule, replacementSetTargetFunction, "", NULL, "(s)", currentTarget );
		PyErr_Print();
		return;
	}

	if ( !updatingTargetInfo ) {
		NiTList<ArtificialIntelligence *> ais( 2 );
		GetAllAIsInTree( ais );
		ais.RemoveHead();
		for ( unsigned int i = 0; i < setTargetTrees.GetSize(); i++ ) {
			long *id = setTargetTrees.GetAt( i );
			if ( id != NULL ) {
				ArtificialIntelligence *tree = GetAIByID( *id );
				if ( tree != NULL ) {
					tree->GetAllAIsInTree( ais );
				}
			}
		}
		updatingTargetInfo = true;
		forcedDormantStatus = FDS_FALSE;
		updateTime = UPDATING_TARGET_INFO_TIME;
		pendingSetTargetAIs.RemoveAll();
		while ( ais.GetSize() != 0 ) {
			pendingSetTargetAIs.AddTail( new long( ais.RemoveHead()->GetID() ) );
		}
	}

	int called = 0;
	PyObject *target = PyString_FromString( currentTarget );
	if ( pendingSetTargetAIs.GetSize() != 0 ) {
		do {
			called++;
			long *id = pendingSetTargetAIs.RemoveHead();
			ArtificialIntelligence *ai = GetAIByID( *id );
			delete id;
			if ( ai != NULL ) {
				ai->CallExternalFunction( "SetTarget", target );
			}
		} while ( ( g_kSystemWrapper.GetTimeSinceFrameStart() <= endTime || called < 10 ) && pendingSetTargetAIs.GetSize() != 0 );
	}
	Py_DECREF( target );
	if ( pendingSetTargetAIs.GetSize() == 0 ) {
		updatingTargetInfo = false;
		forcedDormantStatus = FDS_NORMAL;
	}
}

/*
================
OptimizedSelectTarget::FindGoodTarget

The group's objects in the ship's set that are alive, not cloaked, not the
ship and seen by its sensors (unless they are ignored), and of those the
one GetTargetRating rates best; a single candidate is taken unrated.
================
*/
// FUNCTION: STBC 0x00489910
char *OptimizedSelectTarget::FindGoodTarget( void ) {
	ShipClass *ship = GetShip();
	if ( ship->GetContainingSet() == NULL ) {
		return NULL;
	}

	PyObject *objects = targetGroup->GetActiveObjectTupleInSet( ship->GetContainingSet() );
	NiTList<ObjectClass *> candidates( 2 );
	for ( int i = 0; i < PyTuple_Size( objects ); i++ ) {
		ObjectClass *object = (ObjectClass *)PyEmbed::GetObjectPointer( PyTuple_GetItem( objects, i ) );
		DamageableObject *damageable = DamageableObject::Cast( object );
		if ( damageable != NULL && ( damageable->IsDead() || damageable->GetLifeTime() < INFINITE_LIFE ) ) {
			continue;
		}
		ShipClass *targetShip = ShipClass::Cast( object );
		if ( targetShip != NULL && targetShip->GetCloakingSubsystem() != NULL && targetShip->GetCloakingSubsystem()->IsCloaked() ) {
			continue;
		}
		if ( object->GetObjID() == ship->GetObjID() ) {
			continue;
		}
		if ( !ignoreSensors && !ship->GetSensorSubsystem()->IsObjectVisible( object ) ) {
			continue;
		}
		candidates.AddTail( object );
	}
	Py_DECREF( objects );

	if ( candidates.IsEmpty() ) {
		return NULL;
	}
	if ( candidates.GetSize() == 1 ) {
		return candidates.GetHead()->GetName();
	}

	float bestRating = -1e20f;
	ObjectClass *best = candidates.GetHead();
	while ( !candidates.IsEmpty() ) {
		ObjectClass *object = candidates.RemoveHead();
		float rating = GetTargetRating( ship, object );
		if ( rating > bestRating ) {
			bestRating = rating;
			best = object;
		}
	}
	return best->GetName();
}

/*
================
OptimizedSelectTarget::GetTargetRating

The script's rating of a target, a weighted sum of: nearness
(RATING_DISTANCE over the distance, at most 1), how far in front of the ship it is,
whether it is the ship's target, its shields', weapons' and hull's
condition, the damage it has done the ship, its priority in the group and
how many other ships target it.
================
*/
// FUNCTION: STBC 0x00489bd0
float OptimizedSelectTarget::GetTargetRating( ShipClass *ship, ObjectClass *target ) {
	NiPoint3 offset = target->GetWorldLocation() - ship->GetWorldLocation();
	float distanceRating = 0.0f;
	float inFrontRating = 0.0f;
	float distance = NiPoint3::VectorLength( offset );
	if ( distance > 0.0f ) {
		distanceRating = RATING_DISTANCE / distance;
		if ( distanceRating > 1.0f ) {
			distanceRating = 1.0f;
		}
		inFrontRating = ship->GetWorldForward().Dot( offset ) / distance;
	}

	float isTargetRating = 0.0f;
	if ( setShipTarget ) {
		ObjectClass *shipTarget = ship->GetTarget();
		if ( shipTarget != NULL && shipTarget->GetObjID() == target->GetObjID() ) {
			isTargetRating = 1.0f;
		}
	} else if ( currentTarget != NULL && strcmp( target->GetName(), currentTarget ) == 0 ) {
		isTargetRating = 1.0f;
	}

	float shieldRating = 0.0f;
	float weaponsRating = 0.0f;
	float hullRating = 0.0f;
	ShipClass *targetShip = ShipClass::Cast( target );
	if ( targetShip != NULL ) {
		if ( targetShip->GetShields() != NULL ) {
			shieldRating = targetShip->GetShields()->GetShieldPercentage();
		}
		int weapons = 0;
		NiTListIterator pos = targetShip->GetSubsystemList().GetHeadPos();
		while ( pos != NULL ) {
			ShipSubsystem *subsystem = targetShip->GetSubsystemList().GetNext( pos );
			if ( subsystem != NULL && subsystem->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
				weapons++;
				weaponsRating += subsystem->GetCombinedConditionPercentage();
			}
		}
		if ( weapons != 0 ) {
			weaponsRating /= weapons;
		}
		hullRating = targetShip->GetHull()->GetCombinedConditionPercentage();
	}

	float damageRating = 0.0f;
	damageTaken.GetAt( target->GetObjID(), damageRating );

	float priorityRating = 0.0f;
	ObjectGroupWithInfo *group = ObjectGroupWithInfo::Cast( targetGroup );
	if ( group != NULL ) {
		PyObject *info = group->GetInfo( target->GetName() );
		if ( info != NULL ) {
			PyObject *priority = PyDict_GetItemString( info, "Priority" );
			PyErr_Clear();
			if ( priority != NULL ) {
				priorityRating = (float)PyFloat_AsDouble( priority );
			}
		}
	}

	float popularityRating = 0.0f;
	if ( ship->GetContainingSet() != NULL ) {
		int count = 0;
		ShipClass **ships = ShipClass::GetClassObjects( ship->GetContainingSet(), &count );
		while ( --count >= 0 ) {
			if ( ships[count] != ship && ships[count]->GetTarget() == target ) {
				popularityRating += 1.0f;
			}
		}
	}

	return distanceRating * distanceFactor + inFrontRating * inFrontFactor + isTargetRating * isTargetFactor
		   + shieldRating * shieldFactor + weaponsRating * weaponsFactor + hullRating * hullFactor
		   + damageRating * damageFactor + priorityRating * priorityFactor + popularityRating * popularityFactor;
}

/*
================
OptimizedSelectTarget::ObjectDecloaked

An object of the group decloaking calls for a new choice.
================
*/
// FUNCTION: STBC 0x00489f70
void OptimizedSelectTarget::ObjectDecloaked( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	if ( object != NULL && targetGroup->IsNameInGroup( object->GetName() ) ) {
		forceUpdate = true;
	}
}

/*
================
OptimizedSelectTarget::DamageEvent

Adds the hit's damage, as a fraction of the ship's hull, to what the
attacker has done.
================
*/
// FUNCTION: STBC 0x00489fa0
void OptimizedSelectTarget::DamageEvent( WeaponHitEvent *event ) {
	ObjectClass *attacker = event->GetFiringObject();
	if ( attacker == NULL ) {
		return;
	}
	long attackerID = attacker->GetObjID();
	ShipClass *ship = (ShipClass *)event->GetDestination();
	float damage = 0.0f;
	damageTaken.GetAt( attackerID, damage );
	HullClass *hull = ship->GetHull();
	float total = event->GetDamage() / hull->GetMaxCondition() + damage;
	damageTaken.SetAt( attacker->GetObjID(), total );
}

/*
================
OptimizedSelectTarget::WriteToStream
================
*/
// FUNCTION: STBC 0x0048a0c0
bool OptimizedSelectTarget::WriteToStream( TGStream *stream ) {
	if ( !PreprocessingAI::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( targetGroup->GetObjID() );
	if ( !PyEmbed::SerializePython( targetGroupInstance, stream, 0 ) ) {
		return false;
	}
	eventHandler->WriteToStream( stream );
	stream->VirtualSlot11( currentTarget );
	stream->WriteID( currentTargetID );

	stream->WriteInt( setTargetTrees.GetEffectiveSize() );
	for ( unsigned int i = 0; i < setTargetTrees.GetSize(); i++ ) {
		long *id = setTargetTrees.GetAt( i );
		if ( id != NULL ) {
			stream->WriteLong( *id );
		}
	}
	stream->WriteInt( pendingSetTargetAIs.GetSize() );
	NiTListIterator pos = pendingSetTargetAIs.GetHeadPos();
	while ( pos != NULL ) {
		stream->WriteLong( *pendingSetTargetAIs.GetNext( pos ) );
	}

	stream->WriteFloat( updateTime );
	stream->WriteBool( setShipTarget );
	stream->WriteBool( callSetTargetFuncsWithNoTarget );
	stream->WriteBool( callSetTargetFunctions );
	stream->WriteBool( updateInfoOnCodeAISet );
	stream->WriteBool( updatingTargetInfo );
	stream->WriteBool( ignoreSensors );
	stream->WriteInt( noTargetPreprocessStatus );
	stream->WriteInt( noTargetNoShipTargetPreprocessStatus );
	stream->WriteFloat( distanceFactor );
	stream->WriteFloat( inFrontFactor );
	stream->WriteFloat( isTargetFactor );
	stream->WriteFloat( shieldFactor );
	stream->WriteFloat( weaponsFactor );
	stream->WriteFloat( hullFactor );
	stream->WriteFloat( damageFactor );
	stream->WriteFloat( priorityFactor );
	stream->WriteFloat( popularityFactor );
	stream->VirtualSlot11( replacementSetTargetModule );
	stream->VirtualSlot11( replacementSetTargetFunction );

	stream->WriteInt( damageTaken.GetCount() );
	NiTMapIterator damagePos = damageTaken.GetFirstPos();
	while ( damagePos != NULL ) {
		long attackerID;
		float damage;
		damageTaken.GetNext( damagePos, attackerID, damage );
		stream->WriteID( attackerID );
		stream->WriteFloat( damage );
	}
	return true;
}

/*
================
OptimizedSelectTarget::OptimizedSelectTarget

The group and the event handler are fixed up after the load.
================
*/
// FUNCTION: STBC 0x0048a340 ??0OptimizedSelectTarget@@QAE@PAVTGStream@@PA_N@Z
// PARKED: addressing: in the inlined NiTListBlock constructor of the second loop retail writes [ecx+eax+4], this build [eax+ecx+4]
OptimizedSelectTarget::OptimizedSelectTarget( TGStream *stream, bool *ok ) : PreprocessingAI( stream, ok ), pendingSetTargetAIs( 2 ), damageTaken( 37 ) {
	if ( !*ok ) {
		return;
	}
	targetGroup = (ObjectGroup *)stream->ReadID();
	targetGroupInstance = PyEmbed::UnserializePython( stream, 0 );
	if ( targetGroupInstance == NULL ) {
		*ok = false;
		return;
	}
	eventHandler = (OSTEventHandler *)TGObject::LoadObject( stream, 0 );
	if ( eventHandler == NULL ) {
		*ok = false;
		return;
	}
	currentTarget = stream->VirtualSlot10( -1 );
	currentTargetID = stream->ReadID();

	int i;
	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		long id = stream->ReadLong();
		setTargetTrees.Add( new long( id ) );
	}
	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		long id = stream->ReadLong();
		pendingSetTargetAIs.AddTail( new long( id ) );
	}

	updateTime = stream->ReadFloat();
	setShipTarget = stream->ReadBool();
	callSetTargetFuncsWithNoTarget = stream->ReadBool();
	callSetTargetFunctions = stream->ReadBool();
	updateInfoOnCodeAISet = stream->ReadBool();
	updatingTargetInfo = stream->ReadBool();
	ignoreSensors = stream->ReadBool();
	noTargetPreprocessStatus = stream->ReadInt();
	noTargetNoShipTargetPreprocessStatus = stream->ReadInt();
	distanceFactor = stream->ReadFloat();
	inFrontFactor = stream->ReadFloat();
	isTargetFactor = stream->ReadFloat();
	shieldFactor = stream->ReadFloat();
	weaponsFactor = stream->ReadFloat();
	hullFactor = stream->ReadFloat();
	damageFactor = stream->ReadFloat();
	priorityFactor = stream->ReadFloat();
	popularityFactor = stream->ReadFloat();
	replacementSetTargetModule = stream->VirtualSlot10( -1 );
	replacementSetTargetFunction = stream->VirtualSlot10( -1 );

	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		long attackerID = stream->ReadID();
		float damage = stream->ReadFloat();
		damageTaken.SetAt( attackerID, damage );
	}
}

/*
================
OptimizedSelectTarget::FixUpPointers
================
*/
// FUNCTION: STBC 0x0048a860
bool OptimizedSelectTarget::FixUpPointers( void ) {
	if ( !PreprocessingAI::FixUpPointers() ) {
		return false;
	}
	return TGObject::GetTGObjectPtr( (long)targetGroup ) != NULL ? true : false;
}

/*
================
OptimizedSelectTarget::PostFixUp
================
*/
// FUNCTION: STBC 0x0048a890
void OptimizedSelectTarget::PostFixUp( void ) {
	PreprocessingAI::PostFixUp();
	targetGroup = (ObjectGroup *)TGObject::GetTGObjectPtr( (long)targetGroup );
}

/*
================
OptimizedSelectTarget::Create
================
*/
// FUNCTION: STBC 0x0048a8b0
ArtificialIntelligence *OptimizedSelectTarget::Create( TGStream *stream ) {
	bool ok = true;
	OptimizedSelectTarget *ai = new OptimizedSelectTarget( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}
