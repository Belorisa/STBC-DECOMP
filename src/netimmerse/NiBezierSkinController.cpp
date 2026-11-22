// NiBezierSkinController.cpp -- skinning of Bezier meshes

#include "NiRTLib.h"
#include "NiBezierSkinController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00834ff0 _$E17
// FUNCTION: STBC 0x00835010 _$E15
// FUNCTION: STBC 0x00835020 _$E23
// FUNCTION: STBC 0x00835040 _$E21
// FUNCTION: STBC 0x00835050 _$E239

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00834c20 _$E232
// FUNCTION: STBC 0x00834c40 _$E229
// FUNCTION: STBC 0x00835070 _$E30
// FUNCTION: STBC 0x00835080 _$E41
// FUNCTION: STBC 0x00835090 _$E52
// FUNCTION: STBC 0x008350a0 _$E56
// FUNCTION: STBC 0x008350b0 _$E60
// FUNCTION: STBC 0x008350c0 _$E64
// FUNCTION: STBC 0x008350d0 _$E181
// FUNCTION: STBC 0x008350e0 _$E186
// FUNCTION: STBC 0x008350f0 _$E190
// FUNCTION: STBC 0x00835100 _$E198
// FUNCTION: STBC 0x00835110 _$E202
// FUNCTION: STBC 0x00835120 _$E206
// FUNCTION: STBC 0x00835130 _$E210
// FUNCTION: STBC 0x00835140 _$E218
// FUNCTION: STBC 0x00835150 _$E222
// FUNCTION: STBC 0x00835170 _$E226

// DATA: STBC 0x9B2FE8 ?rtti@NiBezierSkinController@@2VNiRTTI@@B
const NiRTTI NiBezierSkinController::rtti( "NiBezierSkinController", &NiSkinController::rtti );

/*
================
NiBezierSkinController::Register
================
*/
// FUNCTION: STBC 0x834C60 ?Register@NiBezierSkinController@@SAHXZ
NiImplementRegister( NiBezierSkinController )

/*
================
NiBezierSkinController::Update

Deforms the mesh's control vertices, then has the mesh rebuild its
patches and its bound.
================
*/
// FUNCTION: STBC 0x834E70
void NiBezierSkinController::Update( float time ) {
	if ( !DontDoUpdate( time ) ) {
		lastTime = time;

		NiBezierMesh *mesh = (NiBezierMesh *)target;
		NiPoint3 *vertices = mesh->GetVertices();
		Deform( vertices );
		mesh->Unknown830B80();
		mesh->Unknown82FA40();
		mesh->UpdateWorldBound();
	}
}

/*
================
NiBezierSkinController::CreateObject
================
*/
// FUNCTION: STBC 0x834EC0
void NiBezierSkinController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierSkinController *controller = new NiBezierSkinController;
	stream.SetLoadRecord( new NiBezierSkinControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiBezierSkinController::LoadBinary
================
*/
// FUNCTION: STBC 0x834FA0
void NiBezierSkinController::LoadBinary( NiStream &stream ) {
	NiSkinController::LoadBinary( stream );
}

/*
================
NiBezierSkinController::LinkObject
================
*/
// FUNCTION: STBC 0x834FB0
void NiBezierSkinController::LinkObject( NiStream &stream ) {
	NiSkinController::LinkObject( stream );
}

/*
================
NiBezierSkinController::RegisterStreamables

Quirk: calls NiTimeController's, past NiSkinController's, so the bones
are not registered.
================
*/
// FUNCTION: STBC 0x834FC0
bool NiBezierSkinController::RegisterStreamables( NiStream &stream ) {
	return NiTimeController::RegisterStreamables( stream );
}

/*
================
NiBezierSkinController::SaveBinary
================
*/
// FUNCTION: STBC 0x834FD0
void NiBezierSkinController::SaveBinary( NiStream &stream ) {
	NiSkinController::SaveBinary( stream );
}

/*
================
NiBezierSkinController::IsEqual
================
*/
// FUNCTION: STBC 0x834FE0
bool NiBezierSkinController::IsEqual( NiObject *object ) {
	return NiSkinController::IsEqual( object );
}
