/*
===============================================================================

	NiExtraData.h

	A block of application data attached to an NiObjectNET. The blocks of
	one object form a singly linked list.

	Evidence: the vtable at 0x898BC4 (12 slots); the constructor (0x7D91C0)
	and destructor (0x7D9230); the viewer strings name the size
	("m_uiSize"); NiObjectNET walks the list through the pointer at 0x10.

===============================================================================
*/

#ifndef __NIEXTRADATA_H__
#define __NIEXTRADATA_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiExtraDataLoadRecord

The base of the extra-data classes' records. NiObject::LoadBinary keeps the
object's link ID in it, then NiExtraData::LoadBinary (0x7D9510) replaces it
with the next block's, which LinkObject (0x7D9580) resolves. Unlike
NiObjectLoadRecord it leaves the link ID unset at construction and clears it
at destruction (the records' destructors 0x808650, 0x796FD0 and 0x8103D0).
The class name is invented.
================
*/
// FUNCTION: STBC 0x007D9990 ??_GNiExtraDataLoadRecord@@UAEPAXI@Z
class NiExtraDataLoadRecord : public NiLoadRecord {
public:
	virtual				~NiExtraDataLoadRecord() { linkID = 0; }
};

// FUNCTION: STBC 0x007d9970 ??_GNiExtraData@@UAEPAXI@Z @netimmerse/NiExtraData
class NiExtraData : public NiObject {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7D9070)
	static int				Register();
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// FUNCTION: STBC 0x7D9960 ?GetRTTI@NiExtraData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiExtraData();

							NiExtraData();

	// the owner's list
	NiExtraData *			GetNext() const { return next; }
	// the blocks from this one to the end of the list (name invented, after
	// NiTSimpleList::ItemsInList)
	unsigned int			Count() const;
	void					SetNext( NiExtraData *next_ ) { next = next_; }
	// the block's bytes (Gamebryo 1.1.2's names, commented out in
	// NIEXTRADATA.H; AnimOnly.exe 0x47E9B0, 0x47E9D0)
	unsigned int			GetSize() const { return size; }
	char *					GetChunk() const { return data; }

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiExtraData *dest );

public:
	unsigned int			size;
	char *					data;		// size bytes, owned
	NiPointer<NiExtraData>	next;		// the next block in the owner's list
};

NiRegisterStream( NiExtraData );

#endif /* !__NIEXTRADATA_H__ */
