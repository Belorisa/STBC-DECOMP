/*
===============================================================================

	NiCamera.h

	The camera: a scene-graph object whose world transform places the eye.
	The view direction is the world rotation's first column, up the second
	and right the third. It keeps the view frustum, the view port, the world
	planes of the frustum for culling, the scene and renderer it draws with,
	and the screen polygons drawn over the view.

	Evidence: the static initializer at 0x7E7960 builds the NiRTTI at
	0x9A1908 ("NiCamera", based on NiAVObject's); the vtable at 0x89902C has
	NiAVObject's 39 slots and adds none (the two words after it are the
	single-slot vtables of the plane and screen polygon arrays, 0x8990C8 and
	0x8990CC); the constructor (0x7E7AF0) and CreateClone (0x7E9FE0, 0x224
	bytes). The viewer strings (0x7EAAB0) name "m_wLoc" (0x114), "m_wDir"
	(0x120), "m_wUp" (0x12C), "m_wRight" (0x138), "m_frustum" (0x144) and
	"m_port" (0x15C). The streaming functions link the scene (0x16C), the
	renderer (0x188) and the screen polygons (0x170). UpdateWorldData
	(0x7E8780) copies the world transform into the four vectors and builds
	the six frustum planes; WorldToCameraMatrix (0x7E8C00) then fills the world-to-camera matrix
	(0xC8) and the two port scales (0x10C, 0x110). The member names follow
	the viewer strings and the Gamebryo lineage.

===============================================================================
*/

#ifndef __NICAMERA_H__
#define __NICAMERA_H__

#include <string.h>

#include "NiAccumulator.h"
#include "NiAVObject.h"
#include "renderer/NiRender.h"
#include "NiNode.h"
#include "NiFrustum.h"
#include "NiPlane.h"
#include "NiRect.h"
#include "NiScreenPolygon.h"
#include "NiStreamMacros.h"

class NiSortedObject;

/*
================
NiCameraLoadRecord

The link IDs of the scene, the renderer and the screen polygons, read by
LoadBinary (0x7EA3F0) and resolved by LinkObject (0x7EA5C0). The class name
is invented. The link ID array is not an NiTArray<unsigned int>: its vtable
(0x8990D8) differs from that of NiAVObjectLoadRecord's array (0x88B914).
================
*/
// FUNCTION: STBC 0x007eb220 ??_GNiCameraLoadRecord@@UAEPAXI@Z @netimmerse/NiCamera
class NiCameraLoadRecord : public NiAVObjectLoadRecord {
public:
							NiCameraLoadRecord() { sceneLinkID = 0; rendererLinkID = 0; }

	unsigned int			sceneLinkID;
	unsigned int			rendererLinkID;
	NiTArray<unsigned long>	screenPolygonLinkIDs;
};

/*
================
NiBitVector

One bit per frustum plane; a set bit takes the plane out of culling
(0x7E9710). NiCamera's constructor clears it. The class name is
DamageTool.exe's (NiAVObject::CullAgainstPlanes( NiCamera *, NiBitVector & ),
0x437430, byte-identical to retail's 0x7DCA70); the Gamebryo lineage keeps
its forward declaration in NiAVObject.h and NiNode.h. The member names are
invented.
================
*/
class NiBitVector {
public:
							NiBitVector() { memset( bits, 0, sizeof( bits ) ); }

	bool					IsSet( unsigned int index ) const { return ( bits[index >> 5] & ( 1 << ( index & 31 ) ) ) != 0; }
	void					Set( unsigned int index ) { bits[index >> 5] |= 1 << ( index & 31 ); }
	void					Clear( unsigned int index ) { bits[index >> 5] &= ~( 1 << ( index & 31 ) ); }

	unsigned int			bits[4];
};

class NiCamera : public NiAVObject {
public:
	// FUNCTION: STBC 0x7EB1F0 ?GetRTTI@NiCamera@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiCamera();

