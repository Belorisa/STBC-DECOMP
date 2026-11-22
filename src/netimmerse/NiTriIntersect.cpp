// NiTriIntersect.cpp -- the moving triangle-triangle test

#include "NiRTLib.h"
#include <assert.h>
#include <math.h>
#include <string.h>

#include "NiTriIntersect.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0081fff0 _$E36
// FUNCTION: STBC 0x0081ff90 _$E17
// FUNCTION: STBC 0x0081ffb0 _$E15
// FUNCTION: STBC 0x0081ffc0 _$E23
// FUNCTION: STBC 0x0081ffe0 _$E21

// the squared length below which the normals count as parallel, and the
// speed along the normal below which parallel triangles count as at rest
// DATA: STBC 0x9796FC ?epsilon@NiTriIntersect@@2MA
float NiTriIntersect::epsilon = 1e-05f;

// a triangle's centroid is the mean of its vertices
// DATA: STBC 0x89A760
static const float ONE_THIRD = 1.0f / 3.0f;

/*
================
NiTriIntersect::NiTriIntersect
================
*/
// FUNCTION: STBC 0x819310
// Gamebryo 1.2.2: NiTriIntersect::NiTriIntersect
NiTriIntersect::NiTriIntersect( NiPoint3 *p_[3], const NiPoint3 &velocity0_, NiPoint3 *q_[3],
								const NiPoint3 &velocity1_ )
	: velocity0( velocity0_ ), velocity1( velocity1_ ) {
	memcpy( p, p_, 3 * sizeof( NiPoint3 * ) );
	memcpy( q, q_, 3 * sizeof( NiPoint3 * ) );

	velocity = velocity1 - velocity0;
	e0 = *p[1] - *p[0];
	e1 = *p[2] - *p[0];
	f0 = *q[1] - *q[0];
	f1 = *q[2] - *q[0];
	diff = *q[0] - *p[0];
	axis = 0;
}

/*
================
NiTriIntersect::Test

The triangles meet unless some axis keeps them apart for the whole time
step. Parallel triangles moving across their plane are first carried
into it.
================
*/
// FUNCTION: STBC 0x8194B0
// Gamebryo 1.2.2: NiTriIntersect::Test
bool NiTriIntersect::Test( float deltaTime_ ) {
	deltaTime = deltaTime_;

	if ( !TestN() ) {
		return false;
	}
	if ( !TestM() ) {
		return false;
	}

	nxM = n.Cross( m );
	nxMDotNxM = nxM.Dot( nxM );
	if ( nxMDotNxM > epsilon ) {
		if ( !TestE0xF0() ) {
			return false;
		}
		if ( !TestE0xF1() ) {
			return false;
		}
		if ( !TestE0xF2() ) {
			return false;
		}
		if ( !TestE1xF0() ) {
			return false;
		}
		if ( !TestE1xF1() ) {
			return false;
		}
		if ( !TestE1xF2() ) {
			return false;
		}
		if ( !TestE2xF0() ) {
			return false;
		}
		if ( !TestE2xF1() ) {
			return false;
		}
		if ( !TestE2xF2() ) {
			return false;
		}
	} else {
		if ( fabsf( nDotVelocity ) > epsilon ) {
			float time = -nDotDiff / nDotVelocity;
			diff += time * velocity;
			velocity = NiPoint3::ZERO;
		}
		if ( !TestMxF0() ) {
			return false;
		}
		if ( !TestMxF1() ) {
			return false;
		}
		if ( !TestMxF2() ) {
			return false;
		}
		if ( !TestNxE0() ) {
			return false;
		}
		if ( !TestNxE1() ) {
			return false;
		}
		if ( !TestNxE2() ) {
			return false;
		}
	}
	return true;
}

/*
================
NiTriIntersect::Find

As Test, recording the axis that keeps the triangles apart longest; the
contact is where they meet on it.
================
*/
// FUNCTION: STBC 0x819990
// Gamebryo 1.2.2: NiTriIntersect::Find; its one-line axis tests are unbraced as there (their scopes'
// cost decides whether the plane offset's operator* is expanded)
bool NiTriIntersect::Find( float deltaTime_, float &time, NiPoint3 &point, bool calcNormals, NiPoint3 *normal0,
						   NiPoint3 *normal1 ) {
	deltaTime = deltaTime_;
	firstTime = 0.0f;

	if ( !FindN() )
		return false;
	if ( !FindM() )
		return false;

	nxM = n.Cross( m );
	nxMDotNxM = nxM.Dot( nxM );
	if ( nxMDotNxM > epsilon ) {
		if ( !FindE0xF0() )
			return false;
		if ( !FindE0xF1() )
			return false;
		if ( !FindE0xF2() )
			return false;
		if ( !FindE1xF0() )
			return false;
		if ( !FindE1xF1() )
			return false;
		if ( !FindE1xF2() )
			return false;
		if ( !FindE2xF0() )
			return false;
		if ( !FindE2xF1() )
			return false;
		if ( !FindE2xF2() )
			return false;
	} else {
		if ( fabsf( nDotVelocity ) > epsilon ) {
			float planeTime = -nDotDiff / nDotVelocity;
			diff += planeTime * velocity;
			velocity = NiPoint3::ZERO;
		}
		if ( !FindMxF0() )
			return false;
		if ( !FindMxF1() )
			return false;
		if ( !FindMxF2() )
			return false;
		if ( !FindNxE0() )
			return false;
		if ( !FindNxE1() )
			return false;
		if ( !FindNxE2() )
			return false;
	}

	LocateIntersection();
	time = firstTime;
	point = contactPoint;
	if ( calcNormals ) {
		*normal0 = n;
		normal0->Unitize();
		*normal1 = m;
		normal1->Unitize();
	}
	return true;
}

/*
================
NiTriIntersect::Sort
================
*/
// FUNCTION: STBC 0x819F30 ?Sort@NiTriIntersect@@IAEXMMAAM0@Z
void NiTriIntersect::Sort( float value0, float value1, float &min, float &max ) {
	if ( value1 >= value0 ) {
		min = value0;
		max = value1;
	} else {
		min = value1;
		max = value0;
	}
}

/*
================
NiTriIntersect::Sort
================
*/
// FUNCTION: STBC 0x819F70 ?Sort@NiTriIntersect@@IAEXMMMAAM0@Z
void NiTriIntersect::Sort( float value0, float value1, float value2, float &min, float &max ) {
	if ( value2 >= value1 ) {
		if ( value1 >= value0 ) {
			min = value0;
			max = value2;
		} else if ( value2 >= value0 ) {
			min = value1;
			max = value2;
		} else {
			min = value1;
			max = value0;
		}
	} else {
		if ( value2 >= value0 ) {
			min = value0;
			max = value1;
		} else if ( value1 >= value0 ) {
			min = value2;
			max = value1;
		} else {
			min = value2;
			max = value0;
		}
	}
}

