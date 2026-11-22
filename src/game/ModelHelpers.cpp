/*
===============================================================================

	ModelHelpers.cpp

	Game functions on NetImmerse model trees: finding a node's controllers
	and copying the data of a cloned model. The unit these belong to is not
	known (the code lies at 0x45C200 to 0x45D1C6, after TGPoint3::Cross);
	the file name is invented.

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGAnimBlender.h"
#include "NiKeyframeController.h"
#include "NiVisController.h"
#include "ModelHelpers.h"
#include "TGModelUtils.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiMultiTextureProperty.h"
#include "NiNode.h"
#include "NiTextureProperty.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0045be00 _$E475
// FUNCTION: STBC 0x0045be20 _$E474

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0045b7c0 _$E32
// FUNCTION: STBC 0x0045b7d0 _$E29
// FUNCTION: STBC 0x0045b7e0 _$E31
// FUNCTION: STBC 0x0045b7f0 _$E30
// FUNCTION: STBC 0x0045b800 _$E38
// FUNCTION: STBC 0x0045b810 _$E35
// FUNCTION: STBC 0x0045b820 _$E37
// FUNCTION: STBC 0x0045b830 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0045b840 _$E45
// FUNCTION: STBC 0x0045b850 _$E44
// FUNCTION: STBC 0x0045b860 _$E288
// FUNCTION: STBC 0x0045b870 _$E287
// FUNCTION: STBC 0x0045b880 _$E60
// FUNCTION: STBC 0x0045b890 _$E59
// FUNCTION: STBC 0x0045b8a0 _$E56
// FUNCTION: STBC 0x0045b8b0 _$E55
// FUNCTION: STBC 0x0045b8c0 _$E64
// FUNCTION: STBC 0x0045b8d0 _$E63
// FUNCTION: STBC 0x0045b8e0 _$E68
// FUNCTION: STBC 0x0045b8f0 _$E67
// FUNCTION: STBC 0x0045b900 _$E192
// FUNCTION: STBC 0x0045b910 _$E191
// FUNCTION: STBC 0x0045b920 _$E134
// FUNCTION: STBC 0x0045b930 _$E133
// FUNCTION: STBC 0x0045b940 _$E147
// FUNCTION: STBC 0x0045b950 _$E146
// FUNCTION: STBC 0x0045b960 _$E197
// FUNCTION: STBC 0x0045b970 _$E196
// FUNCTION: STBC 0x0045b980 _$E293
// FUNCTION: STBC 0x0045b990 _$E292
// FUNCTION: STBC 0x0045b9a0 _$E201
// FUNCTION: STBC 0x0045b9b0 _$E200
// FUNCTION: STBC 0x0045b9c0 _$E297
// FUNCTION: STBC 0x0045b9d0 _$E296
// FUNCTION: STBC 0x0045b9e0 _$E304
// FUNCTION: STBC 0x0045b9f0 _$E303
// FUNCTION: STBC 0x0045ba00 _$E314
// FUNCTION: STBC 0x0045ba10 _$E313
// FUNCTION: STBC 0x0045ba20 _$E318
// FUNCTION: STBC 0x0045ba30 _$E317
// FUNCTION: STBC 0x0045ba40 _$E92
// FUNCTION: STBC 0x0045ba50 _$E91
// FUNCTION: STBC 0x0045ba60 _$E259
// FUNCTION: STBC 0x0045ba70 _$E258
// FUNCTION: STBC 0x0045ba80 _$E266
// FUNCTION: STBC 0x0045ba90 _$E265
// FUNCTION: STBC 0x0045baa0 _$E270
// FUNCTION: STBC 0x0045bab0 _$E269
// FUNCTION: STBC 0x0045bac0 _$E274
// FUNCTION: STBC 0x0045bad0 _$E273
// FUNCTION: STBC 0x0045bae0 _$E278
// FUNCTION: STBC 0x0045baf0 _$E277
// FUNCTION: STBC 0x0045bb00 _$E284
// FUNCTION: STBC 0x0045bb10 _$E283
// FUNCTION: STBC 0x0045bb20 _$E322
// FUNCTION: STBC 0x0045bb30 _$E321
// FUNCTION: STBC 0x0045bb40 _$E326
// FUNCTION: STBC 0x0045bb50 _$E325
// FUNCTION: STBC 0x0045bb60 _$E330
// FUNCTION: STBC 0x0045bb70 _$E329
// FUNCTION: STBC 0x0045bb80 _$E334
// FUNCTION: STBC 0x0045bb90 _$E333
// FUNCTION: STBC 0x0045bba0 _$E338
// FUNCTION: STBC 0x0045bbb0 _$E337
// FUNCTION: STBC 0x0045bbc0 _$E342
// FUNCTION: STBC 0x0045bbd0 _$E341
// FUNCTION: STBC 0x0045bbe0 _$E346
// FUNCTION: STBC 0x0045bbf0 _$E345
// FUNCTION: STBC 0x0045bc00 _$E350
// FUNCTION: STBC 0x0045bc10 _$E349
// FUNCTION: STBC 0x0045bc20 _$E354
// FUNCTION: STBC 0x0045bc30 _$E353
// FUNCTION: STBC 0x0045bc40 _$E358
// FUNCTION: STBC 0x0045bc50 _$E357
// FUNCTION: STBC 0x0045bc60 _$E97
// FUNCTION: STBC 0x0045bc70 _$E96
// FUNCTION: STBC 0x0045bc80 _$E362
// FUNCTION: STBC 0x0045bc90 _$E361
// FUNCTION: STBC 0x0045bca0 _$E102
// FUNCTION: STBC 0x0045bcb0 _$E101
// FUNCTION: STBC 0x0045bcc0 _$E366
// FUNCTION: STBC 0x0045bcd0 _$E365
// FUNCTION: STBC 0x0045bce0 _$E370
// FUNCTION: STBC 0x0045bcf0 _$E369
// FUNCTION: STBC 0x0045bd00 _$E374
// FUNCTION: STBC 0x0045bd10 _$E373
// FUNCTION: STBC 0x0045bd20 _$E378
// FUNCTION: STBC 0x0045bd30 _$E377
// FUNCTION: STBC 0x0045bd40 _$E111
// FUNCTION: STBC 0x0045bd50 _$E110
// FUNCTION: STBC 0x0045bd60 _$E382
// FUNCTION: STBC 0x0045bd70 _$E381
// FUNCTION: STBC 0x0045bd80 _$E386
// FUNCTION: STBC 0x0045bd90 _$E385
// FUNCTION: STBC 0x0045bda0 _$E390
// FUNCTION: STBC 0x0045bdb0 _$E389
// FUNCTION: STBC 0x0045bdc0 _$E117
// FUNCTION: STBC 0x0045bdd0 _$E116
// FUNCTION: STBC 0x0045bde0 _$E122
// FUNCTION: STBC 0x0045bdf0 _$E121
// FUNCTION: STBC 0x0045be30 _$E152
// FUNCTION: STBC 0x0045be40 _$E151
// FUNCTION: STBC 0x0045be50 _$E394
// FUNCTION: STBC 0x0045be60 _$E393
// FUNCTION: STBC 0x0045be70 _$E157
// FUNCTION: STBC 0x0045be80 _$E156
// FUNCTION: STBC 0x0045be90 _$E398
// FUNCTION: STBC 0x0045bea0 _$E397
// FUNCTION: STBC 0x0045beb0 _$E402
// FUNCTION: STBC 0x0045bec0 _$E401
// FUNCTION: STBC 0x0045bed0 _$E162
// FUNCTION: STBC 0x0045bee0 _$E161
// FUNCTION: STBC 0x0045bef0 _$E406
// FUNCTION: STBC 0x0045bf00 _$E405
// FUNCTION: STBC 0x0045bf10 _$E127
// FUNCTION: STBC 0x0045bf20 _$E126
// FUNCTION: STBC 0x0045bf30 _$E410
// FUNCTION: STBC 0x0045bf40 _$E409
// FUNCTION: STBC 0x0045bf50 _$E414
// FUNCTION: STBC 0x0045bf60 _$E413
// FUNCTION: STBC 0x0045bf70 _$E167
// FUNCTION: STBC 0x0045bf80 _$E166
// FUNCTION: STBC 0x0045bf90 _$E172
// FUNCTION: STBC 0x0045bfa0 _$E171
// FUNCTION: STBC 0x0045bfb0 _$E418
// FUNCTION: STBC 0x0045bfc0 _$E417
// FUNCTION: STBC 0x0045bfd0 _$E422
// FUNCTION: STBC 0x0045bfe0 _$E421
// FUNCTION: STBC 0x0045bff0 _$E426
// FUNCTION: STBC 0x0045c000 _$E425
// FUNCTION: STBC 0x0045c010 _$E430
// FUNCTION: STBC 0x0045c020 _$E429
// FUNCTION: STBC 0x0045c030 _$E434
// FUNCTION: STBC 0x0045c040 _$E433
// FUNCTION: STBC 0x0045c050 _$E438
// FUNCTION: STBC 0x0045c060 _$E437
// FUNCTION: STBC 0x0045c070 _$E442
// FUNCTION: STBC 0x0045c080 _$E441
// FUNCTION: STBC 0x0045c090 _$E446
// FUNCTION: STBC 0x0045c0a0 _$E445
// FUNCTION: STBC 0x0045c0b0 _$E177
// FUNCTION: STBC 0x0045c0c0 _$E176
// FUNCTION: STBC 0x0045c0d0 _$E450
// FUNCTION: STBC 0x0045c0e0 _$E449
// FUNCTION: STBC 0x0045c0f0 _$E182
// FUNCTION: STBC 0x0045c100 _$E181
// FUNCTION: STBC 0x0045c110 _$E187
// FUNCTION: STBC 0x0045c120 _$E186
// FUNCTION: STBC 0x0045c130 _$E454
// FUNCTION: STBC 0x0045c140 _$E453
// FUNCTION: STBC 0x0045c150 _$E459
// FUNCTION: STBC 0x0045c160 _$E458
// FUNCTION: STBC 0x0045c170 _$E463
// FUNCTION: STBC 0x0045c180 _$E462

/*
================
FindKeyframeController

The first keyframe controller of the object's controllers; NULL when it has
none (invented name).
================
*/
// FUNCTION: STBC 0x0045c200 ?FindKeyframeController@@YAPAVNiKeyframeController@@PAVNiObjectNET@@@Z
NiKeyframeController *FindKeyframeController( NiObjectNET *object ) {
	NiTimeController *controller = object->GetControllers();
	while ( controller != NULL ) {
		if ( controller->IsKindOf( &NiKeyframeController::rtti ) ) {
			return (NiKeyframeController *)controller;
		}
		controller = controller->GetNext();
	}
	return NULL;
}

