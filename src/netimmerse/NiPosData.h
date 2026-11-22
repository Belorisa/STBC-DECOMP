/*
===============================================================================

	NiPosData.h

	The keys of a position animation and their interpolation type, shared
	by the controllers that play them.

	Evidence: the vtable at 0x898524 (NiObject's 12 slots, none added); the
	creation function (0x7A5EA0) allocates 0x14 bytes and clears the count
	at 8, the array at 0xC and the key type at 0x10; the destructor
	(0x7A5E30) deletes every key, then the array. The NiRTTI is 0x9A0438,
	named "NiPosData".

===============================================================================
*/

#ifndef __NIPOSDATA_H__
#define __NIPOSDATA_H__

#include "NiObject.h"
#include "NiStream.h"
#include "NiPosKey.h"
#include "NiStreamMacros.h"

/*
================
NiPosDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898554). The class name is invented.
================
*/
// FUNCTION: STBC 0x007a5f40 ??_GNiPosDataLoadRecord@@UAEPAXI@Z @netimmerse/NiPosData
class NiPosDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x007a6450 ??_GNiPosData@@UAEPAXI@Z @netimmerse/NiLightColorController
class NiPosData : public NiObject {
public:
	// FUNCTION: STBC 0x7A6440 ?GetRTTI@NiPosData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning: the object itself, as NiObject's; the class keeps its own
	// copy (AnimOnly.exe 0x4932E0)
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiPosData();

							NiPosData() { numKeys = 0; keys = 0; keyType = NiPosKey::NOINTERP; }

	unsigned int			GetNumKeys() const { return numKeys; }

	// the keys, their count and type (Gamebryo lineage name)
	// FUNCTION: STBC 0x7AE860 ?GetAnim@NiPosData@@QBEPAPAVNiPosKey@@AAIAAW4KeyType@2@@Z @netimmerse/NiPathController
	NiPosKey **			GetAnim( unsigned int &numKeys_, NiPosKey::KeyType &keyType_ ) const {
								numKeys_ = numKeys;
								keyType_ = keyType;
								return keys;
							}

	// takes ownership of the keys, deleting the old ones; without keys the
	// data is left empty and the old keys are kept allocated (Gamebryo
	// lineage name)
	void					ReplaceAnim( NiPosKey **keys_, unsigned int numKeys_, NiPosKey::KeyType keyType_ ) {
								if ( !keys_ || !numKeys_ ) {
									numKeys = 0;
									keys = 0;
									keyType = NiPosKey::NOINTERP;
									return;
								}

								for ( unsigned int i = 0; i < numKeys; i++ ) {
									delete keys[i];
								}
								delete [] keys;

								numKeys = numKeys_;
								keys = keys_;
								keyType = keyType_;
							}

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	unsigned int			numKeys;
	NiPosKey **			keys;		// numKeys keys, each allocated on its own
	NiPosKey::KeyType		keyType;
};

NiRegisterStream( NiPosData );

#endif /* !__NIPOSDATA_H__ */