/*
================
NiTriIntersect::Sort
================
*/
// FUNCTION: STBC 0x81A030 ?Sort@NiTriIntersect@@IAEXMMMQAMQAH@Z
void NiTriIntersect::Sort( float value0, float value1, float value2, float sorted[3], int order[3] ) {
	if ( value2 >= value1 ) {
		if ( value1 >= value0 ) {
			sorted[0] = value0;
			sorted[1] = value1;
			sorted[2] = value2;
			order[0] = 0;
			order[1] = 1;
			order[2] = 2;
		} else if ( value2 >= value0 ) {
			sorted[0] = value1;
			sorted[1] = value0;
			sorted[2] = value2;
			order[0] = 1;
			order[1] = 0;
			order[2] = 2;
		} else {
			sorted[0] = value1;
			sorted[1] = value2;
			sorted[2] = value0;
			order[0] = 1;
			order[1] = 2;
			order[2] = 0;
		}
	} else {
		if ( value2 >= value0 ) {
			sorted[0] = value0;
			sorted[1] = value2;
			sorted[2] = value1;
			order[0] = 0;
			order[1] = 2;
			order[2] = 1;
		} else if ( value1 >= value0 ) {
			sorted[0] = value2;
			sorted[1] = value0;
			sorted[2] = value1;
			order[0] = 2;
			order[1] = 0;
			order[2] = 1;
		} else {
			sorted[0] = value2;
			sorted[1] = value1;
			sorted[2] = value0;
			order[0] = 2;
			order[1] = 1;
			order[2] = 0;
		}
	}
}

/*
================
NiTriIntersect::TestIntervals

Intervals apart at the start stay apart when the second has not reached
the first by the end of the step.
================
*/
// FUNCTION: STBC 0x81A190
bool NiTriIntersect::TestIntervals( float min0, float max0, float min1, float max1, float speed ) {
	if ( min1 > max0 ) {
		if ( min1 + speed * deltaTime > max0 ) {
			return false;
		}
	} else if ( max1 < min0 ) {
		if ( max1 + speed * deltaTime < min0 ) {
			return false;
		}
	}
	return true;
}

/*
================
NiTriIntersect::FindIntervals

As TestIntervals; intervals that meet within the step keep the axis, the
time they meet and their projections when they meet later than on any
axis so far.
================
*/
// FUNCTION: STBC 0x81A1F0
// Gamebryo 1.2.2: NiTriIntersect::FindIntervals
bool NiTriIntersect::FindIntervals( unsigned int axis_, float projection0_[3], int order0_[3], float projection1_[3],
									int order1_[3], float speed ) {
	float time;
	if ( projection1_[0] > projection0_[2] ) {
		if ( projection1_[0] + deltaTime * speed > projection0_[2] ) {
			return false;
		}

		time = ( projection0_[2] - projection1_[0] ) / speed;
		if ( time > firstTime ) {
			firstTime = time;
			axis = axis_;
			memcpy( projection0, projection0_, 3 * sizeof( float ) );
			memcpy( order0, order0_, 3 * sizeof( int ) );
			memcpy( projection1, projection1_, 3 * sizeof( float ) );
			memcpy( order1, order1_, 3 * sizeof( int ) );
		}
	} else if ( projection1_[2] < projection0_[0] ) {
		if ( projection1_[2] + deltaTime * speed < projection0_[0] ) {
			return false;
		}

		time = ( projection0_[0] - projection1_[2] ) / speed;
		if ( time > firstTime ) {
			firstTime = time;
			axis = axis_;
			memcpy( projection0, projection0_, 3 * sizeof( float ) );
			memcpy( order0, order0_, 3 * sizeof( int ) );
			memcpy( projection1, projection1_, 3 * sizeof( float ) );
			memcpy( order1, order1_, 3 * sizeof( int ) );
		}
	}

	return true;
}

/*
================
NiTriIntersect::TestN

The first triangle projects to a point on its normal.
================
*/
// FUNCTION: STBC 0x81A300
bool NiTriIntersect::TestN() {
	n = e0.Cross( e1 );
	nDotF0 = n.Dot( f0 );
	nDotF1 = n.Dot( f1 );
	nDotDiff = n.Dot( diff );
	nDotVelocity = n.Dot( velocity );

	float min1, max1;
	Sort( nDotDiff, nDotDiff + nDotF0,
				 nDotDiff + nDotF1, min1, max1 );
	return TestIntervals( 0.0f, 0.0f, min1, max1, nDotVelocity );
}

/*
================
NiTriIntersect::TestM

The second triangle projects to a point on its normal.
================
*/
// FUNCTION: STBC 0x81A4D0
bool NiTriIntersect::TestM() {
	m = f0.Cross( f1 );
	mDotE0 = m.Dot( e0 );
	mDotE1 = m.Dot( e1 );
	mDotDiff = m.Dot( diff );
	mDotVelocity = m.Dot( velocity );

	float min0, max0;
	Sort( 0.0f, mDotE0, mDotE1, min0, max0 );
	return TestIntervals( min0, max0, mDotDiff, mDotDiff, mDotVelocity );
}

/*
================
NiTriIntersect::TestE0xF0

The axis is the cross product of the first edges of both triangles; the
first triangle's vertices project to 0, 0 and the normal against the
other edge, the second's to the offset and the offset plus its normal
against this edge.
================
*/
// FUNCTION: STBC 0x81A690
bool NiTriIntersect::TestE0xF0() {
	e0xF0 = e0.Cross( f0 );
	e0xF0DotDiff = e0xF0.Dot( diff );
	e0xF0DotVelocity = e0xF0.Dot( velocity );

	float min0, max0, min1, max1;
	Sort( 0.0f, -nDotF0, min0, max0 );
	Sort( e0xF0DotDiff, e0xF0DotDiff, e0xF0DotDiff + mDotE0,
				 min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e0xF0DotVelocity );
}

/*
================
NiTriIntersect::TestE0xF1
================
*/
// FUNCTION: STBC 0x81A7F0
bool NiTriIntersect::TestE0xF1() {
	e0xF1 = e0.Cross( f1 );
	e0xF1DotDiff = e0xF1.Dot( diff );
	e0xF1DotVelocity = e0xF1.Dot( velocity );

	float min0, max0, min1, max1;
	Sort( 0.0f, -nDotF1, min0, max0 );
	Sort( e0xF1DotDiff, e0xF1DotDiff - mDotE0, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e0xF1DotVelocity );
}

/*
================
NiTriIntersect::TestE0xF2

The third edges are the differences of the first two, and so are the
projections onto their axes.
================
*/
// FUNCTION: STBC 0x81A960
bool NiTriIntersect::TestE0xF2() {
	e0xF2DotDiff = e0xF1DotDiff - e0xF0DotDiff;
	e0xF2DotVelocity = e0xF1DotVelocity - e0xF0DotVelocity;
	nDotF2 = nDotF1 - nDotF0;

	float min0, max0, min1, max1;
	Sort( 0.0f, -nDotF2, min0, max0 );
	Sort( e0xF2DotDiff, e0xF2DotDiff - mDotE0, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e0xF2DotVelocity );
}

/*
================
NiTriIntersect::TestE1xF0

The second triangle's interval starts from the offset on the axis of the
first edges, not on this axis (an original slip, kept).
================
*/
// FUNCTION: STBC 0x81AA60
bool NiTriIntersect::TestE1xF0() {
	e1xF0 = e1.Cross( f0 );
	e1xF0DotDiff = e1xF0.Dot( diff );
	e1xF0DotVelocity = e1xF0.Dot( velocity );

	float min0, max0, min1, max1;
	Sort( 0.0f, nDotF0, min0, max0 );
	Sort( e1xF0DotDiff, e0xF0DotDiff + mDotE1, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e1xF0DotVelocity );
}