/*
================
FindVisController

The first visibility controller of the object's controllers; NULL when it
has none (invented name).
================
*/
// FUNCTION: STBC 0x0045c240 ?FindVisController@@YAPAVNiVisController@@PAVNiObjectNET@@@Z
NiVisController *FindVisController( NiObjectNET *object ) {
	NiTimeController *controller = object->GetControllers();
	while ( controller != NULL ) {
		if ( controller->IsKindOf( &NiVisController::rtti ) ) {
			return (NiVisController *)controller;
		}
		controller = controller->GetNext();
	}
	return NULL;
}

/*
================
TGModelUtils::CastNodeToAVObject
================
*/
// FUNCTION: STBC 0x0045c280
NiAVObject *TGModelUtils::CastNodeToAVObject( NiNode *node ) {
	return node;
}

/*
================
EnableGlowAlphaMaps

Gives the object a glow alpha map of its texture's image when the image's
file name has the suffix in it (any image for NULL), making the
multi-texture property when it has none; then does the same for each
child.
================
*/
// FUNCTION: STBC 0x0045c290 ?EnableGlowAlphaMaps@@YAXPAVNiAVObject@@PBD@Z
void EnableGlowAlphaMaps( NiAVObject *object, const char *suffix ) {
	if ( object == NULL ) {
		return;
	}

	NiTextureProperty *texture = (NiTextureProperty *)(NiProperty *)object->GetProperty( NiTextureProperty::TEXTURE );
	if ( texture != NULL && texture->GetImage() != NULL
		 && ( suffix == NULL || ( texture->GetImage()->GetURL() != NULL && strstr( texture->GetImage()->GetURL(), suffix ) != NULL ) ) ) {
		NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
		if ( property == NULL ) {
			property = new NiMultiTextureProperty;
			object->AttachProperty( property );
		}
		property->SetMap( NiMultiTextureProperty::GLOW_ALPHA_INDEX,
			new NiMultiTextureProperty::Map( texture->GetImage(), 0, NiTextureModeProperty::WRAP_S_WRAP_T,
											 NiTextureModeProperty::FILTER_BILERP ) );
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			EnableGlowAlphaMaps( node->GetAt( i ), suffix );
		}
	}
}

