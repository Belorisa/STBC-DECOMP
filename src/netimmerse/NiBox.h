/*
===============================================================================

	NiBox.h

	An oriented box: a center, three unit axes and the half extents along
	them.

	0x3C bytes. Evidence: 0x7BF5C0, called on the box at 0x08 of the game's
	OBBModel (0x464330) with a vertex count (unsigned short) and the
	vertices, averages the points into the center, solves the covariance
	matrix for its eigenvectors (NiMatrix3::EigenSolveSymmetric) into the
	three axes at 0x0C, and fits the center and the half extents at 0x30
	around the projections. The class and method names follow the Gamebryo
	lineage (NiBox::CreateFromData); the member names are invented.

===============================================================================
*/

#ifndef __NIBOX_H__
#define __NIBOX_H__

#include "NiPoint3.h"

class NiStream;

class NiBox {
public:
	// fits the box around count points
	void				CreateFromData( unsigned short count, const NiPoint3 *points );

	// center + p0 axis[0] + p1 axis[1] + p2 axis[2] (Gamebryo lineage name)
	NiPoint3			GetPoint( float p0, float p1, float p2 ) const;

	bool				operator==( const NiBox &box ) const;
	bool				operator!=( const NiBox &box ) const;

	// streaming (Gamebryo lineage names)
	void				LoadBinary( NiStream &stream );
	void				SaveBinary( NiStream &stream );

	NiPoint3			center;			// 0x00
	NiPoint3			axis[3];		// 0x0C unit length
	float				extent[3];		// 0x30 half extents along the axes
};

#endif /* !__NIBOX_H__ */
