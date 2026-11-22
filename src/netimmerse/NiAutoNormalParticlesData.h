/*
===============================================================================

	NiAutoNormalParticlesData.h

	Particle data whose normals are not given but set by the shape, each
	time it is drawn, to face the camera.

	Evidence: the vtable at 0x8996A0 (18 slots, NiParticlesData's with
	GetRTTI, CreateClone and the six streaming slots replaced); the
	constructor (0x7F58D0) passes its three arguments on to
	NiParticlesData's with no normals, then creates an uninitialised normal
	array; the default constructor (0x7F5930) adds no members, and
	CreateObject allocates 0x44 bytes, NiParticlesData's size; the NiRTTI
	(0x9A1D18) is named "NiAutoNormalParticlesData" with NiParticlesData's
	as its base. The destructor is the implicit one: its copy (0x7F5CF0,
	emitted with the particles' unit as GetRTTI's at 0x7F5CC0) jumps
	straight to NiParticlesData's.

===============================================================================
*/

#ifndef __NIAUTONORMALPARTICLESDATA_H__
#define __NIAUTONORMALPARTICLESDATA_H__

#include "NiParticlesData.h"
#include "NiStreamMacros.h"

/*
================
NiAutoNormalParticlesDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x8996E8). The class name is invented.
================
*/
// FUNCTION: STBC 0x007F5A00 ??_GNiAutoNormalParticlesDataLoadRecord@@UAEPAXI@Z
class NiAutoNormalParticlesDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x007F5CD0 ??_GNiAutoNormalParticlesData@@UAEPAXI@Z
class NiAutoNormalParticlesData : public NiParticlesData {
public:
	// FUNCTION: STBC 0x7F5CC0 ?GetRTTI@NiAutoNormalParticlesData@@UBEPBVNiRTTI@@XZ
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

							NiAutoNormalParticlesData( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_ );
							NiAutoNormalParticlesData();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAutoNormalParticlesData *dest );
};

NiRegisterStream( NiAutoNormalParticlesData );

#endif /* !__NIAUTONORMALPARTICLESDATA_H__ */
