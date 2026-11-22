// NiAutoNormalParticles.cpp -- particles with camera-facing normals

#include "NiRTLib.h"
#include <assert.h>
#include "NiCamera.h"
#include "NiStream.h"
#include "NiAutoNormalParticles.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f63d0 _$E30
// FUNCTION: STBC 0x007f63e0 _$E45
// FUNCTION: STBC 0x007f63f0 _$E49
// FUNCTION: STBC 0x007f6400 _$E53
// FUNCTION: STBC 0x007f6410 _$E57
// FUNCTION: STBC 0x007f6420 _$E181
// FUNCTION: STBC 0x007f6430 _$E34
// FUNCTION: STBC 0x007f6440 _$E123
// FUNCTION: STBC 0x007f6450 _$E136
// FUNCTION: STBC 0x007f6460 _$E186
// FUNCTION: STBC 0x007f6470 _$E191
// FUNCTION: STBC 0x007f6480 _$E195
// FUNCTION: STBC 0x007f6490 _$E199
// FUNCTION: STBC 0x007f64a0 _$E206
// FUNCTION: STBC 0x007f64b0 _$E214
// FUNCTION: STBC 0x007f64c0 _$E221
// FUNCTION: STBC 0x007f64d0 _$E225
// FUNCTION: STBC 0x007f64e0 _$E229
// FUNCTION: STBC 0x007f64f0 _$E233
// FUNCTION: STBC 0x007f6500 _$E237
// FUNCTION: STBC 0x007f6510 _$E241
// FUNCTION: STBC 0x007f6560 _$E245

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f63b0 _$E259
// FUNCTION: STBC 0x007f6350 _$E17
// FUNCTION: STBC 0x007f6370 _$E15
// FUNCTION: STBC 0x007f6380 _$E23
// FUNCTION: STBC 0x007f63a0 _$E21

// FUNCTION: STBC 0x007f5d10 _$E251
// FUNCTION: STBC 0x007f5d30 _$E248
// DATA: STBC 0x9A1D58 ?rtti@NiAutoNormalParticles@@2VNiRTTI@@B
const NiRTTI NiAutoNormalParticles::rtti( "NiAutoNormalParticles", &NiParticles::rtti );

/*
================
NiAutoNormalParticles::Register
================
*/
// FUNCTION: STBC 0x7F5D50 ?Register@NiAutoNormalParticles@@SAHXZ
NiImplementRegister( NiAutoNormalParticles )

/*
================
NiAutoNormalParticles::NiAutoNormalParticles

Creates the data, which takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x7F5EA0 ??0NiAutoNormalParticles@@QAE@GPAVNiPoint3@@PAVNiColorA@@@Z
NiAutoNormalParticles::NiAutoNormalParticles( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_ )
	: NiParticles( new NiAutoNormalParticlesData( vertexCount_, vertices_, colors_ ) ) {
}

/*
================
NiAutoNormalParticles::NiAutoNormalParticles
================
*/
// FUNCTION: STBC 0x7F5F20 ??0NiAutoNormalParticles@@QAE@PAVNiAutoNormalParticlesData@@@Z
NiAutoNormalParticles::NiAutoNormalParticles( NiAutoNormalParticlesData *data ) : NiParticles( data ) {
}

/*
================
NiAutoNormalParticles::NiAutoNormalParticles
================
*/
// FUNCTION: STBC 0x7F5F40 ??0NiAutoNormalParticles@@QAE@XZ
NiAutoNormalParticles::NiAutoNormalParticles() {
}

/*
================
NiAutoNormalParticles::Display

Points every normal against the camera direction, then draws as NiParticles does.
================
*/
// FUNCTION: STBC 0x7F5F60
void NiAutoNormalParticles::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	assert( renderer );
	if ( !renderer->RegisterObject( this ) ) {
		const NiPoint3 &toCamera = -camera->GetWorldDirection();	// (name invented; fits the /Od frame order)
		NiPoint3 *normals = GetNormals();
		for ( unsigned short i = 0; i < GetActiveParticleCount(); i++ ) {
			*normals++ = toCamera;
		}
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot49( NiSmartPointerCast( NiParticlesData, modelData ), GetWorldBound() );
		shapesDrawn++;
		trianglesDrawn += GetActiveParticleCount();
		verticesDrawn += GetActiveParticleCount();
	}
}

/*
================
NiAutoNormalParticles::CreateClone
================
*/
// FUNCTION: STBC 0x7F6070
NiObject *NiAutoNormalParticles::CreateClone() {
	NiAutoNormalParticles *clone = new NiAutoNormalParticles;
	CopyMembers( clone );
	return clone;
}

/*
================
NiAutoNormalParticles::CopyMembers
================
*/
// FUNCTION: STBC 0x7F60E0
void NiAutoNormalParticles::CopyMembers( NiAutoNormalParticles *dest ) {
	NiParticles::CopyMembers( dest );
}

/*
================
NiAutoNormalParticles::CreateObject

Creates the particles and loads them.
================
*/
// FUNCTION: STBC 0x7F60F0
void NiAutoNormalParticles::CreateObject( NiStream &stream, NiObject *&object ) {
	NiAutoNormalParticles *particles = new NiAutoNormalParticles;
	stream.SetLoadRecord( new NiAutoNormalParticlesLoadRecord );
	particles->LoadBinary( stream );
	object = particles;
}

/*
================
NiAutoNormalParticles::LoadBinary
================
*/
// FUNCTION: STBC 0x7F6200
void NiAutoNormalParticles::LoadBinary( NiStream &stream ) {
	NiParticles::LoadBinary( stream );
}

/*
================
NiAutoNormalParticles::LinkObject
================
*/
// FUNCTION: STBC 0x7F6210
void NiAutoNormalParticles::LinkObject( NiStream &stream ) {
	NiParticles::LinkObject( stream );
}

/*
================
NiAutoNormalParticles::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F6220
bool NiAutoNormalParticles::RegisterStreamables( NiStream &stream ) {
	return NiParticles::RegisterStreamables( stream );
}

/*
================
NiAutoNormalParticles::SaveBinary
================
*/
// FUNCTION: STBC 0x7F6230
void NiAutoNormalParticles::SaveBinary( NiStream &stream ) {
	NiParticles::SaveBinary( stream );
}

/*
================
NiAutoNormalParticles::IsEqual
================
*/
// FUNCTION: STBC 0x7F6240
bool NiAutoNormalParticles::IsEqual( NiObject *object ) {
	return NiParticles::IsEqual( object );
}

/*
================
NiAutoNormalParticles::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F6250
void NiAutoNormalParticles::GetViewerStrings( NiTArray<char *> *strings ) {
	NiParticles::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
