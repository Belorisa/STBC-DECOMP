/*
===============================================================================

	MetaVolume.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include "MetaVolume.h"
#include "NiNode.h"
#include "NiTriShape.h"
#include "NiWireframeProperty.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004bc9a0 _$E422
// FUNCTION: STBC 0x004bc9c0 _$E421

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004bc920 _$E32
// FUNCTION: STBC 0x004bc930 _$E29
// FUNCTION: STBC 0x004bc940 _$E31
// FUNCTION: STBC 0x004bc950 _$E30
// FUNCTION: STBC 0x004bc960 _$E38
// FUNCTION: STBC 0x004bc970 _$E35
// FUNCTION: STBC 0x004bc980 _$E37
// FUNCTION: STBC 0x004bc990 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004bc9d0 _$E45
// FUNCTION: STBC 0x004bc9e0 _$E44
// FUNCTION: STBC 0x004bc9f0 _$E63
// FUNCTION: STBC 0x004bca00 _$E62
// FUNCTION: STBC 0x004bca10 _$E67
// FUNCTION: STBC 0x004bca20 _$E66
// FUNCTION: STBC 0x004bca30 _$E71
// FUNCTION: STBC 0x004bca40 _$E70
// FUNCTION: STBC 0x004bca50 _$E75
// FUNCTION: STBC 0x004bca60 _$E74
// FUNCTION: STBC 0x004bca70 _$E194
// FUNCTION: STBC 0x004bca80 _$E193
// FUNCTION: STBC 0x004bcaa0 _$E199
// FUNCTION: STBC 0x004bcab0 _$E198
// FUNCTION: STBC 0x004bcac0 _$E203
// FUNCTION: STBC 0x004bcad0 _$E202
// FUNCTION: STBC 0x004bcae0 _$E211
// FUNCTION: STBC 0x004bcaf0 _$E210
// FUNCTION: STBC 0x004bcb00 _$E218
// FUNCTION: STBC 0x004bcb10 _$E217
// FUNCTION: STBC 0x004bcb20 _$E222
// FUNCTION: STBC 0x004bcb30 _$E221
// FUNCTION: STBC 0x004bcb40 _$E226
// FUNCTION: STBC 0x004bcb50 _$E225
// FUNCTION: STBC 0x004bcb60 _$E230
// FUNCTION: STBC 0x004bcb70 _$E229
// FUNCTION: STBC 0x004bcb80 _$E236
// FUNCTION: STBC 0x004bcb90 _$E235
// FUNCTION: STBC 0x004bcba0 _$E184
// FUNCTION: STBC 0x004bcbb0 _$E183

// The class's NiRTTI: its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004bbd10 _$E408
// FUNCTION: STBC 0x004bbd20 _$E404
// FUNCTION: STBC 0x004bbd40 _$E407
// FUNCTION: STBC 0x004bbd50 _$E405
// DATA: STBC 0x983C10 ?rtti@MetaVolume@@2VNiRTTI@@B
const NiRTTI MetaVolume::rtti( "MetaVolume", &NiObject::rtti );

// FUNCTION: STBC 0x004bbd70 _$E413
// FUNCTION: STBC 0x004bbd80 _$E410
// FUNCTION: STBC 0x004bbd90 _$E412
// FUNCTION: STBC 0x004bbda0 _$E411
// DATA: STBC 0x00983c28
NiPointer<NiTriShape> MetaVolume::sphereModel;

/*
================
MetaVolume::MetaVolume
================
*/
// FUNCTION: STBC 0x004bbde0
MetaVolume::MetaVolume( const NiPoint3 &center_, float radius_, float strength_ )
	: center( center_ ), radius( radius_ ), radiusSquared( radius_ * radius_ ), strength( strength_ ) {
	UpdateBounds();
}

/*
================
MetaVolume::~MetaVolume
================
*/
// FUNCTION: STBC 0x004bbe80
MetaVolume::~MetaVolume() {
}

/*
================
MetaVolume::SetRadius
================
*/
// FUNCTION: STBC 0x004bbe90
void MetaVolume::SetRadius( float radius_ ) {
	radius = radius_;
	radiusSquared = radius_ * radius_;
	UpdateBounds();
}

