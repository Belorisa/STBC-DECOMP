/*
===============================================================================

	VisibleDamageBuilder.cpp

	The per-shape data of a VisibleDamage.

===============================================================================
*/

#include <iostream>
#include "VisibleDamageBuilder.h"
#include "NiMath.h"
#include "NiTriShapeData.h"
#include "DamageMesh.h"
#include "DamagePolygonizer.h"
#include "NiAlphaProperty.h"
#include "NiMaterialProperty.h"
#include "NiMultiTextureProperty.h"
#include "NiTextureProperty.h"
#include "NiTextureModeProperty.h"
#include "NiVertexColorProperty.h"
#include "OBBNode.h"

#include <string.h>
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004bb990 _$E458
// FUNCTION: STBC 0x004bb9b0 _$E457

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004b83a0 _$E32
// FUNCTION: STBC 0x004b83b0 _$E29
// FUNCTION: STBC 0x004b83c0 _$E31
// FUNCTION: STBC 0x004b83d0 _$E30
// FUNCTION: STBC 0x004b83e0 _$E38
// FUNCTION: STBC 0x004b83f0 _$E35
// FUNCTION: STBC 0x004b8400 _$E37
// FUNCTION: STBC 0x004b8410 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004b9c60 _$E45
// FUNCTION: STBC 0x004b9c70 _$E44
// FUNCTION: STBC 0x004bae30 _$E56
// FUNCTION: STBC 0x004bae40 _$E55
// FUNCTION: STBC 0x004bae50 _$E60
// FUNCTION: STBC 0x004bae60 _$E59
// FUNCTION: STBC 0x004bae70 _$E64
// FUNCTION: STBC 0x004bae80 _$E63
// FUNCTION: STBC 0x004bae90 _$E68
// FUNCTION: STBC 0x004baea0 _$E67
// FUNCTION: STBC 0x004baeb0 _$E192
// FUNCTION: STBC 0x004baec0 _$E191
// FUNCTION: STBC 0x004bafe0 _$E200
// FUNCTION: STBC 0x004baff0 _$E199
// FUNCTION: STBC 0x004bb080 _$E207
// FUNCTION: STBC 0x004bb090 _$E206
// FUNCTION: STBC 0x004bb0a0 _$E211
// FUNCTION: STBC 0x004bb0b0 _$E210
// FUNCTION: STBC 0x004bb0c0 _$E215
// FUNCTION: STBC 0x004bb0d0 _$E214
// FUNCTION: STBC 0x004bb100 _$E219
// FUNCTION: STBC 0x004bb110 _$E218
// FUNCTION: STBC 0x004bb120 _$E225
// FUNCTION: STBC 0x004bb130 _$E224
// FUNCTION: STBC 0x004bb350 _$E230
// FUNCTION: STBC 0x004bb360 _$E229
// FUNCTION: STBC 0x004bb370 _$E234
// FUNCTION: STBC 0x004bb380 _$E233
// FUNCTION: STBC 0x004bbbd0 _$E134
// FUNCTION: STBC 0x004bbbe0 _$E133
// FUNCTION: STBC 0x004bbbf0 _$E147
// FUNCTION: STBC 0x004bbc00 _$E146
// FUNCTION: STBC 0x004bbc10 _$E167
// FUNCTION: STBC 0x004bbc20 _$E166
// FUNCTION: STBC 0x004bbc50 _$E122
// FUNCTION: STBC 0x004bbc60 _$E121
// FUNCTION: STBC 0x004bbc70 _$E152
// FUNCTION: STBC 0x004bbc80 _$E151
// FUNCTION: STBC 0x004bbc90 _$E117
// FUNCTION: STBC 0x004bbca0 _$E116
// FUNCTION: STBC 0x004bbcb0 _$E177
// FUNCTION: STBC 0x004bbcc0 _$E176
// FUNCTION: STBC 0x004bbcd0 _$E92
// FUNCTION: STBC 0x004bbce0 _$E91
// FUNCTION: STBC 0x004bbcf0 _$E187
// FUNCTION: STBC 0x004bbd00 _$E186

// DATA: STBC 0x00983bc4
SimpleFeatureCollector *MetaSurfacePolygonizer::meshes[3];

/*
================
MetaSurfacePolygonizer::Init
================
*/
// FUNCTION: STBC 0x004b35c0
void MetaSurfacePolygonizer::Init() {
	meshes[0] = new SimpleFeatureCollector( true, true, true );
	meshes[1] = new SimpleFeatureCollector( true, false, false );
	meshes[2] = new SimpleFeatureCollector( true, false, false );
}

/*
================
MetaSurfacePolygonizer::Shutdown
================
*/
// FUNCTION: STBC 0x004b36a0
void MetaSurfacePolygonizer::Shutdown() {
	delete meshes[0];
	meshes[0] = NULL;
	delete meshes[1];
	meshes[1] = NULL;
	delete meshes[2];
	meshes[2] = NULL;
}

/*
================
VisibleDamagePiece::VisibleDamagePiece

Copies the shape's triangles and marks in unknown68 every triangle whose
longest edge is longer than twice the given length.
================
*/
// FUNCTION: STBC 0x004b3710
// DRAFT: inline budget: retail expands the first edge's NiPoint3 difference and calls the constructor for the other two, keeps the longest edge in the parameter's slot and the vertices in EBP; this build calls it for all three
VisibleDamagePiece::VisibleDamagePiece( NiTriShape *shape_, NiNode *model, float unknown )
	: volumes( 2 ),
	  unknown48( shape_->GetShapeData()->GetTriangleCount() ),
	  unknown50( shape_->GetShapeData()->GetTriangleCount() ),
	  unknown58( shape_->GetShapeData()->GetTriangleCount() ),
	  unknown60( shape_->GetShapeData()->GetTriangleCount() ),
	  unknown68( shape_->GetShapeData()->GetTriangleCount() ),
	  unknown70( shape_->GetShapeData()->GetVertexCount() ) {
	unknown48.ClearAll();
	unknown50.ClearAll();
	unknown58.ClearAll();
	unknown60.ClearAll();
	unknown68.ClearAll();
	unknown70.ClearAll();
	shape = shape_;

	int triangleCount = shape_->GetShapeData()->GetTriangleCount();
	triList = new unsigned short[triangleCount * 3];
	memcpy( triList, shape_->GetTriList(), triangleCount * 3 * sizeof( unsigned short ) );

	NiPoint3 *vertices = shape_->GetShapeData()->GetVertices();
	float limit = unknown + unknown;
	for ( int i = 0; i < triangleCount; i++ ) {
		NiPoint3 &corner0 = vertices[triList[i * 3]];
		NiPoint3 &corner1 = vertices[triList[i * 3 + 1]];
		NiPoint3 &corner2 = vertices[triList[i * 3 + 2]];
		float longest = ( corner0 - corner1 ).SqrLength();
		float edge = ( corner1 - corner2 ).SqrLength();
		if ( edge > longest ) {
			longest = edge;
		}
		edge = ( corner2 - corner0 ).SqrLength();
		if ( edge > longest ) {
			longest = edge;
		}
		if ( sqrtf( longest ) > limit ) {
			unknown68.Set( i );
		}
	}

	unknown7C = new unsigned short[shape_->GetShapeData()->GetTriangleCount()];
	unknown80 = 0;
	unknown1C = 0;
}

/*
================
VisibleDamagePiece::~VisibleDamagePiece
================
*/
// FUNCTION: STBC 0x004b3c00
VisibleDamagePiece::~VisibleDamagePiece() {
	delete [] triList;
	delete [] unknown7C;
}

/*
================
VisibleDamagePiece::Unknown4B3D70
================
*/
// FUNCTION: STBC 0x004b3d70
void VisibleDamagePiece::Unknown4B3D70( MetaSurfacePolygonizer *other, NiNode *model, const NiPoint3 *boxMin, const NiPoint3 *boxMax ) {
	other->model->AttachChild( shape, false );
	shape->SetTranslate( ( shape->GetWorldTranslate() - model->GetWorldTranslate() ) * model->GetWorldRotate()
						 * ( 1.0f / model->GetWorldScale() ) );
	float scale = shape->GetWorldScale() / model->GetWorldScale();
	if ( scale <= 0.0f ) {
		scale = -scale;
	}
	shape->SetScale( scale );
	shape->SetRotate( shape->GetWorldRotate().TransposeTimes( model->GetWorldRotate() ) );

	if ( unknown38 != NULL ) {
		other->unknown24->AttachChild( unknown38, false );
	}
	if ( unknown3C != NULL ) {
		other->unknown30->AttachChild( unknown3C, false );
	}
	if ( unknown40 != NULL ) {
		other->unknown28->AttachChild( unknown40, false );
	}
	if ( unknown44 != NULL ) {
		other->unknown2C->AttachChild( unknown44, false );
	}

	NiTListIterator pos = volumes.GetHeadPos();
	while ( pos != NULL ) {
		MetaVolume *volume = volumes.Get( pos );
		NiTListIterator next = volumes.GetNextPos( pos );
		if ( volume->GetBoxMax().x < boxMin->x || volume->GetBoxMax().y < boxMin->y || volume->GetBoxMax().z < boxMin->z
		  || volume->GetBoxMin().x > boxMax->x || volume->GetBoxMin().y > boxMax->y || volume->GetBoxMin().z > boxMax->z ) {
			volumes.RemovePos( pos );
		}
		pos = next;
	}
	unknown1C |= 1;
}

