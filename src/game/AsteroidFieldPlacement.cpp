/*
===============================================================================

	AsteroidFieldPlacement.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include "AsteroidFieldPlacement.h"
#include "SetClass.h"
#include "AsteroidField.h"
#include "TGStream.h"
#include "NiNode.h"
#include "NiLines.h"
#include "NiMatrix3.h"
#include <math.h>
#include "TGEventHandlerTable.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00495130 _$E455
// FUNCTION: STBC 0x00495150 _$E454

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00494ab0 _$E32
// FUNCTION: STBC 0x00494ac0 _$E29
// FUNCTION: STBC 0x00494ad0 _$E31
// FUNCTION: STBC 0x00494ae0 _$E30
// FUNCTION: STBC 0x00494af0 _$E38
// FUNCTION: STBC 0x00494b00 _$E35
// FUNCTION: STBC 0x00494b10 _$E37
// FUNCTION: STBC 0x00494b20 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00494b30 _$E66
// FUNCTION: STBC 0x00494b40 _$E65
// FUNCTION: STBC 0x00494b50 _$E256
// FUNCTION: STBC 0x00494b60 _$E255
// FUNCTION: STBC 0x00494b70 _$E77
// FUNCTION: STBC 0x00494b80 _$E76
// FUNCTION: STBC 0x00494b90 _$E81
// FUNCTION: STBC 0x00494ba0 _$E80
// FUNCTION: STBC 0x00494bb0 _$E85
// FUNCTION: STBC 0x00494bc0 _$E84
// FUNCTION: STBC 0x00494bd0 _$E89
// FUNCTION: STBC 0x00494be0 _$E88
// FUNCTION: STBC 0x00494bf0 _$E206
// FUNCTION: STBC 0x00494c00 _$E205
// FUNCTION: STBC 0x00494c30 _$E148
// FUNCTION: STBC 0x00494c40 _$E147
// FUNCTION: STBC 0x00494c50 _$E161
// FUNCTION: STBC 0x00494c60 _$E160
// FUNCTION: STBC 0x00494c70 _$E211
// FUNCTION: STBC 0x00494c80 _$E210
// FUNCTION: STBC 0x00494c90 _$E261
// FUNCTION: STBC 0x00494ca0 _$E260
// FUNCTION: STBC 0x00494cb0 _$E215
// FUNCTION: STBC 0x00494cc0 _$E214
// FUNCTION: STBC 0x00494cd0 _$E265
// FUNCTION: STBC 0x00494ce0 _$E264
// FUNCTION: STBC 0x00494cf0 _$E272
// FUNCTION: STBC 0x00494d00 _$E271
// FUNCTION: STBC 0x00494d10 _$E282
// FUNCTION: STBC 0x00494d20 _$E281
// FUNCTION: STBC 0x00494d30 _$E286
// FUNCTION: STBC 0x00494d40 _$E285
// FUNCTION: STBC 0x00494d50 _$E106
// FUNCTION: STBC 0x00494d60 _$E105
// FUNCTION: STBC 0x00494d70 _$E237
// FUNCTION: STBC 0x00494d80 _$E236
// FUNCTION: STBC 0x00494d90 _$E244
// FUNCTION: STBC 0x00494da0 _$E243
// FUNCTION: STBC 0x00494db0 _$E298
// FUNCTION: STBC 0x00494dc0 _$E297
// FUNCTION: STBC 0x00494dd0 _$E302
// FUNCTION: STBC 0x00494de0 _$E301
// FUNCTION: STBC 0x00494df0 _$E342
// FUNCTION: STBC 0x00494e00 _$E341
// FUNCTION: STBC 0x00494e10 _$E348
// FUNCTION: STBC 0x00494e20 _$E347
// FUNCTION: STBC 0x00494e30 _$E290
// FUNCTION: STBC 0x00494e40 _$E289
// FUNCTION: STBC 0x00494e50 _$E294
// FUNCTION: STBC 0x00494e60 _$E293
// FUNCTION: STBC 0x00494e70 _$E306
// FUNCTION: STBC 0x00494e80 _$E305
// FUNCTION: STBC 0x00494e90 _$E310
// FUNCTION: STBC 0x00494ea0 _$E309
// FUNCTION: STBC 0x00494eb0 _$E314
// FUNCTION: STBC 0x00494ec0 _$E313
// FUNCTION: STBC 0x00494ed0 _$E318
// FUNCTION: STBC 0x00494ee0 _$E317
// FUNCTION: STBC 0x00494ef0 _$E322
// FUNCTION: STBC 0x00494f00 _$E321
// FUNCTION: STBC 0x00494f10 _$E326
// FUNCTION: STBC 0x00494f20 _$E325
// FUNCTION: STBC 0x00494f30 _$E330
// FUNCTION: STBC 0x00494f40 _$E329
// FUNCTION: STBC 0x00494f50 _$E334
// FUNCTION: STBC 0x00494f60 _$E333
// FUNCTION: STBC 0x00494f70 _$E111
// FUNCTION: STBC 0x00494f80 _$E110
// FUNCTION: STBC 0x00494f90 _$E338
// FUNCTION: STBC 0x00494fa0 _$E337
// FUNCTION: STBC 0x00494fb0 _$E116
// FUNCTION: STBC 0x00494fc0 _$E115
// FUNCTION: STBC 0x00494fd0 _$E352
// FUNCTION: STBC 0x00494fe0 _$E351
// FUNCTION: STBC 0x00494ff0 _$E356
// FUNCTION: STBC 0x00495000 _$E355
// FUNCTION: STBC 0x00495010 _$E360
// FUNCTION: STBC 0x00495020 _$E359
// FUNCTION: STBC 0x00495030 _$E364
// FUNCTION: STBC 0x00495040 _$E363
// FUNCTION: STBC 0x00495050 _$E125
// FUNCTION: STBC 0x00495060 _$E124
// FUNCTION: STBC 0x00495070 _$E248
// FUNCTION: STBC 0x00495080 _$E247
// FUNCTION: STBC 0x00495090 _$E252
// FUNCTION: STBC 0x004950a0 _$E251
// FUNCTION: STBC 0x004950b0 _$E368
// FUNCTION: STBC 0x004950c0 _$E367
// FUNCTION: STBC 0x004950d0 _$E131
// FUNCTION: STBC 0x004950e0 _$E130
// FUNCTION: STBC 0x004950f0 _$E136
// FUNCTION: STBC 0x00495100 _$E135
// FUNCTION: STBC 0x00495110 _$E166
// FUNCTION: STBC 0x00495120 _$E165
// FUNCTION: STBC 0x00495160 _$E372
// FUNCTION: STBC 0x00495170 _$E371
// FUNCTION: STBC 0x00495180 _$E171
// FUNCTION: STBC 0x00495190 _$E170
// FUNCTION: STBC 0x004951a0 _$E376
// FUNCTION: STBC 0x004951b0 _$E375
// FUNCTION: STBC 0x004951c0 _$E380
// FUNCTION: STBC 0x004951d0 _$E379
// FUNCTION: STBC 0x004951e0 _$E176
// FUNCTION: STBC 0x004951f0 _$E175
// FUNCTION: STBC 0x00495200 _$E384
// FUNCTION: STBC 0x00495210 _$E383
// FUNCTION: STBC 0x00495220 _$E141
// FUNCTION: STBC 0x00495230 _$E140
// FUNCTION: STBC 0x00495240 _$E388
// FUNCTION: STBC 0x00495250 _$E387
// FUNCTION: STBC 0x00495260 _$E392
// FUNCTION: STBC 0x00495270 _$E391
// FUNCTION: STBC 0x00495280 _$E181
// FUNCTION: STBC 0x00495290 _$E180
// FUNCTION: STBC 0x004952a0 _$E186
// FUNCTION: STBC 0x004952b0 _$E185
// FUNCTION: STBC 0x004952c0 _$E396
// FUNCTION: STBC 0x004952d0 _$E395
// FUNCTION: STBC 0x004952e0 _$E400
// FUNCTION: STBC 0x004952f0 _$E399
// FUNCTION: STBC 0x00495300 _$E404
// FUNCTION: STBC 0x00495310 _$E403
// FUNCTION: STBC 0x00495320 _$E408
// FUNCTION: STBC 0x00495330 _$E407
// FUNCTION: STBC 0x00495340 _$E412
// FUNCTION: STBC 0x00495350 _$E411
// FUNCTION: STBC 0x00495360 _$E416
// FUNCTION: STBC 0x00495370 _$E415
// FUNCTION: STBC 0x00495380 _$E420
// FUNCTION: STBC 0x00495390 _$E419
// FUNCTION: STBC 0x004953a0 _$E424
// FUNCTION: STBC 0x004953b0 _$E423
// FUNCTION: STBC 0x004953c0 _$E191
// FUNCTION: STBC 0x004953d0 _$E190
// FUNCTION: STBC 0x004953e0 _$E428
// FUNCTION: STBC 0x004953f0 _$E427
// FUNCTION: STBC 0x00495400 _$E196
// FUNCTION: STBC 0x00495410 _$E195
// FUNCTION: STBC 0x00495420 _$E201
// FUNCTION: STBC 0x00495430 _$E200
// FUNCTION: STBC 0x00495440 _$E432
// FUNCTION: STBC 0x00495450 _$E431
// FUNCTION: STBC 0x00495460 _$E437
// FUNCTION: STBC 0x00495470 _$E436
// FUNCTION: STBC 0x00495480 _$E441
// FUNCTION: STBC 0x00495490 _$E440

// The handler table's initialiser and the exit-time destructor.
// FUNCTION: STBC 0x00493c70 _$E447
// FUNCTION: STBC 0x00493c80 _$E443
// FUNCTION: STBC 0x00493c90 _$E446
// FUNCTION: STBC 0x00493ca0 _$E444

// DATA: STBC 0x00982ee0
TGEventHandlerTable AsteroidFieldPlacement::classHandlers;

/*
================
AsteroidFieldPlacement::Cast

The object as an AsteroidFieldPlacement; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00493cc0
AsteroidFieldPlacement *AsteroidFieldPlacement::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( ASTEROIDFIELDPLACEMENT_TYPE ) ) {
		return (AsteroidFieldPlacement *)object;
	}
	return NULL;
}

/*
================
AsteroidFieldPlacement::AsteroidFieldPlacement

The field's defaults, and the node of rings for them under the placement's
node.
================
*/
// FUNCTION: STBC 0x00493cf0
AsteroidFieldPlacement::AsteroidFieldPlacement( char *name_, char *setName, char *modelName_ ) : PlacementObject( name_, setName, modelName_ ) {
	fieldRadius = 1000.0f;
	fieldID = 0;
	numTilesPerAxis = 3;
	numAsteroidsPerTile = 2;
	asteroidSizeFactor = 10.0f;
	NiNode *node = NiDynamicCast( NiNode, niObject );
	node->AttachChild( CreateFieldModel(), false );
}

