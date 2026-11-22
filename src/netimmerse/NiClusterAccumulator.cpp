// NiClusterAccumulator.cpp -- the cluster-sorting accumulator

#include "NiRTLib.h"
#include <assert.h>
#include "NiClusterAccumulator.h"
#include "NiCamera.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007eba10 _$E30
// FUNCTION: STBC 0x007eba20 _$E34
// FUNCTION: STBC 0x007eba50 _$E45
// FUNCTION: STBC 0x007eba60 _$E49
// FUNCTION: STBC 0x007eba70 _$E53
// FUNCTION: STBC 0x007eba80 _$E57
// FUNCTION: STBC 0x007eba90 _$E181
// FUNCTION: STBC 0x007ebaa0 _$E123
// FUNCTION: STBC 0x007ebab0 _$E136
// FUNCTION: STBC 0x007ebac0 _$E186
// FUNCTION: STBC 0x007ebad0 _$E191
// FUNCTION: STBC 0x007ebae0 _$E195
// FUNCTION: STBC 0x007ebaf0 _$E199
// FUNCTION: STBC 0x007ebb00 _$E206
// FUNCTION: STBC 0x007ebb40 _$E231

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007eba30 _$E244
// FUNCTION: STBC 0x007eb9b0 _$E17
// FUNCTION: STBC 0x007eb9d0 _$E15
// FUNCTION: STBC 0x007eb9e0 _$E23
// FUNCTION: STBC 0x007eba00 _$E21

// FUNCTION: STBC 0x007eb2b0 _$E237
// FUNCTION: STBC 0x007eb2d0 _$E234
// DATA: STBC 0x9A1960 ?rtti@NiClusterAccumulator@@2VNiRTTI@@B
const NiRTTI NiClusterAccumulator::rtti( "NiClusterAccumulator", &NiAccumulator::rtti );

/*
================
NiClusterAccumulator::Register
================
*/
// FUNCTION: STBC 0x7EB2F0 ?Register@NiClusterAccumulator@@SAHXZ
NiImplementRegister( NiClusterAccumulator )

/*
================
NiClusterAccumulator::NiClusterAccumulator
================
*/
// FUNCTION: STBC 0x7EB440
NiClusterAccumulator::NiClusterAccumulator() : sorter( 0 ) {
	objects = new NiTList<NiSortedObject *>( 2 );
	assert( objects );
}

/*
================
NiClusterAccumulator::~NiClusterAccumulator

Deletes the objects still in the list, then the list.
================
*/
// FUNCTION: STBC 0x7EB4D0
NiClusterAccumulator::~NiClusterAccumulator() {
	while ( objects->GetSize() ) {
		delete objects->RemoveHead();
	}
	delete objects;
}

/*
================
NiClusterAccumulator::StartAccumulating

The sorter takes its boxes in the camera's view.
================
*/
// FUNCTION: STBC 0x7EB5E0
bool NiClusterAccumulator::StartAccumulating( NiCamera *camera_ ) {
	if ( !NiAccumulator::StartAccumulating( camera_ ) ) {
		return false;
	}
	sorter.SetCamera( camera_ );
	return true;
}

/*
================
NiClusterAccumulator::FinishAccumulating

Draws the registered objects in the sorted order and deletes them. Without
a camera there is nothing to finish.
================
*/
// FUNCTION: STBC 0x7EB630
void NiClusterAccumulator::FinishAccumulating() {
	if ( !camera ) {
		return;
	}

	sorter.Sort( objects );
	sorter.StartIteration();
	NiSortedObject *object = sorter.GetNext();
	while ( object ) {
		camera->Unknown7E8B90( object );
		object = sorter.GetNext();
	}
	sorter.RemoveAllEntries();

	while ( objects->GetSize() ) {
		delete objects->RemoveHead();
	}
	sorter.SetCamera( 0 );
	NiAccumulator::FinishAccumulating();
}

/*
================
NiClusterAccumulator::RegisterObject
================
*/
// FUNCTION: STBC 0x7EB710
bool NiClusterAccumulator::RegisterObject( NiAVObject *geometry ) {
	assert( camera );
	NiSortedObject *object = camera->Unknown7E8B10( geometry );
	assert( object );
	objects->AddTail( object );
	return true;
}

/*
================
NiClusterAccumulator::CreateClone
================
*/
// FUNCTION: STBC 0x7EB790
NiObject *NiClusterAccumulator::CreateClone() {
	NiClusterAccumulator *clone = new NiClusterAccumulator;
	CopyMembers( clone );
	return clone;
}

/*
================
NiClusterAccumulator::CopyMembers
================
*/
// FUNCTION: STBC 0x7EB840
void NiClusterAccumulator::CopyMembers( NiClusterAccumulator *dest ) {
	NiAccumulator::CopyMembers( dest );
}

/*
================
NiClusterAccumulator::CreateObject

Creates the accumulator and loads it.
================
*/
// FUNCTION: STBC 0x7EB850
void NiClusterAccumulator::CreateObject( NiStream &stream, NiObject *&object ) {
	NiClusterAccumulator *accumulator = new NiClusterAccumulator;
	stream.SetLoadRecord( new NiClusterAccumulatorLoadRecord );
	accumulator->LoadBinary( stream );
	object = accumulator;
}

/*
================
NiClusterAccumulator::LoadBinary
================
*/
// FUNCTION: STBC 0x7EB950
void NiClusterAccumulator::LoadBinary( NiStream &stream ) {
	NiAccumulator::LoadBinary( stream );
}

/*
================
NiClusterAccumulator::LinkObject
================
*/
// FUNCTION: STBC 0x7EB960
void NiClusterAccumulator::LinkObject( NiStream &stream ) {
	NiAccumulator::LinkObject( stream );
}

/*
================
NiClusterAccumulator::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7EB970
bool NiClusterAccumulator::RegisterStreamables( NiStream &stream ) {
	return NiAccumulator::RegisterStreamables( stream );
}

/*
================
NiClusterAccumulator::SaveBinary
================
*/
// FUNCTION: STBC 0x7EB980
void NiClusterAccumulator::SaveBinary( NiStream &stream ) {
	NiAccumulator::SaveBinary( stream );
}

/*
================
NiClusterAccumulator::IsEqual
================
*/
// FUNCTION: STBC 0x7EB990
bool NiClusterAccumulator::IsEqual( NiObject *object ) {
	if ( !NiAccumulator::IsEqual( object ) ) {
		return false;
	}
	return true;
}
