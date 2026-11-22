// NiBox.cpp -- the oriented box

#include "NiRTLib.h"
#include "NiBox.h"
#include "NiMatrix3.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007bf9c0 _$E44
// FUNCTION: STBC 0x007bf9e0 _$E43
// FUNCTION: STBC 0x007bf9f0 _$E17
// FUNCTION: STBC 0x007bfa00 _$E14
// FUNCTION: STBC 0x007bfa10 _$E16
// FUNCTION: STBC 0x007bfa20 _$E15
// FUNCTION: STBC 0x007bfa30 _$E23
// FUNCTION: STBC 0x007bfa40 _$E20
// FUNCTION: STBC 0x007bfa50 _$E22
// FUNCTION: STBC 0x007bfa60 _$E21

/*
================
NiBox::GetPoint

The point at the given distances along the axes from the center
(Gamebryo 1.2.2: NiBox::GetPoint).
================
*/
// FUNCTION: STBC 0x7BF450
NiPoint3 NiBox::GetPoint( float p0, float p1, float p2 ) const {
	return center + ( p0 * axis[0] ) + ( p1 * axis[1] ) + ( p2 * axis[2] );
}

/*
================
NiBox::operator==

Gamebryo 1.2.2: NiBox::operator==.
================
*/
// FUNCTION: STBC 0x7BF510 ??8NiBox@@QBE_NABV0@@Z
bool NiBox::operator==( const NiBox &box ) const {
	if ( center != box.center ) {
		return false;
	}
	for ( int i = 0; i < 3; i++ ) {
		if ( axis[i] != box.axis[i] || extent[i] != box.extent[i] ) {
			return false;
		}
	}
	return true;
}

/*
================
NiBox::operator!=
================
*/
// FUNCTION: STBC 0x7BF5A0 ??9NiBox@@QBE_NABV0@@Z
bool NiBox::operator!=( const NiBox &box ) const {
	return !operator==( box );
}

/*
================
NiBox::CreateFromData

The center is the mean of the points. The axes are the eigenvectors of a
covariance matrix, and the center and the half extents then fit the box to
the points projected on them. Gamebryo 1.2.2: NiBox::CreateFromData, which
adds a degenerate-case check.
================
*/
// FUNCTION: STBC 0x7BF5C0
// PARKED: operand sort: center += points[i] weighs the element's z (temp #28 address, hash 0x3B8) over center.z (this, 0x58) as it does y (0x3B0 over 0x50); retail sorts center.z first for z only, which no renumbering of one temporary gives (it would flip y with it); the first bound dot's y/z terms are the same kind of sort
void NiBox::CreateFromData( unsigned short count, const NiPoint3 *points ) {
	center = points[0];
	unsigned short i;
	for ( i = 1; i < count; i++ ) {
		center += points[i];
	}
	center /= (float)count;

	// BUG: the covariance adds the center's products, not the points' differences from it, so
	// every point contributes the same term (Gamebryo 1.2.2 computes the difference and drops it)
	NiMatrix3 covariance = NiMatrix3::ZERO;
	NiPoint3 diff;
	for ( i = 0; i < count; i++ ) {
		covariance.SetEntry( 0, 0, covariance.GetEntry( 0, 0 ) + center.x * center.x );
		covariance.SetEntry( 0, 1, covariance.GetEntry( 0, 1 ) + center.x * center.y );
		covariance.SetEntry( 0, 2, covariance.GetEntry( 0, 2 ) + center.x * center.z );
		covariance.SetEntry( 1, 1, covariance.GetEntry( 1, 1 ) + center.y * center.y );
		covariance.SetEntry( 1, 2, covariance.GetEntry( 1, 2 ) + center.y * center.z );
		covariance.SetEntry( 2, 2, covariance.GetEntry( 2, 2 ) + center.z * center.z );
	}
	covariance.SetEntry( 1, 0, covariance.GetEntry( 0, 1 ) );
	covariance.SetEntry( 2, 0, covariance.GetEntry( 0, 2 ) );
	covariance.SetEntry( 2, 1, covariance.GetEntry( 1, 2 ) );

	float eigenvalue[3];
	covariance.EigenSolveSymmetric( eigenvalue, axis );

	// the axes enclose the points
	NiPoint3 min = points[0] - center;
	NiPoint3 max = min;
	for ( i = 1; i < count; i++ ) {
		diff = points[i] - center;

		float dot = axis[0].Dot( diff );
		if ( dot < min.x ) {
			min.x = dot;
		} else if ( dot > max.x ) {
			max.x = dot;
		}

		dot = axis[1].Dot( diff );
		if ( dot < min.y ) {
			min.y = dot;
		} else if ( dot > max.y ) {
			max.y = dot;
		}

		dot = axis[2].Dot( diff );
		if ( dot < min.z ) {
			min.z = dot;
		} else if ( dot > max.z ) {
			max.z = dot;
		}
	}

	center += ( 0.5f * ( max.x + min.x ) ) * axis[0];
	center += ( 0.5f * ( max.y + min.y ) ) * axis[1];
	center += ( 0.5f * ( max.z + min.z ) ) * axis[2];
	extent[0] = 0.5f * ( max.x - min.x );
	extent[1] = 0.5f * ( max.y - min.y );
	extent[2] = 0.5f * ( max.z - min.z );
}

/*
================
NiBox::LoadBinary

Gamebryo 1.2.2: NiBox::LoadBinary.
================
*/
// FUNCTION: STBC 0x7BF940
void NiBox::LoadBinary( NiStream &stream ) {
	center.LoadBinary( stream );
	for ( int i = 0; i < 3; i++ ) {
		axis[i].LoadBinary( stream );
	}
	NiStreamLoadBinary( stream, extent, 3 );
}

/*
================
NiBox::SaveBinary

Gamebryo 1.2.2: NiBox::SaveBinary.
================
*/
// FUNCTION: STBC 0x7BF980
void NiBox::SaveBinary( NiStream &stream ) {
	center.SaveBinary( stream );
	for ( int i = 0; i < 3; i++ ) {
		axis[i].SaveBinary( stream );
	}
	NiBinaryStreamSave( *stream.GetOstr(), extent, 3 );
}
