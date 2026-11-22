/*
===============================================================================

	NiStringExtraData.h

	Extra data holding one string.

	Evidence: the vtable at 0x89A1B0 (12 slots, NiExtraData's with GetRTTI,
	CreateClone, the six streaming slots and the destructor replaced); the
	constructor (0x808340) clears the string at 0x14 and sets it from its
	argument; the destructor (0x808A00) frees it; the NiRTTI (0x9A2458) is
	named "NiStringExtraData" with NiExtraData's as its base; CreateClone
	allocates 0x18 bytes; the viewer strings name the member ("m_pString").
	The size at 0x08 is kept as the string's length plus four, its size in a
	file (a length, then the characters). The names follow the Gamebryo
	lineage (m_pString, SetValue).

===============================================================================
*/

#ifndef __NISTRINGEXTRADATA_H__
#define __NISTRINGEXTRADATA_H__

#include "NiExtraData.h"
#include "NiStreamMacros.h"

/*
================
NiStringExtraDataLoadRecord

The record CreateObject hands the stream, under a vtable of its own
(0x89A1E0). The class name is invented.
================
*/
// FUNCTION: STBC 0x808630 ??_GNiStringExtraDataLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x808650 ??1NiStringExtraDataLoadRecord@@UAE@XZ
class NiStringExtraDataLoadRecord : public NiExtraDataLoadRecord {
};

// FUNCTION: STBC 0x008089D0 ??_GNiStringExtraData@@UAEPAXI@Z
class NiStringExtraData : public NiExtraData {
public:
	// FUNCTION: STBC 0x8089C0 ?GetRTTI@NiStringExtraData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiStringExtraData();

							NiStringExtraData( const char *string_ );
							NiStringExtraData() { string = 0; }

	const char *			GetValue() const { return string; }
	// copies the string; NULL clears it
	void					SetValue( const char *string_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiStringExtraData *dest );

	char *					string;		// heap copy, owned
};

NiRegisterStream( NiStringExtraData );

#endif /* !__NISTRINGEXTRADATA_H__ */
