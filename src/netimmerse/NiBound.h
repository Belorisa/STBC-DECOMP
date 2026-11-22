/*
===============================================================================

	NiBound.h

	A bounding sphere.

	Evidence: NiBound.cpp's viewer string prints a point and a radius
	("%s = (%f,%f,%f) , %f"); the streaming functions stream an NiPoint3
	and then 4 more bytes; the constructor (0x8122C0) sets the zero point, a
	zero radius and a flag byte at 0x10, which ComputeFromData clears.
	NiAVObject holds its world bound at 0x1C.

===============================================================================
*/

#ifndef __NIBOUND_H__
#define __NIBOUND_H__

#include "NiPlane.h"
#include "NiPoint3.h"

template <class T> class NiTArray;

class NiMatrix3;
class NiStream;

// FUNCTION: STBC 0x804AE0 ??_ENiBound@@QAEPAXI@Z @netimmerse/NiSkinController
class NiBound {
public:
					NiBound();
					~NiBound();

	NiBound &		operator=( const NiBound &bound );
	bool			operator==( const NiBound &bound );
	bool			operator!=( const NiBound &bound ) { return !( *this == bound ); }
	// whether this sphere lies within another, and holds another, up to
	// the fuzz factor (Gamebryo lineage)
	bool			operator<=( const NiBound &bound );
	bool			operator>=( const NiBound &bound );

	// Gamebryo lineage names
	const NiPoint3 &	GetCenter() const { return center; }
	float			GetRadius() const { return radius; }
	void			SetCenter( const NiPoint3 &center_ ) { center = center_; }
	// Gamebryo lineage (NIBOUND.INL): the centre through a temporary point
	void			SetCenter( float x, float y, float z ) { center = NiPoint3( x, y, z ); }
	void			SetRadius( float radius_ ) { radius = radius_; }
	// AnimOnly.exe's assert names it (NiBezierPatch.cpp, 0x4D2D9D)
	bool			GetNeedUpdate() const { return unknown10; }
	// its setter (name invented; AnimOnly.exe 0x4A3780)
	void			SetNeedUpdate( bool needUpdate ) { unknown10 = needUpdate; }
	void			SetCenterAndRadius( const NiPoint3 &center_, float radius_ ) { center = center_; radius = radius_; unknown10 = false; }

	// the bound carried by a scale, a rotation and a translation
	void			Update( const NiBound &bound, const NiMatrix3 &rotate, const NiPoint3 &translate, float scale );

	void			ComputeFromData( int count, const NiPoint3 *points );
	// the sphere holding every sphere of the array, which must not be empty
	// (Gamebryo lineage name; nothing in retail calls it)
	void			ComputeMinimalBound( NiTArray<const NiBound *> &bounds );

	// grows the sphere to hold another (Gamebryo lineage name)
	void			Merge( const NiBound *bound );

	// the side of a plane the sphere lies on (Gamebryo lineage name)
	NiPlane::Side	WhichSide( const NiPlane &plane ) const;

	// the collision tests between two moving spheres: whether they meet
	// within deltaTime, and where and when they first do (Gamebryo lineage
	// names)
	static bool		TestIntersect( float deltaTime, const NiBound &bound0, const NiPoint3 &velocity0,
								   const NiBound &bound1, const NiPoint3 &velocity1 );
	static bool		FindIntersect( float deltaTime, const NiBound &bound0, const NiPoint3 &velocity0,
								   const NiBound &bound1, const NiPoint3 &velocity1, float &time, NiPoint3 &point,
								   bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 );

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

	NiPoint3		center;		// the center
	float			radius;		// the radius
	bool			unknown10;		// set by the constructor, cleared once computed

private:
	// the slack of the containment comparisons (Gamebryo 1.1.2: ms_fFuzzFactor, ms_fTolerance)
	static float	fuzzFactor;
	static float	tolerance;
};

#endif /* !__NIBOUND_H__ */
