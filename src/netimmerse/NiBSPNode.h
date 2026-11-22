/*
===============================================================================

	NiBSPNode.h

	A binary space partition node: two children split by a plane, drawn
	back to front from the camera's side of the plane.

	Evidence: the NiRTTI (0x9A1E68, built at 0x7F7D10) is named "NiBSPNode"
	with NiNode's as its base; the vtable at 0x89986C has NiNode's 44
	slots and adds none. The creation function (0x7F8590) allocates 0x108
	bytes; the constructor, inlined there, builds two planes at 0xE8 and
	0xF8 and sizes the children array to two slots that do not grow. The
	viewer strings (0x7F87D0) name "m_modelPlane" and "m_worldPlane".

===============================================================================
*/

#ifndef __NIBSPNODE_H__
#define __NIBSPNODE_H__

#include "NiNode.h"
#include "NiPlane.h"
#include "NiStreamMacros.h"

/*
================
NiBSPNodeLoadRecord

The record CreateObject hands the stream: NiNode's, under its own vtable.
The class name is invented.

Evidence: CreateObject (0x7F8590) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x89991C last.
================
*/
// FUNCTION: STBC 0x7F86D0 ??_GNiBSPNodeLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7F86F0 ??1NiBSPNodeLoadRecord@@UAE@XZ
class NiBSPNodeLoadRecord : public NiNodeLoadRecord {
};

// FUNCTION: STBC 0x007F8C20 ??_GNiBSPNode@@UAEPAXI@Z
class NiBSPNode : public NiNode {
public:
	// FUNCTION: STBC 0x007F8C10 ?GetRTTI@NiBSPNode@@UBEPBVNiRTTI@@XZ
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

	virtual void			ApplyTransform( const NiMatrix3 &rotate, const NiPoint3 &translate, bool onLeft );
	virtual void			Display( NiCamera *camera );
	virtual void			UpdateWorldData();

	// the children: the left one on the positive side of the plane, the
	// right one on the negative side
							NiBSPNode() { children.SetSize( 2 ); children.SetGrowBy( 0 ); }

	void					SetModelPlane( const NiPlane &plane ) { modelPlane = plane; }
	const NiPlane &			GetModelPlane() const { return modelPlane; }
	const NiPlane &			GetWorldPlane() const { return worldPlane; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiBSPNode *dest );

	// the world plane from the model plane (name invented)
	void					UpdateWorldPlane();

	NiPlane					modelPlane;
	NiPlane					worldPlane;
};

NiRegisterStream( NiBSPNode );

#endif /* !__NIBSPNODE_H__ */
