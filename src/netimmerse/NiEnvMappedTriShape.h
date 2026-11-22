/*
===============================================================================

	NiEnvMappedTriShape.h

	A triangle list drawn with a generated environment map: an NiTriShape
	over NiEnvMappedTriShapeData whose texture coordinates are computed
	from the world normals and the camera each time it is drawn.

	Evidence: the vtable at 0x899B04 (51 slots, NiTriShape's with GetRTTI,
	CreateClone, the streaming slots, the destructor and slot 31 replaced);
	the constructors (0x7FB220, 0x7FB2C0, 0x7FB310) call NiTriShape's and
	add no members, the first creating the data from six arguments, and
	the first two then create the world normals; CopyMembers (0x7FB5F0)
	and LinkObject (0x7FB730) create the copy's or the loaded shape's world
	normals; the NiRTTI (0x9A1FD8) is named "NiEnvMappedTriShape" with
	NiTriShape's as its base; CreateClone (0x7FB570) allocates 0xE4 bytes,
	NiTriShape's size. Slot 31 (0x7FB330) computes the texture coordinates
	(0x7FB3F0) before it draws. The destructor is the implicit one: its copy
	(0x7FBA80, emitted with GetRTTI's at 0x7FBA50 by a later unit that
	includes this header) jumps straight to NiTriShape's.

===============================================================================
*/

#ifndef __NIENVMAPPEDTRISHAPE_H__
#define __NIENVMAPPEDTRISHAPE_H__

#include "NiTriShape.h"
#include "NiEnvMappedTriShapeData.h"
#include "NiStreamMacros.h"

/*
================
NiEnvMappedTriShapeLoadRecord

The record CreateObject hands the stream: NiGeometry's under a vtable of
its own (0x899BD0). The class name is invented.
================
*/
// FUNCTION: STBC 0x7FB6E0 ??_GNiEnvMappedTriShapeLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7FB700 ??1NiEnvMappedTriShapeLoadRecord@@UAE@XZ
class NiEnvMappedTriShapeLoadRecord : public NiTriShapeLoadRecord {
};

// FUNCTION: STBC 0x007FBA60 ??_GNiEnvMappedTriShape@@UAEPAXI@Z
class NiEnvMappedTriShape : public NiTriShape {
public:
	// FUNCTION: STBC 0x007FBA50 ?GetRTTI@NiEnvMappedTriShape@@UBEPBVNiRTTI@@XZ
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

	// slot 31: generates the environment map's texture coordinates for the
	// camera's world location, then draws the shape as NiTriShape's does
	virtual void			Display( NiCamera *camera );

							NiEnvMappedTriShape( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, unsigned short triangleCount_, unsigned short *triList_ );
							NiEnvMappedTriShape( NiEnvMappedTriShapeData *data );
							NiEnvMappedTriShape();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiEnvMappedTriShape *dest );

	// sphere-maps the world normals seen from the camera into the data's
	// texture coordinates (0x7FB3F0; invented name)
	void					UpdateEnvMapTextures( const NiPoint3 &cameraLocation );
};

NiRegisterStream( NiEnvMappedTriShape );

#endif /* !__NIENVMAPPEDTRISHAPE_H__ */
