/*
===============================================================================

	StarSphere.cpp

===============================================================================
*/

#include <iostream>
#include "StarSphere.h"
#include "TGStream.h"
#include "ImageManagerClass.h"
#include "NiRawImageData.h"
#include "NiImageFile.h"
#include "NiCamera.h"
#include "NiTriShape.h"
#include "NiMath.h"
#include "TGIconManager.h"
#include "NiVertexColorProperty.h"
#include "NiAlphaProperty.h"
#include "NiTextureProperty.h"
#include "NiTextureModeProperty.h"
#include "NiMaterialProperty.h"
#include <math.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0058f3f0 _$E445
// FUNCTION: STBC 0x0058f410 _$E444

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0058ed90 _$E32
// FUNCTION: STBC 0x0058eda0 _$E29
// FUNCTION: STBC 0x0058edb0 _$E31
// FUNCTION: STBC 0x0058edc0 _$E30
// FUNCTION: STBC 0x0058edd0 _$E38
// FUNCTION: STBC 0x0058ede0 _$E35
// FUNCTION: STBC 0x0058edf0 _$E37
// FUNCTION: STBC 0x0058ee00 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0058ee10 _$E66
// FUNCTION: STBC 0x0058ee20 _$E65
// FUNCTION: STBC 0x0058ee30 _$E226
// FUNCTION: STBC 0x0058ee40 _$E225
// FUNCTION: STBC 0x0058ee50 _$E77
// FUNCTION: STBC 0x0058ee60 _$E76
// FUNCTION: STBC 0x0058ee70 _$E81
// FUNCTION: STBC 0x0058ee80 _$E80
// FUNCTION: STBC 0x0058ee90 _$E85
// FUNCTION: STBC 0x0058eea0 _$E84
// FUNCTION: STBC 0x0058eeb0 _$E89
// FUNCTION: STBC 0x0058eec0 _$E88
// FUNCTION: STBC 0x0058eed0 _$E206
// FUNCTION: STBC 0x0058eee0 _$E205
// FUNCTION: STBC 0x0058eef0 _$E148
// FUNCTION: STBC 0x0058ef00 _$E147
// FUNCTION: STBC 0x0058ef10 _$E161
// FUNCTION: STBC 0x0058ef20 _$E160
// FUNCTION: STBC 0x0058ef30 _$E211
// FUNCTION: STBC 0x0058ef40 _$E210
// FUNCTION: STBC 0x0058ef50 _$E231
// FUNCTION: STBC 0x0058ef60 _$E230
// FUNCTION: STBC 0x0058ef70 _$E215
// FUNCTION: STBC 0x0058ef80 _$E214
// FUNCTION: STBC 0x0058ef90 _$E235
// FUNCTION: STBC 0x0058efa0 _$E234
// FUNCTION: STBC 0x0058efb0 _$E242
// FUNCTION: STBC 0x0058efc0 _$E241
// FUNCTION: STBC 0x0058efd0 _$E285
// FUNCTION: STBC 0x0058efe0 _$E284
// FUNCTION: STBC 0x0058eff0 _$E289
// FUNCTION: STBC 0x0058f000 _$E288
// FUNCTION: STBC 0x0058f010 _$E106
// FUNCTION: STBC 0x0058f020 _$E105
// FUNCTION: STBC 0x0058f030 _$E250
// FUNCTION: STBC 0x0058f040 _$E249
// FUNCTION: STBC 0x0058f050 _$E257
// FUNCTION: STBC 0x0058f060 _$E256
// FUNCTION: STBC 0x0058f070 _$E261
// FUNCTION: STBC 0x0058f080 _$E260
// FUNCTION: STBC 0x0058f090 _$E265
// FUNCTION: STBC 0x0058f0a0 _$E264
// FUNCTION: STBC 0x0058f0b0 _$E269
// FUNCTION: STBC 0x0058f0c0 _$E268
// FUNCTION: STBC 0x0058f0d0 _$E275
// FUNCTION: STBC 0x0058f0e0 _$E274
// FUNCTION: STBC 0x0058f0f0 _$E293
// FUNCTION: STBC 0x0058f100 _$E292
// FUNCTION: STBC 0x0058f110 _$E297
// FUNCTION: STBC 0x0058f120 _$E296
// FUNCTION: STBC 0x0058f130 _$E301
// FUNCTION: STBC 0x0058f140 _$E300
// FUNCTION: STBC 0x0058f150 _$E305
// FUNCTION: STBC 0x0058f160 _$E304
// FUNCTION: STBC 0x0058f170 _$E309
// FUNCTION: STBC 0x0058f180 _$E308
// FUNCTION: STBC 0x0058f190 _$E313
// FUNCTION: STBC 0x0058f1a0 _$E312
// FUNCTION: STBC 0x0058f1b0 _$E317
// FUNCTION: STBC 0x0058f1c0 _$E316
// FUNCTION: STBC 0x0058f1d0 _$E321
// FUNCTION: STBC 0x0058f1e0 _$E320
// FUNCTION: STBC 0x0058f1f0 _$E325
// FUNCTION: STBC 0x0058f200 _$E324
// FUNCTION: STBC 0x0058f210 _$E329
// FUNCTION: STBC 0x0058f220 _$E328
// FUNCTION: STBC 0x0058f230 _$E111
// FUNCTION: STBC 0x0058f240 _$E110
// FUNCTION: STBC 0x0058f250 _$E333
// FUNCTION: STBC 0x0058f260 _$E332
// FUNCTION: STBC 0x0058f270 _$E116
// FUNCTION: STBC 0x0058f280 _$E115
// FUNCTION: STBC 0x0058f290 _$E337
// FUNCTION: STBC 0x0058f2a0 _$E336
// FUNCTION: STBC 0x0058f2b0 _$E341
// FUNCTION: STBC 0x0058f2c0 _$E340
// FUNCTION: STBC 0x0058f2d0 _$E345
// FUNCTION: STBC 0x0058f2e0 _$E344
// FUNCTION: STBC 0x0058f2f0 _$E349
// FUNCTION: STBC 0x0058f300 _$E348
// FUNCTION: STBC 0x0058f310 _$E125
// FUNCTION: STBC 0x0058f320 _$E124
// FUNCTION: STBC 0x0058f330 _$E353
// FUNCTION: STBC 0x0058f340 _$E352
// FUNCTION: STBC 0x0058f350 _$E357
// FUNCTION: STBC 0x0058f360 _$E356
// FUNCTION: STBC 0x0058f370 _$E361
// FUNCTION: STBC 0x0058f380 _$E360
// FUNCTION: STBC 0x0058f390 _$E131
// FUNCTION: STBC 0x0058f3a0 _$E130
// FUNCTION: STBC 0x0058f3b0 _$E136
// FUNCTION: STBC 0x0058f3c0 _$E135
// FUNCTION: STBC 0x0058f3d0 _$E166
// FUNCTION: STBC 0x0058f3e0 _$E165
// FUNCTION: STBC 0x0058f420 _$E365
// FUNCTION: STBC 0x0058f430 _$E364
// FUNCTION: STBC 0x0058f440 _$E171
// FUNCTION: STBC 0x0058f450 _$E170
// FUNCTION: STBC 0x0058f460 _$E369
// FUNCTION: STBC 0x0058f470 _$E368
// FUNCTION: STBC 0x0058f480 _$E373
// FUNCTION: STBC 0x0058f490 _$E372
// FUNCTION: STBC 0x0058f4a0 _$E176
// FUNCTION: STBC 0x0058f4b0 _$E175
// FUNCTION: STBC 0x0058f4c0 _$E377
// FUNCTION: STBC 0x0058f4d0 _$E376
// FUNCTION: STBC 0x0058f4e0 _$E141
// FUNCTION: STBC 0x0058f4f0 _$E140
// FUNCTION: STBC 0x0058f500 _$E381
// FUNCTION: STBC 0x0058f510 _$E380
// FUNCTION: STBC 0x0058f520 _$E385
// FUNCTION: STBC 0x0058f530 _$E384
// FUNCTION: STBC 0x0058f540 _$E181
// FUNCTION: STBC 0x0058f550 _$E180
// FUNCTION: STBC 0x0058f560 _$E186
// FUNCTION: STBC 0x0058f570 _$E185
// FUNCTION: STBC 0x0058f580 _$E389
// FUNCTION: STBC 0x0058f590 _$E388
// FUNCTION: STBC 0x0058f5a0 _$E393
// FUNCTION: STBC 0x0058f5b0 _$E392
// FUNCTION: STBC 0x0058f5c0 _$E397
// FUNCTION: STBC 0x0058f5d0 _$E396
// FUNCTION: STBC 0x0058f5e0 _$E401
// FUNCTION: STBC 0x0058f5f0 _$E400
// FUNCTION: STBC 0x0058f600 _$E405
// FUNCTION: STBC 0x0058f610 _$E404
// FUNCTION: STBC 0x0058f620 _$E409
// FUNCTION: STBC 0x0058f630 _$E408
// FUNCTION: STBC 0x0058f640 _$E413
// FUNCTION: STBC 0x0058f650 _$E412
// FUNCTION: STBC 0x0058f660 _$E417
// FUNCTION: STBC 0x0058f670 _$E416
// FUNCTION: STBC 0x0058f680 _$E191
// FUNCTION: STBC 0x0058f690 _$E190
// FUNCTION: STBC 0x0058f6a0 _$E421
// FUNCTION: STBC 0x0058f6b0 _$E420
// FUNCTION: STBC 0x0058f6c0 _$E196
// FUNCTION: STBC 0x0058f6d0 _$E195
// FUNCTION: STBC 0x0058f6e0 _$E201
// FUNCTION: STBC 0x0058f6f0 _$E200
// FUNCTION: STBC 0x0058f700 _$E425
// FUNCTION: STBC 0x0058f710 _$E424
// FUNCTION: STBC 0x0058f720 _$E430
// FUNCTION: STBC 0x0058f730 _$E429
// FUNCTION: STBC 0x0058f740 _$E434
// FUNCTION: STBC 0x0058f750 _$E433

