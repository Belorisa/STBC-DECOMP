/*
===============================================================================

	NiTriShapeDynamicData.h

	Triangle list geometry data of which only a leading part is active:
	the shape draws the first activeVertices vertices and activeTriangles
	triangles.

	Evidence: the vtable at 0x89A414 (18 slots, NiTriShapeData's with
	GetRTTI, CreateClone, the six streaming slots, the destructor and the
	active count slots 12, 13, 16 and 17 replaced); the constructor
	(0x80C3E0) builds NiTriShapeData's over the active counts, then keeps
	the active counts at 0x48 and 0x4A and sets the full vertex and
	triangle counts; CreateObject allocates 0x4C bytes; the NiRTTI
	(0x9A25C0) is named "NiTriShapeDynamicData" with NiTriShapeData's as
	its base; the viewer strings name the members ("m_usActiveVertices",
	"m_usActiveTriangles"). The setters are inline: their copies (0x4FB5B0,
	0x4FB5C0) and the implicit destructor's (0x4FB580) come from a game
	unit.

===============================================================================
*/

#ifndef __NITRISHAPEDYNAMICDATA_H__
#define __NITRISHAPEDYNAMICDATA_H__

#include "NiTriShapeData.h"
#include "NiStreamMacros.h"

/*
================
NiTriShapeDynamicDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own. The class name is invented.
================
*/
// FUNCTION: STBC 0x0080C550 ??_GNiTriShapeDynamicDataLoadRecord@@UAEPAXI@Z
class NiTriShapeDynamicDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x0080C8F0 ??_GNiTriShapeDynamicData@@UAEPAXI@Z
class NiTriShapeDynamicData : public NiTriShapeData {
public:
	// FUNCTION: STBC 0x80C8E0 ?GetRTTI@NiTriShapeDynamicData@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x4FB5B0 ?SetActiveVertexCount@NiTriShapeDynamicData@@UAEXG@Z
	virtual void			SetActiveVertexCount( unsigned short count ) { activeVertices = count; }
	virtual unsigned short	GetActiveVertexCount() const;
	// FUNCTION: STBC 0x4FB5C0 ?SetActiveTriangleCount@NiTriShapeDynamicData@@UAEXG@Z
	virtual void			SetActiveTriangleCount( unsigned short count ) { activeTriangles = count; }
	virtual unsigned short	GetActiveTriangleCount() const;

							NiTriShapeDynamicData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_, unsigned short activeVertices_, unsigned short activeTriangles_ );
							NiTriShapeDynamicData();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriShapeDynamicData *dest );

	unsigned short			activeVertices;		// the vertices drawn, from the first
	unsigned short			activeTriangles;	// the triangles drawn, from the first
};

NiRegisterStream( NiTriShapeDynamicData );

#endif /* !__NITRISHAPEDYNAMICDATA_H__ */