/*
================
DisableGlowAlphaMaps

Frees the glow alpha map of the object's multi-texture property, then does
the same for each child.
================
*/
// FUNCTION: STBC 0x0045c540
void DisableGlowAlphaMaps( NiAVObject *object ) {
	if ( object == NULL ) {
		return;
	}

	NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( property != NULL ) {
		property->RemoveMap( NiMultiTextureProperty::GLOW_ALPHA_INDEX );
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			DisableGlowAlphaMaps( node->GetAt( i ) );
		}
	}
}

/*
================
TouchTextureImages

Counts a change on the image of the object's texture and on the images of
its first five multi-texture maps, then does the same for each child. The
model manager passes it to its model walks. The name is invented.
================
*/
// FUNCTION: STBC 0x0045c700
void TouchTextureImages( NiAVObject *object ) {
	if ( object == NULL ) {
		return;
	}

	NiTextureProperty *texture = (NiTextureProperty *)(NiProperty *)object->GetProperty( NiTextureProperty::TEXTURE );
	if ( texture != NULL && texture->GetImage() != NULL ) {
		texture->GetImage()->unknown20++;
	}

	NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( property != NULL ) {
		NiMultiTextureProperty::Map *map = property->GetMap( NiMultiTextureProperty::DARK_INDEX );
		if ( map != NULL && map->image != NULL ) {
			map->image->unknown20++;
		}
		map = property->GetMap( NiMultiTextureProperty::DETAIL_INDEX );
		if ( map != NULL && map->image != NULL ) {
			map->image->unknown20++;
		}
		map = property->GetMap( NiMultiTextureProperty::GLOSS_INDEX );
		if ( map != NULL && map->image != NULL ) {
			map->image->unknown20++;
		}
		map = property->GetMap( NiMultiTextureProperty::GLOW_INDEX );
		if ( map != NULL && map->image != NULL ) {
			map->image->unknown20++;
		}
		map = property->GetMap( NiMultiTextureProperty::GLOW_ALPHA_INDEX );
		if ( map != NULL && map->image != NULL ) {
			map->image->unknown20++;
		}
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			TouchTextureImages( node->GetAt( i ) );
		}
	}
}

