/*
===============================================================================

	NiDynamicEffect.h

	A scene graph object that affects the nodes it is attached to (lights
	and the like). Each effect keeps the list of nodes it affects, and each
	NiNode keeps the list of effects that affect it.

	Evidence: the vtable at 0x898DB0 (40 slots, slot 39 _purecall); the
	constructor (0x7E2200) calls NiAVObject's, sets the bool at 0xC8, takes
	the next index from the static counter at 0x9784D4, sets 0xD4 to 1 and
	clears the node list at 0xD8; the NiRTTI (0x9A16C8) is named
	"NiDynamicEffect" with NiAVObject's as its base. NiNode's destructor
	(0x7E3880) calls DetachAffectedNode (0x7E2380) for each effect it
	still lists. The member names follow the Gamebryo lineage (m_bOn,
	m_iIndex, the affected node list), which keeps this layout. Both
	derived classes seen (NiLight, 0x7F37A0, and NiTextureEffect, 0x808BB0)
	place their first member at 0x120, so the class is 0x120 bytes.

===============================================================================
*/

#ifndef __NIDYNAMICEFFECT_H__
#define __NIDYNAMICEFFECT_H__

#include "NiAVObject.h"
#include "NiStreamMacros.h"

class NiNode;

// the nodes an effect affects (Gamebryo lineage typedef)
typedef NiTSimpleList<NiNode *>	NiNodeList;

/*
================
NiDynamicEffectLoadRecord

The link IDs of the affected nodes. LoadBinary (0x7E2540) reads them and
drops them, so the array stays empty. The class name is invented.

Evidence: the creation functions of the texture effect (0x809120) and the
lights (NiAmbientLight's, 0x7F4130) build the array at 0x28 under the
vtable 0x8987F0 (scalar deleting destructor 0x7B3560), after NiAVObject's
record; NiAmbientLight's record (destructor 0x7F4250) ends there.
================
*/
class NiDynamicEffectLoadRecord : public NiAVObjectLoadRecord {
public:
	// TODO identity: the element type; a 4-byte type whose array vtable
	// differs from those of NiTArray<unsigned int> (0x88B914) and
	// NiTArray<unsigned long> (0x8990D8). The particle system controller's
	// record (0x7B2320) has another array of the same type.
	NiTArray<int>			affectedNodeLinkIDs;
};

// FUNCTION: STBC 0x007e2980 ??_GNiDynamicEffect@@UAEPAXI@Z @netimmerse/NiDynamicEffect
class NiDynamicEffect : public NiAVObject {
public:
	// FUNCTION: STBC 0x7E2970 ?GetRTTI@NiDynamicEffect@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			VirtualSlot03();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiDynamicEffect();

	// FUNCTION: STBC 0x434320 ?VirtualSlot26@NiDynamicEffect@@UAE_NXZ
	virtual bool			VirtualSlot26() { return false; }

	// the kind of effect (Gamebryo lineage names); the lights come first
	enum EffectType {
		AMBIENT_LIGHT,
		DIR_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		TEXTURE_EFFECT
	};

	virtual int				GetEffectType() const = 0;

							NiDynamicEffect();

	// on or off (Gamebryo lineage name); the renderer skips a light that
	// is off (0x7D1570)
	bool					GetSwitch() const { return on; }
	void					SetSwitch( bool on_ ) { on = on_; }

	// unique per effect; the Direct3D renderer uses it as the light index
	int						GetIndex() const { return index; }
	unsigned int			GetRevision() const { return revision; }
	void					IncRevisionID() { revision++; }

	// the nodes the effect affects
	void					AttachAffectedNode( NiNode *node );
	void					DetachAffectedNode( NiNode *node );
	void					DetachAllAffectedNodes();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiDynamicEffect *dest );

	bool					on;				// set by the constructor
	int						index;			// unique, from nextIndex
	int						unknownD0;
	unsigned int			revision;		// 1 after construction; the lights bump it when they move (name invented)
	NiNodeList				affectedNodes;
	// TODO identity: nothing in the class's own functions touches 0xE0-0x11F
	int						unknownE0[16];

	static int				nextIndex;
};

NiRegisterStream( NiDynamicEffect );

#endif /* !__NIDYNAMICEFFECT_H__ */
