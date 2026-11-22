/*
===============================================================================

	DamageableObject.cpp

===============================================================================
*/

#include <iostream>
#include "DamageableObject.h"
#include "CollisionEvent.h"
#include "CollisionTestDisplay.h"
#include "LODModelManager.h"
#include "TGModelManager.h"
#include "LODModel.h"
#include "MetaVolume.h"
#include "ModelHelpers.h"
#include "NiPick.h"
#include "CameraObjectClass.h"
#include "Game.h"
#include "SensorSubsystem.h"
#include "ShipClass.h"
#include "ProximityManager.h"
#include "ShieldClass.h"
#include <string.h>
#include "SetClass.h"
#include "SetManager.h"
#include "SpecularPass.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGModelPropertyInstance.h"
#include "TGStream.h"
#include "MultiplayerGame.h"
#include "TGNetwork.h"
#include "TGMessage.h"
#include "TGBufferStream.h"
#include "TGTimerManager.h"
#include "TGTimer.h"
#include "TGStringEvent.h"
#include "UtopiaModule.h"
#include "WeaponHitEvent.h"
#include "WeaponPayload.h"
#include "Weapon.h"
#include "WarpSequence.h"
#include "Torpedo.h"
#include "STTargetMenu.h"
#include "NiLODNode.h"
#include "DamagePolygonizer.h"
#include "NiVertexColorProperty.h"
#include "Force.h"
#include "CollisionRecorder.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00590630 _$E32
// FUNCTION: STBC 0x00590640 _$E29
// FUNCTION: STBC 0x00590650 _$E31
// FUNCTION: STBC 0x00590660 _$E30
// FUNCTION: STBC 0x00590670 _$E38
// FUNCTION: STBC 0x00590680 _$E35
// FUNCTION: STBC 0x00590690 _$E37
// FUNCTION: STBC 0x005906a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005906b0 _$E66
// FUNCTION: STBC 0x005906c0 _$E65
// FUNCTION: STBC 0x005906d0 _$E272
// FUNCTION: STBC 0x005906e0 _$E271
// FUNCTION: STBC 0x005906f0 _$E77
// FUNCTION: STBC 0x00590700 _$E76
// FUNCTION: STBC 0x00590710 _$E81
// FUNCTION: STBC 0x00590720 _$E80
// FUNCTION: STBC 0x00590730 _$E85
// FUNCTION: STBC 0x00590740 _$E84
// FUNCTION: STBC 0x00590750 _$E89
// FUNCTION: STBC 0x00590760 _$E88
// FUNCTION: STBC 0x00590770 _$E206
// FUNCTION: STBC 0x00590780 _$E205
// FUNCTION: STBC 0x00590790 _$E364
// FUNCTION: STBC 0x005907a0 _$E363
// FUNCTION: STBC 0x005907b0 _$E368
// FUNCTION: STBC 0x005907c0 _$E367
// FUNCTION: STBC 0x005907d0 _$E148
// FUNCTION: STBC 0x005907e0 _$E147
// FUNCTION: STBC 0x005907f0 _$E161
// FUNCTION: STBC 0x00590800 _$E160
// FUNCTION: STBC 0x00590810 _$E211
// FUNCTION: STBC 0x00590820 _$E210
// FUNCTION: STBC 0x00590830 _$E277
// FUNCTION: STBC 0x00590840 _$E276
// FUNCTION: STBC 0x00590850 _$E215
// FUNCTION: STBC 0x00590860 _$E214
// FUNCTION: STBC 0x00590870 _$E281
// FUNCTION: STBC 0x00590880 _$E280
// FUNCTION: STBC 0x00590890 _$E288
// FUNCTION: STBC 0x005908a0 _$E287
// FUNCTION: STBC 0x005908b0 _$E398
// FUNCTION: STBC 0x005908c0 _$E397
// FUNCTION: STBC 0x005908d0 _$E402
// FUNCTION: STBC 0x005908e0 _$E401
// FUNCTION: STBC 0x005908f0 _$E106
// FUNCTION: STBC 0x00590900 _$E105
// FUNCTION: STBC 0x00590910 _$E234
// FUNCTION: STBC 0x00590920 _$E233
// FUNCTION: STBC 0x00590a10 _$E241
// FUNCTION: STBC 0x00590a20 _$E240
// FUNCTION: STBC 0x00590c30 _$E245
// FUNCTION: STBC 0x00590c40 _$E244
// FUNCTION: STBC 0x00590c50 _$E249
// FUNCTION: STBC 0x00590c60 _$E248
// FUNCTION: STBC 0x00590c70 _$E253
// FUNCTION: STBC 0x00590c80 _$E252
// FUNCTION: STBC 0x005911b0 _$E259
// FUNCTION: STBC 0x005911c0 _$E258
// FUNCTION: STBC 0x005918d0 _$E406
// FUNCTION: STBC 0x005918e0 _$E405
// FUNCTION: STBC 0x00591b40 _$E410
// FUNCTION: STBC 0x00591b50 _$E409
// FUNCTION: STBC 0x00591ea0 _$E414
// FUNCTION: STBC 0x00591eb0 _$E413
// FUNCTION: STBC 0x00591ec0 _$E418
// FUNCTION: STBC 0x00591ed0 _$E417
// FUNCTION: STBC 0x00592520 _$E422
// FUNCTION: STBC 0x00592530 _$E421
// FUNCTION: STBC 0x00592540 _$E426
// FUNCTION: STBC 0x00592550 _$E425
// FUNCTION: STBC 0x00592560 _$E430
// FUNCTION: STBC 0x00592570 _$E429
// FUNCTION: STBC 0x00592660 _$E434
// FUNCTION: STBC 0x00592670 _$E433
// FUNCTION: STBC 0x005927c0 _$E438
// FUNCTION: STBC 0x005927d0 _$E437
// FUNCTION: STBC 0x00592bc0 _$E442
// FUNCTION: STBC 0x00592bd0 _$E441
// FUNCTION: STBC 0x00592be0 _$E111
// FUNCTION: STBC 0x00592bf0 _$E110
// FUNCTION: STBC 0x00592e70 _$E446
// FUNCTION: STBC 0x00592e80 _$E445
// FUNCTION: STBC 0x00593080 _$E116
// FUNCTION: STBC 0x00593090 _$E115
// FUNCTION: STBC 0x00593230 _$E450
// FUNCTION: STBC 0x00593240 _$E449
// FUNCTION: STBC 0x00593250 _$E454
// FUNCTION: STBC 0x00593260 _$E453
// FUNCTION: STBC 0x00593530 _$E356
// FUNCTION: STBC 0x00593540 _$E355
// FUNCTION: STBC 0x00593630 _$E458
// FUNCTION: STBC 0x00593640 _$E457
// FUNCTION: STBC 0x00593bf0 _$E125
// FUNCTION: STBC 0x00593c00 _$E124
// FUNCTION: STBC 0x00593e30 _$E462
// FUNCTION: STBC 0x00593e40 _$E461
// FUNCTION: STBC 0x00593f10 _$E466
// FUNCTION: STBC 0x00593f20 _$E465
// FUNCTION: STBC 0x00594000 _$E360
// FUNCTION: STBC 0x00594010 _$E359
// FUNCTION: STBC 0x005942b0 _$E131
// FUNCTION: STBC 0x005942c0 _$E130
// FUNCTION: STBC 0x00594680 _$E136
// FUNCTION: STBC 0x00594690 _$E135
// FUNCTION: STBC 0x00595290 _$E166
// FUNCTION: STBC 0x005952a0 _$E165
// FUNCTION: STBC 0x00595e50 _$E470
// FUNCTION: STBC 0x00595e60 _$E469
// FUNCTION: STBC 0x00595e70 _$E171
// FUNCTION: STBC 0x00595e80 _$E170
// FUNCTION: STBC 0x00595e90 _$E474
// FUNCTION: STBC 0x00595ea0 _$E473
// FUNCTION: STBC 0x00595eb0 _$E478
// FUNCTION: STBC 0x00595ec0 _$E477
// FUNCTION: STBC 0x00595ed0 _$E176
// FUNCTION: STBC 0x00595ee0 _$E175
// FUNCTION: STBC 0x00595ef0 _$E482
// FUNCTION: STBC 0x00595f00 _$E481
// FUNCTION: STBC 0x00595f10 _$E141
// FUNCTION: STBC 0x00595f20 _$E140
// FUNCTION: STBC 0x00595f30 _$E486
// FUNCTION: STBC 0x00595f40 _$E485
// FUNCTION: STBC 0x00595f50 _$E490
// FUNCTION: STBC 0x00595f60 _$E489
// FUNCTION: STBC 0x00595f70 _$E181
// FUNCTION: STBC 0x00595f80 _$E180
// FUNCTION: STBC 0x00595f90 _$E186
// FUNCTION: STBC 0x00595fa0 _$E185
// FUNCTION: STBC 0x00595fb0 _$E494
// FUNCTION: STBC 0x00595fc0 _$E493
// FUNCTION: STBC 0x00595fd0 _$E498
// FUNCTION: STBC 0x00595fe0 _$E497
// FUNCTION: STBC 0x00595ff0 _$E502
// FUNCTION: STBC 0x00596000 _$E501
// FUNCTION: STBC 0x00596010 _$E506
// FUNCTION: STBC 0x00596020 _$E505
// FUNCTION: STBC 0x00596030 _$E510
// FUNCTION: STBC 0x00596040 _$E509
// FUNCTION: STBC 0x00596050 _$E514
// FUNCTION: STBC 0x00596060 _$E513
// FUNCTION: STBC 0x00596070 _$E518
// FUNCTION: STBC 0x00596080 _$E517
// FUNCTION: STBC 0x00596090 _$E522
// FUNCTION: STBC 0x005960a0 _$E521
// FUNCTION: STBC 0x005960b0 _$E191
// FUNCTION: STBC 0x005960c0 _$E190
// FUNCTION: STBC 0x005960d0 _$E526
// FUNCTION: STBC 0x005960e0 _$E525
// FUNCTION: STBC 0x005960f0 _$E196
// FUNCTION: STBC 0x00596100 _$E195
// FUNCTION: STBC 0x00596110 _$E201
// FUNCTION: STBC 0x00596120 _$E200
// FUNCTION: STBC 0x00596290 _$E530
// FUNCTION: STBC 0x005962a0 _$E529
// FUNCTION: STBC 0x005962b0 _$E535
// FUNCTION: STBC 0x005962c0 _$E534
// FUNCTION: STBC 0x005962d0 _$E539
// FUNCTION: STBC 0x005962e0 _$E538

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00590930 _$E545
// FUNCTION: STBC 0x00590940 _$E541
// FUNCTION: STBC 0x00590950 _$E544
// FUNCTION: STBC 0x00590960 _$E542
// DATA: STBC 0x0098dc38
TGEventHandlerTable DamageableObject::classHandlers;

