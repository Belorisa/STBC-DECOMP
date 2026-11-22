// NiEnvMappedTriShape.cpp -- environment-mapped triangle list geometry objects

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiCamera.h"
#include "NiEnvMappedTriShape.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fb900 _$E37
// FUNCTION: STBC 0x007fb910 _$E45
// FUNCTION: STBC 0x007fb920 _$E49
// FUNCTION: STBC 0x007fb930 _$E53
// FUNCTION: STBC 0x007fb940 _$E57
// FUNCTION: STBC 0x007fb950 _$E181
// FUNCTION: STBC 0x007fb960 _$E41
// FUNCTION: STBC 0x007fb970 _$E123
// FUNCTION: STBC 0x007fb980 _$E136
// FUNCTION: STBC 0x007fb990 _$E186
// FUNCTION: STBC 0x007fb9a0 _$E191
// FUNCTION: STBC 0x007fb9b0 _$E195
// FUNCTION: STBC 0x007fb9c0 _$E199
// FUNCTION: STBC 0x007fb9d0 _$E206
// FUNCTION: STBC 0x007fb9e0 _$E214
// FUNCTION: STBC 0x007fb9f0 _$E221
// FUNCTION: STBC 0x007fba00 _$E225
// FUNCTION: STBC 0x007fba10 _$E229
// FUNCTION: STBC 0x007fba20 _$E233
// FUNCTION: STBC 0x007fba30 _$E239
// FUNCTION: STBC 0x007fba40 _$E243
// FUNCTION: STBC 0x007fba90 _$E247

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007fb8e0 _$E260
// FUNCTION: STBC 0x007fb880 _$E17
// FUNCTION: STBC 0x007fb8a0 _$E15
// FUNCTION: STBC 0x007fb8b0 _$E23
// FUNCTION: STBC 0x007fb8d0 _$E21

// FUNCTION: STBC 0x007fb090 _$E253
// FUNCTION: STBC 0x007fb0b0 _$E250
// DATA: STBC 0x9A1FD8 ?rtti@NiEnvMappedTriShape@@2VNiRTTI@@B
const NiRTTI NiEnvMappedTriShape::rtti( "NiEnvMappedTriShape", &NiTriShape::rtti );

/*
================
NiEnvMappedTriShape::Register
================
*/
// FUNCTION: STBC 0x7FB0D0 ?Register@NiEnvMappedTriShape@@SAHXZ
NiImplementRegister( NiEnvMappedTriShape )

/*
================
NiEnvMappedTriShape::NiEnvMappedTriShape

Creates the data, which takes ownership of the arrays, and the world
normals the texture coordinates are generated from.
================
*/
// FUNCTION: STBC 0x7FB220 ??0NiEnvMappedTriShape@@QAE@GPAVNiPoint3@@0PAVNiColorA@@GPAG@Z
NiEnvMappedTriShape::NiEnvMappedTriShape( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, unsigned short triangleCount_, unsigned short *triList_ )
	: NiTriShape( new NiEnvMappedTriShapeData( vertexCount_, vertices_, normals_, colors_, triangleCount_, triList_ ) ) {
	CreateWorldNormals();
}

/*
================
NiEnvMappedTriShape::NiEnvMappedTriShape
================
*/
// FUNCTION: STBC 0x7FB2C0 ??0NiEnvMappedTriShape@@QAE@PAVNiEnvMappedTriShapeData@@@Z
NiEnvMappedTriShape::NiEnvMappedTriShape( NiEnvMappedTriShapeData *data ) : NiTriShape( data ) {
	CreateWorldNormals();
}

/*
================
NiEnvMappedTriShape::NiEnvMappedTriShape
================
*/
// FUNCTION: STBC 0x7FB310 ??0NiEnvMappedTriShape@@QAE@XZ
NiEnvMappedTriShape::NiEnvMappedTriShape() {
}

/*
================
NiEnvMappedTriShape::Display

Regenerates the texture coordinates for the camera, then draws the shape
and counts it.
================
*/
// FUNCTION: STBC 0x7FB330
void NiEnvMappedTriShape::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	if ( !renderer->RegisterObject( this ) ) {
		UpdateEnvMapTextures( camera->GetWorldLocation() );
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot45( GetShapeData(), GetWorldBound() );
		shapesDrawn++;
		trianglesDrawn += GetShapeData()->GetTriangleCount();
		verticesDrawn += GetShapeData()->GetVertexCount();
	}
}

