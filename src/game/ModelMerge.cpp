/*
===============================================================================

	ModelMerge.cpp

===============================================================================
*/

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale
// id pair.
// FUNCTION: STBC 0x0046ed60 _$E32
// FUNCTION: STBC 0x0046ed70 _$E29
// FUNCTION: STBC 0x0046ed80 _$E31
// FUNCTION: STBC 0x0046ed90 _$E30
// FUNCTION: STBC 0x0046eda0 _$E38
// FUNCTION: STBC 0x0046edb0 _$E35
// FUNCTION: STBC 0x0046edc0 _$E37
// FUNCTION: STBC 0x0046edd0 _$E36
// FUNCTION: STBC 0x0046efe0 _$E248
// FUNCTION: STBC 0x0046f000 _$E247
#include <iostream>
#include <string.h>

#include "ModelMerge.h"
#include "renderer/NiImage.h"
#include "NiMaterialProperty.h"
#include "NiNode.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiDynamicEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0046f060 _$E45
// FUNCTION: STBC 0x0046f070 _$E44
// FUNCTION: STBC 0x0046f080 _$E80
// FUNCTION: STBC 0x0046f090 _$E79
// FUNCTION: STBC 0x0046f0a0 _$E84
// FUNCTION: STBC 0x0046f0b0 _$E83
// FUNCTION: STBC 0x0046f0c0 _$E88
// FUNCTION: STBC 0x0046f0d0 _$E87
// FUNCTION: STBC 0x0046f0e0 _$E92
// FUNCTION: STBC 0x0046f0f0 _$E91
// FUNCTION: STBC 0x0046f100 _$E192
// FUNCTION: STBC 0x0046f110 _$E191
// FUNCTION: STBC 0x0046f120 _$E209
// FUNCTION: STBC 0x0046f130 _$E208
// FUNCTION: STBC 0x0046f140 _$E216
// FUNCTION: STBC 0x0046f150 _$E215
// FUNCTION: STBC 0x0046f160 _$E220
// FUNCTION: STBC 0x0046f170 _$E219
// FUNCTION: STBC 0x0046f180 _$E224
// FUNCTION: STBC 0x0046f190 _$E223
// FUNCTION: STBC 0x0046f1a0 _$E228
// FUNCTION: STBC 0x0046f1b0 _$E227
// FUNCTION: STBC 0x0046f1c0 _$E234
// FUNCTION: STBC 0x0046f1d0 _$E233
// FUNCTION: STBC 0x0046f200 _$E201
// FUNCTION: STBC 0x0046f210 _$E200
// FUNCTION: STBC 0x0046f220 _$E59
// FUNCTION: STBC 0x0046f230 _$E58
// FUNCTION: STBC 0x0046f240 _$E72
// FUNCTION: STBC 0x0046f250 _$E71
// FUNCTION: STBC 0x0046f260 _$E167
// FUNCTION: STBC 0x0046f270 _$E166
// FUNCTION: STBC 0x0046f280 _$E177
// FUNCTION: STBC 0x0046f290 _$E176
// FUNCTION: STBC 0x0046f2a0 _$E98
// FUNCTION: STBC 0x0046f2b0 _$E97
// FUNCTION: STBC 0x0046f2c0 _$E142
// FUNCTION: STBC 0x0046f2d0 _$E141
// FUNCTION: STBC 0x0046f1e0 _$E197
// FUNCTION: STBC 0x0046f1f0 _$E196

/*
================
ModelMerge_CopyGeometry
================
*/
// FUNCTION: STBC 0x0046d9f0
void ModelMerge_CopyGeometry( NiAVObject *object ) {
	if ( object == NULL ) {
		return;
	}
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiTriShapeData *data = shape->GetShapeData();
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
			textures = new NiPoint2[data->GetTextureSets() * vertexCount];
			memcpy( textures, data->GetTextures(), data->GetTextureSets() * vertexCount * sizeof( NiPoint2 ) );
		}
		unsigned short triListLength = data->GetTriListLength();
		unsigned short *triList = new unsigned short[triListLength];
		memcpy( triList, data->GetTriList(), triListLength * sizeof( unsigned short ) );

		NiTriShapeData *copy = new NiTriShapeData( vertexCount, vertices, normals, colors, textures, triListLength / 3, triList );
		if ( data->GetTextureSets() > 1 ) {
			copy->SetTextureSets( data->GetTextureSets() );
		}
		shape->SetModelData( copy );
		return;
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			ModelMerge_CopyGeometry( node->GetAt( i ) );
		}
	}
}