/*
================
EnableGlossMaps

Gives the object a gloss map on texture coordinate set 1 when its texture's
image file name has the suffix in it, making the multi-texture property
when it has none; then does the same for each child. The gloss image is
the texture's own image, or with glossSuffix the file of the texture's
name with glossSuffix before the extension. Nothing in the game calls it;
the name is invented, after DisableGlossMaps and EnableGlowAlphaMaps.
================
*/
// FUNCTION: STBC 0x0045c8d0
void EnableGlossMaps( NiAVObject *object, const char *suffix, const char *glossSuffix ) {
	if ( object == NULL ) {
		return;
	}

	NiTextureProperty *texture = (NiTextureProperty *)(NiProperty *)object->GetProperty( NiTextureProperty::TEXTURE );
	if ( texture != NULL && texture->GetImage() != NULL && texture->GetImage()->GetURL() != NULL
		 && strstr( texture->GetImage()->GetURL(), suffix ) != NULL ) {
		NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
		if ( property == NULL ) {
			property = new NiMultiTextureProperty;
			object->AttachProperty( property );
		}

		NiPointer<NiImage> image;
		if ( glossSuffix != NULL ) {
			char *name = new char[strlen( texture->GetImage()->GetURL() ) + strlen( glossSuffix ) + 1];
			strcpy( name, texture->GetImage()->GetURL() );
			char *dot = strrchr( name, '.' );
			if ( dot != NULL ) {
				char *extension = new char[strlen( dot ) + 1];
				strcpy( extension, dot );
				*dot = '\0';
				strcat( name, glossSuffix );
				strcat( name, extension );
				delete extension;
			}
			image = NiImage::Create( name, 2, 7, 2 );
			delete name;
		} else {
			image = texture->GetImage();
		}
		property->SetMap( NiMultiTextureProperty::GLOSS_INDEX,
			new NiMultiTextureProperty::Map( image, 1, NiTextureModeProperty::WRAP_S_WRAP_T,
											 NiTextureModeProperty::FILTER_BILERP ) );
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			EnableGlossMaps( node->GetAt( i ), suffix, glossSuffix );
		}
	}
}

