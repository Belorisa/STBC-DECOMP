/*
===============================================================================

	GridClass.cpp

===============================================================================
*/

#include <iostream>
#include "GridClass.h"
#include "TGEventHandlerTable.h"
#include "NiMatrix3.h"
#include "SetClass.h"
#include "SetManager.h"
#include "CameraObjectClass.h"
#include "TGStream.h"
#include "ImageManagerClass.h"
#include "NiLines.h"
#include "NiLinesData.h"
#include "NiBound.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00499d90 _$E453
// FUNCTION: STBC 0x00499db0 _$E452

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00499730 _$E32
// FUNCTION: STBC 0x00499740 _$E29
// FUNCTION: STBC 0x00499750 _$E31
// FUNCTION: STBC 0x00499760 _$E30
// FUNCTION: STBC 0x00499770 _$E38
// FUNCTION: STBC 0x00499780 _$E35
// FUNCTION: STBC 0x00499790 _$E37
// FUNCTION: STBC 0x004997a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004997b0 _$E66
// FUNCTION: STBC 0x004997c0 _$E65
// FUNCTION: STBC 0x004997d0 _$E254
// FUNCTION: STBC 0x004997e0 _$E253
// FUNCTION: STBC 0x004997f0 _$E77
// FUNCTION: STBC 0x00499800 _$E76
// FUNCTION: STBC 0x00499810 _$E81
// FUNCTION: STBC 0x00499820 _$E80
// FUNCTION: STBC 0x00499830 _$E85
// FUNCTION: STBC 0x00499840 _$E84
// FUNCTION: STBC 0x00499850 _$E89
// FUNCTION: STBC 0x00499860 _$E88
// FUNCTION: STBC 0x00499870 _$E206
// FUNCTION: STBC 0x00499880 _$E205
// FUNCTION: STBC 0x00499890 _$E148
// FUNCTION: STBC 0x004998a0 _$E147
// FUNCTION: STBC 0x004998b0 _$E161
// FUNCTION: STBC 0x004998c0 _$E160
// FUNCTION: STBC 0x004998d0 _$E211
// FUNCTION: STBC 0x004998e0 _$E210
// FUNCTION: STBC 0x004998f0 _$E259
// FUNCTION: STBC 0x00499900 _$E258
// FUNCTION: STBC 0x00499910 _$E215
// FUNCTION: STBC 0x00499920 _$E214
// FUNCTION: STBC 0x00499930 _$E263
// FUNCTION: STBC 0x00499940 _$E262
// FUNCTION: STBC 0x00499950 _$E270
// FUNCTION: STBC 0x00499960 _$E269
// FUNCTION: STBC 0x00499970 _$E280
// FUNCTION: STBC 0x00499980 _$E279
// FUNCTION: STBC 0x00499990 _$E284
// FUNCTION: STBC 0x004999a0 _$E283
// FUNCTION: STBC 0x004999b0 _$E106
// FUNCTION: STBC 0x004999c0 _$E105
// FUNCTION: STBC 0x004999d0 _$E235
// FUNCTION: STBC 0x004999e0 _$E234
// FUNCTION: STBC 0x004999f0 _$E242
// FUNCTION: STBC 0x00499a00 _$E241
// FUNCTION: STBC 0x00499a10 _$E296
// FUNCTION: STBC 0x00499a20 _$E295
// FUNCTION: STBC 0x00499a30 _$E300
// FUNCTION: STBC 0x00499a40 _$E299
// FUNCTION: STBC 0x00499a50 _$E340
// FUNCTION: STBC 0x00499a60 _$E339
// FUNCTION: STBC 0x00499a70 _$E346
// FUNCTION: STBC 0x00499a80 _$E345
// FUNCTION: STBC 0x00499a90 _$E288
// FUNCTION: STBC 0x00499aa0 _$E287
// FUNCTION: STBC 0x00499ab0 _$E292
// FUNCTION: STBC 0x00499ac0 _$E291
// FUNCTION: STBC 0x00499ad0 _$E304
// FUNCTION: STBC 0x00499ae0 _$E303
// FUNCTION: STBC 0x00499af0 _$E308
// FUNCTION: STBC 0x00499b00 _$E307
// FUNCTION: STBC 0x00499b10 _$E312
// FUNCTION: STBC 0x00499b20 _$E311
// FUNCTION: STBC 0x00499b30 _$E316
// FUNCTION: STBC 0x00499b40 _$E315
// FUNCTION: STBC 0x00499b50 _$E320
// FUNCTION: STBC 0x00499b60 _$E319
// FUNCTION: STBC 0x00499b70 _$E324
// FUNCTION: STBC 0x00499b80 _$E323
// FUNCTION: STBC 0x00499b90 _$E328
// FUNCTION: STBC 0x00499ba0 _$E327
// FUNCTION: STBC 0x00499bb0 _$E332
// FUNCTION: STBC 0x00499bc0 _$E331
// FUNCTION: STBC 0x00499bd0 _$E111
// FUNCTION: STBC 0x00499be0 _$E110
// FUNCTION: STBC 0x00499bf0 _$E336
// FUNCTION: STBC 0x00499c00 _$E335
// FUNCTION: STBC 0x00499c10 _$E116
// FUNCTION: STBC 0x00499c20 _$E115
// FUNCTION: STBC 0x00499c30 _$E350
// FUNCTION: STBC 0x00499c40 _$E349
// FUNCTION: STBC 0x00499c50 _$E354
// FUNCTION: STBC 0x00499c60 _$E353
// FUNCTION: STBC 0x00499c70 _$E358
// FUNCTION: STBC 0x00499c80 _$E357
// FUNCTION: STBC 0x00499c90 _$E362
// FUNCTION: STBC 0x00499ca0 _$E361
// FUNCTION: STBC 0x00499cb0 _$E125
// FUNCTION: STBC 0x00499cc0 _$E124
// FUNCTION: STBC 0x00499cd0 _$E246
// FUNCTION: STBC 0x00499ce0 _$E245
// FUNCTION: STBC 0x00499cf0 _$E250
// FUNCTION: STBC 0x00499d00 _$E249
// FUNCTION: STBC 0x00499d10 _$E366
// FUNCTION: STBC 0x00499d20 _$E365
// FUNCTION: STBC 0x00499d30 _$E131
// FUNCTION: STBC 0x00499d40 _$E130
// FUNCTION: STBC 0x00499d50 _$E136
// FUNCTION: STBC 0x00499d60 _$E135
// FUNCTION: STBC 0x00499d70 _$E166
// FUNCTION: STBC 0x00499d80 _$E165
// FUNCTION: STBC 0x00499dc0 _$E370
// FUNCTION: STBC 0x00499dd0 _$E369
// FUNCTION: STBC 0x00499de0 _$E171
// FUNCTION: STBC 0x00499df0 _$E170
// FUNCTION: STBC 0x00499e00 _$E374
// FUNCTION: STBC 0x00499e10 _$E373
// FUNCTION: STBC 0x00499e20 _$E378
// FUNCTION: STBC 0x00499e30 _$E377
// FUNCTION: STBC 0x00499e40 _$E176
// FUNCTION: STBC 0x00499e50 _$E175
// FUNCTION: STBC 0x00499e60 _$E382
// FUNCTION: STBC 0x00499e70 _$E381
// FUNCTION: STBC 0x00499e80 _$E141
// FUNCTION: STBC 0x00499e90 _$E140
// FUNCTION: STBC 0x00499ea0 _$E386
// FUNCTION: STBC 0x00499eb0 _$E385
// FUNCTION: STBC 0x00499ec0 _$E390
// FUNCTION: STBC 0x00499ed0 _$E389
// FUNCTION: STBC 0x00499ee0 _$E181
// FUNCTION: STBC 0x00499ef0 _$E180
// FUNCTION: STBC 0x00499f00 _$E186
// FUNCTION: STBC 0x00499f10 _$E185
// FUNCTION: STBC 0x00499f20 _$E394
// FUNCTION: STBC 0x00499f30 _$E393
// FUNCTION: STBC 0x00499f40 _$E398
// FUNCTION: STBC 0x00499f50 _$E397
// FUNCTION: STBC 0x00499f60 _$E402
// FUNCTION: STBC 0x00499f70 _$E401
// FUNCTION: STBC 0x00499f80 _$E406
// FUNCTION: STBC 0x00499f90 _$E405
// FUNCTION: STBC 0x00499fa0 _$E410
// FUNCTION: STBC 0x00499fb0 _$E409
// FUNCTION: STBC 0x00499fc0 _$E414
// FUNCTION: STBC 0x00499fd0 _$E413
// FUNCTION: STBC 0x00499fe0 _$E418
// FUNCTION: STBC 0x00499ff0 _$E417
// FUNCTION: STBC 0x0049a000 _$E422
// FUNCTION: STBC 0x0049a010 _$E421
// FUNCTION: STBC 0x0049a020 _$E191
// FUNCTION: STBC 0x0049a030 _$E190
// FUNCTION: STBC 0x0049a040 _$E426
// FUNCTION: STBC 0x0049a050 _$E425
// FUNCTION: STBC 0x0049a060 _$E196
// FUNCTION: STBC 0x0049a070 _$E195
// FUNCTION: STBC 0x0049a080 _$E201
// FUNCTION: STBC 0x0049a090 _$E200
// FUNCTION: STBC 0x0049a0a0 _$E430
// FUNCTION: STBC 0x0049a0b0 _$E429
// FUNCTION: STBC 0x0049a0c0 _$E435
// FUNCTION: STBC 0x0049a0d0 _$E434
// FUNCTION: STBC 0x0049a0e0 _$E439
// FUNCTION: STBC 0x0049a0f0 _$E438

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x00498c50 _$E445
// FUNCTION: STBC 0x00498c60 _$E441
// FUNCTION: STBC 0x00498c70 _$E444
// FUNCTION: STBC 0x00498c80 _$E442
// DATA: STBC 0x009832e8
TGEventHandlerTable GridClass::classHandlers;

