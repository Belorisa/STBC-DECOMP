// NiCamera.cpp -- the camera

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>
#include <float.h>

#include "NiCamera.h"
#include "NiNode.h"
#include "NiStream.h"
#include "NiSortedObject.h"
#include "NiPoint2.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007eae30 _$E30
// FUNCTION: STBC 0x007eae40 _$E34
// FUNCTION: STBC 0x007eae80 _$E45
// FUNCTION: STBC 0x007eaf00 _$E49
// FUNCTION: STBC 0x007eaf30 _$E53
// FUNCTION: STBC 0x007eaf40 _$E57
// FUNCTION: STBC 0x007eb000 _$E181
// FUNCTION: STBC 0x007eb190 _$E123
// FUNCTION: STBC 0x007eb1a0 _$E136
// FUNCTION: STBC 0x007eb1b0 _$E186
// FUNCTION: STBC 0x007eb1c0 _$E191
// FUNCTION: STBC 0x007eb1d0 _$E195
// FUNCTION: STBC 0x007eb1e0 _$E199
// FUNCTION: STBC 0x007eb270 _$E206

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007eb100 _$E238
// FUNCTION: STBC 0x007eadd0 _$E17
// FUNCTION: STBC 0x007eadf0 _$E15
// FUNCTION: STBC 0x007eae00 _$E23
// FUNCTION: STBC 0x007eae20 _$E21

// FUNCTION: STBC 0x007e7960 _$E230
// FUNCTION: STBC 0x007e7980 _$E227
// DATA: STBC 0x9A1908 ?rtti@NiCamera@@2VNiRTTI@@B
const NiRTTI NiCamera::rtti( "NiCamera", &NiAVObject::rtti );

// DATA: STBC 0x978564 ?minNearPlaneDist@NiCamera@@2MA
float NiCamera::minNearPlaneDist = 0.1f;

// DATA: STBC 0x978568 ?maxFarNearRatio@NiCamera@@2MA
float NiCamera::maxFarNearRatio = 10000.0f;

// the least w a projected point may have in front of the eye (Gamebryo
// 1.2.2: NiCamera::ms_fProjEps)
// DATA: STBC 0x899028 ?projEps@NiCamera@@2MB
const float NiCamera::projEps = 1e-5f;

/*
================
NiCamera::Register
================
*/
// FUNCTION: STBC 0x7E79A0 ?Register@NiCamera@@SAHXZ
NiImplementRegister( NiCamera )

/*
================
NiCamera::NiCamera

A frustum one unit wide and high at the near distance 1, reaching to 2,
over the whole screen. The six world planes are the first culling planes.
================
*/
// FUNCTION: STBC 0x7E7AF0
NiCamera::NiCamera() : screenPolygons( 5, 5 ), planes( MAX_PLANES, 3 ) {
	frustum.nearDistance = 1.0f;
	frustum.farDistance = 2.0f;
	frustum.top = 0.5f;
	frustum.bottom = -0.5f;
	frustum.left = -0.5f;
	frustum.right = 0.5f;
	port.top = 1.0f;
	port.bottom = 0.0f;
	port.left = 0.0f;
	port.right = 1.0f;

	planes.Add( &worldPlanes[NEAR_PLANE] );
	planes.Add( &worldPlanes[FAR_PLANE] );
	planes.Add( &worldPlanes[LEFT_PLANE] );
	planes.Add( &worldPlanes[RIGHT_PLANE] );
	planes.Add( &worldPlanes[TOP_PLANE] );
	planes.Add( &worldPlanes[BOTTOM_PLANE] );
	numPlanes = MAX_PLANES;

	UpdateWorldData();
	UpdateWorldBound();

	lodAdjust = 1.0f;
	activePlanesSize = MAX_PLANES;
	activePlanes = new NiPlane *[activePlanesSize];
	assert( activePlanes );
}

/*
================
NiCamera::~NiCamera
================
*/
// FUNCTION: STBC 0x7E7DB0
// PARKED: inline budget: retail inlines the smart pointer assignment in RemoveAll and calls the temporary's destructor; this build does the reverse
// FUNCTION: STBC 0x7EB200 ??_GNiCamera@@UAEPAXI@Z
NiCamera::~NiCamera() {
	planes.RemoveAll();
	for ( unsigned int i = 0; i < screenPolygons.GetSize(); i++ ) {
		screenPolygons.SetAt( i, 0 );
	}
	screenPolygons.RemoveAll();
	delete [] activePlanes;
}

/*
================
NiCamera::Clear

Has the renderer clear the camera's port; the flags pass through.
================
*/
// FUNCTION: STBC 0x7E7F70
void NiCamera::Clear( unsigned int flags ) {
	if ( renderer ) {
		renderer->ClearBuffer( &port, flags );
	}
}