/*
================
AsteroidFieldPlacement::~AsteroidFieldPlacement
================
*/
// FUNCTION: STBC 0x00493dc0
AsteroidFieldPlacement::~AsteroidFieldPlacement( void ) {
}

static const float PI = 3.1415927f;

/*
================
AsteroidFieldPlacement::CreateFieldModel

For each axis, 40 circles a twentieth of the radius apart along it, each
with the sphere's radius at that distance: about Z (turned a quarter) for
X, flat for Y and about X for Z.
================
*/
// FUNCTION: STBC 0x00493dd0
// PARKED: schedule tie at c2: the ( i - 20 ) spill store for fild (height 12) ties the NiPoint3::ZERO.z load (height 4 + memory read) at 0x18000 and loses on input order; retail needs one more height on the fild chain or the store earlier in the input, and no spelling of the distance or the copy gives either
NiNode *AsteroidFieldPlacement::CreateFieldModel( void ) {
	NiNode *node = new NiNode;
	for ( int axis = 0; axis <= 2; axis++ ) {
		for ( int i = 0; i < 40; i++ ) {
			NiPoint3 offset = NiPoint3::ZERO;
			float distance = ( ( i - 20 ) / 20.0f ) * fieldRadius;
			switch ( axis ) {
				case 0:
					offset.x += distance;
					break;
				case 1:
					offset.y += distance;
					break;
				case 2:
					offset.z += distance;
					break;
			}

			float ringRadius = (float)sqrt( fieldRadius * fieldRadius - distance * distance );
			NiLines *circle = Unknown493FD0( ringRadius );
			NiMatrix3 rotation;
			switch ( axis ) {
				case 0:
					rotation.MakeZRotation( PI * 0.5f );
					break;
				case 1:
					rotation = NiMatrix3::IDENTITY;
					break;
				case 2:
					rotation.MakeXRotation( PI * 0.5f );
					break;
			}
			circle->SetRotate( rotation );
			circle->SetTranslate( circle->GetTranslate() + offset );
			NiDynamicCast( NiGeometry, circle )->modelData->IncRevisionID();
			node->AttachChild( circle, false );
		}
	}
	node->UpdateProperties();
	return node;
}