/*
================
GridClass::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00498ca0
void GridClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, GRIDCLASS_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
GridClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00498cd0
void GridClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, GRIDCLASS_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
GridClass::GetClassObjects

The set's grids and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x00498d00
GridClass **GridClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (GridClass **)set->objectTable.GetObjects( GRIDCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
GridClass::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x00498d30
int GridClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( GRIDCLASS_TYPE );
	}
	return 0;
}

/*
================
GridClass::GetObjectByID

The grid with the ID in the set, or among every object when the set is
NULL; NULL when it is not a grid.
================
*/
// FUNCTION: STBC 0x00498d50
GridClass *GridClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( GRIDCLASS_TYPE ) ) {
			return (GridClass *)object;
		}
		return NULL;
	}
	return (GridClass *)set->GetClassObject( GRIDCLASS_TYPE, id );
}

/*
================
GridClass::GetObject

The object of the class and name in the set, or in any set when the set
is NULL, the last set first.
================
*/
// FUNCTION: STBC 0x00498dc0
GridClass *GridClass::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( GRIDCLASS_TYPE, name );
		}
		return (GridClass *)object;
	}
	return (GridClass *)set->GetClassObject( GRIDCLASS_TYPE, name );
}

/*
================
GridClass::GridClass

A grid of 1250-unit lines, 5 apart; neither hailable nor scannable, and
it collides with nothing.
================
*/
// FUNCTION: STBC 0x00498e20
GridClass::GridClass( long id ) : ObjectClass( id ) {
	collisionFlags = ObjectClass::CFB_NO_COLLISIONS;
	lineLength = 1250;
	step = 5;
	SetHailable( false );
	SetScannable( false );
}