/*
================
NiCamera::Click

Sets the renderer's view from the camera, then draws the scene through
the renderer's sorter, which keeps the camera's culled objects until
FinishAccumulating; a sorter that declines the camera is dropped for the
scene and restored after. The screen polygons follow, in one batch.
================
*/
// FUNCTION: STBC 0x7E7FA0
void NiCamera::Click() {
	if ( !renderer ) {
		return;
	}

	renderer->VirtualSlot41( worldLoc, worldDir, worldUp, worldRight, frustum, port );
	renderer->VirtualSlot38( this );

	if ( scene && scene->VirtualSlot26() ) {
		NiPointer<NiAccumulator> sorter = renderer->GetSorter();
		if ( sorter && !sorter->StartAccumulating( this ) ) {
			renderer->SetSorter( 0 );
		}
		scene->CullShow( this );
		if ( renderer->GetSorter() ) {
			renderer->GetSorter()->FinishAccumulating();
		}
		renderer->SetSorter( sorter );
	}

	renderer->VirtualSlot43( false );
	for ( unsigned int i = 0; i < screenPolygons.GetSize(); i++ ) {
		if ( screenPolygons.GetAt( i ) ) {
			screenPolygons.GetAt( i )->DrawBatched( renderer );
		}
	}
	renderer->VirtualSlot44();
	renderer->VirtualSlot39();
}

/*
================
NiCamera::Unknown7E81B0
================
*/
// FUNCTION: STBC 0x7E81B0
void NiCamera::Unknown7E81B0() {
	if ( renderer ) {
		renderer->VirtualSlot17();
	}
}

/*
================
NiCamera::SetNearCullingPlane

The plane at a distance along the view direction, facing away from the
eye (Gamebryo 1.2.2 names the six SetNearCullingPlane and so on, writing
the camera's plane array itself; DamageTool.exe's GetNearPlane and so on
take the plane by pointer, as here). UpdateWorldData inlines them.
================
*/
// FUNCTION: STBC 0x7E81C0
void NiCamera::SetNearCullingPlane( float distance, NiPlane *plane ) const {
	NiPoint3 point = worldLoc + distance * worldDir;
	*plane = NiPlane( worldDir, point );
}

/*
================
NiCamera::SetFarCullingPlane

The plane at a distance along the view direction, facing the eye.
================
*/
// FUNCTION: STBC 0x7E8240
void NiCamera::SetFarCullingPlane( float distance, NiPlane *plane ) const {
	NiPoint3 point = worldLoc + distance * worldDir;
	*plane = NiPlane( -worldDir, point );
}

/*
================
NiCamera::SetLeftCullingPlane

The side plane through the eye and the frustum's left edge at unit
distance, facing into the frustum.
================
*/
// FUNCTION: STBC 0x7E82E0
void NiCamera::SetLeftCullingPlane( float left, NiPlane *plane ) const {
	NiPoint3 edge = left * worldRight + worldDir;
	NiPoint3 normal = edge.Cross( worldUp );
	normal.Unitize();
	*plane = NiPlane( normal, worldLoc );
}

/*
================
NiCamera::SetRightCullingPlane
================
*/
// FUNCTION: STBC 0x7E8400
void NiCamera::SetRightCullingPlane( float right, NiPlane *plane ) const {
	NiPoint3 edge = right * worldRight + worldDir;
	NiPoint3 normal = edge.Cross( -worldUp );
	normal.Unitize();
	*plane = NiPlane( normal, worldLoc );
}

/*
================
NiCamera::SetTopCullingPlane
================
*/
// FUNCTION: STBC 0x7E8530
void NiCamera::SetTopCullingPlane( float top, NiPlane *plane ) const {
	NiPoint3 edge = top * worldUp + worldDir;
	NiPoint3 normal = edge.Cross( worldRight );
	normal.Unitize();
	*plane = NiPlane( normal, worldLoc );
}

/*
================
NiCamera::SetBottomCullingPlane
================
*/
// FUNCTION: STBC 0x7E8650
void NiCamera::SetBottomCullingPlane( float bottom, NiPlane *plane ) const {
	NiPoint3 edge = bottom * worldUp + worldDir;
	NiPoint3 normal = edge.Cross( -worldRight );
	normal.Unitize();
	*plane = NiPlane( normal, worldLoc );
}

/*
================
NiCamera::UpdateWorldData

Takes the eye and its axes from the world transform, then rebuilds the
world planes of the frustum, each facing into it.

The axes are read entry by entry: the SDK's debug NetImmerse 3.1
(AnimOnly.exe 0x426171, NiCamera.cpp) calls NiMatrix3::GetEntry nine
times here, in this order, and never GetCol.
================
*/
// FUNCTION: STBC 0x7E8780
void NiCamera::UpdateWorldData() {
	NiAVObject::UpdateWorldData();

	worldLoc = worldTranslate;
	worldDir.x = worldRotate.GetEntry( 0, 0 );
	worldDir.y = worldRotate.GetEntry( 1, 0 );
	worldDir.z = worldRotate.GetEntry( 2, 0 );
	worldUp.x = worldRotate.GetEntry( 0, 1 );
	worldUp.y = worldRotate.GetEntry( 1, 1 );
	worldUp.z = worldRotate.GetEntry( 2, 1 );
	worldRight.x = worldRotate.GetEntry( 0, 2 );
	worldRight.y = worldRotate.GetEntry( 1, 2 );
	worldRight.z = worldRotate.GetEntry( 2, 2 );

	SetNearCullingPlane( frustum.nearDistance, planes.GetAt( NEAR_PLANE ) );
	SetFarCullingPlane( frustum.farDistance, planes.GetAt( FAR_PLANE ) );
	SetLeftCullingPlane( frustum.left, planes.GetAt( LEFT_PLANE ) );
	SetRightCullingPlane( frustum.right, planes.GetAt( RIGHT_PLANE ) );
	SetTopCullingPlane( frustum.top, planes.GetAt( TOP_PLANE ) );
	SetBottomCullingPlane( frustum.bottom, planes.GetAt( BOTTOM_PLANE ) );

	WorldToCameraMatrix();
}