/*
================
DisableGlossMaps

DisableGlowAlphaMaps for the gloss maps. Nothing in the game calls it;
the name is invented.
================
*/
// FUNCTION: STBC 0x0045cd20
// PARKED: declaration count: the loop's back edge lands before or after the inlined GetAt's bounds check with the declarations the unit sees (ctxscan: exact with 1-63 more, 99.9 with 0; listed in config/declaration_sensitive.txt)
void DisableGlossMaps( NiAVObject *object ) {
	if ( object == NULL ) {
		return;
	}

	NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( property != NULL ) {
		property->RemoveMap( NiMultiTextureProperty::GLOSS_INDEX );
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			DisableGlossMaps( node->GetAt( i ) );
		}
	}
}

/*
================
UnshareTriShapeData

Gives every triangle shape in the tree whose data other shapes share (or,
when always is set, every one at the object itself) its own copy of the
data: the vertices, normals, colours, texture coordinates and triangles.
TGModelManager::CopyModel calls it on its copies; the name is invented.
================
*/
// FUNCTION: STBC 0x0045cee0 ?UnshareTriShapeData@@YAXPAVNiObject@@_N@Z
void UnshareTriShapeData( NiObject *object, bool always ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiTriShapeData *data = shape->GetShapeData();
		if ( data->GetRefCount() > 1 || always == true ) {
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
		}
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			UnshareTriShapeData( node->GetAt( i ), false );
		}
	}
}

/*
================
ClearRevisionIDs
================
*/
// FUNCTION: STBC 0x0045d2a0
void ClearRevisionIDs( NiAVObject *object ) {
	if ( object == NULL ) {
		return;
	}

	NiGeometry *geometry = NiDynamicCast( NiGeometry, object );
	if ( geometry != NULL ) {
		geometry->modelData->SetRevisionID( 0 );
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			ClearRevisionIDs( node->GetAt( i ) );
		}
	}
}

/*
================
RemovePropertyFromTree

Takes the property of the type off the object and off every object below it
(invented name; BaseObjectClass::RemovePropertyFromTree is the same walk
without the NULL test first).
================
*/
// FUNCTION: STBC 0x0045d1d0 ?RemovePropertyFromTree@@YAXPAVNiAVObject@@H@Z
void RemovePropertyFromTree( NiAVObject *object, int type ) {
	if ( object == NULL ) {
		return;
	}
	object->RemoveProperty( type );
	if ( object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			RemovePropertyFromTree( node->GetAt( i ), type );
		}
	}
}

