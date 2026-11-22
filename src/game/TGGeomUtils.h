/*
===============================================================================

	TGGeomUtils.h

	The scripting namespace of the geometry helpers: the App methods
	TGGeomUtils_LineSphereIntersection and TGGeomUtils_CalcAngularDifferenceSingle.
	The class is the SWIG name's prefix; its members are inline in the
	wrapper unit, as the wrappers' bytes show.

===============================================================================
*/

#ifndef __TGGEOMUTILS_H__
#define __TGGEOMUTILS_H__

#include <math.h>
#include "NiMath.h"
#include "TGPoint3.h"

class TGGeomUtils {
public:
	// How many points of the ray from start along direction meet the sphere (0 to 2),
	// the first and second in hit1 and hit2.
	static int	LineSphereIntersection( const NiPoint3 &start, const NiPoint3 &direction, const NiPoint3 &center, float radius, NiPoint3 *hit1, NiPoint3 *hit2 ) {
					return Unknown4570D0( start, direction, center, radius, hit1, hit2 );
				}

	// The angle between two vectors, in angle, and the unit vector perpendicular to both, in axis.
	// The angle is NiACos of the dot product: with the clamp in NiACos and the dot
	// in NiPoint3's operator* the wrapper's inline budget calls NiPoint3's
	// three-float constructor out of line inside UnitCross, as retail does.
	static void	CalcAngularDifferenceSingle( float *angle, const NiPoint3 &from, const NiPoint3 &to, NiPoint3 *axis ) {
					*axis = from.UnitCross( to );
					*angle = NiACos( from * to );
				}
};

#endif /* !__TGGEOMUTILS_H__ */
