/*
===============================================================================

	ViewScreenNode.cpp

===============================================================================
*/

#include <stdlib.h>

#include "NiMain.h"
#include "NiMath.h"
#include "ViewScreenNode.h"
#include "NiMaterialProperty.h"
#include "NiVertexColorProperty.h"
#include "NiTextureModeProperty.h"
#include "ModelHelpers.h"
#include "TGModelUtils.h"
#include "ImageManagerClass.h"
#include "TGIconManager.h"
#include "TGIconGroup.h"
#include "UtopiaModule.h"
#include "NiAlphaProperty.h"
#include "NiTextureProperty.h"
#include "NiFlipController.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiCamera.h"
#include "CameraObjectClass.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00677e60 _$E455
// FUNCTION: STBC 0x00677e80 _$E454

// The standard streams' initialisers and exit-time destructors (<iostream>):
// they and the run below come after the unit's functions, as SayLineEvent's do.
// FUNCTION: STBC 0x006777b0 _$E440
// FUNCTION: STBC 0x006777c0 _$E437
// FUNCTION: STBC 0x006777d0 _$E439
// FUNCTION: STBC 0x006777e0 _$E438
// FUNCTION: STBC 0x006777f0 _$E446
// FUNCTION: STBC 0x00677800 _$E443
// FUNCTION: STBC 0x00677810 _$E445
// FUNCTION: STBC 0x00677820 _$E444

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006761c0 _$E405
// FUNCTION: STBC 0x006761d0 _$E404
// FUNCTION: STBC 0x00677830 _$E21
// FUNCTION: STBC 0x00677840 _$E20
// FUNCTION: STBC 0x00677850 _$E25
// FUNCTION: STBC 0x00677860 _$E24
// FUNCTION: STBC 0x00677870 _$E36
// FUNCTION: STBC 0x00677880 _$E35
// FUNCTION: STBC 0x006778c0 _$E40
// FUNCTION: STBC 0x006778d0 _$E39
// FUNCTION: STBC 0x006778e0 _$E44
// FUNCTION: STBC 0x006778f0 _$E43
// FUNCTION: STBC 0x00677900 _$E48
// FUNCTION: STBC 0x00677910 _$E47
// FUNCTION: STBC 0x00677920 _$E172
// FUNCTION: STBC 0x00677930 _$E171
// FUNCTION: STBC 0x00677940 _$E114
// FUNCTION: STBC 0x00677950 _$E113
// FUNCTION: STBC 0x00677960 _$E127
// FUNCTION: STBC 0x00677970 _$E126
// FUNCTION: STBC 0x00677980 _$E177
// FUNCTION: STBC 0x00677990 _$E176
// FUNCTION: STBC 0x006779a0 _$E182
// FUNCTION: STBC 0x006779b0 _$E181
// FUNCTION: STBC 0x006779c0 _$E186
// FUNCTION: STBC 0x006779d0 _$E185
// FUNCTION: STBC 0x006779e0 _$E190
// FUNCTION: STBC 0x006779f0 _$E189
// FUNCTION: STBC 0x00677a00 _$E197
// FUNCTION: STBC 0x00677a10 _$E196
// FUNCTION: STBC 0x00677a20 _$E222
// FUNCTION: STBC 0x00677a30 _$E221
// FUNCTION: STBC 0x00677a40 _$E226
// FUNCTION: STBC 0x00677a50 _$E225
// FUNCTION: STBC 0x00677a60 _$E72
// FUNCTION: STBC 0x00677a70 _$E71
// FUNCTION: STBC 0x00677a80 _$E208
// FUNCTION: STBC 0x00677a90 _$E207
// FUNCTION: STBC 0x00677aa0 _$E215
// FUNCTION: STBC 0x00677ab0 _$E214
// FUNCTION: STBC 0x00677ac0 _$E238
// FUNCTION: STBC 0x00677ad0 _$E237
// FUNCTION: STBC 0x00677ae0 _$E242
// FUNCTION: STBC 0x00677af0 _$E241
// FUNCTION: STBC 0x00677b00 _$E282
// FUNCTION: STBC 0x00677b10 _$E281
// FUNCTION: STBC 0x00677b20 _$E288
// FUNCTION: STBC 0x00677b30 _$E287
// FUNCTION: STBC 0x00677b40 _$E230
// FUNCTION: STBC 0x00677b50 _$E229
// FUNCTION: STBC 0x00677b60 _$E234
// FUNCTION: STBC 0x00677b70 _$E233
// FUNCTION: STBC 0x00677b80 _$E246
// FUNCTION: STBC 0x00677b90 _$E245
// FUNCTION: STBC 0x00677bc0 _$E250
// FUNCTION: STBC 0x00677bd0 _$E249
// FUNCTION: STBC 0x00677be0 _$E254
// FUNCTION: STBC 0x00677bf0 _$E253
// FUNCTION: STBC 0x00677c00 _$E258
// FUNCTION: STBC 0x00677c10 _$E257
// FUNCTION: STBC 0x00677c20 _$E262
// FUNCTION: STBC 0x00677c30 _$E261
// FUNCTION: STBC 0x00677c40 _$E266
// FUNCTION: STBC 0x00677c50 _$E265
// FUNCTION: STBC 0x00677c60 _$E270
// FUNCTION: STBC 0x00677c70 _$E269
// FUNCTION: STBC 0x00677c80 _$E274
// FUNCTION: STBC 0x00677c90 _$E273
// FUNCTION: STBC 0x00677ca0 _$E77
// FUNCTION: STBC 0x00677cb0 _$E76
// FUNCTION: STBC 0x00677cc0 _$E278
// FUNCTION: STBC 0x00677cd0 _$E277
// FUNCTION: STBC 0x00677ce0 _$E82
// FUNCTION: STBC 0x00677cf0 _$E81
// FUNCTION: STBC 0x00677d00 _$E292
// FUNCTION: STBC 0x00677d10 _$E291
// FUNCTION: STBC 0x00677d20 _$E296
// FUNCTION: STBC 0x00677d30 _$E295
// FUNCTION: STBC 0x00677d40 _$E300
// FUNCTION: STBC 0x00677d50 _$E299
// FUNCTION: STBC 0x00677d60 _$E304
// FUNCTION: STBC 0x00677d70 _$E303
// FUNCTION: STBC 0x00677d80 _$E91
// FUNCTION: STBC 0x00677d90 _$E90
// FUNCTION: STBC 0x00677da0 _$E308
// FUNCTION: STBC 0x00677db0 _$E307
// FUNCTION: STBC 0x00677dc0 _$E312
// FUNCTION: STBC 0x00677dd0 _$E311
// FUNCTION: STBC 0x00677de0 _$E316
// FUNCTION: STBC 0x00677df0 _$E315
// FUNCTION: STBC 0x00677e00 _$E97
// FUNCTION: STBC 0x00677e10 _$E96
// FUNCTION: STBC 0x00677e20 _$E102
// FUNCTION: STBC 0x00677e30 _$E101
// FUNCTION: STBC 0x00677e40 _$E132
// FUNCTION: STBC 0x00677e50 _$E131
// FUNCTION: STBC 0x00677e90 _$E320
// FUNCTION: STBC 0x00677ea0 _$E319
// FUNCTION: STBC 0x00677eb0 _$E137
// FUNCTION: STBC 0x00677ec0 _$E136
// FUNCTION: STBC 0x00677ed0 _$E324
// FUNCTION: STBC 0x00677ee0 _$E323
// FUNCTION: STBC 0x00677ef0 _$E328
// FUNCTION: STBC 0x00677f00 _$E327
// FUNCTION: STBC 0x00677f10 _$E142
// FUNCTION: STBC 0x00677f20 _$E141
// FUNCTION: STBC 0x00677f30 _$E332
// FUNCTION: STBC 0x00677f40 _$E331
// FUNCTION: STBC 0x00677f50 _$E107
// FUNCTION: STBC 0x00677f60 _$E106
// FUNCTION: STBC 0x00677f70 _$E336
// FUNCTION: STBC 0x00677f80 _$E335
// FUNCTION: STBC 0x00677f90 _$E340
// FUNCTION: STBC 0x00677fa0 _$E339
// FUNCTION: STBC 0x00677fb0 _$E147
// FUNCTION: STBC 0x00677fc0 _$E146
// FUNCTION: STBC 0x00677fd0 _$E152
// FUNCTION: STBC 0x00677fe0 _$E151
// FUNCTION: STBC 0x00677ff0 _$E344
// FUNCTION: STBC 0x00678000 _$E343
// FUNCTION: STBC 0x00678010 _$E348
// FUNCTION: STBC 0x00678020 _$E347
// FUNCTION: STBC 0x00678030 _$E352
// FUNCTION: STBC 0x00678040 _$E351
// FUNCTION: STBC 0x00678050 _$E356
// FUNCTION: STBC 0x00678060 _$E355
// FUNCTION: STBC 0x00678070 _$E360
// FUNCTION: STBC 0x00678080 _$E359
// FUNCTION: STBC 0x00678090 _$E364
// FUNCTION: STBC 0x006780a0 _$E363
// FUNCTION: STBC 0x006780b0 _$E368
// FUNCTION: STBC 0x006780c0 _$E367
// FUNCTION: STBC 0x006780d0 _$E372
// FUNCTION: STBC 0x006780e0 _$E371
// FUNCTION: STBC 0x006780f0 _$E157
// FUNCTION: STBC 0x00678100 _$E156
// FUNCTION: STBC 0x00678110 _$E376
// FUNCTION: STBC 0x00678120 _$E375
// FUNCTION: STBC 0x00678130 _$E162
// FUNCTION: STBC 0x00678140 _$E161
// FUNCTION: STBC 0x00678150 _$E167
// FUNCTION: STBC 0x00678160 _$E166
// FUNCTION: STBC 0x00678170 _$E409
// FUNCTION: STBC 0x00678180 _$E408
// FUNCTION: STBC 0x00678190 _$E414
// FUNCTION: STBC 0x006781a0 _$E413
// FUNCTION: STBC 0x006781b0 _$E418
// FUNCTION: STBC 0x006781c0 _$E417