	// FUNCTION: STBC 0x7EB280 ?VirtualSlot26@NiCamera@@UAE_NXZ
	virtual bool			VirtualSlot26() { return false; }
	virtual void			UpdateWorldData();
	// a camera has no extent: its world bound is the eye point
	// FUNCTION: STBC 0x7E8AF0 ?UpdateWorldBound@NiCamera@@UAEXXZ
	virtual void			UpdateWorldBound() { worldBound.center = worldTranslate; }

							NiCamera();

	// what Click draws, and what it draws with (Gamebryo lineage names)
	void					SetScene( NiNode *scene_ );
	NiNode *				GetScene();
	void					SetRenderer( NiRender *renderer_ );
	NiRender *				GetRenderer() const;
	void					SetLODAdjust( float lodAdjust_ );
	float					GetLODAdjust() const;
	const NiPoint3 &		GetWorldLocation() const;
	const NiPoint3 &		GetWorldDirection() const;
	const NiPoint3 &		GetWorldUpVector() const;
	const NiPoint3 &		GetWorldRightVector() const;

	// the near distance is kept at least maxFarNearRatio below the far one,
	// and at least minNearPlaneDist (Gamebryo lineage names)
	void					SetViewFrustum( const NiFrustum &frustum_ );
	const NiFrustum &		GetViewFrustum() const;
	void					SetViewPort( const NiRect<float> &port_ );
	const NiRect<float> &	GetViewPort() const;
	// the 4x4 world-to-camera matrix, row by row (Gamebryo 1.2.2 name)
	const float *			GetWorldToCameraMatrix() const;

	// the polygons Click draws over the scene (Gamebryo lineage names);
	// NiFrameRate's out-of-line copies take the smart pointer by value
	void					AttachScreenPolygon( NiPointer<NiScreenPolygon> polygon );
	void					DetachScreenPolygon( NiPointer<NiScreenPolygon> polygon );

	static float			minNearPlaneDist;
	static float			maxFarNearRatio;
	static const float		projEps;

	// draws the scene and then the screen polygons through the renderer
	// (Gamebryo lineage name)
	void					Click();

	// TODO identity: the cluster accumulator's objects: wraps a geometry
	// object with the rectangle at 0x208 (0x7E8B10), and draws one in its
	// rectangle (0x7E8B90)
	NiSortedObject *		Unknown7E8B10( NiAVObject *geometry );
	void					Unknown7E8B90( NiSortedObject *object );

	// the box a world bound covers in the view, screen x and y and depth z
	// (Gamebryo 1.2.2 name; 0x7E8E80); TODO identity: the box of a sorted
	// object's geometry (0x7E9560), which the cluster sorter takes
	void					ScreenSpaceBoundBound( const NiBound &bound, NiPoint3 &min, NiPoint3 &max ) const;
	// the size it covers, not clipped (Gamebryo 1.2.2 name; 0x7E94D0)
	void					ScreenSpaceBoundSize( const NiBound &bound, NiPoint2 &size ) const;
	void					Unknown7E9560( NiSortedObject *object, NiPoint3 &min, NiPoint3 &max );

	// the culling planes and the mask of those out of use (names invented)
	int						GetPlaneCount() const { return numPlanes; }
	const NiPlane &			GetPlane( int index ) const { return *planes.GetAt( index ); }
	// the planes themselves, for a caller that walks them (name invented;
	// SpaceCamera::UpdateSpaceDust, 0x42EEC0)
	NiPlane **				GetPlanes() const { return planes.GetBase(); }
	// the array of them (name invented; AnimOnly.exe 0x417DE0 returns its
	// address to NiAVObject's culling, which reads each plane through GetAt)
	NiTArray<NiPlane *> &	GetPlaneArray() { return planes; }
	NiBitVector &			GetDisabledPlanes() { return disabledPlanes; }
	// passes the planes not disabled to the renderer, growing activePlanes
	// to hold them; the frustum sides among them also as a count and a
	// bit mask (name invented)
	void					UpdateActivePlanes();

	// the screen position of a world point, if it is in view
	bool					WorldPtToScreenPt( const NiPoint3 &pt, float &x, float &y );

