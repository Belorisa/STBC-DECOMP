/*
===============================================================================

	BackdropSphere.cpp

===============================================================================
*/

#include "BackdropSphere.h"
#include "NiAlphaProperty.h"
#include "NiVertexColorProperty.h"
#include "NiTextureProperty.h"
#include "NiTextureModeProperty.h"
#include "NiMaterialProperty.h"
#include "NiMultiTextureProperty.h"
#include "NiTriShape.h"
#include "NiMath.h"
#include <math.h>
#include "TGStream.h"
#include "ImageManagerClass.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

/*
================
BackdropSphere::AddToSet

Adds to the set's list of the class, then of the base class.
================
*/
// FUNCTION: STBC 0x0058d090
void BackdropSphere::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, BACKDROPSPHERE_TYPE );
		Backdrop::AddToSet( set );
	}
}

/*
================
BackdropSphere::RemoveFromSet
================
*/
// FUNCTION: STBC 0x0058d0c0
void BackdropSphere::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, BACKDROPSPHERE_TYPE );
		Backdrop::RemoveFromSet( set );
	}
}

/*
================
BackdropSphere::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x0058d120
int BackdropSphere::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( BACKDROPSPHERE_TYPE );
	}
	return 0;
}

/*
================
BackdropSphere::GetObjectByID

The object of the class with the given ID in the set, or among every
object when the set is NULL; NULL when it is none or of another class.
================
*/
// FUNCTION: STBC 0x0058d140
BackdropSphere *BackdropSphere::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( BACKDROPSPHERE_TYPE ) ) {
			return (BackdropSphere *)object;
		}
		return NULL;
	}
	return (BackdropSphere *)set->GetClassObject( BACKDROPSPHERE_TYPE, id );
}

/*
================
BackdropSphere::GetObject

The object of the class and name in the set, or in any set when the set
is NULL, the last set first.
================
*/
// FUNCTION: STBC 0x0058d1b0
BackdropSphere *BackdropSphere::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( BACKDROPSPHERE_TYPE, name );
		}
		return (BackdropSphere *)object;
	}
	return (BackdropSphere *)set->GetClassObject( BACKDROPSPHERE_TYPE, name );
}

/*
================
BackdropSphere::Cast

The object as a BackdropSphere; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0058d210
BackdropSphere *BackdropSphere::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( BACKDROPSPHERE_TYPE ) ) {
		return (BackdropSphere *)object;
	}
	return NULL;
}

/*
================
BackdropSphere::Create
================
*/
// FUNCTION: STBC 0x0058d270
BackdropSphere *BackdropSphere::Create( void ) {
	return new BackdropSphere;
}

/*
================
BackdropSphere::BackdropSphere

Spans, tiles, radius and polygon count at their defaults.
================
*/
// FUNCTION: STBC 0x0058d2a0
BackdropSphere::BackdropSphere( long id ) : Backdrop( id ) {
	textureFileName = NULL;
	detailTextureFileName = NULL;
	horizontalSpan = 0.25f;
	verticalSpan = 0.5f;
	textureHTile = 1.0f;
	textureVTile = 1.0f;
	detailTextureHTile = 10.0f;
	detailTextureVTile = 10.0f;
	sphereRadius = 300.0f;
	decimate = false;
	srcBlendMode = 0;
	destBlendMode = 0;
	targetPolyCount = 256;
}

/*
================
BackdropSphere::GetClassName
================
*/
// FUNCTION: STBC 0x0058d390
char *BackdropSphere::GetClassName( void ) {
	return "BackdropSphere";
}

/*
================
BackdropSphere::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0058d3a0
char *BackdropSphere::GetSwigTypeName( void ) {
	return "_p_BackdropSphere";
}

/*
================
BackdropSphere::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0058d3b0
char *BackdropSphere::GetPythonClassName( void ) {
	return "BackdropSpherePtr";
}

/*
================
BackdropSphere::~BackdropSphere

Frees the file names; the texture is released with the member.
================
*/
// FUNCTION: STBC 0x0058d3f0
BackdropSphere::~BackdropSphere( void ) {
	delete [] textureFileName;
	delete [] detailTextureFileName;
}

