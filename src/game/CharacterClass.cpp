/*
===============================================================================

	CharacterClass.cpp

===============================================================================
*/

#include <iostream>

#include "CharacterClass.h"
#include "Game.h"
#include "ShipClass.h"
#include "NiBone.h"
#include "CameraObjectClass.h"
#include "CameraMode.h"
#include "CharacterAction.h"
#include "CharacterSpeakingQueue.h"
#include "SetClass.h"
#include "SetManager.h"
#include "PyEmbed.h"
#include "STTopLevelMenu.h"
#include "TGAction.h"
#include "TGAnimAction.h"
#include "TGBoolEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGLocalizationManager.h"
#include "TGActionManager.h"
#include "TGModelManager.h"
#include "TGParagraph.h"
#include "TGPhonemeAction.h"
#include "TGPhoneme.h"
#include "ModelHelpers.h"
#include "TGAnimBlender.h"
#include "NiKeyframeController.h"
#include "TGAnimationManagerClass.h"
#include "TGSequence.h"
#include "TopWindow.h"
#include "MainWindow.h"
#include "TGStringToStringMap.h"
#include "TGStream.h"
#include "TGLocalizationDatabase.h"
#include "TGAnimNode.h"
#include "TGSystemWrapperClass.h"
#include "TGPointerMap.h"
#include "RotateBonesController.h"
#include "ZoomCameraObjectClass.h"
#include "UtopiaModule.h"
#include "NiSpecularProperty.h"
#include "NiTextureModeProperty.h"
#include "NiMaterialProperty.h"
#include "NiVertexColorProperty.h"
#include "NiLight.h"
#include "NiSkinController.h"
#include "NiStream.h"
#include "NiTriShape.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00666b00 _$E66
// FUNCTION: STBC 0x00666b10 _$E65
// FUNCTION: STBC 0x00666b20 _$E334
// FUNCTION: STBC 0x00666b30 _$E333
// FUNCTION: STBC 0x00666b40 _$E77
// FUNCTION: STBC 0x00666b50 _$E76
// FUNCTION: STBC 0x00666b60 _$E81
// FUNCTION: STBC 0x00666b70 _$E80
// FUNCTION: STBC 0x00666b80 _$E85
// FUNCTION: STBC 0x00666b90 _$E84
// FUNCTION: STBC 0x00666ba0 _$E89
// FUNCTION: STBC 0x00666bb0 _$E88
// FUNCTION: STBC 0x00666bc0 _$E206
// FUNCTION: STBC 0x00666bd0 _$E205
// FUNCTION: STBC 0x00666be0 _$E401
// FUNCTION: STBC 0x00666bf0 _$E400
// FUNCTION: STBC 0x00666c00 _$E383
// FUNCTION: STBC 0x00666c10 _$E382
// FUNCTION: STBC 0x00666c20 _$E148
// FUNCTION: STBC 0x00666c30 _$E147
// FUNCTION: STBC 0x00666c40 _$E392
// FUNCTION: STBC 0x00666c50 _$E391
// FUNCTION: STBC 0x00666c60 _$E161
// FUNCTION: STBC 0x00666c70 _$E160
// FUNCTION: STBC 0x00666c80 _$E405
// FUNCTION: STBC 0x00666c90 _$E404
// FUNCTION: STBC 0x00666ca0 _$E410
// FUNCTION: STBC 0x00666cb0 _$E409
// FUNCTION: STBC 0x00666cc0 _$E211
// FUNCTION: STBC 0x00666cd0 _$E210
// FUNCTION: STBC 0x006679b0 _$E339
// FUNCTION: STBC 0x006679c0 _$E338
// FUNCTION: STBC 0x00667b80 _$E215
// FUNCTION: STBC 0x00667b90 _$E214
// FUNCTION: STBC 0x006689e0 _$E343
// FUNCTION: STBC 0x006689f0 _$E342
// FUNCTION: STBC 0x00668ed0 _$E350
// FUNCTION: STBC 0x00668ee0 _$E349
// FUNCTION: STBC 0x00669460 _$E444
// FUNCTION: STBC 0x00669470 _$E443
// FUNCTION: STBC 0x00669630 _$E448
// FUNCTION: STBC 0x00669640 _$E447
// FUNCTION: STBC 0x00669650 _$E106
// FUNCTION: STBC 0x00669660 _$E105
// FUNCTION: STBC 0x00669ca0 _$E251
// FUNCTION: STBC 0x00669cb0 _$E250
// FUNCTION: STBC 0x0066a1e0 _$E258
// FUNCTION: STBC 0x0066a1f0 _$E257
// FUNCTION: STBC 0x0066a680 _$E262
// FUNCTION: STBC 0x0066a690 _$E261
// FUNCTION: STBC 0x0066a7d0 _$E266
// FUNCTION: STBC 0x0066a7e0 _$E265
// FUNCTION: STBC 0x0066aa90 _$E270
// FUNCTION: STBC 0x0066aaa0 _$E269
// FUNCTION: STBC 0x0066aae0 _$E276
// FUNCTION: STBC 0x0066aaf0 _$E275
// FUNCTION: STBC 0x0066b170 _$E430
// FUNCTION: STBC 0x0066b180 _$E429
// FUNCTION: STBC 0x0066b390 _$E452
// FUNCTION: STBC 0x0066b3a0 _$E451
// FUNCTION: STBC 0x0066b3b0 _$E456
// FUNCTION: STBC 0x0066b3c0 _$E455
// FUNCTION: STBC 0x0066b5f0 _$E460
// FUNCTION: STBC 0x0066b600 _$E459
// FUNCTION: STBC 0x0066b740 _$E464
// FUNCTION: STBC 0x0066b750 _$E463
// FUNCTION: STBC 0x0066b820 _$E468
// FUNCTION: STBC 0x0066b830 _$E467
// FUNCTION: STBC 0x0066b970 _$E472
// FUNCTION: STBC 0x0066b980 _$E471
// FUNCTION: STBC 0x0066bb80 _$E285
// FUNCTION: STBC 0x0066bb90 _$E284
// FUNCTION: STBC 0x0066bcf0 _$E476
// FUNCTION: STBC 0x0066bd00 _$E475
// FUNCTION: STBC 0x0066c0a0 _$E480
// FUNCTION: STBC 0x0066c0b0 _$E479
// FUNCTION: STBC 0x0066c1c0 _$E111
// FUNCTION: STBC 0x0066c1d0 _$E110
// FUNCTION: STBC 0x0066c4f0 _$E484
// FUNCTION: STBC 0x0066c500 _$E483
// FUNCTION: STBC 0x0066c510 _$E116
// FUNCTION: STBC 0x0066c520 _$E115
// FUNCTION: STBC 0x0066c7a0 _$E488
// FUNCTION: STBC 0x0066c7b0 _$E487
// FUNCTION: STBC 0x0066c9a0 _$E492
// FUNCTION: STBC 0x0066c9b0 _$E491
// FUNCTION: STBC 0x0066c9c0 _$E496
// FUNCTION: STBC 0x0066c9d0 _$E495
// FUNCTION: STBC 0x0066cb70 _$E500
// FUNCTION: STBC 0x0066cb80 _$E499
// FUNCTION: STBC 0x0066cdd0 _$E125
// FUNCTION: STBC 0x0066cde0 _$E124
// FUNCTION: STBC 0x0066d170 _$E504
// FUNCTION: STBC 0x0066d180 _$E503
// FUNCTION: STBC 0x0066d1c0 _$E508
// FUNCTION: STBC 0x0066d1d0 _$E507
// FUNCTION: STBC 0x0066d330 _$E512
// FUNCTION: STBC 0x0066d340 _$E511
// FUNCTION: STBC 0x0066d430 _$E131
// FUNCTION: STBC 0x0066d440 _$E130
// FUNCTION: STBC 0x0066daa0 _$E136
// FUNCTION: STBC 0x0066dab0 _$E135
// FUNCTION: STBC 0x0066e200 _$E166
// FUNCTION: STBC 0x0066e210 _$E165
// FUNCTION: STBC 0x0066eed0 _$E516
// FUNCTION: STBC 0x0066eee0 _$E515
// FUNCTION: STBC 0x0066eef0 _$E171
// FUNCTION: STBC 0x0066ef00 _$E170
// FUNCTION: STBC 0x0066ef10 _$E434
// FUNCTION: STBC 0x0066ef20 _$E433
// FUNCTION: STBC 0x0066ef30 _$E520
// FUNCTION: STBC 0x0066ef40 _$E519
// FUNCTION: STBC 0x0066ef90 _$E176
// FUNCTION: STBC 0x0066efa0 _$E175
// FUNCTION: STBC 0x0066efc0 _$E524
// FUNCTION: STBC 0x0066efd0 _$E523
// FUNCTION: STBC 0x0066efe0 _$E141
// FUNCTION: STBC 0x0066eff0 _$E140
// FUNCTION: STBC 0x0066f000 _$E528
// FUNCTION: STBC 0x0066f010 _$E527
// FUNCTION: STBC 0x0066f020 _$E532
// FUNCTION: STBC 0x0066f030 _$E531
// FUNCTION: STBC 0x0066f040 _$E181
// FUNCTION: STBC 0x0066f050 _$E180
// FUNCTION: STBC 0x0066f060 _$E186
// FUNCTION: STBC 0x0066f070 _$E185
// FUNCTION: STBC 0x0066f080 _$E536
// FUNCTION: STBC 0x0066f090 _$E535
// FUNCTION: STBC 0x0066f0a0 _$E540
// FUNCTION: STBC 0x0066f0b0 _$E539
// FUNCTION: STBC 0x0066f0c0 _$E544
// FUNCTION: STBC 0x0066f0d0 _$E543
// FUNCTION: STBC 0x0066f0e0 _$E548
// FUNCTION: STBC 0x0066f0f0 _$E547
// FUNCTION: STBC 0x0066f100 _$E552
// FUNCTION: STBC 0x0066f110 _$E551
// FUNCTION: STBC 0x0066f120 _$E556
// FUNCTION: STBC 0x0066f130 _$E555
// FUNCTION: STBC 0x0066f140 _$E560
// FUNCTION: STBC 0x0066f150 _$E559
// FUNCTION: STBC 0x0066f160 _$E564
// FUNCTION: STBC 0x0066f170 _$E563
// FUNCTION: STBC 0x0066f180 _$E191
// FUNCTION: STBC 0x0066f190 _$E190
// FUNCTION: STBC 0x0066f1a0 _$E568
// FUNCTION: STBC 0x0066f1b0 _$E567
// FUNCTION: STBC 0x0066f1c0 _$E196
// FUNCTION: STBC 0x0066f1d0 _$E195
// FUNCTION: STBC 0x0066f1e0 _$E201
// FUNCTION: STBC 0x0066f1f0 _$E200
// FUNCTION: STBC 0x0066f4e0 _$E572
// FUNCTION: STBC 0x0066f4f0 _$E571
// FUNCTION: STBC 0x0066f500 _$E577
// FUNCTION: STBC 0x0066f510 _$E576
// FUNCTION: STBC 0x0066f520 _$E581
// FUNCTION: STBC 0x0066f530 _$E580

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00666a80 _$E32
// FUNCTION: STBC 0x00666a90 _$E29
// FUNCTION: STBC 0x00666aa0 _$E31
// FUNCTION: STBC 0x00666ab0 _$E30
// FUNCTION: STBC 0x00666ac0 _$E38
// FUNCTION: STBC 0x00666ad0 _$E35
// FUNCTION: STBC 0x00666ae0 _$E37
// FUNCTION: STBC 0x00666af0 _$E36
// FUNCTION: STBC 0x0066f790 _$E596
// FUNCTION: STBC 0x0066f7b0 _$E595

// The class handler table's construction and its destruction at exit.
// FUNCTION: STBC 0x00666ce0 _$E587
// FUNCTION: STBC 0x00666cf0 _$E583
// FUNCTION: STBC 0x00666d00 _$E586
// DATA: STBC 0x00991ba0
TGEventHandlerTable CharacterClass::classHandlers;

// DATA: STBC 0x00991cbc
CharacterClass *CharacterClass::walkingExtra;

// DATA: STBC 0x00991cc0
CharacterClass *CharacterClass::currentToolTipOwner;

// DATA: STBC 0x00957160
bool CharacterClass::extraWalkingEnabled = true;

// DATA: STBC 0x00957164
float CharacterClass::extraWalkChance = 0.05f;

// DATA: STBC 0x00957168
bool CharacterClass::toolTipsEnabled = true;

// DATA: STBC 0x00957169
bool CharacterClass::collisionAlertEnabled = true;

// DATA: STBC 0x0095716c
float CharacterClass::lineVolumes[3] = { 1.0f, 1.0f, 1.0f };

/*
================
CharacterClass::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00666d30
void CharacterClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, CHARACTERCLASS_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
CharacterClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00666d60
void CharacterClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, CHARACTERCLASS_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
CharacterClass::GetClassObjects

The set's characters and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x00666d90
CharacterClass **CharacterClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (CharacterClass **)set->objectTable.GetObjects( CHARACTERCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
CharacterClass::GetNumClassObjects

The number of the set's characters.
================
*/
// FUNCTION: STBC 0x00666dc0
int CharacterClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( CHARACTERCLASS_TYPE );
	}
	return 0;
}

/*
================
CharacterClass::GetObjectByID

The character with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a CharacterClass.
================
*/
// FUNCTION: STBC 0x00666de0
CharacterClass *CharacterClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( CHARACTERCLASS_TYPE ) ) {
			return (CharacterClass *)object;
		}
		return NULL;
	}
	return (CharacterClass *)set->GetClassObject( CHARACTERCLASS_TYPE, id );
}

/*
================
CharacterClass::GetObject

The character with the given name in the set, or in the first set that
has one, searching from the last set, when the set is NULL.
================
*/
// FUNCTION: STBC 0x00666e50
CharacterClass *CharacterClass::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( CHARACTERCLASS_TYPE, name_ );
		}
		return (CharacterClass *)object;
	}
	return (CharacterClass *)set->GetClassObject( CHARACTERCLASS_TYPE, name_ );
}

/*
================
CharacterClass::Cast

The object as a CharacterClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00666eb0
CharacterClass *CharacterClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( CHARACTERCLASS_TYPE ) ) {
		return (CharacterClass *)object;
	}
	return NULL;
}

/*
================
CharacterClass::AddAnimation

Files the animation file under a name the scripts play it by.
================
*/
// FUNCTION: STBC 0x00666ee0
void CharacterClass::AddAnimation( char *name_, char *file ) {
	animations->AddMapping( name_, file );
}

/*
================
CharacterClass::IsSomeoneSpeaking

Whether any character is speaking a line.
================
*/
// FUNCTION: STBC 0x00666f00
bool CharacterClass::IsSomeoneSpeaking( void ) {
	return CharacterSpeakingQueue::numSpeaking > 0;
}

