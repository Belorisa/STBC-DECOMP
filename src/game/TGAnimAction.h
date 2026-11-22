/*
===============================================================================

	TGAnimAction.h

	An action that plays an animation of the animation manager on an
	animated node, and completes when the animation ends (the node calls
	back with the action's ID), or after its length when timed.

	0x38 bytes, allocated from the pools. Evidence: the constructor
	(0x703F90, which TGAnimAction_Create calls with the node, the
	animation's name and four ints the Python call may leave out) calls
	TGTimedAction's, stores the vtable 0x896474 (TGAction's 24 slots), takes
	a reference to the node at 0x28, copies the name to 0x30, keeps the
	four flags at 0x34 to 0x37 and sets the duration to the animation's
	length; the destructor (0x7040A0) deletes the name and releases the
	node. GetObjType returns 0x709. The load's constructor (0x6FD300) and
	the creator (0x6FD2D0) are the actions module's. The SWIG wrappers name
	Create, Play, Restart, Completed and Skip. The flag at 0x36 is passed on
	to the blender's unknown55, whose meaning is not known yet.

===============================================================================
*/

#ifndef __TGANIMACTION_H__
#define __TGANIMACTION_H__

#include "TGTimedAction.h"
#include "TGAnimNode.h"
#include "NiPointer.h"

// GetObjType's value for a TGAnimAction.
#define TGANIMACTION_TYPE		0x709

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006fd3a0 ??_GTGAnimAction@@UAEPAXI@Z @game/TGActionModule
class TGAnimAction : public TGTimedAction {
public:
					TGAnimAction( TGAnimNode *node_, char *animation_, int exclusive_, int unknown36_, int useDefault_, int blend_ );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006fd300 ??0TGAnimAction@@QAE@J@Z @game/TGActionModule
					TGAnimAction( long id ) : TGTimedAction( id ) {}
	virtual			~TGAnimAction( void );

	// TGObject
	// FUNCTION: STBC 0x006fd320 ?GetObjType@TGAnimAction@@UAEHXZ @game/TGActionModule
	virtual int		GetObjType( void ) { return TGANIMACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006fd370 ?GetClassName@TGAnimAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetClassName( void ) { return "TGAnimAction"; }
	// FUNCTION: STBC 0x006fd380 ?GetSwigTypeName@TGAnimAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGAnimAction"; }
	// FUNCTION: STBC 0x006fd390 ?GetPythonClassName@TGAnimAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetPythonClassName( void ) { return "TGAnimActionPtr"; }

	// TGAction
	virtual void	Play( void );
	virtual void	Restart( float delay = 0.0f );
	virtual void	Skip( void );
	virtual void	Completed( void );
	virtual bool	PlaysOn( TGAnimNode *node_ );

protected:
	// The node's callback when the animation ends: the action of the ID
	// completes (invented name).
	static void		AnimationDone( long id );

	NiPointer<TGAnimNode>	node;			// 0x28
	long			savedNode;				// 0x2C the node's address when saved, until PostFixUp (invented name)
	char *			animation;				// 0x30 a copy of the animation's name
	// The flags' names are invented, read from the TGAnimNode calls they
	// choose: exclusive picks SetExclusiveAnimation over the non-exclusive
	// one and is UseAnimation's flag that TGAnimNode stores as the
	// blender's exclusive; useDefault picks SetExclusiveAnimationUseDefault;
	// blend picks the Set*Animation calls, which blend in over the node's
	// blend time, over UseAnimation, which clears the blender's blend.
	bool			exclusive;				// 0x34 (name invented)
	bool			useDefault;				// 0x35 (name invented)
	bool			unknown36;				// 0x36 the action also completes after its length
	bool			blend;					// 0x37 the animation is blended in (name invented)

	// Declared for the SWIG wrappers that call them.
public:
	static TGAnimAction *	Create( TGAnimNode *node, char *animation, int exclusive, int unknown36, int useDefault, int blend ) { return new TGAnimAction( node, animation, exclusive, unknown36, useDefault, blend ); }	// the SWIG wrapper TGAnimAction_Create expands it
};

/*
================
TGAnimAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fd330 ?IsTypeOf@TGAnimAction@@UAE_NH@Z @game/TGActionModule
inline bool TGAnimAction::IsTypeOf( int type ) {
	if ( type == TGANIMACTION_TYPE ) {
		return true;
	}
	return TGTimedAction::IsTypeOf( type );
}

#endif /* !__TGANIMACTION_H__ */
