/*
===============================================================================

	NiColorData.h

	The keys of a colour animation and their interpolation type, shared by
	the controllers that play them.

	Evidence: the vtable at 0x898314 (NiObject's 12 slots, none added); the
	creation function (0x79DA20) allocates 0x14 bytes and clears the count
	at 8, the array at 0xC and the key type at 0x10; the destructor
	(0x79D9B0) deletes every key, then the array. The NiRTTI is 0x9A02E0,
	named "NiColorData".

===============================================================================
*/

#ifndef __NICOLORDATA_H__
#define __NICOLORDATA_H__

#include "NiObject.h"
#include "NiStream.h"
#include "NiColorKey.h"
#include "NiStreamMacros.h"

/*
================
NiColorDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898344). The class name is invented.
================
*/
// FUNCTION: STBC 0x0079dac0 ??_GNiColorDataLoadRecord@@UAEPAXI@Z @netimmerse/NiColorData
class NiColorDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x0079df90 ??_GNiColorData@@UAEPAXI@Z @netimmerse/NiColorData
class NiColorData : public NiObject {
public:
	// FUNCTION: STBC 0x79DF80 ?GetRTTI@NiColorData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning: the object itself, as NiObject's; the class keeps its own
	// copy (AnimOnly.exe 0x4880D0)
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiColorData();

							NiColorData() { numKeys = 0; keys = 0; keyType = NiColorKey::NOINTERP; }

	// the keys, their count and type (Gamebryo lineage name)
	NiColorKey **			GetAnim( unsigned int &numKeys_, NiColorKey::KeyType &keyType_ ) const {
								numKeys_ = numKeys;
								keyType_ = keyType;
								return keys;
							}
	unsigned int			GetNumKeys() const { return numKeys; }

	// takes ownership of the keys; the old ones are not deleted (name
	// invented)
	void					SetAnim( NiColorKey **keys_, unsigned int numKeys_, NiColorKey::KeyType keyType_ ) {
								if ( keys_ && numKeys_ ) {
									numKeys = numKeys_;
									keys = keys_;
									keyType = keyType_;
								} else {
									numKeys = 0;
									keys = 0;
									keyType = NiColorKey::NOINTERP;
								}
							}

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	unsigned int			numKeys;
	NiColorKey **			keys;		// numKeys keys, each allocated on its own
	NiColorKey::KeyType		keyType;
};

NiRegisterStream( NiColorData );

#endif /* !__NICOLORDATA_H__ */
