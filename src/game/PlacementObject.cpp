/*
===============================================================================

	PlacementObject.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "PlacementObject.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "MapWindow.h"
#include "TGModelManager.h"
#include "TGAnimNode.h"
#include "TGPoint3.h"
#include "ImageManagerClass.h"
#include "NiTriangles.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004a3ec0 _$E483
// FUNCTION: STBC 0x004a3ee0 _$E482

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004a3480 _$E32
// FUNCTION: STBC 0x004a3490 _$E29
// FUNCTION: STBC 0x004a34a0 _$E31
// FUNCTION: STBC 0x004a34b0 _$E30
// FUNCTION: STBC 0x004a34c0 _$E38
// FUNCTION: STBC 0x004a34d0 _$E35
// FUNCTION: STBC 0x004a34e0 _$E37
// FUNCTION: STBC 0x004a34f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004a3500 _$E66
// FUNCTION: STBC 0x004a3510 _$E65
// FUNCTION: STBC 0x004a3520 _$E239
// FUNCTION: STBC 0x004a3530 _$E238
// FUNCTION: STBC 0x004a3540 _$E77
// FUNCTION: STBC 0x004a3550 _$E76
// FUNCTION: STBC 0x004a3560 _$E81
// FUNCTION: STBC 0x004a3570 _$E80
// FUNCTION: STBC 0x004a3580 _$E85
// FUNCTION: STBC 0x004a3590 _$E84
// FUNCTION: STBC 0x004a35a0 _$E89
// FUNCTION: STBC 0x004a35b0 _$E88
// FUNCTION: STBC 0x004a35c0 _$E206
// FUNCTION: STBC 0x004a35d0 _$E205
// FUNCTION: STBC 0x004a35e0 _$E148
// FUNCTION: STBC 0x004a35f0 _$E147
// FUNCTION: STBC 0x004a3600 _$E161
// FUNCTION: STBC 0x004a3610 _$E160
// FUNCTION: STBC 0x004a3620 _$E211
// FUNCTION: STBC 0x004a3630 _$E210
// FUNCTION: STBC 0x004a3640 _$E244
// FUNCTION: STBC 0x004a3650 _$E243
// FUNCTION: STBC 0x004a3660 _$E215
// FUNCTION: STBC 0x004a3670 _$E214
// FUNCTION: STBC 0x004a3680 _$E248
// FUNCTION: STBC 0x004a3690 _$E247
// FUNCTION: STBC 0x004a36a0 _$E255
// FUNCTION: STBC 0x004a36b0 _$E254
// FUNCTION: STBC 0x004a36c0 _$E316
// FUNCTION: STBC 0x004a36d0 _$E315
// FUNCTION: STBC 0x004a36e0 _$E320
// FUNCTION: STBC 0x004a36f0 _$E319
// FUNCTION: STBC 0x004a3700 _$E106
// FUNCTION: STBC 0x004a3710 _$E105
// FUNCTION: STBC 0x004a3720 _$E270
// FUNCTION: STBC 0x004a3730 _$E269
// FUNCTION: STBC 0x004a3740 _$E277
// FUNCTION: STBC 0x004a3750 _$E276
// FUNCTION: STBC 0x004a3760 _$E281
// FUNCTION: STBC 0x004a3770 _$E280
// FUNCTION: STBC 0x004a3780 _$E285
// FUNCTION: STBC 0x004a3790 _$E284
// FUNCTION: STBC 0x004a37a0 _$E368
// FUNCTION: STBC 0x004a37b0 _$E367
// FUNCTION: STBC 0x004a37c0 _$E374
// FUNCTION: STBC 0x004a37d0 _$E373
// FUNCTION: STBC 0x004a37e0 _$E324
// FUNCTION: STBC 0x004a37f0 _$E323
// FUNCTION: STBC 0x004a3800 _$E328
// FUNCTION: STBC 0x004a3810 _$E327
// FUNCTION: STBC 0x004a3970 _$E332
// FUNCTION: STBC 0x004a3980 _$E331
// FUNCTION: STBC 0x004a3a80 _$E336
// FUNCTION: STBC 0x004a3a90 _$E335
// FUNCTION: STBC 0x004a3aa0 _$E340
// FUNCTION: STBC 0x004a3ab0 _$E339
// FUNCTION: STBC 0x004a3ac0 _$E344
// FUNCTION: STBC 0x004a3ad0 _$E343
// FUNCTION: STBC 0x004a3ae0 _$E348
// FUNCTION: STBC 0x004a3af0 _$E347
// FUNCTION: STBC 0x004a3b00 _$E352
// FUNCTION: STBC 0x004a3b10 _$E351
// FUNCTION: STBC 0x004a3b70 _$E356
// FUNCTION: STBC 0x004a3b80 _$E355
// FUNCTION: STBC 0x004a3b90 _$E360
// FUNCTION: STBC 0x004a3ba0 _$E359
// FUNCTION: STBC 0x004a3bb0 _$E111
// FUNCTION: STBC 0x004a3bc0 _$E110
// FUNCTION: STBC 0x004a3bd0 _$E364
// FUNCTION: STBC 0x004a3be0 _$E363
// FUNCTION: STBC 0x004a3bf0 _$E116
// FUNCTION: STBC 0x004a3c00 _$E115
// FUNCTION: STBC 0x004a3c10 _$E378
// FUNCTION: STBC 0x004a3c20 _$E377
// FUNCTION: STBC 0x004a3c30 _$E382
// FUNCTION: STBC 0x004a3c40 _$E381
// FUNCTION: STBC 0x004a3c50 _$E386
// FUNCTION: STBC 0x004a3c60 _$E385
// FUNCTION: STBC 0x004a3c70 _$E390
// FUNCTION: STBC 0x004a3c80 _$E389
// FUNCTION: STBC 0x004a3d30 _$E125
// FUNCTION: STBC 0x004a3d40 _$E124
// FUNCTION: STBC 0x004a3d50 _$E394
// FUNCTION: STBC 0x004a3d60 _$E393
// FUNCTION: STBC 0x004a3d70 _$E398
// FUNCTION: STBC 0x004a3d80 _$E397
// FUNCTION: STBC 0x004a3d90 _$E402
// FUNCTION: STBC 0x004a3da0 _$E401
// FUNCTION: STBC 0x004a3db0 _$E131
// FUNCTION: STBC 0x004a3dc0 _$E130
// FUNCTION: STBC 0x004a3e10 _$E136
// FUNCTION: STBC 0x004a3e20 _$E135
// FUNCTION: STBC 0x004a3e60 _$E166
// FUNCTION: STBC 0x004a3e70 _$E165
// FUNCTION: STBC 0x004a3e80 _$E406
// FUNCTION: STBC 0x004a3e90 _$E405
// FUNCTION: STBC 0x004a3ea0 _$E171
// FUNCTION: STBC 0x004a3eb0 _$E170
// FUNCTION: STBC 0x004a3ef0 _$E410
// FUNCTION: STBC 0x004a3f00 _$E409
// FUNCTION: STBC 0x004a3f10 _$E414
// FUNCTION: STBC 0x004a3f20 _$E413
// FUNCTION: STBC 0x004a3f30 _$E176
// FUNCTION: STBC 0x004a3f40 _$E175
// FUNCTION: STBC 0x004a3f50 _$E418
// FUNCTION: STBC 0x004a3f60 _$E417
// FUNCTION: STBC 0x004a3f70 _$E141
// FUNCTION: STBC 0x004a3f80 _$E140
// FUNCTION: STBC 0x004a3f90 _$E422
// FUNCTION: STBC 0x004a3fa0 _$E421
// FUNCTION: STBC 0x004a3fb0 _$E426
// FUNCTION: STBC 0x004a3fc0 _$E425
// FUNCTION: STBC 0x004a3fd0 _$E181
// FUNCTION: STBC 0x004a3fe0 _$E180
// FUNCTION: STBC 0x004a3ff0 _$E186
// FUNCTION: STBC 0x004a4000 _$E185
// FUNCTION: STBC 0x004a4010 _$E289
// FUNCTION: STBC 0x004a4020 _$E288
// FUNCTION: STBC 0x004a4030 _$E293
// FUNCTION: STBC 0x004a4040 _$E292
// FUNCTION: STBC 0x004a4050 _$E430
// FUNCTION: STBC 0x004a4060 _$E429
// FUNCTION: STBC 0x004a4070 _$E434
// FUNCTION: STBC 0x004a4080 _$E433
// FUNCTION: STBC 0x004a4090 _$E438
// FUNCTION: STBC 0x004a40a0 _$E437
// FUNCTION: STBC 0x004a40b0 _$E442
// FUNCTION: STBC 0x004a40c0 _$E441
// FUNCTION: STBC 0x004a40d0 _$E446
// FUNCTION: STBC 0x004a40e0 _$E445
// FUNCTION: STBC 0x004a40f0 _$E450
// FUNCTION: STBC 0x004a4100 _$E449
// FUNCTION: STBC 0x004a4110 _$E191
// FUNCTION: STBC 0x004a4120 _$E190
// FUNCTION: STBC 0x004a4130 _$E454
// FUNCTION: STBC 0x004a4140 _$E453
// FUNCTION: STBC 0x004a4150 _$E196
// FUNCTION: STBC 0x004a4160 _$E195
// FUNCTION: STBC 0x004a4170 _$E201
// FUNCTION: STBC 0x004a4180 _$E200
// FUNCTION: STBC 0x004a4190 _$E297
// FUNCTION: STBC 0x004a41a0 _$E296
// FUNCTION: STBC 0x004a41b0 _$E302
// FUNCTION: STBC 0x004a41c0 _$E301
// FUNCTION: STBC 0x004a41d0 _$E306
// FUNCTION: STBC 0x004a41e0 _$E305

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004a1000 _$E460
// FUNCTION: STBC 0x004a1010 _$E456
// FUNCTION: STBC 0x004a1020 _$E459
// FUNCTION: STBC 0x004a1030 _$E457
// DATA: STBC 0x009835d8
TGEventHandlerTable PlacementObject::classHandlers;

/*
================
PlacementObject::Cast

The object as a PlacementObject; NULL when it is none.
================
*/
// FUNCTION: STBC 0x004a1050
PlacementObject *PlacementObject::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PLACEMENTOBJECT_TYPE ) ) {
		return (PlacementObject *)object;
	}
	return NULL;
}

