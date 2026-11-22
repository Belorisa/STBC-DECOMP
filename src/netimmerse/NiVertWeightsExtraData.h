/*
===============================================================================

	NiVertWeightsExtraData.h

	Extra data holding one weight per vertex.

	Evidence: the vtable at 0x89A6F0 (12 slots, NiExtraData's with GetRTTI,
	CreateClone, the six streaming slots and the destructor replaced); the
	constructor (0x8100F0) clears the count at 0x14 (16 bits) and the float
	array at 0x18, then copies its arguments' array (0x8101A0); the
	destructor (0x810700) frees the array; the NiRTTI (0x9A2780) is named
	"NiVertWeightsExtraData" with NiExtraData's as its base; CreateClone
	allocates 0x1C bytes. The size at 0x08 is kept as the array's size plus
	two, its size in a file (the count, then the weights). The names follow
	the Gamebryo lineage (m_usVerts, m_pfWeights); SetData is invented.

===============================================================================
*/

#ifndef __NIVERTWEIGHTSEXTRADATA_H__
#define __NIVERTWEIGHTSEXTRADATA_H__

#include "NiExtraData.h"
#include "NiStreamMacros.h"

/*
================
NiVertWeightsExtraDataLoadRecord

The record CreateObject hands the stream, under a vtable of its own
(0x89A720). The class name is invented.
================
*/
// FUNCTION: STBC 0x8103B0 ??_GNiVertWeightsExtraDataLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x8103D0 ??1NiVertWeightsExtraDataLoadRecord@@UAE@XZ
class NiVertWeightsExtraDataLoadRecord : public NiExtraDataLoadRecord {
};

// FUNCTION: STBC 0x008106D0 ??_GNiVertWeightsExtraData@@UAEPAXI@Z
class NiVertWeightsExtraData : public NiExtraData {
public:
	// FUNCTION: STBC 0x8106C0 ?GetRTTI@NiVertWeightsExtraData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiVertWeightsExtraData();

							NiVertWeightsExtraData( unsigned short vertexCount_, float *weights_ );
							NiVertWeightsExtraData() { weights = 0; vertexCount = 0; }

	unsigned short			GetVertexCount() const { return vertexCount; }
	float *					GetWeights() const { return weights; }
	// copies the weights; no weights or no count clears them
	void					SetData( unsigned short vertexCount_, float *weights_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiVertWeightsExtraData *dest );

	unsigned short			vertexCount;
	float *					weights;		// heap array of vertexCount, owned
};

NiRegisterStream( NiVertWeightsExtraData );

#endif /* !__NIVERTWEIGHTSEXTRADATA_H__ */