/*
================
StarSphere::Cast

The object as a StarSphere; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0058d240
StarSphere *StarSphere::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STARSPHERE_TYPE ) ) {
		return (StarSphere *)object;
	}
	return NULL;
}

/*
================
StarSphere::Create
================
*/
// FUNCTION: STBC 0x0058e3d0
StarSphere *StarSphere::Create( void ) {
	return new StarSphere;
}

/*
================
StarSphere::StarSphere
================
*/
// FUNCTION: STBC 0x0058e400
StarSphere::StarSphere( long id ) : BackdropSphere( id ) {
	textureWidth = 0;
	textureScale = 0;
}

/*
================
StarSphere::GetObjType
================
*/
// FUNCTION: STBC 0x0058e430
int StarSphere::GetObjType( void ) {
	return STARSPHERE_TYPE;
}

/*
================
StarSphere::IsTypeOf
================
*/
// FUNCTION: STBC 0x0058e440
bool StarSphere::IsTypeOf( int type ) {
	if ( type == STARSPHERE_TYPE ) {
		return true;
	}
	return BackdropSphere::IsTypeOf( type );
}

/*
================
StarSphere::GetClassName
================
*/
// FUNCTION: STBC 0x0058e4b0
char *StarSphere::GetClassName( void ) {
	return "StarSphere";
}