// DATA: STBC 0x008e5c18
float INFINITE_LIFE = FLT_MAX;

// A dead object nearer than this to the camera (squared) is left alone.
static const float KEEP_DEAD_OBJECT_DISTANCE_SQUARED = 90000.0f;

// The damage radius per unit of collision force per unit of mass and point, in the unit's constants (retail
// 0x893F28, a copy of 0.1 apart from the pooled literal) (invented name).
static const float COLLISION_DAMAGE_RADIUS_SCALE = 0.1f;

// DATA: STBC 0x008e5c1c
bool DamageableObject::damageGeometryEnabled = true;

// DATA: STBC 0x008e5c1d
bool DamageableObject::volumeDamageGeometryEnabled = true;

// DATA: STBC 0x008e5c1e
bool DamageableObject::breakableComponentsEnabled = true;

/*
================
DamageableObject::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00590980
void DamageableObject::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, DAMAGEABLEOBJECT_TYPE );
		PhysicsObjectClass::AddToSet( set );
	}
}

/*
================
DamageableObject::RemoveFromSet
================
*/
// FUNCTION: STBC 0x005909b0
void DamageableObject::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, DAMAGEABLEOBJECT_TYPE );
		PhysicsObjectClass::RemoveFromSet( set );
	}
}

/*
================
DamageableObject::GetClassObjects

The set's objects of the class and their number; NULL and 0 when there
are none.
================
*/
// FUNCTION: STBC 0x005909e0
DamageableObject **DamageableObject::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (DamageableObject **)set->objectTable.GetObjects( DAMAGEABLEOBJECT_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
DamageableObject::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x00590a30
int DamageableObject::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( DAMAGEABLEOBJECT_TYPE );
	}
	return 0;
}

/*
================
DamageableObject::GetObjectByID

The object with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a DamageableObject.
================
*/
// FUNCTION: STBC 0x00590a50
DamageableObject *DamageableObject::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) ) {
			return (DamageableObject *)object;
		}
		return NULL;
	}
	return (DamageableObject *)set->GetClassObject( DAMAGEABLEOBJECT_TYPE, id );
}

/*
================
DamageableObject::GetObject

The object with the given name in the set, or in the first set that has
one, searching from the last set, when the set is NULL.
================
*/
// FUNCTION: STBC 0x00590ac0
DamageableObject *DamageableObject::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( DAMAGEABLEOBJECT_TYPE, name_ );
		}
		return (DamageableObject *)object;
	}
	return (DamageableObject *)set->GetClassObject( DAMAGEABLEOBJECT_TYPE, name_ );
}

/*
================
DamageableObject::Cast

The object as a DamageableObject; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00590b20
DamageableObject *DamageableObject::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) ) {
		return (DamageableObject *)object;
	}
	return NULL;
}

/*
================
DamageableObject::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00590b50
void DamageableObject::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageableObject::ObjectDestroyedHandler, "DamageableObject::ObjectDestroyedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageableObject::CollisionEffectHandler, "DamageableObject::CollisionEffectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageableObject::DeadObjectRemovalCheck, "DamageableObject::DeadObjectRemovalCheck" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageableObject::ConvertToDeadHulk, "DamageableObject::ConvertToDeadHulk" );
}

/*
================
DamageableObject::RegisterHandlers

A collision is handled twice: by the method, and by the script's
Effects.CollisionEffect.
================
*/
// FUNCTION: STBC 0x00590bb0
void DamageableObject::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_OBJECT_DESTROYED, "DamageableObject::ObjectDestroyedHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_OBJECT_COLLISION, "DamageableObject::CollisionEffectHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_HOST_OBJECT_COLLISION, "DamageableObject::CollisionEffectHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_DEAD_OBJECT_REMOVAL_CHECK, "DamageableObject::DeadObjectRemovalCheck" );
	AddMethodHandlerForClass( &classHandlers, ET_OBJECT_CONVERTED_TO_HULK, "DamageableObject::ConvertToDeadHulk" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_OBJECT_COLLISION, "Effects.CollisionEffect" );
}

/*
================
DamageableObject::SetSpecularKs

Keeps the Ks and gives it to the model's specular passes.
================
*/
// FUNCTION: STBC 0x00590c90
void DamageableObject::SetSpecularKs( float ks ) {
	specularKs = ks;
	SpecularPass::SetSpecularKs( GetNiObject(), ks );
}

/*
================
DamageableObject::InitObject

What the constructors share: alive, with no damage, splash damage over
a unit radius, and the damage modifiers at 1.
================
*/
// FUNCTION: STBC 0x00590cb0
void DamageableObject::InitObject( void ) {
	specularKs = 1.0f;
	unknown108 = NULL;
	unknown15C = true;
	unknown1C4 = NULL;
	unknown13C = new MetaVolumePool;
	unknown140 = 0;
	propertySet = NULL;
	lifeTime = INFINITE_LIFE;
	dead = false;
	splashDamage = 0.0f;
	splashDamageRadius = 1.0f;
	collisionsOn = true;
	unknown1AC = 0;
	unknown1B0 = 0;
	unknown1B4 = 0;
	visibleDamageRadiusModifier = 1.0f;
	visibleDamageStrengthModifier = 1.0f;
	clonedModelCount = 0;
}

/*
================
DamageableObject::DamageableObject

An object loaded by ID; only a multiplayer game gives it properties here.
================
*/
// FUNCTION: STBC 0x00590ec0 ??0DamageableObject@@QAE@J@Z
DamageableObject::DamageableObject( long id ) : PhysicsObjectClass( id ), unknown160( 2 ), unknown178( 2 ), unknown190( 2 ), unknown1C8( 2 ) {
	InitObject();
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		propertySet = new TGModelPropertySet;
	}
}

/*
================
DamageableObject::GetObjType
================
*/
// FUNCTION: STBC 0x005910e0
int DamageableObject::GetObjType( void ) {
	return DAMAGEABLEOBJECT_TYPE;
}

/*
================
DamageableObject::GetClassName
================
*/
// FUNCTION: STBC 0x00591150
char *DamageableObject::GetClassName( void ) {
	return "DamageableObject";
}

/*
================
DamageableObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00591160
char *DamageableObject::GetSwigTypeName( void ) {
	return "_p_DamageableObject";
}

/*
================
DamageableObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00591170
char *DamageableObject::GetPythonClassName( void ) {
	return "DamageableObjectPtr";
}

/*
================
DamageableObject::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00591180
TGEventHandlerTable *DamageableObject::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
DamageableObject::DamageableObject

An object showing a NetImmerse object.
================
*/
// FUNCTION: STBC 0x00591200 ??0DamageableObject@@QAE@PAVNiAVObject@@@Z
DamageableObject::DamageableObject( NiAVObject *object ) : PhysicsObjectClass( object ), unknown160( 2 ), unknown178( 2 ), unknown190( 2 ), unknown1C8( 2 ) {
	InitObject();
	propertySet = new TGModelPropertySet;
}