// Its initialiser (37 buckets), the registration of its exit-time destructor
// and the guarded destructor.
// FUNCTION: STBC 0x004a1080 _$E466
// FUNCTION: STBC 0x004a1090 _$E462
// FUNCTION: STBC 0x004a10a0 _$E465
// FUNCTION: STBC 0x004a10b0 _$E463
// DATA: STBC 0x00983740
NiTStringMap<PlacementList *> PlacementObject::placementsBySet;

// DATA: STBC 0x009837bc
bool PlacementObject::placementsShown;

// The colours of the model's three parts (its arrow head, the right fin and
// the up fin, by ApplyColors' face table): a plain placement's, a static
// one's and a nav point's (invented names).
// DATA: STBC 0x00983658
static NiColorA placementColors[3] = { NiColorA( 1.0f, 0.0f, 0.0f, 1.0f ), NiColorA( 0.0f, 1.0f, 0.0f, 1.0f ), NiColorA( 0.0f, 0.0f, 1.0f, 1.0f ) };
// DATA: STBC 0x009836d8
static NiColorA staticColors[3] = { NiColorA( 1.0f, 0.0f, 1.0f, 1.0f ), NiColorA( 1.0f, 1.0f, 0.0f, 1.0f ), NiColorA( 0.0f, 1.0f, 1.0f, 1.0f ) };
// DATA: STBC 0x00983600
static NiColorA navPointColors[3] = { NiColorA( 0.5f, 0.0f, 0.0f, 1.0f ), NiColorA( 0.5f, 0.5f, 0.0f, 1.0f ), NiColorA( 0.0f, 0.0f, 0.5f, 1.0f ) };
// Their initialisers, each entered by a jump.
// FUNCTION: STBC 0x004a10d0 _$E469
// FUNCTION: STBC 0x004a10e0 _$E468
// FUNCTION: STBC 0x004a1160 _$E472
// FUNCTION: STBC 0x004a1170 _$E471
// FUNCTION: STBC 0x004a11f0 _$E475
// FUNCTION: STBC 0x004a1200 _$E474

