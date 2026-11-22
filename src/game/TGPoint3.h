/*
===============================================================================

	TGPoint3.h

	The game's point or vector. The SWIG interface derives TGPoint3 from
	NiPoint3; LoadBinary and SaveBinary are NiPoint3's (0x811B10, 0x811B50),
	inherited.

	Unlike NiPoint3, TGPoint3 has a destructor: TGMatrix3::Transpose builds
	its rows with the EH vector constructor (constructor and destructor both
	passed), and every function returning a TGPoint3 by value clears a
	constructed-result flag.

===============================================================================
*/

#ifndef __TGPOINT3_H__
#define __TGPOINT3_H__

#include "NiPoint3.h"

class TGMatrix3;

// Where the ray from start along direction meets the sphere: the number of
// meeting points (0 to 2), written to hit1 and hit2. Called by the ship,
// torpedo and AI code; no string names it.
int		Unknown4570D0( const NiPoint3 &start, const NiPoint3 &direction, const NiPoint3 &center, float radius, NiPoint3 *hit1, NiPoint3 *hit2 );

class TGPoint3 : public NiPoint3 {
public:
				TGPoint3( void ) {}
				TGPoint3( float x_, float y_, float z_ ) { x = x_; y = y_; z = z_; }
	// Called out of line by the SWIG wrappers that pass a TGPoint3 by value and
	// by TractorBeamProjector::CalculateRoughDirection's return.
	// FUNCTION: STBC 0x00581f20 ??0TGPoint3@@QAE@ABV0@@Z @game/TractorBeamProjector
				TGPoint3( const TGPoint3 &pt ) { x = pt.x; y = pt.y; z = pt.z; }
	// FUNCTION: STBC 0x00429650 ??0TGPoint3@@QAE@ABVNiPoint3@@@Z @game/TractorBeamProjector
				TGPoint3( const NiPoint3 &pt ) { x = pt.x; y = pt.y; z = pt.z; }
				~TGPoint3( void ) {}
	// Written out member by member like the constructors: ShipClass::VirtualSlot88
	// calls this copy out of line for the torpedo tube's axes where the implicit
	// assignment (forwarding to NiPoint3's, a cheaper inline) would be expanded.
	// FUNCTION: STBC 0x005b5b40 ??4TGPoint3@@QAEAAV0@ABV0@@Z @game/ShipClassSetup
	TGPoint3 &	operator=( const TGPoint3 &pt ) { x = pt.x; y = pt.y; z = pt.z; return *this; }

	float		GetX( void ) const { return x; }
	float		GetY( void ) const { return y; }
	float		GetZ( void ) const { return z; }
	void		SetX( float x_ ) { x = x_; }
	void		SetY( float y_ ) { y = y_; }
	void		SetZ( float z_ ) { z = z_; }
	void		SetXYZ( float x_, float y_, float z_ ) { x = x_; y = y_; z = z_; }
	void		Add( TGPoint3 *pt ) { x += pt->x; y += pt->y; z += pt->z; }
	void		Subtract( TGPoint3 *pt ) { x -= pt->GetX(); y -= pt->GetY(); z -= pt->GetZ(); }
	void		Scale( float scale ) { x *= scale; y *= scale; z *= scale; }

	TGPoint3	Cross( const TGPoint3 &pt ) const;
	TGPoint3	UnitCross( const TGPoint3 &pt ) const;
	TGPoint3	GetAlignedComponent( const TGPoint3 &dir ) const;
	TGPoint3	GetPerpendicularComponent( const TGPoint3 &dir ) const;
	void		MultMatrix( TGMatrix3 *matrix );
	void		MultMatrixLeft( TGMatrix3 *matrix );
	static TGPoint3	GetRandomUnitVector( void );

	// The model axes: forward +Y, up +Z, right +X. The SWIG wrappers
	// TGPoint3_GetModel* name the accessors.
	static TGPoint3	GetModelForward( void );
	static TGPoint3	GetModelBackward( void );
	static TGPoint3	GetModelUp( void );
	static TGPoint3	GetModelDown( void );
	static TGPoint3	GetModelRight( void );
	static TGPoint3	GetModelLeft( void );