/*
================
VisibleDamagePiece::Unknown4B3FE0
================
*/
// FUNCTION: STBC 0x004b3fe0
// DRAFT: register allocation: retail keeps this in EBP and the new piece in EBX (swapped here) and saves EDI only after the piece is built
VisibleDamagePiece *VisibleDamagePiece::Unknown4B3FE0( MetaSurfacePolygonizer *other, NiNode *model, const NiPoint3 *boxMin, const NiPoint3 *boxMax ) {
	NiPointer<NiTriShape> copy = (NiTriShape *)shape->Clone();
	other->model->AttachChild( copy, false );
	VisibleDamagePiece *piece = new VisibleDamagePiece( copy, NULL, 0.0f );

	memcpy( piece->triList, triList, copy->GetShapeData()->GetTriangleCount() * 3 * sizeof( unsigned short ) );
	if ( unknown80 > 0 ) {
		memcpy( piece->unknown7C, unknown7C, unknown80 * sizeof( unsigned short ) );
		piece->unknown80 = unknown80;
	}
	memcpy( piece->unknown48.bits, unknown48.bits, unknown48.GetByteCount() );
	memcpy( piece->unknown50.bits, unknown50.bits, unknown50.GetByteCount() );
	memcpy( piece->unknown58.bits, unknown58.bits, unknown58.GetByteCount() );
	memcpy( piece->unknown60.bits, unknown60.bits, unknown60.GetByteCount() );
	memcpy( piece->unknown68.bits, unknown68.bits, unknown68.GetByteCount() );
	memcpy( piece->unknown70.bits, unknown70.bits, unknown70.GetByteCount() );

	NiTListIterator pos = volumes.GetHeadPos();
	while ( pos != NULL ) {
		MetaVolume *volume = volumes.GetNext( pos );
		if ( volume->GetBoxMax().x > boxMin->x && volume->GetBoxMax().y > boxMin->y && volume->GetBoxMax().z > boxMin->z
		  && volume->GetBoxMin().x < boxMax->x && volume->GetBoxMin().y < boxMax->y && volume->GetBoxMin().z < boxMax->z ) {
			piece->volumes.AddTail( volume );
		}
	}
	piece->unknown20 = unknown20;
	piece->unknown24 = unknown24;
	piece->unknown28 = unknown28;
	piece->unknown2C = unknown2C;
	piece->unknown30 = unknown30;
	piece->unknown34 = unknown34;
	piece->unknown1C |= 1;
	return piece;
}

/*
================
VisibleDamagePiece::Unknown4B4380
================
*/
// FUNCTION: STBC 0x004b4380
void VisibleDamagePiece::Unknown4B4380( unsigned short triangle ) {
	NiTriShapeData *data = shape->GetShapeData();
	if ( data->GetRefCount() > 1 ) {
		unsigned short vertexCount = data->GetVertexCount();
		NiPoint3 *vertices = new NiPoint3[vertexCount];
		memcpy( vertices, data->GetVertices(), vertexCount * sizeof( NiPoint3 ) );
		NiPoint3 *normals = NULL;
		if ( data->GetNormals() != NULL ) {
			normals = new NiPoint3[vertexCount];
			memcpy( normals, data->GetNormals(), vertexCount * sizeof( NiPoint3 ) );
		}
		NiColorA *colors = NULL;
		if ( data->GetColors() != NULL ) {
			colors = new NiColorA[vertexCount];
			memcpy( colors, data->GetColors(), vertexCount * sizeof( NiColorA ) );
		}
		NiPoint2 *textures = NULL;
		if ( data->GetTextures() != NULL ) {
			textures = new NiPoint2[vertexCount];
			memcpy( textures, data->GetTextures(), vertexCount * sizeof( NiPoint2 ) );
		}
		unsigned short length = data->GetTriListLength();
		unsigned short *corners = new unsigned short[length];
		memcpy( corners, data->GetTriList(), length * sizeof( unsigned short ) );
		shape->SetModelData( new NiTriShapeData( vertexCount, vertices, normals, colors, textures, length / 3, corners ) );
	}
	unsigned short *shapeTriList = shape->GetTriList();
	shapeTriList[triangle * 3 + 1] = shapeTriList[triangle * 3];
	shapeTriList[triangle * 3 + 2] = shapeTriList[triangle * 3];
	unknown50.Set( triangle );
}

/*
================
VisibleDamagePiece::Unknown4B45E0
================
*/
// FUNCTION: STBC 0x004b45e0
void VisibleDamagePiece::Unknown4B45E0( unsigned short triangle ) {
	if ( unknown48.Clear( triangle ) ) {
		for ( int i = 0; i < unknown80; i++ ) {
			if ( unknown7C[i] == triangle ) {
				unknown7C[i] = unknown7C[unknown80 - 1];
				unknown80--;
				i = unknown80;
			}
		}
	}
}

/*
================
VisibleDamagePiece::Unknown4B4660
================
*/
// FUNCTION: STBC 0x004b4660
void VisibleDamagePiece::Unknown4B4660( unsigned short triangle ) {
	if ( !unknown58.TestAndSet( triangle ) ) {
		Unknown4B45E0( triangle );
		if ( !unknown50.IsSet( triangle ) ) {
			Unknown4B4380( triangle );
		}
	}
}

/*
================
MetaSurfacePolygonizer::MetaSurfacePolygonizer
================
*/
// FUNCTION: STBC 0x004b46d0
MetaSurfacePolygonizer::MetaSurfacePolygonizer() : pieces( 2 ) {
	unknown08 = 0.8f;
	unknown0C = 0.5f;
	unknown14 = 0.1f;
	unknown10 = NI_INFINITY;
	unknown38 = false;
	unknown18 = false;
	unknown19 = false;
}

/*
================
MetaSurfacePolygonizer::~MetaSurfacePolygonizer
================
*/
// FUNCTION: STBC 0x004b4730
MetaSurfacePolygonizer::~MetaSurfacePolygonizer() {
	NiTListIterator pos = pieces.GetHeadPos();
	while ( pos ) {
		delete pieces.GetNext( pos );
	}
}

/*
================
MetaSurfacePolygonizer::Unknown4B48C0
================
*/
// FUNCTION: STBC 0x004b48c0
// PARKED: addressing order: retail addresses the corners as [triList + index], this build as [index + triList]; int and unsigned indices, 3 * t, a corner pointer and an accessor tried
bool MetaSurfacePolygonizer::Unknown4B48C0( NiTriShape *shape, int triangle, const unsigned short *corner, unsigned short *corner1,
										  unsigned short *corner2, VisibleDamagePiece **piece ) {
	if ( *corner == *corner1 ) {
		VisibleDamagePiece *found = *piece;
		if ( found == NULL ) {
			found = FindPiece( shape );
			*piece = found;
			if ( found == NULL ) {
				return false;
			}
		}
		if ( found->IsTriangleSet( triangle ) ) {
			*corner1 = found->triList[triangle * 3 + 1];
			*corner2 = found->triList[triangle * 3 + 2];
			return true;
		}
	}
	return false;
}

/*
================
MetaSurfacePolygonizer::GetUnknown58
================
*/
// FUNCTION: STBC 0x004b4940
DamageBitArray *MetaSurfacePolygonizer::GetUnknown58( NiTriShape *shape ) {
	VisibleDamagePiece *piece = FindPiece( shape );
	if ( piece != NULL ) {
		return &piece->unknown58;
	}
	return NULL;
}

/*
================
MetaSurfacePolygonizer::GetUnknown70
================
*/
// FUNCTION: STBC 0x004b4960
DamageBitArray *MetaSurfacePolygonizer::GetUnknown70( NiTriShape *shape ) {
	VisibleDamagePiece *piece = FindPiece( shape );
	if ( piece != NULL ) {
		return &piece->unknown70;
	}
	return NULL;
}

/*
================
MetaSurfacePolygonizer::GetTriList
================
*/
// FUNCTION: STBC 0x004b4980
unsigned short *MetaSurfacePolygonizer::GetTriList( NiTriShape *shape ) {
	VisibleDamagePiece *piece = FindPiece( shape );
	if ( piece != NULL ) {
		return piece->triList;
	}
	return shape->GetTriList();
}

/*
================
MetaSurfacePolygonizer::Unknown4B49B0

Each volume adds strength * ( 1 - d^2 / r^2 )^2 within its radius.
================
*/
// FUNCTION: STBC 0x004b49b0
bool MetaSurfacePolygonizer::Unknown4B49B0( VisibleDamagePiece *piece, const NiPoint3 *point ) {
	NiAVObject *object = piece->shape;
	NiPoint3 world = object->GetWorldRotate() * ( object->GetWorldScale() * *point ) + object->GetWorldTranslate();
	float scale = model->GetWorldScale();
	NiPoint3 local = ( 1.0f / scale ) * ( ( world - model->GetWorldTranslate() ) * model->GetWorldRotate() );
	float total = 0.0f;
	NiTListIterator pos = piece->volumes.GetHeadPos();
	while ( pos ) {
		MetaVolume *volume = piece->volumes.GetNext( pos );
		float distance = ( local - volume->GetCenter() ).SqrLength();
		float strength;
		if ( distance < volume->radiusSquared ) {
			float falloff = 1.0f - distance / volume->radiusSquared;
			strength = falloff * volume->strength * falloff;
		} else {
			strength = 0.0f;
		}
		total += strength;
	}
	return total > unknown08;
}

/*
================
MetaSurfacePolygonizer::SetUnknown18
================
*/
// FUNCTION: STBC 0x004b4b20
void MetaSurfacePolygonizer::SetUnknown18( bool enable ) {
	if ( enable == true && model != NULL ) {
		unknown18 = true;
	} else {
		unknown18 = false;
	}
}

/*
================
MetaSurfacePolygonizer::AddVolume

The volume's sphere is taken into world space and tested against the
model's OBBModel, which lists the triangles it touches shape by shape.
================
*/
// FUNCTION: STBC 0x004b4b40
// PARKED: complete; x87 operand order of the two sums' x terms (retail loads boxMin.x and the translation's x first) and inline budget (retail calls DecRefCount out of line in the delete loop); accessor, operand-swap, 0.5f * sum, scale * center and open-coded delete spellings tried
void MetaSurfacePolygonizer::AddVolume( MetaVolume *volume ) {
	NiTArray<OBBTriangleList *> lists( 1, 1 );
	NiPoint3 center = ( volume->GetBoxMin() + volume->GetBoxMax() ) * 0.5f;
	float radiusSquared = volume->radiusSquared;
	NiPoint3 worldCenter = model->GetWorldRotate() * ( center * model->GetWorldScale() ) + model->GetWorldTranslate();
	obbModel->FindSphereIntersections( worldCenter, sqrtf( radiusSquared ) * model->GetWorldScale(), &lists );
	unknown38 = unknown38 || lists.GetSize() > 0;

	unsigned int i;
	for ( i = 0; i < lists.GetSize(); i++ ) {
		OBBTriangleList *list = lists.GetAt( i );
		VisibleDamagePiece *piece = FindPiece( list->shape );
		if ( piece != NULL ) {
			piece->volumes.AddTail( volume );
			piece->unknown1C |= 1;
			for ( unsigned int j = 0; j < list->count; j++ ) {
				unsigned short triangle = list->triangles[j];
				if ( !piece->IsTriangleSet( triangle ) && !piece->unknown58.IsSet( triangle ) ) {
					piece->unknown48.Set( triangle );
					piece->unknown7C[piece->unknown80++] = triangle;
				}
			}
		}
	}

	for ( i = 0; i < lists.GetSize(); i++ ) {
		delete lists.GetAt( i );
	}
}