/*
================
NiEnvMappedTriShape::UpdateEnvMapTextures

Reflects the direction from the shape's world bound to the camera about
each world normal and sphere-maps the reflection into the vertex's texture
coordinates, then marks the data changed. Without world normals nothing
is generated.
================
*/
// FUNCTION: STBC 0x7FB3F0
void NiEnvMappedTriShape::UpdateEnvMapTextures( const NiPoint3 &cameraLocation ) {
	NiPoint3 view = cameraLocation - GetWorldBound().GetCenter();
	view.Unitize();

	UpdateWorldNormals();

	unsigned short vertexCount = modelData->GetVertexCount();
	NiPoint2 *textures = modelData->GetTextures();
	NiPoint3 *normal = worldNormals;
	if ( !normal ) {
		return;
	}

	for ( unsigned short i = 0; i < vertexCount; i++ ) {
		NiPoint3 reflect = view - 2.0f * ( view * *normal ) * *normal;
		textures->y = ( reflect.y + 1.0f ) * 0.5f;
		if ( reflect.x < 0.0f ) {
			textures->x = ( reflect.z + 3.0f ) * 0.25f;
		} else {
			textures->x = ( 1.0f - reflect.z ) * 0.25f;
		}
		textures++;
		normal++;
	}
	modelData->IncRevisionID();
}

/*
================
NiEnvMappedTriShape::CreateClone
================
*/
// FUNCTION: STBC 0x7FB570
NiObject *NiEnvMappedTriShape::CreateClone() {
	NiEnvMappedTriShape *clone = new NiEnvMappedTriShape;
	CopyMembers( clone );
	return clone;
}

/*
================
NiEnvMappedTriShape::CopyMembers

The copy gets world normals of its own.
================
*/
// FUNCTION: STBC 0x7FB5F0
void NiEnvMappedTriShape::CopyMembers( NiEnvMappedTriShape *dest ) {
	NiTriShape::CopyMembers( dest );
	dest->CreateWorldNormals();
}

/*
================
NiEnvMappedTriShape::CreateObject

Creates the shape and loads it.
================
*/
// FUNCTION: STBC 0x7FB610
void NiEnvMappedTriShape::CreateObject( NiStream &stream, NiObject *&object ) {
	NiEnvMappedTriShape *shape = new NiEnvMappedTriShape;
	stream.SetLoadRecord( new NiEnvMappedTriShapeLoadRecord );
	shape->LoadBinary( stream );
	object = shape;
}

/*
================
NiEnvMappedTriShape::LoadBinary
================
*/
// FUNCTION: STBC 0x7FB720
void NiEnvMappedTriShape::LoadBinary( NiStream &stream ) {
	NiTriShape::LoadBinary( stream );
}

/*
================
NiEnvMappedTriShape::LinkObject

Creates the world normals once the model data is linked.
================
*/
// FUNCTION: STBC 0x7FB730
void NiEnvMappedTriShape::LinkObject( NiStream &stream ) {
	NiTriShape::LinkObject( stream );
	CreateWorldNormals();
}

/*
================
NiEnvMappedTriShape::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FB750
bool NiEnvMappedTriShape::RegisterStreamables( NiStream &stream ) {
	return NiTriShape::RegisterStreamables( stream );
}

/*
================
NiEnvMappedTriShape::SaveBinary
================
*/
// FUNCTION: STBC 0x7FB760
void NiEnvMappedTriShape::SaveBinary( NiStream &stream ) {
	NiTriShape::SaveBinary( stream );
}

/*
================
NiEnvMappedTriShape::IsEqual
================
*/
// FUNCTION: STBC 0x7FB770
bool NiEnvMappedTriShape::IsEqual( NiObject *object ) {
	return NiTriShape::IsEqual( object );
}

/*
================
NiEnvMappedTriShape::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FB780
void NiEnvMappedTriShape::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriShape::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