/*
================
MetaVolume::SetStrength
================
*/
// FUNCTION: STBC 0x004bbeb0
void MetaVolume::SetStrength( float strength_ ) {
	strength = strength_;
}

/*
================
MetaVolume::UpdateBounds
================
*/
// FUNCTION: STBC 0x004bbec0
void MetaVolume::UpdateBounds( void ) {
	boxMin = boxMax = center;
	float extent = radius;
	boxMin.x -= extent;
	boxMin.y -= extent;
	boxMin.z -= extent;
	boxMax.x += extent;
	boxMax.y += extent;
	boxMax.z += extent;
}

/*
================
MetaVolume::CreateDebugNode

A new node holding a copy of the sphere model, scaled to the radius.
================
*/
// FUNCTION: STBC 0x004bbf20
NiNode *MetaVolume::CreateDebugNode( void ) {
	if ( sphereModel == NULL ) {
		BuildSphereModel();
	}
	NiNode *node = new NiNode;
	node->AttachChild( (NiAVObject *)sphereModel->Clone(), false );
	float scale = radius;
	if ( scale <= 0.0f ) {
		scale = -scale;
	}
	node->SetScale( scale );
	node->Update( 0.0f );
	node->UpdateProperties();
	return node;
}

/*
================
MetaVolume::BuildSphereModel

The unit sphere of the debug nodes: an icosahedron whose twenty faces are
each split in four at their edges' midpoints, every vertex pushed out to
the unit sphere, drawn in wireframe.
================
*/
// FUNCTION: STBC 0x004bbfd0
// DRAFT: inline budget: this build calls NiProperty::SetBit out of line in the wireframe property's constructor (44 nested units short), where retail expands and folds it; vertexCount and vertices swap ebx and ebp; the face table's stores and the second and third edges' Length squares are ordered differently
void MetaVolume::BuildSphereModel( void ) {
	// the icosahedron's faces, and its corners on three golden rectangles
	NiPoint3 corners[12] = {
		NiPoint3( 1.618034f, 1.0f, 0.0f ),	NiPoint3( -1.618034f, 1.0f, 0.0f ),
		NiPoint3( 1.618034f, -1.0f, 0.0f ),	NiPoint3( -1.618034f, -1.0f, 0.0f ),
		NiPoint3( 1.0f, 0.0f, 1.618034f ),	NiPoint3( 1.0f, 0.0f, -1.618034f ),
		NiPoint3( -1.0f, 0.0f, 1.618034f ),	NiPoint3( -1.0f, 0.0f, -1.618034f ),
		NiPoint3( 0.0f, 1.618034f, 1.0f ),	NiPoint3( 0.0f, -1.618034f, 1.0f ),
		NiPoint3( 0.0f, 1.618034f, -1.0f ),	NiPoint3( 0.0f, -1.618034f, -1.0f )
	};
	unsigned short faces[60] = {
		0, 8, 4,	0, 5, 10,	2, 4, 9,	2, 11, 5,
		1, 6, 8,	1, 10, 7,	3, 9, 6,	3, 7, 11,
		0, 10, 8,	1, 8, 10,	2, 9, 11,	3, 11, 9,
		4, 2, 0,	5, 0, 2,	6, 1, 3,	7, 3, 1,
		8, 6, 4,	9, 4, 6,	10, 5, 7,	11, 7, 5
	};

	int vertexCount = 12;
	NiPoint3 *vertices = new NiPoint3[72];
	unsigned short *oldFaces = new unsigned short[240];
	unsigned short *newFaces = new unsigned short[240];

	int i;
	for ( i = 0; i < 12; i++ ) {
		corners[i].Unitize();
		vertices[i] = corners[i];
	}
	memcpy( oldFaces, faces, sizeof( faces ) );

	unsigned short *face = oldFaces;
	unsigned short *newFace = newFaces;
	for ( i = 0; i < 20; i++ ) {
		unsigned short a = *face++;
		unsigned short b = *face++;
		unsigned short c = *face++;
		vertices[vertexCount] = vertices[a] + vertices[b];
		vertices[vertexCount].Unitize();
		vertices[vertexCount + 1] = vertices[b] + vertices[c];
		vertices[vertexCount + 1].Unitize();
		vertices[vertexCount + 2] = vertices[a] + vertices[c];
		vertices[vertexCount + 2].Unitize();
		*newFace++ = a;
		*newFace++ = vertexCount;
		*newFace++ = vertexCount + 2;
		*newFace++ = b;
		*newFace++ = vertexCount + 1;
		*newFace++ = vertexCount;
		*newFace++ = c;
		*newFace++ = vertexCount + 2;
		*newFace++ = vertexCount + 1;
		*newFace++ = vertexCount;
		*newFace++ = vertexCount + 1;
		*newFace++ = vertexCount + 2;
		vertexCount += 3;
	}

	NiTriShape *shape = new NiTriShape( vertexCount, vertices, NULL, NULL, NULL, 80, newFaces );
	delete [] oldFaces;
	shape->CollapseVertices( 0 );
	sphereModel = shape;

	NiWireframeProperty *wireframe = new NiWireframeProperty;
	wireframe->SetWireframe( true );
	sphereModel->AttachProperty( wireframe );
	sphereModel->UpdateProperties();
}