/*
================
MetaSurfacePolygonizer::Unknown4B4E50

Rebuilds the damage of every piece a volume has touched since the last
build (bit 0 of its unknown1C), then updates the model. Each touched
triangle's corners are weighed by the piece's volumes; the triangles
and the weights go to ProcessMesh, which cuts the damage out of the
shape. Without force, nothing happens until a volume has touched the
model (invented name).
================
*/
// FUNCTION: STBC 0x004b4e50
// DRAFT: register allocation: retail keeps the piece in EBX and the shape in EBP (this build the reverse) and holds the volume array in EAX across its regrowth; the sums over the volumes enter without a count test in retail
void MetaSurfacePolygonizer::Unknown4B4E50( bool force ) {
	if ( !unknown18 || ( !force && !unknown38 ) ) {
		return;
	}
	NiTListIterator pos = pieces.GetHeadPos();
	while ( pos ) {
		VisibleDamagePiece *piece = pieces.GetNext( pos );
		if ( !( piece->unknown1C & 1 ) ) {
			continue;
		}
		meshes[0]->SetVertexArrays( 0, NULL, NULL, NULL, NULL );
		meshes[0]->SetTriangleArray( 0, NULL );
		meshes[1]->SetVertexArrays( 0, NULL, NULL, NULL, NULL );
		meshes[1]->SetTriangleArray( 0, NULL );
		if ( unknown19 ) {
			meshes[2]->SetVertexArrays( 0, NULL, NULL, NULL, NULL );
			meshes[2]->SetTriangleArray( 0, NULL );
		}
		unsigned short triangleCount = piece->unknown80;
		NiTriShape *shape = piece->shape;
		if ( piece->volumes.GetSize() > 0 ) {
			// the piece's volumes in an array
			// DATA: STBC 0x00983bd0 _?volumeArray@?BA@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4PAPAVMetaVolume@@A
			static MetaVolume **volumeArray;
			// DATA: STBC 0x00983bd4 _?volumeCapacity@?BA@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4IA
			static unsigned int volumeCapacity;
			int volumeCount = 0;
			if ( volumeCapacity < piece->volumes.GetSize() ) {
				delete [] volumeArray;
				volumeArray = new MetaVolume *[piece->volumes.GetSize()];
				volumeCapacity = piece->volumes.GetSize();
			}
			MetaVolume **volume = volumeArray;
			NiTListIterator volumePos = piece->volumes.GetHeadPos();
			while ( volumePos ) {
				*volume = piece->volumes.GetNext( volumePos );
				volumeCount++;
				volume++;
			}

			NiTriShapeData *data = shape->GetShapeData();
			NiPoint2 *textures = data->GetTextures();
			NiPoint3 *vertices = data->GetVertices();
			NiPoint3 *normals = data->GetNormals();

			// per vertex: its weight, three vertex indices (0xFFFF for
			// none) and whether it is weighed yet
			// DATA: STBC 0x00983bd8 _?vertexBuffer@?BF@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4PAEA
			static unsigned char *vertexBuffer;
			// DATA: STBC 0x00983bdc _?vertexCapacity@?BF@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4HA
			static int vertexCapacity;
			int vertexCount = piece->shape->GetShapeData()->GetVertexCount();
			if ( vertexCapacity < vertexCount ) {
				delete [] vertexBuffer;
				vertexBuffer = new unsigned char[vertexCount * 11];
				vertexCapacity = vertexCount;
			}
			float *weights = (float *)vertexBuffer;
			unsigned short *remap0 = (unsigned short *)( weights + vertexCount );
			memset( remap0, 0xFF, vertexCount * 3 * sizeof( unsigned short ) );
			unsigned short *remap1 = remap0 + vertexCount;
			unsigned short *remap2 = remap1 + vertexCount;
			bool *weighed = (bool *)( remap2 + vertexCount );
			memset( weighed, 0, vertexCount );

			// per touched triangle: whether it is large (unknown68) and
			// whether it lies in the polygonizer's grid (unknown60), its
			// corners and the flags ProcessMesh gives it
			// DATA: STBC 0x00983b60 _?largeTriangles@?BI@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4UDamageBitArray@@A
			static DamageBitArray largeTriangles;
			// DATA: STBC 0x00983b90 _?gridTriangles@?BI@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4UDamageBitArray@@A
			static DamageBitArray gridTriangles;
			// DATA: STBC 0x00983be0 _?triangleBuffer@?BI@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4PAGA
			static unsigned short *triangleBuffer;
			// DATA: STBC 0x00983ba0 _?triangleCapacity@?BI@??Unknown4B4E50@MetaSurfacePolygonizer@@QAEX_N@Z@4IA
			static unsigned int triangleCapacity;
			if ( triangleCount > triangleCapacity ) {
				delete [] triangleBuffer;
				triangleBuffer = new unsigned short[triangleCount * 4];
				triangleCapacity = triangleCount;
				largeTriangles.SetSize( triangleCount );
				gridTriangles.SetSize( triangleCount );
			}
			unsigned short *triangles = triangleBuffer;
			unsigned short *triangleFlags = triangleBuffer + triangleCount * 3;
			memset( triangleFlags, 0, triangleCount * sizeof( unsigned short ) );
			largeTriangles.ClearAll();
			gridTriangles.ClearAll();

			unsigned short i;
			for ( i = 0; i < triangleCount; i++ ) {
				unsigned short triangle = piece->unknown7C[i];
				if ( !piece->unknown50.IsSet( triangle ) ) {
					piece->Unknown4B4380( triangle );
				}
				if ( piece->unknown60.IsSet( triangle ) ) {
					gridTriangles.Set( i );
					if ( piece->unknown68.IsSet( triangle ) ) {
						largeTriangles.Set( i );
						for ( int k = 0; k < 3; k++ ) {
							triangles[i * 3 + k] = piece->triList[triangle * 3 + k];
						}
					} else {
						for ( int k = 0; k < 3; k++ ) {
							unsigned short corner = piece->triList[triangle * 3 + k];
							if ( !weighed[corner] ) {
								if ( piece->unknown70.IsSet( corner ) ) {
									weights[corner] = unknown08 * 1.1f;
								} else {
									float weight = 0.0f;
									for ( int j = 0; j < volumeCount; j++ ) {
										weight += volumeArray[j]->GetSampleAt( vertices[corner] );
									}
									weights[corner] = weight;
								}
								weighed[corner] = true;
							}
							triangles[i * 3 + k] = corner;
						}
					}
				} else {
					if ( piece->unknown68.IsSet( triangle ) ) {
						largeTriangles.Set( i );
					}
					for ( int k = 0; k < 3; k++ ) {
						unsigned short corner = piece->triList[triangle * 3 + k];
						if ( !weighed[corner] ) {
							float weight = 0.0f;
							for ( int j = 0; j < volumeCount; j++ ) {
								weight += volumeArray[j]->GetSampleAt( vertices[corner] );
							}
							weights[corner] = weight;
							weighed[corner] = true;
						}
						triangles[i * 3 + k] = corner;
					}
				}
			}
			ProcessMesh( remap0, remap1, remap2, &gridTriangles, &largeTriangles, i, triangles, triangleFlags, weights, vertices,
						   textures, normals, volumeCount, volumeArray, true );
		}
		CollectFeatures( piece );
		piece->unknown1C &= ~1;
	}
	model->Update( 0.0f );
	unknown38 = false;
}
// The exit-time destructors of the two static bit arrays.
// FUNCTION: STBC 0x004b5620 _$E440
// FUNCTION: STBC 0x004b5630 _$E439

/*
================
MetaSurfacePolygonizer::CollectFeatures

Replaces the piece's damage shapes with ones built from the shared meshes:
meshes[0] under unknown24 and, unless unknown19, its back faces with the
texture coordinates scaled by 5 under unknown30; meshes[1] under unknown28,
two-sided unless unknown19; meshes[2], with unknown19, under unknown2C.
A mesh left empty takes its shape away.
================
*/
// FUNCTION: STBC 0x004b5640
void MetaSurfacePolygonizer::CollectFeatures( VisibleDamagePiece *piece ) {
	if ( piece->unknown38 != NULL && piece->unknown38->GetParent() != NULL ) {
		piece->unknown38->GetParent()->DetachChild( piece->unknown38 );
	}
	if ( piece->unknown3C != NULL && piece->unknown3C->GetParent() != NULL ) {
		piece->unknown3C->GetParent()->DetachChild( piece->unknown3C );
	}
	if ( meshes[0]->GetVertexCount() > 0 ) {
		int vertexCount = meshes[0]->GetVertexCount();
		int triangleCount = meshes[0]->GetTriangleCount();
		NiPoint3 *vertices = new NiPoint3[vertexCount];
		NiPoint2 *textures = new NiPoint2[vertexCount];
		NiPoint3 *normals = new NiPoint3[vertexCount];
		NiColorA *colors = new NiColorA[vertexCount];
		meshes[0]->GetVertices( vertices, textures, normals, colors );
		unsigned short *triList = new unsigned short[triangleCount * 3];
		meshes[0]->GetTriangles( triList );
		piece->unknown38 = new NiTriShape( vertexCount, vertices, normals, colors, textures, triangleCount, triList );
		unknown24->AttachChild( piece->unknown38, true );
		Unknown4B8AE0( piece->unknown38, piece->shape );
		piece->unknown38->UpdateEffects();
		if ( !unknown19 ) {
			NiPoint3 *backVertices = new NiPoint3[vertexCount];
			memcpy( backVertices, vertices, vertexCount * sizeof( NiPoint3 ) );
			NiPoint2 *backTextures = new NiPoint2[vertexCount];
			memcpy( backTextures, textures, vertexCount * sizeof( NiPoint2 ) );
			unsigned short *backTriList = new unsigned short[triangleCount * 3];
			memcpy( backTriList, triList, triangleCount * 3 * sizeof( unsigned short ) );
			int i;
			for ( i = 0; i < vertexCount; i++ ) {
				backTextures[i] *= 5.0f;
			}
			for ( i = 0; i < triangleCount; i++ ) {
				unsigned short corner = backTriList[i * 3 + 1];
				backTriList[i * 3 + 1] = backTriList[i * 3 + 2];
				backTriList[i * 3 + 2] = corner;
			}
			piece->unknown3C = new NiTriShape( vertexCount, backVertices, NULL, NULL, backTextures, triangleCount, backTriList );
			unknown30->AttachChild( piece->unknown3C, true );
			piece->unknown3C->UpdateProperties();
			piece->unknown3C->UpdateEffects();
		}
	} else {
		piece->unknown38 = NULL;
		piece->unknown3C = NULL;
	}

	if ( piece->unknown40 != NULL && piece->unknown40->GetParent() != NULL ) {
		piece->unknown40->GetParent()->DetachChild( piece->unknown40 );
	}
	if ( meshes[1]->GetVertexCount() > 0 ) {
		int damageCount = meshes[1]->GetVertexCount();
		int triangleCount = meshes[1]->GetTriangleCount();
		NiPoint3 *vertices = new NiPoint3[damageCount];
		NiPoint2 *textures = new NiPoint2[damageCount];
		if ( !unknown19 ) {
			unsigned short *triList = new unsigned short[triangleCount * 6];
			meshes[1]->GetVertices( vertices, textures, NULL, NULL );
			meshes[1]->GetTriangles( triList );
			for ( int i = 0; i < triangleCount; i++ ) {
				triList[( triangleCount + i ) * 3] = triList[i * 3];
				triList[( triangleCount + i ) * 3 + 1] = triList[i * 3 + 2];
				triList[( triangleCount + i ) * 3 + 2] = triList[i * 3 + 1];
			}
			piece->unknown40 = new NiTriShape( damageCount, vertices, NULL, NULL, textures, triangleCount * 2, triList );
		} else {
			unsigned short *triList = new unsigned short[triangleCount * 3];
			meshes[1]->GetVertices( vertices, textures, NULL, NULL );
			meshes[1]->GetTriangles( triList );
			piece->unknown40 = new NiTriShape( damageCount, vertices, NULL, NULL, textures, triangleCount, triList );
		}
		unknown28->AttachChild( piece->unknown40, true );
		piece->unknown40->UpdateProperties();
		piece->unknown40->UpdateEffects();
		meshes[1]->Reset();
	} else {
		piece->unknown40 = NULL;
	}

	if ( piece->unknown44 != NULL && piece->unknown44->GetParent() != NULL ) {
		piece->unknown44->GetParent()->DetachChild( piece->unknown44 );
	}
	if ( unknown19 && meshes[2]->GetVertexCount() > 0 ) {
		int holeCount = meshes[2]->GetVertexCount();
		int triangleCount = meshes[2]->GetTriangleCount();
		NiPoint3 *vertices = new NiPoint3[holeCount];
		NiPoint2 *textures = new NiPoint2[holeCount];
		unsigned short *triList = new unsigned short[triangleCount * 3];
		meshes[2]->GetVertices( vertices, textures, NULL, NULL );
		meshes[2]->GetTriangles( triList );
		piece->unknown44 = new NiTriShape( holeCount, vertices, NULL, NULL, textures, triangleCount, triList );
		unknown2C->AttachChild( piece->unknown44, true );
		piece->unknown44->UpdateProperties();
		meshes[2]->Reset();
	} else {
		piece->unknown44 = NULL;
	}
}