/*
================
ViewScreenNode::AttachChild

A new child resets the static's alpha.
================
*/
// FUNCTION: STBC 0x006761e0
void ViewScreenNode::AttachChild( NiAVObject *child, bool firstAvail ) {
	NiNode::AttachChild( child, firstAvail );
	staticAlphaMin = 1.0f;
	staticAlphaRange = 0.0f;
	Unknown676E50();
}

/*
================
ViewScreenNode::ViewScreenNode

The screen starts off.
================
*/
// FUNCTION: STBC 0x00676220
ViewScreenNode::ViewScreenNode( void ) : isOn( false ), staticIsOn( false ) {
	SetIsOn( false );
}

/*
================
ViewScreenNode::IsInView

The test of the culling pass: the screen is out of view when its bound lies
behind one of the camera's planes. The planes the test disables are put
back before the answer is returned.
================
*/
// FUNCTION: STBC 0x006763b0
bool ViewScreenNode::IsInView( CameraObjectClass *camera ) {
	NiCamera *niCamera = camera->GetNiCamera();
	NiBitVector disabled;
	bool culled = Unknown7DCA70( niCamera, disabled );
	Unknown7DCB00( niCamera, disabled );
	return !culled;
}

/*
================
ViewScreenNode::FindTriShape

The object when it is a triangle shape, else the first one under it when it
is a node; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00676400
NiTriShape *ViewScreenNode::FindTriShape( NiAVObject *object ) {
	if ( object == NULL ) {
		return NULL;
	}
	if ( object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = NiDynamicCast( NiNode, object );
		for ( unsigned int i = 0; i < node->GetChildCount(); i++ ) {
			NiTriShape *shape = FindTriShape( node->GetAt( i ) );
			if ( shape != NULL ) {
				return shape;
			}
		}
		return NULL;
	}
	if ( object->IsKindOf( &NiTriShape::rtti ) ) {
		return NiDynamicCast( NiTriShape, object );
	}
	return NULL;
}

/*
================
ViewScreenNode::SetStaticVariation

The static's alpha flickers between low and high, both kept within 0 to 1.
================
*/
// FUNCTION: STBC 0x006764e0
void ViewScreenNode::SetStaticVariation( float low, float high ) {
	if ( low > 1.0f ) {
		low = 1.0f;
	} else if ( low < 0.0f ) {
		low = 0.0f;
	}
	staticAlphaMin = low;
	staticAlphaRange = high - low;
	if ( staticAlphaRange + low > 1.0f ) {
		staticAlphaRange = 1.0f - low;
	} else if ( staticAlphaRange < 0.0f ) {
		staticAlphaRange = 0.0f;
	}
}

