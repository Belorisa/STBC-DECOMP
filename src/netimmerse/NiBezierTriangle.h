/*
===============================================================================

	NiBezierTriangle.h

	A triangular Bezier patch: ( degree + 1 ) * ( degree + 2 ) / 2 control
	points, tessellated at up to level 7, with a neighbouring patch and edge
	per side.

	Evidence: the NiRTTI (0x9B3068, built at 0x8351B0) is named
	"NiBezierTriangle" with NiBezierPatch's as its base; the vtable at
	0x89A9CC has NiBezierPatch's 47 slots, overrides 0, 1, 4-8, 10, 40,
	42 and 44-46, and adds 47-49 (0x835D90, 0x836F30, 0x837DD0). The constructors (0x835340, 0x835410, 0x8354F0) pass the
	degree and its triangular control point count to NiBezierPatch's, clear
	the three edge words at 0x158 and the members 0x160-0x16C, and allocate
	three blocks of 17 bytes (0x170-0x178), which the destructor (0x835590)
	frees. NiBezierMesh's patch linking (0x82FDA0) stores a patch and an edge
	number per side through 0x8355D0. NiBezierTriangle2, 3 and 4 derive from
	it.

===============================================================================
*/

#ifndef __NIBEZIERTRIANGLE_H__
#define __NIBEZIERTRIANGLE_H__

#include "NiBezierPatch.h"
#include "NiStreamMacros.h"

// FUNCTION: STBC 0x838E20 ??_GNiBezierTriangle@@UAEPAXI@Z
class NiBezierTriangle : public NiBezierPatch {
public:
	// FUNCTION: STBC 0x838E10 ?GetRTTI@NiBezierTriangle@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiBezierTriangle();

	// FUNCTION: STBC 0x838E50 ?VirtualSlot40@NiBezierTriangle@@UAEEXZ
	virtual unsigned char	VirtualSlot40() { return 7; }
	virtual bool			VirtualSlot42( unsigned short start, unsigned short end, unsigned char gridLevel, unsigned char level_, unsigned char pass, short edge );
	virtual void			VirtualSlot44( unsigned char level_ );
	virtual void			VirtualSlot45( unsigned char level_ );
	virtual bool			VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short );

	// TODO identity: slot 45 passes 0, its level, 0, 0, 0 and the three
	// corner flags
	virtual void			VirtualSlot47( unsigned char depth, unsigned char level_, unsigned short u, unsigned short v, bool inverted, unsigned char *corners );
	virtual bool			VirtualSlot48( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, unsigned short side );
	virtual bool			VirtualSlot49( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1, unsigned char level_, bool odd );

							NiBezierTriangle( unsigned short degree_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierTriangle( unsigned short degree_, unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierTriangle();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// the index of the patch across a side among the parent's children and
	// the number of its shared edge, 1-based, 0 for none (the
	// names are invented)
	void					SetNeighbor( unsigned short side, unsigned int neighbor, short edge );

	static const NiRTTI		rtti;

	// the vertex cache's grid: the segments of the highest level
	// (VirtualSlot40); row r holds GRID_ROW - r points, and the first point
	// of the last row is GRID_LAST_ROW
	enum {
		GRID_SEGMENTS		= 128,
		GRID_ROW			= GRID_SEGMENTS + 1,
		GRID_LAST_ROW		= GRID_SEGMENTS * ( GRID_SEGMENTS + 3 ) / 2
	};

protected:
	// the clone gets the neighbours and their edges
	void					CopyMembers( NiBezierTriangle *dest );

	// TODO identity: whether the grid segment from ( u0, v0 ) to ( u1, v1 )
	// is in view and long enough on screen to split; slot 42 asks it where
	// NiBezierRectangle's asks slot 50
	bool					Unknown8381F0( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 );

	// the parameter step between grid points
	static const float		GRID_STEP;

	// the patch slot 45 is tessellating (name invented)
	static NiBezierTriangle *	current;

	unsigned int			neighbors[3];		// per side, the neighbour's index among the parent's children (the name is invented)
	short					neighborEdges[3];	// the neighbour's shared edge per side (the name is invented)
	// the squared lengths of the three sides of the control triangle, set
	// by slot 45 (TODO identity)
	float					unknown160;
	float					unknown164;
	float					unknown168;
	unsigned char			unknown16C;			// flipped by each slot 45 pass; slot 42 compares the caller's pass against it
	// owned blocks of 17 bytes: per side, a bit per grid point of the edge
	// that slot 45 found in the vertex cache (sides: the first row, the
	// first column, the diagonal); slot 42 reads them when the caller's
	// pass differs (the name is invented)
	char *					sideBits[3];
};

NiRegisterStream( NiBezierTriangle );

#endif /* !__NIBEZIERTRIANGLE_H__ */