/*
================
NiTriIntersect::TestE1xF1
================
*/
// FUNCTION: STBC 0x81ABD0
bool NiTriIntersect::TestE1xF1() {
	e1xF1 = e1.Cross( f1 );
	e1xF1DotDiff = e1xF1.Dot( diff );
	e1xF1DotVelocity = e1xF1.Dot( velocity );

	float min0, max0, min1, max1;
	Sort( 0.0f, nDotF1, min0, max0 );
	Sort( e1xF1DotDiff, e1xF1DotDiff - mDotE1, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e1xF1DotVelocity );
}

/*
================
NiTriIntersect::TestE1xF2
================
*/
// FUNCTION: STBC 0x81AD50
bool NiTriIntersect::TestE1xF2() {
	e1xF2DotDiff = e1xF1DotDiff - e1xF0DotDiff;
	e1xF2DotVelocity = e1xF1DotVelocity - e1xF0DotVelocity;

	float min0, max0, min1, max1;
	Sort( 0.0f, nDotF2, min0, max0 );
	Sort( e1xF2DotDiff, e1xF2DotDiff - mDotE1, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e1xF2DotVelocity );
}

/*
================
NiTriIntersect::TestE2xF0
================
*/
// FUNCTION: STBC 0x81AE50
bool NiTriIntersect::TestE2xF0() {
	e2xF0DotDiff = e1xF0DotDiff - e0xF0DotDiff;
	e2xF0DotVelocity = e1xF0DotVelocity - e0xF0DotVelocity;
	mDotE2 = mDotE1 - mDotE0;

	float min0, max0, min1, max1;
	Sort( 0.0f, nDotF0, min0, max0 );
	Sort( e2xF0DotDiff, e2xF0DotDiff + mDotE2, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e2xF0DotVelocity );
}

/*
================
NiTriIntersect::TestE2xF1
================
*/
// FUNCTION: STBC 0x81AF60
bool NiTriIntersect::TestE2xF1() {
	e2xF1DotDiff = e1xF1DotDiff - e0xF1DotDiff;
	e2xF1DotVelocity = e1xF1DotVelocity - e0xF1DotVelocity;

	float min0, max0, min1, max1;
	Sort( 0.0f, nDotF1, min0, max0 );
	Sort( e2xF1DotDiff, e2xF1DotDiff - mDotE2, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e2xF1DotVelocity );
}

/*
================
NiTriIntersect::TestE2xF2
================
*/
// FUNCTION: STBC 0x81B060
bool NiTriIntersect::TestE2xF2() {
	e2xF2DotDiff = e2xF1DotDiff - e2xF0DotDiff;
	e2xF2DotVelocity = e2xF1DotVelocity - e2xF0DotVelocity;

	float min0, max0, min1, max1;
	Sort( 0.0f, nDotF2, min0, max0 );
	Sort( e2xF2DotDiff, e2xF2DotDiff - mDotE2, min1, max1 );
	return TestIntervals( min0, max0, min1, max1, e2xF2DotVelocity );
}

/*
================
NiTriIntersect::TestMxF0

Parallel triangles: the axis is the second normal crossed with the
second triangle's first edge, along which the second triangle spans the
squared length of its normal.
================
*/
// FUNCTION: STBC 0x81B160
bool NiTriIntersect::TestMxF0() {
	mDotM = m.Dot( m );
	mxF0 = m.Cross( f0 );
	mxF0DotE0 = mxF0.Dot( e0 );
	mxF0DotE1 = mxF0.Dot( e1 );
	mxF0DotDiff = mxF0.Dot( diff );
	mxF0DotVelocity = mxF0.Dot( velocity );

	float min0, max0;
	Sort( 0.0f, mxF0DotE0, mxF0DotE1, min0, max0 );
	return TestIntervals( min0, max0, mxF0DotDiff, mxF0DotDiff + mDotM,
						  mxF0DotVelocity );
}

/*
================
NiTriIntersect::TestMxF1
================
*/
// FUNCTION: STBC 0x81B350
bool NiTriIntersect::TestMxF1() {
	mxF1 = m.Cross( f1 );
	mxF1DotE0 = mxF1.Dot( e0 );
	mxF1DotE1 = mxF1.Dot( e1 );
	mxF1DotDiff = mxF1.Dot( diff );
	mxF1DotVelocity = mxF1.Dot( velocity );

	float min0, max0;
	Sort( 0.0f, mxF1DotE0, mxF1DotE1, min0, max0 );
	return TestIntervals( min0, max0, mxF1DotDiff - mDotM, mxF1DotDiff,
						  mxF1DotVelocity );
}

/*
================
NiTriIntersect::TestMxF2
================
*/
// FUNCTION: STBC 0x81B510
bool NiTriIntersect::TestMxF2() {
	mxF2DotE0 = mxF1DotE0 - mxF0DotE0;
	mxF2DotE1 = mxF1DotE1 - mxF0DotE1;
	mxF2DotDiff = mxF1DotDiff - mxF0DotDiff;
	mxF2DotVelocity = mxF1DotVelocity - mxF0DotVelocity;

	float min0, max0;
	Sort( 0.0f, mxF2DotE0, mxF2DotE1, min0, max0 );
	return TestIntervals( min0, max0, mxF2DotDiff - mDotM, mxF2DotDiff,
						  mxF2DotVelocity );
}

/*
================
NiTriIntersect::TestNxE0

The axis is the first normal crossed with the first triangle's first
edge, along which the first triangle spans the squared length of its
normal.
================
*/
// FUNCTION: STBC 0x81B600
bool NiTriIntersect::TestNxE0() {
	nDotN = n.Dot( n );
	nxE0 = n.Cross( e0 );
	nxE0DotF0 = nxE0.Dot( f0 );
	nxE0DotF1 = nxE0.Dot( f1 );
	nxE0DotDiff = nxE0.Dot( diff );
	nxE0DotVelocity = nxE0.Dot( velocity );

	float min1, max1;
	Sort( nxE0DotDiff, nxE0DotDiff + nxE0DotF0,
				 nxE0DotDiff + nxE0DotF1, min1, max1 );
	return TestIntervals( 0.0f, nDotN, min1, max1, nxE0DotVelocity );
}

/*
================
NiTriIntersect::TestNxE1
================
*/
// FUNCTION: STBC 0x81B800
bool NiTriIntersect::TestNxE1() {
	nxE1 = n.Cross( e1 );
	nxE1DotF0 = nxE1.Dot( f0 );
	nxE1DotF1 = nxE1.Dot( f1 );
	nxE1DotDiff = nxE1.Dot( diff );
	nxE1DotVelocity = nxE1.Dot( velocity );

	float min1, max1;
	Sort( nxE1DotDiff, nxE1DotDiff + nxE1DotF0,
				 nxE1DotDiff + nxE1DotF1, min1, max1 );
	return TestIntervals( -nDotN, 0.0f, min1, max1, nxE1DotVelocity );
}

