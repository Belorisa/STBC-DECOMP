/*
===============================================================================

	NiCloneExtraData.h

	The extra data block that cloning attaches to an original object: it
	records the object's clone while a clone is being built, and the
	original drops it again afterwards.

	Evidence: NiObjectNET's member copy (0x7DAE10) allocates 0x18 bytes,
	runs NiExtraData's constructor, stores vtable 0x898C78 and the clone at
	0x14, and adds the block to the original; NiObjectNET's slot 3 (0x7DB080)
	removes a block whose RTTI is this class's ("NiCloneExtraData", 0x9A2CC8).
	The vtable overrides only GetRTTI and the destructor.

===============================================================================
*/

#ifndef __NICLONEEXTRADATA_H__
#define __NICLONEEXTRADATA_H__

#include "NiExtraData.h"

// FUNCTION: STBC 0x007dba90 ??_GNiCloneExtraData@@UAEPAXI@Z @netimmerse/NiAmbientLight
class NiCloneExtraData : public NiExtraData {
public:
	// FUNCTION: STBC 0x7DBA80 ?GetRTTI@NiCloneExtraData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

							NiCloneExtraData( NiObject *clone_ ) { clone = clone_; }
	virtual					~NiCloneExtraData() {}

	// the clone (name invented; AnimOnly.exe 0x413120)
	NiObject *				GetClone() const { return clone; }

	static const NiRTTI		rtti;

	NiObject *				clone;		// the clone of the object that holds this block
};

#endif /* !__NICLONEEXTRADATA_H__ */