	// turns the camera to face the world point with the up vector given,
	// false when the point or the up vector leaves no direction (Gamebryo
	// 1.1.2 SDK name and signature; 0x7E98A0)
	bool					LookAtWorldPoint( const NiPoint3 &worldPt, const NiPoint3 &worldUp );

	// the point of the view frustum that a point of the port is (0..1 across and
	// down the port); false when the point is outside the port (name invented)
	bool					MapBufferPointToViewPoint( float x, float y, float &viewX, float &viewY );

	// the ray through a point of the view plane: the eye point and the unit
	// direction (Gamebryo 1.2.2 name; 0x7E9D50)
	bool					ViewPointToRay( float viewX, float viewY, NiPoint3 &origin, NiPoint3 &direction );

	// the ray through a pixel of the renderer's window: the eye point and
	// the unit direction; false without a renderer or outside the port
	// (Gamebryo lineage name; 0x7E9E60)
	bool					WindowPointToRay( long x, long y, NiPoint3 &origin, NiPoint3 &direction );

	// sets the near and far distances to enclose the bound in front of the
	// eye; false when it lies wholly behind (Gamebryo 1.2.2 name; 0x7E97D0)
	bool					FitNearAndFarToBound( const NiBound &bound );

	// TODO identity: callers in game code; they pass through to renderer
	// slots 18 (a rect and flags) and 17
	void					Clear( unsigned int flags );
	void					Unknown7E81B0();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the frustum planes, in the order UpdateWorldData builds them
	enum {
		NEAR_PLANE,
		FAR_PLANE,
		LEFT_PLANE,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
		MAX_PLANES
	};

protected:
	// the Bezier patches' split tests project through worldToCam
	friend class NiBezierTriangle;
	friend class NiBezierRectangle;

	void					CopyMembers( NiCamera *dest );

	// the frustum planes from the eye and its axes (Gamebryo 1.2.2 names)
	void					SetNearCullingPlane( float distance, NiPlane *plane ) const;
	void					SetFarCullingPlane( float distance, NiPlane *plane ) const;
	void					SetLeftCullingPlane( float left, NiPlane *plane ) const;
	void					SetRightCullingPlane( float right, NiPlane *plane ) const;
	void					SetTopCullingPlane( float top, NiPlane *plane ) const;
	void					SetBottomCullingPlane( float bottom, NiPlane *plane ) const;
	void					WorldToCameraMatrix();	// fills worldToCam and the port scales (Gamebryo 1.2.2 name)

	float					worldToCam[4][4];
	float					viewDist;		// never read or written by the class (Gamebryo 1.1.2: m_fViewDist, between the world-to-camera matrix and m_fTwoDivRmL)
	float					twoDivRmL;		// 2 / ( right - left ), set by WorldToCameraMatrix (Gamebryo 1.2.2: m_fTwoDivRmL)
	float					twoDivTmB;		// 2 / ( top - bottom ), set by WorldToCameraMatrix (Gamebryo 1.2.2: m_fTwoDivTmB)
	NiPoint3				worldLoc;		// the eye, the world translation
	NiPoint3				worldDir;		// the view direction, the world rotation's first column
	NiPoint3				worldUp;		// the second column
	NiPoint3				worldRight;		// the third column
	NiFrustum				frustum;
	NiRect<float>			port;
	NiPointer<NiNode>		scene;			// the scene Click draws
	NiTArray<NiPointer<NiScreenPolygon> >	screenPolygons;
	NiPointer<NiRender>		renderer;
	NiTArray<NiPlane *>		planes;			// the culling planes: worldPlanes, and any added after them
	unsigned int			numPlanes;		// TODO identity: MAX_PLANES after construction; 0x7E9710 culls with this many
	NiPlane					worldPlanes[MAX_PLANES];
	NiBitVector				disabledPlanes;
	NiPlane **				activePlanes;	// heap block of activePlanesSize, filled by 0x7E9710 for the renderer
	unsigned int			activePlanesSize;
	float					lodAdjust;		// the scale of the level-of-detail distances, 1.0 after construction (Gamebryo lineage name; NiLODNode::VirtualSlot35, 0x7FF6B0, squares it)
};

