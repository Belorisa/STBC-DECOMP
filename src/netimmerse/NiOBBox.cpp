// NiOBBox.cpp -- oriented bounding boxes

#include "NiRTLib.h"
#include <math.h>
#include "NiOBBox.h"
#include "NiOBBRoot.h"
#include "NiAVObject.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "NiNode.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiTimeController.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0082e010 _$E195
// FUNCTION: STBC 0x0082df80 _$E17
// FUNCTION: STBC 0x0082dfa0 _$E15
// FUNCTION: STBC 0x0082dfb0 _$E23
// FUNCTION: STBC 0x0082dfd0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0082dfe0 _$E39
// FUNCTION: STBC 0x0082dff0 _$E50
// FUNCTION: STBC 0x0082e000 _$E54
// FUNCTION: STBC 0x0082e030 _$E58
// FUNCTION: STBC 0x0082e040 _$E62
// FUNCTION: STBC 0x0082e050 _$E179
// FUNCTION: STBC 0x0082e060 _$E184
// FUNCTION: STBC 0x0082e070 _$E188

// DATA: STBC 0x89A7D4
const float NiOBBox::PARALLEL_EPSILON = 1.0e-07f;

// DATA: STBC 0x89A7D8
const float NiOBBox::MAX_DISTANCE = 3.4e+38f;

/*
================
NiOBBox::Transform
================
*/
// FUNCTION: STBC 0x829770
void NiOBBox::Transform( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) {
	worldCenter = scale * ( rotate * center ) + translate;
	worldAxis[0] = rotate * axis[0];
	worldAxis[1] = rotate * axis[1];
	worldAxis[2] = rotate * axis[2];
	worldExtent[0] = scale * extent[0];
	worldExtent[1] = scale * extent[1];
	worldExtent[2] = scale * extent[2];
}