/*
================
DamageableObject::DamageableObject

An object showing a clone of the named model.
================
*/
// FUNCTION: STBC 0x00591410 ??0DamageableObject@@QAE@PAD@Z
DamageableObject::DamageableObject( char *modelName ) : unknown160( 2 ), unknown178( 2 ), unknown190( 2 ), unknown1C8( 2 ) {
	InitObject();
	SetupModel( modelName );
	propertySet = new TGModelPropertySet;
}

/*
================
DamageableObject::~DamageableObject
================
*/
// FUNCTION: STBC 0x00591620
// FUNCTION: STBC 0x005911d0 ??_GDamageableObject@@UAEPAXI@Z
DamageableObject::~DamageableObject( void ) {
	if ( unknown108 != NULL ) {
		g_kLODModelManager.Unrefer( unknown108 );
		delete unknown108;
	}

	for ( unsigned int i = 0; i < visibleDamage.GetSize(); i++ ) {
		delete visibleDamage.GetAt( i );
	}
	visibleDamage.RemoveAll();

	delete propertySet;
	propertySet = NULL;

	if ( unknown1AC != 0 ) {
		g_kTimerManager.DeleteTimer( unknown1AC );
	}
	delete unknown1C4;
}

/*
================
DamageableObject::RemoveVisibleDamage

Throws the visible damages, the collision models and the damage volumes
away and builds the model again from the LOD model.
================
*/
// FUNCTION: STBC 0x005918f0
void DamageableObject::RemoveVisibleDamage( void ) {
	unsigned int i;
	for ( i = 0; i < visibleDamage.GetSize(); i++ ) {
		delete visibleDamage.GetAt( i );
	}
	visibleDamage.RemoveAll();
	unknown10C.RemoveAll();
	unknown13C->objects.RemoveAll();

	LODModel *lodModel = g_kLODModelManager.GetModel( unknown108 );
	NiPointer<NiAVObject> model;
	lodModel->Unknown451AC0( model, unknown10C, visibleDamage, unknown1B0, unknown1B4, 0.0f );
	unknown58 = unknown10C.GetAt( 0 );
	for ( i = 0; i < visibleDamage.GetSize(); i++ ) {
		if ( visibleDamage.GetAt( i ) != NULL ) {
			visibleDamage.GetAt( i )->SetVolumeList( unknown13C );
		}
	}
	ClearRevisionIDs( model );
	SetNiObject( model );
	unknown140 = model;
	model->UpdateEffects();
	RefreshReplacedTextures();
}

/*
================
DamageableObject::SetupModel

The model of the LOD model of the name, with a collision model and a
visible damage per level, when there is one; else a clone of the model
file.
================
*/
// FUNCTION: STBC 0x00591b60
// PARKED: complete; calls agree (the static Is*Enabled accessors give retail's inline set); register: retail keeps the name in EBX and zero in EDI, this build the reverse
void DamageableObject::SetupModel( char *modelName ) {
	if ( modelName != NULL ) {
		if ( unknown108 != modelName ) {
			delete unknown108;
			unknown108 = new char[strlen( modelName ) + 1];
			strcpy( unknown108, modelName );
		}

		LODModel *lodModel = g_kLODModelManager.GetModel( unknown108 );
		if ( lodModel != NULL ) {
			unknown10C.RemoveAll();
			unsigned int i;
			for ( i = 0; i < visibleDamage.GetSize(); i++ ) {
				delete visibleDamage.GetAt( i );
			}
			visibleDamage.RemoveAll();
			unknown13C->objects.RemoveAll();

			NiPointer<NiAVObject> model;
			lodModel->Unknown451AC0( model, unknown10C, visibleDamage, unknown1B0, unknown1B4, 0.0f );
			unknown58 = unknown10C.GetAt( 0 );
			for ( i = 0; i < visibleDamage.GetSize(); i++ ) {
				if ( visibleDamage.GetAt( i ) != NULL ) {
					visibleDamage.GetAt( i )->SetVolumeList( unknown13C );
					visibleDamage.GetAt( i )->SetBuilderUnknown18( IsDamageGeometryEnabled() );
					visibleDamage.GetAt( i )->SetPolygonizerUnknown09( IsVolumeDamageGeometryEnabled() );
					visibleDamage.GetAt( i )->SetPolygonizerUnknown08( IsBreakableComponentsEnabled() );
				}
			}
			SetNiObject( model );
			unknown140 = model;
			ClearRevisionIDs( model );
		} else {
			NiPointer<NiAVObject> model = g_kModelManager.CloneModel( unknown108 );
			if ( model != NULL ) {
				SetNiObject( model );
			}
		}
	} else {
		unknown108 = NULL;
	}
}


/*
================
DamageableObject::BreakOffPiece

Makes a new object of the grid's cells: a clone of the damage cut to the
grid, under a node that puts the grid's centre at the new object's
origin, placed where the cells are and turned as this object is. It takes
the cells' share of the mass, this object's velocity and spin, and is
dead from the start. With pushOut it is moved out along the line from
this object's centre, else a force pushes it away in a random direction.
================
*/
// FUNCTION: STBC 0x00591ee0
// DRAFT: stack slots of the NiPoint3 temporaries differ, the mass share is multiplied in another x87 order, and the random push direction calls rand for y, x, z in retail
void DamageableObject::BreakOffPiece( DamageVoxelGrid *grid, bool pushOut ) {
	if ( GetContainingSet() == NULL ) {
		return;
	}

	DamageableObject *piece = new DamageableObject( 0L );
	VisibleDamage *damage = visibleDamage.GetAt( 0 );
	VisibleDamage *clone = damage->CloneGrid( grid, piece->unknown13C );
	piece->visibleDamage.Add( clone );

	float scale = GetNiObject()->GetScale();
	NiNode *pieceNode = new NiNode;
	pieceNode->SetTranslate( -grid->center * scale );
	if ( scale > 0.0f ) {
		pieceNode->SetScale( scale );
	} else {
		pieceNode->SetScale( -scale );
	}
	piece->SetNiObject( pieceNode );
	pieceNode->AttachChild( clone->GetModel(), false );

	NiPoint3 position = GetTranslate();
	NiPoint3 piecePosition = position + GetNode()->GetRotate() * ( scale * grid->center );
	piece->GetNode()->SetTranslate( piecePosition );
	piece->GetNode()->SetRotate( GetNode()->GetRotate() );

	float share = (float)grid->setCount / (float)damage->GetCellCount();
	piece->SetMass( share * GetMass() );
	piece->SetRotationalInertia( share * GetRotationalInertia() );
	piece->angularAxis = angularAxis;
	piece->angularDirectionType = angularDirectionType;
	piece->angularSpeed = angularSpeed;
	piece->isStatic = isStatic;
	piece->SetDead();
	piece->SetSplashDamage( 0.0f, 1.0f );
	piece->SetVelocity( &GetVelocity() );
	piece->DisableGlowAlphaMaps();
	piece->GetNode()->Update( UtopiaApp::GetApp()->GetGameTime() );
	piece->unknown1B0 = 0;
	piece->unknown1B4 = unknown1B4;
	piece->unknown10C.Add( clone->GetOBBModel() );
	piece->SetCollisionFlags( 0 );
	GetContainingSet()->AddObjectToSet( piece, NULL );
	piece->GetNode()->UpdateProperties();

	float radius = piece->GetWorldBound().radius;
	if ( !pushOut ) {
		NiPoint3 random( NiSymmetricRandom(), NiSymmetricRandom(), NiSymmetricRandom() );
		NiPoint3 direction = piecePosition - position + random * radius;
		direction.Unitize();
		direction *= piece->GetMass() / 5.0f;
		Force force( position, direction, NiPoint3::ZERO );
		piece->ApplyForce( &force );
	} else {
		NiPoint3 direction = piecePosition - position;
		direction.Unitize();
		float distance = GetNiObject()->GetWorldBound().radius * ( rand() * ( 1.0f / RAND_MAX ) + 0.5f );
		NiNode *moved = piece->GetNode();
		moved->SetTranslate( direction * distance + moved->GetTranslate() );
	}
}

/*
================
DamageableObject::Unknown592580

Pieces break off only while breakable components are on, and never in a
multiplayer game.
================
*/
// FUNCTION: STBC 0x00592580
bool DamageableObject::Unknown592580( VisibleDamage *damage, bool breakOff, bool pushOut ) {
	damage->Polygonize();
	if ( !breakableComponentsEnabled || damage->GetGrids()->GetSize() <= 1 || g_kUtopiaModule.IsMultiplayer() ) {
		damage->Unknown4B2050();
		return false;
	}

	NiTList<DamageVoxelGrid *> *grids = damage->GetGrids();
	NiTListIterator pos = grids->GetHeadPos();
	unsigned int largestCount = 0;
	DamageVoxelGrid *largest = NULL;
	unsigned int totalCount = 0;
	while ( pos != NULL ) {
		DamageVoxelGrid *grid = grids->GetNext( pos );
		unsigned int count = grid->setCount;
		totalCount += count;	// BUG: redundant: the total is never used
		if ( count > largestCount ) {
			largest = grid;
			largestCount = count;
		}
	}
	if ( breakOff ) {
		pos = grids->GetHeadPos();
		while ( pos != NULL ) {
			DamageVoxelGrid *grid = grids->GetNext( pos );
			if ( grid->setCount < largestCount && grid->setCount > 9 ) {
				BreakOffPiece( grid, pushOut );
			}
		}
	}
	damage->Unknown4B1B90( largest );
	damage->Unknown4B2050();
	return true;
}