/*
================
PlacementObject::PlacementObject

Files the placement under the set's name, in the list sorted by ID.
================
*/
// FUNCTION: STBC 0x004a1280 ??0PlacementObject@@QAE@PAD00@Z
PlacementObject::PlacementObject( char *name, char *setName, char *modelName_ ) : ObjectClass( 0L ),
	modelName( NULL ), isStatic( false ), isNavPoint( false ) {
	SetCollisionFlags( ObjectClass::CFB_NO_COLLISIONS );
	SetName( name );
	SetModel( modelName_ );
	SetScannable( false );
	SetHailable( false );

	PlacementList *list;
	if ( !placementsBySet.GetAt( setName, list ) ) {
		list = new PlacementList;
		placementsBySet.SetAt( setName, list );
	}
	list->Insert( this );
}

/*
================
PlacementObject::~PlacementObject

Takes the placement out of every set's list; a list left empty leaves the
map, and the walk starts over.
================
*/
// FUNCTION: STBC 0x004a14d0
PlacementObject::~PlacementObject( void ) {
	NiTMapIterator pos = placementsBySet.GetFirstPos();
	while ( pos != NULL ) {
		const char *setName;
		PlacementList *list;
		placementsBySet.GetNext( pos, setName, list );
		int i = list->GetCount() - 1;
		PlacementObject **objects = list->GetItems();
		for ( ; i >= 0; i-- ) {
			if ( objects[i] == this ) {
				list->Remove( this );
				if ( list->GetCount() == 0 ) {
					// BUG: the emptied list itself is never deleted
					placementsBySet.RemoveAt( setName );
					pos = placementsBySet.GetFirstPos();
					break;
				}
			}
		}
	}
	delete modelName;
	modelName = NULL;
}