/*
================
CharacterClass::CharacterClass

A character of no size, female, vocal, with its speaking queue unless it
is being loaded (the stream supplies the queue); it takes the initiative.
================
*/
// FUNCTION: STBC 0x00666f10 ??0CharacterClass@@QAE@J@Z
// PARKED: inline budget: retail calls the speaking queue's second list constructor out of line (0x66F4B0), this build inlines it
CharacterClass::CharacterClass( long id ) : ObjectClass( id ), queuedAnimations( 2 ) {
	unknown68 = NULL;
	unknown6C = NULL;
	headTexture = NULL;
	yesSir = NULL;
	animations = new TGStringToStringMap;
	size = SMALL;
	gender = FEMALE;
	flags = CS_IDLE;
	audioMode = CAM_VOCAL;
	characterName = NULL;
	currentAnimation = NULL;
	if ( id == 0 ) {
		speakingQueue = new CharacterSpeakingQueue;
	}
	watching = false;
	unknown98 = 0.5f;
	location = NULL;
	turnTowardsLocation = NULL;
	glanceAtLocation = NULL;
	positionZooms = NULL;
	numPositionZooms = 0;
	lookAtAdj = NiPoint3::ZERO;
	rotateBonesController = NULL;
	blinkChance = 0.1f;
	blinkAnimation = NULL;
	blinkDelay = 0.0f;
	unknownC0 = 0.0f;
	extra = false;
	// The random number is unsigned in retail (GetRandomNumber divides
	// unsigned); the shared declaration says int.
	unknownC8 = -(float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 30 );
	active = false;
	unknownD0 = -1.0f;
	statusBox = NULL;
	unknown100 = 0;
	unknown11C = 0;
	unknown138 = 0;
	randomAnimationEnabled = true;
	randomAnimationChance = 0.0f;
	unknown144 = 0.0f;
	database = NULL;
	menu = NULL;
	overlayController = NULL;
	overlayManager = NULL;
	blinkStages = -1;
	animatedSpeaking = false;
	lastTalkTime = 0.0f;
	menuEnabled = true;
	SetInitiative( true );
}

/*
================
CharacterClass::GetObjType
================
*/
// FUNCTION: STBC 0x00667340
int CharacterClass::GetObjType( void ) {
	return CHARACTERCLASS_TYPE;
}

/*
================
CharacterClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x00667350
bool CharacterClass::IsTypeOf( int type ) {
	if ( type == CHARACTERCLASS_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

/*
================
CharacterClass::GetClassName
================
*/
// FUNCTION: STBC 0x006673b0
char *CharacterClass::GetClassName( void ) {
	return "CharacterClass";
}

/*
================
CharacterClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006673c0
char *CharacterClass::GetSwigTypeName( void ) {
	return "_p_CharacterClass";
}

/*
================
CharacterClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006673d0
char *CharacterClass::GetPythonClassName( void ) {
	return "CharacterClassPtr";
}

/*
================
CharacterClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x006673e0
TGEventHandlerTable *CharacterClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
CharacterClass::~CharacterClass

Aborts the current animation and skips the actions that play on the
character, then frees what it owns and takes its status lines and box out
of the interface.
================
*/
// FUNCTION: STBC 0x00667420
CharacterClass::~CharacterClass( void ) {
	if ( currentAnimation != NULL ) {
		TGAction *action = (TGAction *)TGObject::GetTGObjectPtr( currentAnimation->actionID );
		if ( action != NULL )
			action->Abort();
		delete currentAnimation;
	}
	currentAnimation = NULL;
	if ( database != NULL )
		g_kLocalizationManager.Unload( database );
	g_kTGActionManager.SkipActionsFor( (TGAnimNode *)(NiObject *)overlayManager );
	g_kTGActionManager.SkipActionsFor( GetAnimNode() );
	ClearAnimations();

	NiTMapIterator pos = facialImages.GetFirstPos();
	while ( pos != NULL ) {
		const char *name_;
		char *image;
		facialImages.GetNext( pos, name_, image );
		delete image;
	}
	facialImages.RemoveAll();

	delete speakingQueue;
	delete characterName;
	if ( unknown6C != NULL )
		g_kModelManager.Unrefer( unknown6C );
	if ( unknown68 != NULL )
		g_kModelManager.Unrefer( unknown68 );
	delete unknown6C;
	delete unknown68;
	delete headTexture;

	for ( unsigned int i = 0; i < statuses.GetCount(); i++ ) {
		TGParagraph *status;
		if ( statuses.GetAt( i, status ) && status != NULL ) {
			if ( status->GetParent() != NULL )
				status->GetParent()->RemoveChild( status );
			delete status;
		}
	}
	statuses.RemoveAll();

	if ( statusBox != NULL ) {
		statusBox->SetNotVisible();
		if ( statusBox->GetParent() == NULL )
			delete statusBox;
		else
			statusBox->GetParent()->DeleteChild( statusBox );
	}

	pos = phonemeLists.GetFirstPos();
	while ( pos != NULL ) {
		int type;
		PhonemeList *list;
		phonemeLists.GetNext( pos, type, list );
		delete list;
	}

	if ( walkingExtra == this )
		walkingExtra = NULL;
	if ( currentToolTipOwner == this )
		currentToolTipOwner = NULL;
	overlayManager = NULL;
	overlayController = NULL;
	if ( animations != NULL )
		delete animations;
}

// The skin controller flag MarkSkinControllers sets; its meaning is not
// known (invented name).
#define SKIN_CONTROLLER_FLAG	0x10

/*
================
FindSkinController

The first skinning controller of the object's controllers; NULL when it
has none (invented name).
================
*/
// FUNCTION: STBC 0x006679d0
NiSkinController *FindSkinController( NiObjectNET *object ) {
	NiTimeController *controller = object->GetControllers();
	while ( controller != NULL ) {
		if ( controller->IsKindOf( &NiSkinController::rtti ) ) {
			return (NiSkinController *)controller;
		}
		controller = controller->GetNext();
	}
	return NULL;
}

/*
================
MarkSkinControllers

Sets SKIN_CONTROLLER_FLAG on the skinning controller of each triangle
shape in the tree (invented name).
================
*/
// FUNCTION: STBC 0x00667a10
void MarkSkinControllers( NiAVObject *object ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiSkinController *controller = FindSkinController( shape );
		if ( controller != NULL ) {
			controller->flags |= SKIN_CONTROLLER_FLAG;
		}
	} else {
		NiNode *node = NiDynamicCast( NiNode, object );
		if ( node != NULL ) {
			for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
				if ( node->GetAt( i ) != NULL ) {
					MarkSkinControllers( node->GetAt( i ) );
				}
			}
		}
	}
}

/*
================
AttachLights

Makes each light in the tree light the node alone (invented name).
================
*/
// FUNCTION: STBC 0x00667ac0
void AttachLights( NiAVObject *object, NiNode *affected ) {
	NiLight *light = NiDynamicCast( NiLight, object );
	if ( light != NULL ) {
		light->DetachAllAffectedNodes();
		light->AttachAffectedNode( affected );
		return;
	}
	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			if ( node->GetAt( i ) != NULL ) {
				AttachLights( node->GetAt( i ), affected );
			}
		}
	}
}

/*
================
DetachLights

Takes each light in the tree off the nodes it lights and out of its
parent (invented name; nothing in the unit calls it).
================
*/
// FUNCTION: STBC 0x00667ba0
void DetachLights( NiAVObject *object ) {
	NiLight *light = NiDynamicCast( NiLight, object );
	if ( light != NULL ) {
		light->DetachAllAffectedNodes();
		if ( light->GetParent() != NULL ) {
			light->GetParent()->DetachChild( light );
		}
		return;
	}
	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			if ( node->GetAt( i ) != NULL ) {
				DetachLights( node->GetAt( i ) );
			}
		}
	}
}

/*
================
CharacterClass::Create

A new character with the body and head models.
================
*/
// FUNCTION: STBC 0x00667d10
CharacterClass *CharacterClass::Create( char *bodyFile, char *headFile, bool unknown ) {
	CharacterClass *character = new CharacterClass;
	character->Unknown667ED0( bodyFile, headFile, unknown );
	return character;
}

/*
================
CharacterClass::Unknown667D60

Makes the body the character's object, fits the head on it, starts a
RotateBonesController on it and sets up its properties and its model.
================
*/
// FUNCTION: STBC 0x00667d60
void CharacterClass::Unknown667D60( TGAnimNode *body, TGAnimNode *head ) {
	SetNiObject( body );
	if ( head != NULL ) {
		Unknown668A00( body, head );
	}
	rotateBonesController = new RotateBonesController;
	rotateBonesController->SetTarget( body );
	rotateBonesController->VirtualSlot20();
	Unknown668000( body, head );
	Unknown668250( body );
}

/*
================
CharacterClass::MorphBody

Scales the named bone of the model, the head without a name; a negative
scale is taken as its size.
================
*/
// FUNCTION: STBC 0x00667e10
void CharacterClass::MorphBody( float scale, char *boneName ) {
	NiNode *bone;
	if ( boneName == NULL ) {
		TGAnimNode *animNode = GetAnimNode();
		bone = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 Head" ) );
	} else {
		TGAnimNode *animNode = GetAnimNode();
		bone = NiDynamicCast( NiNode, animNode->GetObjectByName( boneName ) );
	}
	if ( bone != NULL ) {
		if ( scale > 0.0f ) {
			bone->SetScale( scale );
		} else {
			bone->SetScale( -scale );
		}
	}
}

/*
================
CharacterClass::Unknown667ED0

Builds the character from a copy of the body model and a clone of the
head model, and keeps the two model names.
================
*/
// FUNCTION: STBC 0x00667ed0
void CharacterClass::Unknown667ED0( char *bodyFile, char *headFile, bool unknown ) {
	NiPointer<TGAnimNode> head;
	TGAnimNode *body = g_kModelManager.CopyModel( bodyFile );
	if ( body != NULL ) {
		if ( headFile != NULL ) {
			head = g_kModelManager.CloneModel( headFile );
		}
		Unknown667D60( body, head );
		head = NULL;
	}
	unknown6C = new char[strlen( bodyFile ) + 1];
	strcpy( unknown6C, bodyFile );
	// BUG: headFile is copied without the NULL test made above
	unknown68 = new char[strlen( headFile ) + 1];
	strcpy( unknown68, headFile );
}

/*
================
CharacterClass::Unknown668000

Gives the body its own vertex colour, material, texture mode and specular
properties in place of the ones its model came with.
================
*/
// FUNCTION: STBC 0x00668000
// PARKED: inline budget: retail calls NiColor::operator= (0x40DB40) out of line in the material setters except the black emittance copy, and expands the texture mode's two setters, its constructor's clamp SetField and SetSpecular, where this build calls them; the SetBit calls match
void CharacterClass::Unknown668000( TGAnimNode *body, TGAnimNode *head ) {
	BaseObjectClass::RemovePropertyFromTree( body, NiVertexColorProperty::VERTEX_COLOR );
	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );
	body->AttachProperty( vertexColor );

	BaseObjectClass::RemovePropertyFromTree( body, NiMaterialProperty::MATERIAL );
	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAmbientColor( NiColor::WHITE );
	material->SetDiffuseColor( NiColor::WHITE );
	material->SetEmittance( NiColor::BLACK );
	material->SetSpecularColor( NiColor::WHITE );
	body->AttachProperty( material );

	BaseObjectClass::RemovePropertyFromTree( body, NiTextureModeProperty::TEXTURE_MODE );
	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	body->AttachProperty( textureMode );

	BaseObjectClass::RemovePropertyFromTree( body, NiSpecularProperty::SPECULAR );
	NiSpecularProperty *specular = new NiSpecularProperty;
	specular->SetSpecular( false );
	body->AttachProperty( specular );
}

/*
================
CharacterClass::Unknown668250

Lights the body with the lights of the character lighting model: their
clones go under a node that undoes the root node's world transform.
================
*/
// FUNCTION: STBC 0x00668250
void CharacterClass::Unknown668250( TGAnimNode *body ) {
	MarkSkinControllers( body );
	NiNode *rootNode = body->GetRootNode();
	body->Update( 0.0f );
	if ( rootNode != NULL ) {
		NiStream stream;
		if ( stream.Load( "data/Models/Characters/character_lighting.NIF" ) ) {
			NiPointer<NiNode> lights = new NiNode;
			lights->SetTranslate( -rootNode->GetWorldTranslate() );
			lights->SetRotate( rootNode->GetWorldRotate().Transpose() );
			float scale = 1.0f / rootNode->GetWorldScale();
			lights->SetScale( scale > 0.0f ? scale : -scale );
			rootNode->AttachChild( lights, false );
			for ( unsigned int i = 0; i < stream.GetObjectCount(); i++ ) {
				NiPointer<NiAVObject> light = (NiAVObject *)stream.GetObjectAt( i )->CreateClone();
				lights->AttachChild( light, false );
				AttachLights( light, body );
			}
		}
	}
}

/*
================
CharacterClass::ReplaceBodyAndHead

Gives the body the new body texture in place of body.tga and the head the
new head texture, through a new overlay controller, in place of head.tga;
then builds the facial overlays anew: the mouth animations and the
speaking and blinking phonemes.
================
*/
// FUNCTION: STBC 0x00668430
void CharacterClass::ReplaceBodyAndHead( char *bodyTexture, char *headTexture_ ) {
	if ( bodyTexture != NULL ) {
		ReplaceTexture( bodyTexture, "body.tga", true );
	}
	if ( headTexture == NULL || strcmp( headTexture, headTexture_ ) != 0 ) {
		delete headTexture;
		headTexture = new char[strlen( headTexture_ ) + 1];
		strcpy( headTexture, headTexture_ );
	}

	overlayController = new TGOverlayController;
	NiImage *headImage = overlayController->SetImage( NULL, headTexture );
	ReplaceTextureImages( niObject, headImage, "head.tga" );

	overlayManager = new TGOverlayManager;
	overlayManager->AddController( GetAnimNode(), "FaceAnim" );
	overlayManager->AddController( overlayController, "FaceImage" );
	g_kAnimationManager.LoadAnimation( "data/animations/mouthopen.NIF", "MouthOpen" );
	g_kAnimationManager.LoadAnimation( "data/animations/mouthopenpartly.NIF", "MouthOpenPartly" );
	g_kAnimationManager.LoadAnimation( "data/animations/mouth_close.NIF", "MouthClosed" );
	overlayManager->AddFamily( "FaceImage", "Blink" );
	overlayManager->AddFamily( "FaceImage", "Speak" );

	// The speaking phonemes: a mouth animation and a face image each.
	TGPhoneme *phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceAnim", "MouthClosed", 0.0f, 0.1f );
	phoneme->AddEntry( "FaceImage", "Speak", 0.0f, 0.1f );
	phoneme->duration = 0.1f;
	overlayManager->AddPhoneme( phoneme );

	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceAnim", "MouthOpenPartly", 0.0f, 0.1f );
	phoneme->AddEntry( "FaceImage", "SpeakU", 0.0f, 0.1f );
	phoneme->duration = 0.1f;
	overlayManager->AddPhoneme( phoneme );

	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceAnim", "MouthOpen", 0.0f, 0.1f );
	phoneme->AddEntry( "FaceImage", "SpeakA", 0.0f, 0.1f );
	phoneme->duration = 0.1f;
	overlayManager->AddPhoneme( phoneme );

	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceAnim", "MouthClosed", 0.0f, 0.1f );
	phoneme->AddEntry( "FaceImage", "SpeakE", 0.0f, 0.1f );
	phoneme->duration = 0.1f;
	overlayManager->AddPhoneme( phoneme );

	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceAnim", "MouthClosed", 0.0f, 0.1f );
	phoneme->AddEntry( "FaceImage", "Speak", 0.0f, 0.1f );
	phoneme->duration = 0.1f;
	overlayManager->AddPhoneme( phoneme );

	// The blinking phonemes: a face image each.
	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceImage", "Blink0", 0.0f, -1.0f );
	phoneme->duration = 0.05f;
	overlayManager->AddPhoneme( phoneme );

	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceImage", "Blink1", 0.0f, -1.0f );
	phoneme->duration = 0.05f;
	overlayManager->AddPhoneme( phoneme );

	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceImage", "Blink2", 0.0f, -1.0f );
	phoneme->duration = 0.05f;
	overlayManager->AddPhoneme( phoneme );

	phoneme = new TGPhoneme;
	phoneme->AddEntry( "FaceImage", "Blink", 0.0f, -1.0f );
	phoneme->duration = 0.05f;
	overlayManager->AddPhoneme( phoneme );
}