/*
================
DamageableObject::DamageRefresh

Rebuilds the damage of the full model, and with LOD levels either of every
level or of the current one and of the one shown last.
================
*/
// FUNCTION: STBC 0x00592680
// PARKED: scratch registers: the no-LOD arm's vtable load and the all-levels loop's array base take edx and eax here, eax and edx in retail (two independent picks, not one rotation shift); every arm and loop spelling tried leaves them or loses more (cards)
void DamageableObject::DamageRefresh( bool allLevels, bool ) {
	if ( visibleDamage.GetSize() == 0 ) {
		return;
	}
	NiLODNode *lod = NiDynamicCast( NiLODNode, niObject );
	bool split = false;
	if ( lod == NULL ) {
		if ( Unknown592580( visibleDamage.GetAt( 0 ), true, false ) ) {
			VirtualSlot87();
		}
		return;
	}

	if ( allLevels ) {
		for ( unsigned int i = 0; i < visibleDamage.GetSize(); i++ ) {
			if ( Unknown592580( visibleDamage.GetAt( i ), i == 0, false ) ) {
				split = true;
			}
		}
	} else {
		split = Unknown592580( visibleDamage.GetAt( 0 ), true, false );
		int level = lod->GetIndex();
		if ( level > 0 && level < (int)visibleDamage.GetSize() ) {
			Unknown592580( visibleDamage.GetAt( level ), false, false );
		}
		if ( unknown1B0 != 0 && unknown1B0 != level && unknown1B0 < (int)visibleDamage.GetSize() && visibleDamage.GetAt( unknown1B0 ) != NULL ) {
			visibleDamage.GetAt( unknown1B0 )->Polygonize();
		}
	}
	if ( split ) {
		VirtualSlot87();
	}
}

/*
================
DamageableObject::Unknown5927E0

The damage-refresh process's function: with damage geometry on, one step
of the object's refresh; when it rebuilt a LOD level, the LOD node is
brought up to the game time.
================
*/
// FUNCTION: STBC 0x005927e0
void DamageableObject::Unknown5927E0( double time, void *data ) {
	if ( !damageGeometryEnabled ) {
		return;
	}
	DamageableObject *object = (DamageableObject *)data;
	if ( object->Unknown592960( time ) ) {
		NiLODNode *lod = NiDynamicCast( NiLODNode, object->niObject );
		if ( lod != NULL ) {
			lod->Update( UtopiaApp::GetApp()->GetGameTime() );
		}
	}
}

/*
================
DamageableObject::Unknown592850
================
*/
// FUNCTION: STBC 0x00592850
void DamageableObject::Unknown592850( double ) {
	VisibleDamage *damage = visibleDamage.GetAt( 0 );
	damage->Polygonize();
	if ( damage->GetGrids()->GetSize() <= 1 ) {
		return;
	}

	NiTList<DamageVoxelGrid *> *grids = damage->GetGrids();
	NiTListIterator pos = grids->GetHeadPos();
	unsigned int largestCount = 0;
	DamageVoxelGrid *largest = NULL;
	unsigned int totalCount = 0;
	while ( pos != NULL ) {
		DamageVoxelGrid *grid = grids->GetNext( pos );
		unsigned int count = grid->setCount;
		totalCount += count;	// BUG: redundant: the total is never used
		if ( count > largestCount ) {
			largest = grid;
			largestCount = count;
		}
	}
	pos = grids->GetHeadPos();
	while ( pos != NULL ) {
		DamageVoxelGrid *grid = grids->GetNext( pos );
		if ( grid != largest && grid->setCount > 9 ) {
			BreakOffPiece( grid, false );
		}
	}
	damage->Unknown4B1B90( largest );
	damage->Unknown4B2050();
	VirtualSlot87();
}

/*
================
DamageableObject::Unknown592910
================
*/
// FUNCTION: STBC 0x00592910
void DamageableObject::Unknown592910( VisibleDamage *damage ) {
	NiTList<DamageVoxelGrid *> *grids = damage->GetGrids();
	if ( grids->GetSize() <= 1 ) {
		return;
	}

	NiTListIterator pos = grids->GetHeadPos();
	unsigned int largestCount = 0;
	DamageVoxelGrid *largest = NULL;
	unsigned int totalCount = 0;
	while ( pos != NULL ) {
		DamageVoxelGrid *grid = grids->GetNext( pos );
		unsigned int count = grid->setCount;
		totalCount += count;	// BUG: redundant: the total is never used
		if ( count > largestCount ) {
			largest = grid;
			largestCount = count;
		}
	}
	damage->Unknown4B1B90( largest );
}

/*
================
DamageableObject::Unknown592960
================
*/
// FUNCTION: STBC 0x00592960
bool DamageableObject::Unknown592960( double time ) {
	if ( visibleDamage.GetSize() == 0 ) {
		delete unknown1C4;
		unknown1C4 = NULL;
		return false;
	}

	bool breakOff;
	if ( breakableComponentsEnabled && !g_kUtopiaModule.IsMultiplayer() ) {
		breakOff = true;
		if ( unknown15C ) {
			Unknown592850( time );
			unknown15C = false;
			if ( g_kSystemWrapper.GetTimeSinceFrameStart() >= time ) {
				return true;
			}
		}
	} else {
		breakOff = false;
	}

	NiLODNode *lod = NiDynamicCast( NiLODNode, niObject );
	if ( lod != NULL ) {
		int level = lod->GetIndex();
		bool refreshed = false;
		if ( level > -1 && level < (int)visibleDamage.GetSize() ) {
			VisibleDamage *damage = visibleDamage.GetAt( level );
			if ( damage != NULL ) {
				damage->Polygonize();
				if ( breakOff ) {
					Unknown592910( damage );
				}
				damage->Unknown4B2050();
				refreshed = true;
			}
		}
		if ( unknown1B0 != 0 && unknown1B0 != level && unknown1B0 < (int)visibleDamage.GetSize() ) {
			VisibleDamage *damage = visibleDamage.GetAt( unknown1B0 );
			if ( damage != NULL ) {
				damage->Polygonize();
				if ( breakOff ) {
					Unknown592910( damage );
				}
			}
		}
		unknown15C = true;
		if ( g_kSystemWrapper.GetTimeSinceFrameStart() >= time ) {
			Unknown593F30( 1 );
			return refreshed;
		}

		// the level before the shown one; the full model's damage only needs
		// its builder's rebuilt
		if ( level < 0 ) {
			level = 1;
		}
		if ( level == 1 ) {
			VisibleDamage *damage = visibleDamage.GetAt( 0 );
			if ( damage != NULL ) {
				damage->Unknown4B2050();
				refreshed = true;
			}
		} else if ( level > 1 ) {
			VisibleDamage *damage = visibleDamage.GetAt( level - 1 );
			if ( damage != NULL ) {
				damage->Polygonize();
				if ( breakOff ) {
					Unknown592910( damage );
				}
				damage->Unknown4B2050();
				refreshed = true;
			}
		}
		if ( g_kSystemWrapper.GetTimeSinceFrameStart() >= time ) {
			Unknown593F30( 3 );
			return refreshed;
		}

		// the level after it
		if ( level + 1 < visibleDamage.GetSize() ) {
			VisibleDamage *damage = visibleDamage.GetAt( level + 1 );
			if ( damage != NULL ) {
				damage->Polygonize();
				if ( breakOff ) {
					Unknown592910( damage );
				}
				damage->Unknown4B2050();
				refreshed = true;
			}
		}
		// nothing left to rebuild: the process goes
		if ( !refreshed ) {
			delete unknown1C4;
			unknown1C4 = NULL;
		}
		return refreshed;
	}
	return false;
}

/*
================
DamageableObject::Update

A dying object counts its life time down; at its end the object is
destroyed (ET_OBJECT_DESTROYED, with the set's name) and its splash damage
goes out. The cloned model is updated under the node for the moment.
================
*/
// FUNCTION: STBC 0x00592c00
void DamageableObject::Update( float time ) {
	if ( !dead && lifeTime < INFINITE_LIFE && lastUpdateTime > 0.0f ) {
		lifeTime -= time - lastUpdateTime;
		if ( lifeTime < 0.0f ) {
			TGStringEvent *event = new TGStringEvent;
			event->SetDestination( this );
			event->SetEventType( ET_OBJECT_DESTROYED );
			event->SetString( containingSet->GetName() );
			g_kEventManager.AddEvent( event );
			lifeTime = INFINITE_LIFE;
			SetDead();
			ApplySplashDamage( 1.0f );
		}
	}

	unknown160.RemoveAll();
	while ( unknown178.GetSize() != 0 ) {
		unknown160.AddTail( unknown178.RemoveHead() );
	}

	PhysicsObjectClass::Update( time );

	if ( clonedModel != NULL ) {
		node->AttachChild( clonedModel, false );
		node->Update( time );
		node->DetachChild( clonedModel );
	}
}