/*
================
ModelMerge_Merge

A shape's vertices and normals are taken to the model's space through its
world-space copies (the model is updated at time 0 without a parent), and
its own transform is cleared. Every shape left gets the same vertex
colour, material and texture mode properties.
================
*/
// FUNCTION: STBC 0x0046dcd0
// DRAFT: call set and statements are retail's; retail's frame is 4 bytes smaller (the new nodes' constructor results land in merged's own slot, ours copies them) and the slots and the shape-group loop's registers differ
NiAVObject *ModelMerge_Merge( NiNode *root ) {
	NiTList<NiTriShape *> shapes( 2 );
	ModelMerge_FindShapes( root, &shapes );

	NiPointer<NiNode> keep = root;
	NiNode *parent = root->GetParent();
	if ( parent != NULL ) {
		parent->DetachChild( root );
	}
	ModelMerge_CopyGeometry( root );
	root->Update( 0.0f );
	ModelMerge_RemoveProperty( root, NiVertexColorProperty::VERTEX_COLOR );
	ModelMerge_RemoveProperty( root, NiMaterialProperty::MATERIAL );
	ModelMerge_RemoveProperty( root, NiTextureModeProperty::TEXTURE_MODE );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAmbientColor( NiColor::WHITE );
	material->SetDiffuseColor( NiColor::WHITE );
	material->SetEmittance( NiColor::BLACK );
	material->SetSpecularColor( NiColor::WHITE );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_TRILERP );

	NiNode *merged;
	if ( shapes.GetSize() == 1 ) {
		NiTriShape *shape = shapes.GetHead();
		merged = new NiNode;

		shape->CreateWorldNormals();
		shape->UpdateWorldNormals();
		shape->CreateWorldVertices();
		shape->UpdateWorldVertices();
		unsigned short count = shape->GetVertexCount();
		memcpy( shape->GetVertices(), shape->GetWorldVertices(), count * sizeof( NiPoint3 ) );
		memcpy( shape->GetNormals(), shape->GetWorldNormals(), count * sizeof( NiPoint3 ) );
		shape->GetShapeData()->GetBound().unknown10 = true;
		shape->DestroyWorldVertices();
		shape->DestroyWorldNormals();
		shape->SetTranslate( NiPoint3::ZERO );
		shape->SetScale( 1.0f );
		shape->SetRotate( NiMatrix3::IDENTITY );
		shape->AttachProperty( textureMode );
		shape->AttachProperty( material );
		shape->AttachProperty( vertexColor );
		merged->AttachChild( shape, false );
		shape->GetShapeData()->SetRevisionID( 0 );
	} else {
		merged = new NiNode;

		// the shapes by texture
		NiTList<NiTList<NiTriShape *> *> groups( 2 );
		NiTListIterator pos = shapes.GetHeadPos();
		while ( pos ) {
			NiTriShape *shape = shapes.GetNext( pos );
			NiTList<NiTriShape *> *group;
			NiTListIterator groupPos = groups.GetHeadPos();
			while ( true ) {
				if ( !groupPos ) {
					group = new NiTList<NiTriShape *>( 2 );
					groups.AddTail( group );
					break;
				}
				group = groups.GetNext( groupPos );
				if ( ModelMerge_HaveSameTexture( shape, group->GetHead() ) ) {
					break;
				}
			}
			group->AddTail( shape );
		}

		NiTListIterator groupPos = groups.GetHeadPos();
		while ( groupPos ) {
			NiTList<NiTriShape *> *group = groups.GetNext( groupPos );
			if ( group->GetSize() == 1 ) {
				NiTriShape *shape = group->GetHead();
				shape->CreateWorldNormals();
				shape->UpdateWorldNormals();
				shape->CreateWorldVertices();
				shape->UpdateWorldVertices();
				unsigned short count = shape->GetVertexCount();
				memcpy( shape->GetVertices(), shape->GetWorldVertices(), count * sizeof( NiPoint3 ) );
				memcpy( shape->GetNormals(), shape->GetWorldNormals(), count * sizeof( NiPoint3 ) );
				shape->DestroyWorldVertices();
				shape->DestroyWorldNormals();
				shape->SetTranslate( NiPoint3::ZERO );
				shape->SetScale( 1.0f );
				shape->SetRotate( NiMatrix3::IDENTITY );
				shape->AttachProperty( textureMode );
				shape->AttachProperty( material );
				shape->AttachProperty( vertexColor );
				shape->GetShapeData()->GetBound().unknown10 = true;
				shape->GetShapeData()->SetRevisionID( 0 );
				merged->AttachChild( shape, false );
			} else {
				int vertexCount = 0;
				int triangleCount = 0;
				NiTListIterator shapePos = group->GetHeadPos();
				while ( shapePos ) {
					NiTriShape *shape = group->GetNext( shapePos );
					vertexCount += shape->GetVertexCount();
					triangleCount += shape->GetShapeData()->GetTriangleCount();
				}

				NiPoint3 *vertices = new NiPoint3[vertexCount];
				NiPoint3 *normals = new NiPoint3[vertexCount];
				NiPoint2 *textures = new NiPoint2[vertexCount];
				unsigned short *triList = new unsigned short[triangleCount * 3];

				NiPoint3 *vertex = vertices;
				NiPoint3 *normal = normals;
				NiPoint2 *texture = textures;
				unsigned short *tri = triList;
				int base = 0;
				shapePos = group->GetHeadPos();
				while ( shapePos ) {
					NiTriShape *shape = group->GetNext( shapePos );
					int count = shape->GetVertexCount();
					int triangles = shape->GetShapeData()->GetTriangleCount();
					shape->CreateWorldNormals();
					shape->UpdateWorldNormals();
					shape->CreateWorldVertices();
					shape->UpdateWorldVertices();
					memcpy( vertex, shape->GetWorldVertices(), count * sizeof( NiPoint3 ) );
					memcpy( normal, shape->GetWorldNormals(), count * sizeof( NiPoint3 ) );
					memcpy( texture, shape->GetTextures(), count * sizeof( NiPoint2 ) );
					memcpy( tri, shape->GetTriList(), triangles * 3 * sizeof( unsigned short ) );
					shape->DestroyWorldVertices();
					shape->DestroyWorldNormals();
					if ( base > 0 ) {
						for ( int i = 0; i < triangles; i++ ) {
							tri[i * 3] += base;
							tri[i * 3 + 1] += base;
							tri[i * 3 + 2] += base;
						}
					}
					base += count;
					vertex += count;
					normal += count;
					texture += count;
					tri += triangles * 3;
				}

				NiTriShape *shape = new NiTriShape( vertexCount, vertices, normals, NULL, textures, triangleCount, triList );
				shape->AttachProperty( group->GetHead()->GetProperty( NiTextureProperty::TEXTURE ) );
				shape->AttachProperty( textureMode );
				shape->AttachProperty( material );
				shape->AttachProperty( vertexColor );
				shape->GetShapeData()->SetRevisionID( 0 );
				merged->AttachChild( shape, false );
			}
			delete group;
		}
	}

	// the original model goes back under its parent; the merged copy is
	// returned unattached
	if ( parent != NULL ) {
		parent->AttachChild( root, false );
	}
	merged->UpdateProperties();
	merged->SetName( "Scene Root" );
	merged->Update( 0.0f );
	return merged;
}

