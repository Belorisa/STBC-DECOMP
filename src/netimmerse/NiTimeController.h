/*
===============================================================================

	NiTimeController.h

	The base of the animation controllers. The controllers of one
	NiObjectNET form a singly linked list.

	Evidence: the vtable at 0x898BF8 (20 slots, two of them _purecall); the
	constructor (0x7D9B60) and destructor (0x7D9C10); the viewer strings
	name the members ("m_fFrequency", "m_fPhase", "m_fLoKeyTime",
	"m_fHiKeyTime", "m_fStartTime", "m_fLastTime", "m_pTarget") and the two
	enumerations packed in the flags ("anim type": APP_TIME, APP_INIT;
	"cycle type": LOOP, REVERSE, CLAMP).

===============================================================================
*/

#ifndef __NITIMECONTROLLER_H__
#define __NITIMECONTROLLER_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

class NiObjectNET;

/*
================
NiTimeControllerLoadRecord

The link IDs NiTimeController::LoadBinary (0x7DA460) reads: the next
controller's (saved first) and the target's (saved last). The class name
is invented. Its vtable 0x88DBF0 (one slot, the scalar deleting
destructor) is the record of the controllers without one of their own
(SpinController::CreateObject); the copies of its implicit destructors
the linker kept come from FlareController's unit.
================
*/
// FUNCTION: STBC 0x004f2bf0 ??_GNiTimeControllerLoadRecord@@UAEPAXI@Z @game/FlareController
// FUNCTION: STBC 0x004f2c10 ??1NiTimeControllerLoadRecord@@UAE@XZ @game/FlareController
class NiTimeControllerLoadRecord : public NiObjectLoadRecord {
public:
							// FUNCTION: STBC 0x7A9820 ??0NiTimeControllerLoadRecord@@QAE@XZ @netimmerse/NiMorphController
							NiTimeControllerLoadRecord() { targetLinkID = 0; nextLinkID = 0; }

	unsigned int			targetLinkID;
	unsigned int			nextLinkID;
};

// FUNCTION: STBC 0x007daa60 ??_GNiTimeController@@UAEPAXI@Z @netimmerse/NiTimeController
class NiTimeController : public NiObject {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7D9A10)
	static int				Register();
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// FUNCTION: STBC 0x4E3530 ?GetRTTI@NiTimeController@@UBEPBVNiRTTI@@XZ @netimmerse/NiTimeController
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );
	virtual void			VirtualSlot03();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiTimeController();

	virtual void			Start( float time );
	virtual void			Stop();
	virtual void			Update( float time ) = 0;
	// FUNCTION: STBC 0x455540 ?VirtualSlot15@NiTimeController@@UAEXXZ
	virtual void			VirtualSlot15() {}
	virtual void			SetTarget( NiObjectNET *target_ );
	// FUNCTION: STBC 0x455530 ?VirtualSlot17@NiTimeController@@UAEXXZ
	virtual void			VirtualSlot17() { if ( next ) next->VirtualSlot17(); }
	virtual float			ComputeScaledTime( float time );
	virtual bool			TargetIsRequiredType() const = 0;	// Gamebryo lineage name

							NiTimeController();

	NiObjectNET *			GetTarget() const;	// Gamebryo lineage name

	// the target's list
	NiTimeController *		GetNext() const;
	void					SetNext( NiTimeController *next_ );

	enum AnimType {
		APP_TIME,
		APP_INIT
	};

	enum CycleType {
		LOOP,
		REVERSE,
		CLAMP
	};

	// flags; SetField takes a field's width unshifted (a game unit's
	// out-of-line copy, 0x677890, shifts the mask by the position)
	enum {
		ANIMTYPE_MASK		= 0x0001,
		ANIMTYPE_BITS		= 0x0001,
		ANIMTYPE_POS		= 0,
		CYCLETYPE_MASK		= 0x0006,
		CYCLETYPE_BITS		= 0x0003,
		CYCLETYPE_POS		= 1,
		ACTIVE_MASK			= 0x0008,
		ACTIVE_POS			= 3
	};

	bool					GetActive() const;
	// true when an update at this time has nothing to do: the controller is
	// inactive or was updated at this time already (Gamebryo lineage name)
	bool					DontDoUpdate( float time );
	void					SetActive( bool active );	// Gamebryo lineage name
	// APP_TIME animations are always active (Gamebryo 1.1.2's SetAnimType)
	void					SetAnimType( AnimType type );	// Gamebryo lineage name
	AnimType				GetAnimType() const;	// Gamebryo lineage name
	void					SetCycleType( CycleType type );	// Gamebryo lineage name
	CycleType				GetCycleType() const;	// Gamebryo lineage name
	void					SetFrequency( float frequency_ );	// Gamebryo lineage name
	float					GetFrequency() const;	// Gamebryo lineage name
	void					SetPhase( float phase_ );	// Gamebryo lineage name
	float					GetPhase() const;	// Gamebryo lineage name
	void					SetBeginKeyTime( float begin );	// Gamebryo lineage name
	float					GetBeginKeyTime() const;	// Gamebryo lineage name
	void					SetEndKeyTime( float end );	// Gamebryo lineage name
	float					GetEndKeyTime() const;	// Gamebryo lineage name
	// for advanced users; NiAnimBlender uses it to blend and morph (Gamebryo
	// lineage, NITIMECONTROLLER.INL)
	float					GetStartTime() const;

	// FUNCTION: STBC 0x4D4F60 ?SetBit@NiTimeController@@QAEX_NG@Z
	void					SetBit( bool value, unsigned short pos ) { if ( value ) { flags |= 1 << pos; } else { flags &= ~( 1 << pos ); } }
	bool					GetBit( unsigned short pos ) const { return ( flags >> pos ) & 1; }
	// a field of several bits (Gamebryo lineage names)
	// FUNCTION: STBC 0x677890 ?SetField@NiTimeController@@QAEXGGG@Z @game/ViewScreenNode
	void					SetField( unsigned short value, unsigned short mask, unsigned short pos ) { flags = ( flags & ~( mask << pos ) ) | ( value << pos ); }
	unsigned short			GetField( unsigned short mask, unsigned short pos ) const { return ( flags >> pos ) & mask; }

	unsigned int			ItemsInList() const;

	// start or stop every controller of the object and of the objects
	// under it: its extra data's and, for a node, its children's
	// (Gamebryo lineage names; 0x7D9FC0, 0x7DA140)
	static void				StartAnimations( NiObjectNET *object );
	static void				StartAnimations( NiObjectNET *object, float time );
	static void				StopAnimations( NiObjectNET *object );

	static char *			GetViewerString( const char *prefix, AnimType type );
	static char *			GetViewerString( const char *prefix, CycleType type );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTimeController *dest );