/*
================
NiCamera::Unknown7E8B10

Wraps a geometry object for the cluster sorter, with the culling planes
disabled now; the pool's new takes the release build's file and line.
================
*/
// FUNCTION: STBC 0x7E8B10
NiSortedObject *NiCamera::Unknown7E8B10( NiAVObject *geometry ) {
	NiSortedObject *object = new( "UNKNOWN", 0 ) NiSortedObject;
	object->geometry = geometry;
	object->disabledPlanes = disabledPlanes;
	return object;
}

/*
================
NiCamera::Unknown7E8B90

Draws a sorted object with the planes it was culled against disabled again.
================
*/
// FUNCTION: STBC 0x7E8B90
void NiCamera::Unknown7E8B90( NiSortedObject *object ) {
	NiBitVector saved = disabledPlanes;
	disabledPlanes = object->disabledPlanes;
	renderer->Unknown7E2FD0( this, object );
	disabledPlanes = saved;
}

/*
================
NiCamera::WorldToCameraMatrix

The world-to-camera matrix: the perspective projection of the frustum
times the view matrix of the eye and its axes, row by row; also twice the
reciprocal width and height of the frustum. Gamebryo 1.2.2:
NiCamera::WorldToCameraMatrix (its perspective case; this camera has no
orthographic one).
================
*/
// FUNCTION: STBC 0x7E8C00
void NiCamera::WorldToCameraMatrix() {
	float v03 = -worldRight * worldLoc;
	float v13 = -worldUp * worldLoc;
	float v23 = -worldDir * worldLoc;

	float rpl = frustum.left + frustum.right;
	float tpb = frustum.bottom + frustum.top;
	float rml = 1.0f / ( frustum.right - frustum.left );
	float tmb = 1.0f / ( frustum.top - frustum.bottom );
	float fmn = 1.0f / ( frustum.farDistance - frustum.nearDistance );

	twoDivRmL = 2.0f * rml;
	twoDivTmB = 2.0f * tmb;

	float p00 = 2.0f * rml;
	float p02 = -rpl * rml;
	float p11 = 2.0f * tmb;
	float p12 = -tpb * tmb;
	float p22 = frustum.farDistance * fmn;
	float p23 = -frustum.nearDistance * frustum.farDistance * fmn;

	worldToCam[0][0] = p00 * worldRight.x + p02 * worldDir.x;
	worldToCam[0][1] = p00 * worldRight.y + p02 * worldDir.y;
	worldToCam[0][2] = p00 * worldRight.z + p02 * worldDir.z;
	worldToCam[0][3] = p00 * v03 + p02 * v23;
	worldToCam[1][0] = p11 * worldUp.x + p12 * worldDir.x;
	worldToCam[1][1] = p11 * worldUp.y + p12 * worldDir.y;
	worldToCam[1][2] = p11 * worldUp.z + p12 * worldDir.z;
	worldToCam[1][3] = p11 * v13 + p12 * v23;
	worldToCam[2][0] = p22 * worldDir.x;
	worldToCam[2][1] = p22 * worldDir.y;
	worldToCam[2][2] = p22 * worldDir.z;
	worldToCam[2][3] = p22 * v23 + p23;
	worldToCam[3][0] = worldDir.x;
	worldToCam[3][1] = worldDir.y;
	worldToCam[3][2] = worldDir.z;
	worldToCam[3][3] = v23;
}

