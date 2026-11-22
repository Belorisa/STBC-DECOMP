/*
===============================================================================

	NiVisData.h

	The keys of a visibility animation, shared by the NiVisControllers that
	play them.

	Evidence: the vtable at 0x895B5C (NiObject's 12 slots, none added); the
	creation function (0x7B5DB0) allocates 0x10 bytes and clears the count
	at 8 and the array at 0xC; the viewer strings name the count
	("m_uiNumVisKeys"); the destructor (0x7B5CF0) deletes every key, then
	the array. The NiRTTI is 0x9A0858, named "NiVisData".

===============================================================================
*/

#ifndef __NIVISDATA_H__
#define __NIVISDATA_H__

#include "NiObject.h"
#include "NiStream.h"
#include "NiVisKey.h"
#include "NiStreamMacros.h"

/*
================
NiVisDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x89887C). The class name is invented.
================
*/
// FUNCTION: STBC 0x007b5e50 ??_GNiVisDataLoadRecord@@UAEPAXI@Z @netimmerse/NiVisData
class NiVisDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x006cc860 ??_GNiVisData@@UAEPAXI@Z @netimmerse/NiVisController
class NiVisData : public NiObject {
public:
	// FUNCTION: STBC 0x6CC850 ?GetRTTI@NiVisData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning: the object itself, as NiObject's; the class keeps its own
	// copy (AnimOnly.exe 0x4A737F)
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiVisData();

							NiVisData() { numKeys = 0; keys = 0; }

	// the keys and their count (Gamebryo lineage name)
	NiVisKey **				GetAnim( unsigned int &numKeys_ ) const { numKeys_ = numKeys; return keys; }

	unsigned int			GetNumVisKeys() const { return numKeys; }	// Gamebryo lineage name

	// takes ownership of the keys, deleting the old ones (Gamebryo lineage name)
	void					ReplaceAnim( NiVisKey **keys_, unsigned int numKeys_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	unsigned int			numKeys;
	NiVisKey **				keys;		// numKeys keys, each allocated on its own
};

NiRegisterStream( NiVisData );

#endif /* !__NIVISDATA_H__ */
