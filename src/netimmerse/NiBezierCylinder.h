/*
===============================================================================

	NiBezierCylinder.h

	A cylindrical Bezier patch: two rows of degree + 1 control points,
	evaluated through the Bernstein basis of its degree.

	Evidence: the NiRTTI (0x9B32C0, built at 0x84CA20) is named
	"NiBezierCylinder" with NiBezierPatch's as its base; the vtable at
	0x89AF9C has NiBezierPatch's 47 slots, overrides 0, 1, 4-8, 10, 39-41
	and 43-46, and adds 47-49. The constructor (0x84CBB0) passes the degree
	and 2 * ( degree + 1 ) control points to NiBezierPatch's, then
	allocates the power tables (0x84CD10, inlined) and the binomial tables
	(0x84CD60); the creation function (0x850A30) allocates 0x164 bytes and
	a 0x28-byte load record of its own (vtable 0x89B070). The destructor
	(0x84CCC0) frees the four tables.

===============================================================================
*/

#ifndef __NIBEZIERCYLINDER_H__
#define __NIBEZIERCYLINDER_H__

#include "NiBezierPatch.h"
#include "NiStreamMacros.h"

/*
================
NiBezierCylinderLoadRecord

The record CreateObject hands the stream: NiAVObject's link IDs only. The
class name is invented.

Evidence: CreateObject (0x850A30) allocates 0x28 bytes and stores the vptr
0x89B070 after the property link ID array at 0x10; its destructor
(0x850B60) frees the array.
================
*/
// FUNCTION: STBC 0x850B40 ??_GNiBezierCylinderLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x850B60 ??1NiBezierCylinderLoadRecord@@UAE@XZ
class NiBezierCylinderLoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x851190 ??_GNiBezierCylinder@@UAEPAXI@Z
class NiBezierCylinder : public NiBezierPatch {
public:
	// FUNCTION: STBC 0x851180 ?GetRTTI@NiBezierCylinder@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiBezierCylinder();

	// retessellates at the level, when it differs from the current one and
	// does not exceed slot 40's; the cylinder has a single tessellator
	virtual void			VirtualSlot39( unsigned char level_ );
	// the highest level, which falls as unknown160 grows
	virtual unsigned char	VirtualSlot40();
	virtual unsigned int	VirtualSlot41( float, float );
	virtual void			VirtualSlot43( float, float );
	virtual void			VirtualSlot44( unsigned char level_ );
	virtual void			VirtualSlot45( unsigned char level_ );
	virtual bool			VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short );

	// TODO identity: 2 ^ slot 40's level + 1, the vertices along the
	// tessellated curve
	virtual unsigned short	VirtualSlot47();
	// TODO identity: unknown160 + 2
	virtual unsigned short	VirtualSlot48();
	// TODO identity: refines the strip of the grid from the point first,
	// recursing while its rows split; slot 45 passes 0, its level, 0 and a
	// scratch buffer of three words a row
	virtual void			VirtualSlot49( unsigned char depth, unsigned char level_, unsigned short first, unsigned short *columns );

							NiBezierCylinder( unsigned short degree_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierCylinder();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// the clone gets unknown160 (see the definition for its quirk)
	void					CopyMembers( NiBezierCylinder *dest );

	// TODO identity: sets unknown160 when it differs and the current level
	// allows it (Unknown84D020), then retessellates; nothing in retail calls it
	void					Unknown84CFB0( unsigned char unknown160_ );
	// unknown160 (name invented; the debug copy calls it out of line, AnimOnly.exe 0x4F72D0)
	unsigned char			GetUnknown160() const { return unknown160; }
	// TODO identity: the largest unknown160 the current level allows, the
	// inverse of slot 40; nothing in retail calls it
	unsigned char			Unknown84D020();

	// the first entries of the power tables (the name is invented)
	void					AllocatePowers();
	// the binomial coefficient tables of the degree (the name is invented)
	void					ComputeBinomials();

	unsigned short			order;				// degree + 1: the entries of binomials and the power tables (the name is invented)
	unsigned short			unknown14E;			// degree - 1: the entries of unknown154
	float *					binomials;			// owned: C( degree, i ) for i = 0..degree (the name is invented)
	float *					unknown154;			// owned: C( degree - 2, i ) for i = 0..degree - 2 (TODO identity)
	float *					powers;				// owned: t ^ i, filled by slot 43 (the name is invented)
	float *					complementPowers;	// owned: ( 1 - t ) ^ i, filled by slot 43 (the name is invented)
	unsigned char			unknown160;			// streamed; lowers slot 40's level (TODO identity)
};

NiRegisterStream( NiBezierCylinder );

#endif /* !__NIBEZIERCYLINDER_H__ */
