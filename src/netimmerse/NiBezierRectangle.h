/*
===============================================================================

	NiBezierRectangle.h

	A rectangular Bezier patch: ( degree + 1 ) squared control points,
	tessellated at up to level 6, with a neighbouring patch and edge per
	side.

	Evidence: the NiRTTI (0x9B31B8, built at 0x842280) is named
	"NiBezierRectangle" with NiBezierPatch's as its base; the vtable at
	0x89AD30 has NiBezierPatch's 47 slots, overrides 0, 1, 4-8, 10, 40,
	42 and 44-46, and adds 47-50 (0x842D50, 0x846420, 0x8470C0, 0x847450). The constructors (0x842410, 0x8424D0, 0x8425A0) pass the
	degree and its square control point count to NiBezierPatch's, clear the
	four edge words at 0x15C and the byte at 0x164, and allocate four blocks
	of 9 bytes (0x168-0x174), which the destructor (0x842640) frees.
	NiBezierMesh's patch linking (0x82FDA0) stores a patch and an edge
	number per side through 0x842690. NiBezierRectangle2 and 3 derive from
	it.

===============================================================================
*/

#ifndef __NIBEZIERRECTANGLE_H__
#define __NIBEZIERRECTANGLE_H__

#include "NiBezierPatch.h"
#include "NiStreamMacros.h"

// FUNCTION: STBC 0x847F60 ??_GNiBezierRectangle@@UAEPAXI@Z
class NiBezierRectangle : public NiBezierPatch {
public:
	// FUNCTION: STBC 0x847F50 ?GetRTTI@NiBezierRectangle@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiBezierRectangle();

	// FUNCTION: STBC 0x847F90 ?VirtualSlot40@NiBezierRectangle@@UAEEXZ
	virtual unsigned char	VirtualSlot40() { return 6; }
	virtual bool			VirtualSlot42( unsigned short start, unsigned short end, unsigned char gridLevel, unsigned char level_, unsigned char pass, short edge );
	virtual void			VirtualSlot44( unsigned char level_ );
	virtual void			VirtualSlot45( unsigned char level_ );
	virtual bool			VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short );

	// TODO identity: refines the square at the grid point first, at the
	// depth, from the sides the parent split; slot 45 passes 0, its level, 0
	// and the four corner flags
	virtual void			VirtualSlot47( unsigned char depth, unsigned char level_, unsigned short first, unsigned char *corners );
	virtual bool			VirtualSlot48( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, unsigned short side );
	virtual bool			VirtualSlot49( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, bool odd );
	// TODO identity: whether the grid segment from ( u0, v0 ) to ( u1, v1 )
	// is in view and long enough on screen to split
	virtual bool			VirtualSlot50( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 );

							NiBezierRectangle( unsigned short degree_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierRectangle( unsigned short degree_, unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierRectangle();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// the index of the patch across a side among the parent's children and
	// the number of its shared edge, 1-based, 0 for none (the
	// names are invented)
	void					SetNeighbor( unsigned short side, unsigned int neighbor, short edge );

	static const NiRTTI		rtti;

	// the vertex cache's grid: the segments of the highest level
	// (VirtualSlot40), and the points to a row
	enum {
		GRID_SEGMENTS		= 64,
		GRID_ROW			= GRID_SEGMENTS + 1
	};
	// the parameter step between grid points
	static const float		GRID_STEP;

protected:
	// the clone gets the neighbours and their edges
	void					CopyMembers( NiBezierRectangle *dest );

	// the patch slot 45 is tessellating; slot 50 compares against it (name
	// invented)
	static NiBezierRectangle *	current;

	unsigned int			neighbors[4];		// per side, the neighbour's index among the parent's children (the name is invented)
	short					neighborEdges[4];	// the neighbour's shared edge per side (the name is invented)
	unsigned char			unknown164;			// flipped by each slot 45 pass; slot 42 compares the caller's pass against it
	// owned blocks of 9 bytes: per side, a bit per grid point of the edge
	// that slot 45 found in the vertex cache (sides: the first row, the
	// first column, the last column, the last row); slot 42 reads them
	// when the caller's pass differs (the name is invented)
	char *					sideBits[4];
};

NiRegisterStream( NiBezierRectangle );

#endif /* !__NIBEZIERRECTANGLE_H__ */