/*
================
NiCamera::ScreenSpaceBoundBound

The box a world bound covers in the view: x and y the projections of the
sphere's left, right, bottom and top points, z its nearest and farthest
depths as fractions of the frustum's depth, all clamped to the view (x and
y to -1..1, z to 0..1). A point at or behind the eye gives the whole view
across. Gamebryo 1.2.2: NiCamera::ScreenSpaceBoundBound (its perspective
case; this camera has no orthographic one).
================
*/
// FUNCTION: STBC 0x7E8E80
void NiCamera::ScreenSpaceBoundBound( const NiBound &bound, NiPoint3 &min, NiPoint3 &max ) const {
	NiPoint3 center = bound.GetCenter();
	float radius = bound.GetRadius();
	NiPoint3 toCenter = center - worldLoc;

	// the depths
	float invFmN = 1.0f / ( frustum.farDistance - frustum.nearDistance );
	float minZ = toCenter * worldDir - frustum.nearDistance;
	float maxZ = minZ + radius;
	minZ -= radius;
	min.z = minZ * invFmN;
	max.z = maxZ * invFmN;

	// the left and right sides
	NiPoint3 sphereRight = radius * toCenter.UnitCross( worldUp );
	NiPoint3 tmp = center - sphereRight;
	float w;
	w = tmp.x * worldToCam[3][0] + tmp.y * worldToCam[3][1] + tmp.z * worldToCam[3][2] + worldToCam[3][3];
	if ( w > projEps ) {
		min.x = tmp.x * worldToCam[0][0] + tmp.y * worldToCam[0][1] + tmp.z * worldToCam[0][2] + worldToCam[0][3];
		min.x /= w;
	} else {
		min.x = min.y = -1.0f;
		max.x = max.y = 1.0f;
		if ( min.z < 0.0f ) {
			min.z = 0.0f;
			if ( max.z < 0.0f ) {
				max.z = 0.0f;
			} else if ( max.z > 1.0f ) {
				max.z = 1.0f;
			}
		} else if ( max.z > 1.0f ) {
			max.z = 1.0f;
			if ( min.z > 1.0f ) {
				min.z = 1.0f;
			}
		}
		return;
	}
	tmp = center + sphereRight;
	w = tmp.x * worldToCam[3][0] + tmp.y * worldToCam[3][1] + tmp.z * worldToCam[3][2] + worldToCam[3][3];
	if ( w > projEps ) {
		max.x = tmp.x * worldToCam[0][0] + tmp.y * worldToCam[0][1] + tmp.z * worldToCam[0][2] + worldToCam[0][3];
		max.x /= w;
	} else {
		min.x = min.y = -1.0f;
		max.x = max.y = 1.0f;
		if ( min.z < 0.0f ) {
			min.z = 0.0f;
			if ( max.z < 0.0f ) {
				max.z = 0.0f;
			} else if ( max.z > 1.0f ) {
				max.z = 1.0f;
			}
		} else if ( max.z > 1.0f ) {
			max.z = 1.0f;
			if ( min.z > 1.0f ) {
				min.z = 1.0f;
			}
		}
		return;
	}

	// the bottom and top
	NiPoint3 sphereUp = radius * worldRight.UnitCross( toCenter );
	tmp = center - sphereUp;
	w = tmp.x * worldToCam[3][0] + tmp.y * worldToCam[3][1] + tmp.z * worldToCam[3][2] + worldToCam[3][3];
	if ( w > projEps ) {
		min.y = tmp.x * worldToCam[1][0] + tmp.y * worldToCam[1][1] + tmp.z * worldToCam[1][2] + worldToCam[1][3];
		min.y /= w;
	} else {
		min.x = min.y = -1.0f;
		max.x = max.y = 1.0f;
		if ( min.z < 0.0f ) {
			min.z = 0.0f;
			if ( max.z < 0.0f ) {
				max.z = 0.0f;
			} else if ( max.z > 1.0f ) {
				max.z = 1.0f;
			}
		} else if ( max.z > 1.0f ) {
			max.z = 1.0f;
			if ( min.z > 1.0f ) {
				min.z = 1.0f;
			}
		}
		return;
	}
	tmp = center + sphereUp;
	w = tmp.x * worldToCam[3][0] + tmp.y * worldToCam[3][1] + tmp.z * worldToCam[3][2] + worldToCam[3][3];
	if ( w > projEps ) {
		max.y = tmp.x * worldToCam[1][0] + tmp.y * worldToCam[1][1] + tmp.z * worldToCam[1][2] + worldToCam[1][3];
		max.y /= w;
	} else {
		min.x = min.y = -1.0f;
		max.x = max.y = 1.0f;
		if ( min.z < 0.0f ) {
			min.z = 0.0f;
			if ( max.z < 0.0f ) {
				max.z = 0.0f;
			} else if ( max.z > 1.0f ) {
				max.z = 1.0f;
			}
		} else if ( max.z > 1.0f ) {
			max.z = 1.0f;
			if ( min.z > 1.0f ) {
				min.z = 1.0f;
			}
		}
		return;
	}

	if ( min.x < -1.0f ) {
		min.x = -1.0f;
		if ( max.x < -1.0f ) {
			max.x = -1.0f;
		} else if ( max.x > 1.0f ) {
			max.x = 1.0f;
		}
	} else if ( max.x > 1.0f ) {
		max.x = 1.0f;
		if ( min.x > 1.0f ) {
			min.x = 1.0f;
		}
	}

	if ( min.y < -1.0f ) {
		min.y = -1.0f;
		if ( max.y < -1.0f ) {
			max.y = -1.0f;
		} else if ( max.y > 1.0f ) {
			max.y = 1.0f;
		}
	} else if ( max.y > 1.0f ) {
		max.y = 1.0f;
		if ( min.y > 1.0f ) {
			min.y = 1.0f;
		}
	}

	if ( min.z < 0.0f ) {
		min.z = 0.0f;
		if ( max.z < 0.0f ) {
			max.z = 0.0f;
		} else if ( max.z > 1.0f ) {
			max.z = 1.0f;
		}
	} else if ( max.z > 1.0f ) {
		max.z = 1.0f;
		if ( min.z > 1.0f ) {
			min.z = 1.0f;
		}
	}
}

