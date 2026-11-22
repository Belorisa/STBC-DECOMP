// NiAlphaAccumulator.cpp -- the accumulator of alpha-blended geometry

#include "NiRTLib.h"
#include "NiAlphaAccumulator.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007ebf10 _$E30
// FUNCTION: STBC 0x007ebf20 _$E34
// FUNCTION: STBC 0x007ebf50 _$E45
// FUNCTION: STBC 0x007ebf60 _$E49
// FUNCTION: STBC 0x007ebf70 _$E53
// FUNCTION: STBC 0x007ebf80 _$E57
// FUNCTION: STBC 0x007ebf90 _$E181
// FUNCTION: STBC 0x007ebfa0 _$E123
// FUNCTION: STBC 0x007ebfb0 _$E136
// FUNCTION: STBC 0x007ebfc0 _$E186
// FUNCTION: STBC 0x007ebfd0 _$E191
// FUNCTION: STBC 0x007ebfe0 _$E195
// FUNCTION: STBC 0x007ebff0 _$E199
// FUNCTION: STBC 0x007ec000 _$E206
// FUNCTION: STBC 0x007ec010 _$E231
// FUNCTION: STBC 0x007ec020 _$E235
// FUNCTION: STBC 0x007ec030 _$E81

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007ebf30 _$E248
// FUNCTION: STBC 0x007ebeb0 _$E17
// FUNCTION: STBC 0x007ebed0 _$E15
// FUNCTION: STBC 0x007ebee0 _$E23
// FUNCTION: STBC 0x007ebf00 _$E21

// FUNCTION: STBC 0x007ebb50 _$E241
// FUNCTION: STBC 0x007ebb70 _$E238
// DATA: STBC 0x9A19B0 ?rtti@NiAlphaAccumulator@@2VNiRTTI@@B
const NiRTTI NiAlphaAccumulator::rtti( "NiAlphaAccumulator", &NiClusterAccumulator::rtti );

/*
================
NiAlphaAccumulator::Register
================
*/
// FUNCTION: STBC 0x7EBB90 ?Register@NiAlphaAccumulator@@SAHXZ
NiImplementRegister( NiAlphaAccumulator )

/*
================
NiAlphaAccumulator::CreateClone
================
*/
// FUNCTION: STBC 0x7EBCE0
NiObject *NiAlphaAccumulator::CreateClone() {
	NiAlphaAccumulator *clone = new NiAlphaAccumulator;
	CopyMembers( clone );
	return clone;
}

/*
================
NiAlphaAccumulator::CopyMembers
================
*/
// FUNCTION: STBC 0x7EBD60
void NiAlphaAccumulator::CopyMembers( NiAlphaAccumulator *dest ) {
	NiClusterAccumulator::CopyMembers( dest );
	dest->observeNoSortHint = observeNoSortHint;
}

/*
================
NiAlphaAccumulator::CreateObject

Creates the accumulator and loads it.
================
*/
// FUNCTION: STBC 0x7EBD80
void NiAlphaAccumulator::CreateObject( NiStream &stream, NiObject *&object ) {
	NiAlphaAccumulator *accumulator = new NiAlphaAccumulator;
	stream.SetLoadRecord( new NiAlphaAccumulatorLoadRecord );
	accumulator->LoadBinary( stream );
	object = accumulator;
}

/*
================
NiAlphaAccumulator::LoadBinary
================
*/
// FUNCTION: STBC 0x7EBE50
void NiAlphaAccumulator::LoadBinary( NiStream &stream ) {
	NiClusterAccumulator::LoadBinary( stream );
}

/*
================
NiAlphaAccumulator::LinkObject
================
*/
// FUNCTION: STBC 0x7EBE60
void NiAlphaAccumulator::LinkObject( NiStream &stream ) {
	NiClusterAccumulator::LinkObject( stream );
}

/*
================
NiAlphaAccumulator::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7EBE70
bool NiAlphaAccumulator::RegisterStreamables( NiStream &stream ) {
	return NiClusterAccumulator::RegisterStreamables( stream );
}

/*
================
NiAlphaAccumulator::SaveBinary
================
*/
// FUNCTION: STBC 0x7EBE80
void NiAlphaAccumulator::SaveBinary( NiStream &stream ) {
	NiClusterAccumulator::SaveBinary( stream );
}

/*
================
NiAlphaAccumulator::IsEqual
================
*/
// FUNCTION: STBC 0x7EBE90
bool NiAlphaAccumulator::IsEqual( NiObject *object ) {
	if ( !NiClusterAccumulator::IsEqual( object ) ) {
		return false;
	}
	return true;
}
