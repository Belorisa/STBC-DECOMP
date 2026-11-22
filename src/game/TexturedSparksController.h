/*
===============================================================================

	TexturedSparksController.h

	An emitter whose particles are drawn as textured streaks: each quad lies
	along its particle's velocity, and its width and length follow the
	controller's dimension keys over the particle's life.

	0x11C bytes. Evidence: the NiRTTI at 0x985478 is built with the name
	"TexturedSparksController" (0x8E0DA4) and EmitterController's NiRTTI as
	its base (0x4D2D50); Create (0x4D2DF0) allocates 0x11C bytes and calls
	the constructor (0x4D2E70), which calls EmitterController's with its six
	arguments, builds the key array at 0x108 (vtable 0x88CD44, 0x10-byte
	keys) and stores the vtable 0x88CCA4 (EmitterController's 40 slots,
	none added). The destructor is the implicit one (0x4D2F10 frees the key
	array, then calls EmitterController's). TargetIsRequiredType tests the
	NiRTTI at 0x986B08, named "TriShapeOrientedParticles", which
	CreateTarget builds; the directions and dimensions at 0x100 and 0x104
	are the target's arrays.

	The member names are invented, after their use.

===============================================================================
*/

#ifndef __TEXTUREDSPARKSCONTROLLER_H__
#define __TEXTUREDSPARKSCONTROLLER_H__

#include "EmitterController.h"
#include "NiLinPosKey.h"
#include "NiPoint2.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004d2ef0 ??_GTexturedSparksController@@UAEPAXI@Z
// FUNCTION: STBC 0x004d2f10 ??1TexturedSparksController@@UAE@XZ
class TexturedSparksController : public EmitterController {
public:
	// FUNCTION: STBC 0x004d2ee0 ?GetRTTI@TexturedSparksController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual void			Update( float time );
	virtual bool			TargetIsRequiredType() const;

	virtual void			CreateTarget( char *textureName );

	virtual void			VirtualSlot35( int index, float time, float deltaTime, float lifeFraction );
	virtual bool			VirtualSlot36( float time );
	virtual void			VirtualSlot37( int index );
	virtual void			VirtualSlot38();

							TexturedSparksController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ );

	static TexturedSparksController *	Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity, float angleVariance, NiAVObject *emitFrom );

	// the first TexturedSparksController of the target's list (name invented)
	static TexturedSparksController *	FindController( NiObjectNET *target );

	void					AddDimensionKey( float time, float width, float length );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( TexturedSparksController *dest );

	NiPoint3 *				directions;				// 0x100 the target's: each particle's velocity, unitized
	NiPoint2 *				dimensions;				// 0x104 the target's, or NULL
	KeyArray<NiLinPosKey>	dimensionKeys;			// 0x108 x the width, y the length, over the particle's life
};

#endif /* !__TEXTUREDSPARKSCONTROLLER_H__ */
