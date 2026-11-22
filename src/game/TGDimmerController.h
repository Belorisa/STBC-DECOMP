/*
===============================================================================

	TGDimmerController.h

	A float controller that drives a light's dimmer: BridgeObjectClass's red
	alert lights (0x662460) each get one, whose keys hold the light off, fade
	it in, hold it on and fade it out, looping.

	0x3C bytes. Evidence: the vtable at 0x88B7EC (NiTimeController's 20
	slots, none added); slot 0 returns the NiRTTI 0x9809B0, which the
	static initializer 0x455010 names "TGDimmerController" (base NiRTTI
	0x9A00F0, NiFloatController's). BridgeObjectClass's alert light set-up
	inlines the constructor: NiTimeController's constructor, NiFloatController's
	members (the data at 0x34, the last index at 0x30), the member at 0x38,
	then the vtable. BridgeObjectClass::Update reads the member at 0x38 as
	the blinking textures' alpha. The target must be an NiLight (slot 19,
	0x455940); Update sets its dimmer (0x455200). The destructor is the
	implicit one (0x455990 only jumps to NiFloatController's). The class
	registers its loader like the NetImmerse controllers (0x455060; the
	bridge unit's registration run calls it from 0x6606F0).

===============================================================================
*/

#ifndef __TGDIMMERCONTROLLER_H__
#define __TGDIMMERCONTROLLER_H__

#include "NiFloatController.h"
#include "NiLight.h"
#include "NiStreamMacros.h"

/*
================
TGDimmerControllerLoadRecord

The record CreateObject hands the stream, under a vtable of its own
(0x88B83C). The class name is invented.
================
*/
// FUNCTION: STBC 0x004553c0 ??_GTGDimmerControllerLoadRecord@@UAEPAXI@Z @game/TGDimmerController
// FUNCTION: STBC 0x004553e0 ??1TGDimmerControllerLoadRecord@@UAE@XZ @game/TGDimmerController
class TGDimmerControllerLoadRecord : public NiFloatControllerLoadRecord {
};

// FUNCTION: STBC 0x00455970 ??_GTGDimmerController@@UAEPAXI@Z @game/TGDimmerController
// FUNCTION: STBC 0x00455990 ??1TGDimmerController@@UAE@XZ @game/TGDimmerController
class TGDimmerController : public NiFloatController {
public:
	// FUNCTION: STBC 0x455930 ?GetRTTI@TGDimmerController@@UBEPBVNiRTTI@@XZ @game/TGDimmerController
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
	// FUNCTION: STBC 0x455940 ?TargetIsRequiredType@TGDimmerController@@UBE_NXZ @game/TGDimmerController
	virtual bool			TargetIsRequiredType() const {
								if ( !target ) {
									return false;
								}
								return target->IsKindOf( &NiLight::rtti );
							}

							TGDimmerController() : level( 0.0f ) {}

	// the dimmer's level the keys last gave (invented name)
	float					GetLevel() const { return level; }

	// the first TGDimmerController of the target's list (name invented)
	static TGDimmerController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( TGDimmerController *dest );

	float					level;		// 0x38 (invented name)
};

NiRegisterStream( TGDimmerController );

#endif /* !__TGDIMMERCONTROLLER_H__ */
