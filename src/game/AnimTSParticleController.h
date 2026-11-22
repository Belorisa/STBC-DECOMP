/*
===============================================================================

	AnimTSParticleController.h

	An emitter whose particles are drawn as quads playing a texture
	animation: each new particle takes a random track of the controller's
	texture animation and plays it over its life. The particles are kept in
	the order they were emitted, so that they can be drawn oldest first or,
	with the flag cleared, newest first.

	0x110 bytes. Evidence: the NiRTTI at 0x984750 is built with the name
	"AnimTSParticleController" (0x8E0CDC) and EmitterController's NiRTTI as
	its base (0x4C6E70); the constructor (0x4C6F10) calls
	EmitterController's and stores the vtable 0x88C764 (EmitterController's
	40 slots, none added); CreateClone (0x4C7030) and the SWIG wrapper
	AnimTSParticleController_Create allocate 0x110 bytes. The wrapper
	AnimTSParticleController_SetDrawOldToNew writes the flag at 0x10C.
	TargetIsRequiredType tests the NiRTTI at 0x985AA8, named
	"AnimatedTriShapeParticles", which CreateTarget builds over the
	controller's arrays.

	The member names other than the flag are invented, after their use.

===============================================================================
*/

#ifndef __ANIMTSPARTICLECONTROLLER_H__
#define __ANIMTSPARTICLECONTROLLER_H__

#include "EmitterController.h"

class TextureAnimContainer;
class TextureAnimTrack;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004c6fb0 ??_GAnimTSParticleController@@UAEPAXI@Z
class AnimTSParticleController : public EmitterController {
public:
	// FUNCTION: STBC 0x004c6f70 ?GetRTTI@AnimTSParticleController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~AnimTSParticleController();

	virtual void			Update( float time );
	virtual bool			TargetIsRequiredType() const;

	virtual void			CreateTarget( char *textureAnimName );

	virtual void			VirtualSlot35( int index, float time, float deltaTime, float lifeFraction );
	virtual bool			VirtualSlot36( float time );
	// keeps the order of the particles after the removed one
	virtual void			VirtualSlot37( int index );
	virtual void			VirtualSlot38();

							AnimTSParticleController();
	// The script-side constructor (the SWIG wrapper AnimTSParticleController_Create).
	// FUNCTION: STBC 0x0061e2a0 ?Create@AnimTSParticleController@@SAPAV1@XZ @game/AppWrap
	static AnimTSParticleController *	Create() { return new AnimTSParticleController; }

	void					SetDrawOldToNew( bool drawOldToNew_ ) { drawOldToNew = drawOldToNew_; }

	// the first AnimTSParticleController of the target's list
	static AnimTSParticleController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( AnimTSParticleController *dest );

	TextureAnimContainer *	textureAnim;			// 0x100 the tracks the particles play, from g_kTextureAnimManager
	unsigned short *		frames;					// 0x104 one per particle: its frame of its track
	TextureAnimTrack **		tracks;					// 0x108 one per particle
	bool					drawOldToNew;			// 0x10C true at first: the oldest particles are drawn first
};

#endif /* !__ANIMTSPARTICLECONTROLLER_H__ */