/*
================
MetaSurfacePolygonizer::Unknown4B5ED0

A large triangle is cut into a grid of steps x steps small ones (steps the
power of two at most its longest edge over unknown10), each sample weighed
by the volumes, or with inGrid only where the polygonizer's grid is set
(the others count as whole); the small triangles then go through
ProcessTri. False, with nothing built, when the triangle is under two
steps long or every sample falls in one class.
================
*/
// FUNCTION: STBC 0x004b5ed0
// DRAFT: inline budget: retail calls the NiPoint3 and NiPoint2 constructors and operators out of line and builds the first two maps through the out-of-line NiTMap constructor; this build expands them (retail also calls MetaVolume::GetSampleAt out of line at 0x4BAED0 and EdgeMap::EdgeMap at 0x4B7100)
bool MetaSurfacePolygonizer::Unknown4B5ED0( unsigned short *remap0, unsigned short *remap1, unsigned short *remap2, bool inGrid,
										  unsigned short *triangle, unsigned short *flags, NiPoint3 *vertices, NiPoint2 *textures,
										  NiPoint3 *normals, int volumeCount, MetaVolume **volumes ) {
	int corner0 = triangle[0];
	int corner1 = triangle[1];
	int corner2 = triangle[2];
	NiPoint3 normal0;
	NiPoint3 normal1;
	NiPoint3 normal2;
	if ( normals == NULL ) {
		normal0 = NiPoint3::UNIT_X;
		normal1 = NiPoint3::UNIT_X;
		normal2 = NiPoint3::UNIT_X;
	} else {
		normal0 = normals[corner0];
		normal1 = normals[corner1];
		normal2 = normals[corner2];
	}
	NiPoint2 texture0;
	NiPoint2 texture1;
	NiPoint2 texture2;
	if ( textures == NULL ) {
		texture0 = NiPoint2::ZERO;
		texture1 = NiPoint2::ZERO;
		texture2 = NiPoint2::ZERO;
	} else {
		texture0 = textures[corner0];
		texture1 = textures[corner1];
		texture2 = textures[corner2];
	}
	NiPoint3 &vertex0 = vertices[corner0];
	NiPoint3 &vertex1 = vertices[corner1];
	NiPoint3 &vertex2 = vertices[corner2];
	float longest = ( vertex0 - vertex1 ).SqrLength();
	float edge = ( vertex1 - vertex2 ).SqrLength();
	if ( edge > longest ) {
		longest = edge;
	}
	edge = ( vertex2 - vertex0 ).SqrLength();
	if ( edge > longest ) {
		longest = edge;
	}
	if ( sqrtf( longest ) < unknown10 + unknown10 ) {
		return false;
	}
	int count = (int)( sqrtf( longest ) / unknown10 );
	int bits = 0;
	while ( count != 0 ) {
		count /= 2;
		bits++;
	}
	int steps = 1 << ( bits - 1 );
	unsigned int sampleCount = ( ( steps + 1 ) * ( steps + 1 ) + 1 + steps ) / 2;

	// DATA: STBC 0x00983be4 _?sampleWeights@?BH@??Unknown4B5ED0@MetaSurfacePolygonizer@@QAE_NPAG00_N00PAVNiPoint3@@PAVNiPoint2@@2HPAPAVMetaVolume@@@Z@4PAMA
	static float *sampleWeights;
	// DATA: STBC 0x00983be8 _?sampleWeightCapacity@?BH@??Unknown4B5ED0@MetaSurfacePolygonizer@@QAE_NPAG00_N00PAVNiPoint3@@PAVNiPoint2@@2HPAPAVMetaVolume@@@Z@4HA
	static int sampleWeightCapacity;
	if ( sampleWeightCapacity < (int)sampleCount ) {
		delete [] sampleWeights;
		sampleWeights = new float[sampleCount];
		sampleWeightCapacity = sampleCount;
	}
	memset( sampleWeights, 0, sampleCount * sizeof( float ) );
	int triangleCount = steps * steps;
	// DATA: STBC 0x00983bec _?sampleTriangles@?BK@??Unknown4B5ED0@MetaSurfacePolygonizer@@QAE_NPAG00_N00PAVNiPoint3@@PAVNiPoint2@@2HPAPAVMetaVolume@@@Z@4PAGA
	static unsigned short *sampleTriangles;
	// DATA: STBC 0x00983bf0 _?sampleTriangleCapacity@?BK@??Unknown4B5ED0@MetaSurfacePolygonizer@@QAE_NPAG00_N00PAVNiPoint3@@PAVNiPoint2@@2HPAPAVMetaVolume@@@Z@4HA
	static int sampleTriangleCapacity;
	if ( sampleTriangleCapacity < triangleCount ) {
		delete [] sampleTriangles;
		sampleTriangles = new unsigned short[triangleCount * 4];
		sampleTriangleCapacity = triangleCount;
	}
	unsigned short *sampleTriangle = sampleTriangles;
	// per sample: its point, texture coordinates and normal, three
	// remaps, and its class (1 none, 2 below unknown0C, 4 below unknown08,
	// 8 whole)
	// DATA: STBC 0x00983bf4 _?sampleBuffer@?BN@??Unknown4B5ED0@MetaSurfacePolygonizer@@QAE_NPAG00_N00PAVNiPoint3@@PAVNiPoint2@@2HPAPAVMetaVolume@@@Z@4PAEA
	static unsigned char *sampleBuffer;
	// DATA: STBC 0x00983bf8 _?sampleCapacity@?BN@??Unknown4B5ED0@MetaSurfacePolygonizer@@QAE_NPAG00_N00PAVNiPoint3@@PAVNiPoint2@@2HPAPAVMetaVolume@@@Z@4HA
	static int sampleCapacity;
	// DATA: STBC 0x00983bfc _?sampleClasses@?BN@??Unknown4B5ED0@MetaSurfacePolygonizer@@QAE_NPAG00_N00PAVNiPoint3@@PAVNiPoint2@@2HPAPAVMetaVolume@@@Z@4PAEA
	static unsigned char *sampleClasses;
	if ( sampleCapacity < (int)sampleCount ) {
		delete [] sampleBuffer;
		sampleBuffer = new unsigned char[sampleCount * 38];
		sampleCapacity = sampleCount;
		delete [] sampleClasses;
		sampleClasses = new unsigned char[sampleCount];
	}
	unsigned char *classes = sampleClasses;
	NiPoint3 *points = (NiPoint3 *)sampleBuffer;
	NiPoint2 *sampleTextures = (NiPoint2 *)( points + sampleCount );
	NiPoint3 *sampleNormals = (NiPoint3 *)( sampleTextures + sampleCount );

	float scale = 1.0f / steps;
	NiPoint3 stepU = ( vertex1 - vertex0 ) * scale;
	NiPoint3 stepV = ( vertex2 - vertex0 ) * scale;
	NiPoint2 textureStepU = ( texture1 - texture0 ) / steps;
	NiPoint2 textureStepV = ( texture2 - texture0 ) / steps;
	*flags = 1;
	NiPoint3 normalStepU = ( normal1 - normal0 ) * scale;
	NiPoint3 normalStepV = ( normal2 - normal0 ) * scale;
	int sample = 0;
	int i;
	if ( !inGrid ) {
		for ( i = 0; i < steps + 1; i++ ) {
			NiPoint3 point( stepV.x * i + vertex0.x, stepV.y * i + vertex0.y, stepV.z * i + vertex0.z );
			NiPoint2 texture = textureStepV * (float)i + texture0;
			NiPoint3 normal( normalStepV.x * i + normal0.x, normalStepV.y * i + normal0.y, normalStepV.z * i + normal0.z );
			for ( int j = 0; j < steps + 1 - i; j++, sample++ ) {
				float weight = 0.0f;
				for ( int k = 0; k < volumeCount; k++ ) {
					weight += volumes[k]->GetSampleAt( point );
				}
				sampleWeights[sample] = weight;
				points[sample] = point;
				point += stepU;
				sampleTextures[sample] = texture;
				texture = textureStepU + texture;
				sampleNormals[sample] = normal;
				normal += normalStepU;
				if ( sampleWeights[sample] <= 0.0f ) {
					classes[sample] = 1;
				} else if ( sampleWeights[sample] < unknown0C ) {
					classes[sample] = 2;
				} else if ( sampleWeights[sample] < unknown08 ) {
					classes[sample] = 4;
				} else {
					classes[sample] = 8;
				}
				*flags |= classes[sample];
			}
		}
		if ( *flags == 1 ) {
			return false;
		}
		if ( *flags == 4 || *flags == 8 || *flags == 2 ) {
			return false;
		}
	} else {
		float cellScale = 1.0f / grid->cellSize;
		NiPoint3 cell = ( vertex0 - grid->origin ) * cellScale;
		NiPoint3 cellStepU = stepU * cellScale;
		NiPoint3 cellStepV = stepV * cellScale;
		for ( i = 0; i < steps + 1; i++ ) {
			NiPoint3 point = stepV * (float)i + vertex0;
			NiPoint2 texture = textureStepV * (float)i + texture0;
			NiPoint3 normal = normalStepV * (float)i + normal0;
			NiPoint3 cellPoint = cellStepV * (float)i + cell;
			for ( int j = 0; j < steps + 1 - i; j++, sample++ ) {
				unsigned short x = (unsigned short)cellPoint.x;
				unsigned short y = (unsigned short)cellPoint.y;
				unsigned short z = (unsigned short)cellPoint.z;
				if ( grid->IsSet( x, y, z ) ) {
					float weight = 0.0f;
					for ( int k = 0; k < volumeCount; k++ ) {
						weight += volumes[k]->GetSampleAt( point );
					}
					sampleWeights[sample] = weight;
					if ( weight <= 0.0f ) {
						classes[sample] = 1;
					} else if ( weight < unknown0C ) {
						classes[sample] = 2;
					} else if ( weight < unknown08 ) {
						classes[sample] = 4;
					} else {
						classes[sample] = 8;
					}
					*flags |= classes[sample];
				} else {
					sampleWeights[sample] = unknown08 * 1.1f;
					*flags |= 8;
					classes[sample] = 8;
				}
				points[sample] = point;
				point += stepU;
				sampleTextures[sample] = texture;
				texture = textureStepU + texture;
				sampleNormals[sample] = normal;
				normal += normalStepU;
				cellPoint += cellStepU;
			}
		}
	}

	// the small triangles, row by row
	int corner = 0;
	unsigned short index = 0;
	int rowLength = steps + 1;
	for ( i = 0; i < steps + 1; i++, rowLength-- ) {
		for ( int j = 0; j < rowLength; j++, index++ ) {
			if ( i < steps && j < rowLength - 1 ) {
				sampleTriangles[corner] = index;
				sampleTriangles[corner + 1] = index + 1;
				sampleTriangles[corner + 2] = index + rowLength;
				corner += 3;
				if ( j < rowLength - 2 ) {
					sampleTriangles[corner] = index + 1;
					sampleTriangles[corner + 1] = index + 1 + rowLength;
					sampleTriangles[corner + 2] = index + rowLength;
					corner += 3;
				}
			}
		}
	}

	unsigned short *sampleRemap0 = (unsigned short *)( sampleNormals + sampleCount );
	memset( sampleRemap0, 0xFF, sampleCount * 3 * sizeof( unsigned short ) );
	unsigned short *sampleRemap1 = sampleRemap0 + sampleCount;
	unsigned short *sampleRemap2 = sampleRemap1 + sampleCount;
	sampleRemap0[0] = remap0[corner0];
	sampleRemap1[0] = remap1[corner0];
	sampleRemap2[0] = remap2[corner0];
	sampleRemap0[sampleCount - 1] = remap0[corner2];
	sampleRemap1[sampleCount - 1] = remap1[corner2];
	sampleRemap2[sampleCount - 1] = remap2[corner2];
	// BUG: the second corner's sample is the row's last, steps, not steps - 1
	sampleRemap0[steps - 1] = remap0[corner1];
	sampleRemap1[steps - 1] = remap1[corner1];
	sampleRemap2[steps - 1] = remap2[corner1];

	EdgeMap edges0;
	EdgeMap edges1;
	EdgeMap edges2;
	for ( unsigned short t = 0; t < triangleCount; t++, sampleTriangle += 3 ) {
		ProcessTri( sampleRemap0, sampleRemap1, sampleRemap2, &edges0, &edges1, &edges2, sampleTriangle, sampleWeights, points,
					   sampleTextures, sampleNormals );
	}
	return true;
}

