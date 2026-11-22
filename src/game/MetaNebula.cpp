/*
===============================================================================

	MetaNebula.cpp

===============================================================================
*/

#include "MetaNebula.h"
#include "DistanceModifier.h"
#include "TGFuzzyClusterGeom.h"
#include "PyEmbed.h"
#include "SetClass.h"
#include "TGPoint3.h"
#include "TGPointerMap.h"
#include "TGStream.h"
#include "NiCamera.h"
#include "NiMath.h"
#include "NiNode.h"
#include "NiTriShape.h"
#include "NiAlphaProperty.h"
#include "NiFogProperty.h"
#include "NiMaterialProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include <stdlib.h>
#include <string.h>

/*
================
MetaNebula::Cast

The object as a MetaNebula; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005973e0
MetaNebula *MetaNebula::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( METANEBULA_TYPE ) ) {
		return (MetaNebula *)object;
	}
	return NULL;
}

/*
================
MetaNebula::Create

A nebula of the fog colour, the sight distance, the sensor scale, and the
textures of the spheres seen from inside and from outside.
================
*/
// FUNCTION: STBC 0x0059a250
MetaNebula *MetaNebula::Create( float r, float g, float b, float visibility, float sensorDensity, char *textureName, char *externalTextureName ) {
	return new MetaNebula( NiColor( r, g, b ), visibility, sensorDensity, textureName, externalTextureName );
}

/*
================
MetaNebula::CreateExterior

The sensors' distance modifier, and what the nebula shows: the exterior, a
fuzzy sphere scaled to the visibility, drawn without the depth buffer and
fog and blended over the scene; and the spheres' own properties, with the
outside texture when there is one. The overlay's shape learns the node of
the spheres.
================
*/
// FUNCTION: STBC 0x0059a2c0
// PARKED: schedule: retail loads the exterior pointer before the scale's sign test, this build after it; ternary (either direction), local, inline abs helper and if/else tried, only fabs (a call) moves the load but emits fabs
void MetaNebula::CreateExterior( void ) {
	modifier = new SphereDistanceModifier( sensorDensity );

	{
		NiPointer<NiTriShape> sphere = CreateSphere( 1.0f, 2, false, color );
		exterior = new TGFuzzyClusterInnerGeom( sphere->GetShapeData(), (NiNode *)GetNiObject() );
		float scale = visibility * 0.8f;
		exterior->SetScale( scale > 0.0f ? scale : -scale );

		NiPointer<NiZBufferProperty> zBuffer = new NiZBufferProperty;
		zBuffer->SetZBufferTest( false );
		zBuffer->SetZBufferWrite( false );
		exterior->AttachProperty( zBuffer );

		NiPointer<NiVertexColorProperty> vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		exterior->AttachProperty( vertexColor );

		NiPointer<NiFogProperty> fog = new NiFogProperty;
		fog->SetFog( false );
		exterior->AttachProperty( fog );

		NiPointer<NiAlphaProperty> alpha = new NiAlphaProperty;
		alpha->SetAlphaBlending( true );
		alpha->SetNoSorter( true );
		alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
		alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
		exterior->AttachProperty( alpha );

		exterior->UpdateProperties();
	}

	if ( externalTextureName != NULL ) {
		NiPointer<NiTextureProperty> texture = new NiTextureProperty( externalTextureName );
		niObject->AttachProperty( texture );

		NiPointer<NiTextureModeProperty> textureMode = new NiTextureModeProperty;
		textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
		textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_WRAP_T );
		textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
		niObject->AttachProperty( textureMode );
	}

	NiPointer<NiVertexColorProperty> vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_AMB_DIFF );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );
	niObject->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAmbientColor( NiColor::WHITE );
	niObject->AttachProperty( material );

	NiPointer<NiAlphaProperty> alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	niObject->AttachProperty( alpha );

	NiPointer<NiZBufferProperty> zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	niObject->AttachProperty( zBuffer );

	niObject->UpdateProperties();
	innerGeom->SetClusters( (NiNode *)GetNiObject() );
}

/*
================
MetaNebula::MetaNebula
================
*/
// FUNCTION: STBC 0x0059aa80
MetaNebula::MetaNebula( const NiColor &color_, float visibility_, float sensorDensity_, char *textureName_, char *externalTextureName_ ) :
	Nebula( color_, visibility_, sensorDensity_, textureName_ ),
	spheres( 2 ) {
	if ( externalTextureName_ != NULL ) {
		externalTextureName = new char[strlen( externalTextureName_ ) + 1];
		strcpy( externalTextureName, externalTextureName_ );
	} else {
		externalTextureName = NULL;
	}
	CreateExterior();
}