/*
================
DamageableObject::ObjectDestroyedHandler

The object has been destroyed. Unless the removal check deletes it at
once, an object in a set leaves the set; a stand-in object is made to
announce, with the set's name, that the object was converted to a hulk, and
a timer is set to send the stand-in a removal check. Then a delete event is
queued for the object.
================
*/
// FUNCTION: STBC 0x00592e90
void DamageableObject::ObjectDestroyedHandler( TGEvent *event ) {
	if ( containingSet == NULL ) {
		TGEvent *deleteEvent = new TGEvent;
		deleteEvent->SetEventType( ET_DELETE_ME_STAGE_1 );
		deleteEvent->SetDestination( this );
		g_kEventManager.AddEvent( deleteEvent );
	} else {
		DeadObjectRemovalCheck( NULL );
		if ( !deleteMe ) {
			SetClass *set = containingSet;
			if ( set != NULL ) {
				set->RemoveObjectFromSet( id );
				DamageableObject *standIn = new DamageableObject( 0L );

				TGStringEvent *converted = new TGStringEvent;
				converted->SetEventType( ET_OBJECT_CONVERTED_TO_HULK );
				converted->SetDestination( this );
				converted->SetSource( standIn );
				converted->SetString( set->GetName() );
				g_kEventManager.AddEvent( converted );

				TGEvent *removalCheck = new TGEvent;
				removalCheck->SetDestination( standIn );
				removalCheck->SetEventType( ET_DEAD_OBJECT_REMOVAL_CHECK );

				TGTimer *timer = new TGTimer;
				timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + 20.0f );
				timer->SetDuration( TIMER_FOREVER );
				timer->SetDelay( 20.0f );
				timer->SetEvent( removalCheck );
				standIn->unknown1AC = timer->GetObjID();
				g_kTimerManager.AddTimer( timer );
			}
			TGEvent *deleteEvent = new TGEvent;
			deleteEvent->SetEventType( ET_DELETE_ME_STAGE_1 );
			deleteEvent->SetDestination( this );
			g_kEventManager.AddEvent( deleteEvent );
		}
	}
	CallNextHandler( event );
}

/*
================
DamageableObject::DeadObjectRemovalCheck

A dead object is deleted unless the player can still see it: not a
planet-sized asteroid, not the player's target, and in the player's set,
visible to the player's sensors, ahead of the camera and nearer than the
removal distance. Outside a set the object is flagged for deletion at once,
else a delete event is queued.
================
*/
// FUNCTION: STBC 0x005930a0
void DamageableObject::DeadObjectRemovalCheck( TGEvent *event ) {
	bool keep = false;
	if ( !g_kUtopiaModule.IsMultiplayer() && unknown108 != NULL
		&& strncmp( "Asteroid", unknown108, 8 ) != 0 && Game::GetCurrentGame() != NULL ) {
		ShipClass *player = Game::GetCurrentGame()->GetPlayer();
		if ( player != NULL ) {
			if ( player->GetTarget() == this ) {
				keep = true;
			} else {
				SetClass *set = containingSet;
				if ( set != NULL && set == player->GetContainingSet() && player->GetSensorSubsystem()->IsObjectVisible( this ) ) {
					CameraObjectClass *camera = set->GetActiveCamera();
					NiPoint3 offset = GetWorldLocation() - camera->GetWorldLocation();
					if ( offset.Dot( camera->GetWorldForward() ) >= 0.0f || offset.SqrLength() < KEEP_DEAD_OBJECT_DISTANCE_SQUARED ) {
						keep = true;
					}
				}
			}
		}
	}
	if ( !keep ) {
		if ( containingSet != NULL ) {
			deleteMe = true;
		} else {
			TGEvent *deleteEvent = TGEvent::Create();
			deleteEvent->SetEventType( ET_DELETE_ME_STAGE_1 );
			deleteEvent->SetDestination( this );
			g_kEventManager.AddEvent( deleteEvent );
		}
	}
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
DamageableObject::Unknown593270

The first hold clones the model (a LOD node's active child, else its first
child, at the node's scale) and shows the clone in its place, lit through
its vertex colours; the model waits in clonedModel for Unknown593550.
================
*/
// FUNCTION: STBC 0x00593270
// DRAFT: frame slots (clone and scale swapped) and the clone pointers' reference increments take other registers
void DamageableObject::Unknown593270( void ) {
	clonedModelCount++;
	if ( niObject == NULL || clonedModel != NULL || clonedModelCount > 1 ) {
		return;
	}
	NiPointer<NiAVObject> clone;
	NiAVObject *object = niObject;		// NiDynamicCast evaluates its argument twice
	NiLODNode *lod = NiDynamicCast( NiLODNode, object );
	float scale = 1.0f;
	if ( lod != NULL ) {
		int index = lod->GetIndex();
		if ( lod->GetActiveChild() != NULL ) {
			scale = lod->GetScale();
			clone = (NiAVObject *)lod->GetAt( index )->Clone();
		} else if ( lod->GetAt( 0 ) != NULL ) {
			scale = lod->GetScale();
			clone = (NiAVObject *)lod->GetAt( 0 )->Clone();
		}
	}
	if ( clone == NULL ) {
		clone = (NiAVObject *)niObject->Clone();
		if ( clone == NULL ) {
			return;
		}
	}
	UnshareTriShapeData( clone, true );
	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_AMB_DIFF );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );
	clone->AttachProperty( vertexColor );
	clonedModel = niObject;
	if ( scale <= 0.0f ) {
		clone->SetScale( -scale );
	} else {
		clone->SetScale( scale );
	}
	SetNiObject( clone );
	clone->Update( UtopiaApp::GetApp()->GetGameTime() );
	clone->UpdateEffects();
}

/*
================
DamageableObject::Unknown593550
================
*/
// FUNCTION: STBC 0x00593550
void DamageableObject::Unknown593550( void ) {
	if ( clonedModelCount > 0 && --clonedModelCount <= 0 && clonedModel != NULL ) {
		SetNiObject( clonedModel );
		clonedModel->UpdateEffects();
		clonedModel->Update( UtopiaApp::GetApp()->GetGameTime() );
		clonedModel = NULL;
	}
}

/*
================
DamageableObject::SetupProperties

Hands slot 88 every property of every list of the property set, then
calls slot 89.
================
*/
// FUNCTION: STBC 0x005935d0
void DamageableObject::SetupProperties( void ) {
	NiTList<TGNamedModelPropertyList *> *lists = propertySet->GetLists();
	NiTListIterator pos = lists->GetHeadPos();
	while ( pos ) {
		TGNamedModelPropertyList *list = lists->GetNext( pos );
		NiTListIterator instancePos = list->GetHeadPos();
		while ( instancePos ) {
			VirtualSlot88( list->GetNext( instancePos )->GetProperty() );
		}
	}
	VirtualSlot89();
}

/*
================
DamageableObject::AddDamage

Damage at the object's position, carried into this object's model frame.
================
*/
// FUNCTION: STBC 0x00593650
void DamageableObject::AddDamage( NiAVObject *object, float radius, float strength ) {
	NiNode *node = GetNode();
	if ( node != NULL ) {
		NiPoint3 center = ( ( object->GetWorldTranslate() - node->GetWorldTranslate() ) * node->GetWorldRotate() ) * ( 1.0f / node->GetWorldScale() );
		AddDamageVolume( center, radius, strength );
	}
}

