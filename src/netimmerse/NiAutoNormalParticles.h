/*
===============================================================================

	NiAutoNormalParticles.h

	Particles whose normals face the camera: an NiParticles over
	NiAutoNormalParticlesData that sets every normal to the reversed camera
	direction each time it is drawn.

	Evidence: the vtable at 0x8996EC (49 slots, NiParticles' with GetRTTI,
	CreateClone, the streaming slots, the destructor and slot 31 replaced);
	the constructors (0x7F5EA0, 0x7F5F20, 0x7F5F40) call NiParticles' and
	add no members, the first creating the data from three arguments; the
	NiRTTI (0x9A1D58) is named "NiAutoNormalParticles" with NiParticles'
	as its base; CreateClone (0x7F6070) allocates 0xE4 bytes, NiParticles'
	size. The destructor is the implicit one: its copy (0x7F6550, emitted
	with GetRTTI's at 0x7F6520 by a later unit that includes this header)
	jumps straight to NiTriBasedGeom's.

===============================================================================
*/

#ifndef __NIAUTONORMALPARTICLES_H__
#define __NIAUTONORMALPARTICLES_H__

#include "NiParticles.h"
#include "NiAutoNormalParticlesData.h"
#include "NiStreamMacros.h"

/*
================
NiAutoNormalParticlesLoadRecord

The record CreateObject hands the stream: NiGeometry's under a vtable of
its own. The class name is invented.
================
*/
// FUNCTION: STBC 0x7F61C0 ??_GNiAutoNormalParticlesLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7F61E0 ??1NiAutoNormalParticlesLoadRecord@@UAE@XZ
class NiAutoNormalParticlesLoadRecord : public NiParticlesLoadRecord {
};

// FUNCTION: STBC 0x007F6530 ??_GNiAutoNormalParticles@@UAEPAXI@Z
class NiAutoNormalParticles : public NiParticles {
public:
	// FUNCTION: STBC 0x007F6520 ?GetRTTI@NiAutoNormalParticles@@UBEPBVNiRTTI@@XZ
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

	// TODO identity: slot 31 sets every normal of the data to the reversed
	// direction of the camera at the argument, then draws the particles
	// through the renderer at the argument's 0x188 (its slot 49) and counts
	// the shapes, triangles and vertices drawn
	virtual void			Display( NiCamera *camera );

							NiAutoNormalParticles( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_ );
							NiAutoNormalParticles( NiAutoNormalParticlesData *data );
							NiAutoNormalParticles();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAutoNormalParticles *dest );
};

NiRegisterStream( NiAutoNormalParticles );

#endif /* !__NIAUTONORMALPARTICLES_H__ */