/*
================
PlacementObject::Update

Updates the object, and the node of one that is in no set (a set updates
the nodes it holds).
================
*/
// FUNCTION: STBC 0x004a1730
void PlacementObject::Update( float time ) {
	ObjectClass::Update( time );
	if ( containingSet == NULL && node != NULL ) {
		node->Update( time );
	}
}

/*
================
PlacementObject::SetModel

Puts the named model, or the placement's own arrow when there is none,
under a new node and makes that node the object's.
================
*/
// FUNCTION: STBC 0x004a1760
void PlacementObject::SetModel( char *name ) {
	NiAVObject *model = NULL;
	if ( name != NULL ) {
		// BUG: looks up the previous name, and the previous copy is not freed
		model = g_kModelManager.GetModel( modelName );
		modelName = new char[strlen( name ) + 1];
		strcpy( modelName, name );
	}
	if ( model == NULL ) {
		model = CreateModel();
	}
	NiNode *node = new NiNode;
	node->AttachChild( model, false );
	SetNiObject( node );
}

/*
================
PlacementObject::SaveObject

Writes the placement through its three save steps; false as soon as one
fails. The three slots are not identified (0x110, 0x114 and 0x118).
================
*/
// FUNCTION: STBC 0x004a1840
bool PlacementObject::SaveObject( FILE *file, char *setName ) {
	if ( VirtualSlot68( file, setName ) && VirtualSlot69( file ) && VirtualSlot70( file ) ) {
		return true;
	}
	return false;
}

/*
================
PlacementObject::VirtualSlot68

Writes the comment and the line of the saved script that make the
placement, with its model when it has one.
================
*/
// FUNCTION: STBC 0x004a1890
bool PlacementObject::VirtualSlot68( FILE *file, char *setName ) {
	fprintf( file, "\t# Position \"%s\"\n", GetName() );
	if ( modelName != NULL ) {
		fprintf( file, "\tkThis = App.PlacementObject_Create(\"%s\", sSetName, \"%s\")\n", GetName(), modelName );
	} else {
		fprintf( file, "\tkThis = App.PlacementObject_Create(\"%s\", sSetName, None)\n", GetName() );
	}
	return true;
}

/*
================
PlacementObject::VirtualSlot69

Writes the lines of the saved script that set the static and nav point
flags, the position and the orientation.
================
*/
// FUNCTION: STBC 0x004a18f0
bool PlacementObject::VirtualSlot69( FILE *file ) {
	fprintf( file, "\tkThis.SetStatic(%d)\n", isStatic );
	fprintf( file, "\tkThis.SetNavPoint(%d)\n", isNavPoint );
	NiPoint3 position = GetTranslate();
	fprintf( file, "\tkThis.SetTranslateXYZ(%f, %f, %f)\n", position.x, position.y, position.z );
	NiPoint3 forward;
	forward = GetWorldForward();
	fprintf( file, "\tkForward = App.TGPoint3()\n" );
	fprintf( file, "\tkForward.SetXYZ(%f, %f, %f)\n", forward.x, forward.y, forward.z );
	NiPoint3 up;
	up = GetWorldUp();
	fprintf( file, "\tkUp = App.TGPoint3()\n" );
	fprintf( file, "\tkUp.SetXYZ(%f, %f, %f)\n", up.x, up.y, up.z );
	fprintf( file, "\tkThis.AlignToVectors(kForward, kUp)\n" );
	return true;
}