/*
================
ViewScreenNode::VirtualSlot44
================
*/
// FUNCTION: STBC 0x00676570
void ViewScreenNode::VirtualSlot44( float time ) {
	if ( staticIsOn && staticAlphaRange != 0.0f ) {
		material->SetAlpha( rand() * staticAlphaRange * ( 1.0f / RAND_MAX ) + staticAlphaMin );
	}
	Update( time );
}

/*
================
ViewScreenNode::SetIsOn

While the static is off, the screen's own geometry is hidden when it is on.
================
*/
// FUNCTION: STBC 0x006765d0
void ViewScreenNode::SetIsOn( bool on ) {
	isOn = on;
	if ( !staticIsOn ) {
		SetAppCulled( on );
	} else {
		SetAppCulled( false );
	}
}

/*
================
ViewScreenNode::SetStaticIsOn

Swaps the static and the off texture on the node's tree. The first call also
gives the tree the alpha, vertex colour, material and texture mode
properties the static's flicker needs.
================
*/
// FUNCTION: STBC 0x00676600
void ViewScreenNode::SetStaticIsOn( bool on ) {
	staticIsOn = on;
	RemovePropertyFromTree( this, NiTextureProperty::TEXTURE );
	if ( staticIsOn ) {
		if ( staticTexture != NULL ) {
			AttachProperty( staticTexture );
		}
	} else if ( offTexture != NULL ) {
		AttachProperty( offTexture );
	}
	if ( alpha == NULL ) {
		alpha = new NiAlphaProperty;
		RemovePropertyFromTree( this, NiAlphaProperty::ALPHA );
		AttachProperty( alpha );

		RemovePropertyFromTree( this, NiVertexColorProperty::VERTEX_COLOR );
		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		AttachProperty( vertexColor );

		RemovePropertyFromTree( this, NiMaterialProperty::MATERIAL );
		material = new NiMaterialProperty;
		material->SetEmittance( NiColor::WHITE );
		AttachProperty( material );

		RemovePropertyFromTree( this, NiTextureModeProperty::TEXTURE_MODE );
		NiTextureModeProperty *textureMode = new NiTextureModeProperty;
		textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
		textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
		AttachProperty( textureMode );
	}
	UpdateProperties();
	if ( staticIsOn ) {
		alpha->SetAlphaBlending( true );
		material->SetAlpha( staticAlphaMin );
		SetAppCulled( false );
	} else {
		alpha->SetAlphaBlending( false );
		material->SetAlpha( 1.0f );
		SetAppCulled( isOn );
	}
}