/*
================
AsteroidFieldPlacement::Unknown493FD0

A circle of the radius in the XY plane, as 40 connected white points.
================
*/
// FUNCTION: STBC 0x00493fd0
// DRAFT: x87 operand order: in the loop's first sum (the cosine term plus the centre) retail loads the centre first for z and y; this build loads the product first for all three
NiLines *AsteroidFieldPlacement::Unknown493FD0( float radius ) {
	NiPoint3 *vertices = new NiPoint3[40];
	NiPoint3 center = NiPoint3::ZERO;
	NiPoint3 axis2 = NiPoint3::UNIT_Z;
	NiPoint3 axis1 = NiPoint3::UNIT_X;
	vertices[0] = axis1 * radius + center;
	for ( int i = 1; i < 40; i++ ) {
		// BUG: the angle lags a step: the second point repeats the first, and
		// the circle stops two steps short of closing
		float fraction = ( i - 1 ) / 40.0f;
		float angle = fraction * PI * 2.0f;
		float s = (float)sin( angle );
		float c = (float)cos( angle );
		vertices[i] = ( axis1 * c * radius + center ) + axis2 * s * radius;
	}

	NiColorA *colors = new NiColorA[40];
	for ( int j = 0; j < 40; j++ ) {
		colors[j] = NiColorA::WHITE;
	}
	bool *flags = new bool[40];
	memset( flags, true, 40 );

	NiLines *circle = new NiLines( 40, vertices, colors, NULL, flags );
	NiBound &bound = circle->modelData->GetBound();
	bound.SetRadius( radius );
	bound.unknown10 = true;
	return circle;
}