/*
================
The cutting tables

A triangle's six points: corner k at 2k, the point where the edge from
corner k to corner k + 1 crosses the threshold at 2k + 1. Indexed by the
bits of the corners below the threshold: which edges cross it, and the up
to two triangles that cover the part below it (0xFFFF after the last).
The complement's entry covers the part above.
================
*/
// DATA: STBC 0x0088c620
static const unsigned char edgeCrossings[8][3] = {
	{ 0, 0, 0 },
	{ 1, 0, 1 },
	{ 1, 1, 0 },
	{ 0, 1, 1 },
	{ 0, 1, 1 },
	{ 1, 1, 0 },
	{ 1, 0, 1 },
	{ 0, 0, 0 }
};
// DATA: STBC 0x0088c638
static const short cutTriangles[8][2][3] = {
	{ { -1, -1, -1 }, { -1, -1, -1 } },
	{ { 0, 1, 5 }, { -1, -1, -1 } },
	{ { 1, 2, 3 }, { -1, -1, -1 } },
	{ { 0, 2, 3 }, { 3, 5, 0 } },
	{ { 3, 4, 5 }, { -1, -1, -1 } },
	{ { 0, 1, 3 }, { 0, 3, 4 } },
	{ { 1, 2, 4 }, { 1, 4, 5 } },
	{ { 0, 2, 4 }, { -1, -1, -1 } }
};

/*
================
EdgeKey

(invented) The edge map's key of the edge between two points: the larger
key in the high bits.
================
*/
inline int EdgeKey( unsigned short point0, unsigned short point1 ) {
	if ( point1 < point0 ) {
		return ( point0 << 16 ) | point1;
	}
	return ( point1 << 16 ) | point0;
}

inline __int64 EdgeKey( __int64 key0, __int64 key1 ) {
	if ( key0 <= key1 ) {
		return ( key1 << 16 ) | key0;
	}
	return ( key0 << 16 ) | key1;
}

/*
================
CutEdge

(invented) The point and texture coordinates where the threshold cuts the
edge from corner 0 to corner 1.
================
*/
inline void CutEdge( const float &threshold, float weight0, float weight1, const NiPoint3 &point0, const NiPoint3 &point1,
					 const NiPoint2 &texture0, const NiPoint2 &texture1, NiPoint3 &point, NiPoint2 &texture ) {
	float t = threshold - weight0;
	if ( NiAbs( t ) < 0.001f ) {
		point = point0;
		texture = texture0;
	} else if ( NiAbs( threshold - weight1 ) < 0.001f ) {
		point = point1;
		texture = texture1;
	} else {
		if ( NiAbs( weight0 - weight1 ) < 0.001f ) {
			t = 0.5f;
		} else {
			t /= weight1 - weight0;
		}
		point = point0 + t * ( point1 - point0 );
		texture = texture0 + ( texture1 - texture0 ) * t;
	}
}

/*
================
CutEdge

(invented) The same with the normals.
================
*/
inline void CutEdge( const float &threshold, float weight0, float weight1, const NiPoint3 &point0, const NiPoint3 &point1,
					 const NiPoint2 &texture0, const NiPoint2 &texture1, const NiPoint3 &normal0, const NiPoint3 &normal1,
					 NiPoint3 &point, NiPoint2 &texture, NiPoint3 &normal ) {
	float t = threshold - weight0;
	if ( NiAbs( t ) < 0.001f ) {
		point = point0;
		texture = texture0;
		normal = normal0;
	} else if ( NiAbs( threshold - weight1 ) < 0.001f ) {
		point = point1;
		texture = texture1;
		normal = normal1;
	} else {
		if ( NiAbs( weight0 - weight1 ) < 0.001f ) {
			t = 0.5f;
		} else {
			t /= weight1 - weight0;
		}
		point = point0 + t * ( point1 - point0 );
		texture = texture0 + ( texture1 - texture0 ) * t;
		normal = normal0 + t * ( normal1 - normal0 );
	}
}

/*
================
MetaSurfacePolygonizer::ProcessTri

The part of the triangle whose weights fall below unknown0C goes into
meshes[0], its corners darker the closer they are to the threshold; the
rest is cut again by unknown08 into meshes[1] (and meshes[2] with
unknown19). Returns 2 when meshes[0] got a triangle, with the flags of the
second cut.
================
*/
// FUNCTION: STBC 0x004b71e0
// DRAFT: call set as retail (the cut through CutEdge, the key through EdgeKey: one nested level and one site more); frame slots and register picks differ
unsigned short MetaSurfacePolygonizer::ProcessTri( unsigned short *remap0, unsigned short *remap1, unsigned short *remap2,
													EdgeMap *edges0, EdgeMap *edges1, EdgeMap *edges2,
													unsigned short *triangle, float *weights, NiPoint3 *vertices, NiPoint2 *textures,
													NiPoint3 *normals ) {
	unsigned short result = 0;
	unsigned int below = weights[triangle[0]] < unknown0C;
	if ( weights[triangle[1]] < unknown0C ) {
		below |= 2;
	}
	if ( weights[triangle[2]] < unknown0C ) {
		below |= 4;
	}

	__int64 keys[6];
	NiPoint3 points[6];
	NiPoint2 pointTextures[6];
	NiPoint3 pointNormals[6];
	float pointWeights[6];
	unsigned short indices[6];
	unsigned short slot = 0;
	const unsigned char *crossings = edgeCrossings[below];
	for ( int k = 0; k < 3; k++, slot += 2 ) {
		unsigned short corner = triangle[k];
		if ( !( below & ( 1 << k ) ) ) {
			keys[slot] = corner;
			points[slot] = vertices[corner];
			pointTextures[slot] = textures[corner];
			pointNormals[slot] = normals[corner];
			pointWeights[slot] = weights[corner];
		} else if ( remap0[corner] == 0xFFFF ) {
			float shade = ( unknown0C - weights[corner] ) / unknown0C;
			shade *= shade;
			if ( shade > 1.0f ) {
				shade = 1.0f;
			} else if ( shade < 0.0f ) {
				shade = 0.0f;
			}
			NiColorA color( shade, shade, shade, 1.0f );
			remap0[corner] = meshes[0]->AddVertex( &vertices[corner], &textures[corner], &normals[corner], &color );
			indices[slot] = remap0[corner];
		} else {
			indices[slot] = remap0[corner];
		}
		if ( crossings[k] ) {
			unsigned short next = triangle[( k + 1 ) % 3];
			unsigned short edge = slot + 1;
			CutEdge( unknown0C, weights[corner], weights[next], vertices[corner], vertices[next], textures[corner], textures[next],
					 normals[corner], normals[next], points[edge], pointTextures[edge], pointNormals[edge] );
			pointWeights[edge] = unknown0C;
			keys[edge] = EdgeKey( corner, next );
			unsigned short index;
			if ( edges0->GetAt( keys[edge], index ) ) {
				indices[edge] = index;
			} else {
				NiColorA black( 0.0f, 0.0f, 0.0f, 1.0f );
				indices[edge] = meshes[0]->AddVertex( &points[edge], &pointTextures[edge], &pointNormals[edge], &black );
				edges0->AddEdge( keys[edge], indices[edge] );
			}
		}
	}

	int i;
	const short *corners = cutTriangles[below][0];
	for ( i = 0; i < 2; i++, corners += 3 ) {
		if ( corners[0] == -1 ) {
			break;
		}
		meshes[0]->AddTriangle( indices[corners[0]], indices[corners[1]], indices[corners[2]] );
		result = 2;
	}
	corners = cutTriangles[~below & 7][0];
	if ( unknown19 ) {
		for ( i = 0; i < 2; i++, corners += 3 ) {
			if ( corners[0] == -1 ) {
				break;
			}
			result |= ProcessDamageAndHoleTri( remap1, remap2, edges1, edges2, (const unsigned short *)corners, keys, pointWeights, points, pointTextures );
		}
	} else {
		for ( i = 0; i < 2; i++, corners += 3 ) {
			if ( corners[0] == -1 ) {
				break;
			}
			result |= ProcessDamageTri( remap1, edges1, (const unsigned short *)corners, keys, pointWeights, points, pointTextures );
		}
	}
	return result;
}

