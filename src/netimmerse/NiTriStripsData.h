/*
===============================================================================

	NiTriStripsData.h

	Triangle data drawn as several triangle strips over shared vertices.

	Evidence: the NiRTTI (0x9A26C8, initialised at 0x80E450) is named
	"NiTriStripsData" with NiTriBasedGeomData's as its base; CreateObject
	(0x80E6B0) allocates 0x44 bytes, runs NiTriBasedGeomData's constructor,
	stores the vtable 0x89A5D8 (18 slots; the load record's follows) and
	clears the word at 0x38 and the pointers at 0x3C and 0x40. Only slot 14
	is overridden past the streaming slots. The renderer's slot 48 reads
	the strip count (a word at 0x38), the lengths (0x3C) and the vertex
	lists (0x40). Member names follow the Gamebryo lineage (m_usStrips,
	m_pusStripLengths, m_pusStripLists).

===============================================================================
*/

#ifndef __NITRISTRIPSDATA_H__
#define __NITRISTRIPSDATA_H__

#include "NiStream.h"
#include "NiTriBasedGeomData.h"
#include "NiStreamMacros.h"

/*
================
NiTriStripsDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x89A620). The class name is invented.
================
*/
// FUNCTION: STBC 0x0080E750 ??_GNiTriStripsDataLoadRecord@@UAEPAXI@Z
class NiTriStripsDataLoadRecord : public NiObjectLoadRecord {
};

class NiTriStripsData : public NiTriBasedGeomData {
public:
	// FUNCTION: STBC 0x80EBB0 ?GetRTTI@NiTriStripsData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiTriStripsData();

	virtual void			CalculateNormals();

							NiTriStripsData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short stripCount_, unsigned short *stripLengths_, unsigned short *stripLists_ );
							NiTriStripsData();

	// the length of the strip lists: each strip adds two vertices to its
	// triangles (Gamebryo lineage name)
	unsigned short			GetStripLengthSum() const;

	unsigned short			GetStripCount() const { return stripCount; }
	unsigned short *		GetStripLengths() const { return stripLengths; }
	unsigned short *		GetStripLists() const { return stripLists; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriStripsData *dest );

	unsigned short			stripCount;
	unsigned short *		stripLengths;	// stripCount lengths, in vertices
	unsigned short *		stripLists;		// the strips' vertex indices, one after another
};

NiRegisterStream( NiTriStripsData );

#endif /* !__NITRISTRIPSDATA_H__ */