/*
================
DamageableObject::ConvertToDeadHulk

Hands this object's model, collision models, visible damage and spin to
the dead hulk that sent the event, which joins the set the event names;
without that set the hulk is deleted. This object must already be out of
its set.
================
*/
// FUNCTION: STBC 0x005936f0
void DamageableObject::ConvertToDeadHulk( TGStringEvent *event ) {
	if ( GetContainingSet() != NULL ) {
		return;
	}
	DamageableObject *hulk = Cast( event->GetSource() );
	if ( hulk == NULL ) {
		return;
	}

	char *setName = event->GetCString();
	SetClass *set = g_kSetManager.GetSet( setName );
	delete [] setName;
	if ( set == NULL ) {
		TGEvent *deleteEvent = new TGEvent;
		deleteEvent->SetEventType( ET_DELETE_OBJECT_PUBLIC );
		deleteEvent->SetDestination( hulk );
		g_kEventManager.AddEvent( deleteEvent );
	} else {
		hulk->node = node;
		node = NULL;
		hulk->niObject = niObject;
		niObject = NULL;
		hulk->unknown58 = unknown58;
		unknown58 = NULL;

		hulk->SetAcceleration( NiPoint3::ZERO );
		hulk->SetAngularAcceleration( &NiPoint3::ZERO );
		hulk->mass = mass;
		hulk->rotationalInertia = rotationalInertia;
		hulk->angularAxis = angularAxis;
		hulk->angularDirectionType = angularDirectionType;
		// a slow spin is sped up so the hulk is seen to tumble
		if ( angularSpeed > 0.01f && angularSpeed < 0.175f ) {
			hulk->angularSpeed = 0.175f;
		} else {
			hulk->angularSpeed = angularSpeed;
		}
		hulk->isStatic = isStatic;

		// the hulk drifts off sideways at a quarter of its speed
		TGPoint3 velocity = hulk->GetVelocityTG();
		float drift = velocity.Length() * 0.25f;
		TGPoint3 sideways = TGPoint3::GetRandomUnitVector().GetPerpendicularComponent( velocity );
		sideways.Unitize();
		NiPoint3 newVelocity = velocity + sideways * drift;
		hulk->SetVelocity( &newVelocity );
		hulk->dead = true;
		hulk->splashDamage = 0.0f;
		hulk->splashDamageRadius = 1.0f;

		hulk->unknown108 = unknown108;
		unknown108 = NULL;
		unsigned int i;
		for ( i = 0; i < unknown10C.GetSize(); i++ ) {
			hulk->unknown10C.Add( unknown10C.GetAt( i ) );
		}
		unknown10C.RemoveAll();
		for ( i = 0; i < visibleDamage.GetSize(); i++ ) {
			hulk->visibleDamage.Add( visibleDamage.GetAt( i ) );
		}
		visibleDamage.RemoveAll();
		hulk->unknown13C = unknown13C;
		unknown13C = NULL;
		hulk->clonedModel = clonedModel;
		clonedModel = NULL;
		hulk->unknown1B0 = unknown1B0;
		hulk->unknown1B4 = unknown1B4;
		hulk->DisableGlowAlphaMaps();
		set->AddObjectToSet( hulk, NULL );
	}
	CallNextHandler( event );
}

/*
================
DamageableObject::ApplySplashDamage

Each ship's damage falls off linearly with its distance, and its shields
take an equal share of it on each side first.
================
*/
// FUNCTION: STBC 0x00593c10
// PARKED: complete; retail passes the shield's new value through a frame slot (fstp, then push through eax: a 4-byte local ours lacks, frame 0x28 against 0x24), and the scratch and callee-saved picks around it follow; a named new value or shield -= share don't keep the slot
void DamageableObject::ApplySplashDamage( float fraction ) {
	if ( splashDamage <= 0.0f || splashDamageRadius <= 0.0f || containingSet == NULL ) {
		return;
	}
	ProximityManager *proximity = containingSet->GetProximityManager();
	if ( proximity == NULL ) {
		return;
	}

	float radius = splashDamageRadius;
	NiPoint3 position = GetWorldLocation();
	ProximityManager::ObjectIterator *iterator = proximity->GetNearObjects( position, radius, false );
	while ( true ) {
		ObjectClass *object = proximity->GetNextObject( iterator );
		float damage = fraction * splashDamage;
		if ( object == NULL ) {
			break;
		}
		ShipClass *ship = ShipClass::Cast( object );
		if ( ship == NULL || ship->GetLifeTime() < INFINITE_LIFE || ship->IsDead() || ship == this ) {
			continue;
		}

		float distance = ( ship->GetWorldLocation() - GetWorldLocation() ).Length();
		damage = ( 1.0f - distance / splashDamageRadius ) * damage;
		if ( damage <= 0.0f ) {
			continue;
		}

		float absorbed = 0.0f;
		ShieldClass *shields = ship->GetShields();
		if ( shields != NULL && shields->IsOn() && !shields->IsDisabled() ) {
			for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
				float shield = shields->GetCurShields( i );
				float share = damage * ( 1.0f / ShieldClass::NUM_SHIELDS );
				if ( shield < share ) {
					share = shield;
				}
				shields->SetCurShields( i, shield - share );
				absorbed += share;
			}
		}
		damage -= absorbed;
		if ( damage <= 0.0f ) {
			continue;
		}
		ship->Unknown5AFD70( GetWorldLocation(), &damage, splashDamageRadius, this, true );
	}
	proximity->EndObjectIteration( iterator );
}

/*
================
DamageableObject::AddMetaVolume

Scales the volume's radius and strength by the visible damage modifiers,
hands it to each of the visible damages and the volume list, and sets up
the refresh process at the priority 1 (invented name).
================
*/
// FUNCTION: STBC 0x00593e50
void DamageableObject::AddMetaVolume( MetaVolume *volume ) {
	if ( visibleDamageRadiusModifier != 1.0f ) {
		volume->SetRadius( volume->GetRadius() * visibleDamageRadiusModifier );
	}
	if ( visibleDamageStrengthModifier != 1.0f ) {
		volume->SetStrength( volume->GetStrength() * visibleDamageStrengthModifier );
	}
	for ( unsigned int i = 0; i < visibleDamage.GetSize(); i++ ) {
		if ( visibleDamage.GetAt( i ) != NULL ) {
			visibleDamage.GetAt( i )->AddVolume( volume );
		}
	}
	unknown13C->AddVolume( volume );
	Unknown593F30( 1 );
}

/*
================
DamageableObject::Unknown593F30

With damage geometry on outside multiplayer, the object has a time-slice
process of the priority that runs its refresh function; one of another
priority is replaced (invented name).
================
*/
// FUNCTION: STBC 0x00593f30
void DamageableObject::Unknown593F30( int priority ) {
	if ( unknown1C4 != NULL && unknown1C4->GetPriority() != priority ) {
		delete unknown1C4;
		unknown1C4 = NULL;
	}
	if ( unknown1C4 == NULL && damageGeometryEnabled && !g_kUtopiaModule.IsMultiplayer() ) {
		unknown1C4 = new STTargetMenuProcess( Unknown5927E0, this );
		unknown1C4->SetDelayUsesGameTime( false );
		unknown1C4->SetPriority( priority );
	}
}

/*
================
DamageableObject::AddDamageVolume

Adds a damage volume at a point of the object's node, carried into the
frame of the model the volumes are expressed in.
================
*/
// FUNCTION: STBC 0x00594020
void DamageableObject::AddDamageVolume( const NiPoint3 &center, float radius, float strength ) {
	NiNode *node = GetNode();
	if ( node != NULL && unknown140 != NULL ) {
		AddMetaVolume( new MetaVolume(
			TGModelUtils::LocalToLocalPoint( node, unknown140, center ),
			( 1.0f / unknown140->GetWorldScale() ) * radius, strength ) );
	}
}

/*
================
DamageableObject::AddObjectDamageVolume
================
*/
// FUNCTION: STBC 0x005941b0
void DamageableObject::AddObjectDamageVolume( float x, float y, float z, float radius, float strength ) {
	NiPoint3 center( x, y, z );
	AddDamageVolume( center, radius, strength );
}

/*
================
DamageableObject::EnableGlowAlphaMaps

Gives the shown model's LOD levels, from the dropped level on, their glow
alpha maps back: each child of the model's LOD node with its level's glow
suffix.
================
*/
// FUNCTION: STBC 0x005941f0
void DamageableObject::EnableGlowAlphaMaps( void ) {
	if ( unknown108 == NULL ) {
		return;
	}
	NiAVObject *model = clonedModel != NULL ? (NiAVObject *)clonedModel : (NiAVObject *)niObject;
	LODModel *lodModel = g_kLODModelManager.GetModel( unknown108 );
	if ( lodModel == NULL ) {
		return;
	}
	int level = g_kLODModelManager.GetDropLODLevel();
	int count = lodModel->GetNumLevels();
	if ( level >= count ) {
		level = count - 1;
	}
	if ( model == NULL ) {
		return;
	}
	NiLODNode *node = model->IsKindOf( &NiLODNode::rtti ) ? (NiLODNode *)model : NULL;
	for ( unsigned int i = level; i < lodModel->GetNumLevels(); i++ ) {
		LODModelPart *part = lodModel->GetLevel( i );
		NiAVObject *child = node->GetAt( i );
		if ( part->glowSuffix != NULL ) {
			::EnableGlowAlphaMaps( child, part->glowSuffix );
			child->UpdateProperties();
		}
	}
}

/*
================
DamageableObject::DisableGlowAlphaMaps

Drops the glow alpha maps of the model and of the cloned model.
================
*/
// FUNCTION: STBC 0x005942d0
void DamageableObject::DisableGlowAlphaMaps( void ) {
	if ( niObject != NULL ) {
		::DisableGlowAlphaMaps( niObject );
		niObject->UpdateProperties();
	}
	if ( clonedModel != NULL ) {
		::DisableGlowAlphaMaps( clonedModel );
		clonedModel->UpdateProperties();
	}
}