/*
================
MetaSurfacePolygonizer::ProcessDamageAndHoleTri

The second cut with unknown19: the part of the polygon below unknown08
goes into meshes[1], the part above into meshes[2], both with the
texture coordinates scaled by 5.
================
*/
// FUNCTION: STBC 0x004b79c0
// DRAFT: inline budget: CutEdge's share is 309 where retail needs 260-272 (operator- out of line, operator* expanded): about 37 units x 4 sites (own cost ~74 tuples less) before it; the corner SetAt goes through AddEdge out of line as retail
unsigned short MetaSurfacePolygonizer::ProcessDamageAndHoleTri( unsigned short *remap1, unsigned short *remap2, EdgeMap *edges1,
													EdgeMap *edges2, const unsigned short *corners, const __int64 *keys,
													const float *weights, const NiPoint3 *points, const NiPoint2 *textures ) {
	unsigned short result = 0;
	unsigned int below = 0;
	if ( weights[corners[0]] < unknown08 ) {
		below |= 1;
	}
	if ( weights[corners[1]] < unknown08 ) {
		below |= 2;
	}
	if ( weights[corners[2]] < unknown08 ) {
		below |= 4;
	}
	unsigned short indices1[6];
	unsigned short indices2[6];
	unsigned short slot = 0;
	for ( int k = 0; k < 3; k++, slot += 2 ) {
		unsigned short point = corners[k];
		if ( !( below & ( 1 << k ) ) ) {
			if ( keys[point] < 0x10000 ) {
				if ( remap2[keys[point]] == 0xFFFF ) {
					NiPoint2 texture = textures[point] * 5.0f;
					indices2[slot] = remap2[keys[point]] = meshes[2]->AddVertex( &points[point], &texture, NULL, NULL );
				} else {
					indices2[slot] = remap2[keys[point]];
				}
			} else if ( !edges2->GetAt( keys[point], indices2[slot] ) ) {
				NiPoint2 texture = textures[point] * 5.0f;
				indices2[slot] = meshes[2]->AddVertex( &points[point], &texture, NULL, NULL );
				edges2->AddEdge( keys[point], indices2[slot] );
			}
		} else {
			if ( keys[point] < 0xFFFF ) {
				if ( remap1[keys[point]] == 0xFFFF ) {
					NiPoint2 texture = textures[point] * 5.0f;
					indices1[slot] = remap1[keys[point]] = meshes[1]->AddVertex( &points[point], &texture, NULL, NULL );
				} else {
					indices1[slot] = remap1[keys[point]];
				}
			} else if ( !edges1->GetAt( keys[point], indices1[slot] ) ) {
				NiPoint2 texture = textures[point] * 5.0f;
				indices1[slot] = meshes[1]->AddVertex( &points[point], &texture, NULL, NULL );
				edges1->AddEdge( keys[point], indices1[slot] );
			}
		}
		if ( edgeCrossings[below][k] ) {
			unsigned short next = corners[( k + 1 ) % 3];
			__int64 key = EdgeKey( keys[point], keys[next] );
			if ( edges1->GetAt( key, indices1[slot + 1] ) ) {
				edges2->GetAt( key, indices2[slot + 1] );
			} else {
				NiPoint3 edgePoint;
				NiPoint2 texture;
				CutEdge( unknown08, weights[point], weights[next], points[point], points[next], textures[point], textures[next],
						 edgePoint, texture );
				texture *= 5.0f;
				indices1[slot + 1] = meshes[1]->AddVertex( &edgePoint, &texture, NULL, NULL );
				indices2[slot + 1] = meshes[2]->AddVertex( &edgePoint, &texture, NULL, NULL );
				edges1->AddEdge( key, indices1[slot + 1] );
				edges2->AddEdge( key, indices2[slot + 1] );
			}
		}
	}

	int i;
	const short *triangle = cutTriangles[below][0];
	for ( i = 0; i < 2; i++, triangle += 3 ) {
		if ( triangle[0] == -1 ) {
			break;
		}
		meshes[1]->AddTriangle( indices1[triangle[0]], indices1[triangle[1]], indices1[triangle[2]] );
		result |= 4;
	}
	triangle = cutTriangles[~below & 7][0];
	for ( i = 0; i < 2; i++, triangle += 3 ) {
		if ( triangle[0] == -1 ) {
			break;
		}
		meshes[2]->AddTriangle( indices2[triangle[0]], indices2[triangle[1]], indices2[triangle[2]] );
		result |= 8;
	}
	return result;
}

/*
================
MetaSurfacePolygonizer::ProcessDamageTri

The second cut without unknown19: the part of the polygon below unknown08
goes into meshes[1] with the texture coordinates scaled by 5. Returns 4
when meshes[1] got a triangle, with 8 when a part lies above.
================
*/
// FUNCTION: STBC 0x004b8420
// DRAFT: call set as retail (CutEdge, AddEdge, EdgeKey); one 4-byte frame slot more than retail (0xC4 against 0xC0) and the callee-saved pushes in another order
unsigned short MetaSurfacePolygonizer::ProcessDamageTri( unsigned short *remap1, EdgeMap *edges1, const unsigned short *corners,
													const __int64 *keys, const float *weights, const NiPoint3 *points,
													const NiPoint2 *textures ) {
	unsigned short result = 0;
	unsigned int below = 0;
	if ( weights[corners[0]] < unknown08 ) {
		below |= 1;
	}
	if ( weights[corners[1]] < unknown08 ) {
		below |= 2;
	}
	if ( weights[corners[2]] < unknown08 ) {
		below |= 4;
	}
	unsigned short indices[6];
	unsigned short slot = 0;
	for ( int k = 0; k < 3; k++, slot += 2 ) {
		unsigned short point = corners[k];
		if ( below & ( 1 << k ) ) {
			if ( keys[point] < 0xFFFF ) {
				if ( remap1[keys[point]] == 0xFFFF ) {
					NiPoint2 texture = textures[point] * 5.0f;
					indices[slot] = meshes[1]->AddVertex( &points[point], &texture, NULL, NULL );
					remap1[keys[point]] = indices[slot];
				} else {
					indices[slot] = remap1[keys[point]];
				}
			} else if ( !edges1->GetAt( keys[point], indices[slot] ) ) {
				NiPoint2 texture = textures[point] * 5.0f;
				indices[slot] = meshes[1]->AddVertex( &points[point], &texture, NULL, NULL );
				edges1->AddEdge( keys[point], indices[slot] );
			}
		}
		if ( edgeCrossings[below][k] ) {
			unsigned short next = corners[( k + 1 ) % 3];
			__int64 key = EdgeKey( keys[point], keys[next] );
			if ( !edges1->GetAt( key, indices[slot + 1] ) ) {
				NiPoint3 edgePoint;
				NiPoint2 texture;
				CutEdge( unknown08, weights[point], weights[next], points[point], points[next], textures[point], textures[next],
						 edgePoint, texture );
				texture *= 5.0f;
				indices[slot + 1] = meshes[1]->AddVertex( &edgePoint, &texture, NULL, NULL );
				edges1->AddEdge( key, indices[slot + 1] );
			}
		}
	}

	const short *triangle = cutTriangles[below][0];
	for ( int i = 0; i < 2; i++, triangle += 3 ) {
		if ( triangle[0] == -1 ) {
			break;
		}
		meshes[1]->AddTriangle( indices[triangle[0]], indices[triangle[1]], indices[triangle[2]] );
		result = 4;
	}
	if ( cutTriangles[~below & 7][0][0] != -1 ) {
		result |= 8;
	}
	return result;
}

/*
================
MetaSurfacePolygonizer::Unknown4B8AE0
================
*/
// FUNCTION: STBC 0x004b8ae0
void MetaSurfacePolygonizer::Unknown4B8AE0( NiTriShape *damage, NiTriShape *source ) {
	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_AMB_DIFF );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );
	damage->AttachProperty( vertexColor );

	NiPointer<NiTextureProperty> texture = NiDynamicCast( NiTextureProperty, source->GetProperty( NiTextureProperty::TEXTURE ) );
	if ( texture ) {
		damage->AttachProperty( (NiProperty *)texture->Clone() );
	}
	NiPointer<NiTextureModeProperty> textureMode =
		NiDynamicCast( NiTextureModeProperty, source->GetProperty( NiTextureModeProperty::TEXTURE_MODE ) );
	if ( textureMode ) {
		damage->AttachProperty( (NiProperty *)textureMode->Clone() );
	}
	NiPointer<NiMultiTextureProperty> multiTexture =
		NiDynamicCast( NiMultiTextureProperty, source->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( multiTexture ) {
		damage->AttachProperty( (NiProperty *)multiTexture->Clone() );
	}

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAmbientColor( NiColor::WHITE );
	material->SetEmittance( NiColor::BLACK );
	damage->AttachProperty( material );
	damage->UpdateProperties();
}

