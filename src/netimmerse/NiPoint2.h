/*
===============================================================================

	NiPoint2.h

	A two-component float vector; geometry data keeps its texture
	coordinates in arrays of them.

	Evidence: NiGeometryData's texture coordinate arrays (0x2C) hold 8-byte
	elements, vertex count times set count of them (0x7ECE70). The unit at
	0x820010 builds ZERO, UNIT_X and UNIT_Y and streams and prints the two
	components ("%s = (%f,%f)").

===============================================================================
*/

#ifndef __NIPOINT2_H__
#define __NIPOINT2_H__

#include "NiMath.h"

class NiStream;

class NiPoint2 {
public:
	float			x;
	float			y;

	// FUNCTION: STBC 0x00620680 ??0NiPoint2@@QAE@XZ @game/AppWrap
					NiPoint2() {}
					NiPoint2( float x_, float y_ );
					// FUNCTION: STBC 0x46C2A0 ??0NiPoint2@@QAE@ABV0@@Z @game/OBBNode
					NiPoint2( const NiPoint2 &pt ) { x = pt.x; y = pt.y; }

	// FUNCTION: STBC 0x4BAF30 ??4NiPoint2@@QAEAAV0@ABV0@@Z @game/FlareController
	NiPoint2 &		operator=( const NiPoint2 &pt ) { x = pt.x; y = pt.y; return *this; }

	bool			operator==( const NiPoint2 &pt ) const;
	bool			operator!=( const NiPoint2 &pt ) const;

	NiPoint2		operator+( const NiPoint2 &pt ) const;
	NiPoint2		operator-( const NiPoint2 &pt ) const;
	float			operator*( const NiPoint2 &pt ) const;
	NiPoint2		operator*( float scalar ) const;
	NiPoint2		operator/( float scalar ) const;
	NiPoint2		operator-() const;
	friend NiPoint2	operator*( float scalar, const NiPoint2 &pt );
	NiPoint2		ComponentProduct( const NiPoint2 &p0, const NiPoint2 &p1 );

	NiPoint2 &		operator+=( const NiPoint2 &pt );
	NiPoint2 &		operator-=( const NiPoint2 &pt );
	NiPoint2 &		operator*=( float scalar );
	NiPoint2 &		operator/=( float scalar );

	float			Length() const;
	float			SqrLength() const;
	float			Dot( const NiPoint2 &pt ) const;
	float			Unitize();

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

	static const NiPoint2	ZERO;
	static const NiPoint2	UNIT_X;
	static const NiPoint2	UNIT_Y;
};

inline NiPoint2::NiPoint2( float x_, float y_ ) {
	x = x_;
	y = y_;
}

inline bool NiPoint2::operator==( const NiPoint2 &pt ) const {
	return x == pt.x && y == pt.y;
}

inline bool NiPoint2::operator!=( const NiPoint2 &pt ) const {
	return !operator==( pt );
}

// built in a copy of this point and returned by value, as NiColorA's
// FUNCTION: STBC 0x4BAF50 ??HNiPoint2@@QBE?AV0@ABV0@@Z @game/VisibleDamageBuilder
inline NiPoint2 NiPoint2::operator+( const NiPoint2 &pt ) const {
	NiPoint2 result = *this;
	result.x += pt.x;
	result.y += pt.y;
	return result;
}

// FUNCTION: STBC 0x4BAF80 ??GNiPoint2@@QBE?AV0@ABV0@@Z @game/VisibleDamageBuilder
inline NiPoint2 NiPoint2::operator-( const NiPoint2 &pt ) const {
	NiPoint2 result = *this;
	result.x -= pt.x;
	result.y -= pt.y;
	return result;
}

// the dot product
inline float NiPoint2::operator*( const NiPoint2 &pt ) const {
	return x * pt.x + y * pt.y;
}

// FUNCTION: STBC 0x4BAFB0 ??DNiPoint2@@QBE?AV0@M@Z @netimmerse/NiTriBasedGeom
inline NiPoint2 NiPoint2::operator*( float scalar ) const {
	NiPoint2 result = *this;
	result.x *= scalar;
	result.y *= scalar;
	return result;
}

inline NiPoint2 NiPoint2::operator/( float scalar ) const {
	NiPoint2 result = *this;
	result.x /= scalar;
	result.y /= scalar;
	return result;
}

inline NiPoint2 NiPoint2::operator-() const {
	return NiPoint2( -x, -y );
}

// FUNCTION: STBC 0x851100 ??D@YA?AVNiPoint2@@MABV0@@Z @netimmerse/NiBezierCylinder
inline NiPoint2 operator*( float scalar, const NiPoint2 &pt ) {
	return pt * scalar;
}

inline NiPoint2 &NiPoint2::operator+=( const NiPoint2 &pt ) {
	x += pt.x;
	y += pt.y;
	return *this;
}

inline NiPoint2 &NiPoint2::operator-=( const NiPoint2 &pt ) {
	x -= pt.x;
	y -= pt.y;
	return *this;
}

inline NiPoint2 &NiPoint2::operator*=( float scalar ) {
	x *= scalar;
	y *= scalar;
	return *this;
}

inline NiPoint2 &NiPoint2::operator/=( float scalar ) {
	x /= scalar;
	y /= scalar;
	return *this;
}

// the product of each component pair (Gamebryo declares it a member, not a static)
inline NiPoint2 NiPoint2::ComponentProduct( const NiPoint2 &p0, const NiPoint2 &p1 ) {
	return NiPoint2( p0.x * p1.x, p0.y * p1.y );
}

// sqrtf, not Gamebryo 1.1.2's NiSqrt (NiPoint3::Length's form): with NiSqrt,
// DisableGlossMaps (0x45CD20) takes another loop branch target than retail's.
inline float NiPoint2::Length() const {
	return sqrtf( x * x + y * y );
}

inline float NiPoint2::SqrLength() const {
	return x * x + y * y;
}

inline float NiPoint2::Dot( const NiPoint2 &pt ) const {
	return x * pt.x + y * pt.y;
}

inline float NiPoint2::Unitize() {
	float length = Length();
	if ( length > 1e-06f ) {
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;
	} else {
		x = 0.0f;
		y = 0.0f;
		length = 0.0f;
	}
	return length;
}

#endif /* !__NIPOINT2_H__ */