/*
================
PlacementObject::VirtualSlot70

Writes the closing lines of the placement in the saved script.
================
*/
// FUNCTION: STBC 0x004a1a20
bool PlacementObject::VirtualSlot70( FILE *file ) {
	fprintf( file, "\tkThis.Update(0)\n" );
	fprintf( file, "\tkThis = None\n" );
	fprintf( file, "\t# End position \"%s\"\n\n", GetName() );
	return true;
}

/*
================
PlacementObject::CreateModel

The placement's own model: an arrow along the model's forward axis, its
head in the right/forward plane and a fin up from it, six triangles in all,
coloured by the placement's kind and lit by its vertex colours alone.
================
*/
// FUNCTION: STBC 0x004a1a60
// PARKED: register: the texture loop's induction pointer is &textures[2].y in retail, &textures[0].y in ours (stride-3 index and component-wise stores tried); invariant under the declaration count (ctxscan)
NiTriangles *PlacementObject::CreateModel( void ) {
	NiPoint3 *vertices = new NiPoint3[18];
	vertices[0] = MODEL_FORWARD * 10.0f;
	vertices[1] = MODEL_RIGHT;
	vertices[2] = -MODEL_RIGHT;
	vertices[3] = -MODEL_RIGHT;
	vertices[4] = MODEL_RIGHT;
	vertices[5] = MODEL_UP * 3.0f;
	vertices[6] = -MODEL_RIGHT;
	vertices[7] = MODEL_UP * 3.0f;
	vertices[8] = MODEL_UP + MODEL_FORWARD;
	vertices[9] = MODEL_RIGHT;
	vertices[10] = MODEL_UP + MODEL_FORWARD;
	vertices[11] = MODEL_UP * 3.0f;
	vertices[12] = MODEL_RIGHT;
	vertices[13] = MODEL_FORWARD * 10.0f;
	vertices[14] = MODEL_UP + MODEL_FORWARD;
	vertices[15] = MODEL_FORWARD * 10.0f;
	vertices[16] = -MODEL_RIGHT;
	vertices[17] = MODEL_UP + MODEL_FORWARD;

	NiColorA *colors = new NiColorA[18];
	ApplyColors( colors, IsStatic() ? staticColors : placementColors );

	NiPoint2 *textures = new NiPoint2[18];
	for ( int i = 0; i < 6; i++ ) {
		textures[i * 3] = NiPoint2::ZERO;
		textures[i * 3 + 1].x = 1.0f;
		textures[i * 3 + 1].y = 1.0f;
		textures[i * 3 + 2] = NiPoint2::UNIT_Y;
	}

	NiTriangles *shape = new NiTriangles( vertices, NULL, colors, textures, 6 );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	shape->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAmbientColor( NiColor( 1.0f, 1.0f, 1.0f ) );
	material->SetDiffuseColor( NiColor( 1.0f, 1.0f, 1.0f ) );
	shape->AttachProperty( material );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( "data/Textures/White.tga", true, false ) );
	shape->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	shape->AttachProperty( textureMode );

	shape->UpdateProperties();
	return shape;
}

/*
================
PlacementObject::SetColors

Recolours the model's shape by part and marks its data changed (invented
name).
================
*/
// FUNCTION: STBC 0x004a2190
void PlacementObject::SetColors( const NiColorA *partColors ) {
	NiNode *model = NiDynamicCast( NiNode, niObject );
	NiTriangles *shape = NiDynamicCast( NiTriangles, model->GetAt( 0 ) );
	NiGeometryData *data = shape->modelData;
	ApplyColors( data->GetColors(), partColors );
	data->IncRevisionID();
}

