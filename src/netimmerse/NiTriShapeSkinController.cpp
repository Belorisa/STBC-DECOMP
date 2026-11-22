// NiTriShapeSkinController.cpp -- skinning of triangle lists

#include "NiRTLib.h"
#include "NiTriShapeSkinController.h"
#include "NiTriShapeDynamicData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080ce00 _$E235
// FUNCTION: STBC 0x0080ce20 _$E17
// FUNCTION: STBC 0x0080ce40 _$E15
// FUNCTION: STBC 0x0080ce50 _$E23
// FUNCTION: STBC 0x0080ce70 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080ce80 _$E30
// FUNCTION: STBC 0x0080ce90 _$E41
// FUNCTION: STBC 0x0080cea0 _$E52
// FUNCTION: STBC 0x0080ceb0 _$E56
// FUNCTION: STBC 0x0080cec0 _$E60
// FUNCTION: STBC 0x0080ced0 _$E64
// FUNCTION: STBC 0x0080cee0 _$E181
// FUNCTION: STBC 0x0080cef0 _$E189
// FUNCTION: STBC 0x0080cf00 _$E196
// FUNCTION: STBC 0x0080cf10 _$E200
// FUNCTION: STBC 0x0080cf20 _$E204
// FUNCTION: STBC 0x0080cf30 _$E208
// FUNCTION: STBC 0x0080cf40 _$E214
// FUNCTION: STBC 0x0080cf60 _$E218

// FUNCTION: STBC 0x0080c920 _$E228
// FUNCTION: STBC 0x0080c940 _$E225
// DATA: STBC 0x9A25F0 ?rtti@NiTriShapeSkinController@@2VNiRTTI@@B
const NiRTTI NiTriShapeSkinController::rtti( "NiTriShapeSkinController", &NiSkinController::rtti );

/*
================
NiTriShapeSkinController::Register
================
*/
// FUNCTION: STBC 0x80C960 ?Register@NiTriShapeSkinController@@SAHXZ
NiImplementRegister( NiTriShapeSkinController )

/*
================
NiTriShapeSkinController::FindController
================
*/
// FUNCTION: STBC 0x80CAB0
NiTriShapeSkinController *NiTriShapeSkinController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiTriShapeSkinController *skinController = NiDynamicCast( NiTriShapeSkinController, controller );
		if ( skinController ) {
			return skinController;
		}
	}
	return 0;
}

/*
================
NiTriShapeSkinController::GetVertexCount
================
*/
// FUNCTION: STBC 0x80CB00
unsigned short NiTriShapeSkinController::GetVertexCount() const {
	return ( (NiTriShape *)target )->GetVertexCount();
}

/*
================
NiTriShapeSkinController::CreateNew
================
*/
// FUNCTION: STBC 0x80CB10
NiSkinController *NiTriShapeSkinController::CreateNew() {
	return new NiTriShapeSkinController;
}

/*
================
NiTriShapeSkinController::Update

Gives the target's model data the bound of the bones' vertices and leaves
the deformation to slot 15.
================
*/
// FUNCTION: STBC 0x80CBC0
void NiTriShapeSkinController::Update( float time ) {
	if ( !DontDoUpdate( time ) ) {
		lastTime = time;

		NiBound bound;
		ComputeWorldBound( bound );
		NiTriShape *shape = (NiTriShape *)target;
		shape->SetModelBound( bound );
		deformPending = true;
	}
}

/*
================
NiTriShapeSkinController::VirtualSlot15

Deforms the target's vertices when Update has marked it, recomputes the
normals when asked to and the target has some, and marks the model data
changed.
================
*/
// FUNCTION: STBC 0x80CC60
void NiTriShapeSkinController::VirtualSlot15() {
	if ( deformPending ) {
		NiTriShape *shape = (NiTriShape *)target;
		Deform( shape->GetPackedVertices() );
		if ( GetUpdateNormals() && shape->GetNormals() ) {
			shape->CalculateNormals();
		}
		shape->GetModelData()->IncRevisionID();
	}
}

/*
================
NiTriShapeSkinController::CreateObject
================
*/
// FUNCTION: STBC 0x80CCD0
void NiTriShapeSkinController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriShapeSkinController *controller = new NiTriShapeSkinController;
	stream.SetLoadRecord( new NiTriShapeSkinControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiTriShapeSkinController::LoadBinary
================
*/
// FUNCTION: STBC 0x80CDB0
void NiTriShapeSkinController::LoadBinary( NiStream &stream ) {
	NiSkinController::LoadBinary( stream );
}

/*
================
NiTriShapeSkinController::LinkObject
================
*/
// FUNCTION: STBC 0x80CDC0
void NiTriShapeSkinController::LinkObject( NiStream &stream ) {
	NiSkinController::LinkObject( stream );
}

/*
================
NiTriShapeSkinController::RegisterStreamables

Quirk: calls NiTimeController's, past NiSkinController's, so the bones
are not registered.
================
*/
// FUNCTION: STBC 0x80CDD0
bool NiTriShapeSkinController::RegisterStreamables( NiStream &stream ) {
	return NiTimeController::RegisterStreamables( stream );
}

/*
================
NiTriShapeSkinController::SaveBinary
================
*/
// FUNCTION: STBC 0x80CDE0
void NiTriShapeSkinController::SaveBinary( NiStream &stream ) {
	NiSkinController::SaveBinary( stream );
}

/*
================
NiTriShapeSkinController::IsEqual
================
*/
// FUNCTION: STBC 0x80CDF0
bool NiTriShapeSkinController::IsEqual( NiObject *object ) {
	return NiSkinController::IsEqual( object );
}
