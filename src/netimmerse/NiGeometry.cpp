// NiGeometry.cpp -- scene graph leaves drawn from shareable model data

#include "NiRTLib.h"
#include <assert.h>
#include "NiGeometry.h"
#include "NiCamera.h"
#include "NiProcessorSpecificCode.h"
#include "NiStream.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007ee9b0 _$E30
// FUNCTION: STBC 0x007ee9c0 _$E41
// FUNCTION: STBC 0x007ee9d0 _$E45
// FUNCTION: STBC 0x007ee9e0 _$E49
// FUNCTION: STBC 0x007ee9f0 _$E53
// FUNCTION: STBC 0x007eea00 _$E177
// FUNCTION: STBC 0x007eea10 _$E196
// FUNCTION: STBC 0x007eea20 _$E119
// FUNCTION: STBC 0x007eea30 _$E132
// FUNCTION: STBC 0x007eea40 _$E201
// FUNCTION: STBC 0x007eea50 _$E206
// FUNCTION: STBC 0x007eea60 _$E210
// FUNCTION: STBC 0x007eea70 _$E214
// FUNCTION: STBC 0x007eea80 _$E221
// FUNCTION: STBC 0x007eea90 _$E185
// FUNCTION: STBC 0x007eead0 _$E192

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007ee990 _$E241
// FUNCTION: STBC 0x007ee910 _$E17
// FUNCTION: STBC 0x007ee930 _$E15
// FUNCTION: STBC 0x007ee940 _$E23
// FUNCTION: STBC 0x007ee960 _$E21

// FUNCTION: STBC 0x007edb30 _$E232
// FUNCTION: STBC 0x007edb50 _$E229
// DATA: STBC 0x9A1A98 ?rtti@NiGeometry@@2VNiRTTI@@B
const NiRTTI NiGeometry::rtti( "NiGeometry", &NiAVObject::rtti );

// DATA: STBC 0x9A1B2C ?shapesDrawn@NiGeometry@@2IA
unsigned int NiGeometry::shapesDrawn;
// DATA: STBC 0x9A1B30 ?trianglesDrawn@NiGeometry@@2IA
unsigned int NiGeometry::trianglesDrawn;
// DATA: STBC 0x9A1B34 ?verticesDrawn@NiGeometry@@2IA
unsigned int NiGeometry::verticesDrawn;

/*
================
NiGeometry::Register
================
*/
// FUNCTION: STBC 0x7EDB70 ?Register@NiGeometry@@SAHXZ
NiImplementRegister( NiGeometry )

/*
================
NiGeometry::NiGeometry
================
*/
// FUNCTION: STBC 0x7EDCC0 ??0NiGeometry@@QAE@PAVNiGeometryData@@@Z
NiGeometry::NiGeometry( NiGeometryData *data ) : modelData( data ) {
	worldVertices = 0;
	worldNormals = 0;
	worldVerticesChanged = false;
	worldNormalsChanged = false;
}

/*
================
NiGeometry::NiGeometry
================
*/
// FUNCTION: STBC 0x7EDD10 ??0NiGeometry@@QAE@XZ
NiGeometry::NiGeometry() {
	worldVertices = 0;
	worldNormals = 0;
	worldVerticesChanged = false;
	worldNormalsChanged = false;
}

/*
================
NiGeometry::~NiGeometry
================
*/
// FUNCTION: STBC 0x7EDD50 ??1NiGeometry@@UAE@XZ
NiGeometry::~NiGeometry() {
	modelData = 0;
	delete [] worldVertices;
	delete [] worldNormals;
}

/*
================
NiGeometry::SetModelData

NULL is ignored. Data of another vertex count rebuilds the world copies
that exist.
================
*/
// FUNCTION: STBC 0x7EDE60
void NiGeometry::SetModelData( NiGeometryData *data ) {
	assert( data );
	if ( !data ) {
		return;
	}
	if ( modelData && data->GetVertexCount() != modelData->GetVertexCount() ) {
		if ( GetWorldVertices() ) {
			DestroyWorldVertices();
			CreateWorldVertices();
		}
		if ( GetWorldNormals() ) {
			DestroyWorldNormals();
			CreateWorldNormals();
		}
	}
	// NOTE: the world copies update from the old data, before the new data
	// takes its place
	UpdateWorldVertices();
	UpdateWorldNormals();
	modelData = data;
}

/*
================
NiGeometry::GetModelData
================
*/
// FUNCTION: STBC 0x7EDF00
NiGeometryData *NiGeometry::GetModelData() const {
	return modelData;
}

/*
================
NiGeometry::VirtualSlot35

Hands the camera's renderer the geometry's property and effect states,
after the base's pass.
================
*/
// FUNCTION: STBC 0x7EDF10
void NiGeometry::VirtualSlot35( NiCamera *camera ) {
	NiAVObject::VirtualSlot35( camera );
	assert( propertyState );
	if ( propertyState ) {
		camera->GetRenderer()->SetPropertyState( propertyState );
	}
	camera->GetRenderer()->SetEffectState( effectState );
}