/*
================
CharacterClass::Unknown668A00

Fits the head on the body.
================
*/
// FUNCTION: STBC 0x00668a00
void CharacterClass::Unknown668A00( TGAnimNode *body, TGAnimNode *head ) {
	Unknown668A20( body, head );
}

/*
================
CharacterClass::Unknown668A20

Fits the head (and, recursively, each object under it) on the body: a
skinned head takes the body's bones of the same names, or has its bones
attached under the body's objects of their parents' names, gives the body
the image of its head.tga texture, and has its shape attached beside the
body's first bone; an animated head has the target of its keyframe
controller attached under the body's object of the target's parent's name,
unless the body has a node of the target's own name.
================
*/
// FUNCTION: STBC 0x00668a20
// DRAFT: register picks: retail keeps the body in ebp and reuses its parameter slot for IsEmpty's NULL NiPointer (a frame 4 bytes smaller), where this build keeps 0 in ebp; and one GetProperty temporary of the texture test is released through an inline DecRefCount where retail calls NiObject::DecRefCount (0x40CFE0). The calls, the casts and the three passes agree
void CharacterClass::Unknown668A20( TGAnimNode *body, NiAVObject *head ) {
	NiSkinController *skin = FindSkinController( head );
	if ( skin != NULL ) {
		unsigned int boneCount = skin->GetBoneCount();
		NiBone **bones = skin->GetBones();
		for ( unsigned int i = 0; i < boneCount; i++ ) {
			const char *name = bones[i]->GetName();
			NiBone *bone = NiDynamicCast( NiBone, body->GetObjectByName( name ) );
			if ( bone != NULL ) {
				bones[i] = bone;
			} else {
				const char *parentName = bones[i]->GetParent()->GetName();
				NiNode *parent = NiDynamicCast( NiNode, body->GetObjectByName( parentName ) );
				if ( parent != NULL ) {
					parent->AttachChild( bones[i], false );
				}
			}
		}

		for ( NiPropertyList *list = &head->GetPropertyList(); list != NULL && !list->IsEmpty(); list = list->next ) {
			if ( NiIsKindOf( NiTextureProperty, list->GetItem() ) ) {
				NiTextureProperty *texture = NiDynamicCast( NiTextureProperty, list->GetItem() );
				if ( texture->GetImage()->GetURL() != NULL && strstr( texture->GetImage()->GetURL(), "head.tga" ) != NULL ) {
					ReplaceTextureImages( body, texture->GetImage(), "head.tga" );
				}
			}
		}

		NiTriShape *shape = NiDynamicCast( NiTriShape, skin->GetTarget() );
		if ( shape != NULL ) {
			FindBone( body )->GetParent()->AttachChild( shape, false );
		}
	} else {
		NiKeyframeController *keyframes = FindKeyframeController( head );
		if ( keyframes != NULL ) {
			NiObjectNET *target = keyframes->GetTarget();
			const char *name = target->GetName();
			if ( NiDynamicCast( NiNode, body->GetObjectByName( name ) ) == NULL ) {
				NiNode *targetNode = NiDynamicCast( NiNode, target );
				if ( targetNode != NULL ) {
					const char *parentName = targetNode->GetParent()->GetName();
					NiNode *parent = NiDynamicCast( NiNode, body->GetObjectByName( parentName ) );
					if ( parent != NULL ) {
						parent->AttachChild( targetNode, false );
					}
				}
			}
		}
	}

	if ( head != NULL && head->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)head;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child != NULL && child->IsKindOf( &NiAVObject::rtti ) ) {
				Unknown668A20( body, child );
			}
		}
	}
}

/*
================
CharacterClass::SetYesSir
================
*/
// FUNCTION: STBC 0x00668ef0
void CharacterClass::SetYesSir( char *yesSir_ ) {
	if ( yesSir != NULL ) {
		delete yesSir;
	}
	if ( yesSir_ != NULL ) {
		yesSir = new char[strlen( yesSir_ ) + 1];
		strcpy( yesSir, yesSir_ );
	} else {
		yesSir = NULL;
	}
}

/*
================
CharacterClass::AddFacialImage

Loads the image file into the overlay controller under the name, and keeps
a copy of the file name under it.
================
*/
// FUNCTION: STBC 0x00668f60
void CharacterClass::AddFacialImage( char *name_, char *file ) {
	if ( overlayController != NULL ) {
		overlayController->AddImage( name_, file );
		char *copy = new char[strlen( file ) + 1];
		strcpy( copy, file );
		facialImages.SetAt( name_, copy );
	}
}

/*
================
CharacterClass::AddPhoneme

Files the phoneme in the overlay manager, and its ID and group in the list
of the phoneme type; returns the ID, 0 without an overlay manager.
================
*/
// FUNCTION: STBC 0x00669050
int CharacterClass::AddPhoneme( TGPhoneme *phoneme, int group, int type ) {
	if ( overlayManager != NULL ) {
		long id = overlayManager->AddPhoneme( phoneme );
		PhonemeList *list;
		bool found = true;
		if ( !phonemeLists.GetAt( type, list ) ) {
			list = new PhonemeList;
			found = false;
		}
		list->phonemeIDs.AddTail( id );
		list->groups.AddTail( group );
		if ( !found ) {
			phonemeLists.SetAt( type, list );
		}
		return id;
	}
	return 0;
}

/*
================
CharacterClass::GetHeadHeight

The height of the head node of the character's animation; 0 when it has
none.
================
*/
// FUNCTION: STBC 0x006692a0
float CharacterClass::GetHeadHeight( void ) {
	TGAnimNode *animNode = GetAnimNode();
	NiNode *head = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 Head" ) );
	if ( head == NULL ) {
		return 0.0f;
	}
	return head->GetWorldTranslate().z;
}

/*
================
CharacterClass::CanPlayAnimation

A non-interruptable animation always may; while the character is turned
only the turn back may, and while it glances only a glance back or a turn.
================
*/
// FUNCTION: STBC 0x00669300
bool CharacterClass::CanPlayAnimation( CharacterAnimation *animation ) {
	if ( animation == NULL ) {
		return false;
	}
	int type = animation->type;
	if ( type != CAT_NON_INTERRUPTABLE ) {
		if ( turnTowardsLocation != NULL && type != CAT_TURN_BACK ) {
			return false;
		}
		if ( glanceAtLocation != NULL && type != CAT_GLANCE_BACK && type != CAT_TURN ) {
			return false;
		}
	}
	return true;
}

/*
================
CharacterClass::StartAnimation

The state an animation sets as it starts: its flags, and where a turn or
a glance looks. Anything but a glance ends the glance before it.
================
*/
// FUNCTION: STBC 0x00669350
void CharacterClass::StartAnimation( CharacterAnimation *animation ) {
	if ( animation != NULL ) {
		SetFlags( animation->flags );
		if ( animation->type == CAT_TURN ) {
			turnTowardsLocation = new char[strlen( animation->name ) + 1];
			strcpy( turnTowardsLocation, animation->name );
			if ( IsStateSet( CS_GLANCING ) ) {
				if ( glanceAtLocation != NULL ) {
					delete glanceAtLocation;
				}
				glanceAtLocation = NULL;
				ClearFlags( CS_GLANCING );
			}
		}
		if ( animation->type == CAT_GLANCE ) {
			glanceAtLocation = new char[strlen( animation->name ) + 1];
			strcpy( glanceAtLocation, animation->name );
		} else {
			if ( glanceAtLocation != NULL ) {
				delete glanceAtLocation;
			}
			glanceAtLocation = NULL;
			ClearFlags( CS_GLANCING );
		}
	}
}

/*
================
CharacterClass::AnimationEnded

A glance back or a turn back that ends forgets where the character looked
and clears the state it set.
================
*/
// FUNCTION: STBC 0x00669480
void CharacterClass::AnimationEnded( CharacterAnimation *animation ) {
	if ( animation != NULL ) {
		if ( animation->type == CAT_GLANCE_BACK ) {
			if ( glanceAtLocation != NULL ) {
				delete glanceAtLocation;
			}
			glanceAtLocation = NULL;
			ClearFlags( CS_GLANCING );
		}
		if ( animation->type == CAT_TURN_BACK ) {
			if ( turnTowardsLocation != NULL ) {
				delete turnTowardsLocation;
			}
			turnTowardsLocation = NULL;
			ClearFlags( CS_TURNED );
		}
	}
}

/*
================
CharacterClass::PlayNextAnimation

With nothing playing, starts the first queued animation: a glance back or
a turn back gets its animation appended to its sequence, anything else
plays when the current state allows it and is completed unplayed when
not. While the character's tool tip is up and its UI is disabled, the tool
tips drop.
================
*/
// FUNCTION: STBC 0x006694f0
void CharacterClass::PlayNextAnimation( void ) {
	ClearDoneAnimation( NULL );
	if ( currentAnimation == NULL && queuedAnimations.GetSize() > 0 ) {
		CharacterAnimation *animation = queuedAnimations.RemoveHead();
		if ( animation != NULL ) {
			if ( animation->type == CAT_TURN_BACK ) {
				if ( !AppendTurnBack( animation ) ) {
					TGAction *action = (TGAction *)GetTGObjectPtr( animation->actionID );
					if ( action != NULL ) {
						action->Completed();
					}
				}
			} else if ( animation->type == CAT_GLANCE_BACK ) {
				if ( !AppendGlanceAway( animation ) ) {
					TGAction *action = (TGAction *)GetTGObjectPtr( animation->actionID );
					if ( action != NULL ) {
						action->Completed();
					}
				}
			} else {
				TGAction *action = (TGAction *)GetTGObjectPtr( animation->actionID );
				if ( action != NULL ) {
					if ( CanPlayAnimation( animation ) ) {
						StartAnimation( animation );
						action->Play();
					} else {
						action->Completed();
					}
				}
			}
			currentAnimation = animation;
			if ( currentToolTipOwner == this && IsStateSet( CS_UI_DISABLED ) ) {
				PyEmbed::CallFunction( "BridgeHandlers", "DropCharacterToolTips", NULL, NULL, NULL, NULL );
			}
		}
	}
}

/*
================
CharacterClass::IsReadyToSpeak
================
*/
// FUNCTION: STBC 0x00669670
bool CharacterClass::IsReadyToSpeak( void ) {
	return speakingQueue->IsReadyToSpeak();
}

/*
================
CharacterClass::Update

Once a frame while the player's ship lives: speaks the next queued line,
shows the overlays while the character speaks, starts the queued
animation, follows the camera while watching, keeps the tool tip, blinks
while the character faces the camera, starts a random animation when the
wait is over, and lets an extra walk on or off the bridge.
================
*/
// FUNCTION: STBC 0x00669680
// DRAFT: the tool tip's Python call and None reference count differ in order; the speaking overlay update after the head test is one shared tail in retail (push 1 or 0 from a test), here two copies
void CharacterClass::Update( float time ) {
	float elapsed;
	if ( lastUpdateTime < 0.0f ) {
		elapsed = 0.001f;
	} else {
		float delta = time - lastUpdateTime;
		if ( delta < 0.001f ) {
			delta = 0.001f;
		}
		elapsed = delta;
	}
	if ( Game::GetCurrentGame() == NULL ) {
		return;
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || player->IsDead() || player->IsDying() ) {
		return;
	}

	unknownD0 -= elapsed;
	if ( speakingQueue->IsReadyToSpeak() ) {
		speakingQueue->SpeakNextLine();
		lastTalkTime = g_kUtopiaModule.GetGameTime();
	}
	if ( IsSpeaking() ) {
		overlayManager->Unknown707240( true );
	}
	PlayNextAnimation();
	bool animating = IsAnimating();
	bool wasActive = active;
	bool hasQueued = queuedAnimations.GetSize() != 0;
	if ( node != NULL && node->GetAppCulled() ) {
		goto walkExtra;
	}
	if ( watching ) {
		UpdateWatching( elapsed );
	}
	if ( currentToolTipOwner == this && TopWindow::GetTopWindow() != NULL && TopWindow::GetTopWindow()->IsBridgeVisible() ) {
		PyObject *self = VirtualSlot08();
		char function[256];
		sprintf( function, "%sUpdateToolTip", name );
		Py_INCREF( Py_None );
		int result = PyEmbed::CallFunction( "BridgeHandlers", function, NULL, NULL, "(O)", self );
		Py_DECREF( Py_None );
		Py_DECREF( self );
		if ( result == -1 ) {
			PyErr_Print();
		}
	}
	bool facing;
	if ( containingSet != NULL && containingSet->IsRendered() == true && containingSet->GetDotProductToObject( this ) > 0.35f ) {
		facing = true;
		TGAnimNode *animNode = GetAnimNode();
		NiNode *head = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 Head" ) );
		bool seen = false;
		if ( head != NULL ) {
			CameraObjectClass *camera = containingSet->GetActiveCamera();
			NiPoint3 direction = camera->GetWorldLocation();
			direction = head->GetWorldTranslate() - direction;
			direction.Unitize();
			NiPoint3 axis = head->GetWorldRotate() * NiPoint3( 0.0f, 1.0f, 0.0f );
			seen = direction.Dot( axis ) > 0.0f;
		}
		if ( seen ) {
			overlayManager->Unknown707240( IsSpeaking() != 0 );
		} else {
			overlayManager->Unknown707240( true );
			blinkDelay -= elapsed;
			if ( blinkDelay <= 0.0f ) {
				Blink();
			}
		}
	} else {
		facing = false;
		if ( !wasActive && !animating ) {
			if ( IsSpeaking() ) {
				overlayManager->Unknown707240( true );
			}
			goto walkExtra;
		}
		overlayManager->Unknown707240( IsSpeaking() != 0 );
	}
	if ( !animating && !hasQueued && !IsStateSet( CS_TURNED ) && !IsStateSet( CS_GLANCING ) && ( facing || wasActive ) ) {
		unknown144 -= elapsed;
		if ( active ) {
			unknown144 -= elapsed;
		}
		if ( unknown144 <= 0.0f ) {
			if ( !speakingQueue->IsReadyToSpeak() && !IsStateSet( CS_TURNED ) && !IsStateSet( CS_GLANCING ) && !IsAnimating()
				&& ( randomAnimationEnabled || wasActive ) && ( facing || wasActive ) ) {
				TGSequence *sequence = CreateRandomAnimation();
				if ( sequence != NULL ) {
					SetAnimationDoneEvent( sequence, 0 );
					SetCurrentAnimation( sequence, CAT_BREATHE, 0, NULL );
				}
			}
			unknown144 = 0.5f;
		}
	}

walkExtra:
	if ( extra && !animating && extraWalkingEnabled ) {
		if ( unknownC8 > 30.0f && walkingExtra == NULL
			&& (float)(unsigned int)( g_kSystemWrapper.GetRandomNumber( 100 ) + 1 ) * 0.01f < extraWalkChance ) {
			if ( node != NULL && node->GetAppCulled() ) {
				if ( MoveTo( "G" ) ) {
					unknownC8 = -(float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 60 );
					walkingExtra = this;
				}
			} else if ( MoveTo( "L" ) ) {
				unknownC8 = -(float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 30 );
				walkingExtra = this;
			}
		} else {
			unknownC8 += elapsed;
		}
	}
	ObjectClass::Update( time );
}

