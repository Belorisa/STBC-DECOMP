// NiBound.cpp -- bounding spheres

#include "NiRTLib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "NiBound.h"
#include "NiMatrix3.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "NiNode.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiTimeController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00812ca0 _$E193
// FUNCTION: STBC 0x00812c40 _$E17
// FUNCTION: STBC 0x00812c60 _$E15
// FUNCTION: STBC 0x00812c70 _$E23
// FUNCTION: STBC 0x00812c90 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00812cc0 _$E46
// FUNCTION: STBC 0x00812cd0 _$E50
// FUNCTION: STBC 0x00812ce0 _$E54
// FUNCTION: STBC 0x00812cf0 _$E58
// FUNCTION: STBC 0x00812d00 _$E62
// FUNCTION: STBC 0x00812d10 _$E177
// FUNCTION: STBC 0x00812d20 _$E182
// FUNCTION: STBC 0x00812d30 _$E186

// The containment comparisons' slack (Gamebryo 1.1.2: NiBound::ms_fFuzzFactor
// and ms_fTolerance).
// DATA: STBC 0x97941C ?fuzzFactor@NiBound@@0MA
float NiBound::fuzzFactor = 1e-04f;
// DATA: STBC 0x979420 ?tolerance@NiBound@@0MA
float NiBound::tolerance = 1e-06f;

/*
================
NiBound::NiBound
================
*/
// FUNCTION: STBC 0x8122C0
NiBound::NiBound() {
	unknown10 = true;
	center = NiPoint3::ZERO;
	radius = 0.0f;
}

/*
================
NiBound::~NiBound
================
*/
// FUNCTION: STBC 0x8122F0
NiBound::~NiBound() {
}

/*
================
NiBound::ComputeFromData

The sphere about the center of the points' axis-aligned box that holds
them all. No points give the zero sphere.
================
*/
// FUNCTION: STBC 0x812300
// Gamebryo 1.2.2: NiBound::ComputeFromData (NiBound_Win32.cpp)
void NiBound::ComputeFromData( int count, const NiPoint3 *points ) {
	if ( count <= 0 ) {
		center = NiPoint3::ZERO;
		radius = 0.0f;
		return;
	}

	NiPoint3 min = points[0];
	NiPoint3 max = min;
	int i;
	for ( i = 1; i < count; i++ ) {
		if ( min.x > points[i].x ) {
			min.x = points[i].x;
		}
		if ( min.y > points[i].y ) {
			min.y = points[i].y;
		}
		if ( min.z > points[i].z ) {
			min.z = points[i].z;
		}
		if ( max.x < points[i].x ) {
			max.x = points[i].x;
		}
		if ( max.y < points[i].y ) {
			max.y = points[i].y;
		}
		if ( max.z < points[i].z ) {
			max.z = points[i].z;
		}
	}

	center = 0.5f * ( min + max );

	float maxRadiusSqr = 0.0f;
	for ( i = 0; i < count; i++ ) {
		NiPoint3 diff = points[i] - center;
		float radiusSqr = diff.SqrLength();
		if ( radiusSqr > maxRadiusSqr ) {
			maxRadiusSqr = radiusSqr;
		}
	}

	radius = (float)sqrt( maxRadiusSqr );
	unknown10 = false;
}

/*
================
NiBound::ComputeMinimalBound

Starts from the first sphere and merges the others into it (Gamebryo
lineage), then clears the flag at 0x10 as SetCenterAndRadius does.
================
*/
// FUNCTION: STBC 0x8124A0
void NiBound::ComputeMinimalBound( NiTArray<const NiBound *> &bounds ) {
	const NiBound *bound = bounds.GetAt( 0 );
	center = bound->center;
	radius = bound->radius;

	for ( unsigned int i = 1; i < bounds.GetSize(); i++ ) {
		Merge( bounds.GetAt( i ) );
	}
	unknown10 = false;
}

/*
================
NiBound::WhichSide

The side of a plane the whole sphere lies on, or NO_SIDE when the plane
cuts it.
================
*/
// FUNCTION: STBC 0x8124F0
NiPlane::Side NiBound::WhichSide( const NiPlane &plane ) const {
	float distance = plane.GetNormal().Dot( center ) - plane.GetConstant();
	if ( distance <= -radius ) {
		return NiPlane::NEGATIVE_SIDE;
	}
	if ( distance >= radius ) {
		return NiPlane::POSITIVE_SIDE;
	}
	return NiPlane::NO_SIDE;
}

