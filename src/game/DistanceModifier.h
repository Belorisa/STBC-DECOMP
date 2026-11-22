/*
===============================================================================

	DistanceModifier.h

	Something in a set that changes how far apart two points count as being
	(a nebula makes distances longer for the sensors). A set keeps a list of
	them; SetClass::GetDistance adds up their changes. Three kinds exist:
	one that scales every distance, one that scales the parts of the
	segment on each side of a plane, and one that scales the parts inside a
	group of spheres. No string names the classes: every name here is
	invented.

	Evidence: the abstract vtable 0x88BAE0 has the destructor (0x46D990),
	a pure slot 1 (_purecall) and the stream pair (0x46CA70 writes the
	type at 4 as a long, 0x46CA90 does nothing). The subclasses' vtables
	follow it in .rdata: 0x88BB00 (0xC bytes, type 1: made by the nebula's
	setup 0x5991F0 and handed to the set through 0x4106C0; scales the
	distance by its factor at 8, 0x46D9B0), 0x88BAD0 (0x20 bytes, type 2:
	constructor 0x46CB00) and 0x88BAF0 (0x24 bytes, type 3: constructor
	0x46CD50, which 0x59A2FC calls). The reader 0x46D5F0 takes the type
	from the stream, makes the matching object and has it read itself.

===============================================================================
*/

#ifndef __DISTANCEMODIFIER_H__
#define __DISTANCEMODIFIER_H__

#include "NiPoint3.h"
#include "NiTList.h"

class TGStream;

// The kinds, as the streams store them.
enum distanceModifierType_t {
	DMT_NONE,
	DMT_SCALE,
	DMT_PLANE,
	DMT_SPHERES
};

// FUNCTION: STBC 0x0046d990 ??_GDistanceModifier@@UAEPAXI@Z
class DistanceModifier {
public:
					DistanceModifier( void ) { type = DMT_NONE; }
	virtual			~DistanceModifier( void ) {}

	// The distance between the points as this modifier sees it, given the
	// straight one.
	virtual float	ModifyDistance( float distance, NiPoint3 *from, NiPoint3 *to ) = 0;

	// The base writes the type; the reader below has already taken it when
	// ReadFromStream runs.
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	// A modifier in a stream: the type, then what the kind writes.
	static bool		Write( DistanceModifier *modifier, TGStream *stream );
	static DistanceModifier *	Read( TGStream *stream );

protected:
	long			type;					// 0x04 distanceModifierType_t
};

/*
================
ScaleDistanceModifier

Every distance counts factor times.
================
*/
// FUNCTION: STBC 0x0046d9c0 ??_GScaleDistanceModifier@@UAEPAXI@Z
class ScaleDistanceModifier : public DistanceModifier {
public:
					ScaleDistanceModifier( void ) : factor( 1.0f ) { type = DMT_SCALE; }
	// FUNCTION: STBC 0x0046d9e0 ??1ScaleDistanceModifier@@UAE@XZ
	virtual			~ScaleDistanceModifier( void ) {}

	// FUNCTION: STBC 0x0046d9b0 ?ModifyDistance@ScaleDistanceModifier@@UAEMMPAVNiPoint3@@0@Z
	virtual float	ModifyDistance( float distance, NiPoint3 *from, NiPoint3 *to ) { return distance * factor; }
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	void			SetFactor( float factor_ ) { factor = factor_; }

protected:
	float			factor;					// 0x08
};

/*
================
PlaneDistanceModifier

The part of the segment in front of the plane counts frontFactor times,
the part behind it backFactor times.
================
*/
// FUNCTION: STBC 0x0046cb50 ??_GPlaneDistanceModifier@@UAEPAXI@Z
class PlaneDistanceModifier : public DistanceModifier {
public:
					PlaneDistanceModifier( void );
	virtual			~PlaneDistanceModifier( void );

	virtual float	ModifyDistance( float distance, NiPoint3 *from, NiPoint3 *to );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	// the plane through point with the given normal
	void			SetPlane( const NiPoint3 &point, const NiPoint3 &normal_ );

protected:
	float			frontFactor;			// 0x08
	float			backFactor;				// 0x0C
	NiPoint3		normal;					// 0x10
	float			constant;				// 0x1C a point's distance in front is normal . point + constant
};

// A sphere of a SphereDistanceModifier.
struct DistanceSphere {
	NiPoint3		center;
	float			radius;
};

// A part of a segment, as fractions of its length from the start.
struct DistanceRange {
	float			start;
	float			end;
};

/*
================
SphereDistanceModifier

The parts of the segment inside the spheres count factor times.
================
*/
// FUNCTION: STBC 0x0046cd90 ??_GSphereDistanceModifier@@UAEPAXI@Z
class SphereDistanceModifier : public DistanceModifier {
public:
					SphereDistanceModifier( float factor_ );
	virtual			~SphereDistanceModifier( void );

	virtual float	ModifyDistance( float distance, NiPoint3 *from, NiPoint3 *to );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

protected:
	float			factor;					// 0x08
	NiTList<DistanceSphere *>	spheres;	// 0x0C owned

public:
	// takes over the sphere (invented name). Evidence: MetaNebula::AddSphere
	// (0x59B520) expands an AddTail on the list at 0xC of its modifier, with
	// the out-of-line NewItem 0x59C5C0.
	void			AddSphere( DistanceSphere *sphere ) { spheres.AddTail( sphere ); }
};

#endif /* !__DISTANCEMODIFIER_H__ */