/*
================
MetaNebula::~MetaNebula
================
*/
// FUNCTION: STBC 0x0059ab80
MetaNebula::~MetaNebula( void ) {
	NiTListIterator pos = spheres.GetHeadPos();
	while ( pos ) {
		delete spheres.GetNext( pos );
	}
	spheres.RemoveAll();
	delete [] externalTextureName;
}

/*
================
MetaNebula::VirtualSlot72

Where a line from start to end first comes into a sphere: the start when
it is inside one.
================
*/
// FUNCTION: STBC 0x0059acb0
bool MetaNebula::VirtualSlot72( NiPoint3 *start, NiPoint3 *end, NiPoint3 *entry ) {
	float nearest = NI_INFINITY;
	bool found = false;
	NiNode *clusters = (NiNode *)(NiAVObject *)niObject;
	for ( unsigned int i = 0; i < clusters->GetArrayCount(); i++ ) {
		TGFuzzyClusterGeom *geom = NiDynamicCast( TGFuzzyClusterGeom, clusters->GetAt( i ) );
		if ( geom != NULL ) {
			NiPoint3 center = geom->GetWorldTranslate();
			float radius = geom->GetWorldBound().GetRadius();
			if ( ( *start - center ).SqrLength() <= radius * radius ) {
				*entry = *start;
				return true;
			}
			NiPoint3 hit1;
			NiPoint3 hit2;
			NiPoint3 direction = *end - *start;
			if ( Unknown4570D0( *start, direction, center, radius, &hit1, &hit2 ) ) {
				found = true;
				float distance = ( *start - hit1 ).SqrLength();
				if ( distance < nearest ) {
					nearest = distance;
					*entry = hit1;
				}
			}
		}
	}
	return found;
}

/*
================
MetaNebula::AttachToSet

Gives the set the overlay, the sensors' distance modifier and the spheres
seen from outside.
================
*/
// FUNCTION: STBC 0x0059ae80
void MetaNebula::AttachToSet( void ) {
	containingSet->GetForegroundRoot()->AttachChild( overlayNode, false );
	containingSet->GetForegroundRoot()->Update( 0.0f );
	containingSet->GetForegroundRoot()->UpdateProperties();
	containingSet->AddDistanceModifier( modifier );
	containingSet->GetBackdropRoot()->AttachChild( exterior, false );
}

/*
================
MetaNebula::VirtualSlot68

How far the camera sees: the nebula's sight distance inside a sphere,
farther when it only looks into one. Sets the fog of the set to match.
================
*/
// FUNCTION: STBC 0x0059aef0
// DRAFT: calls and branches agree; retail keeps this in EDI and the camera in EBP (swapped here), stores the first length straight from fsqrt, and its scaled colour copies g and b before the multiplies
float MetaNebula::VirtualSlot68( NiCamera *camera ) {
	float fogDistance = NI_INFINITY;
	bool inside = false;
	NiNode *clusters = (NiNode *)(NiAVObject *)niObject;
	NiPoint3 ahead = camera->GetWorldTranslate() + visibility * camera->GetWorldDirection();
	for ( unsigned int i = 0; i < clusters->GetArrayCount() && fogDistance > visibility; i++ ) {
		TGFuzzyClusterGeom *geom = NiDynamicCast( TGFuzzyClusterGeom, clusters->GetAt( i ) );
		if ( geom != NULL ) {
			float radius = geom->GetWorldBound().GetRadius();
			float aheadDistance = ( geom->GetWorldTranslate() - ahead ).Length();
			float distance = ( geom->GetWorldTranslate() - camera->GetWorldTranslate() ).Length();
			if ( distance < radius ) {
				if ( aheadDistance < radius ) {
					fogDistance = visibility;
				} else {
					float fraction = 1.0f - ( aheadDistance - radius ) / visibility;
					fogDistance = ( 1.0f - fraction ) * fogDistance + fraction * visibility;
				}
				inside = true;
			} else if ( aheadDistance < radius ) {
				float fraction = ( radius - aheadDistance ) / visibility;
				fogDistance = ( 1.0f - fraction ) * fogDistance + fraction * visibility;
				inside = true;
			}
		}
	}
	if ( inside ) {
		containingSet->SetFog( true );
		NiColor fogColor = color;
		fogColor *= visibility / fogDistance;
		containingSet->SetFogColor( &fogColor );
	} else {
		containingSet->SetFog( false );
		containingSet->SetFogColor( (NiColor *)&NiColor::BLACK );
	}
	return fogDistance;
}

