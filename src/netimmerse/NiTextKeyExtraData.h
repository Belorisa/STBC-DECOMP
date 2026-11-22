/*
===============================================================================

	NiTextKeyExtraData.h

	Extra data holding an animation's text keys: strings at times that mark
	its events.

	Evidence: the NiRTTI (0x9A01C0) is named "NiTextKeyExtraData" with
	NiExtraData's as its base; the vtable at 0x898288 replaces GetRTTI,
	CreateClone, the six streaming slots and the destructor; CreateClone
	(0x796DA0) allocates 0x1C bytes and clears the count at 0x14 and the
	array at 0x18; the destructor (0x797530) destroys the array of 8-byte
	NiTextKeys; the viewer strings name the count ("m_uiNumKeys"). The
	names follow the Gamebryo lineage (m_uiNumKeys, m_pKeys).

===============================================================================
*/

#ifndef __NITEXTKEYEXTRADATA_H__
#define __NITEXTKEYEXTRADATA_H__

#include "NiExtraData.h"
#include "NiStream.h"
#include "NiTextKey.h"
#include "NiStreamMacros.h"

/*
================
NiTextKeyExtraDataLoadRecord

The record CreateObject hands the stream, under a vtable of its own
(0x8982B8). The class name is invented.
================
*/
// FUNCTION: STBC 0x796FB0 ??_GNiTextKeyExtraDataLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x796FD0 ??1NiTextKeyExtraDataLoadRecord@@UAE@XZ
class NiTextKeyExtraDataLoadRecord : public NiExtraDataLoadRecord {
};

// FUNCTION: STBC 0x7974F0 ??_GNiTextKeyExtraData@@UAEPAXI@Z
class NiTextKeyExtraData : public NiExtraData {
public:
	// FUNCTION: STBC 0x7974E0 ?GetRTTI@NiTextKeyExtraData@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x797530 ??1NiTextKeyExtraData@@UAE@XZ
	virtual					~NiTextKeyExtraData() { delete [] keys; }

							NiTextKeyExtraData() { numKeys = 0; keys = 0; }

	// the keys and their count (Gamebryo lineage names; AnimOnly.exe
	// 0x47EC20)
	void					SetKeys( NiTextKey *keys_, unsigned int numKeys_ ) { numKeys = numKeys_; keys = keys_; }
	NiTextKey *				GetKeys( unsigned int &numKeys_ ) const { numKeys_ = numKeys; return keys; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTextKeyExtraData *dest );

	unsigned int			numKeys;
	NiTextKey *				keys;		// numKeys keys, owned
};

NiRegisterStream( NiTextKeyExtraData );

#endif /* !__NITEXTKEYEXTRADATA_H__ */
