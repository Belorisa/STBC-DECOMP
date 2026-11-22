/*
===============================================================================

	AsteroidTile.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AsteroidTile.h"
#include "AsteroidField.h"
#include "DamageableObject.h"
#include "LODModel.h"
#include "LODModelManager.h"
#include "PlacementObject.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGPoint3.h"
#include "TGStream.h"
#include "TGStringEvent.h"
#include "TGSystemWrapperClass.h"
#include "UtopiaModule.h"
#include "TGPoolManager.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0041d6c0 _$E486
// FUNCTION: STBC 0x0041d6e0 _$E485

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0041bd60 _$E32
// FUNCTION: STBC 0x0041bd70 _$E29
// FUNCTION: STBC 0x0041bd80 _$E31
// FUNCTION: STBC 0x0041bd90 _$E30
// FUNCTION: STBC 0x0041bda0 _$E38
// FUNCTION: STBC 0x0041bdb0 _$E35
// FUNCTION: STBC 0x0041bdc0 _$E37
// FUNCTION: STBC 0x0041bdd0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0041c140 _$E66
// FUNCTION: STBC 0x0041c150 _$E65
// FUNCTION: STBC 0x0041c250 _$E287
// FUNCTION: STBC 0x0041c260 _$E286
// FUNCTION: STBC 0x0041ce40 _$E77
// FUNCTION: STBC 0x0041ce50 _$E76
// FUNCTION: STBC 0x0041ce60 _$E81
// FUNCTION: STBC 0x0041ce70 _$E80
// FUNCTION: STBC 0x0041ce80 _$E85
// FUNCTION: STBC 0x0041ce90 _$E84
// FUNCTION: STBC 0x0041cea0 _$E89
// FUNCTION: STBC 0x0041ceb0 _$E88
// FUNCTION: STBC 0x0041cec0 _$E206
// FUNCTION: STBC 0x0041ced0 _$E205
// FUNCTION: STBC 0x0041cee0 _$E148
// FUNCTION: STBC 0x0041cef0 _$E147
// FUNCTION: STBC 0x0041cf00 _$E161
// FUNCTION: STBC 0x0041cf10 _$E160
// FUNCTION: STBC 0x0041cf20 _$E211
// FUNCTION: STBC 0x0041cf30 _$E210
// FUNCTION: STBC 0x0041cf40 _$E292
// FUNCTION: STBC 0x0041cf50 _$E291
// FUNCTION: STBC 0x0041cf60 _$E215
// FUNCTION: STBC 0x0041cf70 _$E214
// FUNCTION: STBC 0x0041cf80 _$E296
// FUNCTION: STBC 0x0041cf90 _$E295
// FUNCTION: STBC 0x0041cfa0 _$E303
// FUNCTION: STBC 0x0041cfb0 _$E302
// FUNCTION: STBC 0x0041cfc0 _$E321
// FUNCTION: STBC 0x0041cfd0 _$E320
// FUNCTION: STBC 0x0041cfe0 _$E325
// FUNCTION: STBC 0x0041cff0 _$E324
// FUNCTION: STBC 0x0041d000 _$E106
// FUNCTION: STBC 0x0041d010 _$E105
// FUNCTION: STBC 0x0041d020 _$E237
// FUNCTION: STBC 0x0041d030 _$E236
// FUNCTION: STBC 0x0041d040 _$E244
// FUNCTION: STBC 0x0041d050 _$E243
// FUNCTION: STBC 0x0041d060 _$E248
// FUNCTION: STBC 0x0041d070 _$E247
// FUNCTION: STBC 0x0041d080 _$E252
// FUNCTION: STBC 0x0041d090 _$E251
// FUNCTION: STBC 0x0041d0a0 _$E256
// FUNCTION: STBC 0x0041d0b0 _$E255
// FUNCTION: STBC 0x0041d0c0 _$E262
// FUNCTION: STBC 0x0041d0d0 _$E261
// FUNCTION: STBC 0x0041d0e0 _$E329
// FUNCTION: STBC 0x0041d0f0 _$E328
// FUNCTION: STBC 0x0041d100 _$E333
// FUNCTION: STBC 0x0041d110 _$E332
// FUNCTION: STBC 0x0041d120 _$E337
// FUNCTION: STBC 0x0041d130 _$E336
// FUNCTION: STBC 0x0041d140 _$E341
// FUNCTION: STBC 0x0041d150 _$E340
// FUNCTION: STBC 0x0041d160 _$E345
// FUNCTION: STBC 0x0041d170 _$E344
// FUNCTION: STBC 0x0041d180 _$E349
// FUNCTION: STBC 0x0041d190 _$E348
// FUNCTION: STBC 0x0041d1a0 _$E353
// FUNCTION: STBC 0x0041d1b0 _$E352
// FUNCTION: STBC 0x0041d1c0 _$E357
// FUNCTION: STBC 0x0041d1d0 _$E356
// FUNCTION: STBC 0x0041d1e0 _$E361
// FUNCTION: STBC 0x0041d1f0 _$E360
// FUNCTION: STBC 0x0041d200 _$E365
// FUNCTION: STBC 0x0041d210 _$E364
// FUNCTION: STBC 0x0041d220 _$E111
// FUNCTION: STBC 0x0041d230 _$E110
// FUNCTION: STBC 0x0041d240 _$E369
// FUNCTION: STBC 0x0041d250 _$E368
// FUNCTION: STBC 0x0041d260 _$E116
// FUNCTION: STBC 0x0041d270 _$E115
// FUNCTION: STBC 0x0041d280 _$E373
// FUNCTION: STBC 0x0041d290 _$E372
// FUNCTION: STBC 0x0041d2a0 _$E377
// FUNCTION: STBC 0x0041d2b0 _$E376
// FUNCTION: STBC 0x0041d2c0 _$E381
// FUNCTION: STBC 0x0041d2d0 _$E380
// FUNCTION: STBC 0x0041d2e0 _$E385
// FUNCTION: STBC 0x0041d2f0 _$E384
// FUNCTION: STBC 0x0041d300 _$E125
// FUNCTION: STBC 0x0041d310 _$E124
// FUNCTION: STBC 0x0041d320 _$E389
// FUNCTION: STBC 0x0041d330 _$E388
// FUNCTION: STBC 0x0041d340 _$E393
// FUNCTION: STBC 0x0041d350 _$E392
// FUNCTION: STBC 0x0041d360 _$E397
// FUNCTION: STBC 0x0041d370 _$E396
// FUNCTION: STBC 0x0041d380 _$E131
// FUNCTION: STBC 0x0041d390 _$E130
// FUNCTION: STBC 0x0041d3a0 _$E136
// FUNCTION: STBC 0x0041d3b0 _$E135
// FUNCTION: STBC 0x0041d3c0 _$E166
// FUNCTION: STBC 0x0041d3d0 _$E165
// FUNCTION: STBC 0x0041d3e0 _$E401
// FUNCTION: STBC 0x0041d3f0 _$E400
// FUNCTION: STBC 0x0041d400 _$E171
// FUNCTION: STBC 0x0041d410 _$E170
// FUNCTION: STBC 0x0041d420 _$E405
// FUNCTION: STBC 0x0041d430 _$E404
// FUNCTION: STBC 0x0041d440 _$E409
// FUNCTION: STBC 0x0041d450 _$E408
// FUNCTION: STBC 0x0041d460 _$E176
// FUNCTION: STBC 0x0041d470 _$E175
// FUNCTION: STBC 0x0041d480 _$E413
// FUNCTION: STBC 0x0041d490 _$E412
// FUNCTION: STBC 0x0041d4a0 _$E141
// FUNCTION: STBC 0x0041d4b0 _$E140
// FUNCTION: STBC 0x0041d4c0 _$E417
// FUNCTION: STBC 0x0041d4d0 _$E416
// FUNCTION: STBC 0x0041d4e0 _$E421
// FUNCTION: STBC 0x0041d4f0 _$E420
// FUNCTION: STBC 0x0041d500 _$E181
// FUNCTION: STBC 0x0041d510 _$E180
// FUNCTION: STBC 0x0041d520 _$E186
// FUNCTION: STBC 0x0041d530 _$E185
// FUNCTION: STBC 0x0041d540 _$E425
// FUNCTION: STBC 0x0041d550 _$E424
// FUNCTION: STBC 0x0041d560 _$E429
// FUNCTION: STBC 0x0041d570 _$E428
// FUNCTION: STBC 0x0041d580 _$E433
// FUNCTION: STBC 0x0041d590 _$E432
// FUNCTION: STBC 0x0041d5a0 _$E437
// FUNCTION: STBC 0x0041d5b0 _$E436
// FUNCTION: STBC 0x0041d5c0 _$E441
// FUNCTION: STBC 0x0041d5d0 _$E440
// FUNCTION: STBC 0x0041d5e0 _$E445
// FUNCTION: STBC 0x0041d5f0 _$E444
// FUNCTION: STBC 0x0041d600 _$E449
// FUNCTION: STBC 0x0041d610 _$E448
// FUNCTION: STBC 0x0041d620 _$E453
// FUNCTION: STBC 0x0041d630 _$E452
// FUNCTION: STBC 0x0041d640 _$E191
// FUNCTION: STBC 0x0041d650 _$E190
// FUNCTION: STBC 0x0041d660 _$E457
// FUNCTION: STBC 0x0041d670 _$E456
// FUNCTION: STBC 0x0041d680 _$E196
// FUNCTION: STBC 0x0041d690 _$E195
// FUNCTION: STBC 0x0041d6a0 _$E201
// FUNCTION: STBC 0x0041d6b0 _$E200
// FUNCTION: STBC 0x0041d6f0 _$E461
// FUNCTION: STBC 0x0041d700 _$E460
// FUNCTION: STBC 0x0041d710 _$E466
// FUNCTION: STBC 0x0041d720 _$E465
// FUNCTION: STBC 0x0041d730 _$E470
// FUNCTION: STBC 0x0041d740 _$E469

// A full turn, the App constant TWO_PI.
#define TWO_PI		6.2831855f

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x0041be00 _$E476
// FUNCTION: STBC 0x0041be10 _$E472
// FUNCTION: STBC 0x0041be20 _$E475
// FUNCTION: STBC 0x0041be30 _$E473
// DATA: STBC 0x0097ece8
TGEventHandlerTable AsteroidTile::classHandlers;

// DATA: STBC 0x0097ed70
int AsteroidTile::asteroidNumber;

/*
================
AsteroidTile::AddToSet

Adds the tile to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x0041be50
void AsteroidTile::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, ASTEROIDTILE_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
AsteroidTile::RemoveFromSet
================
*/
// FUNCTION: STBC 0x0041be80
void AsteroidTile::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, ASTEROIDTILE_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
AsteroidTile::GetClassObjects
================
*/
// FUNCTION: STBC 0x0041beb0
AsteroidTile **AsteroidTile::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (AsteroidTile **)set->objectTable.GetObjects( ASTEROIDTILE_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
AsteroidTile::GetNumClassObjects
================
*/
// FUNCTION: STBC 0x0041bee0
int AsteroidTile::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( ASTEROIDTILE_TYPE );
	}
	return 0;
}