/*
================
NiOBBox::TestIntersection

The separating axis test of two boxes moving over the root's time step:
the boxes' axes and their cross products, each tested at the start and
the end of the step. Gamebryo 1.2.2 adds the axes orthogonal to the
relative motion after these; this version has none.
================
*/
// FUNCTION: STBC 0x829860
// PARKED: x87 schedule: in four of the edge-edge tests retail evaluates the other box's radius before this box's; same calls, CFG and frame
// Gamebryo 1.2.2: NiOBBox::TestIntersection
// The locals keep Gamebryo 1.2.2's names (A, B, extA, extB, rsum, C, D, V, dt): the debug
// NetImmerse 3.1's /Od frame order (AnimOnly 0x46B415) needs them.
int NiOBBox::TestIntersection( const NiOBBox &box ) {
	const NiPoint3 *A = worldAxis;
	const NiPoint3 *B = box.worldAxis;
	const float *extA = worldExtent;
	const float *extB = box.worldExtent;

	float r0, r1, rsum, u0, u1;

	// kept for the edge-edge tests: the axes' dot products and the center
	// offset along this box's axes at the start and the end of the step
	float C[3][3];
	float d0[3], d1[3];

	NiPoint3 D = box.worldCenter - worldCenter;
	NiPoint3 V = box.root->GetVelocity() - root->GetVelocity();

	float dt = root->GetDeltaTime();

	// L = A0
	C[0][0] = A[0].Dot( B[0] );
	C[0][1] = A[0].Dot( B[1] );
	C[0][2] = A[0].Dot( B[2] );
	d0[0] = A[0].Dot( D );
	d1[0] = d0[0] + dt * A[0].Dot( V );
	r0 = fabsf( extA[0] );
	r1 = fabsf( extB[0] * C[0][0] ) + fabsf( extB[1] * C[0][1] ) + fabsf( extB[2] * C[0][2] );
	rsum = r0 + r1;
	if ( ( d0[0] > rsum && d1[0] > rsum ) || ( d0[0] < -rsum && d1[0] < -rsum ) ) {
		return 0;
	}

	// L = A1
	C[1][0] = A[1].Dot( B[0] );
	C[1][1] = A[1].Dot( B[1] );
	C[1][2] = A[1].Dot( B[2] );
	d0[1] = A[1].Dot( D );
	d1[1] = d0[1] + dt * A[1].Dot( V );
	r0 = fabsf( extA[1] );
	r1 = fabsf( extB[0] * C[1][0] ) + fabsf( extB[1] * C[1][1] ) + fabsf( extB[2] * C[1][2] );
	rsum = r0 + r1;
	if ( ( d0[1] > rsum && d1[1] > rsum ) || ( d0[1] < -rsum && d1[1] < -rsum ) ) {
		return 0;
	}

	// L = A2
	C[2][0] = A[2].Dot( B[0] );
	C[2][1] = A[2].Dot( B[1] );
	C[2][2] = A[2].Dot( B[2] );
	d0[2] = A[2].Dot( D );
	d1[2] = d0[2] + dt * A[2].Dot( V );
	r0 = fabsf( extA[2] );
	r1 = fabsf( extB[0] * C[2][0] ) + fabsf( extB[1] * C[2][1] ) + fabsf( extB[2] * C[2][2] );
	rsum = r0 + r1;
	if ( ( d0[2] > rsum && d1[2] > rsum ) || ( d0[2] < -rsum && d1[2] < -rsum ) ) {
		return 0;
	}

	// L = B0
	r0 = fabsf( extA[0] * C[0][0] ) + fabsf( extA[1] * C[1][0] ) + fabsf( extA[2] * C[2][0] );
	r1 = fabsf( extB[0] );
	rsum = r0 + r1;
	u0 = B[0].Dot( D );
	u1 = u0 + dt * B[0].Dot( V );
	if ( ( u0 > rsum && u1 > rsum ) || ( u0 < -rsum && u1 < -rsum ) ) {
		return 0;
	}

	// L = B1
	r0 = fabsf( extA[0] * C[0][1] ) + fabsf( extA[1] * C[1][1] ) + fabsf( extA[2] * C[2][1] );
	r1 = fabsf( extB[1] );
	rsum = r0 + r1;
	u0 = B[1].Dot( D );
	u1 = u0 + dt * B[1].Dot( V );
	if ( ( u0 > rsum && u1 > rsum ) || ( u0 < -rsum && u1 < -rsum ) ) {
		return 0;
	}

	// L = B2
	r0 = fabsf( extA[0] * C[0][2] ) + fabsf( extA[1] * C[1][2] ) + fabsf( extA[2] * C[2][2] );
	r1 = fabsf( extB[2] );
	rsum = r0 + r1;
	u0 = B[2].Dot( D );
	u1 = u0 + dt * B[2].Dot( V );
	if ( ( u0 > rsum && u1 > rsum ) || ( u0 < -rsum && u1 < -rsum ) ) {
		return 0;
	}

	// L = A0 x B0
	r0 = fabsf( extA[1] * C[2][0] ) + fabsf( extA[2] * C[1][0] );
	r1 = fabsf( extB[1] * C[0][2] ) + fabsf( extB[2] * C[0][1] );
	rsum = r0 + r1;
	u0 = d0[2] * C[1][0] - d0[1] * C[2][0];
	if ( u0 > rsum ) {
		u1 = d1[2] * C[1][0] - d1[1] * C[2][0];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[2] * C[1][0] - d1[1] * C[2][0];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A0 x B1
	r0 = fabsf( extA[1] * C[2][1] ) + fabsf( extA[2] * C[1][1] );
	r1 = fabsf( extB[0] * C[0][2] ) + fabsf( extB[2] * C[0][0] );
	rsum = r0 + r1;
	u0 = d0[2] * C[1][1] - d0[1] * C[2][1];
	if ( u0 > rsum ) {
		u1 = d1[2] * C[1][1] - d1[1] * C[2][1];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[2] * C[1][1] - d1[1] * C[2][1];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A0 x B2
	r0 = fabsf( extA[1] * C[2][2] ) + fabsf( extA[2] * C[1][2] );
	r1 = fabsf( extB[0] * C[0][1] ) + fabsf( extB[1] * C[0][0] );
	rsum = r0 + r1;
	u0 = d0[2] * C[1][2] - d0[1] * C[2][2];
	if ( u0 > rsum ) {
		u1 = d1[2] * C[1][2] - d1[1] * C[2][2];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[2] * C[1][2] - d1[1] * C[2][2];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A1 x B0
	r0 = fabsf( extA[0] * C[2][0] ) + fabsf( extA[2] * C[0][0] );
	r1 = fabsf( extB[1] * C[1][2] ) + fabsf( extB[2] * C[1][1] );
	rsum = r0 + r1;
	u0 = d0[0] * C[2][0] - d0[2] * C[0][0];
	if ( u0 > rsum ) {
		u1 = d1[0] * C[2][0] - d1[2] * C[0][0];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[0] * C[2][0] - d1[2] * C[0][0];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A1 x B1
	r0 = fabsf( extA[0] * C[2][1] ) + fabsf( extA[2] * C[0][1] );
	r1 = fabsf( extB[0] * C[1][2] ) + fabsf( extB[2] * C[1][0] );
	rsum = r0 + r1;
	u0 = d0[0] * C[2][1] - d0[2] * C[0][1];
	if ( u0 > rsum ) {
		u1 = d1[0] * C[2][1] - d1[2] * C[0][1];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[0] * C[2][1] - d1[2] * C[0][1];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A1 x B2
	r0 = fabsf( extA[0] * C[2][2] ) + fabsf( extA[2] * C[0][2] );
	r1 = fabsf( extB[0] * C[1][1] ) + fabsf( extB[1] * C[1][0] );
	rsum = r0 + r1;
	u0 = d0[0] * C[2][2] - d0[2] * C[0][2];
	if ( u0 > rsum ) {
		u1 = d1[0] * C[2][2] - d1[2] * C[0][2];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[0] * C[2][2] - d1[2] * C[0][2];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A2 x B0
	r0 = fabsf( extA[0] * C[1][0] ) + fabsf( extA[1] * C[0][0] );
	r1 = fabsf( extB[1] * C[2][2] ) + fabsf( extB[2] * C[2][1] );
	rsum = r0 + r1;
	u0 = d0[1] * C[0][0] - d0[0] * C[1][0];
	if ( u0 > rsum ) {
		u1 = d1[1] * C[0][0] - d1[0] * C[1][0];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[1] * C[0][0] - d1[0] * C[1][0];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A2 x B1
	r0 = fabsf( extA[0] * C[1][1] ) + fabsf( extA[1] * C[0][1] );
	r1 = fabsf( extB[0] * C[2][2] ) + fabsf( extB[2] * C[2][0] );
	rsum = r0 + r1;
	u0 = d0[1] * C[0][1] - d0[0] * C[1][1];
	if ( u0 > rsum ) {
		u1 = d1[1] * C[0][1] - d1[0] * C[1][1];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[1] * C[0][1] - d1[0] * C[1][1];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	// L = A2 x B2
	r0 = fabsf( extA[0] * C[1][2] ) + fabsf( extA[1] * C[0][2] );
	r1 = fabsf( extB[0] * C[2][1] ) + fabsf( extB[1] * C[2][0] );
	rsum = r0 + r1;
	u0 = d0[1] * C[0][2] - d0[0] * C[1][2];
	if ( u0 > rsum ) {
		u1 = d1[1] * C[0][2] - d1[0] * C[1][2];
		if ( u1 > rsum ) {
			return 0;
		}
	} else if ( u0 < -rsum ) {
		u1 = d1[1] * C[0][2] - d1[0] * C[1][2];
		if ( u1 < -rsum ) {
			return 0;
		}
	}

	return 1;
}

// the axis that separates two moving boxes longest, which locates their
// contact: a box's axis or the cross product of one of each box's
enum {
	SEPARATING_NONE,
	SEPARATING_A0, SEPARATING_A1, SEPARATING_A2,
	SEPARATING_B0, SEPARATING_B1, SEPARATING_B2,
	SEPARATING_A0B0, SEPARATING_A0B1, SEPARATING_A0B2,
	SEPARATING_A1B0, SEPARATING_A1B1, SEPARATING_A1B2,
	SEPARATING_A2B0, SEPARATING_A2B1, SEPARATING_A2B2
};

// The projections u0 at the start and u1 at the end of the step lie
// outside the radius sum on the same side: the boxes are apart. Otherwise
// the time the projections close to the radius sum is a candidate for the
// first contact, kept with its axis and side when it is the latest.
#define UPDATE_TIME( AXIS ) \
	{ \
		if ( u0 > rsum ) { \
			if ( u1 > rsum ) { \
				return 0; \
			} \
			tintr = dt * ( rsum - u0 ) / ( u1 - u0 ); \
			if ( tintr > tmax ) { \
				tmax = tintr; \
				type = AXIS; \
				side = +1; \
			} \
		} else if ( u0 < -rsum ) { \
			if ( u1 < -rsum ) { \
				return 0; \
			} \
			tintr = dt * ( -rsum - u0 ) / ( u1 - u0 ); \
			if ( tintr > tmax ) { \
				tmax = tintr; \
				type = AXIS; \
				side = -1; \
			} \
		} \
	}

// the coordinate along a box axis of the box's corner nearest the other
// box: the extent, signed by the side and the axes' dot product
#define GET_COEFF_PLUS_MINUS( coeff, side, product, extent ) \
	{ \
		if ( product > 0.0f ) { \
			coeff = side * extent; \
		} else if ( product < 0.0f ) { \
			coeff = -side * extent; \
		} else { \
			coeff = 0.0f; \
		} \
	}

#define GET_COEFF_MINUS_PLUS( coeff, side, product, extent ) \
	{ \
		if ( product > 0.0f ) { \
			coeff = -side * extent; \
		} else if ( product < 0.0f ) { \
			coeff = side * extent; \
		} else { \
			coeff = 0.0f; \
		} \
	}

/*
================
NiOBBox::FindIntersection

The separating axis test of TestIntersection, keeping the axis whose
projections close last over the time step: that time is the first
contact, and the axis says which corner, edge or face of each box
touches, which gives the contact point.
================
*/
// FUNCTION: STBC 0x82A440
// PARKED: x87 order: retail sums the first axis's offset dot product z, y, x (this build x, z, y), and one edge case's contact offset dot z, x, y (this build x, z, y); same calls, CFG and jump table
// Gamebryo 1.2.2: NiOBBox::FindIntersection
// The locals keep Gamebryo 1.2.2's names (A, B, extA, extB, rsum, C, D, V, dt, type, tintr, tmax, tmp, DTW, V0, V1): the debug
// NetImmerse 3.1's /Od frame order (AnimOnly 0x46C2F5) needs them.
int NiOBBox::FindIntersection( const NiOBBox &box, float &time, NiPoint3 &point ) {
	const NiPoint3 *A = worldAxis;
	const NiPoint3 *B = box.worldAxis;
	const float *extA = worldExtent;
	const float *extB = box.worldExtent;

	float r0, r1, rsum, u0, u1;

	// kept for the edge-edge tests: the axes' dot products and the center
	// offset along this box's axes at the start and the end of the step
	float C[3][3];
	float d0[3], d1[3];

	NiPoint3 D = box.worldCenter - worldCenter;
	NiPoint3 V = box.root->GetVelocity() - root->GetVelocity();

	float dt = root->GetDeltaTime();

	// the latest time two projections close, its axis and side
	int type = SEPARATING_NONE, side = 0;
	float tintr, tmax = 0.0f;

	// L = A0
	C[0][0] = A[0].Dot( B[0] );
	C[0][1] = A[0].Dot( B[1] );
	C[0][2] = A[0].Dot( B[2] );
	u0 = d0[0] = A[0].Dot( D );
	u1 = d1[0] = d0[0] + dt * V.Dot( A[0] );
	r0 = NiAbs( extA[0] );
	r1 = NiAbs( extB[0] * C[0][0] ) + NiAbs( extB[1] * C[0][1] ) + NiAbs( extB[2] * C[0][2] );
	rsum = r0 + r1;
	UPDATE_TIME( SEPARATING_A0 );

	// L = A1
	C[1][0] = A[1].Dot( B[0] );
	C[1][1] = A[1].Dot( B[1] );
	C[1][2] = A[1].Dot( B[2] );
	d0[1] = A[1].Dot( D );
	d1[1] = d0[1] + dt * A[1].Dot( V );
	r0 = NiAbs( extA[1] );
	r1 = NiAbs( extB[0] * C[1][0] ) + NiAbs( extB[1] * C[1][1] ) + NiAbs( extB[2] * C[1][2] );
	rsum = r0 + r1;
	UPDATE_TIME( SEPARATING_A1 );	// BUG: tests A0's projections again, u0 and u1 are not updated

	// L = A2
	C[2][0] = A[2].Dot( B[0] );
	C[2][1] = A[2].Dot( B[1] );
	C[2][2] = A[2].Dot( B[2] );
	d0[2] = A[2].Dot( D );
	d1[2] = d0[2] + dt * A[2].Dot( V );
	r0 = NiAbs( extA[2] );
	r1 = NiAbs( extB[0] * C[2][0] ) + NiAbs( extB[1] * C[2][1] ) + NiAbs( extB[2] * C[2][2] );
	rsum = r0 + r1;
	UPDATE_TIME( SEPARATING_A2 );	// BUG: tests A0's projections again, u0 and u1 are not updated

	// L = B0
	r0 = NiAbs( extA[0] * C[0][0] ) + NiAbs( extA[1] * C[1][0] ) + NiAbs( extA[2] * C[2][0] );
	r1 = NiAbs( extB[0] );
	rsum = r0 + r1;
	u0 = B[0].Dot( D );
	u1 = u0 + dt * B[0].Dot( V );
	UPDATE_TIME( SEPARATING_B0 );

	// L = B1
	r0 = NiAbs( extA[0] * C[0][1] ) + NiAbs( extA[1] * C[1][1] ) + NiAbs( extA[2] * C[2][1] );
	r1 = NiAbs( extB[1] );
	rsum = r0 + r1;
	u0 = B[1].Dot( D );
	u1 = u0 + dt * B[1].Dot( V );
	UPDATE_TIME( SEPARATING_B1 );

	// L = B2
	r0 = NiAbs( extA[0] * C[0][2] ) + NiAbs( extA[1] * C[1][2] ) + NiAbs( extA[2] * C[2][2] );
	r1 = NiAbs( extB[2] );
	rsum = r0 + r1;
	u0 = B[2].Dot( D );
	u1 = u0 + dt * B[2].Dot( V );
	UPDATE_TIME( SEPARATING_B2 );

	// L = A0 x B0
	r0 = NiAbs( extA[1] * C[2][0] ) + NiAbs( extA[2] * C[1][0] );
	r1 = NiAbs( extB[1] * C[0][2] ) + NiAbs( extB[2] * C[0][1] );
	rsum = r0 + r1;
	u0 = d0[2] * C[1][0] - d0[1] * C[2][0];
	u1 = d1[2] * C[1][0] - d1[1] * C[2][0];
	UPDATE_TIME( SEPARATING_A0B0 );

	// L = A0 x B1
	r0 = NiAbs( extA[1] * C[2][1] ) + NiAbs( extA[2] * C[1][1] );
	r1 = NiAbs( extB[0] * C[0][2] ) + NiAbs( extB[2] * C[0][0] );
	rsum = r0 + r1;
	u0 = d0[2] * C[1][1] - d0[1] * C[2][1];
	u1 = d1[2] * C[1][1] - d1[1] * C[2][1];
	UPDATE_TIME( SEPARATING_A0B1 );

	// L = A0 x B2
	r0 = NiAbs( extA[1] * C[2][2] ) + NiAbs( extA[2] * C[1][2] );
	r1 = NiAbs( extB[0] * C[0][1] ) + NiAbs( extB[1] * C[0][0] );
	rsum = r0 + r1;
	u0 = d0[2] * C[1][2] - d0[1] * C[2][2];
	u1 = d1[2] * C[1][2] - d1[1] * C[2][2];
	UPDATE_TIME( SEPARATING_A0B2 );

	// L = A1 x B0
	r0 = NiAbs( extA[0] * C[2][0] ) + NiAbs( extA[2] * C[0][0] );
	r1 = NiAbs( extB[1] * C[1][2] ) + NiAbs( extB[2] * C[1][1] );
	rsum = r0 + r1;
	u0 = d0[0] * C[2][0] - d0[2] * C[0][0];
	u1 = d1[0] * C[2][0] - d1[2] * C[0][0];
	UPDATE_TIME( SEPARATING_A1B0 );

	// L = A1 x B1
	r0 = NiAbs( extA[0] * C[2][1] ) + NiAbs( extA[2] * C[0][1] );
	r1 = NiAbs( extB[0] * C[1][2] ) + NiAbs( extB[2] * C[1][0] );
	rsum = r0 + r1;
	u0 = d0[0] * C[2][1] - d0[2] * C[0][1];
	u1 = d1[0] * C[2][1] - d1[2] * C[0][1];
	UPDATE_TIME( SEPARATING_A1B1 );

	// L = A1 x B2
	r0 = NiAbs( extA[0] * C[2][2] ) + NiAbs( extA[2] * C[0][2] );
	r1 = NiAbs( extB[0] * C[1][1] ) + NiAbs( extB[1] * C[1][0] );
	rsum = r0 + r1;
	u0 = d0[0] * C[2][2] - d0[2] * C[0][2];
	u1 = d1[0] * C[2][2] - d1[2] * C[0][2];
	UPDATE_TIME( SEPARATING_A1B2 );

	// L = A2 x B0
	r0 = NiAbs( extA[0] * C[1][0] ) + NiAbs( extA[1] * C[0][0] );
	r1 = NiAbs( extB[1] * C[2][2] ) + NiAbs( extB[2] * C[2][1] );
	rsum = r0 + r1;
	u0 = d0[1] * C[0][0] - d0[0] * C[1][0];
	u1 = d1[1] * C[0][0] - d1[0] * C[1][0];
	UPDATE_TIME( SEPARATING_A2B0 );

	// L = A2 x B1
	r0 = NiAbs( extA[0] * C[1][1] ) + NiAbs( extA[1] * C[0][1] );
	r1 = NiAbs( extB[0] * C[2][2] ) + NiAbs( extB[2] * C[2][0] );
	rsum = r0 + r1;
	u0 = d0[1] * C[0][1] - d0[0] * C[1][1];
	u1 = d1[1] * C[0][1] - d1[0] * C[1][1];
	UPDATE_TIME( SEPARATING_A2B1 );

	// L = A2 x B2
	r0 = NiAbs( extA[0] * C[1][2] ) + NiAbs( extA[1] * C[0][2] );
	r1 = NiAbs( extB[0] * C[2][1] ) + NiAbs( extB[1] * C[2][0] );
	rsum = r0 + r1;
	u0 = d0[1] * C[0][2] - d0[0] * C[1][2];
	u1 = d1[1] * C[0][2] - d1[0] * C[1][2];
	UPDATE_TIME( SEPARATING_A2B2 );

	time = tmax;

	// the contact point
	const float epsilon = 1e-06f;
	int i, j;
	float x[3], y[3], ad, bd, div, tmp;	// x and y: coordinates along this box's and the other box's axes

	NiPoint3 DTW;
	const NiPoint3 &V0 = root->GetVelocity();
	const NiPoint3 &V1 = box.root->GetVelocity();

	switch ( type ) {
		case SEPARATING_A0:
		case SEPARATING_A1:
		case SEPARATING_A2: {
			// a corner of the other box on a face of this one
			i = type - SEPARATING_A0;
			for ( j = 0; j < 3; j++ ) {
				GET_COEFF_MINUS_PLUS( y[j], side, C[i][j], extB[j] );
			}
			point = box.worldCenter + time * V1 + y[0] * B[0] + y[1] * B[1] + y[2] * B[2];
			break;
		}
		case SEPARATING_B0:
		case SEPARATING_B1:
		case SEPARATING_B2: {
			// a corner of this box on a face of the other
			j = type - SEPARATING_B0;
			for ( i = 0; i < 3; i++ ) {
				GET_COEFF_PLUS_MINUS( x[i], side, C[i][j], extA[i] );
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A0B0: {
			GET_COEFF_MINUS_PLUS( x[1], side, C[2][0], extA[1] );
			GET_COEFF_PLUS_MINUS( x[2], side, C[1][0], extA[2] );
			GET_COEFF_MINUS_PLUS( y[1], side, C[0][2], extB[1] );
			GET_COEFF_PLUS_MINUS( y[2], side, C[0][1], extB[2] );
			div = 1.0f - C[0][0] * C[0][0];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[0].Dot( DTW );
				bd = B[0].Dot( DTW );
				tmp = C[1][0] * x[1] + C[2][0] * x[2] - bd;
				x[0] = ( ad + C[0][0] * tmp + C[0][1] * y[1] + C[0][2] * y[2] ) / div;
			} else {
				x[0] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A0B1: {
			GET_COEFF_MINUS_PLUS( x[1], side, C[2][1], extA[1] );
			GET_COEFF_PLUS_MINUS( x[2], side, C[1][1], extA[2] );
			GET_COEFF_PLUS_MINUS( y[0], side, C[0][2], extB[0] );
			GET_COEFF_MINUS_PLUS( y[2], side, C[0][0], extB[2] );
			div = 1.0f - C[0][1] * C[0][1];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[0].Dot( DTW );
				bd = B[1].Dot( DTW );
				tmp = C[1][1] * x[1] + C[2][1] * x[2] - bd;
				x[0] = ( ad + C[0][1] * tmp + C[0][0] * y[0] + C[0][2] * y[2] ) / div;
			} else {
				x[0] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A0B2: {
			GET_COEFF_MINUS_PLUS( x[1], side, C[2][2], extA[1] );
			GET_COEFF_PLUS_MINUS( x[2], side, C[1][2], extA[2] );
			GET_COEFF_MINUS_PLUS( y[0], side, C[0][1], extB[0] );
			GET_COEFF_PLUS_MINUS( y[1], side, C[0][0], extB[1] );
			div = 1.0f - C[0][2] * C[0][2];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[0].Dot( DTW );
				bd = B[2].Dot( DTW );
				tmp = C[1][2] * x[1] + C[2][2] * x[2] - bd;
				x[0] = ( ad + C[0][2] * tmp + C[0][0] * y[0] + C[0][1] * y[1] ) / div;
			} else {
				x[0] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A1B0: {
			GET_COEFF_PLUS_MINUS( x[0], side, C[2][0], extA[0] );
			GET_COEFF_MINUS_PLUS( x[2], side, C[0][0], extA[2] );
			GET_COEFF_MINUS_PLUS( y[1], side, C[1][2], extB[1] );
			GET_COEFF_PLUS_MINUS( y[2], side, C[1][1], extB[2] );
			div = 1.0f - C[1][0] * C[1][0];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[1].Dot( DTW );
				bd = B[0].Dot( DTW );
				tmp = C[0][0] * x[0] + C[2][0] * x[2] - bd;
				x[1] = ( ad + C[1][0] * tmp + C[1][1] * y[1] + C[1][2] * y[2] ) / div;
			} else {
				x[1] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A1B1: {
			GET_COEFF_PLUS_MINUS( x[0], side, C[2][1], extA[0] );
			GET_COEFF_MINUS_PLUS( x[2], side, C[0][1], extA[2] );
			GET_COEFF_PLUS_MINUS( y[0], side, C[1][2], extB[0] );
			GET_COEFF_MINUS_PLUS( y[2], side, C[1][0], extB[2] );
			div = 1.0f - C[1][1] * C[1][1];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[1].Dot( DTW );
				bd = B[1].Dot( DTW );
				tmp = C[0][1] * x[0] + C[2][1] * x[2] - bd;
				x[1] = ( ad + C[1][1] * tmp + C[1][0] * y[0] + C[1][2] * y[2] ) / div;
			} else {
				x[1] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A1B2: {
			GET_COEFF_PLUS_MINUS( x[0], side, C[2][2], extA[0] );
			GET_COEFF_MINUS_PLUS( x[2], side, C[0][2], extA[2] );
			GET_COEFF_MINUS_PLUS( y[0], side, C[1][1], extB[0] );
			GET_COEFF_PLUS_MINUS( y[1], side, C[1][0], extB[1] );
			div = 1.0f - C[1][2] * C[1][2];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[1].Dot( DTW );
				bd = B[2].Dot( DTW );
				tmp = C[0][2] * x[0] + C[2][2] * x[2] - bd;
				x[1] = ( ad + C[1][2] * tmp + C[1][0] * y[0] + C[1][1] * y[1] ) / div;
			} else {
				x[1] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A2B0: {
			GET_COEFF_MINUS_PLUS( x[0], side, C[1][0], extA[0] );
			GET_COEFF_PLUS_MINUS( x[1], side, C[0][0], extA[1] );
			GET_COEFF_MINUS_PLUS( y[1], side, C[2][2], extB[1] );
			GET_COEFF_PLUS_MINUS( y[2], side, C[2][1], extB[2] );
			div = 1.0f - C[2][0] * C[2][0];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[2].Dot( DTW );
				bd = B[0].Dot( DTW );
				tmp = C[0][0] * x[0] + C[1][0] * x[1] - bd;
				x[2] = ( ad + C[2][0] * tmp + C[2][1] * y[1] + C[2][2] * y[2] ) / div;
			} else {
				x[2] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A2B1: {
			GET_COEFF_MINUS_PLUS( x[0], side, C[1][1], extA[0] );
			GET_COEFF_PLUS_MINUS( x[1], side, C[0][1], extA[1] );
			GET_COEFF_PLUS_MINUS( y[0], side, C[2][2], extB[0] );
			GET_COEFF_MINUS_PLUS( y[2], side, C[2][0], extB[2] );
			div = 1.0f - C[2][1] * C[2][1];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[2].Dot( DTW );
				bd = B[1].Dot( DTW );
				tmp = C[0][1] * x[0] + C[1][1] * x[1] - bd;
				x[2] = ( ad + C[2][1] * tmp + C[2][0] * y[0] + C[2][2] * y[2] ) / div;
			} else {
				x[2] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
		case SEPARATING_A2B2: {
			GET_COEFF_MINUS_PLUS( x[0], side, C[1][2], extA[0] );
			GET_COEFF_PLUS_MINUS( x[1], side, C[0][2], extA[1] );
			GET_COEFF_MINUS_PLUS( y[0], side, C[2][1], extB[0] );
			GET_COEFF_PLUS_MINUS( y[1], side, C[2][0], extB[1] );
			div = 1.0f - C[2][2] * C[2][2];
			if ( NiAbs( div ) > epsilon ) {
				DTW = D + time * V;
				ad = A[2].Dot( DTW );
				bd = B[2].Dot( DTW );
				tmp = C[0][2] * x[0] + C[1][2] * x[1] - bd;
				x[2] = ( ad + C[2][2] * tmp + C[2][0] * y[0] + C[2][1] * y[1] ) / div;
			} else {
				x[2] = 0.0f;
			}
			point = worldCenter + time * V0 + x[0] * A[0] + x[1] * A[1] + x[2] * A[2];
			break;
		}
	}

	return 1;
}

/*
================
NiOBBox::GetNormal

A box that is thin along an axis has that axis as its normal everywhere.
Otherwise the normal is the gradient of the box's ellipsoid at the point;
it is left in the box's axis coordinates and never rotated back into world
space (the original's quirk, kept).
================
*/
// FUNCTION: STBC 0x82DA30
void NiOBBox::GetNormal( const NiPoint3 &point, NiPoint3 &normal ) const {
	NiPoint3 shape( worldExtent[0], worldExtent[1], worldExtent[2] );
	shape.Unitize();
	if ( shape.x <= 1.0e-04f ) {
		normal = worldAxis[0];
		return;
	}
	if ( shape.y <= 1.0e-04f ) {
		normal = worldAxis[1];
		return;
	}
	if ( shape.z <= 1.0e-04f ) {
		normal = worldAxis[2];
		return;
	}

	NiPoint3 diff = point - worldCenter;
	normal.x = diff.Dot( worldAxis[0] ) / ( worldExtent[0] * worldExtent[0] );
	normal.y = diff.Dot( worldAxis[1] ) / ( worldExtent[1] * worldExtent[1] );
	normal.z = diff.Dot( worldAxis[2] ) / ( worldExtent[2] * worldExtent[2] );
	normal.Unitize();
}

/*
================
NiOBBox::TestRay

Narrows the ray's parameter interval by each pair of face planes.
================
*/
// FUNCTION: STBC 0x82DBF0
// Gamebryo 1.2.2: NiOBBox::TestIntersection (ray)
int NiOBBox::TestRay( const NiPoint3 &origin, const NiPoint3 &direction ) {
	float tMin = -MAX_DISTANCE;
	float tMax = MAX_DISTANCE;
	float absExtent[3];
	absExtent[0] = NiAbs( worldExtent[0] );
	absExtent[1] = NiAbs( worldExtent[1] );
	absExtent[2] = NiAbs( worldExtent[2] );
	NiPoint3 diff = worldCenter - origin;
	const NiPoint3 *axes = worldAxis;

	for ( int i = 0; i < 3; i++ ) {
		float e = axes[i].Dot( diff );
		float f = axes[i].Dot( direction );
		if ( NiAbs( f ) > PARALLEL_EPSILON ) {
			float invF = 1.0f / f;
			float t1 = ( e + absExtent[i] ) * invF;
			float t2 = ( e - absExtent[i] ) * invF;
			if ( t1 > t2 ) {
				float swap = t1;
				t1 = t2;
				t2 = swap;
			}
			if ( t1 > tMin ) {
				tMin = t1;
			}
			if ( t2 < tMax ) {
				tMax = t2;
			}
			if ( tMin > tMax || tMax < 0.0f ) {
				return 0;
			}
		} else if ( NiAbs( e ) > absExtent[i] ) {
			return 0;
		}
	}
	return 1;
}

/*
================
NiOBBox::FindRayIntersection

The slab test of TestRay, keeping the axes of the planes that narrow the
interval. A ray that starts inside the box leaves it through the far face.
================
*/
// FUNCTION: STBC 0x82DD60
// Gamebryo 1.2.2: NiOBBox::FindIntersection (ray)
int NiOBBox::FindRayIntersection( const NiPoint3 &origin, const NiPoint3 &direction, float &distance,
								  NiPoint3 &normal ) {
	float tMin = -MAX_DISTANCE;
	float tMax = MAX_DISTANCE;
	float absExtent[3];
	absExtent[0] = NiAbs( worldExtent[0] );
	absExtent[1] = NiAbs( worldExtent[1] );
	absExtent[2] = NiAbs( worldExtent[2] );
	NiPoint3 diff = worldCenter - origin;
	const NiPoint3 *axes = worldAxis;
	int minAxis = -1;
	int maxAxis = -1;

	for ( int i = 0; i < 3; i++ ) {
		float e = axes[i].Dot( diff );
		float f = axes[i].Dot( direction );
		if ( NiAbs( f ) > PARALLEL_EPSILON ) {
			float invF = 1.0f / f;
			float t1 = ( e + absExtent[i] ) * invF;
			float t2 = ( e - absExtent[i] ) * invF;
			if ( t1 > t2 ) {
				float swap = t1;
				t1 = t2;
				t2 = swap;
			}
			if ( t1 > tMin ) {
				tMin = t1;
				minAxis = i;
			}
			if ( t2 < tMax ) {
				tMax = t2;
				maxAxis = i;
			}
			if ( tMin > tMax || tMax < 0.0f ) {
				return 0;
			}
		} else if ( NiAbs( e ) > absExtent[i] ) {
			return 0;
		}
	}

	if ( tMin > 0.0f ) {
		distance = tMin;
		normal = axes[minAxis];
	} else {
		distance = tMax;
		if ( maxAxis == -1 ) {
			return 0;
		}
		normal = axes[maxAxis];
	}
	if ( direction.Dot( normal ) > 0.0f ) {
		normal = -normal;
	}
	return 1;
}