/*
================
PlacementObject::ApplyColors

Gives the model's vertices their part's colour: the face table lists each
part's vertices, a part ending at -1 (invented name).
================
*/
// FUNCTION: STBC 0x004a2240
void PlacementObject::ApplyColors( NiColorA *colors, const NiColorA *partColors ) {
	int vertices[] = { 0, 13, 15, -1, 1, 2, 3, 4, 6, 8, 9, 10, 12, 14, 16, 17, -1, 5, 7, 11, -1 };
	int part = 0;
	int *vertex = vertices;
	do {
		if ( *vertex == -1 ) {
			part++;
		} else {
			colors[*vertex] = partColors[part];
		}
		vertex++;
	} while ( part < 3 );
}

/*
================
PlacementObject::FindContainingSet

The set whose placement list has the object; NULL when none has.
================
*/
// FUNCTION: STBC 0x004a2340
SetClass *PlacementObject::FindContainingSet( void ) {
	NiTMapIterator pos = placementsBySet.GetFirstPos();
	while ( pos != NULL ) {
		const char *setName;
		PlacementList *list;
		placementsBySet.GetNext( pos, setName, list );
		int i = list->GetCount() - 1;
		PlacementObject **objects = list->GetItems();
		for ( ; i >= 0; i-- ) {
			if ( objects[i] == this ) {
				return g_kSetManager.GetSet( (char *)setName );
			}
		}
	}
	return NULL;
}

/*
================
PlacementObject::GetObject

The placement of the name in the set, or in any set when the set is NULL.
================
*/
// FUNCTION: STBC 0x004a2430
PlacementObject *PlacementObject::GetObject( SetClass *set, char *name ) {
	if ( name == NULL ) {
		return NULL;
	}
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
				return objects[i];
			}
		}
	}
	return NULL;
}

/*
================
PlacementObject::GetObjectBySetName

The placement of the name in the set of that name.
================
*/
// FUNCTION: STBC 0x004a2540
PlacementObject *PlacementObject::GetObjectBySetName( char *setName, char *name ) {
	if ( name == NULL ) {
		return NULL;
	}
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
	return found;
}

/*
================
PlacementObject::AddToSet

Files the placement under its class type, then with the other objects of
the set.
================
*/
// FUNCTION: STBC 0x004a2610
void PlacementObject::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, PLACEMENTOBJECT_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
PlacementObject::RemoveFromSet
================
*/
// FUNCTION: STBC 0x004a2640
void PlacementObject::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, PLACEMENTOBJECT_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
PlacementObject::GetClassObjects

The set's placements and their number; NULL and 0 when the set is NULL or
has none.
================
*/
// FUNCTION: STBC 0x004a2670
PlacementObject **PlacementObject::GetClassObjects( SetClass *set, int *count ) {
	PlacementObject **objects = NULL;
	*count = 0;
	PlacementList *list;
	if ( set != NULL && placementsBySet.GetAt( set->GetName(), list ) ) {
		objects = list->GetItems();
		*count = list->GetCount();
	}
	return objects;
}

/*
================
PlacementObject::GetObjectByID

The placement with the ID in the set; NULL when the set is NULL or has none.
================
*/
// FUNCTION: STBC 0x004a26e0
PlacementObject *PlacementObject::GetObjectByID( SetClass *set, long id ) {
	PlacementObject *object = NULL;
	PlacementList *list;
	if ( set != NULL && placementsBySet.GetAt( set->GetName(), list ) ) {
		object = list->Get( id );
	}
	return object;
}

/*
================
PlacementObject::ShowPlacements

Puts every set's placements into the set and shows them.
================
*/
// FUNCTION: STBC 0x004a2790
void PlacementObject::ShowPlacements( void ) {
	NiTMapIterator pos = placementsBySet.GetFirstPos();
	while ( pos != NULL ) {
		const char *setName;
		PlacementList *list;
		placementsBySet.GetNext( pos, setName, list );
		SetClass *set = g_kSetManager.GetSet( (char *)setName );
		if ( set != NULL ) {
			PlacementObject **objects = list->GetItems();
			for ( int i = list->GetCount() - 1; i >= 0; i-- ) {
				PlacementObject *placement = objects[i];
				set->AddObjectToSet( placement, placement->GetName() );
				placement->SetHidden( false );
			}
		}
	}
	placementsShown = true;
}

