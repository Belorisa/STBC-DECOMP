/*
===============================================================================

	NiBezierSkinController.h

	Skins the control vertices of an NiBezierMesh.

	Evidence: the NiRTTI (0x9B2FE8, built at 0x834C20) is named
	"NiBezierSkinController" with NiSkinController's as its base; the
	vtable at 0x89A960 has NiSkinController's 24 slots and adds none (the
	load record's vtable, 0x89A9C0, follows it). The creation function the
	stream registers (0x834EC0) allocates 0x48 bytes, NiSkinController's
	size, and a 0x14-byte record. Slot 22 (0x834DB0) reads the target's
	vertex count at 0xE8; slot 19 (0x835180) tests the target against
	NiBezierMesh's NiRTTI.

===============================================================================
*/

#ifndef __NIBEZIERSKINCONTROLLER_H__
#define __NIBEZIERSKINCONTROLLER_H__

#include "NiSkinController.h"
#include "NiBezierMesh.h"
#include "NiStreamMacros.h"

/*
================
NiBezierSkinControllerLoadRecord

The record CreateObject hands the stream: NiSkinController's, under a
vtable of its own (0x89A9C0). The class name is invented.
================
*/
// FUNCTION: STBC 0x834F70 ??_GNiBezierSkinControllerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x834F90 ??1NiBezierSkinControllerLoadRecord@@UAE@XZ
class NiBezierSkinControllerLoadRecord : public NiSkinControllerLoadRecord {
};

// FUNCTION: STBC 0x834E40 ??_GNiBezierSkinController@@UAEPAXI@Z
// FUNCTION: STBC 0x834E60 ??1NiBezierSkinController@@UAE@XZ
class NiBezierSkinController : public NiSkinController {
public:
	// FUNCTION: STBC 0x835160 ?GetRTTI@NiBezierSkinController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual void			Update( float time );
	// FUNCTION: STBC 0x835180 ?TargetIsRequiredType@NiBezierSkinController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiBezierMesh, target );
							}

	// FUNCTION: STBC 0x834DB0 ?GetVertexCount@NiBezierSkinController@@UBEGXZ
	virtual unsigned short	GetVertexCount() const { return ( (NiBezierMesh *)target )->GetVertexCount(); }
	// a new default object, which NiSkinController::CreateClone fills
	// FUNCTION: STBC 0x834DC0 ?CreateNew@NiBezierSkinController@@UAEPAVNiSkinController@@XZ
	virtual NiSkinController *	CreateNew() { return new NiBezierSkinController; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;
};

NiRegisterStream( NiBezierSkinController );

#endif /* !__NIBEZIERSKINCONTROLLER_H__ */