/*
================
DamageableObject::VirtualSlot80

Tests a hit along the segment from start to end, nearest triangle first.
Gives the point and the face normal of the hit; false when the ray misses
or hits beyond the end.
================
*/
// FUNCTION: STBC 0x00594310
bool DamageableObject::VirtualSlot80( const NiPoint3 *start, const NiPoint3 *end, NiPoint3 *hitPoint, NiPoint3 *hitNormal, bool ) {
	NiPick pick;
	pick.SetSortType( NiPick::SORT );
	pick.SetFrontOnly( false );
	pick.SetReturnSmoothNormal( false );
	pick.SetPickType( NiPick::FIND_FIRST );
	pick.SetIntersectType( NiPick::TRIANGLE_INTERSECT );
	pick.SetCoordinateType( NiPick::WORLD_COORDINATES );
	pick.SetReturnNormal( true );

	NiPoint3 direction = *end - *start;
	if ( VirtualSlot63( start, direction, &pick ) ) {
		NiPick::Record *hit = pick.GetRecord( 0 );
		if ( hit->distance < 1.0f ) {
			*hitPoint = hit->intersection;
			*hitNormal = hit->normal;
			return true;
		}
	}
	return false;
}

/*
================
DamageableObject::VirtualSlot81

An object that is not a ship takes no damage from the hit: the amount
left over (the fifth argument) is cleared, and the hit counts as handled.
ShipClass overrides it.
================
*/
// FUNCTION: STBC 0x00594430
bool DamageableObject::VirtualSlot81( const NiPoint3 *, const NiPoint3 *, NiPoint3 *, NiPoint3 *, float *damage, WeaponPayload *, TGObject *, ObjectClass * ) {
	*damage = 0.0f;
	return true;
}

/*
================
DamageableObject::VirtualSlot82

Hands the other object this object's side of the collision test.
================
*/
// FUNCTION: STBC 0x00594440
bool DamageableObject::VirtualSlot82( DamageableObject *other, CollisionRecorder *recorder ) {
	OBBModel *model = GetFirstCollisionModel();
	if ( model == NULL ) {
		return false;
	}

	DamageCollisionTest test( this, model, GetFirstSetUpDamage(), unknown1B4 );
	return other->VirtualSlot83( &test, recorder );
}

/*
================
DamageableObject::GetFirstCollisionModel
================
*/
// FUNCTION: STBC 0x00594540
OBBModel *DamageableObject::GetFirstCollisionModel( void ) {
	OBBModel *model = NULL;
	for ( unsigned int i = 0; i < unknown10C.GetSize(); i++ ) {
		model = unknown10C.GetAt( i );
		if ( model != NULL ) {
			break;
		}
	}
	return model;
}

/*
================
DamageableObject::VirtualSlot83

Tests this object's side of the collision test against the other's.
================
*/
// FUNCTION: STBC 0x005945b0
bool DamageableObject::VirtualSlot83( CollisionTest *other, CollisionRecorder *recorder ) {
	OBBModel *model = GetFirstCollisionModel();
	if ( model == NULL ) {
		return false;
	}

	DamageCollisionTest test( this, model, GetFirstSetUpDamage(), unknown1B4 );
	// MATCH-PENDING: R17, retail calls slot 0 through the side's known vtable (call [0x893F04]) rather than directly; the pointer reproduces that call, the original's spelling of it is not known
	CollisionTest *side = &test;
	return side->VirtualSlot00( other, recorder );
}

/*
================
DamageableObject::CanCollide

Both objects must have collisions on, and the other must not be one this
object ignores.
================
*/
// FUNCTION: STBC 0x005946a0
bool DamageableObject::CanCollide( DamageableObject *object ) {
	if ( object == NULL || !collisionsOn || !object->collisionsOn ) {
		return false;
	}

	long id = object->GetObjID();
	NiTListIterator pos = unknown190.GetHeadPos();
	while ( pos ) {
		if ( unknown190.GetNext( pos ) == id ) {
			return false;
		}
	}
	return true;
}

/*
================
DamageableObject::EnableCollisionsWith

Lets this object collide with the other, or not; the other object is
then made to agree.
================
*/
// FUNCTION: STBC 0x005946f0
void DamageableObject::EnableCollisionsWith( DamageableObject *object, bool enable ) {
	if ( object == NULL ) {
		return;
	}

	long id = object->GetObjID();
	if ( enable ) {
		unknown190.Remove( id );
	} else if ( unknown190.FindPos( id ) == NULL ) {
		unknown190.AddTail( id );
	}

	if ( enable != object->CanCollide( this ) ) {
		object->EnableCollisionsWith( this, enable );
	}
}

/*
================
DamageableObject::SetCollisionsOn
================
*/
// FUNCTION: STBC 0x00594830
void DamageableObject::SetCollisionsOn( bool on ) {
	collisionsOn = on;
}

/*
================
DamageableObject::VirtualSlot84

Takes the collision the recorder holds with the other object, once per
contact: the object pushes back at each point of contact, by the
difference of the two objects' velocities there when the two were not
touching before, else straight away from the other object, by how deep
it reaches into this one; this object's share of the push grows as its
mass falls against the other's. An ET_OBJECT_COLLISION event carries the
force and the point of contact (the two farthest apart, for several).
================
*/
// FUNCTION: STBC 0x00594840
// DRAFT: inline budget: retail calls NiPoint3's copy constructor (0x423480) out of line for the total force and the single point (both nested somewhere: later copies expand) and expands the farthest pair's two operator= stores; the call set and block order agree otherwise
void DamageableObject::VirtualSlot84( CollisionRecorder *recorder, DamageableObject *other, int index ) {
	NiTList<NiPoint3 *> points( 2 );
	if ( !recorder->GetPoints( &points, 2, this, other ) ) {
		return;
	}
	if ( unknown178.FindPos( other->GetObjID() ) != 0 ) {
		return;
	}
	unknown178.AddTail( other->GetObjID() );

	CollisionState state = recorder->GetState( index );
	CollisionState otherState = recorder->GetState( !index );
	float share = ( 1.0f - state.mass / ( otherState.mass + state.mass ) ) * 2.0f;

	Force total;
	if ( unknown160.FindPos( other->GetObjID() ) != 0 ) {
		if ( share > 0.001 ) {
			NiPoint3 direction = GetWorldLocation() - other->GetWorldLocation();
			float distance = direction.Unitize();
			if ( distance == 0.0f ) {
				direction = NiPoint3::UNIT_X;
				distance = 0.01f;
			}
			float step = UtopiaApp::GetApp()->GetGameFrameStep();
			float scale = ( GetRadius() + other->GetRadius() ) * step / distance * 0.25f;
			direction *= scale;
			direction *= share / (float)points.GetSize();

			NiTListIterator pos = points.GetHeadPos();
			while ( pos ) {
				NiPoint3 *point = points.GetNext( pos );
				Force force( *point, direction * GetMass(), NiPoint3::ZERO );
				total += force;
			}
		}
	} else {
		NiTListIterator pos = points.GetHeadPos();
		while ( pos ) {
			NiPoint3 *point = points.GetNext( pos );
			NiPoint3 offset = *point - GetWorldLocation();
			NiPoint3 velocity = offset.Cross( state.angularVelocity ) + state.velocity;
			NiPoint3 otherOffset = *point - GetWorldLocation();
			NiPoint3 otherVelocity = otherOffset.Cross( otherState.angularVelocity ) + otherState.velocity;
			NiPoint3 relative = otherVelocity - velocity;

			Force force;
			force.SetPosition( *point );
			float mass = GetMass();
			force.SetForce( relative * share * mass / (float)points.GetSize() );
			total += force;
		}
	}

	NiPoint3 totalForce = total.GetForce();
	float magnitude = totalForce.Length();
	CollisionEvent *event = new CollisionEvent;
	event->SetEventType( ET_OBJECT_COLLISION );
	event->SetSource( other );
	event->SetDestination( this );
	event->SetCollisionForce( magnitude );
	if ( points.GetSize() == 1 ) {
		event->AddPoint( new NiPoint3( *points.GetHead() ) );
	} else {
		// the two points farthest apart
		NiTListIterator pos = points.GetHeadPos();
		NiPoint3 first = *points.GetNext( pos );
		NiPoint3 second = *points.GetNext( pos );
		float farthest = ( first - second ).SqrLength();
		NiTListIterator outer = points.GetHeadPos();
		while ( outer ) {
			NiPoint3 a = *points.GetNext( outer );
			NiTListIterator inner = outer;
			while ( inner ) {
				NiPoint3 b = *points.GetNext( inner );
				float distance = ( b - a ).SqrLength();
				if ( distance > farthest ) {
					first = a;
					second = b;
					farthest = distance;
				}
			}
		}
		event->AddPoint( new NiPoint3( first ) );
		event->AddPoint( new NiPoint3( second ) );
	}
	g_kEventManager.AddEvent( event );
	ApplyForce( &total );
}

/*
================
DamageableObject::CollisionEffectHandler
================
*/
// FUNCTION: STBC 0x005952b0
void DamageableObject::CollisionEffectHandler( TGEvent *event ) {
	Unknown5952D0( event );
	CallNextHandler( event );
}