/*
================
NiTriIntersect::TestNxE2
================
*/
// FUNCTION: STBC 0x81B9D0
bool NiTriIntersect::TestNxE2() {
	nxE2DotF0 = nxE1DotF0 - nxE0DotF0;
	nxE2DotF1 = nxE1DotF1 - nxE0DotF1;
	nxE2DotDiff = nxE1DotDiff - nxE0DotDiff;
	nxE2DotVelocity = nxE1DotVelocity - nxE0DotVelocity;

	float min1, max1;
	Sort( nxE2DotDiff, nxE2DotDiff + nxE2DotF0,
				 nxE2DotDiff + nxE2DotF1, min1, max1 );
	return TestIntervals( -nDotN, 0.0f, min1, max1, nxE2DotVelocity );
}

/*
================
NiTriIntersect::FindN
================
*/
// FUNCTION: STBC 0x81BAD0
// Gamebryo 1.2.2: NiTriIntersect::FindN
bool NiTriIntersect::FindN() {
	n = e0.Cross( e1 );
	nDotF0 = n.Dot( f0 );
	nDotF1 = n.Dot( f1 );
	nDotDiff = n.Dot( diff );
	nDotVelocity = n.Dot( velocity );

	float projection0_[3] = { 0.0f, 0.0f, 0.0f };
	int order0_[3] = { 0, 1, 2 };
	float projection1_[3];
	int order1_[3];
	Sort( nDotDiff, nDotDiff + nDotF0, nDotDiff + nDotF1, projection1_, order1_ );

	return FindIntervals( AXIS_N, projection0_, order0_, projection1_, order1_, nDotVelocity );
}

/*
================
NiTriIntersect::FindM
================
*/
// FUNCTION: STBC 0x81BC80
// Gamebryo 1.2.2: NiTriIntersect::FindM
bool NiTriIntersect::FindM() {
	m = f0.Cross( f1 );
	mDotE0 = m.Dot( e0 );
	mDotE1 = m.Dot( e1 );
	mDotDiff = m.Dot( diff );
	mDotVelocity = m.Dot( velocity );

	float projection1_[3] = { mDotDiff, mDotDiff, mDotDiff };
	int order1_[3] = { 0, 1, 2 };
	float projection0_[3];
	int order0_[3];
	Sort( 0.0f, mDotE0, mDotE1, projection0_, order0_ );

	return FindIntervals( AXIS_M, projection0_, order0_, projection1_, order1_, mDotVelocity );
}

/*
================
NiTriIntersect::FindE0xF0
================
*/
// FUNCTION: STBC 0x81BE20
bool NiTriIntersect::FindE0xF0() {
	e0xF0 = e0.Cross( f0 );
	e0xF0DotDiff = e0xF0.Dot( diff );
	e0xF0DotVelocity = e0xF0.Dot( velocity );

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, 0.0f, -nDotF0, projection0_, order0_ );
	Sort( e0xF0DotDiff, e0xF0DotDiff, e0xF0DotDiff + mDotE0, projection1_, order1_ );
	return FindIntervals( AXIS_E0XF0, projection0_, order0_, projection1_, order1_, e0xF0DotVelocity );
}

/*
================
NiTriIntersect::FindE0xF1
================
*/
// FUNCTION: STBC 0x81BF40
bool NiTriIntersect::FindE0xF1() {
	e0xF1 = e0.Cross( f1 );
	e0xF1DotDiff = e0xF1.Dot( diff );
	e0xF1DotVelocity = e0xF1.Dot( velocity );

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, 0.0f, -nDotF1, projection0_, order0_ );
	Sort( e0xF1DotDiff, e0xF1DotDiff - mDotE0, e0xF1DotDiff, projection1_, order1_ );
	return FindIntervals( AXIS_E0XF1, projection0_, order0_, projection1_, order1_, e0xF1DotVelocity );
}

/*
================
NiTriIntersect::FindE0xF2
================
*/
// FUNCTION: STBC 0x81C060
bool NiTriIntersect::FindE0xF2() {
	e0xF2DotDiff = e0xF1DotDiff - e0xF0DotDiff;
	e0xF2DotVelocity = e0xF1DotVelocity - e0xF0DotVelocity;
	nDotF2 = nDotF1 - nDotF0;

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, 0.0f, -nDotF2, projection0_, order0_ );
	float dist = e0xF2DotDiff;
	Sort( dist, dist - mDotE0, dist - mDotE0, projection1_, order1_ );
	return FindIntervals( AXIS_E0XF2, projection0_, order0_, projection1_, order1_, e0xF2DotVelocity );
}

/*
================
NiTriIntersect::FindE1xF0
================
*/
// FUNCTION: STBC 0x81C110
bool NiTriIntersect::FindE1xF0() {
	e1xF0 = e1.Cross( f0 );
	e1xF0DotDiff = e1xF0.Dot( diff );
	e1xF0DotVelocity = e1xF0.Dot( velocity );

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, nDotF0, 0.0f, projection0_, order0_ );
	Sort( e1xF0DotDiff, e1xF0DotDiff, e1xF0DotDiff + mDotE1, projection1_, order1_ );
	return FindIntervals( AXIS_E1XF0, projection0_, order0_, projection1_, order1_, e1xF0DotVelocity );
}

/*
================
NiTriIntersect::FindE1xF1
================
*/
// FUNCTION: STBC 0x81C230
bool NiTriIntersect::FindE1xF1() {
	e1xF1 = e1.Cross( f1 );
	e1xF1DotDiff = e1xF1.Dot( diff );
	e1xF1DotVelocity = e1xF1.Dot( velocity );

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, nDotF1, 0.0f, projection0_, order0_ );
	Sort( e1xF1DotDiff, e1xF1DotDiff - mDotE1, e1xF1DotDiff, projection1_, order1_ );
	return FindIntervals( AXIS_E1XF1, projection0_, order0_, projection1_, order1_, e1xF1DotVelocity );
}

/*
================
NiTriIntersect::FindE1xF2
================
*/
// FUNCTION: STBC 0x81C350
bool NiTriIntersect::FindE1xF2() {
	e1xF2DotDiff = e1xF1DotDiff - e1xF0DotDiff;
	e1xF2DotVelocity = e1xF1DotVelocity - e1xF0DotVelocity;

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, nDotF2, 0.0f, projection0_, order0_ );
	float dist = e1xF2DotDiff;
	Sort( dist, dist - mDotE1, dist - mDotE1, projection1_, order1_ );
	return FindIntervals( AXIS_E1XF2, projection0_, order0_, projection1_, order1_, e1xF2DotVelocity );
}

/*
================
NiTriIntersect::FindE2xF0
================
*/
// FUNCTION: STBC 0x81C400
bool NiTriIntersect::FindE2xF0() {
	e2xF0DotDiff = e1xF0DotDiff - e0xF0DotDiff;
	e2xF0DotVelocity = e1xF0DotVelocity - e0xF0DotVelocity;
	mDotE2 = mDotE1 - mDotE0;

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, nDotF0, nDotF0, projection0_, order0_ );
	Sort( e2xF0DotDiff, e2xF0DotDiff, e2xF0DotDiff + mDotE2, projection1_, order1_ );
	return FindIntervals( AXIS_E2XF0, projection0_, order0_, projection1_, order1_, e2xF0DotVelocity );
}

