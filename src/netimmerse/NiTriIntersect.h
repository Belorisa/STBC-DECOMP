/*
===============================================================================

	NiTriIntersect.h

	The collision test between two moving triangles: a separating axis
	test over the time step, in the manner of Eberly's moving triangle
	tests. The candidate axes are the two normals, the cross products of the
	edges and, for parallel triangles, the normals crossed with the edges.
	Each axis keeps the projections it needs in the object, so the find
	pass can recover the contact from the last axis that separated the
	triangles longest. The class and method names are Gamebryo 1.1.2's
	(NiTriIntersect.h: N and M are the normals of the first and second
	triangle, Ei and Fj their edges, so E0xF1 is the cross product of the
	first triangle's edge 0 with the second's edge 1); BSim pairs each
	method with its Gamebryo counterpart in declaration order. The members
	follow Gamebryo 1.2.2's notation and layout (NiTriIntersect.h: D is the
	offset of the second triangle, W the relative velocity, so nxE0DotF1 is
	(N x E0).F1), as separate members rather than arrays: the constructor
	then builds no vector constructor iterator, as retail.

	Evidence: NiTriBasedGeom's triangle-pair tests (0x7F00B0, 0x7F01B0)
	build one on the stack (0x240 bytes) for each pair of world triangles
	with the two objects' velocities, then call the test (0x8194B0) or the
	find (0x819990) with the time step; the constructor (0x819310) keeps the
	vertex and velocity pointers and computes the triangles' edges, the
	relative velocity and the offset of the second triangle. The axes'
	tests are separate functions; the test and the find expand the first
	few and call the rest.

===============================================================================
*/

#ifndef __NITRIINTERSECT_H__
#define __NITRIINTERSECT_H__

#include "NiPoint3.h"

class NiTriIntersect {
public:
	// each triangle by its three vertices (DamageTool.exe's constructor:
	// ??0NiTriIntersect@@QAE@QAPAVNiPoint3@@ABV1@01@Z)
							NiTriIntersect( NiPoint3 *p_[3], const NiPoint3 &velocity0_, NiPoint3 *q_[3],
											const NiPoint3 &velocity1_ );

	// whether the triangles meet within deltaTime
	bool					Test( float deltaTime );

	// when and where the triangles first meet within deltaTime, and their
	// normals there
	bool					Find( float deltaTime, float &time, NiPoint3 &point, bool calcNormals, NiPoint3 *normal0,
								  NiPoint3 *normal1 );

	static float			epsilon;

protected:
	// the projection extremes of one triangle onto an axis
	void					Sort( float value0, float value1, float &min, float &max );
	void					Sort( float value0, float value1, float value2, float &min, float &max );
	// the projections sorted, with the vertex each came from
	void					Sort( float value0, float value1, float value2, float sorted[3], int order[3] );

	// whether the intervals, the second moving at speed, stay apart over
	// the time step
	bool					TestIntervals( float min0, float max0, float min1, float max1, float speed );
	// the same, keeping the axis that separates the intervals longest
	bool					FindIntervals( unsigned int axis_, float projection0_[3], int order0_[3], float projection1_[3],
										   int order1_[3], float speed );

	// the separating axis tests: the normals, the edge cross products
	// and, for parallel triangles, the coplanar axes of each triangle
	bool					TestN();
	bool					TestM();
	bool					TestE0xF0();
	bool					TestE0xF1();
	bool					TestE0xF2();
	bool					TestE1xF0();
	bool					TestE1xF1();
	bool					TestE1xF2();
	bool					TestE2xF0();
	bool					TestE2xF1();
	bool					TestE2xF2();
	bool					TestMxF0();
	bool					TestMxF1();
	bool					TestMxF2();
	bool					TestNxE0();
	bool					TestNxE1();
	bool					TestNxE2();

	// the separating axes, as the find records the last one
	enum {
		AXIS_N		= 0x00001,
		AXIS_M		= 0x00002,
		AXIS_E0XF0		= 0x00004,
		AXIS_E0XF1		= 0x00008,
		AXIS_E0XF2		= 0x00010,
		AXIS_E1XF0		= 0x00020,
		AXIS_E1XF1		= 0x00040,
		AXIS_E1XF2		= 0x00080,
		AXIS_E2XF0		= 0x00100,
		AXIS_E2XF1		= 0x00200,
		AXIS_E2XF2		= 0x00400,
		AXIS_NXE0		= 0x00800,
		AXIS_NXE1		= 0x01000,
		AXIS_NXE2		= 0x02000,
		AXIS_MXF0		= 0x04000,
		AXIS_MXF1		= 0x08000,
		AXIS_MXF2		= 0x10000
	};

