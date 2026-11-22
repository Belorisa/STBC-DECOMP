/*
===============================================================================

	BezierCurve.h

	A curve through a list of control points, evaluated with the Bernstein
	polynomials of their count: a waypoint's path to the next waypoint (four
	points, built by Waypoint::UpdateCurve) and the camera's named modes
	(0x42B2E0) use it.

	Evidence: the vtable 0x88B9D0 (five slots) is stored by four
	constructors (0x460AA0, 0x460B30, 0x460B90, 0x460BF0); the destructor
	(0x460C10) frees the point and coefficient arrays and stores the base's
	vtable 0x88B9E4, whose second slot is _purecall. Slot 1 sums the points
	weighted by C(n-1,i) t^i (1-t)^(n-1-i) (the binomial coefficients come
	from 0x460D20), slot 2 the derivative, and slots 3 and 4 stream the
	curve through TGStream. No string names either class: Curve and
	BezierCurve, and every method name, are invented.

===============================================================================
*/

#ifndef __BEZIERCURVE_H__
#define __BEZIERCURVE_H__

#include "NiPoint3.h"

class TGStream;

// A curve that can be evaluated at a parameter from 0 to 1 (invented name).
// FUNCTION: STBC 0x00461100 ??_GCurve@@UAEPAXI@Z
class Curve {
public:
	// the out-of-line copy is the unwind action of the derived constructors
	// FUNCTION: STBC 0x004610f0 ??1Curve@@UAE@XZ
	virtual				~Curve() {}

	// the point at t
	virtual NiPoint3	Evaluate( float t ) = 0;
};

// FUNCTION: STBC 0x00460b10 ??_GBezierCurve@@UAEPAXI@Z
class BezierCurve : public Curve {
public:
						BezierCurve( int count_ );
						BezierCurve( int count_, NiPoint3 *points_ );
						BezierCurve( const BezierCurve &curve );
						BezierCurve();
	virtual				~BezierCurve();

	virtual NiPoint3	Evaluate( float t );
	// the curve's tangent (its derivative along t) at t
	virtual NiPoint3	EvaluateDerivative( float t );
	virtual bool		WriteToStream( TGStream *stream );
	virtual bool		ReadFromStream( TGStream *stream );

	void				SetPoint( int index, const NiPoint3 &point );
	// replaces the control points with a copy of count_ points
	void				SetPoints( int count_, NiPoint3 *points_ );
	NiPoint3 *			GetPoints();
	int					GetCount() const { return count; }		// invented name

protected:
	// fills coefficients with the binomial coefficients C(count-1,i)
	void				ComputeCoefficients();

	int					count;			// 0x04 control points
	NiPoint3 *			points;			// 0x08 owned, count of them
	float *				coefficients;	// 0x0C owned, one per point
};

#endif /* !__BEZIERCURVE_H__ */
