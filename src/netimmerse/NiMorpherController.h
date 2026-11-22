/*
===============================================================================

	NiMorpherController.h

	Morphs its target's vertices by the weighted targets of an NiMorphData.
	Update only notes the time; the morph itself waits for slot 15, and
	slot 17 drops a pending one.

	Evidence: the static initializer at 0x7AAC60 builds the NiRTTI at
	0x9A0650 ("NiMorpherController", based on NiTimeController's); the
	vtable at 0x898690 has NiTimeController's 20 slots, none added; the
	constructor (0x7AAE60) takes the data into the smart pointer at 0x30 and
	clears the bytes at 0x34 and 0x35 and the time at 0x38; CreateClone
	(0x7AB2E0) allocates 0x3C bytes; slot 19 (0x7ABCD0) tests the target
	against NiAVObject's NiRTTI. LoadBinary reads files older than "3.05"
	in a format of their own, which it converts into an NiMorphData.

===============================================================================
*/

#ifndef __NIMORPHERCONTROLLER_H__
#define __NIMORPHERCONTROLLER_H__

#include "NiAVObject.h"
#include "NiTimeController.h"
#include "NiMorphData.h"
#include "NiStreamMacros.h"

/*
================
NiMorpherControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs and
the data's, under a vtable of its own (0x8986E0). The class name is
invented.
================
*/
// FUNCTION: STBC 0x7ABC60 ??_GNiMorpherControllerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7ABC80 ??1NiMorpherControllerLoadRecord@@UAE@XZ
class NiMorpherControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiMorpherControllerLoadRecord() { dataLinkID = 0; }

	unsigned int			dataLinkID;
};

// FUNCTION: STBC 0x7ABC40 ??_GNiMorpherController@@UAEPAXI@Z
class NiMorpherController : public NiTimeController {
public:
	// flags, past NiTimeController's
	enum {
		UNKNOWN_10_POS		= 4		// an NiTriBasedGeom target's normals are recalculated after a morph
	};

	// FUNCTION: STBC 0x7ABC30 ?GetRTTI@NiMorpherController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiMorpherController();

	// notes the time; the morph waits for VirtualSlot15
	virtual void			Update( float time );
	// morphs the target at the time Update noted
	virtual void			VirtualSlot15();
	// FUNCTION: STBC 0x7ABCB0 ?VirtualSlot17@NiMorpherController@@UAEXXZ
	virtual void			VirtualSlot17() { NiTimeController::VirtualSlot17(); updatePending = false; }
	// FUNCTION: STBC 0x7ABCD0 ?TargetIsRequiredType@NiMorpherController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiAVObject, target );
							}

							NiMorpherController( NiMorphData *data_ = 0 );

	// Gamebryo 1.1.2 lineage names (AnimOnly.exe 0x498080)
	void					SetUpdateNormals( bool updateNormals ) { SetBit( updateNormals, UNKNOWN_10_POS ); }
	bool					GetUpdateNormals() const { return GetBit( UNKNOWN_10_POS ); }

	NiMorphData *			GetMorphData() const { return data; }
	// the targets and each target's keys (Gamebryo 1.1.2's
	// NIMORPHERCONTROLLER.INL; AnimOnly.exe 0x49A970, 0x49A9D0, 0x49AA00)
	unsigned int			GetNumTargets() const { return data ? data->GetNumTargets() : 0; }
	unsigned int			GetNumKeys( unsigned int index ) const { return data->GetNumKeys( index ); }
	NiFloatKey **			GetKeys( unsigned int index ) const { return data->GetKeys( index ); }
	void					SetMorphData( NiMorphData *data_ ) { data = data_; }

	// the first and last key times of the targets bound the controller's
	// time (Gamebryo lineage name)
	void					ResetTimeExtrema();
	// gives the data new targets, creating the data first (name invented)
	void					ReplaceTargets( NiMorphData::MorphTarget *targets, unsigned int numTargets, unsigned int numVertsPerTarget, bool resetTimes );

	// the first morpher controller of target, or NULL (name invented)
	static NiMorpherController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiMorpherController *dest );
	// sets the target's vertices to the weighted sum of the targets at time
	void					GenMorphInterp( float time );	// Gamebryo lineage name

	NiPointer<NiMorphData>	data;
	bool					updatePending;	// Update noted a time the morph has not used yet
	bool					ignoreBaseTarget;	// Gamebryo lineage name: when set the morph adds to the target's vertices and the first target weighs nothing
	float					updateTime;		// the time Update noted
};

NiRegisterStream( NiMorpherController );

#endif /* !__NIMORPHERCONTROLLER_H__ */