/*
================
GridClass::GetObjType
================
*/
// FUNCTION: STBC 0x00498e90
int GridClass::GetObjType( void ) {
	return GRIDCLASS_TYPE;
}

/*
================
GridClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x00498ea0
bool GridClass::IsTypeOf( int type ) {
	if ( type == GRIDCLASS_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

/*
================
GridClass::GetClassName
================
*/
// FUNCTION: STBC 0x00498f00
char *GridClass::GetClassName( void ) {
	return "GridClass";
}

/*
================
GridClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00498f10
char *GridClass::GetSwigTypeName( void ) {
	return "_p_GridClass";
}

/*
================
GridClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00498f20
char *GridClass::GetPythonClassName( void ) {
	return "GridClassPtr";
}

/*
================
GridClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00498f30
TGEventHandlerTable *GridClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
GridClass::~GridClass
================
*/
// FUNCTION: STBC 0x00498f70
GridClass::~GridClass( void ) {
}

/*
================
GridClass::Create

A new grid whose step makes its 125 lines span the line length.
================
*/
// FUNCTION: STBC 0x00498f80
GridClass *GridClass::Create( void ) {
	GridClass *grid = new GridClass;
	grid->step = grid->lineLength / 125;
	grid->Unknown498FD0();
	return grid;
}