/*
================
NiGeometry::CreateWorldVertices
================
*/
// FUNCTION: STBC 0x7EDFA0
void NiGeometry::CreateWorldVertices() {
	if ( !worldVertices ) {
		worldVertices = new NiPoint3[modelData->GetVertexCount()];
		worldVerticesChanged = true;
	}
}

/*
================
NiGeometry::UpdateWorldVertices

Transforms the model vertices into the world copy when the world
transform moved.
================
*/
// FUNCTION: STBC 0x7EE000
void NiGeometry::UpdateWorldVertices() {
	if ( worldVertices && worldVerticesChanged ) {
		NiProcessorSpecificCode::TransformPoints( modelData->GetActiveVertexCount(), (const float *)modelData->GetVertices(), (float *)worldVertices, &worldRotate, worldScale, (const float *)&worldTranslate );
		worldVerticesChanged = false;
	}
}

/*
================
NiGeometry::DestroyWorldVertices
================
*/
// FUNCTION: STBC 0x7EE060
void NiGeometry::DestroyWorldVertices() {
	delete [] worldVertices;
	worldVertices = 0;
	worldVerticesChanged = false;
}

/*
================
NiGeometry::CreateWorldNormals
================
*/
// FUNCTION: STBC 0x7EE090
void NiGeometry::CreateWorldNormals() {
	if ( !worldNormals ) {
		worldNormals = new NiPoint3[modelData->GetVertexCount()];
		worldNormalsChanged = true;
	}
}

/*
================
NiGeometry::UpdateWorldNormals

Rotates the model normals into the world copy when the world transform
moved.
================
*/
// FUNCTION: STBC 0x7EE0F0
void NiGeometry::UpdateWorldNormals() {
	if ( worldNormals && worldNormalsChanged ) {
		NiProcessorSpecificCode::TransformVectors( modelData->GetActiveVertexCount(), (const float *)modelData->GetNormals(), (float *)worldNormals, &worldRotate );
		worldNormalsChanged = false;
	}
}

/*
================
NiGeometry::DestroyWorldNormals
================
*/
// FUNCTION: STBC 0x7EE140
void NiGeometry::DestroyWorldNormals() {
	delete [] worldNormals;
	worldNormals = 0;
	worldNormalsChanged = false;
}

/*
================
NiGeometry::CalculateNormals
================
*/
// FUNCTION: STBC 0x7EE170
void NiGeometry::CalculateNormals() {
	modelData->CalculateNormals();
}

/*
================
NiGeometry::UpdateWorldData

The world copies are out of date once the world transform moves.
================
*/
// FUNCTION: STBC 0x7EE180
void NiGeometry::UpdateWorldData() {
	NiAVObject::UpdateWorldData();
	worldVerticesChanged = true;
	worldNormalsChanged = true;
}

/*
================
NiGeometry::UpdateWorldBound

The model bound, recomputed first when it is out of date, carried into
world space.
================
*/
// FUNCTION: STBC 0x7EE1A0
void NiGeometry::UpdateWorldBound() {
	NiBound &modelBound = modelData->GetBound();
	if ( modelBound.GetNeedUpdate() ) {
		modelBound.ComputeFromData( modelData->GetActiveVertexCount(), modelData->GetVertices() );
	}
	worldBound.Update( modelBound, worldRotate, worldTranslate, worldScale );
}

/*
================
NiGeometry::GetPropertyState
================
*/
// FUNCTION: STBC 0x7EE1F0
NiPointer<NiPropertyState> NiGeometry::GetPropertyState() {
	return propertyState;
}

/*
================
NiGeometry::UpdatePropertiesDownward

Takes the parent's state with this object's own properties applied.
================
*/
// FUNCTION: STBC 0x7EE210
void NiGeometry::UpdatePropertiesDownward( NiPropertyState *parentState ) {
	propertyState = PushLocalProperties( parentState, true );
}

/*
================
NiGeometry::GetEffectState
================
*/
// FUNCTION: STBC 0x7EE2D0
NiPointer<NiDynamicEffectState> NiGeometry::GetEffectState() {
	return effectState;
}

/*
================
NiGeometry::UpdateEffectsDownward

Takes the parent's state, or the last state shared when it is equal, so
that geometry under the same effects shares one state.
================
*/
// FUNCTION: STBC 0x7EE2F0
void NiGeometry::UpdateEffectsDownward( NiDynamicEffectState *parentState ) {
	if ( parentState ) {
		// AnimOnly.exe 0x42EC89 (NiGeometry.cpp)
		NiDynamicEffectState *lastState = NiDynamicEffectState::GetCachedState();
		if ( parentState->Equal( lastState ) ) {
			effectState = lastState;
		} else {
			effectState = parentState;
			NiDynamicEffectState::SetCachedState( parentState );
		}
	} else {
		effectState = 0;
	}
}