/*
================
MetaSurfacePolygonizer::FindPiece
================
*/
// FUNCTION: STBC 0x004b8e80
VisibleDamagePiece *MetaSurfacePolygonizer::FindPiece( NiTriShape *shape ) {
	NiTListIterator pos = pieces.GetHeadPos();
	while ( pos ) {
		VisibleDamagePiece *piece = pieces.GetNext( pos );
		if ( piece != NULL && piece->shape == shape ) {
			return piece;
		}
	}
	return NULL;
}

/*
================
MetaSurfacePolygonizer::SetModel
================
*/
// FUNCTION: STBC 0x004b8eb0
void MetaSurfacePolygonizer::SetModel( NiNode *model_, NiProperty *property0, NiProperty *property1, NiProperty *property2 ) {
	model = model_;
	AddPieces( model );
	unknown24 = new NiNode;
	unknown30 = new NiNode;
	unknown28 = new NiNode;
	unknown2C = new NiNode;
	model->AttachChild( unknown24, false );
	model->AttachChild( unknown30, false );
	model->AttachChild( unknown28, false );
	unknown28->AttachChild( unknown2C, false );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_TRILERP );
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_WRAP_T );
	unknown28->AttachProperty( textureMode );
	unknown2C->AttachProperty( textureMode );
	unknown30->AttachProperty( textureMode );
	unknown30->AttachProperty( property0 );

	NiVertexColorProperty *unlit = new NiVertexColorProperty;
	unlit->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	unlit->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	unknown30->AttachProperty( unlit );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor::WHITE );
	unknown30->AttachProperty( material );

	NiVertexColorProperty *emissive = new NiVertexColorProperty;
	emissive->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	emissive->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	unknown28->AttachProperty( emissive );
	if ( property1 != NULL ) {
		unknown28->AttachProperty( property1 );
		if ( !unknown19 ) {
			NiAlphaProperty *alpha = new NiAlphaProperty;
			alpha->SetAlphaBlending( true );
			alpha->SetNoSorter( true );
			alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
			alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
			unknown28->AttachProperty( alpha );
		}
	}
	unknown2C->AttachProperty( emissive );
	if ( property2 != NULL ) {
		unknown2C->AttachProperty( property2 );
	}
}

/*
================
MetaSurfacePolygonizer::Unknown4B93D0

Adds to list the pieces whose shape's world bound, in model's frame, meets
the box (no caller in retail; invented name).
================
*/
// FUNCTION: STBC 0x004b93d0
void MetaSurfacePolygonizer::Unknown4B93D0( NiTList<VisibleDamagePiece *> *list, NiNode *model, const NiPoint3 *boxMin, const NiPoint3 *boxMax ) {
	NiTListIterator pos = pieces.GetHeadPos();
	while ( pos ) {
		VisibleDamagePiece *piece = pieces.GetNext( pos );
		NiTriShape *shape = piece->shape;
		if ( shape != NULL ) {
			float scale = model->GetWorldScale();
			NiPoint3 center = ( 1.0f / scale )
							  * ( ( shape->GetWorldBound().GetCenter() - model->GetWorldTranslate() ) * model->GetWorldRotate() );
			float radius = shape->GetWorldBound().GetRadius() / model->GetWorldScale();
			if ( center.x + radius > boxMin->x && center.y + radius > boxMin->y && center.z + radius > boxMin->z
			  && center.x - radius < boxMax->x && center.y - radius < boxMax->y && center.z - radius < boxMax->z ) {
				list->AddTail( piece );
			}
		}
	}
}

/*
================
MetaSurfacePolygonizer::AddPieces
================
*/
// FUNCTION: STBC 0x004b9570
void MetaSurfacePolygonizer::AddPieces( NiAVObject *object ) {
	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			AddPieces( child );
		}
	} else {
		NiTriShape *shape = NiDynamicCast( NiTriShape, object );
		if ( shape != NULL ) {
			pieces.AddTail( new VisibleDamagePiece( shape, model, unknown10 ) );
		}
	}
}

/*
================
MetaSurfacePolygonizer::RemovePiece

Takes the piece's shape and its damage shapes off the model and deletes
the piece (invented name).
================
*/
// FUNCTION: STBC 0x004b9760
void MetaSurfacePolygonizer::RemovePiece( VisibleDamagePiece *piece ) {
	if ( piece->shape->GetParent() != NULL ) {
		piece->shape->GetParent()->DetachChild( piece->shape );
	}
	if ( piece->unknown38 != NULL && piece->unknown38->GetParent() != NULL ) {
		piece->unknown38->GetParent()->DetachChild( piece->unknown38 );
	}
	if ( piece->unknown3C != NULL && piece->unknown3C->GetParent() != NULL ) {
		piece->unknown3C->GetParent()->DetachChild( piece->unknown3C );
	}
	if ( piece->unknown40 != NULL && piece->unknown40->GetParent() != NULL ) {
		piece->unknown40->GetParent()->DetachChild( piece->unknown40 );
	}
	if ( piece->unknown44 != NULL && piece->unknown44->GetParent() != NULL ) {
		piece->unknown44->GetParent()->DetachChild( piece->unknown44 );
	}
	pieces.Remove( piece );
	delete piece;
}

/*
================
MetaSurfacePolygonizer::Unknown4B98B0

Samples the triangle (three points in the model's frame) in steps of at
most unknown10, rounded to a power of two per edge: true when no sample
lies in a set cell of the current grid (invented name).
================
*/
// FUNCTION: STBC 0x004b98b0
// DRAFT: inline budget: retail calls NiPoint3::NiPoint3 for the edges and DamageBitArray::IsSet out of line; this build calls DamageVoxelGrid::IsSet
bool MetaSurfacePolygonizer::Unknown4B98B0( const NiPoint3 *triangle ) {
	float longest = ( triangle[0] - triangle[1] ).SqrLength();
	float edge = ( triangle[1] - triangle[2] ).SqrLength();
	if ( edge > longest ) {
		longest = edge;
	}
	edge = ( triangle[2] - triangle[0] ).SqrLength();
	if ( edge > longest ) {
		longest = edge;
	}
	int count = (int)( sqrtf( longest ) / unknown10 );
	int bits = 0;
	while ( count != 0 ) {
		count /= 2;
		bits++;
	}
	int steps = 1 << ( bits - 1 );
	NiPoint3 stepU = ( triangle[1] - triangle[0] ) * ( 1.0f / ( steps * grid->cellSize ) );
	NiPoint3 stepV = ( triangle[2] - triangle[0] ) * ( 1.0f / ( steps * grid->cellSize ) );
	NiPoint3 start = ( triangle[0] - grid->origin ) / grid->cellSize;
	for ( int i = 0; i < steps + 1; i++ ) {
		NiPoint3 sample = stepV * (float)i + start;
		for ( int j = 0; j < steps + 1 - i; j++ ) {
			int x = (int)sample.x;
			int y = (int)sample.y;
			int z = (int)sample.z;
			if ( x >= 0 && y >= 0 && z >= 0 && x < grid->size[0] && y < grid->size[1] && z < grid->size[2]
			  && grid->IsSet( x, y, z ) ) {
				return false;
			}
			sample += stepU;
		}
	}
	return true;
}

/*
================
MetaSurfacePolygonizer::Unknown4B9C80

As Unknown4B98B0, but true as soon as a sample inside the grid lies in a
cell that is not set (invented name).
================
*/
// FUNCTION: STBC 0x004b9c80
// DRAFT: inline budget: as Unknown4B98B0
bool MetaSurfacePolygonizer::Unknown4B9C80( const NiPoint3 *triangle ) {
	float longest = ( triangle[0] - triangle[1] ).SqrLength();
	float edge = ( triangle[1] - triangle[2] ).SqrLength();
	if ( edge > longest ) {
		longest = edge;
	}
	edge = ( triangle[2] - triangle[0] ).SqrLength();
	if ( edge > longest ) {
		longest = edge;
	}
	int count = (int)( sqrtf( longest ) / unknown10 );
	int bits = 0;
	while ( count != 0 ) {
		count /= 2;
		bits++;
	}
	int steps = 1 << ( bits - 1 );
	NiPoint3 stepU = ( triangle[1] - triangle[0] ) * ( 1.0f / ( steps * grid->cellSize ) );
	NiPoint3 stepV = ( triangle[2] - triangle[0] ) * ( 1.0f / ( steps * grid->cellSize ) );
	NiPoint3 start = ( triangle[0] - grid->origin ) / grid->cellSize;
	for ( int i = 0; i < steps + 1; i++ ) {
		NiPoint3 sample = stepV * (float)i + start;
		for ( int j = 0; j < steps + 1 - i; j++ ) {
			int x = (int)sample.x;
			int y = (int)sample.y;
			int z = (int)sample.z;
			if ( x >= 0 && y >= 0 && z >= 0 && x < grid->size[0] && y < grid->size[1] && z < grid->size[2]
			  && !grid->IsSet( x, y, z ) ) {
				return true;
			}
			sample += stepU;
		}
	}
	return false;
}

/*
================
MetaSurfacePolygonizer::Unknown4BA030

Gives the other builder a copy of each piece whose shape's bound meets the
grid's box, cut to the box (VisibleDamagePiece::Unknown4B3FE0), and has
it rebuilt (invented name).
================
*/
// FUNCTION: STBC 0x004ba030
// PARKED: the bound's address: with the reference it is a temporary in eax (add eax, 0xc) that the four loads use, where retail folds 0xc into each load's displacement; every spelling that folds it (no reference, a data local, the chain twice) also drops retail's spill slot of this (frame 4 bytes short, this in edi: 89.4-90.0)
void MetaSurfacePolygonizer::Unknown4BA030( DamageVoxelGrid *grid_, MetaSurfacePolygonizer *other ) {
	NiPoint3 boxMin;
	NiPoint3 boxMax;
	grid_->GetBox( boxMin, boxMax );
	NiTListIterator pos = pieces.GetHeadPos();
	while ( pos ) {
		VisibleDamagePiece *piece = pieces.GetNext( pos );
		const NiBound &bound = piece->shape->GetShapeData()->GetBound();
		NiPoint3 center = bound.GetCenter();
		float radius = bound.GetRadius();
		if ( boxMin.x < center.x + radius && boxMin.y < center.y + radius && boxMin.z < center.z + radius
		  && center.x - radius < boxMax.x && center.y - radius < boxMax.y && center.z - radius < boxMax.z ) {
			other->pieces.AddTail( piece->Unknown4B3FE0( other, model, &boxMin, &boxMax ) );
		}
	}
	other->model->Update( 0.0f );
	other->unknown38 = true;
}