/*
================
ViewScreenNode::SetStaticTextureIconGroup

The static is the flip of the icon group's textures, ten to a second, from
now on.
================
*/
// FUNCTION: STBC 0x00676940
void ViewScreenNode::SetStaticTextureIconGroup( char *name ) {
	TGIconGroup *group = g_kIconManager.GetIconGroup( name );
	if ( group == NULL ) {
		return;
	}
	NiTArray<NiPointer<NiTextureProperty> > *textures = group->GetTextures();
	NiPointer<NiFlipController> controller( new NiFlipController );
	for ( unsigned int i = 0; i < textures->GetSize(); i++ ) {
		NiTextureProperty *texture = textures->GetAt( i );
		if ( texture == NULL || texture->GetImage() == NULL ) {
			return;
		}
		controller->SetImage( texture->GetImage(), i );
	}
	staticTexture = new NiTextureProperty;
	controller->SetAnimType( NiTimeController::APP_INIT );
	controller->SetSecondsPerFrame( 0.1f );
	controller->SetCycleType( NiTimeController::LOOP );
	controller->SetTarget( staticTexture );
	controller->SetStartTime( g_kUtopiaModule.GetGameTime() );
	controller->Start( g_kUtopiaModule.GetGameTime() );
	controller->loKeyTime = 0.0f;
	controller->hiKeyTime = controller->GetImageCount() * controller->GetSecondsPerFrame();
}

