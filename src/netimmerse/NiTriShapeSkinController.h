/*
===============================================================================

	NiTriShapeSkinController.h

	Skins the vertices of an NiTriShape. Update only computes the bound of
	the bones' vertices and marks the deformation pending; slot 15 deforms
	the vertices when one is.

	Evidence: the NiRTTI (0x9A25F0, built at 0x80C920) is named
	"NiTriShapeSkinController" with NiSkinController's as its base; the
	vtable at 0x89A460 has NiSkinController's 24 slots and adds none (the
	load record's vtable, 0x89A4C0, follows it). CreateNew (0x80CB10) and
	the creation function the stream registers (0x80CCD0) allocate 0x4C
	bytes, one member past NiSkinController's 0x48, and a 0x14-byte record.
	Update (0x80CBC0) sets the byte at 0x48, slot 15 (0x80CC60) tests it,
	slot 17 (0x80CF70) clears it; slot 19 (0x80CF90) tests the target
	against NiTriShape's NiRTTI (0x9A1BB8).

===============================================================================
*/

#ifndef __NITRISHAPESKINCONTROLLER_H__
#define __NITRISHAPESKINCONTROLLER_H__

#include "NiSkinController.h"
#include "NiTriShape.h"
#include "NiStreamMacros.h"

/*
================
NiTriShapeSkinControllerLoadRecord

The record CreateObject hands the stream: NiSkinController's, under a
vtable of its own (0x89A4C0). The class name is invented.
================
*/
// FUNCTION: STBC 0x80CD80 ??_GNiTriShapeSkinControllerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x80CDA0 ??1NiTriShapeSkinControllerLoadRecord@@UAE@XZ
class NiTriShapeSkinControllerLoadRecord : public NiSkinControllerLoadRecord {
};

// FUNCTION: STBC 0x80CB90 ??_GNiTriShapeSkinController@@UAEPAXI@Z
// FUNCTION: STBC 0x80CBB0 ??1NiTriShapeSkinController@@UAE@XZ
class NiTriShapeSkinController : public NiSkinController {
public:
	// flags, past NiTimeController's
	enum {
		UNKNOWN_10_POS		= 4		// slot 15 recomputes the target's normals, when it has some (TODO identity)
	};

	// whether slot 15 recomputes the normals (name invented; AnimOnly.exe
	// 0x452270)
	bool					GetUpdateNormals() const { return GetBit( UNKNOWN_10_POS ); }

	// FUNCTION: STBC 0x80CF50 ?GetRTTI@NiTriShapeSkinController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual void			Update( float time );
	virtual void			VirtualSlot15();
	// FUNCTION: STBC 0x80CF70 ?VirtualSlot17@NiTriShapeSkinController@@UAEXXZ
	virtual void			VirtualSlot17() {
								NiTimeController::VirtualSlot17();
								deformPending = false;
							}
	// FUNCTION: STBC 0x80CF90 ?TargetIsRequiredType@NiTriShapeSkinController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiTriShape, target );
							}

	virtual unsigned short	GetVertexCount() const;
	// a new default object, which NiSkinController::CreateClone fills
	virtual NiSkinController *	CreateNew();

	// the first NiTriShapeSkinController of the target's list (name
	// invented; nothing calls it)
	static NiTriShapeSkinController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// set by Update, cleared once slot 15 has deformed the vertices (name
	// invented); the constructor leaves it unset
	bool					deformPending;
};

NiRegisterStream( NiTriShapeSkinController );

#endif /* !__NITRISHAPESKINCONTROLLER_H__ */
