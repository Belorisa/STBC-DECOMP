/*
===============================================================================

	NiObjectNET.h

	An NiObject with a name, a list of extra data blocks and a list of time
	controllers.

	Evidence: the vtable at 0x898C48 (12 slots); the constructor (0x7DAC80)
	and destructor (0x7DAD10); the viewer strings name the name member
	("m_pName"); the list functions walk NiExtraData through its pointer at
	0x10 and NiTimeController through its pointer at 0x2C.

===============================================================================
*/

#ifndef __NIOBJECTNET_H__
#define __NIOBJECTNET_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiExtraData.h"
#include "NiTimeController.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiObjectNETLoadRecord

The link IDs of the heads of the extra data and controller lists, read by
LoadBinary (0x7DB5F0) and resolved by LinkObject (0x7DB630). The class name
is invented. The copies its vtable (0x88B918) keeps come from
TGFuzzyTriShape's unit.
================
*/
// FUNCTION: STBC 0x456E00 ??_GNiObjectNETLoadRecord@@UAEPAXI@Z @game/TGFuzzyTriShape
// FUNCTION: STBC 0x456E20 ??1NiObjectNETLoadRecord@@UAE@XZ @game/TGFuzzyTriShape
class NiObjectNETLoadRecord : public NiObjectLoadRecord {
public:
						// FUNCTION: STBC 0x7F7C50 ??0NiObjectNETLoadRecord@@QAE@XZ @netimmerse/NiBone
						NiObjectNETLoadRecord() { extraLinkID = 0; controllersLinkID = 0; }

	unsigned int		extraLinkID;
	unsigned int		controllersLinkID;
};

// FUNCTION: STBC 0x007dba50 ??_GNiObjectNET@@UAEPAXI@Z @netimmerse/NiObjectNET
class NiObjectNET : public NiObject {
public:
	// FUNCTION: STBC 0x7DBA40 ?GetRTTI@NiObjectNET@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );
	virtual void			VirtualSlot03();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiObjectNET();

							NiObjectNET();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// name
	void					SetName( const char *name_ );
	const char *			GetName() const;

	// extra data
	void					AddExtraData( NiExtraData *extraData );
	void					SetExtraData( NiExtraData *extraData );
	NiExtraData *			GetExtraData() const;
	void					RemoveExtraData( NiExtraData *extraData );
	void					RemoveAllExtraData();

	// time controllers
	void					PrependController( NiTimeController *controller );
	void					SetControllers( NiTimeController *controller );
	NiTimeController *		GetControllers() const;
	void					RemoveController( NiTimeController *controller );
	void					RemoveAllControllers();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiObjectNET *dest );

	char *					name;
	NiPointer<NiExtraData>	extra;		// the head of the extra data list
	NiPointer<NiTimeController>	controllers;	// the head of the controller list
};

NiRegisterStream( NiObjectNET );

#endif /* !__NIOBJECTNET_H__ */