/*
================
StarSphere::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0058e4c0
char *StarSphere::GetSwigTypeName( void ) {
	return "_p_StarSphere";
}

/*
================
StarSphere::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0058e4d0
char *StarSphere::GetPythonClassName( void ) {
	return "StarSpherePtr";
}

/*
================
StarSphere::VirtualSlot71

Builds the star sphere's mesh: a full sphere of the grid size's rows and
columns at the sphere radius, facing inward through its normals, with a
second texture set that keeps the unscaled coordinates for
UpdateTextureScale. Lit by nothing, additively blended, unsorted. NULL
while the sphere has no texture.
================
*/
// FUNCTION: STBC 0x0058e520
NiPointer<NiAVObject> StarSphere::VirtualSlot71( void ) {
	if ( image == NULL ) {
		return NULL;
	}

	int across;
	int down;
	GetGridSize( &across, &down );
	int vertexCount = across * down;
	int triangleCount = ( 2 * across - 2 ) * ( down - 1 );

	NiPoint3 *vertices = new NiPoint3[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount * 2];
	NiPoint3 *normals = new NiPoint3[vertexCount];
	unsigned short *triList = new unsigned short[triangleCount * 3];

	float theta = 0.0f;
	float phiStep = NI_TWO_PI / ( across - 1 );
	float thetaStep = NI_PI / ( down - 1 );
	int triangle = 0;
	for ( int row = 0; row < down; row++ ) {
		float phi = -NI_PI;
		float v = (float)row / ( down - 1 ) * 0.5f;
		float cosTheta = (float)cos( theta );
		float sinTheta = (float)sin( theta );
		for ( int column = 0; column < across; column++ ) {
			float sinPhi = (float)sin( phi );
			int index = row * across + column;
			normals[index] = NiPoint3( sinPhi * sinTheta, (float)cos( phi ) * sinTheta, cosTheta );
			vertices[index] = sphereRadius * normals[index];
			textures[vertexCount + index] = textures[index] = NiPoint2( (float)column / ( across - 1 ), v );
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
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetNoSorter( true );
	shape->AttachProperty( alpha );

	if ( image != NULL ) {
		NiTextureProperty *texture = new NiTextureProperty;
		texture->SetImage( image );
		shape->AttachProperty( texture );
	}

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( g_kImageManager.GetTextureFilterMode() );
	textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_WRAP_T );
	shape->AttachProperty( textureMode );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor::WHITE );
	shape->AttachProperty( material );

	shape->UpdateProperties();
	return shape;
}