/*
================
GridClass::Unknown498FD0

Builds the grid's lines: 125 pairs of lines lineLength long, one along X
and one along Y, step apart, white and untextured, as the object's model.
================
*/
// FUNCTION: STBC 0x00498fd0
void GridClass::Unknown498FD0( void ) {
	NiPoint3 *vertices = new NiPoint3[500];
	NiPoint2 *textures = new NiPoint2[500];
	NiColorA *colors = new NiColorA[500];
	bool *flags = new bool[500];

	float x = 0.0f;
	float y = 0.0f;
	for ( int i = 0; i < 125; i++ ) {
		vertices[i * 4].x = 0.0f;
		vertices[i * 4].y = y;
		vertices[i * 4].z = 0.0f;
		vertices[i * 4 + 1].x = (float)GetLineLength();
		vertices[i * 4 + 1].y = y;
		vertices[i * 4 + 1].z = 0.0f;
		vertices[i * 4 + 2].x = x;
		vertices[i * 4 + 2].y = 0.0f;
		vertices[i * 4 + 2].z = 0.0f;
		vertices[i * 4 + 3].x = x;
		vertices[i * 4 + 3].y = (float)GetLineLength();
		vertices[i * 4 + 3].z = 0.0f;

		flags[i * 4] = true;
		flags[i * 4 + 1] = false;
		flags[i * 4 + 2] = true;
		flags[i * 4 + 3] = false;

		colors[i * 4].r = 1.0f;
		colors[i * 4].g = 1.0f;
		colors[i * 4].b = 1.0f;
		colors[i * 4 + 1].r = 1.0f;
		colors[i * 4 + 1].g = 1.0f;
		colors[i * 4 + 1].b = 1.0f;
		colors[i * 4 + 2].r = 1.0f;
		colors[i * 4 + 2].g = 1.0f;
		colors[i * 4 + 2].b = 1.0f;
		colors[i * 4 + 3].r = 1.0f;
		colors[i * 4 + 3].g = 1.0f;
		colors[i * 4 + 3].b = 1.0f;

		textures[i * 4].x = 0.0f;
		textures[i * 4].y = 0.0f;
		textures[i * 4 + 1].x = 0.0f;
		textures[i * 4 + 1].y = 0.0f;
		textures[i * 4 + 2].x = 0.0f;
		textures[i * 4 + 2].y = 0.0f;
		textures[i * 4 + 3].x = 0.0f;
		textures[i * 4 + 3].y = 0.0f;

		x += GetStep();
		y += GetStep();
	}

	NiLines *lines = new NiLines( 500, vertices, colors, textures, flags );
	SetNiObject( lines );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	lines->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAmbientColor( NiColor( 1.0f, 1.0f, 1.0f ) );
	material->SetDiffuseColor( NiColor( 1.0f, 1.0f, 1.0f ) );
	lines->AttachProperty( material );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( "data/Textures/White.tga", true, true ) );
	lines->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	lines->AttachProperty( textureMode );

	lines->UpdateProperties();
}

