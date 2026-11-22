// NiVertWeightsExtraData.cpp -- per-vertex weight extra data

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiStream.h"
#include "NiVertWeightsExtraData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00810620 _$E58
// FUNCTION: STBC 0x00810640 _$E17
// FUNCTION: STBC 0x00810660 _$E15
// FUNCTION: STBC 0x00810670 _$E23
// FUNCTION: STBC 0x00810690 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008106a0 _$E37
// FUNCTION: STBC 0x008106b0 _$E41
// FUNCTION: STBC 0x008106f0 _$E45

// FUNCTION: STBC 0x0080ff60 _$E51
// FUNCTION: STBC 0x0080ff80 _$E48
// DATA: STBC 0x9A2780 ?rtti@NiVertWeightsExtraData@@2VNiRTTI@@B
const NiRTTI NiVertWeightsExtraData::rtti( "NiVertWeightsExtraData", &NiExtraData::rtti );

/*
================
NiVertWeightsExtraData::Register
================
*/
// FUNCTION: STBC 0x80FFA0 ?Register@NiVertWeightsExtraData@@SAHXZ
NiImplementRegister( NiVertWeightsExtraData )

/*
================
NiVertWeightsExtraData::NiVertWeightsExtraData
================
*/
// FUNCTION: STBC 0x8100F0 ??0NiVertWeightsExtraData@@QAE@GPAM@Z
NiVertWeightsExtraData::NiVertWeightsExtraData( unsigned short vertexCount_, float *weights_ ) {
	weights = 0;
	vertexCount = 0;
	SetData( vertexCount_, weights_ );
}

/*
================
NiVertWeightsExtraData::SetData

The size is the weights' size in a file: the count, then the weights.
================
*/
// FUNCTION: STBC 0x8101A0
void NiVertWeightsExtraData::SetData( unsigned short vertexCount_, float *weights_ ) {
	delete [] weights;
	weights = 0;
	vertexCount = 0;
	if ( weights_ && vertexCount_ ) {
		unsigned int bytes = vertexCount_ * sizeof( float );
		weights = new float[vertexCount_];
		memcpy( weights, weights_, bytes );
		vertexCount = vertexCount_;
		size = bytes + sizeof( unsigned short );
	}
}

/*
================
NiVertWeightsExtraData::CreateClone
================
*/
// FUNCTION: STBC 0x810210
NiObject *NiVertWeightsExtraData::CreateClone() {
	NiVertWeightsExtraData *clone = new NiVertWeightsExtraData;
	CopyMembers( clone );
	return clone;
}

/*
================
NiVertWeightsExtraData::CopyMembers

BUG: the copy's weight array is allocated but never filled; the weights
are not copied.
================
*/
// FUNCTION: STBC 0x8102C0
void NiVertWeightsExtraData::CopyMembers( NiVertWeightsExtraData *dest ) {
	NiExtraData::CopyMembers( dest );
	dest->vertexCount = vertexCount;
	if ( vertexCount ) {
		dest->weights = new float[vertexCount];
	} else {
		dest->weights = 0;
	}
}

/*
================
NiVertWeightsExtraData::CreateObject

Creates the data and loads it.
================
*/
// FUNCTION: STBC 0x810310
void NiVertWeightsExtraData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiVertWeightsExtraData *data = new NiVertWeightsExtraData;
	stream.SetLoadRecord( new NiVertWeightsExtraDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiVertWeightsExtraData::LoadBinary
================
*/
// FUNCTION: STBC 0x8103E0
void NiVertWeightsExtraData::LoadBinary( NiStream &stream ) {
	NiExtraData::LoadBinary( stream );
	NiStreamLoadBinary( stream, vertexCount );
	if ( vertexCount ) {
		weights = new float[vertexCount];
		assert( weights );
		for ( int i = 0; i < vertexCount; i++ ) {
			NiStreamLoadBinary( stream, weights[i] );
		}
	}
}

/*
================
NiVertWeightsExtraData::LinkObject
================
*/
// FUNCTION: STBC 0x810450
void NiVertWeightsExtraData::LinkObject( NiStream &stream ) {
	NiExtraData::LinkObject( stream );
}

/*
================
NiVertWeightsExtraData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x810460
bool NiVertWeightsExtraData::RegisterStreamables( NiStream &stream ) {
	return NiExtraData::RegisterStreamables( stream );
}

/*
================
NiVertWeightsExtraData::SaveBinary
================
*/
// FUNCTION: STBC 0x810470
void NiVertWeightsExtraData::SaveBinary( NiStream &stream ) {
	NiExtraData::SaveBinary( stream );
	NiStreamSaveBinary( stream, vertexCount );
	for ( int i = 0; i < vertexCount; i++ ) {
		NiStreamSaveBinary( stream, weights[i] );
	}
}

/*
================
NiVertWeightsExtraData::IsEqual
================
*/
// FUNCTION: STBC 0x8104C0
bool NiVertWeightsExtraData::IsEqual( NiObject *object ) {
	if ( !NiExtraData::IsEqual( object ) ) {
		return false;
	}
	NiVertWeightsExtraData *otherData = (NiVertWeightsExtraData *)object;	// (name invented; fits the /Od frame order)
	if ( vertexCount != otherData->vertexCount ) {
		return false;
	}
	for ( unsigned short i = 0; i < vertexCount; i++ ) {
		if ( weights[i] != otherData->weights[i] ) {
			return false;
		}
	}
	return true;
}

/*
================
NiVertWeightsExtraData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x810520
void NiVertWeightsExtraData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiExtraData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiVertWeightsExtraData::~NiVertWeightsExtraData
================
*/
// FUNCTION: STBC 0x810700
NiVertWeightsExtraData::~NiVertWeightsExtraData() {
	delete [] weights;
}
