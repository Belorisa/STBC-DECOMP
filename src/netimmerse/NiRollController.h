/*
===============================================================================

	NiRollController.h

	Rolls its target, an NiAVObject, about its z axis by the keys' angle.

	Evidence: the vtable at 0x8987F4 (NiTimeController's 20 slots, none
	added); CreateClone (0x7B3F90) allocates 0x38 bytes, NiFloatController's
	size; Update (0x7B3EB0) multiplies the target's local rotation by a z
	rotation of the negated value; slot 19 (0x7B4470) tests the target
	against NiAVObject's NiRTTI. The NiRTTI is 0x9A07D8, named
	"NiRollController".

===============================================================================
*/

#ifndef __NIROLLCONTROLLER_H__
#define __NIROLLCONTROLLER_H__

#include "NiFloatController.h"
#include "NiAVObject.h"
#include "NiStreamMacros.h"

/*
================
NiRollControllerLoadRecord

The record CreateObject hands the stream, under a vtable of its own
(0x898844). The class name is invented.
================
*/
// FUNCTION: STBC 0x007b40c0 ??_GNiRollControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiRollController
class NiRollControllerLoadRecord : public NiFloatControllerLoadRecord {
};

// FUNCTION: STBC 0x007b4420 ??_GNiRollController@@UAEPAXI@Z @netimmerse/NiRollController
class NiRollController : public NiFloatController {
public:
	// FUNCTION: STBC 0x7B4410 ?GetRTTI@NiRollController@@UBEPBVNiRTTI@@XZ
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

	virtual void			Update( float time );
	// FUNCTION: STBC 0x7B4470 ?TargetIsRequiredType@NiRollController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiAVObject, target );
							}

							// the data defaults to none (Gamebryo lineage; CreateClone
							// passes the default, AnimOnly.exe 0x4A5498)
							NiRollController( NiFloatData *data_ = 0 ) : NiFloatController( data_ ) {}

	// the first NiRollController of the target's list (name invented)
	static NiRollController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiRollController *dest );
};

NiRegisterStream( NiRollController );

#endif /* !__NIROLLCONTROLLER_H__ */