/*
================
AsteroidTile::GetObjectByID
================
*/
// FUNCTION: STBC 0x0041bf00
AsteroidTile *AsteroidTile::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( ASTEROIDTILE_TYPE ) ) {
			return (AsteroidTile *)object;
		}
		return NULL;
	}
	return (AsteroidTile *)set->GetClassObject( ASTEROIDTILE_TYPE, id );
}

/*
================
AsteroidTile::GetObject

The tile of the name in the set, or in the last set that has one when the
set is NULL.
================
*/
// FUNCTION: STBC 0x0041bf70 ?GetObject@AsteroidTile@@SAPAV1@PAVSetClass@@PAD@Z
AsteroidTile *AsteroidTile::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( ASTEROIDTILE_TYPE, name_ );
		}
		return (AsteroidTile *)object;
	}
	return (AsteroidTile *)set->GetClassObject( ASTEROIDTILE_TYPE, name_ );
}

/*
================
AsteroidTile::AsteroidTile

The loader's constructor: the stream fills the members.
================
*/
// FUNCTION: STBC 0x0041bfd0 ??0AsteroidTile@@QAE@J@Z
AsteroidTile::AsteroidTile( long id ) : ObjectClass( id ) {
	SetNiObject( new NiNode );
	asteroids = NULL;
	field = NULL;
	tileSize = 0.0f;
	numAsteroids = 0;
	asteroidsPerTile = 3;
	asteroidSizeFactor = 10.0f;
	randomSeed = 0x10ABCDEF;
	SetCollisionFlags( 0 );
	SetHailable( false );
	SetScannable( false );
}

