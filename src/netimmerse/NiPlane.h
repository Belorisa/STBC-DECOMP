/*
===============================================================================

	NiPlane.h

	A plane by its unit normal and its constant: the points p with
	normal.Dot( p ) == constant.

	Evidence: the functions at 0x816860-0x816AD0 (constructors, assignment,
	comparison, streaming and the viewer string "(%f,%f,%f), %f") work on
	16 bytes, a point and a float; the point-and-normal constructor stores
	normal.Dot( point ) as the float. NiCamera holds six of them, its
	frustum planes. The member names follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NIPLANE_H__
#define __NIPLANE_H__

#include "NiPoint3.h"

class NiStream;

class NiPlane {
public:
					NiPlane();
					NiPlane( const NiPoint3 &normal_, float constant_ );
					NiPlane( const NiPoint3 &normal_, const NiPoint3 &point );
					NiPlane( const NiPoint3 &point0, const NiPoint3 &point1, const NiPoint3 &point2 );

	NiPlane &		operator=( const NiPlane &plane );
	bool			operator==( const NiPlane &plane );
	// AnimOnly.exe 0x43B7F0
	bool			operator!=( const NiPlane &plane ) { return !( *this == plane ); }

	void			SetNormal( const NiPoint3 &normal_ );
	const NiPoint3 &	GetNormal() const;
	void			SetConstant( float constant_ );
	float			GetConstant() const;
	// the signed distance of a point (Gamebryo lineage name)
	float			Distance( const NiPoint3 &point ) const;

	// the side of the plane a point lies on (Gamebryo lineage names)
	enum Side {
		NO_SIDE,
		POSITIVE_SIDE,
		NEGATIVE_SIDE
	};

	int				WhichSide( const NiPoint3 &point ) const;

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

protected:
	NiPoint3		normal;
	float			constant;
};

/*
================
NiPlane::WhichSide
================
*/
inline int NiPlane::WhichSide( const NiPoint3 &point ) const {
	float distance = normal.x * point.x + normal.y * point.y + normal.z * point.z - constant;
	if ( distance < 0.0f ) {
		return NEGATIVE_SIDE;
	} else if ( distance > 0.0f ) {
		return POSITIVE_SIDE;
	} else {
		return NO_SIDE;
	}
}

inline void NiPlane::SetNormal( const NiPoint3 &normal_ ) {
	normal = normal_;
}

inline const NiPoint3 &NiPlane::GetNormal() const {
	return normal;
}

inline void NiPlane::SetConstant( float constant_ ) {
	constant = constant_;
}

inline float NiPlane::GetConstant() const {
	return constant;
}

inline float NiPlane::Distance( const NiPoint3 &point ) const {
	return normal * point - constant;
}

#endif /* !__NIPLANE_H__ */
