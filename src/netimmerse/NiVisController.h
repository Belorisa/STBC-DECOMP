/*
===============================================================================

	NiVisController.h

	Shows and hides its target, an NiAVObject, by the keys of an NiVisData.

	Evidence: the vtable at 0x898880 (NiTimeController's 20 slots, none
	added); the constructor (0x7B64A0) takes the data into the smart
	pointer at 0x30 and clears the key index at 0x34; CreateClone (0x7B6700)
	allocates 0x38 bytes; Update (0x7B6690) culls the target through
	NiAVObject::SetAppCulled; slot 19 (0x7B6C90) tests the target against
	NiAVObject's NiRTTI. The NiRTTI is 0x9A0890, named "NiVisController".

===============================================================================
*/

#ifndef __NIVISCONTROLLER_H__
#define __NIVISCONTROLLER_H__

#include "NiTimeController.h"
#include "NiVisData.h"
#include "NiStreamMacros.h"

/*
================
NiVisControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs and
the data's, under a vtable of its own (0x8988D0). The class name is
invented.
================
*/
// FUNCTION: STBC 0x007b6c40 ??_GNiVisControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiVisController
class NiVisControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiVisControllerLoadRecord() { dataLinkID = 0; }

	unsigned int			dataLinkID;
};

// FUNCTION: STBC 0x007b6c20 ??_GNiVisController@@UAEPAXI@Z @netimmerse/NiVisController
class NiVisController : public NiTimeController {
public:
	// FUNCTION: STBC 0x7B6C10 ?GetRTTI@NiVisController@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiVisController();

	virtual void			Update( float time );
	virtual bool			TargetIsRequiredType() const;

							NiVisController( NiVisData *data_ = 0 );

	NiVisData *				GetVisData() const { return data; }	// Gamebryo lineage name
	void					SetVisData( NiVisData *data_ ) { data = data_; }	// Gamebryo lineage name
	// the data's keys and their count; none without data (Gamebryo lineage
	// name)
	NiVisKey **				GetVisData( unsigned int &numKeys ) const {
								if ( data ) {
									return data->GetAnim( numKeys );
								}
								numKeys = 0;
								return 0;
							}

	// the key times bound the controller's time (Gamebryo lineage name)
	void					ResetTimeExtrema();

	// gives the data new keys, creating the data first (name invented)
	void					ReplaceKeys( NiVisKey **keys, unsigned int numKeys, bool resetTimes );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// the first NiVisController of the target's list (name invented)
	static NiVisController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiVisController *dest );

	NiPointer<NiVisData>	data;
	unsigned int			lastIndex;	// the key the last update found
};

NiRegisterStream( NiVisController );

#endif /* !__NIVISCONTROLLER_H__ */