/*
================
PlacementObject::HidePlacements

Takes the placements out of every set; nav points stay, hidden.
================
*/
// FUNCTION: STBC 0x004a2890
void PlacementObject::HidePlacements( void ) {
	int numSets = g_kSetManager.GetNumSets();
	SetClass **sets = g_kSetManager.GetSets();
	for ( int i = numSets - 1; i >= 0; i-- ) {
		SetClass *set = sets[i];
		int count = 0;
		PlacementObject **objects = GetClassObjects( set, &count );
		while ( --count >= 0 ) {
			PlacementObject *placement = objects[count];
			if ( placement->IsNavPoint() ) {
				placement->SetHidden( true );
			} else {
				set->RemoveObjectFromSet( placement->GetObjID() );
			}
		}
	}
	placementsShown = false;
}

/*
================
PlacementObject::TogglePlacements
================
*/
// FUNCTION: STBC 0x004a2930
void PlacementObject::TogglePlacements( void ) {
	if ( placementsShown ) {
		HidePlacements();
	} else {
		ShowPlacements();
	}
}

/*
================
PlacementObject::DeleteAll

Takes the placements out of the sets and deletes every one that is not a
nav point.
================
*/
// FUNCTION: STBC 0x004a2950
void PlacementObject::DeleteAll( void ) {
	HidePlacements();
	NiTMapIterator pos = placementsBySet.GetFirstPos();
	while ( pos != NULL ) {
		const char *setName;
		PlacementList *list;
		placementsBySet.GetNext( pos, setName, list );
		PlacementObject **objects = list->GetItems();
		for ( int i = list->GetCount() - 1; i >= 0; i-- ) {
			if ( !objects[i]->IsNavPoint() ) {
				delete objects[i];
			}
		}
	}
}

/*
================
PlacementObject::DeleteSetPlacements

The set is going away (SetManager::DeleteSet): the placements it holds
leave it through the set, the others are deleted, and the set's list
leaves the map.
================
*/
// FUNCTION: STBC 0x004a2a00
void PlacementObject::DeleteSetPlacements( SetClass *set ) {
	PlacementList *list;
	if ( !placementsBySet.GetAt( set->GetName(), list ) ) {
		return;
	}
	PlacementObject **objects = list->GetItems();
	for ( int i = list->GetCount() - 1; i >= 0; i-- ) {
		PlacementObject *object = objects[i];
		if ( object->containingSet != NULL ) {
			list->RemoveAt( i );
			set->DeleteObjectFromSet( object->GetObjID() );
		} else {
			delete object;
		}
	}
	placementsBySet.RemoveAt( set->GetName() );
	delete list;
}

/*
================
PlacementObject::SaveStatics

Writes whether the placements are shown, then every set's list: the set's
name and each placement that is not in a set (a placement in a set is
saved with the set), each after a true; a false ends the list. False when
a placement fails to write itself. The name is invented, after
LensFlare::SaveStatics.
================
*/
// FUNCTION: STBC 0x004a2be0
bool PlacementObject::SaveStatics( TGStream *stream ) {
	stream->WriteBool( placementsShown );
	stream->WriteInt( placementsBySet.GetCount() );
	NiTMapIterator pos = placementsBySet.GetFirstPos();
	while ( pos != NULL ) {
		const char *setName;
		PlacementList *list;
		placementsBySet.GetNext( pos, setName, list );
		stream->VirtualSlot11( (char *)setName );
		int count = list->GetCount();
		PlacementObject **objects = list->GetItems();
		for ( int i = 0; i < count; i++ ) {
			PlacementObject *placement = objects[i];
			if ( placement->GetContainingSet() == NULL ) {
				stream->WriteBool( true );
				if ( !placement->WriteToStream( stream ) ) {
					return false;
				}
			}
		}
		stream->WriteBool( false );
	}
	return true;
}

/*
================
PlacementObject::LoadStatics

Reads what SaveStatics wrote, filing each placement in its set's new list.
False when a placement fails to load or is not a placement. The name is
invented, after LensFlare::LoadStatics.
================
*/
// FUNCTION: STBC 0x004a2d00
bool PlacementObject::LoadStatics( TGStream *stream ) {
	placementsShown = stream->ReadBool();
	int count = stream->ReadInt();
	while ( --count >= 0 ) {
		char *setName = stream->VirtualSlot10( -1 );
		PlacementList *list = new PlacementList;
		placementsBySet.SetAt( setName, list );
		delete setName;
		while ( stream->ReadBool() ) {
			TGObject *object = TGObject::LoadObject( stream, 0 );
			if ( object == NULL || !object->IsTypeOf( PLACEMENTOBJECT_TYPE ) ) {
				return false;
			}
			list->Insert( (PlacementObject *)object );
		}
	}
	return true;
}

