/*
===============================================================================

	NiLinesData.h

	Vertex data drawn as lines through the vertices, each vertex flagged
	whether it connects to the next.

	Evidence: the NiRTTI (0x9A2100, initialised at 0x7FE1F0) is named
	"NiLinesData" with NiGeometryData's as its base; CreateObject
	(0x7FE4C0) allocates 0x38 bytes, runs NiGeometryData's constructor,
	stores the vtable 0x899D40 (16 slots; the load record's follows) and
	clears the pointer at 0x34. Only slot 14 is overridden past the
	streaming slots. The renderer's slot 50 tests the bytes the pointer at
	0x34 points at. Member name from the Gamebryo lineage (m_pkFlags).

===============================================================================
*/

#ifndef __NILINESDATA_H__
#define __NILINESDATA_H__

#include "NiGeometryData.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiLinesDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x899D80). The class name is invented.
================
*/
// FUNCTION: STBC 0x007FE560 ??_GNiLinesDataLoadRecord@@UAEPAXI@Z
class NiLinesDataLoadRecord : public NiObjectLoadRecord {
};

class NiLinesData : public NiGeometryData {
public:
	// FUNCTION: STBC 0x7FE900 ?GetRTTI@NiLinesData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiLinesData();

	virtual void			CalculateNormals();

	// without flags, every other vertex starts a line to the next
							NiLinesData( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_, NiPoint2 *textures_, bool *flags_ );
							NiLinesData();

	bool *					GetFlags() const;

	// replaces the flags, freeing the old ones
	void					Replace( bool *flags_ );
	// takes the flags without freeing the old ones (name invented)
	void					SetData( bool *flags_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiLinesData *dest );

	bool *					flags;		// per vertex: connected to the next
};

NiRegisterStream( NiLinesData );

/*
================
NiLinesData::GetFlags
================
*/
inline bool *NiLinesData::GetFlags() const {
	return flags;
}

#endif /* !__NILINESDATA_H__ */