/*
================
ReplaceTextureImages

Gives the image to the object's texture and to its first five
multi-texture maps wherever the image they have now has the name in its
file name, then does the same for each child. ObjectClass::ReplaceTexture
and the character's body and head replacement call it; the name is
invented.
================
*/
// FUNCTION: STBC 0x0045d340
// DRAFT: register picks: retail keeps each image's file name in eax for strstr where this build loads it into ecx and copies it, and parks the later maps' argument-copy pointer in the image parameter's slot
void ReplaceTextureImages( NiAVObject *object, NiImage *image, const char *name ) {
	NiPointer<NiTextureProperty> texture = NiDynamicCast( NiTextureProperty, object->GetProperty( NiTextureProperty::TEXTURE ) );
	if ( texture != NULL && texture->GetImage() != NULL && texture->GetImage()->GetURL() != NULL
		 && strstr( texture->GetImage()->GetURL(), name ) != NULL ) {
		texture->SetImage( image );
	}

	NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( property != NULL ) {
		NiMultiTextureProperty::Map *map = property->GetMap( NiMultiTextureProperty::DARK_INDEX );
		if ( map != NULL && map->image != NULL && map->image->GetURL() != NULL && strstr( map->image->GetURL(), name ) != NULL ) {
			map->SetImage( image );
		}
		map = property->GetMap( NiMultiTextureProperty::DETAIL_INDEX );
		if ( map != NULL && map->image != NULL && map->image->GetURL() != NULL && strstr( map->image->GetURL(), name ) != NULL ) {
			map->SetImage( image );
		}
		map = property->GetMap( NiMultiTextureProperty::GLOSS_INDEX );
		if ( map != NULL && map->image != NULL && map->image->GetURL() != NULL && strstr( map->image->GetURL(), name ) != NULL ) {
			map->SetImage( image );
		}
		map = property->GetMap( NiMultiTextureProperty::GLOW_INDEX );
		if ( map != NULL && map->image != NULL && map->image->GetURL() != NULL && strstr( map->image->GetURL(), name ) != NULL ) {
			map->SetImage( image );
		}
		map = property->GetMap( NiMultiTextureProperty::GLOW_ALPHA_INDEX );
		if ( map != NULL && map->image != NULL && map->image->GetURL() != NULL && strstr( map->image->GetURL(), name ) != NULL ) {
			map->SetImage( image );
		}
	}

	if ( object != NULL && object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child != NULL ) {
				ReplaceTextureImages( child, image, name );
			}
		}
	}
}

/*
================
FindNamedObject
================
*/
// FUNCTION: STBC 0x0045d6c0 ?FindNamedObject@@YAPAVNiAVObject@@PAV1@PBD@Z
NiAVObject *FindNamedObject( NiAVObject *object, const char *name ) {
	if ( object != NULL ) {
		if ( object->GetName() != NULL && strcmp( name, object->GetName() ) == 0 ) {
			return object;
		}
		NiNode *node = NiDynamicCast( NiNode, object );
		if ( node != NULL ) {
			for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
				if ( node->GetAt( i ) != NULL ) {
					NiAVObject *found = FindNamedObject( node->GetAt( i ), name );
					if ( found != NULL ) {
						return found;
					}
				}
			}
		}
	}
	return NULL;
}

/*
================
FindNamedProperty
================
*/
// FUNCTION: STBC 0x0045d790 ?FindNamedProperty@@YAPAVNiProperty@@PAVNiAVObject@@PBDH@Z
NiProperty *FindNamedProperty( NiAVObject *object, const char *name, int type ) {
	if ( object != NULL ) {
		if ( object->GetName() != NULL && strcmp( name, object->GetName() ) == 0 ) {
			return object->GetProperty( type );
		}
		NiNode *node = NiDynamicCast( NiNode, object );
		if ( node != NULL ) {
			for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
				if ( node->GetAt( i ) != NULL ) {
					NiProperty *found = FindNamedProperty( node->GetAt( i ), name, type );
					if ( found != NULL ) {
						return found;
					}
				}
			}
		}
	}
	return NULL;
}