/*
================
BackdropSphere::SetDetailTextureFileName

The call does nothing.
================
*/
// FUNCTION: STBC 0x0058d470
void BackdropSphere::SetDetailTextureFileName( char * ) {
}

/*
================
BackdropSphere::SetDecimate

The call does nothing.
================
*/
// FUNCTION: STBC 0x0058d480
void BackdropSphere::SetDecimate( bool ) {
}

/*
================
BackdropSphere::SetDetailTextureHTile
================
*/
// FUNCTION: STBC 0x0058d490
void BackdropSphere::SetDetailTextureHTile( float tile ) {
	detailTextureHTile = tile;
}

/*
================
BackdropSphere::SetDetailTextureVTile
================
*/
// FUNCTION: STBC 0x0058d4a0
void BackdropSphere::SetDetailTextureVTile( float tile ) {
	detailTextureVTile = tile;
}

/*
================
BackdropSphere::SetTargetPolyCount
================
*/
// FUNCTION: STBC 0x0058d4b0
void BackdropSphere::SetTargetPolyCount( int count ) {
	targetPolyCount = count;
}

/*
================
BackdropSphere::GetHorizontalSpan
================
*/
// FUNCTION: STBC 0x0058d4c0
float BackdropSphere::GetHorizontalSpan( void ) {
	return horizontalSpan;
}

/*
================
BackdropSphere::GetVerticalSpan
================
*/
// FUNCTION: STBC 0x0058d4d0
float BackdropSphere::GetVerticalSpan( void ) {
	return verticalSpan;
}

/*
================
BackdropSphere::GetTextureFileName
================
*/
// FUNCTION: STBC 0x0058d4e0
char *BackdropSphere::GetTextureFileName( void ) {
	return textureFileName;
}

/*
================
BackdropSphere::GetTextureHTile
================
*/
// FUNCTION: STBC 0x0058d4f0
float BackdropSphere::GetTextureHTile( void ) {
	return textureHTile;
}

/*
================
BackdropSphere::GetTextureVTile
================
*/
// FUNCTION: STBC 0x0058d500
float BackdropSphere::GetTextureVTile( void ) {
	return textureVTile;
}

/*
================
BackdropSphere::GetDetailTextureFileName
================
*/
// FUNCTION: STBC 0x0058d510
char *BackdropSphere::GetDetailTextureFileName( void ) {
	return detailTextureFileName;
}

/*
================
BackdropSphere::GetDetailTextureHTile
================
*/
// FUNCTION: STBC 0x0058d520
float BackdropSphere::GetDetailTextureHTile( void ) {
	return detailTextureHTile;
}

/*
================
BackdropSphere::GetDetailTextureVTile
================
*/
// FUNCTION: STBC 0x0058d530
float BackdropSphere::GetDetailTextureVTile( void ) {
	return detailTextureVTile;
}

/*
================
BackdropSphere::GetDecimate
================
*/
// FUNCTION: STBC 0x0058d540
bool BackdropSphere::GetDecimate( void ) {
	return decimate;
}

/*
================
BackdropSphere::GetTargetPolyCount
================
*/
// FUNCTION: STBC 0x0058d550
int BackdropSphere::GetTargetPolyCount( void ) {
	return targetPolyCount;
}

/*
================
BackdropSphere::GetSphereRadius
================
*/
// FUNCTION: STBC 0x0058d560
float BackdropSphere::GetSphereRadius( void ) {
	return sphereRadius;
}

/*
================
BackdropSphere::SetSphereRadius
================
*/
// FUNCTION: STBC 0x0058d570
void BackdropSphere::SetSphereRadius( float radius ) {
	sphereRadius = radius;
}

/*
================
BackdropSphere::SetTextureFileName

Loads the texture of the name and keeps a copy of the name.
================
*/
// FUNCTION: STBC 0x0058d580
void BackdropSphere::SetTextureFileName( char *name ) {
	image = g_kImageManager.GetImage( name, true, false );
	delete [] textureFileName;
	textureFileName = new char[strlen( name ) + 1];
	strcpy( textureFileName, name );
}