/*
================
NiCamera::ScreenSpaceBoundSize

The size a world bound covers in the view, not clipped: its radius over
its distance along the view direction, in units of the frustum's width and
height. A bound centred at the eye covers any size. Gamebryo 1.2.2:
NiCamera::ScreenSpaceBoundSize (its perspective case).
================
*/
// FUNCTION: STBC 0x7E94D0
void NiCamera::ScreenSpaceBoundSize( const NiBound &bound, NiPoint2 &size ) const {
	float viewDist = ( bound.GetCenter() - worldLoc ) * worldDir;
	if ( viewDist < projEps ) {
		if ( viewDist > -projEps ) {
			size.x = FLT_MAX;
			size.y = FLT_MAX;
			return;
		}
		viewDist = -viewDist;
	}
	float ratio = bound.GetRadius() / viewDist;
	size.x = ratio * twoDivRmL;
	size.y = ratio * twoDivTmB;
}

/*
================
NiCamera::Unknown7E9560
================
*/
// FUNCTION: STBC 0x7E9560
void NiCamera::Unknown7E9560( NiSortedObject *object, NiPoint3 &min, NiPoint3 &max ) {
	const NiBound &bound = object->geometry->GetWorldBound();
	ScreenSpaceBoundBound( bound, min, max );
}

/*
================
NiCamera::WorldPtToScreenPt

Projects a world point into the port: false when it lies behind the eye or
outside the frustum's sides, otherwise its screen position in the port's
units. The name follows the Gamebryo lineage.
================
*/
// FUNCTION: STBC 0x7E9590
bool NiCamera::WorldPtToScreenPt( const NiPoint3 &pt, float &x, float &y ) {
	float ptX = pt.x;
	float ptY = pt.y;
	float ptZ = pt.z;
	double w = ptX * worldToCam[3][0] + ptY * worldToCam[3][1] + ptZ * worldToCam[3][2] + worldToCam[3][3];
	if ( w > projEps ) {
		double invW = 1.0 / w;
		x = ptX * worldToCam[0][0] + ptY * worldToCam[0][1] + ptZ * worldToCam[0][2] + worldToCam[0][3];
		y = ptX * worldToCam[1][0] + ptY * worldToCam[1][1] + ptZ * worldToCam[1][2] + worldToCam[1][3];
		x *= invW;
		y *= invW;
		if ( x >= -1.0f && x <= 1.0f && y >= -1.0f && y <= 1.0f ) {
			x = ( port.right - port.left ) * x * 0.5f;
			y = ( port.top - port.bottom ) * y * 0.5f;
			x += ( port.left + port.right ) * 0.5f;
			y += ( port.bottom + port.top ) * 0.5f;
			return true;
		}
	}
	return false;
}

/*
================
NiCamera::UpdateActivePlanes
================
*/
// FUNCTION: STBC 0x7E9710
void NiCamera::UpdateActivePlanes() {
	unsigned int count = 0;
	unsigned int sideCount = 0;
	unsigned int sideMask = 0;
	if ( activePlanesSize < numPlanes ) {
		activePlanesSize = numPlanes + MAX_PLANES;
		delete [] activePlanes;
		activePlanes = new NiPlane *[activePlanesSize];
	}
	for ( unsigned int i = 0; i < numPlanes; i++ ) {
		if ( !disabledPlanes.IsSet( i ) ) {
			activePlanes[count++] = planes.GetAt( i );
			if ( i < MAX_PLANES ) {
				sideCount++;
				sideMask |= 1 << i;
			}
		}
	}
	renderer->VirtualSlot52( activePlanes, count, sideCount, sideMask );
}

/*
================
NiCamera::FitNearAndFarToBound

Puts the near and far planes at the bound's nearest and farthest signed
distances along the view direction, the near one held at least
maxFarNearRatio below the far one and at least minNearPlaneDist. Gamebryo
1.2.2: NiCamera::FitNearAndFarToBound.
================
*/
// FUNCTION: STBC 0x7E97D0
bool NiCamera::FitNearAndFarToBound( const NiBound &bound ) {
	float distToCenter = ( bound.GetCenter() - worldLoc ) * worldDir;
	float minDist = distToCenter - bound.GetRadius();
	float maxDist = distToCenter + bound.GetRadius();
	if ( maxDist <= 0.0f ) {
		return false;
	}
	if ( minDist < maxDist / maxFarNearRatio ) {
		minDist = maxDist / maxFarNearRatio;
	}
	if ( minDist < minNearPlaneDist ) {
		minDist = minNearPlaneDist;
	}
	frustum.nearDistance = minDist;
	frustum.farDistance = maxDist;
	UpdateWorldData();
	UpdateWorldBound();
	return true;
}