/*
================
ViewScreenNode::SetOffTexture

Makes the off texture from the image of the name; it is put on the tree at
once while the static is off.
================
*/
// FUNCTION: STBC 0x00676b20
// PARKED: PCH boundary: exact only with the unit's leading include block inside the precompiled block (/YX); the game units build without a precompiled header (accepted loss, user's ruling 2026-11-20; docs/c2-backend.md, "A/B over the tree")
void ViewScreenNode::SetOffTexture( char *name ) {
	offTexture = new NiTextureProperty;
	offTexture->SetImage( g_kImageManager.GetImage( name, true, false ) );
	if ( alpha == NULL ) {
		alpha = new NiAlphaProperty;
		RemovePropertyFromTree( this, NiAlphaProperty::ALPHA );
		AttachProperty( alpha );

		RemovePropertyFromTree( this, NiVertexColorProperty::VERTEX_COLOR );
		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		AttachProperty( vertexColor );

		RemovePropertyFromTree( this, NiMaterialProperty::MATERIAL );
		material = new NiMaterialProperty;
		material->SetEmittance( NiColor::WHITE );
		AttachProperty( material );

		RemovePropertyFromTree( this, NiTextureModeProperty::TEXTURE_MODE );
		NiTextureModeProperty *textureMode = new NiTextureModeProperty;
		textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
		textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
		AttachProperty( textureMode );
	}
	if ( !IsStaticOn() ) {
		RemovePropertyFromTree( this, NiTextureProperty::TEXTURE );
		AttachProperty( offTexture );
	}
}

/*
================
ViewScreenNode::Unknown676E50

Measures the screen's first triangle shape: the facing of its first triangle,
reversed, and the world-space bounding box of all its vertices (kept as the
box's middle and half its size).
================
*/
// FUNCTION: STBC 0x00676e50
// PARKED: zero-extension: retail reads triList[0] with mov ax; and eax, 0xffff where this build pre-zeroes eax (xor; mov ax); the rest agrees
void ViewScreenNode::Unknown676E50( void ) {
	NiTriShape *shape = FindTriShape( GetAt( 0 ) );
	if ( shape == NULL ) {
		return;
	}
	VirtualSlot44( 0.0f );

	NiTriShapeData *data = shape->GetShapeData();
	unsigned short *triList = data->GetTriList();
	unsigned int count = data->GetVertexCount();
	NiPoint3 *vertices = data->GetVertices();

	NiPoint3 &first = vertices[triList[0]];
	NiPoint3 edge1 = vertices[triList[1]] - first;
	NiPoint3 edge2 = vertices[triList[2]] - first;
	screenNormal = edge1.Cross( edge2 );
	screenNormal.Unitize();
	screenNormal = -screenNormal;

	NiPoint3 lower( NI_INFINITY, NI_INFINITY, NI_INFINITY );
	NiPoint3 upper( -NI_INFINITY, -NI_INFINITY, -NI_INFINITY );
	for ( ; count > 0; count-- ) {
		NiPoint3 world = TGModelUtils::LocalToWorldPoint( shape, *vertices );
		if ( world.x < lower.x ) {
			lower.x = world.x;
		} else if ( world.x > upper.x ) {
			upper.x = world.x;
		}
		if ( world.y < lower.y ) {
			lower.y = world.y;
		} else if ( world.y > upper.y ) {
			upper.y = world.y;
		}
		if ( world.z < lower.z ) {
			lower.z = world.z;
		} else if ( world.z > upper.z ) {
			upper.z = world.z;
		}
		vertices++;
	}
	screenHalfExtent = ( upper - lower ) * 0.5f;
	screenCenter = 0.5f * ( upper + lower );
}