/*
================
AsteroidTile::GetObjType
================
*/
// FUNCTION: STBC 0x0041c090
int AsteroidTile::GetObjType( void ) {
	return ASTEROIDTILE_TYPE;
}

/*
================
AsteroidTile::IsTypeOf
================
*/
// FUNCTION: STBC 0x0041c0a0
bool AsteroidTile::IsTypeOf( int type ) {
	if ( type == ASTEROIDTILE_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

/*
================
AsteroidTile::GetClassName
================
*/
// FUNCTION: STBC 0x0041c100
char *AsteroidTile::GetClassName( void ) {
	return "AsteroidTile";
}

/*
================
AsteroidTile::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0041c110
char *AsteroidTile::GetSwigTypeName( void ) {
	return "_p_AsteroidTile";
}

/*
================
AsteroidTile::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0041c120
char *AsteroidTile::GetPythonClassName( void ) {
	return "AsteroidTilePtr";
}

/*
================
AsteroidTile::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0041c130
TGEventHandlerTable *AsteroidTile::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
AsteroidTile::AsteroidTile

A new tile of the field: the asteroids it makes and their size factor.
================
*/
// FUNCTION: STBC 0x0041c190 ??0AsteroidTile@@QAE@HMJ@Z
AsteroidTile::AsteroidTile( int asteroidsPerTile_, float asteroidSizeFactor_, long id ) : ObjectClass( id ) {
	SetNiObject( new NiNode );
	asteroidsPerTile = asteroidsPerTile_;
	asteroids = NULL;
	field = NULL;
	tileSize = 0.0f;
	numAsteroids = 0;
	asteroidSizeFactor = asteroidSizeFactor_;
	randomSeed = 0x10ABCDEF;
	SetCollisionFlags( 0 );
	SetHailable( false );
	SetScannable( false );
}

/*
================
AsteroidTile::~AsteroidTile
================
*/
// FUNCTION: STBC 0x0041c270
AsteroidTile::~AsteroidTile( void ) {
}

/*
================
AsteroidTile::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0041c280
void AsteroidTile::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&AsteroidTile::HandleEnterSet, "AsteroidTile::HandleEnterSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&AsteroidTile::HandleExitSet, "AsteroidTile::HandleExitSet" );
}

/*
================
AsteroidTile::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0041c2b0
void AsteroidTile::RegisterHandlers( void ) {
	TGEventHandlerObject::AddMethodHandlerForClass( &classHandlers, ET_ENTERED_SET, "AsteroidTile::HandleEnterSet" );
	TGEventHandlerObject::AddMethodHandlerForClass( &classHandlers, ET_EXITED_SET, "AsteroidTile::HandleExitSet" );
}

/*
================
AsteroidTile::IsAsteroidPlaceFree
================
*/
// FUNCTION: STBC 0x0041c2e0
// DRAFT: the placement-frame test: retail keeps the WorldToLocalPoint helper's NiPoint3 constructor (0x414540) and its * ( 1 / scale ) (0x41CBD0) out of line (inline budget); ours expands them; the location sits in ebx, ours esi
bool AsteroidTile::IsAsteroidPlaceFree( NiPoint3 *location, float radius ) {
	SetClass *set = containingSet;
	if ( !set->IsLocationEmpty( location, radius, true ) ) {
		return false;
	}
	int count = 0;
	PlacementObject **placements = PlacementObject::GetClassObjects( set, &count );
	float diameter = radius + radius;
	float diameterSquared = diameter * diameter;
	for ( int i = 0; i < count; i++ ) {
		if ( placements[i]->GetObjType() == 0x80DE || placements[i]->GetObjType() == 0x80E0 ) {
			NiPoint3 placementLocation = placements[i]->GetWorldLocation();
			NiPoint3 offset = *location - placementLocation;
			if ( offset.SqrLength() < diameterSquared ) {
				return false;
			}
			// The place in the placement's own frame: nothing up to 1600
			// behind it.
			NiPoint3 local = TGModelUtils::WorldToLocalPoint( placements[i]->GetNode(), *location );
			if ( local.y <= 0.0f && local.y >= -1600.0f && 1600.0f * 1600.0f < diameterSquared ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
AsteroidTile::HandleEnterSet

Makes the tile's asteroids, the same each time from the tile's seed, and
puts them in the set, each turned at random.
================
*/
// FUNCTION: STBC 0x0041c470
void AsteroidTile::HandleEnterSet( TGEvent *event ) {
	if ( asteroids == NULL ) {
		asteroids = new ObjectClass *[asteroidsPerTile];
		numAsteroids = asteroidsPerTile;
		srand( randomSeed );
		UpdateNodeOnly();
		for ( int i = 0; i < numAsteroids; i++ ) {
			ObjectClass *asteroid = CreateAsteroid();
			char name[32];
			sprintf( name, "Asteroid (Field Object) %d", asteroidNumber++ );
			containingSet->AddObjectToSet( asteroid, name );
			NiMatrix3 xRotation;
			xRotation.MakeXRotation( NiSymmetricRandom() * TWO_PI );
			NiMatrix3 yRotation;
			yRotation.MakeYRotation( NiSymmetricRandom() * TWO_PI );
			NiMatrix3 zRotation;
			zRotation.MakeZRotation( NiSymmetricRandom() * TWO_PI );
			asteroid->Rotate( xRotation * yRotation * zRotation );
			asteroid->UpdateNodeOnly();
			asteroids[i] = asteroid;
		}
		if ( !g_kUtopiaModule.IsMultiplayer() ) {
			g_kSystemWrapper.SetRandomSeed( timeGetTime() );
		}
	}
}

/*
================
AsteroidTile::HandleExitSet

Deletes the tile's asteroids from the set it left.
================
*/
// FUNCTION: STBC 0x0041c600
void AsteroidTile::HandleExitSet( TGStringEvent *event ) {
	char *setName = event->GetCString();
	SetClass *set = g_kSetManager.GetSet( setName );
	delete setName;
	if ( asteroids != NULL && set != NULL ) {
		for ( int i = 0; i < numAsteroids; i++ ) {
			set->DeleteObjectFromSet( asteroids[i]->GetObjID() );
		}
		delete [] asteroids;
		asteroids = NULL;
	}
	numAsteroids = 0;
}

/*
================
AsteroidTile::CreateAsteroid
================
*/
// FUNCTION: STBC 0x0041c6c0
// DRAFT: retail loads the node before the scale's sign test and copies the tile width through the FPU (fld; fst; fmul for the half width) where this build copies it with integer moves
ObjectClass *AsteroidTile::CreateAsteroid( void ) {
	if ( !g_kLODModelManager.Contains( "Asteroid" ) ) {
		LODModel *model = g_kLODModelManager.Create( "Asteroid", 10000, 10000, 10000, 0.01f, NULL );
		model->AddLOD( "data/Models/Misc/Asteroids/asteroid.NIF", 10, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		model->SetTextureSharePath( "data/Models/Misc/Asteroids" );
		model->Load();
	}
	if ( !g_kLODModelManager.Contains( "Asteroid1" ) ) {
		LODModel *model = g_kLODModelManager.Create( "Asteroid1", 10000, 10000, 10000, 0.01f, NULL );
		model->AddLOD( "data/Models/Misc/Asteroids/asteroid1.NIF", 10, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		model->SetTextureSharePath( "data/Models/Misc/Asteroids" );
		model->Load();
	}
	if ( !g_kLODModelManager.Contains( "Asteroid2" ) ) {
		LODModel *model = g_kLODModelManager.Create( "Asteroid2", 10000, 10000, 10000, 0.01f, NULL );
		model->AddLOD( "data/Models/Misc/Asteroids/asteroid2.NIF", 10, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		model->SetTextureSharePath( "data/Models/Misc/Asteroids" );
		model->Load();
	}
	if ( !g_kLODModelManager.Contains( "Asteroid3" ) ) {
		LODModel *model = g_kLODModelManager.Create( "Asteroid3", 10000, 10000, 10000, 0.01f, NULL );
		model->AddLOD( "data/Models/Misc/Asteroids/asteroid3.NIF", 10, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		model->SetTextureSharePath( "data/Models/Misc/Asteroids" );
		model->Load();
	}

	DamageableObject *asteroid;
	g_kUtopiaModule.SetIgnoreClientIDForObjectCreation( true );
	switch ( g_kSystemWrapper.GetRandomNumber( 4 ) ) {
		case 1:
			asteroid = new DamageableObject( "Asteroid1" );
			break;
		case 2:
			asteroid = new DamageableObject( "Asteroid2" );
			break;
		case 3:
			asteroid = new DamageableObject( "Asteroid3" );
			break;
		default:
			asteroid = new DamageableObject( "Asteroid" );
			break;
	}
	g_kUtopiaModule.SetIgnoreClientIDForObjectCreation( false );

	rand();
	float size = ( rand() * ( 1.0f / RAND_MAX ) + 0.05f ) * asteroidSizeFactor ;
	size = UtopiaModule::ConvertKilometersToGameUnits( size );
	float scale = size / asteroid->GetRadius();
	if ( scale <= 0.0f ) {
		scale = -scale;
	}
	asteroid->GetNode()->SetScale( scale );
	asteroid->UpdateNodeOnly();

	float width = tileSize;
	float halfWidth = width * 0.5f;
	NiPoint3 location;
	float x = rand() * width * ( 1.0f / RAND_MAX ) - halfWidth;
	float y = rand() * width * ( 1.0f / RAND_MAX ) - halfWidth;
	float z = rand() * width * ( 1.0f / RAND_MAX ) - halfWidth;
	location = NiPoint3( x, y, z );
	location += GetWorldLocation();
	int tries = 0;
	while ( !IsAsteroidPlaceFree( &location, asteroid->GetRadius() ) && tries < 30 ) {
		x = rand() * width * ( 1.0f / RAND_MAX ) - halfWidth;
		y = rand() * width * ( 1.0f / RAND_MAX ) - halfWidth;
		z = rand() * width * ( 1.0f / RAND_MAX ) - halfWidth;
		location = NiPoint3( x, y, z );
		location += GetWorldLocation();
		tries++;
	}
	asteroid->SetHailable( false );
	asteroid->SetScannable( false );
	asteroid->SetTranslate( &location );
	asteroid->SetStatic( true );
	asteroid->SetMass( 10000.0f );
	return asteroid;
}

/*
================
AsteroidTile::WriteToStream
================
*/
// FUNCTION: STBC 0x0041cc00
bool AsteroidTile::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( GetSavedID( field ) );
	stream->WriteFloat( tileSize );
	stream->WriteFloat( asteroidSizeFactor );
	stream->WriteInt( numAsteroids );
	stream->WriteInt( randomSeed );
	stream->WriteInt( asteroidsPerTile );
	for ( int i = 0; i < numAsteroids; i++ ) {
		stream->WriteID( GetSavedID( asteroids[i] ) );
	}
	return true;
}

/*
================
AsteroidTile::ReadFromStream
================
*/
// FUNCTION: STBC 0x0041cca0
bool AsteroidTile::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	field = (AsteroidField *)stream->ReadID();
	tileSize = stream->ReadFloat();
	asteroidSizeFactor = stream->ReadFloat();
	numAsteroids = stream->ReadInt();
	randomSeed = stream->ReadInt();
	asteroidsPerTile = stream->ReadInt();
	if ( numAsteroids > 0 ) {
		asteroids = new ObjectClass *[numAsteroids];
		for ( int i = 0; i < numAsteroids; i++ ) {
			asteroids[i] = (ObjectClass *)stream->ReadID();
		}
	} else {
		asteroids = NULL;
	}
	return true;
}

/*
================
AsteroidTile::FixUpPointers

Fails until the field and the asteroids the stream named are loaded.
================
*/
// FUNCTION: STBC 0x0041cd50
bool AsteroidTile::FixUpPointers( void ) {
	if ( !BaseObjectClass::FixUpPointers() ) {
		return false;
	}
	if ( field != NULL && GetTGObjectPtr( (long)field ) == NULL ) {
		return false;
	}
	for ( int i = 0; i < numAsteroids; i++ ) {
		if ( asteroids[i] != NULL && GetTGObjectPtr( (long)asteroids[i] ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
AsteroidTile::PostFixUp

Replaces the IDs with the objects.
================
*/
// FUNCTION: STBC 0x0041cdb0
void AsteroidTile::PostFixUp( void ) {
	ObjectClass::PostFixUp();
	field = (AsteroidField *)GetTGObjectPtr( (long)field );
	for ( int i = 0; i < numAsteroids; i++ ) {
		asteroids[i] = (ObjectClass *)GetTGObjectPtr( (long)asteroids[i] );
	}
}

/*
================
AsteroidTile::WriteStaticData
================
*/
// FUNCTION: STBC 0x0041ce00
bool AsteroidTile::WriteStaticData( TGStream *stream ) {
	stream->WriteInt( asteroidNumber );
	return true;
}

/*
================
AsteroidTile::ReadStaticData
================
*/
// FUNCTION: STBC 0x0041ce20
bool AsteroidTile::ReadStaticData( TGStream *stream ) {
	asteroidNumber = stream->ReadInt();
	return true;
}