/*
================
BackdropSphere::Rebuild

Makes the node again, once the sphere has been set up and decimate
is clear.
================
*/
// FUNCTION: STBC 0x0058d610
void BackdropSphere::Rebuild( void ) {
	if ( image != NULL && !decimate ) {
		NiPointer<NiAVObject> newObject = VirtualSlot71();
		SetNiObject( newObject );
	}
}

/*
================
BackdropSphere::GetGridSize
================
*/
// FUNCTION: STBC 0x0058d6a0
void BackdropSphere::GetGridSize( int *across, int *down ) {
	if ( decimate ) {
		*across = image->GetWidth();
		*down = image->GetHeight();
		return;
	}
	unsigned int halfCount = (unsigned int)targetPolyCount >> 1;
	float root = (float)sqrt( (double)halfCount ) + 1.0f;
	float aspect = (float)image->GetWidth() / image->GetHeight();
	*across = (int)( root / aspect );
	*down = (int)( root * aspect );
}

/*
================
BackdropSphere::SetHorizontalSpan

The span is limited to 1.
================
*/
// FUNCTION: STBC 0x0058d730
void BackdropSphere::SetHorizontalSpan( float span ) {
	horizontalSpan = span;
	// BUG: the lower limit stores the value the member already holds.
	if ( span > 0.0f ) {
		horizontalSpan = span;
	}
	if ( span > 1.0f ) {
		horizontalSpan = 1.0f;
	}
}

/*
================
BackdropSphere::SetVerticalSpan

The span is limited to 1.
================
*/
// FUNCTION: STBC 0x0058d770
void BackdropSphere::SetVerticalSpan( float span ) {
	verticalSpan = span;
	// BUG: the lower limit stores the value the member already holds.
	if ( span > 0.0f ) {
		verticalSpan = span;
	}
	if ( span > 1.0f ) {
		verticalSpan = 1.0f;
	}
}

/*
================
BackdropSphere::SetTextureHTile
================
*/
// FUNCTION: STBC 0x0058d7b0
void BackdropSphere::SetTextureHTile( float tile ) {
	textureHTile = tile;
}

/*
================
BackdropSphere::SetTextureVTile
================
*/
// FUNCTION: STBC 0x0058d7c0
void BackdropSphere::SetTextureVTile( float tile ) {
	textureVTile = tile;
}

/*
================
BackdropSphere::VirtualSlot67

The first lines of the sphere's script: its name and its creation.
================
*/
// FUNCTION: STBC 0x0058d7d0
bool BackdropSphere::VirtualSlot67( FILE *file ) {
	fprintf( file, "\t# Backdrop Sphere \"%s\"\n", name );
	fprintf( file, "\tkThis = App.BackdropSphere_Create()\n" );
	return true;
}

/*
================
BackdropSphere::VirtualSlot68

The lines that give the sphere its texture, polygon count, spans, radius and
tiles, after the backdrop's own. False when the sphere has no texture.
================
*/
// FUNCTION: STBC 0x0058d800
bool BackdropSphere::VirtualSlot68( FILE *file ) {
	Backdrop::VirtualSlot68( file );
	if ( GetTextureFileName() == NULL ) {
		return false;
	}
	fprintf( file, "\tkThis.SetTextureFileName(\"%s\")\n", GetTextureFileName() );
	if ( GetDecimate() ) {
		fprintf( file, "\tkThis.SetDecimate(1)\n" );
	}
	fprintf( file, "\tkThis.SetTargetPolyCount(%i)\n", GetTargetPolyCount() );
	fprintf( file, "\tkThis.SetHorizontalSpan(%f)\n", GetHorizontalSpan() );
	fprintf( file, "\tkThis.SetVerticalSpan(%f)\n", GetVerticalSpan() );
	fprintf( file, "\tkThis.SetSphereRadius(%f)\n", GetSphereRadius() );
	fprintf( file, "\tkThis.SetTextureHTile(%f)\n", GetTextureHTile() );
	fprintf( file, "\tkThis.SetTextureVTile(%f)\n", GetTextureVTile() );
	if ( GetDetailTextureFileName() != NULL ) {
		fprintf( file, "\tkThis.SetDetailTextureFileName(\"%s\")\n", GetDetailTextureFileName() );
		fprintf( file, "\tkThis.SetDetailTextureHTile(%f)\n", GetDetailTextureHTile() );
		fprintf( file, "\tkThis.SetDetailTextureVTile(%f)\n", GetDetailTextureVTile() );
	}
	return true;
}