/*
================
NiTriIntersect::FindE2xF1
================
*/
// FUNCTION: STBC 0x81C4B0
bool NiTriIntersect::FindE2xF1() {
	e2xF1DotDiff = e1xF1DotDiff - e0xF1DotDiff;
	e2xF1DotVelocity = e1xF1DotVelocity - e0xF1DotVelocity;

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, nDotF1, nDotF1, projection0_, order0_ );
	Sort( e2xF1DotDiff, e2xF1DotDiff - mDotE2, e2xF1DotDiff, projection1_, order1_ );
	return FindIntervals( AXIS_E2XF1, projection0_, order0_, projection1_, order1_, e2xF1DotVelocity );
}

/*
================
NiTriIntersect::FindE2xF2
================
*/
// FUNCTION: STBC 0x81C550
bool NiTriIntersect::FindE2xF2() {
	e2xF2DotDiff = e2xF1DotDiff - e2xF0DotDiff;
	e2xF2DotVelocity = e2xF1DotVelocity - e2xF0DotVelocity;

	float projection0_[3], projection1_[3];
	int order0_[3], order1_[3];
	Sort( 0.0f, nDotF2, nDotF2, projection0_, order0_ );
	float dist = e2xF2DotDiff;
	Sort( dist, dist - mDotE2, dist - mDotE2, projection1_, order1_ );
	return FindIntervals( AXIS_E2XF2, projection0_, order0_, projection1_, order1_, e2xF2DotVelocity );
}

/*
================
NiTriIntersect::FindMxF0
================
*/
// FUNCTION: STBC 0x81C600
// Gamebryo 1.2.2: NiTriIntersect::FindMxF0
bool NiTriIntersect::FindMxF0() {
	mDotM = m.Dot( m );
	mxF0 = m.Cross( f0 );
	mxF0DotE0 = mxF0.Dot( e0 );
	mxF0DotE1 = mxF0.Dot( e1 );
	mxF0DotDiff = mxF0.Dot( diff );
	mxF0DotVelocity = mxF0.Dot( velocity );

	float projection1_[3] = { mxF0DotDiff, mxF0DotDiff, mxF0DotDiff + mDotM };
	int order1_[3] = { 0, 1, 2 };
	float projection0_[3];
	int order0_[3];
	Sort( 0.0f, mxF0DotE0, mxF0DotE1, projection0_, order0_ );

	return FindIntervals( AXIS_MXF0, projection0_, order0_, projection1_, order1_, mxF0DotVelocity );
}

/*
================
NiTriIntersect::FindMxF1
================
*/
// FUNCTION: STBC 0x81C7D0
// Gamebryo 1.2.2: NiTriIntersect::FindMxF1
bool NiTriIntersect::FindMxF1() {
	mxF1 = m.Cross( f1 );
	mxF1DotE0 = mxF1.Dot( e0 );
	mxF1DotE1 = mxF1.Dot( e1 );
	mxF1DotDiff = mxF1.Dot( diff );
	mxF1DotVelocity = mxF1.Dot( velocity );

	float projection1_[3] = { mxF1DotDiff - mDotM, mxF1DotDiff, mxF1DotDiff };
	int order1_[3] = { 1, 0, 2 };
	float projection0_[3];
	int order0_[3];
	Sort( 0.0f, mxF1DotE0, mxF1DotE1, projection0_, order0_ );

	return FindIntervals( AXIS_MXF1, projection0_, order0_, projection1_, order1_, mxF1DotVelocity );
}

/*
================
NiTriIntersect::FindMxF2
================
*/
// FUNCTION: STBC 0x81C980
bool NiTriIntersect::FindMxF2() {
	mxF2DotE0 = mxF1DotE0 - mxF0DotE0;
	mxF2DotE1 = mxF1DotE1 - mxF0DotE1;
	mxF2DotDiff = mxF1DotDiff - mxF0DotDiff;
	mxF2DotVelocity = mxF1DotVelocity - mxF0DotVelocity;

	float q1 = mxF2DotDiff - mDotM;
	float projection1_[3] = { q1, q1, mxF2DotDiff };
	int order1_[3] = { 1, 2, 0 };
	float projection0_[3];
	int order0_[3];
	Sort( 0.0f, mxF2DotE0, mxF2DotE1, projection0_, order0_ );

	return FindIntervals( AXIS_MXF2, projection0_, order0_, projection1_, order1_, mxF2DotVelocity );
}

/*
================
NiTriIntersect::FindNxE0
================
*/
// FUNCTION: STBC 0x81CA50
// Gamebryo 1.2.2: NiTriIntersect::FindNxE0
bool NiTriIntersect::FindNxE0() {
	nDotN = n.Dot( n );
	nxE0 = n.Cross( e0 );
	nxE0DotF0 = nxE0.Dot( f0 );
	nxE0DotF1 = nxE0.Dot( f1 );
	nxE0DotDiff = nxE0.Dot( diff );
	nxE0DotVelocity = nxE0.Dot( velocity );

	float projection0_[3] = { 0.0f, 0.0f, nDotN };
	int order0_[3] = { 0, 1, 2 };
	float projection1_[3];
	int order1_[3];
	Sort( nxE0DotDiff, nxE0DotDiff + nxE0DotF0, nxE0DotDiff + nxE0DotF1, projection1_, order1_ );

	return FindIntervals( AXIS_NXE0, projection0_, order0_, projection1_, order1_, nxE0DotVelocity );
}

/*
================
NiTriIntersect::FindNxE1
================
*/
// FUNCTION: STBC 0x81CC40
// Gamebryo 1.2.2: NiTriIntersect::FindNxE1
bool NiTriIntersect::FindNxE1() {
	nxE1 = n.Cross( e1 );
	nxE1DotF0 = nxE1.Dot( f0 );
	nxE1DotF1 = nxE1.Dot( f1 );
	nxE1DotDiff = nxE1.Dot( diff );
	nxE1DotVelocity = nxE1.Dot( velocity );

	float projection0_[3] = { -nDotN, 0.0f, 0.0f };
	int order0_[3] = { 1, 0, 2 };
	float projection1_[3];
	int order1_[3];
	Sort( nxE1DotDiff, nxE1DotDiff + nxE1DotF0, nxE1DotDiff + nxE1DotF1, projection1_, order1_ );

	return FindIntervals( AXIS_NXE1, projection0_, order0_, projection1_, order1_, nxE1DotVelocity );
}

/*
================
NiTriIntersect::FindNxE2
================
*/
// FUNCTION: STBC 0x81CE00
// Gamebryo 1.2.2: NiTriIntersect::FindNxE2
bool NiTriIntersect::FindNxE2() {
	nxE2DotF0 = nxE1DotF0 - nxE0DotF0;
	nxE2DotF1 = nxE1DotF1 - nxE0DotF1;
	nxE2DotDiff = nxE1DotDiff - nxE0DotDiff;
	nxE2DotVelocity = nxE1DotVelocity - nxE0DotVelocity;

	float projection0_[3] = { -nDotN, -nDotN, 0.0f };
	int order0_[3] = { 1, 2, 0 };
	float projection1_[3];
	int order1_[3];
	Sort( nxE2DotDiff, nxE2DotDiff + nxE2DotF0, nxE2DotDiff + nxE2DotF1, projection1_, order1_ );

	return FindIntervals( AXIS_NXE2, projection0_, order0_, projection1_, order1_, nxE2DotVelocity );
}