/*
================
NiCamera::LookAtWorldPoint

Turns the camera to face the world point, with the up vector as near the
given one as the direction allows (Gamebryo 1.1.2 NiCamera::LookAtWorldPoint).
False, with the camera left as it was, when the point is at the camera or
the up vector is along the direction. The axes are taken into the parent's
frame first, and the rotation turns the model's forward axis to the camera's.
================
*/
// FUNCTION: STBC 0x7E98A0 ?LookAtWorldPoint@NiCamera@@QAE_NABVNiPoint3@@0@Z
bool NiCamera::LookAtWorldPoint( const NiPoint3 &worldPt, const NiPoint3 &worldUp ) {
	NiNode *parentNode = parent;

	NiPoint3 direction = worldLoc - worldPt;
	if ( direction * direction < 1e-10f ) {
		return false;
	}
	direction.Unitize();

	NiPoint3 right = worldUp.UnitCross( direction );
	if ( right.SqrLength() < 0.5f ) {
		return false;
	}
	NiPoint3 up = direction.UnitCross( right );
	if ( up.SqrLength() < 0.5f ) {
		return false;
	}

	NiPoint3 lookDirection;
	NiPoint3 lookRight;
	NiPoint3 lookUp;
	if ( parentNode != NULL ) {
		const NiMatrix3 &parentRotate = parentNode->GetWorldRotate();
		lookDirection = direction * parentRotate;
		lookRight = right * parentRotate;
		lookUp = up * parentRotate;
	} else {
		lookDirection = direction;
		lookRight = right;
		lookUp = up;
	}

	NiMatrix3 look;
	look.SetCol( 0, lookRight.x, lookRight.y, lookRight.z );
	look.SetCol( 1, lookUp.x, lookUp.y, lookUp.z );
	look.SetCol( 2, lookDirection.x, lookDirection.y, lookDirection.z );
	NiMatrix3 modelToCamera;
	modelToCamera.SetEntry( 0, 0, 0.0f );
	modelToCamera.SetEntry( 0, 1, 0.0f );
	modelToCamera.SetEntry( 0, 2, 1.0f );
	modelToCamera.SetEntry( 1, 0, 0.0f );
	modelToCamera.SetEntry( 1, 1, 1.0f );
	modelToCamera.SetEntry( 1, 2, 0.0f );
	modelToCamera.SetEntry( 2, 0, -1.0f );
	modelToCamera.SetEntry( 2, 1, 0.0f );
	modelToCamera.SetEntry( 2, 2, 0.0f );
	localRotate = look * modelToCamera;
	UpdateWorldData();
	UpdateWorldBound();
	return true;
}

/*
================
NiCamera::MapBufferPointToViewPoint

The point of the frustum's near plane that a point of the port lies over;
false when the point is outside the port. The name is invented.
================
*/
// FUNCTION: STBC 0x7E9C90 ?MapBufferPointToViewPoint@NiCamera@@QAE_NMMAAM0@Z
bool NiCamera::MapBufferPointToViewPoint( float x, float y, float &viewX, float &viewY ) {
	float fractionY = ( y - port.bottom ) / ( port.top - port.bottom );
	float fractionX = ( x - port.left ) / ( port.right - port.left );
	viewX = fractionX * ( frustum.right - frustum.left ) + frustum.left;
	viewY = ( frustum.top - frustum.bottom ) * fractionY + frustum.bottom;
	if ( x <= port.right && x >= port.left && y <= port.top && y >= port.bottom ) {
		return true;
	}
	return false;
}

/*
================
NiCamera::ViewPointToRay

The ray from the eye through a point of the view plane, the direction
unitized. Gamebryo 1.2.2: NiCamera::ViewPointToRay (its perspective case;
this camera has no orthographic one).
================
*/
// FUNCTION: STBC 0x7E9D50
bool NiCamera::ViewPointToRay( float viewX, float viewY, NiPoint3 &origin, NiPoint3 &direction ) {
	direction = worldDir + worldRight * viewX + worldUp * viewY;
	direction.Unitize();
	origin = worldLoc;
	return true;
}

/*
================
NiCamera::WindowPointToRay

The ray through a point of the renderer's window: the eye and the unit
direction through the matching point of the view plane. Gamebryo 1.2.2:
NiCamera::WindowPointToRay, with its ViewPointToRay's perspective case
written in place.
================
*/
// FUNCTION: STBC 0x7E9E60 ?WindowPointToRay@NiCamera@@QAE_NJJAAVNiPoint3@@0@Z
bool NiCamera::WindowPointToRay( long x, long y, NiPoint3 &origin, NiPoint3 &direction ) {
	NiRender *render = renderer;
	if ( !render ) {
		return false;
	}

	float bufferX, bufferY;
	if ( !render->MapWindowPointToBufferPoint( (unsigned int)x, (unsigned int)y, bufferX, bufferY ) ) {
		return false;
	}

	float viewX, viewY;
	if ( !MapBufferPointToViewPoint( bufferX, bufferY, viewX, viewY ) ) {
		return false;
	}

	return ViewPointToRay( viewX, viewY, origin, direction );
}