/*
================
CharacterClass::GetStatus

The status line with the index; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00669cc0
TGParagraph *CharacterClass::GetStatus( int index ) {
	TGParagraph *status = NULL;
	statuses.GetAt( index, status );
	return status;
}

/*
================
CharacterClass::SetStatus

Shows the string as the status line with the index, under the ones before
it in the status box; the scripts make the box on first use.
================
*/
// FUNCTION: STBC 0x00669d10
// PARKED: x87 residue: retail subtracts the line's size from the parent's straight from memory (fld/fsub), this build loads both sizes through the inline accessors and copies the widths to the frame; a local for the offset tried
void CharacterClass::SetStatus( TGString status, int index ) {
	if ( index > MAX_STATUS_INDEX ) {
		return;
	}
	if ( statusBox == NULL ) {
		PyObject *self = VirtualSlot08();
		PyObject *box = NULL;
		int result = PyEmbed::CallFunction( "Bridge.BridgeMenus", "CreateCharacterTooltipBox", "O", &box, "(O)", self );
		Py_DECREF( self );
		if ( result != -1 ) {
			statusBox = (TGPane *)PyEmbed::GetObjectPointer( box );
		}
		PyErr_Print();
	}
	if ( statusBox == NULL ) {
		return;
	}
	TGParagraph *line;
	if ( !statuses.GetAt( index, line ) ) {
		line = new TGParagraph( "", statusBox->GetWidth() - 0.05f, NULL, NULL, 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
		( (TGPane *)statusBox->GetFirstChild() )->InsertChild( 0x7FFFFFFF, line );
		statuses.SetAt( index, line );
	}
	line->SetString( &status );
	line->SetWrapWidth( statusBox->GetWidth() - 0.05f );
	TGPane *parent = line->GetParent();
	if ( parent != NULL ) {
		line->SetPosition( ( parent->GetWidth() - line->GetWidth() ) * 0.5f, ( parent->GetHeight() - line->GetHeight() ) * index * 0.2f );
	}
}

/*
================
CharacterClass::ClearStatus

Drops the status line with the index and deletes it from the status box.
================
*/
// FUNCTION: STBC 0x00669f70
void CharacterClass::ClearStatus( int index ) {
	if ( index < 0 || index > MAX_STATUS_INDEX ) {
		return;
	}
	TGParagraph *status;
	if ( !statuses.GetAt( index, status ) ) {
		return;
	}
	statuses.RemoveAt( index );
	if ( statusBox != NULL ) {
		TGPane *lines = (TGPane *)statusBox->GetFirstChild();
		if ( lines != NULL ) {
			lines->DeleteChild( status );
		}
	}
}

/*
================
CharacterClass::ToolTip

Shows the character's status box on the bridge window, front and centre,
and glances the character at the captain and back; nothing while the
interface is disabled or the box is already the shown one.
================
*/
// FUNCTION: STBC 0x0066a080
void CharacterClass::ToolTip( void ) {
	if ( statusBox == NULL || IsStateSet( CS_UI_DISABLED ) || currentToolTipOwner == this ) {
		return;
	}
	TGSequence *sequence = new TGSequence;
	sequence->AppendAction( new CharacterAction( this, CharacterAction::AT_GLANCE_AT, "Captain", NULL, 0, NULL, 0, 0 ), 0.5f );
	sequence->AppendAction( new CharacterAction( this, CharacterAction::AT_GLANCE_AWAY, "Captain", NULL, 0, NULL, 0, 0 ), 0.5f );
	sequence->Play();

	MainWindow *bridgeWindow = TopWindow::GetTopWindow()->FindMainWindow( 0 );	// MWT_BRIDGE
	if ( statusBox->GetParent() == NULL ) {
		bridgeWindow->InsertChild( 0x7FFFFFFF, statusBox, ( bridgeWindow->GetWidth() - statusBox->GetWidth() ) * 0.5f, 0.0f );
	}
	bridgeWindow->MoveToFront( statusBox, false );
	statusBox->SetVisible();
	currentToolTipOwner = this;
}

/*
================
CharacterClass::RefreshStatusBox

Lets the bridge menus fit the status box again and sets each status line
anew, which lays it out in the box (InterfaceModule::DoTheRightThing calls
it for every character).
================
*/
// FUNCTION: STBC 0x0066a200
// PARKED: register allocation: retail keeps the EH state's -1 in EBP across the loop (`or ebp, -1` before it); this build stores the constant each time; the statements, calls and strings match (an early return made no difference)
void CharacterClass::RefreshStatusBox( void ) {
	if ( statusBox != NULL ) {
		PyObject *box = statusBox->VirtualSlot08();
		PyEmbed::CallFunction( "Bridge.BridgeMenus", "UIFixTooltipBox", NULL, NULL, "(O)", box );
		Py_DECREF( box );
		// only the first four of the status lines (0 to MAX_STATUS_INDEX)
		for ( int i = 0; i < 4; i++ ) {
			TGParagraph *line = GetStatus( i );
			TGString status;
			if ( line != NULL ) {
				line->GetString( &status );
				SetStatus( status, i );
			}
		}
	}
}

/*
================
CharacterClass::UpdateWatching

While the character watches the camera (WatchMe), turns the active camera
to the character's head, upright; the camera's third LookToward argument
falls by half the frame time each frame, to 0.15 at least.
================
*/
// FUNCTION: STBC 0x0066a2e0
void CharacterClass::UpdateWatching( float time ) {
	CameraObjectClass *camera = containingSet->GetActiveCamera();
	TGAnimNode *animNode = GetAnimNode();
	NiNode *head = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 Head" ) );
	NiPoint3 target = head->GetWorldTranslate();
	TGPoint3 position = camera->GetWorldLocation();
	NiPoint3 forward = target - position;
	forward.Unitize();
	NiPoint3 up( 0.0f, 0.0f, 1.0f );
	NiPoint3 right;
	right = forward.UnitCross( up );
	NiPoint3 newUp = right.UnitCross( forward );
	unknown98 -= time * 0.5f;
	if ( unknown98 < 0.15f ) {
		unknown98 = 0.15f;
	}
	camera->LookToward( &forward, &newUp, unknown98 );
}

/*
================
CharacterClass::IsSpeaking
================
*/
// FUNCTION: STBC 0x0066a570
int CharacterClass::IsSpeaking( void ) {
	return speakingQueue->IsSpeaking();
}

/*
================
CharacterClass::IsAnimating

Whether an animation is queued, or the current one's action still exists.
================
*/
// FUNCTION: STBC 0x0066a580
bool CharacterClass::IsAnimating( void ) {
	if ( queuedAnimations.GetSize() > 0 ) {
		return true;
	}
	ClearDoneAnimation( NULL );
	if ( currentAnimation == NULL ) {
		return false;
	}
	return TGObject::GetTGObjectPtr( currentAnimation->actionID ) ? true : false;
}

/*
================
CharacterClass::IsGoingToAnimate
================
*/
// FUNCTION: STBC 0x0066a5c0
bool CharacterClass::IsGoingToAnimate( void ) {
	return queuedAnimations.GetSize() > 0 ? true : false;
}

/*
================
CharacterClass::IsAnimatingInterruptable

Whether the current animation and every queued one may be interrupted.
================
*/
// FUNCTION: STBC 0x0066a5d0
bool CharacterClass::IsAnimatingInterruptable( void ) {
	ClearDoneAnimation( NULL );
	if ( currentAnimation != NULL ) {
		int type = currentAnimation->type;
		if ( type != CAT_INTERRUPTABLE && type != CAT_BREATHE && type != CAT_GLANCE && type != CAT_GLANCE_BACK ) {
			return false;
		}
	}
	NiTListIterator pos = queuedAnimations.GetHeadPos();
	while ( pos != NULL ) {
		CharacterAnimation *animation = queuedAnimations.GetNext( pos );
		if ( animation != NULL ) {
			int type = animation->type;
			if ( type != CAT_INTERRUPTABLE && type != CAT_BREATHE && type != CAT_GLANCE && type != CAT_GLANCE_BACK ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
CharacterClass::IsAnimatingNonInterruptable

Whether the current animation or a queued one may not be interrupted.
================
*/
// FUNCTION: STBC 0x0066a630
bool CharacterClass::IsAnimatingNonInterruptable( void ) {
	ClearDoneAnimation( NULL );
	if ( currentAnimation != NULL && currentAnimation->type == CAT_NON_INTERRUPTABLE ) {
		return true;
	}
	NiTListIterator pos = queuedAnimations.GetHeadPos();
	while ( pos != NULL ) {
		CharacterAnimation *animation = queuedAnimations.GetNext( pos );
		if ( animation != NULL && animation->type == CAT_NON_INTERRUPTABLE ) {
			return true;
		}
	}
	return false;
}

/*
================
CharacterClass::SetMenuEnabled

A disabled menu is taken down.
================
*/
// FUNCTION: STBC 0x0066a6a0
void CharacterClass::SetMenuEnabled( bool enabled ) {
	if ( !enabled ) {
		MenuDown();
		menuEnabled = false;
	} else {
		menuEnabled = true;
	}
}

/*
================
CharacterClass::SetInitiative
================
*/
// FUNCTION: STBC 0x0066a6d0
void CharacterClass::SetInitiative( bool initiative ) {
	if ( initiative ) {
		SetFlags( CS_INITIATIVE );
	} else if ( IsStateSet( CS_INITIATIVE ) ) {
		ClearFlags( CS_INITIATIVE );
	}
}

/*
================
CharacterClass::IsMenuUp

Whether the character's menu is the open top-level menu.
================
*/
// FUNCTION: STBC 0x0066a700
int CharacterClass::IsMenuUp( void ) {
	if ( menu == NULL ) {
		return 0;
	}
	return menu == STTopLevelMenu::GetOpenMenu();
}

/*
================
CharacterClass::GetCharacterFromMenu

The character of the bridge set whose menu it is; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0066a720
CharacterClass *CharacterClass::GetCharacterFromMenu( STTopLevelMenu *menu_ ) {
	if ( menu_ == NULL ) {
		return NULL;
	}
	SetClass *bridge = g_kSetManager.GetSet( "bridge" );
	int count;
	CharacterClass **characters = GetClassObjects( bridge, &count );
	for ( int i = 0; i < count; i++ ) {
		if ( characters[i]->menu == menu_ ) {
			return characters[i];
		}
	}
	return NULL;
}

/*
================
CharacterClass::ClearAnimations

Completes and drops the queued animations, forgets where the character
looks, empties its speaking queue, and drops its random animations, its
animation files and its position zooms.
================
*/
// FUNCTION: STBC 0x0066a7f0
void CharacterClass::ClearAnimations( void ) {
	while ( queuedAnimations.GetSize() != 0 ) {
		CharacterAnimation *animation = queuedAnimations.RemoveHead();
		if ( animation != NULL ) {
			TGAction *action = (TGAction *)TGObject::GetTGObjectPtr( animation->actionID );
			if ( action != NULL ) {
				action->Completed();
			}
			delete animation;
		}
	}
	if ( location != NULL ) {
		delete location;
	}
	location = NULL;
	if ( turnTowardsLocation != NULL ) {
		delete turnTowardsLocation;
	}
	turnTowardsLocation = NULL;
	speakingQueue->ClearQueue();

	int count = unknownE8.GetSize();
	int i;
	for ( i = 0; i < count; i++ ) {
		RandomAnimation *randomAnimation = unknownE8.GetAt( i );
		if ( randomAnimation != NULL ) {
			if ( randomAnimation->name != NULL ) {
				delete randomAnimation->name;
			}
			delete randomAnimation;
		}
	}
	unknownE8.RemoveAll();
	unknown100 = 0;

	count = unknown104.GetSize();
	for ( i = 0; i < count; i++ ) {
		RandomAnimation *randomAnimation = unknown104.GetAt( i );
		if ( randomAnimation != NULL ) {
			if ( randomAnimation->name != NULL ) {
				delete randomAnimation->name;
			}
			delete randomAnimation;
		}
	}
	unknown104.RemoveAll();
	unknown11C = 0;

	count = unknown120.GetSize();
	for ( i = 0; i < count; i++ ) {
		RandomAnimation *randomAnimation = unknown120.GetAt( i );
		if ( randomAnimation != NULL ) {
			if ( randomAnimation->name != NULL ) {
				delete randomAnimation->name;
			}
			delete randomAnimation;
		}
	}
	unknown120.RemoveAll();
	unknown138 = 0;

	animations->DeleteMappings();
	if ( positionZooms != NULL ) {
		for ( i = 0; i < numPositionZooms; i++ ) {
			if ( positionZooms[i].position != NULL ) {
				delete positionZooms[i].position;
			}
			if ( positionZooms[i].lookAtName != NULL ) {
				delete positionZooms[i].lookAtName;
			}
		}
		free( positionZooms );
	}
	positionZooms = NULL;
	numPositionZooms = 0;
}

/*
================
CharacterClass::ClearExtraAnimations

Clears the queued animations that may be interrupted.
================
*/
// FUNCTION: STBC 0x0066aab0
void CharacterClass::ClearExtraAnimations( void ) {
	ClearAnimationsOfType( CAT_BREATHE );
	ClearAnimationsOfType( CAT_INTERRUPTABLE );
	ClearAnimationsOfType( CAT_GLANCE );
	ClearAnimationsOfType( CAT_GLANCE_BACK );
}

/*
================
CharacterClass::AddRandomAnimation

Adds an animation the character may play at random, to the list of its
category, weighted by the weight.
================
*/
// FUNCTION: STBC 0x0066ab00
void CharacterClass::AddRandomAnimation( char *name_, int type, int weight, int unknown ) {
	RandomAnimation *randomAnimation = new RandomAnimation;
	randomAnimation->name = new char[strlen( name_ ) + 1];
	strcpy( randomAnimation->name, name_ );
	randomAnimation->unknown0C = unknown == 1;
	if ( type == CAT_NON_INTERRUPTABLE ) {
		randomAnimation->weight = unknown100 + weight;
		unknownE8.Add( randomAnimation );
		unknown100 += weight;
	} else if ( type == CAT_INTERRUPTABLE ) {
		randomAnimation->weight = unknown11C + weight;
		unknown104.Add( randomAnimation );
		unknown11C += weight;
	} else {
		randomAnimation->weight = unknown138 + weight;
		unknown120.Add( randomAnimation );
		unknown138 += weight;
	}
}

/*
================
CharacterClass::SetBlinkAnimation
================
*/
// FUNCTION: STBC 0x0066ada0
void CharacterClass::SetBlinkAnimation( char *blinkAnimation_ ) {
	if ( blinkAnimation != NULL ) {
		delete blinkAnimation;
	}
	blinkAnimation = NULL;
	if ( blinkAnimation_ != NULL ) {
		blinkAnimation = new char[strlen( blinkAnimation_ ) + 1];
		strcpy( blinkAnimation, blinkAnimation_ );
	}
}

/*
================
CharacterClass::SetActive

An inactive character drops its queued animations that may be interrupted.
================
*/
// FUNCTION: STBC 0x0066ae10
void CharacterClass::SetActive( bool active_ ) {
	active = active_;
	if ( !active_ ) {
		ClearAnimationsOfType( CAT_BREATHE );
		ClearAnimationsOfType( CAT_INTERRUPTABLE );
		ClearAnimationsOfType( CAT_GLANCE );
		ClearAnimationsOfType( CAT_GLANCE_BACK );
	}
}

/*
================
CharacterClass::ClearAnimationsOfType

Skips the actions of the queued animations of the category.
================
*/
// FUNCTION: STBC 0x0066ae50
void CharacterClass::ClearAnimationsOfType( int type ) {
	NiTListIterator pos = queuedAnimations.GetHeadPos();
	while ( pos != NULL ) {
		CharacterAnimation *animation = queuedAnimations.GetNext( pos );
		if ( animation->type == type ) {
			TGAction *action = (TGAction *)TGObject::GetTGObjectPtr( animation->actionID );
			if ( action != NULL ) {
				action->Skip();
			}
		}
	}
}

/*
================
CharacterClass::SetAnimationDoneEvent

When the action completes, the character is told, with the flag to set or
the state to clear (AnimationDoneHandler).
================
*/
// FUNCTION: STBC 0x0066ae90
void CharacterClass::SetAnimationDoneEvent( TGAction *action, long flags_ ) {
	TGIntEvent *event = new TGIntEvent;
	event->SetEventType( ET_CHARACTER_ANIMATION_DONE );
	event->SetDestination( this );
	event->SetSource( action );
	event->SetInt( flags_ );
	action->AddCompletedEvent( event );
}

/*
================
CharacterClass::SetCurrentAnimation

Queues the action as an animation of the category. An animation it
replaces is completed; when the new one is dropped or cancels out, its
action is completed at once instead. A current animation whose action is
gone neither completes nor drops anything: the walk goes straight on to
the queue (retail's NULL test jumps past the drop test), as a queued one
whose action is gone is skipped; empty queue entries are skipped too.
================
*/
// FUNCTION: STBC 0x0066aef0
void CharacterClass::SetCurrentAnimation( TGAction *action, int type, long flags_, char *name_ ) {
	CharacterAnimation *animation = new CharacterAnimation;
	if ( action != NULL ) {
		animation->actionID = action->GetObjID();
	} else {
		animation->actionID = 0;
	}
	animation->type = type;
	animation->flags = flags_;
	animation->name = NULL;
	if ( name_ != NULL ) {
		animation->name = new char[strlen( name_ ) + 1];
		strcpy( animation->name, name_ );
	}

	animationConflict_t conflict = ResolveConflictWithCurrent( animation );
	if ( conflict == AC_REPLACE || conflict == AC_CANCEL ) {
		TGAction *currentAction = (TGAction *)TGObject::GetTGObjectPtr( currentAnimation->actionID );
		if ( currentAction == NULL ) {
			goto queue;
		}
		currentAction->Completed();
	}
	if ( conflict == AC_DROP || conflict == AC_CANCEL ) {
		action->Completed();
		delete animation;
		return;
	}
queue:
	NiTListIterator pos = queuedAnimations.GetHeadPos();
	while ( pos != NULL ) {
		CharacterAnimation *queued = queuedAnimations.GetNext( pos );
		if ( queued == NULL ) {
			continue;
		}
		conflict = ResolveConflict( queued, animation );
		if ( conflict == AC_REPLACE || conflict == AC_CANCEL ) {
			TGAction *queuedAction = (TGAction *)TGObject::GetTGObjectPtr( queued->actionID );
			if ( queuedAction == NULL ) {
				continue;
			}
			queuedAction->Completed();
		}
		if ( conflict == AC_DROP || conflict == AC_CANCEL ) {
			action->Completed();
			delete animation;
			return;
		}
	}
	queuedAnimations.AddTail( animation );
}

/*
================
CharacterClass::ForceBreathe

Drops the current animation and plays the location's breathing animation
(else the plain one) at once, as a non-interruptable animation
(CharacterAction's AT_FORCE_BREATHE).
================
*/
// FUNCTION: STBC 0x0066b190
void CharacterClass::ForceBreathe( void ) {
	delete currentAnimation;
	currentAnimation = NULL;
	char animationName[256];
	sprintf( animationName, "%sBreathe", location );
	TGAction *action = CreateScriptAction( animations->GetDest( animationName ) );
	if ( action == NULL ) {
		action = CreateScriptAction( animations->GetDest( "Breathe" ) );
		if ( action == NULL ) {
			return;
		}
	}
	SetAnimationDoneEvent( action, 0 );
	currentAnimation = new CharacterAnimation;
	currentAnimation->actionID = action->GetObjID();
	currentAnimation->type = CAT_NON_INTERRUPTABLE;
	action->Play();
}

/*
================
CharacterClass::CreateScriptAction

Calls the script function a "module.function" name gives, with the
character's shadow instance, and returns the action it makes; NULL without
a name or when the call fails.
================
*/
// FUNCTION: STBC 0x0066b290
TGAction *CharacterClass::CreateScriptAction( char *function ) {
	if ( function == NULL ) {
		return NULL;
	}
	char module[256];
	strcpy( module, function );
	char *dot = module + strlen( module ) - 1;
	while ( dot > module && *dot != '.' ) {
		dot--;
	}
	if ( *dot == '.' ) {
		*dot = '\0';
	}
	PyObject *self = VirtualSlot08();
	PyObject *result;
	int status = PyEmbed::CallFunction( module, dot + 1, "O", &result, "(O)", self );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
		return NULL;
	}
	TGAction *action = (TGAction *)PyEmbed::GetObjectPointer( result );
	Py_DECREF( result );
	return action;
}

/*
================
CharacterClass::SetLocation

Moves the character to the location at once: drops the queued animations
and an interruptable current one, stops the model's animation, clears the
turn and the glance, and plays the scripts' positioning action.
================
*/
// FUNCTION: STBC 0x0066b3d0
bool CharacterClass::SetLocation( char *location_ ) {
	SetLocationName( location_ );
	PyObject *self = VirtualSlot08();
	PyObject *result;
	int status = PyEmbed::CallFunction( "Bridge.Characters.CommonAnimations", "SetPosition", "O", &result, "(O)", self );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
		return false;
	}
	TGAction *action = (TGAction *)PyEmbed::GetObjectPointer( result );
	Py_DECREF( result );
	if ( action == NULL ) {
		return false;
	}

	while ( queuedAnimations.GetSize() != 0 ) {
		CharacterAnimation *animation = queuedAnimations.RemoveHead();
		if ( animation != NULL ) {
			TGAction *queued = (TGAction *)TGObject::GetTGObjectPtr( animation->actionID );
			if ( queued != NULL ) {
				queued->Skip();
			}
			delete animation;
		}
	}
	if ( currentAnimation != NULL && currentAnimation->type != CAT_NON_INTERRUPTABLE ) {
		TGAction *current = (TGAction *)TGObject::GetTGObjectPtr( currentAnimation->actionID );
		if ( current != NULL ) {
			current->Skip();
		}
		delete currentAnimation;
		currentAnimation = NULL;
	}

	GetAnimNode()->Stop();
	GetAnimNode()->SetTranslate( NiPoint3( 0.0f, 0.0f, 0.0f ) );
	node->SetTranslate( NiPoint3( 0.0f, 0.0f, 0.0f ) );
	UpdateNodeOnly();

	if ( turnTowardsLocation != NULL ) {
		delete turnTowardsLocation;
	}
	turnTowardsLocation = NULL;
	if ( IsStateSet( CS_TURNED ) ) {
		ClearFlags( CS_TURNED );
	}
	if ( glanceAtLocation != NULL ) {
		delete glanceAtLocation;
	}
	glanceAtLocation = NULL;
	if ( IsStateSet( CS_GLANCING ) ) {
		ClearFlags( CS_GLANCING );
	}

	action->Play();
	UpdateNodeOnly();
	return true;
}

/*
================
CharacterClass::SetLocationName
================
*/
// FUNCTION: STBC 0x0066b610
void CharacterClass::SetLocationName( char *location_ ) {
	if ( location != NULL ) {
		delete location;
	}
	location = NULL;
	if ( location_ != NULL ) {
		location = new char[strlen( location_ ) + 1];
		strcpy( location, location_ );
	}
}

/*
================
CharacterClass::MoveTo

Plays the animation that walks the character from its location to the
destination, with the interface disabled until it ends.
================
*/
// FUNCTION: STBC 0x0066b680
bool CharacterClass::MoveTo( char *destination, TGEvent *event ) {
	if ( destination == NULL ) {
		return false;
	}
	char animationName[256];
	sprintf( animationName, "%sTo%s", location, destination );
	TGAction *action = CreateScriptAction( animations->GetDest( animationName ) );
	if ( action == NULL ) {
		return false;
	}
	SetFlags( CS_UI_DISABLED );
	SetAnimationDoneEvent( action, CS_UI_ENABLED );
	if ( event != NULL ) {
		action->AddCompletedEvent( event );
	}
	SetCurrentAnimation( action, CAT_NON_INTERRUPTABLE, CS_UI_DISABLED );
	if ( node != NULL ) {
		node->SetAppCulled( false );
	}
	return true;
}

/*
================
CharacterClass::GlanceAt

Plays the animation that glances from the character's location at the
target.
================
*/
// FUNCTION: STBC 0x0066b760
bool CharacterClass::GlanceAt( char *target, TGEvent *event ) {
	if ( target == NULL ) {
		return false;
	}
	char animationName[256];
	sprintf( animationName, "%sGlance%s", location, target );
	TGAction *action = CreateScriptAction( animations->GetDest( animationName ) );
	if ( action == NULL ) {
		return false;
	}
	SetAnimationDoneEvent( action, 0 );
	if ( event != NULL ) {
		action->AddCompletedEvent( event );
	}
	ClearAnimationsOfType( CAT_BREATHE );
	ClearAnimationsOfType( CAT_INTERRUPTABLE );
	SetCurrentAnimation( action, CAT_GLANCE, CS_GLANCING, target );
	return true;
}

/*
================
CharacterClass::GlanceAway

Ends a glance: an empty sequence clears the glancing state when it is done.
================
*/
// FUNCTION: STBC 0x0066b840
bool CharacterClass::GlanceAway( TGEvent *event ) {
	unknownC0 = -2.0f;
	TGSequence *sequence = new TGSequence;
	SetAnimationDoneEvent( sequence, CS_CLEAR_GLANCE );
	if ( event != NULL ) {
		sequence->AddCompletedEvent( event );
	}
	ClearAnimationsOfType( CAT_BREATHE );
	ClearAnimationsOfType( CAT_INTERRUPTABLE );
	SetCurrentAnimation( sequence, CAT_GLANCE_BACK );
	return true;
}

/*
================
CharacterClass::AppendGlanceAway

Appends the animation that glances away from where the character looked
to the glance back's sequence and plays it. Fails without a glance, an
animation, or the sequence (the animation then plays alone).
================
*/
// FUNCTION: STBC 0x0066b8c0
bool CharacterClass::AppendGlanceAway( CharacterAnimation *animation ) {
	if ( glanceAtLocation == NULL ) {
		return false;
	}
	char animationName[256];
	sprintf( animationName, "%sGlanceAway%s", location, glanceAtLocation );
	TGAction *action = CreateScriptAction( animations->GetDest( animationName ) );
	if ( action == NULL ) {
		return false;
	}
	TGSequence *sequence = (TGSequence *)GetTGObjectPtr( animation->actionID );
	if ( sequence == NULL ) {
		action->Play();
		return false;
	}
	sequence->AppendAction( action );
	sequence->Play();
	return true;
}

/*
================
CharacterClass::TurnTowards

Plays the animation that turns the character from its location towards
the target. An active character asked to turn to the captain glances at
him and back instead, and the call fails.
================
*/
// FUNCTION: STBC 0x0066b990
bool CharacterClass::TurnTowards( char *target, TGEvent *event ) {
	if ( target == NULL ) {
		return false;
	}
	if ( active && strcmp( target, "Captain" ) == 0 ) {
		TGSequence *sequence = new TGSequence;
		CharacterAction *glance = new CharacterAction( this, CharacterAction::AT_GLANCE_AT, target, NULL, 0, NULL, 0, 0 );
		CharacterAction *glanceAway = new CharacterAction( this, CharacterAction::AT_GLANCE_AWAY, NULL, NULL, 0, NULL, 0, 0 );
		sequence->AddAction( glance );
		sequence->AddAction( glanceAway, glance, 0.5f );
		sequence->Play();
		return false;
	}
	char animationName[256];
	sprintf( animationName, "%sTurn%s", location, target );
	TGAction *action = CreateScriptAction( animations->GetDest( animationName ) );
	if ( action == NULL ) {
		return false;
	}
	SetAnimationDoneEvent( action, 0 );
	if ( event != NULL ) {
		action->AddCompletedEvent( event );
	}
	ClearAnimationsOfType( CAT_BREATHE );
	ClearAnimationsOfType( CAT_INTERRUPTABLE );
	ClearAnimationsOfType( CAT_GLANCE );
	ClearAnimationsOfType( CAT_GLANCE_BACK );
	SetCurrentAnimation( action, CAT_TURN, CS_TURNED, target );
	return true;
}

/*
================
CharacterClass::TurnBack

Ends a turn: an empty sequence clears the turned state when it is done.
================
*/
// FUNCTION: STBC 0x0066bba0
bool CharacterClass::TurnBack( TGEvent *event ) {
	unknownC0 = -2.0f;
	TGSequence *sequence = new TGSequence;
	SetAnimationDoneEvent( sequence, CS_CLEAR_TURNED );
	if ( event != NULL ) {
		sequence->AddCompletedEvent( event );
	}
	ClearAnimationsOfType( CAT_BREATHE );
	ClearAnimationsOfType( CAT_INTERRUPTABLE );
	ClearAnimationsOfType( CAT_GLANCE );
	ClearAnimationsOfType( CAT_GLANCE_BACK );
	SetCurrentAnimation( sequence, CAT_TURN_BACK );
	return true;
}

/*
================
CharacterClass::AppendTurnBack

Appends the animation that turns the character back from where it turned
to the turn back's sequence and plays it. Fails without a turn, an
animation, or the sequence (the animation then plays alone).
================
*/
// FUNCTION: STBC 0x0066bc40
bool CharacterClass::AppendTurnBack( CharacterAnimation *animation ) {
	if ( turnTowardsLocation == NULL ) {
		return false;
	}
	char animationName[256];
	sprintf( animationName, "%sBack%s", location, turnTowardsLocation );
	TGAction *action = CreateScriptAction( animations->GetDest( animationName ) );
	if ( action == NULL ) {
		return false;
	}
	TGSequence *sequence = (TGSequence *)GetTGObjectPtr( animation->actionID );
	if ( sequence == NULL ) {
		action->Play();
		return false;
	}
	sequence->AppendAction( action );
	sequence->Play();
	return true;
}

/*
================
CharacterClass::Breathe

An idle character, neither turned nor glancing, plays its location's
breathing animation, else the plain one.
================
*/
// FUNCTION: STBC 0x0066bd10
bool CharacterClass::Breathe( TGEvent *event ) {
	if ( IsAnimating() || !queuedAnimations.IsEmpty() || IsStateSet( CS_TURNED ) || IsStateSet( CS_GLANCING ) ) {
		return false;
	}
	char animationName[256];
	sprintf( animationName, "%sBreathe", location );
	TGAction *action = CreateScriptAction( animations->GetDest( animationName ) );
	if ( action == NULL ) {
		action = CreateScriptAction( animations->GetDest( "Breathe" ) );
		if ( action == NULL ) {
			return false;
		}
	}
	SetAnimationDoneEvent( action, 0 );
	if ( event != NULL ) {
		action->AddCompletedEvent( event );
	}
	SetCurrentAnimation( action, CAT_BREATHE );
	return true;
}

/*
================
CharacterClass::PlayAnimation

Plays the named animation: with the interface disabled for priority 0,
non-interruptable for a negative priority, interruptable for a positive one.
================
*/
// FUNCTION: STBC 0x0066be00
bool CharacterClass::PlayAnimation( char *name_, int priority, TGEvent *event ) {
	if ( name_ == NULL ) {
		return false;
	}
	TGAction *action = CreateScriptAction( animations->GetDest( name_ ) );
	if ( action == NULL ) {
		return false;
	}
	if ( event != NULL ) {
		action->AddCompletedEvent( event );
	}
	if ( priority <= 0 ) {
		if ( priority == 0 ) {
			SetAnimationDoneEvent( action, CS_UI_ENABLED );
			SetCurrentAnimation( action, CAT_NON_INTERRUPTABLE, CS_UI_DISABLED );
		} else {
			SetCurrentAnimation( action, CAT_NON_INTERRUPTABLE );
		}
	} else {
		SetAnimationDoneEvent( action, 0 );
		SetCurrentAnimation( action, CAT_INTERRUPTABLE );
	}
	return true;
}

/*
================
CharacterClass::PlayAnimationFile

Plays the animation file on the character's model: interruptable unless
the priority is 0, which disables the interface until it ends.
================
*/
// FUNCTION: STBC 0x0066beb0
bool CharacterClass::PlayAnimationFile( char *file, int priority, TGEvent *event ) {
	if ( file == NULL ) {
		return false;
	}
	TGAnimAction *animation = new TGAnimAction( GetAnimNode(), file, 0, 0, 0, 1 );
	TGSequence *sequence = new TGSequence;
	sequence->AddAction( animation );
	if ( event != NULL ) {
		sequence->AddCompletedEvent( event );
	}
	if ( priority == 0 ) {
		SetAnimationDoneEvent( sequence, CS_UI_ENABLED );
		SetCurrentAnimation( sequence, CAT_NON_INTERRUPTABLE, CS_UI_DISABLED );
	} else {
		SetAnimationDoneEvent( sequence, 0 );
		SetCurrentAnimation( sequence, CAT_INTERRUPTABLE );
	}
	return true;
}

/*
================
CharacterClass::SpeakLine

Speaks the line of the database at once, without turning.
================
*/
// FUNCTION: STBC 0x0066bfa0
bool CharacterClass::SpeakLine( TGLocalizationDatabase *database_, char *line, int unknown, int unknown2, TGEvent *event ) {
	TGSequence *sequence = CreateLineSequence( database_, line, NULL, 0, unknown, unknown2, 0 );
	if ( sequence == NULL ) {
		return false;
	}
	if ( event != NULL ) {
		sequence->AddCompletedEvent( event );
	}
	ClearAnimationsOfType( CAT_BREATHE );
	ClearAnimationsOfType( CAT_INTERRUPTABLE );
	ClearAnimationsOfType( CAT_GLANCE );
	ClearAnimationsOfType( CAT_GLANCE_BACK );
	sequence->Play();
	return true;
}

/*
================
CharacterClass::SayLine

Speaks the line of the database, turned towards the target first when one
is given.
================
*/
// FUNCTION: STBC 0x0066c020
bool CharacterClass::SayLine( TGLocalizationDatabase *database_, char *line, char *target, int turnBack, int unknown, int unknown2,
							  int turnFirst, TGEvent *event ) {
	TGSequence *sequence = CreateLineSequence( database_, line, target, turnBack, unknown, unknown2, turnFirst );
	if ( sequence == NULL ) {
		return false;
	}
	if ( event != NULL ) {
		sequence->AddCompletedEvent( event );
	}
	ClearAnimationsOfType( CAT_BREATHE );
	ClearAnimationsOfType( CAT_INTERRUPTABLE );
	ClearAnimationsOfType( CAT_GLANCE );
	ClearAnimationsOfType( CAT_GLANCE_BACK );
	sequence->Play();
	return true;
}

/*
================
CharacterClass::IsStateSet
================
*/
// FUNCTION: STBC 0x0066c0c0
int CharacterClass::IsStateSet( int flags_ ) {
	return ( flags & flags_ ) != 0;
}

/*
================
CharacterClass::SetFlags

CS_HIDDEN and CS_VISIBLE hide and show the character's node; the other
flags are added to its state. A character whose interface is disabled
takes its menu down.
================
*/
// FUNCTION: STBC 0x0066c0e0
void CharacterClass::SetFlags( int flags_ ) {
	if ( flags_ == 0 ) {
		return;
	}
	if ( flags_ == CS_HIDDEN ) {
		SetHidden( true );
	} else if ( flags_ == CS_VISIBLE ) {
		SetHidden( false );
	} else {
		if ( !IsStateSet( flags_ ) ) {
			flags |= flags_;
		}
		if ( IsStateSet( CS_UI_DISABLED ) && IsMenuUp() ) {
			MenuDown();
		}
	}
}

/*
================
CharacterClass::ClearFlags

CS_HIDDEN shows the character's node and CS_VISIBLE hides it; the other
flags are taken out of its state.
================
*/
// FUNCTION: STBC 0x0066c160
void CharacterClass::ClearFlags( int flags_ ) {
	if ( flags_ == 0 ) {
		return;
	}
	if ( flags_ == CS_HIDDEN ) {
		SetHidden( false );
	} else if ( flags_ == CS_VISIBLE ) {
		SetHidden( true );
	} else if ( IsStateSet( flags_ ) ) {
		flags &= ~flags_;
	}
}

/*
================
CharacterClass::SetStanding
================
*/
// FUNCTION: STBC 0x0066c1e0
void CharacterClass::SetStanding( int standing ) {
	if ( standing ) {
		SetFlags( CS_STANDING );
	} else {
		ClearFlags( CS_STANDING );
	}
}

/*
================
CharacterClass::Blink

Draws the time to the next blink, then blinks: through the blink phonemes,
closing the eyes stage by stage and opening them again, or, for a
character without blink stages, through its blink animation (the default
one without its own).
================
*/
// FUNCTION: STBC 0x0066c200
void CharacterClass::Blink( void ) {
	if ( blinkChance <= 0.0f ) {
		return;
	}
	blinkDelay = (float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 3500 ) / 1000.0f + 1.5f;
	char name[256];
	if ( blinkStages != -1 ) {
		// BUG: a percentage is drawn and never used.
		g_kSystemWrapper.GetRandomNumber( 100 );
		TGSequence *sequence = new TGSequence;
		TGAction *previous = NULL;
		int i;
		for ( i = 0; i < blinkStages; i++ ) {
			// BUG: the stage's name is formatted and never used.
			sprintf( name, "Blink%d", i );
			TGPhonemeAction *action = new TGPhonemeAction( overlayManager, overlayManager->GetPhoneme( FIRST_BLINK_PHONEME + i ), -1.0f );
			sequence->AddAction( action, previous );
			previous = action;
		}
		for ( i = 0; i < blinkStages - 1; i++ ) {
			// BUG: the stage's name is formatted and never used.
			sprintf( name, "Blink%d", i );
			TGPhonemeAction *action = new TGPhonemeAction( overlayManager, overlayManager->GetPhoneme( FIRST_BLINK_PHONEME + blinkStages - 1 - i ), -1.0f );
			sequence->AddAction( action, previous );
			previous = action;
		}
		TGPhonemeAction *action = new TGPhonemeAction( overlayManager, overlayManager->GetPhoneme( OPEN_EYES_PHONEME ), -1.0f );
		sequence->AddAction( action, previous );
		sequence->Play();
		return;
	}
	TGAnimNode *animNode = GetAnimNode();
	if ( animNode == NULL ) {
		return;
	}
	TGAnimAction *action;
	if ( blinkAnimation == NULL ) {
		g_kAnimationManager.LoadAnimation( "data/animations/blink.NIF", "__blink" );
		action = new TGAnimAction( animNode, "__blink", 0, 0, 0, 0 );
	} else {
		sprintf( name, "%s_blink", location );
		g_kAnimationManager.LoadAnimation( blinkAnimation, name );
		action = new TGAnimAction( animNode, name, 0, 0, 0, 0 );
	}
	if ( action != NULL ) {
		action->Play();
	}
}

/*
================
CharacterClass::AddPositionZoom

Files the zoom and the look-at target for the position, unless it has one.
================
*/
// FUNCTION: STBC 0x0066c530
void CharacterClass::AddPositionZoom( char *position, float zoom, char *lookAtName ) {
	if ( GetPositionZoom( position ) != NO_POSITION_ZOOM ) {
		return;
	}
	positionZooms = (PositionZoom *)realloc( positionZooms, ( numPositionZooms + 1 ) * sizeof( PositionZoom ) );
	positionZooms[numPositionZooms].zoom = zoom;
	positionZooms[numPositionZooms].position = new char[strlen( position ) + 1];
	strcpy( positionZooms[numPositionZooms].position, position );
	if ( lookAtName != NULL ) {
		positionZooms[numPositionZooms].lookAtName = new char[strlen( lookAtName ) + 1];
		strcpy( positionZooms[numPositionZooms].lookAtName, lookAtName );
	} else {
		positionZooms[numPositionZooms].lookAtName = NULL;
	}
	numPositionZooms++;
}

/*
================
CharacterClass::GetPositionZoom

The zoom filed for the position; NO_POSITION_ZOOM when there is none.
================
*/
// FUNCTION: STBC 0x0066c690
float CharacterClass::GetPositionZoom( char *position ) {
	if ( position != NULL ) {
		for ( int i = 0; i < numPositionZooms; i++ ) {
			if ( strcmp( position, positionZooms[i].position ) == 0 ) {
				return positionZooms[i].zoom;
			}
		}
	}
	return NO_POSITION_ZOOM;
}

/*
================
CharacterClass::GetPositionLookAtName

The look-at target filed for the position; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0066c720
char *CharacterClass::GetPositionLookAtName( char *position ) {
	if ( position != NULL ) {
		for ( int i = 0; i < numPositionZooms; i++ ) {
			if ( strcmp( position, positionZooms[i].position ) == 0 ) {
				return positionZooms[i].lookAtName;
			}
		}
	}
	return NULL;
}

/*
================
CharacterClass::SetName

Names the object; a character without a character name takes it as that
too.
================
*/
// FUNCTION: STBC 0x0066c7c0
void CharacterClass::SetName( const char *name_ ) {
	BaseObjectClass::SetName( const_cast<char *>( name_ ) );
	if ( characterName == NULL ) {
		SetCharacterName( name_ );
	}
}

/*
================
CharacterClass::SetCharacterName
================
*/
// FUNCTION: STBC 0x0066c7f0
void CharacterClass::SetCharacterName( const char *characterName_ ) {
	if ( characterName != NULL ) {
		delete characterName;
	}
	characterName = NULL;
	if ( characterName_ != NULL ) {
		characterName = new char[strlen( characterName_ ) + 1];
		strcpy( characterName, characterName_ );
	}
}

/*
================
CharacterClass::ResolveConflict

A breathing animation gives way to anything but another breath; an
interruptable one to a turn or a non-interruptable one, and a second
interruptable one is dropped; a glance gives way to a turn, a turn back or
a non-interruptable animation, and so does a glance back. A glance back to
the same place cancels a queued glance, and a turn back a queued turn; the
current one plays on.
================
*/
// FUNCTION: STBC 0x0066c860
// PARKED: register allocation: retail loads the added type once into eax before the first branch, so the flag goes to BL; this build loads it per branch and keeps the flag in AL (a local for it gives ecx)
CharacterClass::animationConflict_t CharacterClass::ResolveConflict( CharacterAnimation *existing, CharacterAnimation *added ) {
	if ( existing == NULL ) {
		return AC_QUEUE;
	}
	bool isCurrent = false;
	if ( existing == currentAnimation ) {
		isCurrent = true;
	}
	if ( existing->type == CAT_BREATHE ) {
		return added->type != CAT_BREATHE ? AC_REPLACE : AC_DROP;
	}
	if ( existing->type == CAT_INTERRUPTABLE ) {
		if ( added->type == CAT_TURN || added->type == CAT_NON_INTERRUPTABLE ) {
			return AC_REPLACE;
		}
		return added->type != CAT_INTERRUPTABLE ? AC_QUEUE : AC_DROP;
	}
	if ( existing->type == CAT_GLANCE ) {
		if ( added->type == CAT_GLANCE ) {
			return AC_QUEUE;
		}
		if ( added->type == CAT_GLANCE_BACK ) {
			if ( existing->name != NULL && added->name != NULL && strcmp( existing->name, added->name ) == 0 ) {
				return isCurrent ? AC_QUEUE : AC_CANCEL;
			}
			return AC_QUEUE;
		}
		if ( added->type == CAT_TURN || added->type == CAT_TURN_BACK || added->type == CAT_NON_INTERRUPTABLE ) {
			return AC_REPLACE;
		}
		return AC_QUEUE;
	}
	if ( existing->type == CAT_GLANCE_BACK ) {
		if ( added->type == CAT_TURN || added->type == CAT_TURN_BACK || added->type == CAT_NON_INTERRUPTABLE ) {
			return AC_REPLACE;
		}
		return AC_QUEUE;
	}
	if ( existing->type == CAT_TURN && added->type == CAT_TURN_BACK && existing->name != NULL && added->name != NULL && strcmp( existing->name, added->name ) == 0 ) {
		return isCurrent ? AC_QUEUE : AC_CANCEL;
	}
	return AC_QUEUE;
}

/*
================
CharacterClass::ResolveConflictWithCurrent
================
*/
// FUNCTION: STBC 0x0066c9e0
CharacterClass::animationConflict_t CharacterClass::ResolveConflictWithCurrent( CharacterAnimation *added ) {
	return ResolveConflict( currentAnimation, added );
}

/*
================
CharacterClass::CreateLineSequence

A sequence that speaks the line: after a turn towards the target when one
is given (a slow turn when turnFirst is set, else at once), and followed by
a turn back when turnBack is set. NULL when the speaking action can't be
made.
================
*/
// FUNCTION: STBC 0x0066ca00
TGSequence *CharacterClass::CreateLineSequence( TGLocalizationDatabase *database_, char *line, char *target, int turnBack, int unknown,
												 int unknown2, int turnFirst ) {
	TGSequence *sequence = new TGSequence;
	CharacterAction *speak = new CharacterAction( this, CharacterAction::AT_SPEAK_LINE, line, NULL, 0, database_, unknown, unknown2 );
	if ( speak == NULL ) {
		return NULL;
	}
	CharacterAction *turn = NULL;
	if ( target != NULL ) {
		if ( turnFirst ) {
			turn = new CharacterAction( this, CharacterAction::AT_TURN, target, NULL, 0, NULL, 0, 0 );
		} else {
			turn = new CharacterAction( this, CharacterAction::AT_TURN_NOW, target, NULL, 0, NULL, 0, 0 );
		}
		sequence->AddAction( turn );
	}
	sequence->AddAction( speak, turn );
	if ( turnBack ) {
		sequence->AddAction( new CharacterAction( this, CharacterAction::AT_TURN_BACK_NOW, NULL, NULL, 0, NULL, 0, 0 ), speak );
	}
	return sequence;
}

/*
================
CharacterClass::AddSoundToQueue

A line of the second kind is dropped, completed at once, while the
character is about to speak or speaking.
================
*/
// FUNCTION: STBC 0x0066cb90
void CharacterClass::AddSoundToQueue( TGAction *action, int type, int unknown ) {
	if ( speakingQueue != NULL && action != NULL ) {
		if ( type == 2 && ( IsReadyToSpeak() || IsSpeaking() ) ) {
			action->Completed();
			return;
		}
		speakingQueue->AddSoundToQueue( action, type, unknown );
	}
}

/*
================
CharacterClass::CreateRandomAnimation

A sequence playing one of the character's random animations, chosen by
weight: three times in four from unknown120 when it has any, else from the
standing or the seated ones; an inactive character breathes after it. NULL
while animating, with random animations off, or when the active character
may not play the one chosen.
================
*/
// FUNCTION: STBC 0x0066cbf0
TGSequence *CharacterClass::CreateRandomAnimation( void ) {
	if ( IsAnimating() || !randomAnimationEnabled ) {
		return NULL;
	}
	RandomAnimation *animation = NULL;
	if ( unknown138 > 0 && (unsigned int)g_kSystemWrapper.GetRandomNumber( 1000 ) < 750 ) {
		int choice = g_kSystemWrapper.GetRandomNumber( unknown138 );
		int count = unknown120.GetSize();
		for ( int i = 0; i < count; i++ ) {
			animation = unknown120.GetAt( i );
			if ( choice < animation->weight ) {
				break;
			}
		}
	} else if ( IsStateSet( CS_STANDING ) ) {
		if ( unknown100 > 0 ) {
			int choice = g_kSystemWrapper.GetRandomNumber( unknown100 );
			int count = unknownE8.GetSize();
			for ( int i = 0; i < count; i++ ) {
				animation = unknownE8.GetAt( i );
				if ( choice < animation->weight ) {
					break;
				}
			}
		}
	} else {
		if ( unknown11C > 0 ) {
			int choice = g_kSystemWrapper.GetRandomNumber( unknown11C );
			int count = unknown104.GetSize();
			for ( int i = 0; i < count; i++ ) {
				animation = unknown104.GetAt( i );
				if ( choice < animation->weight ) {
					break;
				}
			}
		}
	}
	if ( animation == NULL ) {
		return NULL;
	}
	if ( active && !animation->unknown0C ) {
		return NULL;
	}

	TGSequence *sequence = new TGSequence;
	sequence->AddAction( CreateScriptAction( animation->name ) );
	if ( !active ) {
		char animationName[256];
		sprintf( animationName, "%sBreathe", location );
		TGAction *breathe = CreateScriptAction( animations->GetDest( animationName ) );
		if ( breathe != NULL ) {
			sequence->AppendAction( breathe );
		}
	}
	return sequence;
}

/*
================
CharacterClass::MenuUp

Opens the character's menu, through a menu event to itself; false when it
cannot be opened.
================
*/
// FUNCTION: STBC 0x0066cdf0
bool CharacterClass::MenuUp( void ) {
	if ( menu == NULL || IsStateSet( CS_UI_DISABLED ) || IsHidden() || !menuEnabled || !( menu->GetFlags() & TGUIO_ENABLED ) ) {
		return false;
	}
	TGBoolEvent *event = new TGBoolEvent;
	event->SetDestination( this );
	event->SetBool( true );
	event->SetEventType( ET_CHARACTER_MENU );
	ProcessEvent( event );
	delete event;
	return IsMenuUp() ? true : false;
}

/*
================
CharacterClass::MenuDown

Closes the character's menu, through a menu event to itself.
================
*/
// FUNCTION: STBC 0x0066cea0
bool CharacterClass::MenuDown( void ) {
	if ( menu == NULL ) {
		return false;
	}
	TGBoolEvent *event = new TGBoolEvent;
	event->SetDestination( this );
	event->SetBool( false );
	event->SetEventType( ET_CHARACTER_MENU );
	ProcessEvent( event );
	delete event;
	return IsMenuUp() == 0 ? true : false;
}

/*
================
CharacterClass::LookAtMe

Drops the bridge menus (unless the character's menu is up) and turns the
active camera to the character's head, upright.
================
*/
// FUNCTION: STBC 0x0066cf20
// PARKED: frame: the call set agrees (right assigned from UnitCross: its operator= site leaves the second UnitCross 163 < 166); retail keeps target and right in one frame slot (0x40 against 0x4C) and orders the slots target, forward, position
void CharacterClass::LookAtMe( void ) {
	if ( !IsMenuUp() ) {
		PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", NULL, NULL, NULL, NULL );
	}
	CameraObjectClass *camera = containingSet->GetActiveCamera();
	TGAnimNode *animNode = GetAnimNode();
	NiNode *head = NiDynamicCast( NiNode, animNode->GetObjectByName( "Bip01 Head" ) );
	NiPoint3 target = head->GetWorldTranslate();
	NiPoint3 position = camera->GetWorldLocation();
	PlaceByDirectionMode *mode = PlaceByDirectionMode::Cast( camera->GetCurrentCameraMode( true ) );
	if ( mode != NULL ) {
		position = mode->Unknown427E50( target );
	}
	NiPoint3 forward = target - position;
	forward.Unitize();
	NiPoint3 up( 0.0f, 0.0f, 1.0f );
	NiPoint3 right;
	right = forward.UnitCross( up );
	NiPoint3 newUp = right.UnitCross( forward );
	camera->LookToward( &forward, &newUp, 1.0f );
	unknownD0 = 1.0f;
}

/*
================
CharacterClass::WatchMe

Drops the bridge menus and has the character follow the camera
(CharacterAction's AT_WATCH_ME).
================
*/
// FUNCTION: STBC 0x0066d190
void CharacterClass::WatchMe( void ) {
	PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", NULL, NULL, NULL, NULL );
	watching = true;
	unknown98 = 0.5f;
}

/*
================
CharacterClass::StopWatchingMe

CharacterAction's AT_STOP_WATCHING_ME.
================
*/
// FUNCTION: STBC 0x0066d1e0
void CharacterClass::StopWatchingMe( void ) {
	watching = false;
}

/*
================
CharacterClass::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0066d1f0
void CharacterClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CharacterClass::AnimationDoneHandler, "CharacterClass::AnimationDoneHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CharacterClass::MenuEventHandler, "CharacterClass::MenuEventHandler" );
}

/*
================
CharacterClass::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0066d220
void CharacterClass::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_CHARACTER_ANIMATION_DONE, "CharacterClass::AnimationDoneHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_CHARACTER_MENU, "CharacterClass::MenuEventHandler" );
}

/*
================
CharacterClass::IsCurrentAnimationFlagSet
================
*/
// FUNCTION: STBC 0x0066d250
bool CharacterClass::IsCurrentAnimationFlagSet( int flags_ ) {
	if ( currentAnimation != NULL && ( currentAnimation->flags & flags_ ) ) {
		return true;
	}
	return false;
}

/*
================
CharacterClass::ClearDoneAnimation

Ends the current animation when its action no longer exists, or when the
given action is the one playing it.
================
*/
// FUNCTION: STBC 0x0066d270
void CharacterClass::ClearDoneAnimation( TGAction *action ) {
	if ( currentAnimation == NULL ) {
		return;
	}
	if ( TGObject::GetTGObjectPtr( currentAnimation->actionID ) == NULL ) {
		AnimationEnded( currentAnimation );
		delete currentAnimation;
		currentAnimation = NULL;
	}
	if ( action != NULL ) {
		// BUG: the animation may have been ended just above; it is read without a test
		if ( currentAnimation->actionID == action->GetObjID() ) {
			AnimationEnded( currentAnimation );
			delete currentAnimation;
			currentAnimation = NULL;
		} else if ( currentAnimation != NULL ) {
			// BUG: the action is looked up and the result dropped
			TGObject::GetTGObjectPtr( currentAnimation->actionID );
		}
	}
}

/*
================
CharacterClass::AnimationDoneHandler

An animation's action is done: the flag it carries is set, or the state it
ends is cleared, and an idle character goes back to breathing.
================
*/
// FUNCTION: STBC 0x0066d350
void CharacterClass::AnimationDoneHandler( TGEvent *event ) {
	int flags_ = ( (TGIntEvent *)event )->GetInt();
	ClearDoneAnimation( (TGAction *)event->GetSource() );
	if ( flags_ < CS_MIDDLE ) {
		SetFlags( flags_ );
	} else if ( !IsCurrentAnimationFlagSet( flags_ ) ) {
		if ( flags_ == CS_SEATED ) {
			ClearFlags( CS_STANDING );
		} else if ( flags_ == CS_VISIBLE ) {
			ClearFlags( CS_HIDDEN );
		} else if ( flags_ == CS_CLEAR_TURNED ) {
			if ( turnTowardsLocation != NULL ) {
				delete turnTowardsLocation;
			}
			turnTowardsLocation = NULL;
			ClearFlags( CS_TURNED );
		} else if ( flags_ == CS_CLEAR_GLANCE ) {
			if ( glanceAtLocation != NULL ) {
				delete glanceAtLocation;
			}
			glanceAtLocation = NULL;
			ClearFlags( CS_GLANCING );
		} else if ( flags_ == CS_UI_ENABLED ) {
			ClearFlags( CS_UI_DISABLED );
		}
	}
	if ( walkingExtra == this ) {
		walkingExtra = NULL;
	}
	if ( flags_ != 0 && !IsAnimating() ) {
		Breathe( NULL );
	}
}

/*
================
CharacterClass::MenuEventHandler

Opens or closes the character's menu on the bridge. Opening it turns the
character to the captain and back, shows the menu, points the camera at
the character's look-at target (or the character) and zooms in by the
position's zoom; closing it drops out of manual fire mode, turns the
character back, hides the menu and zooms out.
================
*/
// FUNCTION: STBC 0x0066d450
void CharacterClass::MenuEventHandler( TGEvent *event ) {
	if ( GetMenu() == NULL ) {
		return;
	}
	if ( GetNode() != NULL && GetNode()->GetAppCulled() ) {
		return;
	}
	if ( !IsMenuEnabled() ) {
		return;
	}
	TGBoolEvent *boolEvent = (TGBoolEvent *)event;
	if ( boolEvent->GetBool() == true ) {
		if ( !( GetMenu()->GetFlags() & TGUIO_ENABLED ) ) {
			return;
		}
		if ( IsStateSet( CS_UI_DISABLED ) ) {
			return;
		}
	}

	SetClass *bridge = g_kSetManager.GetSet( "bridge" );
	ZoomCameraObjectClass *camera = ZoomCameraObjectClass::GetObject( bridge, "maincamera" );
	TopWindow *topWindow = TopWindow::GetTopWindow();
	GetMenu()->SetOwner( this );
	NiPoint3 translate = NiPoint3::ZERO;
	TGAnimNode *animNode = NiDynamicCast( TGAnimNode, GetNiObject() );
	if ( boolEvent->GetBool() ) {
		TGSequence *sequence = new TGSequence;
		sequence->AppendAction( new CharacterAction( this, CharacterAction::AT_TURN, "Captain", NULL, 0, NULL, 0, 0 ), 0.0f );
		sequence->AppendAction( new CharacterAction( this, CharacterAction::AT_TURN_BACK, NULL, NULL, 0, NULL, 0, 0 ), 5.0f );
		sequence->Play();
		GetMenu()->SetVisible( true );
		if ( topWindow->IsBridgeVisible() ) {
			if ( lookAtAdj != NiPoint3::ZERO && animNode != NULL ) {
				animNode->SetWorldTranslate( translate + lookAtAdj );
			}
			char *lookAtName = GetPositionLookAtName( location );
			if ( lookAtName == NULL ) {
				LookAtMe();
			} else {
				bridge->Unknown414270( bridge->GetObject( lookAtName ) );
			}
			// BUG: the node is put back at the origin, not where it was
			if ( lookAtAdj != NiPoint3::ZERO && animNode != NULL ) {
				animNode->SetWorldTranslate( translate );
			}
			if ( camera != NULL ) {
				if ( camera->IsZoomed() ) {
					camera->ToggleZoom( g_kUtopiaModule.GetGameTime() );
				}
				float zoom = GetPositionZoom( location );
				if ( zoom == NO_POSITION_ZOOM ) {
					zoom = 0.7f;
				}
				camera->SetZoom( zoom );
				camera->UpdateViewFrustum();
				camera->ToggleZoom( g_kUtopiaModule.GetGameTime() );
			}
		}
	} else {
		PyEmbed::CallFunction( "BridgeHandlers", "DropOutOfManualFireMode", NULL, NULL, NULL, NULL );
		TurnBack();
		GetMenu()->SetNotVisible( true );
		if ( topWindow->IsBridgeVisible() && camera != NULL && camera->IsZoomed() ) {
			camera->ToggleZoom( g_kUtopiaModule.GetGameTime() );
		}
	}
	CallNextHandler( event );
}

/*
================
Unknown66D830

Walks the tree looking at each object's texture properties; nothing calls
it, and it does nothing with what it finds.
================
*/
// FUNCTION: STBC 0x0066d830
// DRAFT: retail calls NiObject::IsKindOf (0x450530) out of line in the unused cast where this build expands it, with a frame 4 bytes smaller; the texture test's conditional form (flagged temporaries) is from the bytes, its original spelling (a macro?) unknown
void Unknown66D830( NiObject *object ) {
	if ( object == NULL ) {
		return;
	}

	if ( object->IsKindOf( &NiAVObject::rtti ) ) {
		NiAVObject *avObject = (NiAVObject *)object;
		for ( NiPropertyList *list = &avObject->GetPropertyList(); list != NULL && !list->IsEmpty(); list = list->next ) {
			if ( NiIsKindOf( NiTextureProperty, list->GetItem() ) ) {
				// BUG: redundant: the texture property found is never used
				NiTextureProperty *texture = NiDynamicCast( NiTextureProperty, list->GetItem() );
			}
		}
	}

	if ( object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child != NULL && child->IsKindOf( &NiAVObject::rtti ) ) {
				Unknown66D830( child );
			}
		}
	}
}

/*
================
CharacterClass::GetWorldLocation

The world position of the animation's root node; the origin without one.
================
*/
// FUNCTION: STBC 0x0066dac0
const TGPoint3 &CharacterClass::GetWorldLocation( void ) {
	TGAnimNode *animNode = BaseObjectClass::GetAnimNode();
	if ( animNode != NULL && animNode->GetRootNode() != NULL ) {
		return static_cast<const TGPoint3 &>( animNode->GetRootNode()->GetWorldTranslate() );
	}
	return TGPoint3::ZERO;
}

/*
================
CharacterClass::FindBone

The object itself when it is a bone, else the first bone found under it,
depth first; NULL without one.
================
*/
// FUNCTION: STBC 0x0066db10
// PARKED: register allocation: retail keeps the node in EDI and the index in EBX and tests the node again after the cast's `and`; this build swaps the two and branches on the `and`'s flags (early returns, an explicit NULL return and a function-scope index changed nothing)
NiBone *CharacterClass::FindBone( NiAVObject *object ) {
	if ( object != NULL ) {
		if ( object->IsKindOf( &NiBone::rtti ) ) {
			return (NiBone *)object;
		}
		NiNode *node = NiDynamicCast( NiNode, object );
		if ( node != NULL ) {
			for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
				NiAVObject *child = node->GetAt( i );
				if ( child != NULL && child->IsKindOf( &NiAVObject::rtti ) ) {
					NiBone *bone = FindBone( child );
					if ( bone != NULL ) {
						return bone;
					}
				}
			}
		}
	}
	return NULL;
}