/*
================
ModelMerge_HaveSameTexture
================
*/
// FUNCTION: STBC 0x0046e7f0
// PARKED: registers: retail tests both URLs through edx and reloads the first for strcmp (the second kept, moved to esi), and builds the image compare's bool in dl; this build keeps both URLs in registers from their tests; the image locals compared at the end inline retail's two out-of-line DecRefCount calls (77.9), nested URL tests, URL locals, !strcmp and an if/return true flat or worse
bool ModelMerge_HaveSameTexture( NiAVObject *object1, NiAVObject *object2 ) {
	NiTextureProperty *texture1 = NiDynamicCast( NiTextureProperty, object1->GetProperty( NiTextureProperty::TEXTURE ) );
	NiTextureProperty *texture2 = NiDynamicCast( NiTextureProperty, object2->GetProperty( NiTextureProperty::TEXTURE ) );
	if ( texture1 != NULL && texture2 != NULL ) {
		NiImage *image1 = texture1->GetImage();
		NiImage *image2 = texture2->GetImage();
		if ( image1 != NULL && image2 != NULL && image1->GetURL() != NULL && image2->GetURL() != NULL ) {
			return strcmp( image1->GetURL(), image2->GetURL() ) == 0;
		}
		return texture1->GetImage() == texture2->GetImage();
	}
	return false;
}

/*
================
ModelMerge_FindShapes
================
*/
// FUNCTION: STBC 0x0046ea20
void ModelMerge_FindShapes( NiAVObject *object, NiTList<NiTriShape *> *shapes ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiTextureProperty *texture = NiDynamicCast( NiTextureProperty, object->GetProperty( NiTextureProperty::TEXTURE ) );
		if ( texture != NULL ) {
			shapes->AddTail( shape );
		}
		return;
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child != NULL ) {
				ModelMerge_FindShapes( child, shapes );
			}
		}
	}
}

/*
================
ModelMerge_RemoveProperty
================
*/
// FUNCTION: STBC 0x0046ec90
void ModelMerge_RemoveProperty( NiAVObject *object, int type ) {
	if ( object == NULL ) {
		return;
	}
	object->RemoveProperty( type );
	if ( object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			ModelMerge_RemoveProperty( node->GetAt( i ), type );
		}
	}
}
