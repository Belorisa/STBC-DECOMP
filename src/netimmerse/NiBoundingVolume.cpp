// NiBoundingVolume.cpp -- the collision dispatch of the alternate bounding volumes

#include "NiRTLib.h"
#include "NiBoundingVolume.h"
#include "NiBound.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00822170 _$E51
// FUNCTION: STBC 0x00822190 _$E17
// FUNCTION: STBC 0x008221b0 _$E15
// FUNCTION: STBC 0x008221c0 _$E23
// FUNCTION: STBC 0x008221e0 _$E21

// DATA: STBC 0x9A2CE4 ?loaders@NiBoundingVolume@@2PAP6APAV1@AAVNiStream@@@ZA
NiBoundingVolume::LoadFunction NiBoundingVolume::loaders[MAX_TYPES];
// DATA: STBC 0x9A2CFC
NiBoundingVolume::TestFunction *NiBoundingVolume::testFunctions;
// DATA: STBC 0x9A2D00
NiBoundingVolume::FindFunction *NiBoundingVolume::findFunctions;
// DATA: STBC 0x9A2D04
NiBoundingVolume::TestTriFunction *NiBoundingVolume::testTriFunctions;
// DATA: STBC 0x9A2D08
NiBoundingVolume::FindTriFunction *NiBoundingVolume::findTriFunctions;
// DATA: STBC 0x9A2D0C
NiBoundingVolume::SphereFunction NiBoundingVolume::sphereFunction;
// DATA: STBC 0x9A2D10
NiBoundingVolume::DataFunction NiBoundingVolume::dataFunction;

/*
================
NiBoundingVolume::TestIntersect
================
*/
// FUNCTION: STBC 0x821EA0
bool NiBoundingVolume::TestIntersect( float deltaTime, const NiBoundingVolume &volume0, const NiPoint3 &velocity0,
	const NiBoundingVolume &volume1, const NiPoint3 &velocity1 ) {
	TestFunction test = testFunctions[volume0.GetType() * MAX_TYPES + volume1.GetType()];
	if ( !test ) {
		test = testFunctions[volume1.GetType() * MAX_TYPES + volume0.GetType()];
		if ( test ) {
			return test( deltaTime, volume1, velocity1, volume0, velocity0 );
		}
	}
	if ( test ) {
		return test( deltaTime, volume0, velocity0, volume1, velocity1 );
	}
	return false;
}

/*
================
NiBoundingVolume::FindIntersect
================
*/
// FUNCTION: STBC 0x821F30
bool NiBoundingVolume::FindIntersect( float deltaTime, const NiBoundingVolume &volume0, const NiPoint3 &velocity0,
	const NiBoundingVolume &volume1, const NiPoint3 &velocity1, float &time, NiPoint3 &point,
	bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 ) {
	FindFunction find = findFunctions[volume0.GetType() * MAX_TYPES + volume1.GetType()];
	if ( !find ) {
		find = findFunctions[volume1.GetType() * MAX_TYPES + volume0.GetType()];
		if ( find ) {
			return find( deltaTime, volume1, velocity1, volume0, velocity0, time, point, calcNormals, normal1, normal0 );
		}
	}
	if ( find ) {
		return find( deltaTime, volume0, velocity0, volume1, velocity1, time, point, calcNormals, normal0, normal1 );
	}
	return false;
}

/*
================
NiBoundingVolume::TestTriIntersect
================
*/
// FUNCTION: STBC 0x821FF0
bool NiBoundingVolume::TestTriIntersect( float deltaTime, const NiBoundingVolume &volume, const NiPoint3 &velocity0,
	const NiPoint3 &vertex0, const NiPoint3 &vertex1, const NiPoint3 &vertex2, const NiPoint3 &velocity1 ) {
	TestTriFunction test = testTriFunctions[volume.GetType()];
	if ( test ) {
		return test( deltaTime, volume, velocity0, vertex0, vertex1, vertex2, velocity1 );
	}
	return false;
}

/*
================
NiBoundingVolume::FindTriIntersect
================
*/
// FUNCTION: STBC 0x822040
bool NiBoundingVolume::FindTriIntersect( float deltaTime, const NiBoundingVolume &volume, const NiPoint3 &velocity0,
	const NiPoint3 &vertex0, const NiPoint3 &vertex1, const NiPoint3 &vertex2, const NiPoint3 &velocity1,
	float &time, NiPoint3 &point, bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 ) {
	FindTriFunction find = findTriFunctions[volume.GetType()];
	if ( find ) {
		return find( deltaTime, volume, velocity0, vertex0, vertex1, vertex2, velocity1, time, point, calcNormals, normal0, normal1 );
	}
	return false;
}

/*
================
NiBoundingVolume::CreateFromBound
================
*/
// FUNCTION: STBC 0x8220A0
NiBoundingVolume *NiBoundingVolume::CreateFromBound( const NiBound &bound ) {
	if ( sphereFunction ) {
		return sphereFunction( bound.GetRadius(), bound.GetCenter() );
	}
	return 0;
}

/*
================
NiBoundingVolume::CreateFromData
================
*/
// FUNCTION: STBC 0x8220D0
NiBoundingVolume *NiBoundingVolume::CreateFromData( unsigned short count, const NiPoint3 *points ) {
	if ( dataFunction ) {
		return dataFunction( count, points );
	}
	return 0;
}

/*
================
NiBoundingVolume::CreateFromStream

The type, then the volume its loader reads; NULL for a type out of range.
================
*/
// FUNCTION: STBC 0x8220F0
NiBoundingVolume *NiBoundingVolume::CreateFromStream( NiStream &stream ) {
	int type;
	NiStreamLoadBinary( stream, type );
	if ( type >= 0 && type < MAX_TYPES ) {
		LoadFunction load = loaders[type];
		return load( stream );
	}
	return 0;
}

/*
================
NiBoundingVolume::LoadBinary

The type was read by CreateFromStream.
================
*/
// FUNCTION: STBC 0x822130
void NiBoundingVolume::LoadBinary( NiStream &stream ) {
}

/*
================
NiBoundingVolume::SaveBinary

The type, which CreateFromStream reads back.
================
*/
// FUNCTION: STBC 0x822140
void NiBoundingVolume::SaveBinary( NiStream &stream ) {
	int type = GetType();
	NiStreamSaveBinary( stream, type );
}