/*
================
CharacterClass::FindSkinController

The first skin controller among the object's controllers; NULL without one.
================
*/
// FUNCTION: STBC 0x0066dbd0
NiSkinController *CharacterClass::FindSkinController( NiAVObject *object ) {
	NiTimeController *controller = object->GetControllers();
	while ( controller != NULL ) {
		if ( controller->IsKindOf( &NiSkinController::rtti ) ) {
			return (NiSkinController *)controller;
		}
		controller = controller->GetNext();
	}
	return NULL;
}

/*
================
CharacterClass::SetDatabase

Unloads the character's localization database and loads the named one;
none without a name.
================
*/
// FUNCTION: STBC 0x0066dc10
TGLocalizationDatabase *CharacterClass::SetDatabase( char *file ) {
	if ( database != NULL ) {
		g_kLocalizationManager.Unload( database );
	}
	database = NULL;
	if ( file != NULL ) {
		database = g_kLocalizationManager.Load( file, true );
	}
	return database;
}

/*
================
CharacterClass::WriteToStream

Saves the character: its models, state, animations and phonemes, with
the objects it points at saved by ID. The status box is saved with the
character when it has no parent pane.
================
*/
// FUNCTION: STBC 0x0066dc60
bool CharacterClass::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}

	char message[256];
	sprintf( message, "Saving character %s w/anim node %d\n", GetCharacterName(), GetAnimNode() );
	TGObject::Log( message );

	stream->VirtualSlot11( (char *)GetAnimNode()->GetRootNode()->GetName() );
	stream->WriteID( GetSavedID( menu ) );
	stream->WriteLong( size );
	stream->WriteLong( gender );
	stream->WriteLong( flags );
	stream->WriteLong( audioMode );
	if ( !speakingQueue->WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( unknown68 );
	stream->VirtualSlot11( unknown6C );
	stream->VirtualSlot11( headTexture );
	stream->VirtualSlot11( yesSir );
	stream->WriteBool( watching );
	stream->WriteFloat( unknown98 );
	stream->VirtualSlot11( location );
	stream->VirtualSlot11( turnTowardsLocation );
	if ( !animations->WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( blinkChance );
	stream->VirtualSlot11( blinkAnimation );

	NiTArray<RandomAnimation *> *randomAnimations[3] = { &unknownE8, &unknown104, &unknown120 };
	int i;
	for ( i = 0; i < 3; i++ ) {
		NiTArray<RandomAnimation *> *array = randomAnimations[i];
		int count = array->GetSize();
		stream->WriteLong( count );
		RandomAnimation **animations = array->GetBase();
		for ( int j = 0; j < count; j++ ) {
			if ( animations[j] == NULL ) {
				stream->WriteBool( false );
			} else {
				stream->WriteBool( true );
				stream->VirtualSlot11( animations[j]->name );
				stream->WriteLong( animations[j]->unknown04 );
				stream->WriteLong( animations[j]->weight );
			}
		}
	}
	stream->WriteLong( unknown100 );
	stream->WriteLong( unknown11C );
	stream->WriteLong( unknown138 );
	stream->WriteBool( randomAnimationEnabled );
	stream->WriteFloat( randomAnimationChance );
	stream->WriteFloat( unknown144 );
	stream->WriteBool( extra );
	stream->WriteFloat( unknownC8 );

	if ( currentAnimation != NULL ) {
		stream->WriteID( currentAnimation->actionID );
		stream->WriteLong( currentAnimation->type );
		stream->WriteLong( currentAnimation->flags );
		stream->VirtualSlot11( currentAnimation->name );
	} else {
		stream->WriteID( 0 );
	}
	NiTListIterator pos = queuedAnimations.GetHeadPos();
	while ( pos != NULL ) {
		CharacterAnimation *animation = queuedAnimations.GetNext( pos );
		if ( animation != NULL ) {
			stream->WriteBool( true );
			stream->WriteID( animation->actionID );
			stream->WriteLong( animation->type );
			stream->WriteLong( animation->flags );
			stream->VirtualSlot11( animation->name );
		}
	}
	stream->WriteBool( false );

	stream->WriteID( GetSavedID( walkingExtra ) );
	stream->WriteChar( extraWalkingEnabled );
	stream->WriteFloat( extraWalkChance );

	stream->WriteLong( numPositionZooms );
	for ( i = 0; i < numPositionZooms; i++ ) {
		stream->VirtualSlot11( positionZooms[i].position );
		stream->WriteFloat( positionZooms[i].zoom );
		stream->VirtualSlot11( positionZooms[i].lookAtName );
	}
	stream->VirtualSlot11( characterName );
	stream->WriteFloat( lookAtAdj.x );
	stream->WriteFloat( lookAtAdj.y );
	stream->WriteFloat( lookAtAdj.z );
	stream->VirtualSlot17( (long)GetAnimNode() );

	NiTMapIterator imagePos = facialImages.GetFirstPos();
	while ( imagePos != NULL ) {
		stream->WriteBool( true );
		const char *name_;
		char *image;
		facialImages.GetNext( imagePos, name_, image );
		stream->VirtualSlot11( (char *)name_ );
		stream->VirtualSlot11( image );
	}
	stream->WriteBool( false );
	stream->VirtualSlot17( (long)(TGOverlayManager *)overlayManager );
	stream->WriteBool( menuEnabled );
	stream->VirtualSlot11( database == NULL ? NULL : database->GetName() );

	stream->WriteID( GetSavedID( statusBox ) );
	if ( statusBox != NULL ) {
		if ( statusBox->GetParent() == NULL ) {
			stream->WriteBool( true );
			statusBox->WriteToStream( stream );
		} else {
			stream->WriteBool( false );
		}
	}
	// BUG: the last status line (index MAX_STATUS_INDEX) is not saved
	for ( i = 0; i < MAX_STATUS_INDEX; i++ ) {
		TGParagraph *status;
		if ( statuses.GetAt( i, status ) ) {
			stream->WriteID( GetSavedID( status ) );
		} else {
			stream->WriteID( 0 );
		}
	}
	return true;
}

/*
================
CharacterClass::ReadFromStream

Loads what WriteToStream saved. The menu, the status box and the status
lines are read as saved IDs, which PostFixUp turns back into objects; the
models are loaded again from their saved names, and the animation node and
the overlay manager are filed under their saved addresses.
================
*/
// FUNCTION: STBC 0x0066e220
bool CharacterClass::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}

	char *animNodeName = stream->VirtualSlot10( -1 );
	menu = (STTopLevelMenu *)stream->ReadID();
	size = stream->ReadLong();
	gender = stream->ReadLong();
	flags = stream->ReadLong();
	audioMode = stream->ReadLong();
	speakingQueue = (CharacterSpeakingQueue *)TGObject::LoadObject( stream, 0 );
	if ( speakingQueue == NULL ) {
		return false;
	}
	char *headFile = stream->VirtualSlot10( -1 );
	char *bodyFile = stream->VirtualSlot10( -1 );
	char *replacementFile = stream->VirtualSlot10( -1 );
	yesSir = stream->VirtualSlot10( -1 );
	watching = stream->ReadBool();
	unknown98 = stream->ReadFloat();
	location = stream->VirtualSlot10( -1 );
	turnTowardsLocation = stream->VirtualSlot10( -1 );
	if ( !animations->ReadFromStream( stream ) ) {
		return false;
	}
	blinkChance = stream->ReadFloat();
	blinkAnimation = stream->VirtualSlot10( -1 );

	NiTArray<RandomAnimation *> *randomAnimations[3] = { &unknownE8, &unknown104, &unknown120 };
	int i;
	for ( i = 0; i < 3; i++ ) {
		int count = stream->ReadLong();
		if ( count > 0 ) {
			randomAnimations[i]->SetSize( count );
			for ( int j = 0; j < count; j++ ) {
				if ( stream->ReadBool() ) {
					RandomAnimation *animation = new RandomAnimation;
					animation->name = stream->VirtualSlot10( -1 );
					animation->unknown04 = stream->ReadLong();
					animation->weight = stream->ReadLong();
					randomAnimations[i]->SetAt( j, animation );
				} else {
					randomAnimations[i]->SetAt( j, NULL );
				}
			}
		}
	}
	unknown100 = stream->ReadLong();
	unknown11C = stream->ReadLong();
	unknown138 = stream->ReadLong();
	randomAnimationEnabled = stream->ReadBool();
	randomAnimationChance = stream->ReadFloat();
	unknown144 = stream->ReadFloat();
	extra = stream->ReadBool();
	unknownC8 = stream->ReadFloat();

	long actionID = stream->ReadID();
	if ( actionID != 0 ) {
		currentAnimation = new CharacterAnimation;
		currentAnimation->actionID = actionID;
		currentAnimation->type = stream->ReadLong();
		currentAnimation->flags = stream->ReadLong();
		currentAnimation->name = stream->VirtualSlot10( -1 );
	} else {
		currentAnimation = NULL;
	}
	while ( stream->ReadBool() == true ) {
		CharacterAnimation *animation = new CharacterAnimation;
		animation->actionID = stream->ReadID();
		animation->type = stream->ReadLong();
		animation->flags = stream->ReadLong();
		animation->name = stream->VirtualSlot10( -1 );
		queuedAnimations.AddTail( animation );
	}

	long walkingExtraID = stream->ReadID();
	if ( walkingExtra == NULL ) {
		if ( walkingExtraID == 0 ) {
			walkingExtra = NULL;
		} else if ( walkingExtraID == id ) {
			walkingExtra = this;
		}
	}
	extraWalkingEnabled = stream->ReadChar() != 0;
	extraWalkChance = stream->ReadFloat();
	currentToolTipOwner = NULL;

	numPositionZooms = stream->ReadLong();
	positionZooms = numPositionZooms == 0 ? NULL : (PositionZoom *)malloc( numPositionZooms * sizeof( PositionZoom ) );
	for ( i = 0; i < numPositionZooms; i++ ) {
		positionZooms[i].position = stream->VirtualSlot10( -1 );
		positionZooms[i].zoom = stream->ReadFloat();
		positionZooms[i].lookAtName = stream->VirtualSlot10( -1 );
	}
	characterName = stream->VirtualSlot10( -1 );
	lookAtAdj.x = stream->ReadFloat();
	lookAtAdj.y = stream->ReadFloat();
	lookAtAdj.z = stream->ReadFloat();

	g_kModelManager.LoadModel( bodyFile, animNodeName, NULL );
	Unknown667ED0( bodyFile, headFile, 0 );
	ReplaceBodyAndHead( NULL, replacementFile );
	delete animNodeName;
	long savedAnimNode = stream->VirtualSlot16();
	pointerMap.Add( savedAnimNode, GetAnimNode() );

	while ( stream->ReadBool() == true ) {
		char *name_ = stream->VirtualSlot10( -1 );
		char *image = stream->VirtualSlot10( -1 );
		if ( name_ != NULL ) {
			if ( image != NULL ) {
				AddFacialImage( name_, image );
			}
			delete name_;
		}
		if ( image != NULL ) {
			delete image;
		}
	}
	pointerMap.Add( stream->VirtualSlot16(), overlayManager );
	menuEnabled = stream->ReadBool();

	char *databaseName = stream->VirtualSlot10( -1 );
	if ( databaseName != NULL ) {
		database = g_kLocalizationManager.Load( databaseName, false );
	} else {
		database = NULL;
	}
	// SetDatabase unloads the database just loaded and loads it again
	SetDatabase( databaseName );

	statusBox = (TGPane *)stream->ReadID();
	if ( statusBox != NULL && stream->ReadBool() ) {
		TGObject::LoadObject( stream, 0 );
	}
	for ( i = 0; i < MAX_STATUS_INDEX; i++ ) {
		long statusID = stream->ReadID();
		if ( statusID != 0 ) {
			statuses.SetAt( i, (TGParagraph *)statusID );
		}
	}

	char message[256];
	sprintf( message, "Loading character %s w/anim node %d\n", GetCharacterName(), GetAnimNode() );
	TGObject::Log( message );
	delete databaseName;
	delete headFile;
	delete bodyFile;
	delete replacementFile;
	return true;
}