/*
================
DamageableObject::Unknown5952D0

Damages the object at each point of contact of the collision, with a
radius from the collision force per unit of mass, shared among the points,
of at most 0.5.
================
*/
// FUNCTION: STBC 0x005952d0
// PARKED: x87 schedule: retail pops the mass right after the first divide and compares the radius still on the stack (fst; fcomp), this build pops the mass after storing the radius and reloads it
void DamageableObject::Unknown5952D0( TGEvent *event ) {
	CollisionEvent *collision = (CollisionEvent *)event;
	float mass = GetMass();
	float radius = collision->GetCollisionForce() / mass / collision->GetNumPoints() * COLLISION_DAMAGE_RADIUS_SCALE + 0.1f;
	if ( radius > 0.5f ) {
		radius = 0.5f;
	}

	for ( int i = 0; i < collision->GetNumPoints(); i++ ) {
		NiPoint3 center = TGModelUtils::WorldToLocalPoint( GetNode(), collision->GetPoint( i ) );
		AddDamageVolume( center, radius, 6000.0f );
	}
}

/*
================
DamageableObject::Unknown595440
================
*/
// FUNCTION: STBC 0x00595440
bool DamageableObject::Unknown595440( const NiPoint3 *point ) {
	VisibleDamage *damage = GetFirstSetUpDamage();
	if ( damage != NULL ) {
		NiNode *node = GetNode();
		NiPoint3 worldPoint = node->GetWorldRotate() * ( node->GetWorldScale() * *point ) + node->GetWorldTranslate();
		return damage->Unknown4B1940( &worldPoint, true );
	}
	return false;
}

/*
================
DamageableObject::CreateWeaponHitEvent

A WeaponHitEvent from the source (the weapon or torpedo) to this object.
In a multiplayer game the host records the firing player: a torpedo's
own, else the network ID of the weapon's ship or of the firing ship.
================
*/
// FUNCTION: STBC 0x00595500
WeaponHitEvent *DamageableObject::CreateWeaponHitEvent( const NiPoint3 *point, const NiPoint3 *normal, float damage, float condition, WeaponPayload *payload, TGObject *source, ObjectClass *firingObject ) {
	WeaponHitEvent *event = new WeaponHitEvent;
	event->SetSource( source );
	event->SetDestination( this );
	event->SetFiringObject( firingObject );
	event->SetCondition( condition );
	event->SetDamage( damage );
	event->SetObjectHitPoint( *point );
	event->SetObjectHitNormal( *normal );
	event->SetWeaponInstanceID( payload->GetID() );
	event->SetRadius( payload->GetDamageRadiusFactor() );
	if ( g_kUtopiaModule.IsMultiplayer() && g_kUtopiaModule.IsHost() ) {
		if ( source->IsTypeOf( TORPEDO_TYPE ) ) {
			event->SetFiringPlayerID( Torpedo::Cast( source )->GetPlayerID() );
			return event;
		}
		if ( source->IsTypeOf( WEAPON_TYPE ) || source->IsTypeOf( SHIPCLASS_TYPE ) ) {
			ShipClass *ship;
			if ( source->IsTypeOf( WEAPON_TYPE ) ) {
				ship = Weapon::Cast( source )->GetParentShip();
			} else {
				ship = ShipClass::Cast( firingObject );
			}
			if ( ship != NULL ) {
				event->SetFiringPlayerID( ship->GetNetPlayerID() );
				return event;
			}
			event->SetFiringPlayerID( 0 );
		}
	}
	return event;
}

/*
================
DamageableObject::WriteToStream

PhysicsObjectClass's state, then the model name, the property set (after
whether there is one), the life and damage values, the two ID lists (each
its length, then the IDs), the timer and the damage volumes, each after a
true; a false ends them.
================
*/
// FUNCTION: STBC 0x00595650
bool DamageableObject::WriteToStream( TGStream *stream ) {
	if ( !PhysicsObjectClass::WriteToStream( stream ) ) {
		return false;
	}

	stream->VirtualSlot11( unknown108 );
	if ( propertySet != NULL ) {
		stream->WriteBool( true );
		propertySet->WriteToStream( stream );
	} else {
		stream->WriteBool( false );
	}

	stream->WriteFloat( lifeTime );
	stream->WriteBool( dead );
	stream->WriteFloat( splashDamage );
	stream->WriteFloat( splashDamageRadius );
	stream->WriteFloat( visibleDamageRadiusModifier );
	stream->WriteFloat( visibleDamageStrengthModifier );
	stream->WriteFloat( specularKs );

	stream->WriteInt( unknown178.GetSize() );
	NiTListIterator pos = unknown178.GetHeadPos();
	while ( pos ) {
		stream->WriteID( unknown178.GetNext( pos ) );
	}
	stream->WriteInt( unknown190.GetSize() );
	pos = unknown190.GetHeadPos();
	while ( pos ) {
		stream->WriteID( unknown190.GetNext( pos ) );
	}

	stream->WriteBool( collisionsOn );
	stream->WriteID( unknown1AC );
	stream->WriteInt( unknown1B0 );
	stream->WriteInt( unknown1B4 );
	stream->WriteInt( clonedModelCount );

	NiTArray< NiPointer<MetaVolume> > &volumes = unknown13C->objects;
	int count = volumes.GetSize();
	for ( int i = 0; i < count; i++ ) {
		MetaVolume *volume = volumes.GetAt( i );
		if ( volume != NULL ) {
			stream->WriteBool( true );
			NiPoint3 center = volume->GetCenter();
			stream->WriteFloat( center.x );
			stream->WriteFloat( center.y );
			stream->WriteFloat( center.z );
			stream->WriteFloat( volume->GetRadius() );
			stream->WriteFloat( volume->GetStrength() );
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
DamageableObject::ReadFromStream

Reads what WriteToStream wrote, sets up the model under the streamed name
and adds the damage volumes; the clone count starts again from zero.
================
*/
// FUNCTION: STBC 0x00595890
bool DamageableObject::ReadFromStream( TGStream *stream ) {
	if ( !PhysicsObjectClass::ReadFromStream( stream ) ) {
		return false;
	}

	char *modelName = stream->VirtualSlot10( -1 );
	if ( stream->ReadBool() == true ) {
		propertySet = (TGModelPropertySet *)TGObject::LoadObject( stream, 0 );
	} else {
		propertySet = NULL;
	}

	lifeTime = stream->ReadFloat();
	dead = stream->ReadBool();
	splashDamage = stream->ReadFloat();
	splashDamageRadius = stream->ReadFloat();
	visibleDamageRadiusModifier = stream->ReadFloat();
	visibleDamageStrengthModifier = stream->ReadFloat();
	specularKs = stream->ReadFloat();

	int i;
	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		unknown178.AddTail( stream->ReadID() );
	}
	for ( i = stream->ReadInt() - 1; i >= 0; i-- ) {
		unknown190.AddTail( stream->ReadID() );
	}

	collisionsOn = stream->ReadBool();
	unknown1AC = stream->ReadID();
	unknown1B0 = stream->ReadInt();
	unknown1B4 = stream->ReadInt();
	clonedModelCount = stream->ReadInt();

	SetupModel( modelName );
	SpecularPass::SetSpecularKs( GetNiObject(), specularKs );
	delete modelName;

	while ( stream->ReadBool() == true ) {
		NiPoint3 center;
		center.x = stream->ReadFloat();
		center.y = stream->ReadFloat();
		center.z = stream->ReadFloat();
		float radius = stream->ReadFloat();
		float strength = stream->ReadFloat();
		AddMetaVolume( new MetaVolume( center, radius, strength ) );
	}

	clonedModelCount = 0;
	return true;
}

/*
================
DamageableObject::SendDamageVolumes
================
*/
// FUNCTION: STBC 0x00595c60
void DamageableObject::SendDamageVolumes( int playerID ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}

	NiTArray< NiPointer<MetaVolume> > &volumes = unknown13C->objects;
	int count = volumes.GetSize();
	TGBufferStream stream;
	for ( int i = 0; i < count; i++ ) {
		MetaVolume *volume = volumes.GetAt( i );
		if ( volume != NULL ) {
			TGMessage *message = new TGMessage;
			message->SetGuaranteed( true );
			message->SetAggregate( false );

			char buffer[MAX_MESSAGE_SIZE];
			stream.VirtualSlot60( buffer, sizeof( buffer ) );
			stream.WriteChar( DAMAGE_VOLUME_MESSAGE );
			stream.WriteID( GetObjID() );
			NiPoint3 center = volume->GetCenter();
			stream.VirtualSlot36( center.x, center.y, center.z, true );
			float radius = volume->GetRadius();
			unsigned short packedRadius = Unknown6D3A90( radius );
			float strength = volume->GetStrength();
			unsigned short packedStrength = Unknown6D3A90( strength );
			stream.WriteShort( packedRadius );
			stream.WriteShort( packedStrength );
			message->SetData( buffer, stream.GetPos() );
			stream.Close();
			network->SendTGMessage( playerID, message, 0 );
		}
	}
}

/*
================
DamageableObject::VirtualSlot87

Nothing for an object that is not a ship; ShipClass overrides it.
================
*/
// FUNCTION: STBC 0x00595e40
void DamageableObject::VirtualSlot87( void ) {
}

