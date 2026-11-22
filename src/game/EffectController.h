/*
===============================================================================

	EffectController.h

	The base of the particle and effect controllers: a time controller with
	a lifetime and three key tracks over it, the colour, the alpha and the
	size of what it animates, kept in a shared EffectControllerData. Its
	target is the effect's own scene graph object, which the controller
	attaches to and detaches from a parent node.

	0x38 bytes. Evidence: the NiRTTI at 0x9849D8 is built with the name
	"EffectController" (0x8E0D30) and NiTimeController's NiRTTI as its base
	(0x4C9C00); the vtable 0x88C8A4 has NiTimeController's 20 slots and 14
	of its own, of which the SWIG wrappers EffectController_* call 23 to 32
	by name; the constructors 0x4C9CA0 and 0x4C9EE0 and the destructor
	0x4C9F10. EffectController_GetEffectLifeTime and _SetEffectLifeTime
	read and write 0x30; EffectController_GetEffectLevel and
	_SetEffectLevel the static at 0x8E0D14. The level names are the App
	constants EffectController_LOWEST to _HIGHEST, in value order.

===============================================================================
*/

#ifndef __EFFECTCONTROLLER_H__
#define __EFFECTCONTROLLER_H__

#include "NiTimeController.h"
#include "NiLinColorKey.h"
#include "NiLinFloatKey.h"
#include "KeyArray.h"

class NiAVObject;
class NiNode;

/*
================
EffectControllerData

The key tracks of an effect controller, shared between a controller and
its clones.

0x44 bytes. Evidence: the NiRTTI at 0x984A18 is built with the name
"EffectControllerData" (0x8E0D18) and NiObject's NiRTTI as its base
(0x4C9BA0); the vtable 0x88C92C overrides GetRTTI (0x4C9D70) and the
destructor (0x4C9DE0). The member names are invented, after the SWIG
names of the controller functions that fill them.
================
*/
// FUNCTION: STBC 0x004c9dc0 ??_GEffectControllerData@@UAEPAXI@Z
// FUNCTION: STBC 0x004c9de0 ??1EffectControllerData@@UAE@XZ
class EffectControllerData : public NiObject {
public:
	// FUNCTION: STBC 0x004c9d70 ?GetRTTI@EffectControllerData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	static const NiRTTI		rtti;

	KeyArray<NiLinColorKey>	colorKeys;		// 0x08
	KeyArray<NiLinFloatKey>	alphaKeys;		// 0x1C
	KeyArray<NiLinFloatKey>	sizeKeys;		// 0x30
};

// FUNCTION: STBC 0x004c9da0 ??_GEffectController@@UAEPAXI@Z
class EffectController : public NiTimeController {
public:
	// FUNCTION: STBC 0x004c9d80 ?GetRTTI@EffectController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~EffectController();

	virtual void			Start( float time );
	virtual void			Stop();

	// remove the key at the time, if there is one (names invented)
	virtual void			RemoveColorKey( float time );
	virtual void			RemoveAlphaKey( float time );
	virtual void			RemoveSizeKey( float time );

	// builds the effect's target; the base builds nothing
	// FUNCTION: STBC 0x004c9d90 ?CreateTarget@EffectController@@UAEXPAD@Z
	virtual void			CreateTarget( char *name ) {}
	virtual NiAVObject *	GetTargetCast();
	virtual void			AttachEffect( NiNode *parent );
	virtual void			DetachEffect();

	virtual void			AddColorKey( float time, float r, float g, float b );
	virtual void			RemoveAllColorKeys();
	virtual void			AddAlphaKey( float time, float alpha );
	virtual void			RemoveAllAlphaKeys();
	virtual void			AddSizeKey( float time, float size );
	virtual void			RemoveAllSizeKeys();

	// gives each empty track one key at time 0: white, opaque and of
	// size 1 (name invented)
	virtual void			AddDefaultKeys();

							EffectController( float lifeTime_ );
							EffectController( EffectControllerData *data_ );

	// the levels of detail the effects are built for
	enum EffectLevel {
		LOWEST,
		LOW,
		MEDIUM,
		HIGH,
		HIGHEST
	};

	static int				GetEffectLevel() { return effectLevel; }
	static void				SetEffectLevel( int level ) { effectLevel = level; }

	float					GetEffectLifeTime() { return lifeTime; }
	void					SetEffectLifeTime( float lifeTime_ ) { lifeTime = lifeTime_; }

	void					SetTargetAlphaBlendModes( int srcMode, int destMode );

	// the first EffectController of the target's list (name invented)
	static EffectController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;

protected:
	// the tracks' values at the time; the first key's before it and the
	// last key's after it (names invented)
	void					GetColor( float time, float &r, float &g, float &b );
	void					GetAlpha( float time, float &alpha );
	void					GetSize( float time, float &size );
	void					GetFloatValue( KeyArray<NiLinFloatKey> &keys, float time, float &value );
	void					AddFloatKey( KeyArray<NiLinFloatKey> &keys, float time, float value );
	void					RemoveFloatKey( KeyArray<NiLinFloatKey> &keys, float time );

	void					CopyMembers( EffectController *dest );

	// the run state, packed in the flags (bits 4 and 5); the names are
	// invented
	enum {
		STATE_MASK			= 0x0003,
		STATE_POS			= 4,
		STATE_NEW			= 0,
		STATE_RUNNING		= 2,
		STATE_STOPPED		= 3
	};

	float					lifeTime;				// 0x30 seconds
	NiPointer<EffectControllerData>	data;			// 0x34

	static int				effectLevel;			// an EffectLevel
};

#endif /* !__EFFECTCONTROLLER_H__ */