/*
================
CharacterClass::FixUpPointers

Fails until the menu, the status box and every status line are loaded.
================
*/
// FUNCTION: STBC 0x0066ebf0
bool CharacterClass::FixUpPointers( void ) {
	if ( !ObjectClass::FixUpPointers() ) {
		return false;
	}
	if ( menu != NULL && TGObject::GetTGObjectPtr( (long)menu ) == NULL ) {
		return false;
	}
	if ( statusBox != NULL && TGObject::GetTGObjectPtr( (long)statusBox ) == NULL ) {
		return false;
	}
	for ( unsigned int i = 0; i < statuses.GetCount(); i++ ) {
		TGParagraph *status;
		if ( statuses.GetAt( i, status ) && status != NULL && TGObject::GetTGObjectPtr( (long)status ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
CharacterClass::PostFixUp

Turns the saved IDs of the menu, the status box and the status lines back
into the objects.
================
*/
// FUNCTION: STBC 0x0066ecb0
void CharacterClass::PostFixUp( void ) {
	ObjectClass::PostFixUp();
	menu = (STTopLevelMenu *)GetSavedObject( (long)menu );
	statusBox = (TGPane *)GetSavedObject( (long)statusBox );
	for ( unsigned int i = 0; i < statuses.GetCount(); i++ ) {
		TGParagraph *status;
		if ( statuses.GetAt( i, status ) ) {
			statuses.SetAt( i, (TGParagraph *)GetSavedObject( (long)status ) );
		} else {
			statuses.SetAt( i, NULL );
		}
	}
}

/*
================
CharacterClass::VirtualSlot66

Puts the character back at its location, as if it had just been set
there, and hides its status box.
================
*/
// FUNCTION: STBC 0x0066ee50
void CharacterClass::VirtualSlot66( void ) {
	UpdateNodeOnly();
	char *location_ = location;
	location = NULL;
	if ( location_ != NULL ) {
		SetLocation( location_ );
	} else {
		SetLocation( "" );
	}
	char message[256];
	sprintf( message, "Reseting position for %s to \"%s\"\n", characterName, location_ );
	TGObject::Log( message );
	delete location_;
	if ( statusBox != NULL ) {
		statusBox->SetNotVisible();
	}
}
