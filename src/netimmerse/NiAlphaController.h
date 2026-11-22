/*
===============================================================================

	NiAlphaController.h

	Animates the alpha of its target, an NiMaterialProperty.

	Evidence: the vtable at 0x88D018 (NiTimeController's 20 slots, none
	added); CreateClone (0x795A50) allocates 0x38 bytes, NiFloatController's
	size; Update (0x7959D0) stores the interpolated value at the material's
	alpha (0x50); slot 19 (0x4DB1A0, an inline copy in the game code) tests
	the target against NiMaterialProperty's NiRTTI. The NiRTTI is 0x9A0138,
	named "NiAlphaController".

===============================================================================
*/

#ifndef __NIALPHACONTROLLER_H__
#define __NIALPHACONTROLLER_H__

#include "NiFloatController.h"
#include "NiMaterialProperty.h"
#include "NiStreamMacros.h"

/*
================
NiAlphaControllerLoadRecord

The record CreateObject hands the stream, under a vtable of its own
(0x898284). The class name is invented.
================
*/
// FUNCTION: STBC 0x00795b80 ??_GNiAlphaControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiAlphaController
class NiAlphaControllerLoadRecord : public NiFloatControllerLoadRecord {
};

// FUNCTION: STBC 0x004db170 ??_GNiAlphaController@@UAEPAXI@Z @netimmerse/NiAlphaController
class NiAlphaController : public NiFloatController {
public:
	// FUNCTION: STBC 0x4DB160 ?GetRTTI@NiAlphaController@@UBEPBVNiRTTI@@XZ
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
	// FUNCTION: STBC 0x4DB1A0 ?TargetIsRequiredType@NiAlphaController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiMaterialProperty, target );
							}

							// the data defaults to none (Gamebryo lineage; CreateClone
							// passes the default, AnimOnly.exe 0x47A9E7)
							NiAlphaController( NiFloatData *data_ = 0 ) : NiFloatController( data_ ) {}

	// the first NiAlphaController of the target's list (name invented)
	static NiAlphaController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAlphaController *dest );
};

NiRegisterStream( NiAlphaController );

#endif /* !__NIALPHACONTROLLER_H__ */
