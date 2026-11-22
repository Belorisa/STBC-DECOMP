/*
===============================================================================

	NiSequenceStreamHelper.h

	An object with nothing of its own beyond NiObjectNET's names, extra data
	and controllers: the root a keyframe sequence file is saved under.

	Evidence: the static initializer at 0x7B44A0 builds the NiRTTI at
	0x9A0808 ("NiSequenceStreamHelper", based on NiObjectNET's); the vtable
	at 0x89884C has NiObject's 12 slots and replaces GetRTTI, the five
	streaming slots and the destructor; the creation function (0x7B4650)
	allocates 0x14 bytes, NiObjectNET's size. The name and role follow the
	Gamebryo lineage.

===============================================================================
*/

#ifndef __NISEQUENCESTREAMHELPER_H__
#define __NISEQUENCESTREAMHELPER_H__

#include "NiObjectNET.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiSequenceStreamHelperLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x898848). The class name is invented.
================
*/
// FUNCTION: STBC 0x7B46F0 ??_GNiSequenceStreamHelperLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7B4710 ??1NiSequenceStreamHelperLoadRecord@@UAE@XZ
class NiSequenceStreamHelperLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x7B48B0 ??_GNiSequenceStreamHelper@@UAEPAXI@Z
// FUNCTION: STBC 0x7B48D0 ??1NiSequenceStreamHelper@@UAE@XZ
class NiSequenceStreamHelper : public NiObjectNET {
public:
	// FUNCTION: STBC 0x7B48A0 ?GetRTTI@NiSequenceStreamHelper@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;
};

NiRegisterStream( NiSequenceStreamHelper );

#endif /* !__NISEQUENCESTREAMHELPER_H__ */