	// The zero vector and the unit axes. Invented names, after NiPoint3's
	// ZERO and UNIT_X..UNIT_Z: no string names them, and nothing reads the
	// three unit vectors. They share one exit guard byte (0x980E00), so they
	// are class statics.
	static const TGPoint3	ZERO;
	static const TGPoint3	UNIT_X;
	static const TGPoint3	UNIT_Y;
	static const TGPoint3	UNIT_Z;

	// Declared for the SWIG wrappers that call them.
public:
	void	Set( TGPoint3 pt );	// retail 0x5EE360: the SWIG wrapper TGPoint3_Set calls it
};

// The save's handlers for the shadow instances: the coordinates go into the
// instance's state as a tuple (invented names, after
// ArtificialIntelligence_SaveInstance; UtopiaModule files them).
bool		TGPoint3_SaveInstance( struct _object *instance, struct _object **state );
bool		TGPoint3_LoadInstance( struct _object *instance, struct _object *state );

// The model axes GetModel* return; the names are invented. Their exit
// destructors are unguarded, so they are namespace-scope constants, not
// class statics.
extern const TGPoint3	MODEL_FORWARD;
extern const TGPoint3	MODEL_BACKWARD;
extern const TGPoint3	MODEL_UP;
extern const TGPoint3	MODEL_DOWN;
extern const TGPoint3	MODEL_RIGHT;
extern const TGPoint3	MODEL_LEFT;

/*
================
TGPoint3::GetModelForward

The linker folded this copy with an identical one emitted elsewhere.
================
*/
// FUNCTION: STBC 0x00434cd0
inline TGPoint3 TGPoint3::GetModelForward( void ) {
	return MODEL_FORWARD;
}

/*
================
TGPoint3::GetModelBackward
================
*/
// FUNCTION: STBC 0x005ed6d0
inline TGPoint3 TGPoint3::GetModelBackward( void ) {
	return MODEL_BACKWARD;
}

/*
================
TGPoint3::GetModelUp

The linker folded this copy with an identical one emitted elsewhere.
================
*/
// FUNCTION: STBC 0x005022e0
inline TGPoint3 TGPoint3::GetModelUp( void ) {
	return MODEL_UP;
}

/*
================
TGPoint3::GetModelDown
================
*/
// FUNCTION: STBC 0x005ed7c0
inline TGPoint3 TGPoint3::GetModelDown( void ) {
	return MODEL_DOWN;
}

/*
================
TGPoint3::GetModelRight

The linker folded this copy with an identical one emitted elsewhere.
================
*/
// FUNCTION: STBC 0x00502310
inline TGPoint3 TGPoint3::GetModelRight( void ) {
	return MODEL_RIGHT;
}

/*
================
TGPoint3::GetModelLeft
================
*/
// FUNCTION: STBC 0x005ed8b0
inline TGPoint3 TGPoint3::GetModelLeft( void ) {
	return MODEL_LEFT;
}

/*
================
TGPoint3::Cross
================
*/
// FUNCTION: STBC 0x0045c1a0
inline TGPoint3 TGPoint3::Cross( const TGPoint3 &pt ) const {
	TGPoint3 cross( y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x );
	return cross;
}

/*
================
TGPoint3::UnitCross

The cross product scaled to unit length, or zero when it is shorter than
1e-06, as NiPoint3::UnitCross (the SWIG wrapper TGPoint3_UnitCross calls it).
================
*/
// FUNCTION: STBC 0x00581e60 ?UnitCross@TGPoint3@@QBE?AV1@ABV1@@Z @game/TractorBeamProjector
inline TGPoint3 TGPoint3::UnitCross( const TGPoint3 &pt ) const {
	TGPoint3 cross( y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x );
	float length = cross.Length();
	if ( length > 1e-06f ) {
		cross /= length;
	} else {
		cross = TGPoint3( 0.0f, 0.0f, 0.0f );
	}
	return cross;
}

#endif /* !__TGPOINT3_H__ */