/*
================
AsteroidFieldPlacement::Update
================
*/
// FUNCTION: STBC 0x00494330
void AsteroidFieldPlacement::Update( float time ) {
	PlacementObject::Update( time );
}

/*
================
AsteroidFieldPlacement::VirtualSlot68

The lines that make the placement: a comment with its name, and the call
that creates it, with the model name when it has one.
================
*/
// FUNCTION: STBC 0x00494340
bool AsteroidFieldPlacement::VirtualSlot68( FILE *file, char *setName ) {
	fprintf( file, "\t# Asteroid Field Position \"%s\"\n", name );
	if ( modelName != NULL ) {
		fprintf( file, "\tkThis = App.AsteroidFieldPlacement_Create(\"%s\", sSetName, \"%s\")\n", name, modelName );
		return true;
	}
	fprintf( file, "\tkThis = App.AsteroidFieldPlacement_Create(\"%s\", sSetName, None)\n", name );
	return true;
}

/*
================
AsteroidFieldPlacement::VirtualSlot69

The lines that give the placement its field's radius, tiles, asteroids and
size factor, and have the field made.
================
*/
// FUNCTION: STBC 0x004943a0
bool AsteroidFieldPlacement::VirtualSlot69( FILE *file ) {
	PlacementObject::VirtualSlot69( file );
	fprintf( file, "\tkThis.SetFieldRadius(%f)\n", fieldRadius );
	fprintf( file, "\tkThis.SetNumTilesPerAxis(%d)\n", numTilesPerAxis );
	fprintf( file, "\tkThis.SetNumAsteroidsPerTile(%d)\n", numAsteroidsPerTile );
	fprintf( file, "\tkThis.SetAsteroidSizeFactor(%f)\n", asteroidSizeFactor );
	fprintf( file, "\tkThis.UpdateNodeOnly()\n" );
	fprintf( file, "\tkThis.ConfigField()\n" );
	return true;
}