/*
================
ViewScreenNode::FindScreenPort

Projects the corners of the screen's bounding box into the view camera's
port and keeps the rect they span, clipped to the port, with how far it ran
past each edge and its full size. False when the rect lies wholly outside
the port; the port is then the camera's own. The screen camera is not used.
================
*/
// FUNCTION: STBC 0x006771a0
// DRAFT: corner build: retail copies the box corners through NiPoint3 temporaries (fst/mov chains, frame 0x8c), this build stores them straight; loop and clipping tail match up to slots and store order
bool ViewScreenNode::FindScreenPort( NiCamera *viewCamera, NiCamera *screenCamera ) {
	port = viewCamera->GetViewPort();

	float maxX = -NI_INFINITY;
	float maxY = -NI_INFINITY;
	float minX = NI_INFINITY;
	float minY = NI_INFINITY;

	NiPoint3 upper = screenCenter + screenHalfExtent;
	NiPoint3 lower = screenCenter - screenHalfExtent;
	NiPoint3 corners[8] = {
		NiPoint3( upper.x, upper.y, upper.z ),
		NiPoint3( lower.x, upper.y, upper.z ),
		NiPoint3( lower.x, lower.y, upper.z ),
		NiPoint3( lower.x, lower.y, lower.z ),
		NiPoint3( lower.x, upper.y, lower.z ),
		NiPoint3( upper.x, upper.y, lower.z ),
		NiPoint3( upper.x, lower.y, lower.z ),
		NiPoint3( upper.x, lower.y, upper.z )
	};
	for ( int i = 0; i < 8; i++ ) {
		float x, y;
		ProjectPoint( viewCamera, corners[i], x, y );
		if ( x > maxX ) {
			maxX = x;
		}
		if ( x < minX ) {
			minX = x;
		}
		if ( y > maxY ) {
			maxY = y;
		}
		if ( y < minY ) {
			minY = y;
		}
	}
	if ( maxX < 0.0f || minX > 1.0f || maxY < 0.0f || minY > 1.0f ) {
		return false;
	}

	port.right = maxX;
	unknown11C[4] = maxX - minX;
	port.left = minX;
	port.top = maxY;
	unknown11C[5] = maxY - minY;
	port.bottom = minY;
	unknown11C[0] = 0.0f;
	unknown11C[1] = 0.0f;
	unknown11C[2] = 0.0f;
	unknown11C[3] = 0.0f;
	if ( minX < 0.0f ) {
		unknown11C[0] = -minX;
		port.left = 0.0f;
	}
	if ( maxX > 1.0f ) {
		unknown11C[1] = maxX - 1.0f;
		port.right = 1.0f;
	}
	if ( maxY > 1.0f ) {
		unknown11C[2] = maxY - 1.0f;
		port.top = 1.0f;
	}
	if ( minY < 0.0f ) {
		unknown11C[3] = -minY;
		port.bottom = 0.0f;
	}
	return true;
}

/*
================
ViewScreenNode::FitScreenCamera

Widens the screen camera's frustum to the screen's proportions, crops it
where the screen's rect was clipped, and gives it the rect as its port. The
view camera is not used.
================
*/
// FUNCTION: STBC 0x00677540
bool ViewScreenNode::FitScreenCamera( NiCamera *viewCamera, NiCamera *screenCamera ) {
	NiFrustum frustum = screenCamera->GetViewFrustum();
	float width = frustum.right - frustum.left;
	float height = frustum.top - frustum.bottom;
	float extra = screenHalfExtent.x / screenHalfExtent.z * height - width;
	width += extra;
	frustum.left -= extra * 0.5f;
	frustum.right += extra * 0.5f;
	frustum.left += unknown11C[0] / unknown11C[4] * width;
	frustum.right -= unknown11C[1] / unknown11C[4] * width;
	frustum.top -= unknown11C[2] / unknown11C[5] * height;
	frustum.bottom += unknown11C[3] / unknown11C[5] * height;
	screenCamera->SetViewPort( port );
	screenCamera->SetViewFrustum( frustum );
	return true;
}

/*
================
ViewScreenNode::ProjectPoint

NiCamera::WorldPtToScreenPt without its tests of the frustum's sides: the
point's place in the camera's port, left unset when it is behind the eye.
================
*/
// FUNCTION: STBC 0x00677680
void ViewScreenNode::ProjectPoint( NiCamera *camera, const NiPoint3 &point, float &x, float &y ) {
	float ptX = point.x;
	float ptY = point.y;
	float ptZ = point.z;
	const float *m = camera->GetWorldToCameraMatrix();
	double w = ptX * m[12] + ptY * m[13] + ptZ * m[14] + m[15];
	if ( w > 1e-5f ) {
		double invW = 1.0 / w;
		x = ptX * m[0] + ptY * m[1] + ptZ * m[2] + m[3];
		y = ptX * m[4] + ptY * m[5] + ptZ * m[6] + m[7];
		x *= invW;
		y *= invW;
		x = ( camera->GetViewPort().right - camera->GetViewPort().left ) * x * 0.5f;
		y = ( camera->GetViewPort().top - camera->GetViewPort().bottom ) * y * 0.5f;
		x += ( camera->GetViewPort().left + camera->GetViewPort().right ) * 0.5f;
		y += ( camera->GetViewPort().bottom + camera->GetViewPort().top ) * 0.5f;
	}
}