/*
================
BackdropSphere::VirtualSlot69

The lines that put the sphere in the set, rebuild it and close its script.
================
*/
// FUNCTION: STBC 0x0058d950
bool BackdropSphere::VirtualSlot69( FILE *file ) {
	fprintf( file, "\tkThis.Rebuild()\n" );
	fprintf( file, "\tpSet.AddBackdropToSet(kThis,\"%s\")\n", name );
	fprintf( file, "\tkThis.Update(0)\n" );
	fprintf( file, "\tkThis = None\n" );
	fprintf( file, "\t# End Backdrop Sphere \"%s\"\n\n", name );
	return true;
}

/*
================
BackdropSphere::SetAlphaBlendModes

Keeps the modes, and sets them in the node's alpha property.
================
*/
// FUNCTION: STBC 0x0058d9b0
void BackdropSphere::SetAlphaBlendModes( int srcMode, int destMode ) {
	srcBlendMode = srcMode;
	destBlendMode = destMode;
	if ( niObject != NULL ) {
		NiAlphaProperty *alpha = NiDynamicCast( NiAlphaProperty, niObject->GetProperty( NiAlphaProperty::ALPHA ) );
		if ( alpha ) {
			alpha->SetSrcBlendMode( (NiAlphaProperty::AlphaFunction)srcBlendMode );
			alpha->SetDestBlendMode( (NiAlphaProperty::AlphaFunction)destBlendMode );
		}
	}
}

/*
================
BackdropSphere::VirtualSlot71

Builds the backdrop's mesh: the part of a sphere the spans cover, centred
on the horizon, at the sphere radius and facing inward through its normals,
with the texture tiled across it. Unlit, blended by the alpha modes,
unsorted. A detail texture adds a second map over a copy of the texture
coordinates scaled by the detail tiling.
================
*/
// FUNCTION: STBC 0x0058db00
// PARKED: three residues left: the normals' stores are [index + normals] against retail's [normals + index] (the global optimiser's base/index rewrite, C2 0x1071F93A; StarSphere's identical loop keeps [index + normals]); the detail loop's tile * x loads the coordinate first (operand sort by symbol number); and phiStep's fidiv is scheduled after down - 1's lea (the fmul's 3-cycle latency leaves the lea alone at c16, retail needs the lea ready no earlier than c18)
NiPointer<NiAVObject> BackdropSphere::VirtualSlot71( void ) {
	int across;
	int down;
	GetGridSize( &across, &down );
	int vertexCount = across * down;
	int triangleCount = ( 2 * across - 2 ) * ( down - 1 );

	NiPoint3 *vertices = new NiPoint3[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	NiPoint3 *normals = new NiPoint3[vertexCount];
	unsigned short *triList = new unsigned short[triangleCount * 3];

	float verticalAngle = NI_PI * verticalSpan;
	float theta = NI_HALF_PI - verticalAngle * 0.5f;
	float phiStep = NI_TWO_PI * horizontalSpan / ( across - 1 );
	float thetaStep = verticalAngle / ( down - 1 );
	int triangle = 0;
	for ( int row = 0; row < down; row++ ) {
		float phi = -( NI_TWO_PI * horizontalSpan * 0.5f );
		float v = (float)row / ( down - 1 ) * textureVTile;
		float cosTheta = (float)cos( theta );
		float sinTheta = (float)sin( theta );
		for ( int column = 0; column < across; column++ ) {
			float sinPhi = (float)sin( phi );
			int index = row * across + column;
			normals[index] = NiPoint3( sinPhi * sinTheta, (float)cos( phi ) * sinTheta, cosTheta );
			vertices[index] = sphereRadius * normals[index];
			textures[index].x = (float)column / ( across - 1 ) * textureHTile;
			textures[index].y = v;
			if ( row > 0 && column > 0 ) {
				int above = index - across;
				triList[triangle++] = index;
				triList[triangle++] = above;
				triList[triangle++] = index - 1;
				triList[triangle++] = above;
				triList[triangle++] = above - 1;
				triList[triangle++] = index - 1;
			}
			phi += phiStep;
		}
		theta += thetaStep;
	}

	NiTriShape *shape = new NiTriShape( vertexCount, vertices, normals, NULL, textures, triangleCount, triList );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	shape->AttachProperty( vertexColor );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( (NiAlphaProperty::AlphaFunction)srcBlendMode );
	alpha->SetDestBlendMode( (NiAlphaProperty::AlphaFunction)destBlendMode );
	alpha->SetNoSorter( true );
	shape->AttachProperty( alpha );

	NiPointer<NiTextureProperty> texture = new NiTextureProperty;
	texture->SetImage( image );
	shape->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_WRAP_T );
	shape->AttachProperty( textureMode );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor::WHITE );
	shape->AttachProperty( material );

	if ( detailTextureFileName != NULL ) {
		// BUG: the property is never attached to the shape, and leaks
		NiMultiTextureProperty *multiTexture = new NiMultiTextureProperty;
		NiPointer<NiImage> detailImage = g_kImageManager.GetImage( detailTextureFileName, true, false );
		if ( detailImage != NULL ) {
			multiTexture->SetMap( NiMultiTextureProperty::DETAIL_INDEX, new NiMultiTextureProperty::Map( detailImage, 0, NiTextureModeProperty::WRAP_S_WRAP_T, NiTextureModeProperty::FILTER_BILERP ) );
			for ( int i = 0; i < vertexCount; i++ ) {
				textures[i].x *= detailTextureHTile;
				textures[i].y *= detailTextureVTile;
			}
			// BUG: textures is the shape's own first set, which
			// AppendTextureSet frees before it copies from it
			shape->GetShapeData()->AppendTextureSet( textures );
		}
	}

	shape->UpdateProperties();
	shape->GetShapeData()->SetRevisionID( 0 );
	return shape;
}