/*
================
NiGeometry::ApplyTransform

Moves the model vertices and normals by the transform. With onLeft the
transform applies in the parent's space and is carried into model space
through the local transform first. Recomputes the model bound.
================
*/
// FUNCTION: STBC 0x7EE3E0
void NiGeometry::ApplyTransform( const NiMatrix3 &rotate, const NiPoint3 &translate, bool onLeft ) {
	unsigned short count = modelData->GetActiveVertexCount();
	unsigned short i;
	NiPoint3 *vertices = modelData->GetVertices();
	NiPoint3 *normals = modelData->GetNormals();

	if ( onLeft ) {
		NiMatrix3 m1 = localRotate.TransposeTimes( rotate * localRotate );	// (name invented; fits the /Od frame order)
		NiPoint3 t1 = ( ( rotate * localTranslate + translate - localTranslate ) * localRotate ) / localScale;	// (name invented; fits the /Od frame order)
		if ( vertices ) {
			for ( i = 0; i < count; i++ ) {
				vertices[i] = t1 + m1 * vertices[i];
			}
		}
		if ( normals ) {
			NiMatrix3 inverse;
			m1.Inverse( inverse );
			for ( i = 0; i < count; i++ ) {
				normals[i] = normals[i] * inverse;
			}
		}
	} else {
		if ( vertices ) {
			for ( i = 0; i < count; i++ ) {
				vertices[i] = translate + rotate * vertices[i];
			}
		}
		if ( normals ) {
			NiMatrix3 inverse;
			rotate.Inverse( inverse );
			for ( i = 0; i < count; i++ ) {
				normals[i] = normals[i] * inverse;
			}
		}
	}

	modelData->GetBound().ComputeFromData( count, vertices );
}

/*
================
NiGeometry::CreateClone

NiGeometry itself is never cloned; the derived classes create the clone.
================
*/
// FUNCTION: STBC 0x7EE660
NiObject *NiGeometry::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiGeometry::CopyMembers

The clone gets a clone of the model data.
================
*/
// FUNCTION: STBC 0x7EE670
void NiGeometry::CopyMembers( NiGeometry *dest ) {
	NiAVObject::CopyMembers( dest );
	dest->SetModelData( (NiGeometryData *)modelData->CreateClone() );
}

/*
================
NiGeometry::ProcessClone
================
*/
// FUNCTION: STBC 0x7EE6A0
void NiGeometry::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiGeometry, clone ) );
	NiAVObject::ProcessClone( clone );
}

/*
================
NiGeometry::CreateObject

The class is abstract: the stream creates nothing for it.
================
*/
// FUNCTION: STBC 0x7EE6B0
void NiGeometry::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiGeometry::LoadBinary

NiAVObject's members, then the link ID of the model data.
================
*/
// FUNCTION: STBC 0x7EE6C0
void NiGeometry::LoadBinary( NiStream &stream ) {
	NiAVObject::LoadBinary( stream );
	NiGeometryLoadRecord *record = (NiGeometryLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->modelDataLinkID );
	assert( record->modelDataLinkID );
}

/*
================
NiGeometry::LinkObject
================
*/
// FUNCTION: STBC 0x7EE6E0
void NiGeometry::LinkObject( NiStream &stream ) {
	NiAVObject::LinkObject( stream );
	NiGeometryLoadRecord *record = (NiGeometryLoadRecord *)stream.GetLoadRecord();
	assert( record );
	modelData = (NiGeometryData *)stream.GetObjectFromLinkID( record->modelDataLinkID );
}

/*
================
NiGeometry::RegisterStreamables

Registers the model data after the object.
================
*/
// FUNCTION: STBC 0x7EE740
bool NiGeometry::RegisterStreamables( NiStream &stream ) {
	if ( !NiAVObject::RegisterStreamables( stream ) ) {
		return false;
	}
	modelData->RegisterStreamables( stream );
	return true;
}

/*
================
NiGeometry::SaveBinary
================
*/
// FUNCTION: STBC 0x7EE770
void NiGeometry::SaveBinary( NiStream &stream ) {
	NiAVObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, (NiGeometryData *)modelData );
}

/*
================
NiGeometry::IsEqual
================
*/
// FUNCTION: STBC 0x7EE7A0
bool NiGeometry::IsEqual( NiObject *object ) {
	if ( !NiAVObject::IsEqual( object ) ) {
		return false;
	}
	NiGeometry *geometry = (NiGeometry *)object;
	return modelData->IsEqual( geometry->modelData );
}

/*
================
NiGeometry::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7EE7D0
void NiGeometry::GetViewerStrings( NiTArray<char *> *strings ) {
	NiAVObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiGeometry::ResetDrawCounts
================
*/
// FUNCTION: STBC 0x7EF300
void NiGeometry::ResetDrawCounts() {
	shapesDrawn = 0;
	trianglesDrawn = 0;
	verticesDrawn = 0;
}

/*
================
NiGeometry::GetDrawCounts
================
*/
// FUNCTION: STBC 0x7EF320
void NiGeometry::GetDrawCounts( unsigned int *shapes, unsigned int *triangles, unsigned int *vertices ) {
	*shapes = shapesDrawn;
	*triangles = trianglesDrawn;
	*vertices = verticesDrawn;
}
