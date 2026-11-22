/*
===============================================================================

	NiMorphData.h

	The morph targets of a morphing animation: sets of vertices, each with
	the float keys of its weight, shared by the controllers that play them.

	Evidence: the static initializer at 0x7A9E60 builds the NiRTTI at
	0x9A05D0 ("NiMorphData", based on NiObject's); the vtable at 0x89865C
	has NiObject's 12 slots, none added; the constructor (0x7AA0D0) clears
	0x08-0x10 and the byte at 0x14; the destructor (0x7AA0F0) deletes the
	array of 0x14-byte targets at 0x10 (constructor 0x7AA010, destructor
	0x7AA030). The viewer strings name the counts ("m_uiNumTargets",
	"m_uiNumVertsPerTarget"); the other names follow the Gamebryo lineage
	(MorphTarget, ReplaceAnim, SetRelativeTargets).

===============================================================================
*/

#ifndef __NIMORPHDATA_H__
#define __NIMORPHDATA_H__

#include "NiObject.h"
#include "NiStream.h"
#include "NiFloatKey.h"
#include "NiPoint3.h"
#include "NiStreamMacros.h"

/*
================
NiMorphDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x89868C). The class name is invented.
================
*/
// FUNCTION: STBC 0x7AA370 ??_GNiMorphDataLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7AA390 ??1NiMorphDataLoadRecord@@UAE@XZ
class NiMorphDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x7AAC20 ??_GNiMorphData@@UAEPAXI@Z
class NiMorphData : public NiObject {
public:
	/*
	================
	NiMorphData::MorphTarget

	One target: its vertices and the keys of its weight.
	================
	*/
	// FUNCTION: STBC 0x7A91F0 ??_EMorphTarget@NiMorphData@@QAEPAXI@Z @netimmerse/NiMorphController
	class MorphTarget {
	public:
							MorphTarget();
							~MorphTarget();

		NiPoint3 *			GetTargetVerts() const { return targetVerts; }
		// one vertex, by value (AnimOnly.exe 0x499040; name invented)
		NiPoint3			GetTargetVert( unsigned int i ) const { return targetVerts[i]; }
		// takes the vertices, deleting the old ones
		void				ReplaceTargetVerts( NiPoint3 *targetVerts_ );

		unsigned int		GetNumKeys() const { return numKeys; }
		NiFloatKey::KeyType	GetKeyType() const { return keyType; }
		NiFloatKey **		GetKeys() const { return keys; }
		NiFloatKey *		GetKeyAt( unsigned int i ) const { return i < numKeys ? keys[i] : 0; }
		unsigned int &		GetLastIndex() { return lastIndex; }	// Gamebryo lineage name
		// takes the keys; the old array is deleted, its keys are not
		void				ReplaceAnim( NiFloatKey **keys_, unsigned int numKeys_, NiFloatKey::KeyType keyType_ );

		NiPoint3 *			targetVerts;	// owned: the owner's numVertsPerTarget vertices
		unsigned int		numKeys;
		NiFloatKey::KeyType	keyType;
		NiFloatKey **		keys;			// owned: numKeys keys, each allocated on its own
		unsigned int		lastIndex;		// the key found by the last interpolation
	};

	// FUNCTION: STBC 0x7AAC10 ?GetRTTI@NiMorphData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning: the object itself, as NiObject's; the class keeps its own
	// copy (AnimOnly.exe 0x4985BD)
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

							NiMorphData();
	virtual					~NiMorphData();

	unsigned int			GetNumTargets() const { return numTargets; }
	unsigned int			GetNumVertsPerTarget() const { return numVertsPerTarget; }
	MorphTarget *			GetTargets() const { return targets; }
	// one target's members (Gamebryo 1.1.2 lineage names)
	unsigned int			GetNumKeys( unsigned int index ) const { return targets[index].GetNumKeys(); }
	NiFloatKey::KeyType		GetKeyType( unsigned int index ) const { return targets[index].GetKeyType(); }
	NiFloatKey **			GetKeys( unsigned int index ) const { return targets[index].GetKeys(); }
	NiPoint3 *				GetTargetVerts( unsigned int index ) const { return targets[index].GetTargetVerts(); }
	unsigned int &			GetLastIndex( unsigned int index ) { return targets[index].GetLastIndex(); }
	// takes the targets, deleting the old ones
	void					ReplaceTargets( MorphTarget *targets_, unsigned int numTargets_, unsigned int numVertsPerTarget_ );

	bool					GetRelativeTargets() const { return relativeTargets; }
	// only the flag; the targets are not converted (Gamebryo 1.1.2:
	// NIMORPHDATA.INL; AnimOnly.exe 0x498060)
	void					SetRelativeTargets( bool relativeTargets_ ) { relativeTargets = relativeTargets_; }
	// makes the targets after the first relative to it, or absolute again
	// (Gamebryo 1.1.2 lineage name)
	void					SetAndAdjustRelativeTargets( bool relativeTargets_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	friend class NiMorpherController;	// converts the targets of old files
	friend class NiMorphController;		// converts its own data

	unsigned int			numTargets;
	unsigned int			numVertsPerTarget;
	MorphTarget *			targets;			// owned: numTargets targets
	bool					relativeTargets;	// the targets after the first hold offsets from it
};

NiRegisterStream( NiMorphData );

#endif /* !__NIMORPHDATA_H__ */