/*
================
NiTriIntersect::PEdgeQEdgeIntersect

The contact of an edge of each triangle at firstTime: where the second
edge crosses the first, through the second triangle's normal plane; for
parallel edges, the end that the projections onto the first edge put
first.
================
*/
// FUNCTION: STBC 0x81CEE0
// Gamebryo 1.2.2: NiTriIntersect::PEdgeQEdgeIntersect
void NiTriIntersect::PEdgeQEdgeIntersect( const NiPoint3 &end00, const NiPoint3 &end01, const NiPoint3 &end10,
										  const NiPoint3 &end11 ) {
	// ( 1 - u ) * P0 + u * P1 = Q0 + T * W + v * ( Q1 - Q0 ): the edges meet
	// at a single point unless M . ( P1 - P0 ) is zero, when they overlap
	NiPoint3 edge = end01 - end00;
	NiPoint3 move = firstTime * velocity;
	NiPoint3 startOffset = end10 + move - end00;
	float dot = m.Dot( edge );
	if ( fabsf( dot ) > epsilon ) {
		dot = m.Dot( startOffset ) / dot;	// u, in the denominator's variable as Gamebryo 1.2.2 has it
		contactPoint = end00 + firstTime * velocity0 + dot * edge;
	} else {
		NiPoint3 endOffset = end11 + move - end01;
		float startDot = edge.Dot( startOffset );
		float endDot = edge.Dot( endOffset );
		if ( startDot <= endDot ) {
			if ( startDot >= 0.0f ) {
				contactPoint = end10 + firstTime * velocity1;
			} else {
				contactPoint = end00 + firstTime * velocity0;
			}
		} else {
			if ( endDot >= 0.0f ) {
				contactPoint = end11 + firstTime * velocity1;
			} else {
				contactPoint = end00 + firstTime * velocity0;
			}
		}
	}
}

/*
================
NiTriIntersect::PEdgeQFaceIntersect

An edge of the first triangle touches the second's face: the contact is
an end of the edge inside the face, else where the edge crosses one of the
face's edges at its first vertex, else that vertex. In the face's plane,
P - Q = r F0 + s F1, so |M|^2 r = -[M, F1, P - Q] and |M|^2 s = [M, F0, P - Q].
================
*/
// FUNCTION: STBC 0x81D240
// Gamebryo 1.2.2: NiTriIntersect::PEdgeQFaceIntersect
void NiTriIntersect::PEdgeQFaceIntersect( const NiPoint3 &end0, const NiPoint3 &end1 ) {
	NiPoint3 cross;
	float r, s;
	NiPoint3 delta = *q[0] + firstTime * velocity;

	// the first end inside the face
	NiPoint3 diff0 = end0 - delta;
	cross = f1.Cross( diff0 );
	r = -m.Dot( cross );
	if ( 0.0f <= r ) {
		cross = f0.Cross( diff0 );
		s = m.Dot( cross );
		if ( 0.0f <= s && r + s <= mDotM ) {
			contactPoint = end0 + firstTime * velocity0;
			return;
		}
	}

	// the second end inside the face
	NiPoint3 diff1 = end1 - delta;
	cross = f1.Cross( diff1 );
	r = -m.Dot( cross );
	if ( 0.0f <= r ) {
		cross = f0.Cross( diff1 );
		s = m.Dot( cross );
		if ( 0.0f <= s && r + s <= mDotM ) {
			contactPoint = end1 + firstTime * velocity0;
			return;
		}
	}

	// the crossing on the face's edge from its vertex 0 to vertex 1
	if ( nDotF0 > epsilon ) {
		r = n.Dot( diff0 );
		if ( 0.0f <= r && r <= nDotF0 ) {
			r /= nDotF0;
			contactPoint = *q[0] + firstTime * velocity1 + r * f0;
			return;
		}
	} else if ( nDotF0 < -epsilon ) {
		r = n.Dot( diff0 );
		if ( nDotF0 <= r && r <= 0.0f ) {
			r /= nDotF0;
			contactPoint = *q[0] + firstTime * velocity1 + r * f0;
			return;
		}
	} else if ( fabsf( m.Dot( diff0 ) ) <= epsilon ) {
		// parallel edges: the face's first vertex approximates the contact
		contactPoint = *q[0] + firstTime * velocity1;
	}

	// the crossing on the face's edge from its vertex 0 to vertex 2
	if ( nDotF1 > epsilon ) {
		s = n.Dot( diff0 );
		if ( 0.0f <= s && s <= nDotF1 ) {
			s /= nDotF1;
			contactPoint = *q[0] + firstTime * velocity1 + s * f1;
			return;
		}
	} else if ( nDotF1 < -epsilon ) {
		s = n.Dot( diff0 );
		if ( nDotF1 <= s && s <= 0.0f ) {
			s /= nDotF1;
			contactPoint = *q[0] + firstTime * velocity1 + s * f1;
			return;
		}
	} else if ( fabsf( m.Dot( diff0 ) ) <= epsilon ) {
		contactPoint = *q[0] + firstTime * velocity1;
	}
}

/*
================
NiTriIntersect::PFaceQEdgeIntersect

PEdgeQFaceIntersect with the triangles' roles exchanged: an edge of the
second triangle touches the first's face.
================
*/
// FUNCTION: STBC 0x81DA90
// Gamebryo 1.2.2: NiTriIntersect::PFaceQEdgeIntersect
void NiTriIntersect::PFaceQEdgeIntersect( const NiPoint3 &end0, const NiPoint3 &end1 ) {
	NiPoint3 cross;
	float r, s;
	NiPoint3 delta = firstTime * velocity - *p[0];

	// the first end inside the face
	NiPoint3 diff0 = end0 + delta;
	cross = e1.Cross( diff0 );
	r = -n.Dot( cross );
	if ( 0.0f <= r ) {
		cross = e0.Cross( diff0 );
		s = n.Dot( cross );
		if ( 0.0f <= s && r + s <= nDotN ) {
			contactPoint = end0 + firstTime * velocity1;
			return;
		}
	}

	// the second end inside the face
	NiPoint3 diff1 = end1 + delta;
	cross = e1.Cross( diff1 );
	r = -n.Dot( cross );
	if ( 0.0f <= r ) {
		cross = e0.Cross( diff1 );
		s = n.Dot( cross );
		if ( 0.0f <= s && r + s <= nDotN ) {
			contactPoint = end1 + firstTime * velocity1;
			return;
		}
	}

	// the crossing on the face's edge from its vertex 0 to vertex 1
	if ( mDotE0 > epsilon ) {
		r = m.Dot( diff0 );
		if ( 0.0f <= r && r <= mDotE0 ) {
			r /= mDotE0;
			contactPoint = *p[0] + firstTime * velocity0 + r * e0;
			return;
		}
	} else if ( mDotE0 < -epsilon ) {
		r = m.Dot( diff0 );
		if ( mDotE0 <= r && r <= 0.0f ) {
			r /= mDotE0;
			contactPoint = *p[0] + firstTime * velocity0 + r * e0;
			return;
		}
	} else if ( fabsf( n.Dot( diff0 ) ) <= epsilon ) {
		// parallel edges: the face's first vertex approximates the contact
		contactPoint = *p[0] + firstTime * velocity0;
	}

	// the crossing on the face's edge from its vertex 0 to vertex 2
	if ( mDotE1 > epsilon ) {
		s = m.Dot( diff0 );
		if ( 0.0f <= s && s <= mDotE1 ) {
			s /= mDotE1;
			contactPoint = *p[0] + firstTime * velocity0 + s * e1;
			return;
		}
	} else if ( mDotE1 < -epsilon ) {
		s = m.Dot( diff0 );
		if ( mDotE1 <= s && s <= 0.0f ) {
			s /= mDotE1;
			contactPoint = *p[0] + firstTime * velocity0 + s * e1;
			return;
		}
	} else if ( fabsf( n.Dot( diff0 ) ) <= epsilon ) {
		contactPoint = *p[0] + firstTime * velocity0;
	}
}