/*
================
NiBound::TestIntersect

Whether two spheres moving at constant velocities meet within deltaTime:
the squared distance of the centers, c + 2bt + at^2, is smallest at
t = -b / a.
================
*/
// FUNCTION: STBC 0x812540
// Gamebryo 1.2.2: NiBound::TestIntersect
bool NiBound::TestIntersect( float deltaTime, const NiBound &bound0, const NiPoint3 &velocity0,
							 const NiBound &bound1, const NiPoint3 &velocity1 ) {
	NiPoint3 velocity = velocity1 - velocity0;
	float a = velocity.SqrLength();
	NiPoint3 diff = bound1.GetCenter() - bound0.GetCenter();
	float c = diff.SqrLength();
	float radiusSum = bound0.GetRadius() + bound1.GetRadius();
	float radiusSumSqr = radiusSum * radiusSum;

	if ( a > 0.0f ) {
		float b = diff.Dot( velocity );
		if ( b <= 0.0f ) {
			if ( -deltaTime * a <= b ) {
				return a * c - b * b <= a * radiusSumSqr;
			}
			return deltaTime * ( deltaTime * a + 2.0f * b ) + c <= radiusSumSqr;
		}
	}

	return c <= radiusSumSqr;
}

/*
================
NiBound::FindIntersect

When and where two moving spheres first meet within deltaTime, and the
normals of both at the contact. Spheres that already overlap meet at time
0, halfway between their centers.
================
*/
// FUNCTION: STBC 0x812690
// Gamebryo 1.2.2: NiBound::FindIntersect
bool NiBound::FindIntersect( float deltaTime, const NiBound &bound0, const NiPoint3 &velocity0,
							 const NiBound &bound1, const NiPoint3 &velocity1, float &time, NiPoint3 &point,
							 bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 ) {
	NiPoint3 velocity = velocity1 - velocity0;
	float a = velocity.SqrLength();
	NiPoint3 diff = bound1.GetCenter() - bound0.GetCenter();
	float c = diff.SqrLength();
	float radiusSum = bound0.GetRadius() + bound1.GetRadius();
	float radiusSumSqr = radiusSum * radiusSum;

	if ( a > 0.0f ) {
		float b = diff.Dot( velocity );
		if ( b <= 0.0f ) {
			if ( -deltaTime * a <= b || deltaTime * ( deltaTime * a + 2.0f * b ) + c <= radiusSumSqr ) {
				float cDiff = c - radiusSumSqr;
				float discr = b * b - a * cDiff;
				if ( discr >= 0.0f ) {
					if ( cDiff <= 0.0f ) {
						time = 0.0f;
						point = 0.5f * ( bound0.GetCenter() + bound1.GetCenter() );
						if ( calcNormals ) {
							normal0 = diff;
							normal0.Unitize();
							normal1 = -normal0;
						}
					} else {
						time = -( b + sqrtf( discr ) ) / a;
						if ( time < 0.0f ) {
							time = 0.0f;
						} else if ( time > deltaTime ) {
							time = deltaTime;
						}

						NiPoint3 newDiff = diff + time * velocity;
						point = bound0.GetCenter() + time * velocity0 + ( bound0.GetRadius() / radiusSum ) * newDiff;
						if ( calcNormals ) {
							normal0 = newDiff;
							normal0.Unitize();
							normal1 = -normal0;
						}
					}
					return true;
				}
			}
			return false;
		}
	}

	if ( c <= radiusSumSqr ) {
		time = 0.0f;
		point = 0.5f * ( bound0.GetCenter() + bound1.GetCenter() );
		if ( calcNormals ) {
			normal0 = diff;
			normal0.Unitize();
			normal1 = -normal0;
		}
		return true;
	} else {
		return false;
	}
}

/*
================
NiBound::LoadBinary
================
*/
// FUNCTION: STBC 0x812B80
void NiBound::LoadBinary( NiStream &stream ) {
	center.LoadBinary( stream );
	NiStreamLoadBinary( stream, radius );
}

/*
================
NiBound::SaveBinary
================
*/
// FUNCTION: STBC 0x812BB0
void NiBound::SaveBinary( NiStream &stream ) {
	center.SaveBinary( stream );
	NiStreamSaveBinary( stream, radius );
}

/*
================
NiBound::GetViewerString

Returns "<prefix> = (x,y,z) , radius" in a new string that the caller
deletes.
================
*/
// FUNCTION: STBC 0x812BE0
char *NiBound::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 64];
	sprintf( string, "%s = (%f,%f,%f) , %f", prefix, center.x, center.y, center.z, radius );
	return string;
}