/*
================
AsteroidFieldPlacement::VirtualSlot70
================
*/
// FUNCTION: STBC 0x00494420
bool AsteroidFieldPlacement::VirtualSlot70( FILE *file ) {
	PlacementObject::VirtualSlot70( file );
	return true;
}

/*
================
AsteroidFieldPlacement::GetObject

The placement of the name in the set, or in any set when the set is NULL.
The search is PlacementObject's, so it finds a placement of any class.
================
*/
// FUNCTION: STBC 0x00494430
// BUG: a NULL name is not checked; strcmp reads it
AsteroidFieldPlacement *AsteroidFieldPlacement::GetObject( SetClass *set, char *name ) {
	if ( set != NULL ) {
		return GetObjectBySetName( set->GetName(), name );
	}
	NiTMapIterator pos = placementsBySet.GetFirstPos();
	while ( pos != NULL ) {
		const char *setName;
		PlacementList *list;
		placementsBySet.GetNext( pos, setName, list );
		PlacementObject **objects = list->GetItems();
		for ( int i = list->GetCount() - 1; i >= 0; i-- ) {
			if ( strcmp( objects[i]->GetName(), name ) == 0 ) {
				return static_cast<AsteroidFieldPlacement *>( objects[i] );
			}
		}
	}
	return NULL;
}

/*
================
AsteroidFieldPlacement::GetObjectBySetName

The placement of the name in the set of that name.
================
*/
// FUNCTION: STBC 0x00494530
AsteroidFieldPlacement *AsteroidFieldPlacement::GetObjectBySetName( char *setName, char *name ) {
	PlacementObject *found = NULL;
	PlacementList *list;
	if ( placementsBySet.GetAt( setName, list ) ) {
		PlacementObject **objects = list->GetItems();
		for ( int i = list->GetCount() - 1; i >= 0; i-- ) {
			if ( strcmp( objects[i]->GetName(), name ) == 0 ) {
				found = objects[i];
				break;
			}
		}
	}
	return static_cast<AsteroidFieldPlacement *>( found );
}

/*
================
AsteroidFieldPlacement::AddToSet

Files the placement under its class type, then with the other objects of
the set.
================
*/
// FUNCTION: STBC 0x004945f0
void AsteroidFieldPlacement::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, ASTEROIDFIELDPLACEMENT_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
AsteroidFieldPlacement::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00494620
void AsteroidFieldPlacement::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, ASTEROIDFIELDPLACEMENT_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
AsteroidFieldPlacement::GetObjectByID

The placement with the ID in the set; NULL when the set is NULL or has none.
The body of PlacementObject::GetObjectByID, over the same placements.
================
*/
// FUNCTION: STBC 0x00494650
AsteroidFieldPlacement *AsteroidFieldPlacement::GetObjectByID( SetClass *set, long id ) {
	AsteroidFieldPlacement *object = NULL;
	PlacementList *list;
	if ( set != NULL && placementsBySet.GetAt( set->GetName(), list ) ) {
		object = (AsteroidFieldPlacement *)list->Get( id );
	}
	return object;
}

/*
================
AsteroidFieldPlacement::GetFieldModel

The child of the placement's node that SetFieldRadius makes (the node's
second child; the first is the placement's own model).
================
*/
// FUNCTION: STBC 0x00494700
NiAVObject *AsteroidFieldPlacement::GetFieldModel( void ) {
	NiNode *node = NiDynamicCast( NiNode, niObject );
	return node->GetAt( 1 );
}

/*
================
AsteroidFieldPlacement::SetFieldRadius

Takes the field's model off the placement's node, and makes a new one for
the radius.
================
*/
// FUNCTION: STBC 0x00494760
void AsteroidFieldPlacement::SetFieldRadius( float radius ) {
	fieldRadius = radius;
	if ( GetFieldModel() != NULL ) {
		NiNode *parent = GetFieldModel()->GetParent();
		if ( parent != NULL ) {
			parent->DetachChild( GetFieldModel() );
		}
	}
	NiNode *node = NiDynamicCast( NiNode, niObject );
	node->AttachChild( CreateFieldModel(), false );
}