/*
================
NiTriIntersect::PFaceQFaceIntersect

The faces touch: the contact is a vertex of the second triangle inside the
first, else where an edge of the second crosses an edge of the first at
the first vertex, else the first triangle's first vertex. In the first
triangle's plane, |N|^2 r = a0 + b00 u + b01 v and |N|^2 s = a1 + b10 u +
b11 v for the second triangle's point Q0 + u F0 + v F1.

BUG: the collinear cases of the edges against <P0, P2> move along E0
rather than E1, and the collinear case of <Q1, Q2> against <P0, P1> sets
the contact without returning, so the tests after it can replace it.
================
*/
// FUNCTION: STBC 0x81E2D0
// Gamebryo 1.2.2: NiTriIntersect::PFaceQFaceIntersect
void NiTriIntersect::PFaceQFaceIntersect() {
	float r, s;

	// (u, v) = (0, 0): the second triangle's vertex 0 inside the first
	float a0 = -( nxE1DotDiff + firstTime * nxE1DotVelocity );
	float a1 = nxE0DotDiff + firstTime * nxE0DotVelocity;
	r = a0;
	s = a1;
	if ( r >= 0.0f ) {
		s = a1;
		if ( s >= 0.0f && r + s <= nDotN ) {
			contactPoint = *q[0] + firstTime * velocity1;
			return;
		}
	}

	// (u, v) = (1, 0): its vertex 1
	float b00 = -nxE1DotF0;
	float b10 = +nxE0DotF0;
	r = a0 + b00;
	if ( r >= 0.0f ) {
		s = a1 + b10;
		if ( s >= 0.0f && r + s <= nDotN ) {
			contactPoint = *q[1] + firstTime * velocity1;
			return;
		}
	}

	// (u, v) = (0, 1): its vertex 2
	float b01 = -nxE1DotF1;
	float b11 = +nxE0DotF1;
	r = a0 + b01;
	if ( r >= 0.0f ) {
		s = a1 + b11;
		if ( s >= 0.0f && r + s <= nDotN ) {
			contactPoint = *q[2] + firstTime * velocity1;
			return;
		}
	}

	float det, bound;

	// v = 0, s = 0: <Q0, Q1> against <P0, P1>
	if ( b10 > 0.0f ) {
		det = b10 * a0 - b00 * a1;
		if ( det >= 0.0f ) {
			bound = b10 * nDotN;
			if ( det <= bound ) {
				r = det / bound;
				contactPoint = *p[0] + r * e0 + firstTime * velocity0;
				return;
			}
		}
	} else if ( b10 < 0.0f ) {
		det = b10 * a0 - b00 * a1;
		if ( det <= 0.0f ) {
			bound = b10 * nDotN;
			if ( bound <= det ) {
				r = det / bound;
				contactPoint = *p[0] + r * e0 + firstTime * velocity0;
				return;
			}
		}
	} else if ( fabsf( a1 ) <= epsilon ) {
		if ( b00 > 0.0f ) {
			r = ( a0 >= 0.0f ? a0 : a0 + b00 );
		} else if ( b00 < 0.0f ) {
			r = ( a0 <= nDotN ? a0 : a0 + b00 );
		} else {
			r = a0;
		}

		r /= nDotN;
		contactPoint = *p[0] + r * e0 + firstTime * velocity0;
		return;
	}

	// v = 0, r = 0: <Q0, Q1> against <P0, P2>
	if ( b00 > 0.0f ) {
		det = b00 * a1 - b10 * a0;
		if ( det >= 0.0f ) {
			bound = b00 * nDotN;
			if ( det <= bound ) {
				s = det / bound;
				contactPoint = *p[0] + s * e1 + firstTime * velocity0;
				return;
			}
		}
	} else if ( b00 < 0.0f ) {
		det = b00 * a1 - b10 * a0;
		if ( det <= 0.0f ) {
			bound = b00 * nDotN;
			if ( bound <= det ) {
				s = det / bound;
				contactPoint = *p[0] + s * e1 + firstTime * velocity0;
				return;
			}
		}
	} else if ( fabsf( a0 ) <= epsilon ) {
		if ( b10 > 0.0f ) {
			s = ( a1 >= 0.0f ? a1 : a1 + b10 );
		} else if ( b10 < 0.0f ) {
			s = ( a1 <= nDotN ? a1 : a1 + b10 );
		} else {
			s = a1;
		}

		s /= nDotN;
		contactPoint = *p[0] + s * e0 + firstTime * velocity0;
		return;
	}

	// u = 0, s = 0: <Q0, Q2> against <P0, P1>
	if ( b11 > 0.0f ) {
		det = b11 * a0 - b01 * a1;
		if ( det >= 0.0f ) {
			bound = b11 * nDotN;
			if ( det <= bound ) {
				r = det / bound;
				contactPoint = *p[0] + r * e0 + firstTime * velocity0;
				return;
			}
		}
	} else if ( b11 < 0.0f ) {
		det = b11 * a0 - b01 * a1;
		if ( det <= 0.0f ) {
			bound = b11 * nDotN;
			if ( bound <= det ) {
				r = det / bound;
				contactPoint = *p[0] + r * e0 + firstTime * velocity0;
				return;
			}
		}
	} else if ( fabsf( a1 ) <= epsilon ) {
		if ( b01 > 0.0f ) {
			r = ( a0 >= 0.0f ? a0 : a0 + b01 );
		} else if ( b01 < 0.0f ) {
			r = ( a0 <= nDotN ? a0 : a0 + b01 );
		} else {
			r = a0;
		}

		r /= nDotN;
		contactPoint = *p[0] + r * e0 + firstTime * velocity0;
		return;
	}

	// u = 0, r = 0: <Q0, Q2> against <P0, P2>
	if ( b01 > 0.0f ) {
		det = b01 * a1 - b11 * a0;
		if ( det >= 0.0f ) {
			bound = b01 * nDotN;
			if ( det <= bound ) {
				s = det / bound;
				contactPoint = *p[0] + s * e1 + firstTime * velocity0;
				return;
			}
		}
	} else if ( b01 < 0.0f ) {
		det = b01 * a1 - b11 * a0;
		if ( det <= 0.0f ) {
			bound = b01 * nDotN;
			if ( bound <= det ) {
				s = det / bound;
				contactPoint = *p[0] + s * e1 + firstTime * velocity0;
				return;
			}
		}
	} else if ( fabsf( a0 ) <= epsilon ) {
		if ( b11 > 0.0f ) {
			s = ( a1 >= 0.0f ? a1 : a1 + b11 );
		} else if ( b11 < 0.0f ) {
			s = ( a1 <= nDotN ? a1 : a1 + b11 );
		} else {
			s = a1;
		}

		s /= nDotN;
		contactPoint = *p[0] + s * e0 + firstTime * velocity0;
		return;
	}

	float b00MinusB01 = b00 - b01;
	float b10MinusB11 = b10 - b11;
	float a0PlusB01 = a0 + b01;
	float a1PlusB11 = a1 + b11;

	// u + v = 1, s = 0: <Q1, Q2> against <P0, P1>
	if ( b10MinusB11 > 0.0f ) {
		det = b10MinusB11 * a0PlusB01 - b00MinusB01 * a1PlusB11;
		if ( det >= 0.0f ) {
			bound = b10MinusB11 * nDotN;
			if ( det <= bound ) {
				r = det / bound;
				contactPoint = *p[0] + r * e0 + firstTime * velocity0;
				return;
			}
		}
	} else if ( b10MinusB11 < 0.0f ) {
		det = b10MinusB11 * a0PlusB01 - b00MinusB01 * a1PlusB11;
		if ( det <= 0.0f ) {
			bound = b10MinusB11 * nDotN;
			if ( bound <= det ) {
				r = det / bound;
				contactPoint = *p[0] + r * e0 + firstTime * velocity0;
				return;
			}
		}
	} else if ( fabsf( a1PlusB11 ) <= epsilon ) {
		if ( b00MinusB01 > 0.0f ) {
			r = ( a0PlusB01 >= 0.0f ? a0PlusB01 : a0PlusB01 + b00MinusB01 );
		} else if ( b00MinusB01 < 0.0f ) {
			r = ( a0PlusB01 <= nDotN ? a0PlusB01 : a0PlusB01 + b00MinusB01 );
		} else {
			r = a0PlusB01;
		}

		r /= nDotN;
		contactPoint = *p[0] + r * e0 + firstTime * velocity0;
	}

	// u + v = 1, r = 0: <Q1, Q2> against <P0, P2>
	if ( b00MinusB01 > 0.0f ) {
		det = b00MinusB01 * a1PlusB11 - b10MinusB11 * a0PlusB01;
		if ( det >= 0.0f ) {
			bound = b00MinusB01 * nDotN;
			if ( det <= bound ) {
				s = det / bound;
				contactPoint = *p[0] + s * e1 + firstTime * velocity0;
				return;
			}
		}
	} else if ( b00MinusB01 < 0.0f ) {
		det = b00MinusB01 * a1PlusB11 - b10MinusB11 * a0PlusB01;
		if ( det <= 0.0f ) {
			bound = b00MinusB01 * nDotN;
			if ( bound <= det ) {
				s = det / bound;
				contactPoint = *p[0] + s * e1 + firstTime * velocity0;
				return;
			}
		}
	} else if ( fabsf( a0PlusB01 ) <= epsilon ) {
		if ( b10MinusB11 > 0.0f ) {
			s = ( a1PlusB11 >= 0.0f ? a1PlusB11 : a1PlusB11 + b10MinusB11 );
		} else if ( b10MinusB11 < 0.0f ) {
			s = ( a1PlusB11 <= nDotN ? a1PlusB11 : a1PlusB11 + b10MinusB11 );
		} else {
			s = a1PlusB11;
		}

		s /= nDotN;
		contactPoint = *p[0] + s * e0 + firstTime * velocity0;
		return;
	}

	// the first triangle inside the second
	contactPoint = *p[0] + firstTime * velocity0;
}

