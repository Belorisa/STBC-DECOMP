/*
===============================================================================

	NiFloatData.h

	The keys of a float animation and their interpolation type, shared by
	the NiFloatControllers that play them.

	Evidence: the vtable at 0x898250 (NiObject's 12 slots, none added); the
	creation function (0x795250) allocates 0x14 bytes and clears the count
	at 8, the array at 0xC and the key type at 0x10; the viewer strings
	name the count ("m_uiNumKeys"). The NiRTTI is 0x9A0110, named
	"NiFloatData".

===============================================================================
*/

#ifndef __NIFLOATDATA_H__
#define __NIFLOATDATA_H__

#include "NiObject.h"
#include "NiStream.h"
#include "NiFloatKey.h"
#include "NiStreamMacros.h"

/*
================
NiFloatDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898280). The class name is invented.
================
*/
// FUNCTION: STBC 0x007952f0 ??_GNiFloatDataLoadRecord@@UAEPAXI@Z @netimmerse/NiFloatData
class NiFloatDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x00794f10 ??_GNiFloatData@@UAEPAXI@Z @netimmerse/NiFloatController
class NiFloatData : public NiObject {
public:
	// FUNCTION: STBC 0x794F00 ?GetRTTI@NiFloatData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning: the object itself, as NiObject's; the class keeps its own
	// copy (AnimOnly.exe 0x479817)
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiFloatData();

							NiFloatData() { numKeys = 0; keys = 0; keyType = NiFloatKey::NOINTERP; }

	// the keys, their count and type (Gamebryo lineage name)
	NiFloatKey **			GetAnim( unsigned int &numKeys_, NiFloatKey::KeyType &keyType_ ) const {
								numKeys_ = numKeys;
								keyType_ = keyType;
								return keys;
							}

	// takes ownership of the keys, deleting the old ones (Gamebryo lineage name)
	void					ReplaceAnim( NiFloatKey **keys_, unsigned int numKeys_, NiFloatKey::KeyType keyType_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	unsigned int			numKeys;
	NiFloatKey **			keys;		// numKeys keys, each allocated on its own
	NiFloatKey::KeyType		keyType;
};

NiRegisterStream( NiFloatData );

#endif /* !__NIFLOATDATA_H__ */