/*
================
MetaNebula::VirtualSlot71

The distance from the sphere to the nearest sphere of the nebula; 0 when
they meet.
================
*/
// FUNCTION: STBC 0x0059b170
float MetaNebula::VirtualSlot71( NiBound *bound ) {
	float nearest = NI_INFINITY;
	NiTListIterator pos = spheres.GetHeadPos();
	while ( pos && nearest > 0.0f ) {
		DistanceSphere *sphere = spheres.GetNext( pos );
		NiPoint3 offset = bound->GetCenter() - sphere->center;
		float range = bound->GetRadius() + sphere->radius;
		float distance = offset.SqrLength() - range * range;
		if ( nearest > distance ) {
			nearest = distance;
		}
	}
	if ( nearest > 0.0f ) {
		return sqrtf( nearest );
	}
	return 0.0f;
}

/*
================
MetaNebula::VirtualSlot70

The distance from the point to the nearest sphere of the nebula; 0 inside
one.
================
*/
// FUNCTION: STBC 0x0059b220
float MetaNebula::VirtualSlot70( NiPoint3 *point ) {
	float nearest = NI_INFINITY;
	NiTListIterator pos = spheres.GetHeadPos();
	while ( pos && nearest > 0.0f ) {
		DistanceSphere *sphere = spheres.GetNext( pos );
		NiPoint3 offset = *point - sphere->center;
		float distance = offset.SqrLength() - sphere->radius * sphere->radius;
		if ( nearest > distance ) {
			nearest = distance;
		}
	}
	if ( nearest > 0.0f ) {
		return sqrtf( nearest );
	}
	return 0.0f;
}

/*
================
MetaNebula::VirtualSlot69

Adds up the parts of the way from start to end that lie inside the
spheres: the points see each other while that is no more than the sight
distance.
================
*/
// FUNCTION: STBC 0x0059b2d0
// DRAFT: retail calls NiPoint3's three-float constructor out of line for the direction, both end differences and the whole way, with an out-of-line operator= and Length in the last branch; this build expands them all
int MetaNebula::VirtualSlot69( NiPoint3 *end, NiPoint3 *start ) {
	NiPoint3 direction = *end - *start;
	float length = 0.0f;
	NiTListIterator pos = spheres.GetHeadPos();
	while ( pos && length < visibility ) {
		DistanceSphere *sphere = spheres.GetNext( pos );
		NiPoint3 hit1;
		NiPoint3 hit2;
		NiPoint3 inside;
		int hits = Unknown4570D0( *start, direction, sphere->center, sphere->radius, &hit1, &hit2 );
		if ( hits > 0 ) {
			if ( hits == 2 ) {
				inside = hit2 - hit1;
			} else {
				inside = *end - hit1;
			}
			length += inside.Length();
		} else {
			NiPoint3 toStart = sphere->center - *start;
			NiPoint3 toEnd = sphere->center - *end;
			float radiusSqr = sphere->radius * sphere->radius;
			if ( toStart.SqrLength() < radiusSqr && toEnd.SqrLength() < radiusSqr ) {
				inside = *end - *start;
				length += inside.Length();
			}
		}
	}
	return length <= visibility;
}

/*
================
MetaNebula::AddSphere

Shows a white sphere of the radius at the centre, its texture turned a
random way about a random axis, fading as the camera comes within a fifth
of the visibility of its surface, and records the sphere for the distance
modifier and for the nebula.
================
*/
// FUNCTION: STBC 0x0059b520
// DRAFT: complete; retail fills the random axis y, x, z, calls the vertex scaling out of line in the texture loop and has a larger frame
void MetaNebula::AddSphere( NiPoint3 *center, float radius ) {
	NiPointer<NiTriShape> shape = CreateSphere( 1.0f, 2, true, NiColor::WHITE );
	TGFuzzyClusterGeom *geom = new TGFuzzyClusterGeom( shape );
	geom->AllocateFuzziness();
	if ( radius > 0.0f ) {
		geom->SetScale( radius );
	} else {
		geom->SetScale( -radius );
	}
	geom->SetTranslate( *center );

	NiPoint3 axis( NiSymmetricRandom(), NiSymmetricRandom(), NiSymmetricRandom() );
	axis.Unitize();
	float scale = 1.0f;
	NiMatrix3 rotation;
	rotation.MakeRotation( rand() * NI_PI * ( 1.0f / RAND_MAX ), axis );
	NiPoint3 offset = NiPoint3::ZERO;
	NiPoint3 *vertices = geom->GetShapeData()->GetVertices();
	NiPoint2 *textures = geom->GetShapeData()->GetTextures();
	for ( unsigned int i = 0; i < geom->GetShapeData()->GetVertexCount(); i++ ) {
		NiPoint3 texture = rotation * ( vertices[i] * scale ) + offset;
		textures[i].x = texture.x;
		textures[i].y = texture.y;
	}
	geom->SetFadeDistance( visibility * 0.8f );

	NiNode *clusters = (NiNode *)(NiAVObject *)niObject;
	clusters->AttachChild( geom, false );
	geom->UpdateEffects();
	geom->UpdateProperties();
	geom->Update( 0.0f );

	DistanceSphere *sphere = new DistanceSphere;
	sphere->radius = radius;
	sphere->center = *center;
	modifier->AddSphere( sphere );

	sphere = new DistanceSphere;
	sphere->radius = radius;
	sphere->center = *center;
	spheres.AddTail( sphere );
}

