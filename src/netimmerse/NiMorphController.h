/*
===============================================================================

	NiMorphController.h

	Morphs its target's vertices between target vertex sets by keys.

	Evidence: the static initializer at 0x7A8300 builds the NiRTTI at
	0x9A0558 ("NiMorphController", based on NiTimeController's); the vtable
	at 0x898604 has NiTimeController's 20 slots and adds 1; the constructor
	(0x7A84F0) sets flag 0x10 and clears 0x30-0x4C and the byte at 0x50;
	the destructor (0x7A8530) frees each vertex set of the array at 0x4C
	(0x30 of them), then the keys at 0x48 (0x38 of them; keys of type 5
	are destroyed through 0x79C060 first), then the bound at 0x44.
	SetTarget (0x7A8660) records whether the target is an NiTriBasedGeom
	and then calls slot 20, which (0x7A8640) jumps to 0x7A8AE0: that
	function builds the bound at 0x44 from the keys and merges it into the
	target's. Slot 11 calls slot 20. The vtable ends there: 0x898658 is
	the load record's (stored by CreateObject, 0x7A8DD0) and 0x89865C
	another class's (stored by the constructor at 0x7AA0D0).

	Update (0x7A8750) morphs an NiTriBasedGeom's model data or an
	NiBezierMesh's control vertices; LoadBinary is open.

===============================================================================
*/

#ifndef __NIMORPHCONTROLLER_H__
#define __NIMORPHCONTROLLER_H__

#include "NiAVObject.h"
#include "NiFloatKey.h"
#include "NiTimeController.h"
#include "NiTriBasedGeom.h"
#include "NiStreamMacros.h"

class NiBound;

/*
================
NiMorphControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs, under
a vtable of its own (0x898658). The class name is invented.
================
*/
// FUNCTION: STBC 0x7A9250 ??_GNiMorphControllerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7A9270 ??1NiMorphControllerLoadRecord@@UAE@XZ
class NiMorphControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiMorphControllerLoadRecord() {}
};

// FUNCTION: STBC 0x007a9930 ??_GNiMorphController@@UAEPAXI@Z @netimmerse/NiMorphController
class NiMorphController : public NiTimeController {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7A8350)
	static int				Register();

	// the first NiMorphController of the target's list (name invented)
	static NiMorphController *	FindController( NiObjectNET *target );
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// flags, past NiTimeController's
	enum {
		UNKNOWN_10_POS		= 4,	// set by the constructor; compared by IsEqual; Update then makes the vertex sets after the first relative to it, once
		UNKNOWN_20_POS		= 5		// compared by IsEqual; Update recalculates an NiTriBasedGeom target's normals
	};

	// FUNCTION: STBC 0x7A9920 ?GetRTTI@NiMorphController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

							NiMorphController();
	virtual					~NiMorphController();

	// FUNCTION: STBC 0x7A8650 ?VirtualSlot11@NiMorphController@@UAEXXZ
	virtual void			VirtualSlot11() { VirtualSlot20(); }

	virtual void			Update( float time );
	virtual void			SetTarget( NiObjectNET *target_ );
	virtual bool			TargetIsRequiredType() const;

	// FUNCTION: STBC 0x7A8640 ?VirtualSlot20@NiMorphController@@UAEXXZ
	virtual void			VirtualSlot20() { Unknown7A8AE0(); }

	// takes the keys; with none, the controller has no keys (name invented)
	void					SetKeys( NiFloatKey **keys_, unsigned int numKeys_, NiFloatKey::KeyType keyType_ );
	// takes the vertex sets, then rebuilds the bound (name invented)
	void					SetTargets( NiPoint3 **targets_, unsigned int numTargets_, unsigned int numVertices_ );

	// whether Update recalculates the normals (name invented, after
	// NiMorpherController's; AnimOnly.exe 0x497FA0)
	bool					GetUpdateNormals() const { return GetBit( UNKNOWN_20_POS ); }
	// the constructor's flag (names invented; AnimOnly.exe 0x497FC0, 0x497FE0)
	void					SetUnknown10( bool value ) { SetBit( value, UNKNOWN_10_POS ); }
	bool					GetUnknown10() const { return GetBit( UNKNOWN_10_POS ); }
	// the vertex sets and the keys (names invented; AnimOnly.exe 0x497F30 and
	// 0x497F60); a key holds one value per vertex set
	NiPoint3 **				GetTargetVerts( unsigned int &numTargets_, unsigned int &numVertices_ ) const {
								numTargets_ = numTargets;
								numVertices_ = numVertices;
								return targets;
							}
	NiFloatKey **			GetMorphKeys( unsigned int &numKeys_, NiFloatKey::KeyType &keyType_, unsigned int &numValues ) const {
								numKeys_ = numKeys;
								keyType_ = keyType;
								numValues = numTargets;
								return keys;
							}

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiMorphController *dest );
	void					Unknown7A8AE0();	// TODO identity: builds the bound from the keys
	void					Unknown7A8880( float time );	// TODO identity: morphs the target's vertices by the weights at time

	unsigned int			numTargets;		// vertex sets in targets
	unsigned int			numVertices;	// vertices in each set
	unsigned int			numKeys;
	NiFloatKey::KeyType		keyType;
	int						unknown40;
	NiBound *				bound;			// owned: the bound of every morph
	NiFloatKey **			keys;			// owned: numKeys keys
	NiPoint3 **				targets;		// owned: numTargets vertex sets
	bool					triBasedTarget;	// the target is an NiTriBasedGeom (name invented)
};

NiRegisterStream( NiMorphController );

#endif /* !__NIMORPHCONTROLLER_H__ */
