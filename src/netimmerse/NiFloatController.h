/*
===============================================================================

	NiFloatController.h

	The base of the controllers that animate one float of their target by
	the keys of an NiFloatData.

	Evidence: the vtable at 0x88D068 (NiTimeController's 20 slots, none
	added; Update and slot 19 stay _purecall); the destructor (0x7949B0)
	releases the smart pointer at 0x34; the derived constructors (inlined in
	NiAlphaController::CreateClone, 0x795A50) clear the data at 0x34, then
	the key index at 0x30. The NiRTTI is 0x9A00F0, named
	"NiFloatController".

===============================================================================
*/

#ifndef __NIFLOATCONTROLLER_H__
#define __NIFLOATCONTROLLER_H__

#include "NiTimeController.h"
#include "NiFloatData.h"
#include "NiStreamMacros.h"

/*
================
NiFloatControllerLoadRecord

NiTimeController's link IDs and the data's. The class name is invented;
NiFloatController is abstract, so only the derived records have vtables.
================
*/
class NiFloatControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiFloatControllerLoadRecord() { dataLinkID = 0; }

	unsigned int			dataLinkID;
};

// FUNCTION: STBC 0x004db140 ??_GNiFloatController@@UAEPAXI@Z @netimmerse/NiFloatController
class NiFloatController : public NiTimeController {
public:
	// FUNCTION: STBC 0x4DB130 ?GetRTTI@NiFloatController@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiFloatController();

							// FUNCTION: STBC 0x004db100 ??0NiFloatController@@QAE@PAVNiFloatData@@@Z @game/BlinkingLightUnit
							NiFloatController( NiFloatData *data_ = 0 ) : data( data_ ) { lastIndex = 0; }

	// the data's keys, their count and type; none without data (Gamebryo
	// lineage name)
	NiFloatKey **			GetAnim( unsigned int &numKeys, NiFloatKey::KeyType &keyType ) const;

	// gives the data new keys, creating the data first; the key times
	// become those of the first and last keys (Gamebryo lineage name; the
	// count comes first, as the game's callers 0x4DB0A7 and 0x6625D2 pass it)
	void					ReplaceAnim( unsigned int numKeys, NiFloatKey **keys, NiFloatKey::KeyType keyType );

	// the keys' owner (Gamebryo lineage names; AnimOnly.exe 0x47A770)
	void					SetFloatData( NiFloatData *data_ ) { data = data_; }
	NiFloatData *			GetFloatData() { return data; }

	// the first NiFloatController of the target's list (name invented)
	static NiFloatController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiFloatController *dest );

	unsigned int			lastIndex;	// the key the last update found
	NiPointer<NiFloatData>	data;
};

NiRegisterStream( NiFloatController );

#endif /* !__NIFLOATCONTROLLER_H__ */