public:
	unsigned short			flags;		// the flags above
	float					frequency;
	float					phase;
	float					loKeyTime;
	float					hiKeyTime;
	float					startTime;
	float					lastTime;
	float					scaledTime;		// the scaled time, accumulated
	NiObjectNET *			target;
	NiPointer<NiTimeController>	next;	// the next controller of the target
};

NiRegisterStream( NiTimeController );

/*
================
NiTimeController::SetAnimType
================
*/
inline void NiTimeController::SetAnimType( AnimType type ) {
	SetField( (unsigned short)type, ANIMTYPE_BITS, ANIMTYPE_POS );
	SetActive( type == APP_TIME );
}

/*
================
NiTimeController::GetAnimType

AnimOnly.exe 0x412FA0.
================
*/
inline NiTimeController::AnimType NiTimeController::GetAnimType() const {
	return (AnimType)GetField( ANIMTYPE_BITS, ANIMTYPE_POS );
}

/*
================
NiTimeController::SetCycleType
================
*/
inline void NiTimeController::SetCycleType( CycleType type ) {
	SetField( (unsigned short)type, CYCLETYPE_BITS, CYCLETYPE_POS );
}

/*
================
NiTimeController::GetCycleType
================
*/
inline NiTimeController::CycleType NiTimeController::GetCycleType() const {
	return (CycleType)GetField( CYCLETYPE_BITS, CYCLETYPE_POS );
}

/*
================
NiTimeController::SetFrequency
================
*/
inline void NiTimeController::SetFrequency( float frequency_ ) {
	frequency = frequency_;
}

/*
================
NiTimeController::GetFrequency
================
*/
inline float NiTimeController::GetFrequency() const {
	return frequency;
}

/*
================
NiTimeController::SetPhase
================
*/
inline void NiTimeController::SetPhase( float phase_ ) {
	phase = phase_;
}

/*
================
NiTimeController::GetPhase
================
*/
inline float NiTimeController::GetPhase() const {
	return phase;
}

/*
================
NiTimeController::GetStartTime
================
*/
inline float NiTimeController::GetStartTime() const {
	return startTime;
}

/*
================
NiTimeController::SetBeginKeyTime
================
*/
inline void NiTimeController::SetBeginKeyTime( float begin ) {
	loKeyTime = begin;
}

/*
================
NiTimeController::GetBeginKeyTime
================
*/
inline float NiTimeController::GetBeginKeyTime() const {
	return loKeyTime;
}

/*
================
NiTimeController::SetEndKeyTime
================
*/
inline void NiTimeController::SetEndKeyTime( float end ) {
	hiKeyTime = end;
}

/*
================
NiTimeController::GetEndKeyTime
================
*/
inline float NiTimeController::GetEndKeyTime() const {
	return hiKeyTime;
}

/*
================
NiTimeController::GetTarget
================
*/
inline NiObjectNET *NiTimeController::GetTarget() const {
	return target;
}

/*
================
NiTimeController::SetNext
================
*/
inline void NiTimeController::SetNext( NiTimeController *next_ ) {
	next = next_;
}

/*
================
NiTimeController::GetNext
================
*/
inline NiTimeController *NiTimeController::GetNext() const {
	return next;
}

/*
================
NiTimeController::SetActive
================
*/
inline void NiTimeController::SetActive( bool active ) {
	SetBit( active, ACTIVE_POS );
}

/*
================
NiTimeController::GetActive
================
*/
inline bool NiTimeController::GetActive() const {
	return GetBit( ACTIVE_POS );
}

/*
================
NiTimeController::DontDoUpdate

AnimOnly.exe 0x452060.
================
*/
inline bool NiTimeController::DontDoUpdate( float time ) {
	return !GetActive() || lastTime == time;
}

#endif /* !__NITIMECONTROLLER_H__ */