/*
================
MetaVolume::GetTotalStrength

The sum of the volumes' strengths at the point. The list is never empty.
================
*/
// FUNCTION: STBC 0x004bc750
float MetaVolume::GetTotalStrength( const NiPoint3 *point, NiTArray< NiPointer<MetaVolume> > *volumes ) {
	float total = 0.0f;
	NiPointer<MetaVolume> *volume = volumes->GetBase();
	unsigned int count = volumes->GetSize();
	do {
		total += ( *volume )->GetSampleAt( *point );
		volume++;
	} while ( --count );
	return total;
}

/*
================
MetaVolume::Unknown4BC7C0

GetTotalStrength that also adds up, in direction, each volume's offset to
the point scaled by its strength there. The list is never empty.
================
*/
// FUNCTION: STBC 0x004bc7c0
// DRAFT: x87 order: retail squares the offset's x, z, then y from its stack copy and scales y and x off the stack; this build keeps z on the stack top
float MetaVolume::Unknown4BC7C0( const NiPoint3 *point, NiTArray< NiPointer<MetaVolume> > *volumes, NiPoint3 *direction ) {
	float total = 0.0f;
	NiPointer<MetaVolume> *volume = volumes->GetBase();
	unsigned int count = volumes->GetSize();
	do {
		MetaVolume *metaVolume = *volume;
		NiPoint3 offset = *point - metaVolume->center;
		float distance = offset.SqrLength();
		float volumeStrength;
		if ( distance < metaVolume->radiusSquared ) {
			float falloff = 1.0f - distance / metaVolume->radiusSquared;
			volumeStrength = falloff * metaVolume->strength * falloff;
			*direction += offset * volumeStrength;
		} else {
			volumeStrength = 0.0f;
		}
		total += volumeStrength;
		volume++;
	} while ( --count );
	return total;
}

/*
================
MetaVolumePool::GetVolumesInBox
================
*/
// FUNCTION: STBC 0x004bc890
void MetaVolumePool::GetVolumesInBox( const NiPoint3 *boxMin, const NiPoint3 *boxMax, NiTArray< NiPointer<MetaVolume> > *volumes ) {
	NiPointer<MetaVolume> *volume = objects.GetBase();
	unsigned int count = objects.GetSize();
	do {
		if ( boxMin->x <= ( *volume )->GetBoxMax().x && boxMax->x >= ( *volume )->GetBoxMin().x
		  && boxMin->y <= ( *volume )->GetBoxMax().y && boxMax->y >= ( *volume )->GetBoxMin().y
		  && boxMin->z <= ( *volume )->GetBoxMax().z && boxMax->z >= ( *volume )->GetBoxMin().z ) {
			volumes->Add( *volume );
		}
		volume++;
	} while ( --count );
}