/*
================
StarSphere::SetTextureFileName

Reads the file once for its width, loads the texture of the name and keeps a
copy of the name; a NULL name leaves the sphere without a texture file name.
================
*/
// FUNCTION: STBC 0x0058eaa0
void StarSphere::SetTextureFileName( char *name ) {
	delete [] textureFileName;
	textureFileName = NULL;
	if ( name != NULL ) {
		NiPointer<NiRawImageData> raw = new NiRawImageData;
		if ( NiReadImageFile( name, raw ) ) {
			textureWidth = (float)raw->GetWidth();
		}
		image = g_kImageManager.GetImage( name, true, false );
		textureFileName = new char[strlen( name ) + 1];
		strcpy( textureFileName, name );
	}
}

/*
================
StarSphere::UpdateTextureScale

Scales the sphere's texture coordinates by the screen width over the part
of a turn the camera's view spans, in texture widths, once that scale is
more than 2 from the one last applied. The scaled coordinates are made
from the second texture set, which keeps the unscaled ones.
================
*/
// FUNCTION: STBC 0x0058ebe0
void StarSphere::UpdateTextureScale( NiCamera *camera ) {
	if ( camera == NULL ) {
		return;
	}
	NiAVObject *object = niObject;
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape == NULL ) {
		return;
	}
	const NiFrustum &frustum = camera->GetViewFrustum();
	float scale = g_kIconManager.GetScreenWidth() * 0.6f / ( (float)atan( ( frustum.right - frustum.left ) * 0.5f ) / NI_TWO_PI * textureWidth );
	if ( fabsf( textureScale - scale ) <= 2.0f ) {
		return;
	}
	NiTriShapeData *data = shape->GetShapeData();
	NiPoint2 *textures = data->GetTextures();
	if ( textures != NULL ) {
		unsigned short count = data->GetVertexCount();
		for ( unsigned short i = 0; i < count; i++ ) {
			textures[i] = textures[count + i] * scale;
		}
	}
	shape->GetShapeData()->IncRevisionID();
	textureScale = scale;
}

/*
================
StarSphere::VirtualSlot67

The first lines of the star sphere's script: its name and its creation.
================
*/
// FUNCTION: STBC 0x0058ed00
bool StarSphere::VirtualSlot67( FILE *file ) {
	fprintf( file, "\t# Star Sphere \"%s\"\n", name );
	fprintf( file, "\tkThis = App.StarSphere_Create()\n" );
	return true;
}

/*
================
StarSphere::WriteToStream
================
*/
// FUNCTION: STBC 0x0058ed30
bool StarSphere::WriteToStream( TGStream *stream ) {
	if ( !BackdropSphere::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( textureWidth );
	return true;
}

/*
================
StarSphere::ReadFromStream
================
*/
// FUNCTION: STBC 0x0058ed60
bool StarSphere::ReadFromStream( TGStream *stream ) {
	if ( !BackdropSphere::ReadFromStream( stream ) ) {
		return false;
	}
	textureWidth = stream->ReadFloat();
	return true;
}