/*
================
NiTriIntersect::LocateIntersection

The contact point at firstTime. With no separating axis the triangles
already meet, and the larger one's centroid stands for the contact.
Otherwise the triangles touch along the recorded axis: the sorted
projections tell which vertex, edge or face of each triangle is the
nearest, and a vertex alone is the contact, moved to firstTime.
================
*/
// FUNCTION: STBC 0x81F9D0
// Gamebryo 1.2.2: NiTriIntersect::LocateIntersection
void NiTriIntersect::LocateIntersection() {
	if ( axis == 0 ) {
		nDotN = n.Dot( n );
		mDotM = m.Dot( m );
		if ( nDotN <= mDotM ) {
			contactPoint = ( *p[0] + *p[1] + *p[2] ) * ONE_THIRD;
		} else {
			contactPoint = ( *q[0] + *q[1] + *q[2] ) * ONE_THIRD;
		}
		return;
	}

	if ( projection1[0] > projection0[2] ) {
		// the second triangle lies past the first's far side
		if ( projection0[1] < projection0[2] ) {
			contactPoint = *p[order0[2]] + firstTime * velocity0;
		} else if ( projection0[0] < projection0[1] ) {
			if ( projection1[0] < projection1[1] ) {
				contactPoint = *q[order1[0]] + firstTime * velocity1;
			} else if ( projection1[1] < projection1[2] ) {
				PEdgeQEdgeIntersect( *p[order0[1]], *p[order0[2]],
									 *q[order1[0]], *q[order1[1]] );
			} else {
				PEdgeQFaceIntersect( *p[order0[1]], *p[order0[2]] );
			}
		} else {
			if ( projection1[0] < projection1[1] ) {
				contactPoint = *q[order1[0]] + firstTime * velocity1;
			} else if ( projection1[1] < projection1[2] ) {
				PFaceQEdgeIntersect( *q[order1[0]], *q[order1[1]] );
			} else {
				PFaceQFaceIntersect();
			}
		}
	} else if ( projection1[2] < projection0[0] ) {
		// the second triangle lies before the first's near side
		if ( projection0[0] < projection0[1] ) {
			contactPoint = *p[order0[0]] + firstTime * velocity0;
		} else if ( projection0[1] < projection0[2] ) {
			if ( projection1[1] < projection1[2] ) {
				contactPoint = *q[order1[2]] + firstTime * velocity1;
			} else if ( projection1[0] < projection1[1] ) {
				PEdgeQEdgeIntersect( *p[order0[0]], *p[order0[1]],
									 *q[order1[1]], *q[order1[2]] );
			} else {
				PEdgeQFaceIntersect( *p[order0[0]], *p[order0[1]] );
			}
		} else {
			if ( projection1[1] < projection1[2] ) {
				contactPoint = *q[order1[2]] + firstTime * velocity1;
			} else if ( projection1[0] < projection1[1] ) {
				PFaceQEdgeIntersect( *q[order1[1]], *q[order1[2]] );
			} else {
				PFaceQFaceIntersect();
			}
		}
	} else {
		// the find pass leaves the triangles apart along the axis, so this
		// is never reached
		assert( 0 );
	}
}