	// the find's versions of the axis tests: the sorted projections of
	// both triangles go to FindIntervals
	bool					FindN();
	bool					FindM();
	bool					FindE0xF0();
	bool					FindE0xF1();
	bool					FindE0xF2();
	bool					FindE1xF0();
	bool					FindE1xF1();
	bool					FindE1xF2();
	bool					FindE2xF0();
	bool					FindE2xF1();
	bool					FindE2xF2();
	bool					FindMxF0();
	bool					FindMxF1();
	bool					FindMxF2();
	bool					FindNxE0();
	bool					FindNxE1();
	bool					FindNxE2();

	// the contact point at firstTime, from the recorded axis and
	// projections
	void					LocateIntersection();
	// the contact of an edge of each triangle (0x81CEE0)
	void					PEdgeQEdgeIntersect( const NiPoint3 &end00, const NiPoint3 &end01, const NiPoint3 &end10,
												 const NiPoint3 &end11 );
	// the contact of an edge of the first triangle with the second's face
	// (0x81D240)
	void					PEdgeQFaceIntersect( const NiPoint3 &end0, const NiPoint3 &end1 );
	// the contact of an edge of the second triangle with the first's face
	// (0x81DA90)
	void					PFaceQEdgeIntersect( const NiPoint3 &end0, const NiPoint3 &end1 );
	// the contact of the two faces (0x81E2D0)
	void					PFaceQFaceIntersect();

	const NiPoint3 *		p[3];				// the first triangle's vertices (Gamebryo 1.1.2's m_apkP)
	const NiPoint3 &		velocity0;			// Gamebryo 1.1.2's NITRIINTERSECT.H: a reference member (m_rkV0)
	const NiPoint3 *		q[3];				// the second triangle's vertices (m_apkQ)
	const NiPoint3 &		velocity1;

	NiPoint3				diff;				// vertex1[0] less vertex0[0] (D)
	NiPoint3				velocity;			// velocity1 less velocity0 (W)
	NiPoint3				nxM;				// the normals' cross product
	float					nxMDotNxM;

	NiPoint3				e0xF0, e0xF1, e1xF0, e1xF1;
	float					e0xF0DotDiff, e0xF1DotDiff, e0xF2DotDiff;
	float					e1xF0DotDiff, e1xF1DotDiff, e1xF2DotDiff;
	float					e2xF0DotDiff, e2xF1DotDiff, e2xF2DotDiff;
	float					e0xF0DotVelocity, e0xF1DotVelocity, e0xF2DotVelocity;
	float					e1xF0DotVelocity, e1xF1DotVelocity, e1xF2DotVelocity;
	float					e2xF0DotVelocity, e2xF1DotVelocity, e2xF2DotVelocity;

	// the first triangle
	NiPoint3				e0, e1;				// vertex 1 and 2 less vertex 0
	NiPoint3				n;					// e0 x e1, not unit length
	NiPoint3				nxE0, nxE1, nxE2;	// the coplanar axes
	float					nDotN;
	float					nDotF0, nDotF1, nDotF2;	// against the other triangle's edges (the third is the difference)
	float					nDotDiff, nDotVelocity;
	float					nxE0DotDiff, nxE1DotDiff, nxE2DotDiff;
	float					nxE0DotVelocity, nxE1DotVelocity, nxE2DotVelocity;
	float					nxE0DotF0, nxE0DotF1, nxE0DotF2;
	float					nxE1DotF0, nxE1DotF1, nxE1DotF2;
	float					nxE2DotF0, nxE2DotF1, nxE2DotF2;

	// the second triangle
	NiPoint3				f0, f1;				// vertex 1 and 2 less vertex 0
	NiPoint3				m;					// f0 x f1, not unit length
	NiPoint3				mxF0, mxF1, mxF2;	// the coplanar axes
	float					mDotM;
	float					mDotE0, mDotE1, mDotE2;	// against the other triangle's edges (the third is the difference)
	float					mDotDiff, mDotVelocity;
	float					mxF0DotDiff, mxF1DotDiff, mxF2DotDiff;
	float					mxF0DotVelocity, mxF1DotVelocity, mxF2DotVelocity;
	float					mxF0DotE0, mxF0DotE1, mxF0DotE2;
	float					mxF1DotE0, mxF1DotE1, mxF1DotE2;
	float					mxF2DotE0, mxF2DotE1, mxF2DotE2;

	unsigned int			axis;				// the find's separating axis
	float					projection0[3];		// the find's sorted projections and their vertices
	float					projection1[3];
	int						order0[3];
	int						order1[3];
	float					deltaTime;
	float					firstTime;			// the find's latest first separation
	NiPoint3				contactPoint;		// the find's result
};

#endif /* !__NITRIINTERSECT_H__ */