/*
================
BackdropSphere::WriteToStream
================
*/
// FUNCTION: STBC 0x0058e200
bool BackdropSphere::WriteToStream( TGStream *stream ) {
	if ( !Backdrop::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( textureFileName );
	stream->VirtualSlot11( detailTextureFileName );
	stream->WriteFloat( horizontalSpan );
	stream->WriteFloat( verticalSpan );
	stream->WriteFloat( detailTextureHTile );
	stream->WriteFloat( detailTextureVTile );
	stream->WriteFloat( textureHTile );
	stream->WriteFloat( textureVTile );
	stream->WriteFloat( sphereRadius );
	stream->WriteInt( srcBlendMode );
	stream->WriteInt( destBlendMode );
	stream->WriteBool( decimate );
	stream->WriteLong( targetPolyCount );
	return true;
}

/*
================
BackdropSphere::ReadFromStream
================
*/
// FUNCTION: STBC 0x0058e2d0
bool BackdropSphere::ReadFromStream( TGStream *stream ) {
	if ( !Backdrop::ReadFromStream( stream ) ) {
		return false;
	}
	textureFileName = stream->VirtualSlot10( -1 );
	detailTextureFileName = stream->VirtualSlot10( -1 );
	horizontalSpan = stream->ReadFloat();
	verticalSpan = stream->ReadFloat();
	detailTextureHTile = stream->ReadFloat();
	detailTextureVTile = stream->ReadFloat();
	textureHTile = stream->ReadFloat();
	textureVTile = stream->ReadFloat();
	sphereRadius = stream->ReadFloat();
	srcBlendMode = stream->ReadInt();
	destBlendMode = stream->ReadInt();
	decimate = stream->ReadBool();
	targetPolyCount = stream->ReadLong();
	return true;
}

/*
================
BackdropSphere::PostFixUp

Loads the texture again by its name, and rebuilds the sphere.
================
*/
// FUNCTION: STBC 0x0058e390
void BackdropSphere::PostFixUp( void ) {
	ObjectClass::PostFixUp();
	char *name_ = textureFileName;
	textureFileName = NULL;
	SetTextureFileName( name_ );
	delete [] name_;
	Rebuild();
}