/*
================
MetaNebula::AddNebulaSphere

Adds a sphere at the world position.
================
*/
// FUNCTION: STBC 0x0059b920
void MetaNebula::AddNebulaSphere( float x, float y, float z, float radius ) {
	NiPoint3 center( x, y, z );
	AddSphere( &center, radius );
}

/*
================
MetaNebula::IsObjectInside

Whether the object's bound reaches into one of the spheres.
================
*/
// FUNCTION: STBC 0x0059b950
bool MetaNebula::IsObjectInside( ObjectClass *object ) {
	if ( !Nebula::IsObjectInside( object ) ) {
		return false;
	}
	NiPoint3 location = object->GetWorldLocation();
	NiNode *clusters = (NiNode *)(NiAVObject *)niObject;
	for ( unsigned int i = 0; i < clusters->GetArrayCount(); i++ ) {
		TGFuzzyClusterGeom *geom = NiDynamicCast( TGFuzzyClusterGeom, clusters->GetAt( i ) );
		if ( geom != NULL ) {
			float radius = geom->GetWorldBound().GetRadius();
			float range = object->GetWorldBound().GetRadius() + radius;
			if ( ( location - geom->GetWorldTranslate() ).SqrLength() <= range * range ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
MetaNebula::WriteToStream
================
*/
// FUNCTION: STBC 0x0059ba80
bool MetaNebula::WriteToStream( TGStream *stream ) {
	if ( !Nebula::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( externalTextureName );
	stream->VirtualSlot17( (long)(NiAVObject *)exterior );
	stream->WriteInt( spheres.GetSize() );
	NiTListIterator pos = spheres.GetHeadPos();
	while ( pos ) {
		DistanceSphere *sphere = spheres.GetNext( pos );
		stream->WriteFloat( sphere->center.x );
		stream->WriteFloat( sphere->center.y );
		stream->WriteFloat( sphere->center.z );
		stream->WriteFloat( sphere->radius );
	}
	return true;
}

/*
================
MetaNebula::ReadFromStream

The exterior is built again, and the spheres added as they were.
================
*/
// FUNCTION: STBC 0x0059bb10
bool MetaNebula::ReadFromStream( TGStream *stream ) {
	if ( !Nebula::ReadFromStream( stream ) ) {
		return false;
	}
	externalTextureName = stream->VirtualSlot10( -1 );
	CreateExterior();
	long savedExterior = stream->VirtualSlot16();
	pointerMap.Add( savedExterior, exterior );
	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		NiPoint3 center;
		center.x = stream->ReadFloat();
		center.y = stream->ReadFloat();
		center.z = stream->ReadFloat();
		float radius = stream->ReadFloat();
		AddSphere( &center, radius );
	}
	return true;
}

/*
================
MetaNebula_GetNebulaSpheres

The Python method MetaNebula.GetNebulaSpheres: a tuple of ( centre, radius )
pairs, the centre a new TGPoint3 the script owns.
================
*/
// FUNCTION: STBC 0x0059bc60
PyObject *MetaNebula_GetNebulaSpheres( PyObject *self, PyObject *args ) {
	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance == NULL ) {
		return NULL;
	}
	MetaNebula *nebula = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&nebula, "MetaNebula" );
	if ( nebula == NULL ) {
		return NULL;
	}
	NiTList<DistanceSphere *> *spheres = nebula->GetSpheres();
	PyObject *tuple = PyTuple_New( spheres->GetSize() );
	int index = 0;
	NiTListIterator pos = spheres->GetHeadPos();
	while ( pos ) {
		DistanceSphere *sphere = spheres->GetNext( pos );
		PyObject *center = PyEmbed::Unknown6F9920( new NiPoint3( sphere->center ), "TGPoint3" );
		PyEmbed::SetAttribute( center, "thisown", "i", 1 );
		PyObject *radius = PyFloat_FromDouble( sphere->radius );
		PyObject *pair = PyTuple_New( 2 );
		PyTuple_SetItem( pair, 0, center );
		PyTuple_SetItem( pair, 1, radius );
		PyTuple_SetItem( tuple, index++, pair );
	}
	return tuple;
}