/*
================
AsteroidFieldPlacement::GetField

The field object, looked up by its ID in the placement's set (or the set
the placement is listed under); NULL when there is none.
================
*/
// FUNCTION: STBC 0x00494820 ?GetField@AsteroidFieldPlacement@@QAEPAVAsteroidField@@XZ
AsteroidField *AsteroidFieldPlacement::GetField( void ) {
	SetClass *set = GetContainingSet();
	if ( set == NULL ) {
		set = FindContainingSet();
	}
	if ( set != NULL ) {
		return AsteroidField::GetObjectByID( set, fieldID );
	}
	return NULL;
}

/*
================
AsteroidFieldPlacement::ConfigField

Replaces the field object with a new one named after the placement, at
its location, with the placement's radius, tiles and asteroids.
================
*/
// FUNCTION: STBC 0x00494850 ?ConfigField@AsteroidFieldPlacement@@QAEXXZ
void AsteroidFieldPlacement::ConfigField( void ) {
	AsteroidField *field = GetField();
	if ( field != NULL ) {
		if ( field->GetContainingSet() != NULL ) {
			field->GetContainingSet()->DeleteObjectFromSet( field->GetObjID() );
		} else {
			delete field;
		}
	}

	char *fieldName = new char[strlen( GetName() ) + strlen( "AsteroidField" ) + 1];
	strcpy( fieldName, GetName() );
	strcat( fieldName, "AsteroidField" );
	field = AsteroidField::Create( GetNumTilesPerAxis() );
	field->SetOuterFieldRadius( GetFieldRadius() );
	field->SetInnerFieldRadius( GetFieldRadius() );
	field->SetNumAsteroidsPerTile( GetNumAsteroidsPerTile() );
	field->SetAsteroidSizeFactor( GetAsteroidSizeFactor() );
	field->SetTranslate( GetWorldLocation() );
	fieldID = field->GetObjID();
	if ( !FindContainingSet()->AddObjectToSet( field, fieldName ) ) {
		delete field;
		fieldID = 0;
		delete [] fieldName;
		return;
	}
	field->UpdateNodeOnly();
	delete [] fieldName;
}

/*
================
AsteroidFieldPlacement::SetNumTilesPerAxis
================
*/
// FUNCTION: STBC 0x004949c0
void AsteroidFieldPlacement::SetNumTilesPerAxis( int count ) {
	numTilesPerAxis = count;
}

/*
================
AsteroidFieldPlacement::SetNumAsteroidsPerTile
================
*/
// FUNCTION: STBC 0x004949d0
void AsteroidFieldPlacement::SetNumAsteroidsPerTile( int count ) {
	numAsteroidsPerTile = count;
}

/*
================
AsteroidFieldPlacement::SetAsteroidSizeFactor
================
*/
// FUNCTION: STBC 0x004949e0
void AsteroidFieldPlacement::SetAsteroidSizeFactor( float factor ) {
	asteroidSizeFactor = factor;
}

/*
================
AsteroidFieldPlacement::WriteToStream
================
*/
// FUNCTION: STBC 0x004949f0
bool AsteroidFieldPlacement::WriteToStream( TGStream *stream ) {
	if ( !PlacementObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( fieldRadius );
	stream->WriteInt( numTilesPerAxis );
	stream->WriteInt( numAsteroidsPerTile );
	stream->WriteFloat( asteroidSizeFactor );
	stream->WriteID( fieldID );
	return true;
}

/*
================
AsteroidFieldPlacement::ReadFromStream
================
*/
// FUNCTION: STBC 0x00494a50
bool AsteroidFieldPlacement::ReadFromStream( TGStream *stream ) {
	if ( !PlacementObject::ReadFromStream( stream ) ) {
		return false;
	}
	fieldRadius = stream->ReadFloat();
	numTilesPerAxis = stream->ReadInt();
	numAsteroidsPerTile = stream->ReadInt();
	asteroidSizeFactor = stream->ReadFloat();
	fieldID = stream->ReadID();
	return true;
}