/*
================
MetaSurfacePolygonizer::Unknown4BA250

Sorts every piece's triangles against the polygonizer's current grid. A
corner is gone when it lies outside the grid's box or in a cell that is
not set (then marked in unknown70); a triangle is gone when its three
corners are, cut when some are. A piece whose shape misses the box, or
whose triangles are all gone or skipped (unknown58), is removed; gone
triangles are folded away (Unknown4B4660) and cut ones marked for the
next build (unknown60). Large triangles (unknown68) are sampled first
(invented name).
================
*/
// FUNCTION: STBC 0x004ba250
// DRAFT: call set and branch count are retail's; retail places the k loop's outside-the-box and already-tested blocks after the loop's exit (this build inline), and its frame is 4 bytes larger
void MetaSurfacePolygonizer::Unknown4BA250( void ) {
	bool removed = false;
	NiPoint3 boxMin;
	NiPoint3 boxMax;
	grid->GetBox( boxMin, boxMax );
	DamageVoxelGrid *grid_ = grid;
	float cellSize = grid_->cellSize;
	NiTListIterator pos = pieces.GetHeadPos();
	while ( pos ) {
		VisibleDamagePiece *piece = pieces.GetNext( pos );
		NiTriShape *shape = piece->shape;
		NiTriShapeData *data = shape->GetShapeData();
		NiPoint3 center = data->GetBound().GetCenter();
		float radius = data->GetBound().GetRadius();
		if ( center.x + radius < boxMin.x || center.y + radius < boxMin.y || center.z + radius < boxMin.z
		  || center.x - radius > boxMax.x || center.y - radius > boxMax.y || center.z - radius > boxMax.z ) {
			RemovePiece( piece );
			removed = true;
			continue;
		}

		NiPoint3 *vertices = data->GetVertices();
		unsigned short *triList = piece->triList;
		int cutCount = 0;
		int goneCount = 0;
		int skippedCount = 0;
		DamageBitArray goneVertices( data->GetVertexCount() );
		goneVertices.ClearAll();
		DamageBitArray testedVertices( shape->GetShapeData()->GetVertexCount() );
		testedVertices.ClearAll();
		DamageBitArray cutTriangles( shape->GetShapeData()->GetTriangleCount() );
		cutTriangles.ClearAll();
		DamageBitArray goneTriangles( shape->GetShapeData()->GetTriangleCount() );
		goneTriangles.ClearAll();

		int triangleCount = shape->GetShapeData()->GetTriangleCount();
		int i;
		for ( i = 0; i < triangleCount; i++, triList += 3 ) {
			if ( !piece->unknown58.IsSet( i ) ) {
				bool gone[3];
				for ( int k = 0; k < 3; k++ ) {
					unsigned short corner = triList[k];
					if ( testedVertices.IsSet( corner ) ) {
						gone[k] = goneVertices.IsSet( corner );
					} else {
						testedVertices.Set( corner );
						NiPoint3 &vertex = vertices[corner];
						gone[k] = false;
						if ( vertex.x < boxMin.x || vertex.y < boxMin.y || vertex.z < boxMin.z
						  || vertex.x > boxMax.x || vertex.y > boxMax.y || vertex.z > boxMax.z ) {
							gone[k] = true;
							piece->unknown70.Set( corner );
							goneVertices.Set( corner );
						} else {
							NiPoint3 cell = ( vertex - grid_->origin ) / cellSize;
							unsigned short x = (unsigned short)cell.x;
							unsigned short y = (unsigned short)cell.y;
							unsigned short z = (unsigned short)cell.z;
							if ( !grid->IsSet( x, y, z ) ) {
								piece->unknown70.Set( corner );
								goneVertices.Set( corner );
								gone[k] = true;
							}
						}
					}
				}
				if ( !piece->unknown68.IsSet( i ) ) {
					if ( gone[0] && gone[1] && gone[2] ) {
						goneTriangles.Set( i );
						goneCount++;
					} else if ( gone[0] || gone[1] || gone[2] ) {
						cutTriangles.Set( i );
						cutCount++;
					}
				} else if ( gone[0] && gone[1] && gone[2] ) {
					NiPoint3 corners[3];
					for ( int k = 0; k < 3; k++ ) {
						corners[k] = vertices[triList[k]];
					}
					if ( Unknown4B98B0( corners ) ) {
						goneTriangles.Set( i );
						goneCount++;
					} else {
						cutTriangles.Set( i );
						cutCount++;
					}
				} else if ( !gone[0] && !gone[1] && !gone[2] ) {
					NiPoint3 corners[3];
					for ( int k = 0; k < 3; k++ ) {
						corners[k] = vertices[triList[k]];
					}
					if ( Unknown4B9C80( corners ) ) {
						cutTriangles.Set( i );
						cutCount++;
					}
				} else {
					cutTriangles.Set( i );
					cutCount++;
				}
			} else {
				skippedCount++;
			}
		}

		if ( skippedCount + goneCount == shape->GetShapeData()->GetTriangleCount() ) {
			RemovePiece( piece );
			removed = true;
		} else if ( goneCount > 0 ) {
			if ( cutCount > 0 ) {
				unknown38 = true;
				piece->unknown1C |= 1;
				for ( i = 0; i < triangleCount; i++ ) {
					if ( cutTriangles.IsSet( i ) ) {
						piece->unknown60.Set( (unsigned short)i );
					} else if ( goneTriangles.IsSet( i ) ) {
						piece->Unknown4B4660( i );
					}
				}
			} else {
				for ( i = 0; i < triangleCount; i++ ) {
					if ( goneTriangles.IsSet( i ) ) {
						piece->Unknown4B4660( i );
					}
				}
			}
		} else if ( cutCount > 0 ) {
			unknown38 = true;
			piece->unknown1C |= 1;
			for ( i = 0; i < triangleCount; i++ ) {
				if ( cutTriangles.IsSet( i ) ) {
					piece->unknown60.Set( (unsigned short)i );
				}
			}
		}
	}
	if ( removed ) {
		obbModel->RefreshShapes();
	}
}

/*
================
MetaSurfacePolygonizer::ProcessMesh

Each large triangle is first tessellated (Unknown4B5ED0); the others, and
the large ones it leaves whole, are cut by their corners' weights
(ProcessTri), the three maps sharing the vertices made on the edges.
================
*/
// FUNCTION: STBC 0x004bab90
// PARKED: register: register allocation of the loop pointers; calls and branches agree
void MetaSurfacePolygonizer::ProcessMesh( unsigned short *remap0, unsigned short *remap1, unsigned short *remap2,
										  DamageBitArray *gridTriangles, DamageBitArray *largeTriangles, unsigned short triangleCount,
										  unsigned short *triangles, unsigned short *triangleFlags, float *weights, NiPoint3 *vertices,
										  NiPoint2 *textures, NiPoint3 *normals, int volumeCount, MetaVolume **volumes, bool ) {
	EdgeMap edges0;
	EdgeMap edges1;
	EdgeMap edges2;
	for ( unsigned short i = 0; i < triangleCount; i++ ) {
		if ( !largeTriangles->IsSet( i )
		  || !Unknown4B5ED0( remap0, remap1, remap2, gridTriangles->IsSet( i ), &triangles[i * 3], &triangleFlags[i], vertices,
							 textures, normals, volumeCount, volumes ) ) {
			triangleFlags[i] = ProcessTri( remap0, remap1, remap2, &edges0, &edges1, &edges2, &triangles[i * 3], weights,
											  vertices, textures, normals );
		}
	}
}

/*
================
SimpleFeatureCollector::AddVertex
================
*/
// FUNCTION: STBC 0x004bb4d0 ?AddVertex@SimpleFeatureCollector@@QAEGPBVNiPoint3@@PBVNiPoint2@@0PBVNiColorA@@@Z
unsigned short SimpleFeatureCollector::AddVertex( const NiPoint3 *vertex, const NiPoint2 *texture, const NiPoint3 *normal,
													const NiColorA *color ) {
	if ( vertexCount < vertexCapacity ) {
		vertices[vertexCount] = *vertex;
		if ( texture ) {
			textures[vertexCount] = *texture;
		}
		if ( normal != NULL ) {
			normals[vertexCount] = *normal;
		}
		if ( color != NULL ) {
			colors[vertexCount] = *color;
		}
	} else {
		unsigned int extra = vertexCount - vertexCapacity;
		unsigned int blockIndex = extra / DamageVertexBlock::SIZE;
		unsigned int slot = extra % DamageVertexBlock::SIZE;
		DamageVertexBlock *block;
		if ( blockIndex + 1 > vertexBlocks.GetSize() ) {
			block = new DamageVertexBlock( hasTextures, hasNormals, hasColors );
			vertexBlocks.Add( block );
		} else {
			block = vertexBlocks.GetAt( blockIndex );
		}
		block->vertices[slot] = *vertex;
		if ( texture ) {
			block->textures[slot] = *texture;
		}
		if ( normal != NULL ) {
			block->normals[slot] = *normal;
		}
		if ( color != NULL ) {
			block->colors[slot] = *color;
		}
	}
	vertexCount++;
	return vertexCount - 1;
}

/*
================
SimpleFeatureCollector::AddTriangle
================
*/
// FUNCTION: STBC 0x004bb780 ?AddTriangle@SimpleFeatureCollector@@QAEGGGG@Z
// DRAFT: inline budget: retail calls NiTArray SetSize and SetAt out of line (0x4BBAE0, 0x4BBB60); this build expands SetSize
unsigned short SimpleFeatureCollector::AddTriangle( unsigned short corner0, unsigned short corner1, unsigned short corner2 ) {
	if ( triangleCount < triangleCapacity ) {
		int index = triangleCount * 3;
		triangles[index++] = corner0;
		triangles[index++] = corner1;
		triangles[index] = corner2;
	} else {
		unsigned int extra = triangleCount - triangleCapacity;
		unsigned int blockIndex = extra / DamageVertexBlock::SIZE;
		unsigned int slot = extra % DamageVertexBlock::SIZE;
		unsigned short *block;
		if ( blockIndex + 1 > triangleBlocks.GetSize() ) {
			block = new unsigned short[DamageVertexBlock::SIZE * 3];
			triangleBlocks.Add( block );
		} else {
			block = triangleBlocks.GetAt( blockIndex );
		}
		int index = slot * 3;
		block[index++] = corner0;
		block[index++] = corner1;
		block[index] = corner2;
	}
	triangleCount++;
	return triangleCount - 1;
}