/*
================
NiCamera::CreateClone
================
*/
// FUNCTION: STBC 0x7E9FE0
NiObject *NiCamera::CreateClone() {
	NiCamera *clone = new NiCamera;
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiCamera::CopyMembers

The clone draws with neither the scene nor the renderer. The screen
polygons are meant to be cloned, but the loop runs over a new, empty array
instead of this camera's, so the clone gets none.
================
*/
// FUNCTION: STBC 0x7EA050
void NiCamera::CopyMembers( NiCamera *dest ) {
	// sourcePlane, polygonArray: (names invented; fit the /Od frame order)
	NiAVObject::CopyMembers( dest );

	memcpy( dest->worldToCam, worldToCam, sizeof( worldToCam ) );
	dest->worldLoc = worldLoc;
	dest->worldDir = worldDir;
	dest->worldUp = worldUp;
	dest->worldRight = worldRight;
	dest->frustum = frustum;
	dest->port = port;
	dest->lodAdjust = lodAdjust;

	for ( unsigned int i = 0; i < numPlanes; i++ ) {
		NiPlane *sourcePlane = planes.GetAt( i );
		NiPlane *destPlane = dest->planes.GetAt( i );
		*destPlane = *sourcePlane;
	}

	NiTArray<NiPointer<NiScreenPolygon> > polygonArray;
	for ( unsigned int j = 0; j < polygonArray.GetSize(); j++ ) {
		NiPointer<NiScreenPolygon> polygon = polygonArray.GetAt( j );
		if ( polygon ) {
			NiScreenPolygon *clone = (NiScreenPolygon *)polygon->CreateClone();
			assert( clone );
			dest->screenPolygons.SetAt( j, clone );
		} else {
			dest->screenPolygons.SetAt( j, 0 );
		}
	}

	dest->scene = 0;
	dest->renderer = 0;
}

/*
================
NiCamera::CreateObject

Creates the camera and loads it.
================
*/
// FUNCTION: STBC 0x7EA2E0
void NiCamera::CreateObject( NiStream &stream, NiObject *&object ) {
	NiCamera *camera = new NiCamera;
	stream.SetLoadRecord( new NiCameraLoadRecord );
	camera->LoadBinary( stream );
	object = camera;
}

/*
================
NiCamera::LoadBinary

NiAVObject's members, the frustum, the port and the LOD adjustment, then the link
IDs of the scene, the renderer and the screen polygons.
================
*/
// FUNCTION: STBC 0x7EA3F0
void NiCamera::LoadBinary( NiStream &stream ) {
	// polygonCount: (name invented; fits the /Od frame order)
	NiAVObject::LoadBinary( stream );

	NiStreamLoadBinary( stream, frustum.left );
	NiStreamLoadBinary( stream, frustum.right );
	NiStreamLoadBinary( stream, frustum.top );
	NiStreamLoadBinary( stream, frustum.bottom );
	NiStreamLoadBinary( stream, frustum.nearDistance );
	NiStreamLoadBinary( stream, frustum.farDistance );
	NiStreamLoadBinary( stream, port.left );
	NiStreamLoadBinary( stream, port.right );
	NiStreamLoadBinary( stream, port.top );
	NiStreamLoadBinary( stream, port.bottom );
	NiStreamLoadBinary( stream, lodAdjust );

	NiCameraLoadRecord *record = (NiCameraLoadRecord *)stream.GetLoadRecord();
	NiStreamLoadBinary( stream, record->sceneLinkID );
	NiStreamLoadBinary( stream, record->rendererLinkID );

	unsigned int polygonCount;
	NiStreamLoadBinary( stream, polygonCount );
	for ( unsigned int i = 0; i < polygonCount; i++ ) {
		unsigned long linkID;
		NiStreamLoadBinary( stream, linkID );
		record->screenPolygonLinkIDs.Add( linkID );
	}
}

/*
================
NiCamera::LinkObject
================
*/
// FUNCTION: STBC 0x7EA5C0
void NiCamera::LinkObject( NiStream &stream ) {
	NiAVObject::LinkObject( stream );

	NiCameraLoadRecord *record = (NiCameraLoadRecord *)stream.GetLoadRecord();
	scene = (NiNode *)stream.GetObjectFromLinkID( record->sceneLinkID );
	renderer = (NiRender *)stream.GetObjectFromLinkID( record->rendererLinkID );
	for ( unsigned int i = 0; i < record->screenPolygonLinkIDs.GetSize(); i++ ) {
		// the polygon in a local of its own, as Gamebryo 1.2.2's pkPoly
		NiScreenPolygon *polygon = (NiScreenPolygon *)stream.GetObjectFromLinkID( record->screenPolygonLinkIDs.GetAt( i ) );
		screenPolygons.Add( polygon );
	}
}

/*
================
NiCamera::RegisterStreamables

Registers the scene and the screen polygons after the camera; the renderer
is not saved with it.
================
*/
// FUNCTION: STBC 0x7EA6A0
bool NiCamera::RegisterStreamables( NiStream &stream ) {
	if ( !NiAVObject::RegisterStreamables( stream ) ) {
		return false;
	}

	if ( scene ) {
		scene->RegisterStreamables( stream );
	}
	for ( unsigned int i = 0; i < screenPolygons.GetSize(); i++ ) {
		NiScreenPolygon *polygon = screenPolygons.GetAt( i );
		if ( polygon ) {
			polygon->RegisterStreamables( stream );
		}
	}
	return true;
}

/*
================
NiCamera::SaveBinary
================
*/
// FUNCTION: STBC 0x7EA740
void NiCamera::SaveBinary( NiStream &stream ) {
	// polygonCount: (name invented; fits the /Od frame order)
	NiAVObject::SaveBinary( stream );

	NiStreamSaveBinary( stream, frustum.left );
	NiStreamSaveBinary( stream, frustum.right );
	NiStreamSaveBinary( stream, frustum.top );
	NiStreamSaveBinary( stream, frustum.bottom );
	NiStreamSaveBinary( stream, frustum.nearDistance );
	NiStreamSaveBinary( stream, frustum.farDistance );
	NiStreamSaveBinary( stream, port.left );
	NiStreamSaveBinary( stream, port.right );
	NiStreamSaveBinary( stream, port.top );
	NiStreamSaveBinary( stream, port.bottom );
	NiStreamSaveBinary( stream, lodAdjust );
	NiStreamSaveBinary( stream, scene );
	NiStreamSaveBinary( stream, renderer );

	unsigned int polygonCount = screenPolygons.GetSize();
	NiStreamSaveBinary( stream, polygonCount );
	for ( unsigned int i = 0; i < screenPolygons.GetSize(); i++ ) {
		NiScreenPolygon *polygon = screenPolygons.GetAt( i );
		NiStreamSaveBinary( stream, polygon );
	}
}

/*
================
NiCamera::IsEqual
================
*/
// FUNCTION: STBC 0x7EA8C0
bool NiCamera::IsEqual( NiObject *object ) {
	// screenPolygonCount, otherScreenPolygonCount, otherCamera: (names invented; fit the /Od frame order)
	if ( !NiAVObject::IsEqual( object ) ) {
		return false;
	}

	NiCamera *otherCamera = (NiCamera *)object;
	if ( frustum.left != otherCamera->frustum.left || frustum.right != otherCamera->frustum.right
		|| frustum.top != otherCamera->frustum.top || frustum.bottom != otherCamera->frustum.bottom
		|| frustum.nearDistance != otherCamera->frustum.nearDistance || frustum.farDistance != otherCamera->frustum.farDistance ) {
		return false;
	}
	if ( port.left != otherCamera->port.left || port.right != otherCamera->port.right
		|| port.top != otherCamera->port.top || port.bottom != otherCamera->port.bottom ) {
		return false;
	}
	if ( lodAdjust != otherCamera->lodAdjust ) {
		return false;
	}

	unsigned int screenPolygonCount = screenPolygons.GetSize();
	unsigned int otherScreenPolygonCount = otherCamera->screenPolygons.GetSize();
	if ( screenPolygonCount != otherScreenPolygonCount ) {
		return false;
	}
	for ( unsigned int i = 0; i < screenPolygonCount; i++ ) {
		NiScreenPolygon *polygon = screenPolygons.GetAt( i );
		NiScreenPolygon *otherPolygon = otherCamera->screenPolygons.GetAt( i );
		if ( ( polygon && !otherPolygon ) || ( !polygon && otherPolygon ) ) {
			return false;
		}
		if ( polygon && !polygon->IsEqual( otherPolygon ) ) {
			return false;
		}
	}
	return true;
}

/*
================
NiCamera::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7EAAB0
void NiCamera::GetViewerStrings( NiTArray<char *> *strings ) {
	NiAVObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( frustum.GetViewerString( "m_frustum" ) );
	strings->Add( port.GetViewerString( "m_port" ) );
	strings->Add( worldLoc.GetViewerString( "m_wLoc" ) );
	strings->Add( worldDir.GetViewerString( "m_wDir" ) );
	strings->Add( worldUp.GetViewerString( "m_wUp" ) );
	strings->Add( worldRight.GetViewerString( "m_wRight" ) );
}

/*
================
NiFrustum::GetViewerString

Returns "<prefix> = (N=near,F=far,L=left,R=right,T=top,B=bottom)" in a new
string that the caller deletes.
================
*/
// FUNCTION: STBC 0x7EAD60
char *NiFrustum::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 80];
	sprintf( string, "%s = (N=%f,F=%f,L=%f,R=%f,T=%f,B=%f)", prefix, nearDistance, farDistance, left, right, top, bottom );
	return string;
}