NiRegisterStream( NiCamera );

/*
================
NiCamera::GetWorldLocation
================
*/
inline const NiPoint3 &NiCamera::GetWorldLocation() const {
	return worldLoc;
}

/*
================
NiCamera::GetWorldDirection
================
*/
inline const NiPoint3 &NiCamera::GetWorldDirection() const {
	return worldDir;
}

/*
================
NiCamera::GetWorldUpVector
================
*/
inline const NiPoint3 &NiCamera::GetWorldUpVector() const {
	return worldUp;
}

/*
================
NiCamera::GetWorldRightVector
================
*/
inline const NiPoint3 &NiCamera::GetWorldRightVector() const {
	return worldRight;
}

/*
================
NiCamera::SetViewFrustum
================
*/
inline void NiCamera::SetViewFrustum( const NiFrustum &frustum_ ) {
	float nearDistance = frustum_.nearDistance;
	float minNear = frustum_.farDistance / maxFarNearRatio;
	if ( nearDistance < minNear ) {
		nearDistance = minNear;
	}
	if ( nearDistance < minNearPlaneDist ) {
		nearDistance = minNearPlaneDist;
	}
	frustum.left = frustum_.left;
	frustum.right = frustum_.right;
	frustum.top = frustum_.top;
	frustum.bottom = frustum_.bottom;
	frustum.nearDistance = nearDistance;
	frustum.farDistance = frustum_.farDistance;
}

/*
================
NiCamera::GetViewFrustum
================
*/
inline const NiFrustum &NiCamera::GetViewFrustum() const {
	return frustum;
}

/*
================
NiCamera::SetViewPort
================
*/
inline void NiCamera::SetViewPort( const NiRect<float> &port_ ) {
	port = port_;
}

/*
================
NiCamera::GetViewPort
================
*/
inline const NiRect<float> &NiCamera::GetViewPort() const {
	return port;
}

/*
================
NiCamera::SetScene
================
*/
inline void NiCamera::SetScene( NiNode *scene_ ) {
	scene = scene_;
}

/*
================
NiCamera::GetScene
================
*/
inline NiNode *NiCamera::GetScene() {
	return scene;
}

/*
================
NiCamera::SetRenderer
================
*/
inline void NiCamera::SetRenderer( NiRender *renderer_ ) {
	renderer = renderer_;
}

/*
================
NiCamera::GetRenderer
================
*/
inline NiRender *NiCamera::GetRenderer() const {
	return renderer;
}

/*
================
NiCamera::AttachScreenPolygon
================
*/
// FUNCTION: STBC 0x7BCA10 ?AttachScreenPolygon@NiCamera@@QAEXV?$NiPointer@VNiScreenPolygon@@@@@Z
inline void NiCamera::AttachScreenPolygon( NiPointer<NiScreenPolygon> polygon ) {
	screenPolygons.AddFirstEmpty( polygon );
}

/*
================
NiCamera::DetachScreenPolygon

Empties the polygon's slot; the array keeps its size.
================
*/
// FUNCTION: STBC 0x7BCE40 ?DetachScreenPolygon@NiCamera@@QAEXV?$NiPointer@VNiScreenPolygon@@@@@Z
inline void NiCamera::DetachScreenPolygon( NiPointer<NiScreenPolygon> polygon ) {
	for ( unsigned int i = 0; i < screenPolygons.GetSize(); i++ ) {
		if ( screenPolygons.GetAt( i ) == polygon ) {
			screenPolygons.SetAt( i, 0 );
			break;
		}
	}
}

/*
================
NiCamera::SetLODAdjust
================
*/
inline void NiCamera::SetLODAdjust( float lodAdjust_ ) {
	lodAdjust = lodAdjust_;
}

/*
================
NiCamera::GetLODAdjust
================
*/
inline float NiCamera::GetLODAdjust() const {
	return lodAdjust;
}

/*
================
NiCamera::GetWorldToCameraMatrix
================
*/
inline const float *NiCamera::GetWorldToCameraMatrix() const {
	return &worldToCam[0][0];
}

#endif /* !__NICAMERA_H__ */
