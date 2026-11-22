/*
===============================================================================

	NiBillboardNode.h

	A node that turns its subtree to face the camera each time it is drawn:
	always facing it, turning about the up axis only, or facing it rigidly.

	Evidence: the NiRTTI (0x9A1DC8, built at 0x7F6570) is named
	"NiBillboardNode" with NiNode's as its base; the vtable at 0x88CD48 has
	NiNode's 44 slots and adds none (its GetRTTI and destructor are inline
	copies the linker took from game units). The creation function
	(0x7F6CF0) allocates 0xEC bytes; the constructor, inlined there, sets the
	mode in flag bits 5-6 to 1 and clears the float at 0xE8, which slot 25
	(0x7F6BB0) sets to the update time. The viewer strings (0x7F6EF0) name
	"m_eMode" and the modes "ALWAYS_FACE_CAMERA", "ROTATE_ABOUT_UP" and
	"RIGID_FACE_CAMERA".

===============================================================================
*/

#ifndef __NIBILLBOARDNODE_H__
#define __NIBILLBOARDNODE_H__

#include "NiNode.h"
#include "NiStreamMacros.h"

/*
================
NiBillboardNodeLoadRecord

The record CreateObject hands the stream: NiNode's, under its own vtable.
The class name is invented.

Evidence: CreateObject (0x7F6CF0) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x8997B4 last.
================
*/
// FUNCTION: STBC 0x7F6E20 ??_GNiBillboardNodeLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7F6E40 ??1NiBillboardNodeLoadRecord@@UAE@XZ
class NiBillboardNodeLoadRecord : public NiNodeLoadRecord {
};

// FUNCTION: STBC 0x4D53B0 ??_GNiBillboardNode@@UAEPAXI@Z @game/WarpFlash
// FUNCTION: STBC 0x4D53D0 ??1NiBillboardNode@@UAE@XZ @game/WarpFlash
class NiBillboardNode : public NiNode {
public:
	// FUNCTION: STBC 0x4D53A0 ?GetRTTI@NiBillboardNode@@UBEPBVNiRTTI@@XZ @netimmerse/NiBillboardNode
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

	virtual void			UpdateDownwardPass( float time );
	virtual void			VirtualSlot35( NiCamera *camera );
	virtual void			UpdateWorldBound();

	// how the subtree faces the camera
	enum FaceMode {
		ALWAYS_FACE_CAMERA,
		ROTATE_ABOUT_UP,
		RIGID_FACE_CAMERA
	};

							NiBillboardNode() { SetMode( ROTATE_ABOUT_UP ); savedTime = 0.0f; }

	void					SetMode( FaceMode mode );
	FaceMode				GetMode() const { return (FaceMode)( ( flags & MODE_MASK ) >> MODE_POS ); }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// flags, above NiAVObject's
	enum {
		MODE_MASK				= 0x0060,
		MODE_BITS				= 0x0003,	// MODE_MASK unshifted, for SetField
		MODE_POS				= 5
	};

	void					CopyMembers( NiBillboardNode *dest );

	// rebuilds the world transform with the rotation that faces the camera
	// for the mode and updates the children at savedTime (Gamebryo lineage
	// name)
	void					RotateToCamera( NiCamera *camera );

	static char *			GetViewerString( const char *prefix, FaceMode mode );

	float					savedTime;		// the time of the last update, for the children's update when drawn
};

NiRegisterStream( NiBillboardNode );

/*
================
NiBillboardNode::SetMode
================
*/
inline void NiBillboardNode::SetMode( FaceMode mode ) {
	SetField( (unsigned short)mode, MODE_BITS, MODE_POS );
}

#endif /* !__NIBILLBOARDNODE_H__ */
