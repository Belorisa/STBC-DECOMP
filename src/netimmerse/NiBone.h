/*
===============================================================================

	NiBone.h

	A node of a skeleton: an NiNode that also keeps its transform relative
	to the skeleton's root, the space the skinning controllers deform in.

	Evidence: the NiRTTI (0x9A1E08, built at 0x7F7280) is named "NiBone"
	with NiNode's as its base; the vtable at 0x8997B8 has NiNode's 44
	slots and adds none. The constructor (0x7F7410) sets the identity
	rotation at 0xE8, the zero point at 0x10C and the scale 1 at 0x118;
	slot 25 (0x7F74B0) composes the parent's transform at 0xE8 with the
	local one when the parent is an NiBone, and takes the local one alone
	otherwise. NiSkinController's bones are NiBones: its bound and its
	Deform (0x8239D0) read the three members of each bone.

===============================================================================
*/

#ifndef __NIBONE_H__
#define __NIBONE_H__

#include "NiNode.h"
#include "NiStreamMacros.h"

/*
================
NiBoneLoadRecord

The record CreateObject hands the stream: NiNode's, under its own vtable.
The class name is invented.

Evidence: CreateObject (0x7F7990) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x899868 last; that vtable's only reference is
there, and its destructor (0x7F7B10) frees NiNode's two arrays and
NiAVObject's.
================
*/
// FUNCTION: STBC 0x7F7AF0 ??_GNiBoneLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7F7B10 ??1NiBoneLoadRecord@@UAE@XZ
class NiBoneLoadRecord : public NiNodeLoadRecord {
};

// FUNCTION: STBC 0x7F7CD0 ??_GNiBone@@UAEPAXI@Z
// FUNCTION: STBC 0x7F7CF0 ??1NiBone@@UAE@XZ
class NiBone : public NiNode {
public:
	// FUNCTION: STBC 0x7F7CC0 ?GetRTTI@NiBone@@UBEPBVNiRTTI@@XZ
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

	virtual void			UpdateDownwardPass( float time );

							NiBone();

	// the transform relative to the skeleton's root (names invented)
	const NiMatrix3 &		GetBoneRotate() const { return boneRotate; }
	const NiPoint3 &		GetBoneTranslate() const { return boneTranslate; }
	float					GetBoneScale() const { return boneScale; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiBone *dest );

	// true unless the parent is a bone: the bone is the root of its
	// skeleton (name invented)
	int						IsSkeletonRoot() const;
	// the bone transform from the parent bone's (names invented)
	void					UpdateBoneTransform( const NiMatrix3 &parentRotate, const NiPoint3 &parentTranslate, float parentScale );

	// the transform relative to the skeleton's root (names invented)
	NiMatrix3				boneRotate;
	NiPoint3				boneTranslate;
	float					boneScale;
};

NiRegisterStream( NiBone );

#endif /* !__NIBONE_H__ */