/*
================
PlacementObject::WriteToStream

The object, then the model name and the nav point flag.
================
*/
// FUNCTION: STBC 0x004a2f30
bool PlacementObject::WriteToStream( TGStream *stream ) {
	char text[256];

	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	NiPoint3 position = GetTranslate();
	char *model = modelName == NULL ? "(none)" : modelName;
	char *className = GetClassName();
	sprintf( text, "Saving ID %d: %s of name %s, (%f, %f, %f), model %s\n", GetObjID(), className, GetName(),
		position.x, position.y, position.z, model );
	TGObject::Log( text );
	stream->VirtualSlot11( modelName );
	stream->WriteBool( isStatic );
	stream->WriteBool( isNavPoint );
	return true;
}

/*
================
PlacementObject::ReadFromStream

The object, then the model name and the nav point flag.
================
*/
// FUNCTION: STBC 0x004a3000
bool PlacementObject::ReadFromStream( TGStream *stream ) {
	char text[256];

	modelName = NULL;
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	char *model = stream->VirtualSlot10( -1 );
	SetModel( model );
	delete model;
	isStatic = stream->ReadBool();
	isNavPoint = stream->ReadBool();
	NiPoint3 position = GetTranslate();
	char *shownModel = modelName == NULL ? "(none)" : modelName;
	char *className = GetClassName();
	sprintf( text, "Loaded %s of name %s, (%f, %f, %f), model %s\n", className, GetName(),
		position.x, position.y, position.z, shownModel );
	TGObject::Log( text );
	UpdateNodeOnly();
	return true;
}

/*
================
PlacementObject::FixUpStatics

Files the placements the sets loaded in their sets' lists. The name is
invented, after LensFlare::FixUpStatics.
================
*/
// FUNCTION: STBC 0x004a30f0
bool PlacementObject::FixUpStatics( void ) {
	SetClass **sets = g_kSetManager.GetSets();
	for ( int i = 0; i < g_kSetManager.GetNumSets(); i++ ) {
		SetClass *set = sets[i];
		int count = 0;
		PlacementObject **objects = (PlacementObject **)set->objectTable.GetObjects( PLACEMENTOBJECT_TYPE, &count );
		while ( --count >= 0 ) {
			PlacementObject *placement = objects[count];
			PlacementList *list;
			if ( !placementsBySet.GetAt( set->GetName(), list ) ) {
				list = new PlacementList;
				placementsBySet.SetAt( set->GetName(), list );
			}
			list->Insert( placement );
		}
	}
	return true;
}

/*
================
PlacementObject::SetStatic

The model, the placement's own unless a model is set, shows the kind.
================
*/
// FUNCTION: STBC 0x004a3360
void PlacementObject::SetStatic( bool isStatic_ ) {
	isStatic = isStatic_;
	if ( modelName != NULL ) {
		return;
	}
	if ( !isStatic_ ) {
		if ( isNavPoint ) {
			SetColors( navPointColors );
		} else {
			SetColors( placementColors );
		}
	} else {
		SetColors( staticColors );
	}
}

/*
================
PlacementObject::SetNavPoint

A nav point is always in its set, hidden; a plain placement is in it only
while the placements are shown. Tells the map the nav points changed.
================
*/
// FUNCTION: STBC 0x004a33a0
void PlacementObject::SetNavPoint( bool isNavPoint_ ) {
	if ( isNavPoint == isNavPoint_ ) {
		return;
	}
	isNavPoint = isNavPoint_;
	if ( isNavPoint_ ) {
		if ( containingSet == NULL ) {
			SetClass *set = FindContainingSet();
			if ( set != NULL ) {
				set->AddObjectToSet( this, name );
			}
			if ( node != NULL ) {
				node->SetAppCulled( true );
			}
		}
		SetColors( navPointColors );
	} else {
		if ( containingSet != NULL && !placementsShown ) {
			containingSet->RemoveObjectFromSet( GetObjID() );
		}
		if ( node != NULL ) {
			node->SetAppCulled( false );
		}
		if ( isStatic ) {
			SetColors( staticColors );
		} else {
			SetColors( placementColors );
		}
	}
	TGEvent *event = new TGEvent;
	event->SetEventType( ET_NAV_POINT_CHANGED );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );
}
