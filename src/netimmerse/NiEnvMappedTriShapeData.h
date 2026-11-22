/*
===============================================================================

	NiEnvMappedTriShapeData.h

	Triangle list geometry data for an environment-mapped shape: the
	texture coordinates are not given but generated, one set, by the shape
	each time it is drawn.

	Evidence: the vtable at 0x899AB8 (18 slots, NiTriShapeData's with
	GetRTTI, CreateClone and the six streaming slots replaced); the
	constructor (0x7FACB0) passes its six arguments on to NiTriShapeData's
	with no texture coordinates, then creates one uninitialised set; the
	default constructor (0x7FAD30) adds no members, and CreateObject
	allocates 0x48 bytes, NiTriShapeData's size; the NiRTTI (0x9A1F88) is
	named "NiEnvMappedTriShapeData" with NiTriShapeData's as its base. The
	destructor is the implicit one: its copy (0x7FB070, emitted with the
	shape's unit as GetRTTI's at 0x7FB040) jumps straight to
	NiTriShapeData's.

===============================================================================
*/

#ifndef __NIENVMAPPEDTRISHAPEDATA_H__
#define __NIENVMAPPEDTRISHAPEDATA_H__

#include "NiTriShapeData.h"
#include "NiStreamMacros.h"

/*
================
NiEnvMappedTriShapeDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x899B00). The class name is invented.
================
*/
// FUNCTION: STBC 0x007FAE00 ??_GNiEnvMappedTriShapeDataLoadRecord@@UAEPAXI@Z
class NiEnvMappedTriShapeDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x007FB050 ??_GNiEnvMappedTriShapeData@@UAEPAXI@Z
class NiEnvMappedTriShapeData : public NiTriShapeData {
public:
	// FUNCTION: STBC 0x7FB040 ?GetRTTI@NiEnvMappedTriShapeData@@UBEPBVNiRTTI@@XZ
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

							NiEnvMappedTriShapeData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, unsigned short triangleCount_, unsigned short *triList_ );
							NiEnvMappedTriShapeData();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiEnvMappedTriShapeData *dest );
};

NiRegisterStream( NiEnvMappedTriShapeData );

#endif /* !__NIENVMAPPEDTRISHAPEDATA_H__ */