/*
================
GridClass::Update

Follows the containing set's active camera.
================
*/
// FUNCTION: STBC 0x00499380
void GridClass::Update( float time ) {
	if ( containingSet != NULL ) {
		CameraObjectClass *camera = containingSet->GetActiveCamera();
		if ( camera != NULL ) {
			// UpdatePosition takes a non-const point (its SWIG signature)
			UpdatePosition( (NiPoint3 *)&camera->GetWorldLocation() );
		}
	}
	ObjectClass::Update( time );
}

/*
================
GridClass::GetLines
================
*/
// FUNCTION: STBC 0x004993c0
NiLines *GridClass::GetLines( void ) {
	return NiDynamicCast( NiLines, GetNiObject() );
}

/*
================
GridClass::UpdatePosition

Moves the lines to the step nearest below the point, the grid centered on
it, with a bound of radius 50 around the point.
================
*/
// FUNCTION: STBC 0x00499400
// DRAFT: calls, loop and stores agree; register and x87 schedule: retail keeps the step and length in EDI and ESI, the vertex count in EBP, subtracts the x remainder with fisub and keeps the start corner on the FPU through the loop
void GridClass::UpdatePosition( NiPoint3 *position ) {
	NiLines *lines = GetLines();
	NiLinesData *data = lines->GetLinesData();
	int stepSize = step;
	int length = lineLength;
	int count = data->GetVertexCount();
	NiPoint3 *vertices = data->GetVertices();
	float px = position->x;
	float half = length / 2;
	float startX = px - (int)px % stepSize - half;
	float startY = position->y - (int)position->y % stepSize - half;
	float x = startX;
	float y = startY;
	float endX = startX + length;
	float endY = startY + length;
	float increment = stepSize;
	for ( int i = 0; i < count / 4; i++ ) {
		vertices[0] = NiPoint3( startX, y, 0.0f );
		vertices[1] = NiPoint3( endX, y, 0.0f );
		vertices[2] = NiPoint3( x, startY, 0.0f );
		vertices[3] = NiPoint3( x, endY, 0.0f );
		vertices += 4;
		x += increment;
		y += increment;
	}

	NiBound bound;
	bound.SetCenter( *position );
	bound.SetRadius( 50.0f );
	lines->GetLinesData()->GetBound() = bound;
}

/*
================
GridClass::Unknown499590

TODO identity: where the ray from the origin along the direction meets the
grid's plane; the rotation's third column is the plane's normal.
================
*/
// FUNCTION: STBC 0x00499590 ?Unknown499590@GridClass@@QAE?AVNiPoint3@@V2@0@Z
// PARKED: x87 order: retail multiplies the center (held on the FPU) by the normal's components from memory in the plane's dot product, and builds the result's z in a local copied to the return slot
NiPoint3 GridClass::Unknown499590( NiPoint3 origin, NiPoint3 direction ) {
	NiPoint3 normal( 0.0f, 0.0f, 1.0f );
	normal = normal * NiMatrix3( GetWorldRotation() );
	NiPoint3 center = GetWorldLocation();
	float planeDistance = -( normal * center );
	float denominator = planeDistance + normal * direction;
	if ( denominator == 0.0f ) {
		denominator = 1e-06f;
	}
	float t = -( normal * origin ) / denominator;
	return direction * t + origin;
}

/*
================
GridClass::WriteToStream
================
*/
// FUNCTION: STBC 0x004996b0
bool GridClass::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( lineLength );
	stream->WriteLong( step );
	return true;
}

/*
================
GridClass::ReadFromStream

Reads the line length and step and builds the lines again.
================
*/
// FUNCTION: STBC 0x004996f0
bool GridClass::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	lineLength = stream->ReadLong();
	step = stream->ReadLong();
	Unknown498FD0();
	return true;
}
