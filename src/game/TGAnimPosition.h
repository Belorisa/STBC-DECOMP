/*
===============================================================================

	TGAnimPosition.h

	An action that puts a node in the position an animation gives it, at
	once: Play applies the animation's position, if the animation is
	loaded, and completes.

	0x2C bytes, allocated from the pools.

	Evidence: the constructor (0x704730) calls TGAction's, stores the vtable
	0x8964D4 (TGAction's 24 slots), holds a reference to the node and copies
	the animation's name; the destructor (0x7047F0) frees them. GetObjType
	returns 0x70B. The SWIG wrappers TGAnimPosition_Create ("OO") and Play
	call the constructor and slot 16. The class strings are
	"TGAnimPosition", "_p_TGAnimPosition" and "TGAnimPositionPtr". The node
	is saved by its address and found again after a load through the map
	of the loaded NetImmerse objects (0x99A418).

===============================================================================
*/

#ifndef __TGANIMPOSITION_H__
#define __TGANIMPOSITION_H__

#include "TGAction.h"
#include "TGAnimNode.h"
#include "NiPointer.h"

// GetObjType's value for a TGAnimPosition.
#define TGANIMPOSITION_TYPE		0x70B

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006fd490 ??_GTGAnimPosition@@UAEPAXI@Z @game/TGActionModule
class TGAnimPosition : public TGAction {
public:
					TGAnimPosition( TGAnimNode *node_, char *animation_ );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006fd400 ??0TGAnimPosition@@QAE@J@Z @game/TGActionModule
					TGAnimPosition( long id ) : TGAction( id ) {}
	virtual			~TGAnimPosition( void );

	// TGObject
	// FUNCTION: STBC 0x006fd420 ?GetObjType@TGAnimPosition@@UAEHXZ @game/TGActionModule
	virtual int		GetObjType( void ) { return TGANIMPOSITION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006fd460 ?GetClassName@TGAnimPosition@@UAEPADXZ @game/TGActionModule
	virtual char *	GetClassName( void ) { return "TGAnimPosition"; }
	// FUNCTION: STBC 0x006fd470 ?GetSwigTypeName@TGAnimPosition@@UAEPADXZ @game/TGActionModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGAnimPosition"; }
	// FUNCTION: STBC 0x006fd480 ?GetPythonClassName@TGAnimPosition@@UAEPADXZ @game/TGActionModule
	virtual char *	GetPythonClassName( void ) { return "TGAnimPositionPtr"; }

	// TGAction
	virtual void	Play( void );
	virtual bool	PlaysOn( TGAnimNode *node_ );

protected:
	NiPointer<TGAnimNode>	node;			// 0x20
	long			savedNode;				// 0x24 the node's address when saved, until PostFixUp (invented name)
	char *			animation;				// 0x28 a copy of the animation's name

	// Declared for the SWIG wrappers that call them.
public:
	static TGAnimPosition *	Create( TGAnimNode *node, char *animation ) { return new TGAnimPosition( node, animation ); }	// the SWIG wrapper TGAnimPosition_Create expands it
};

/*
================
TGAnimPosition::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fd430 @game/TGActionModule
inline bool TGAnimPosition::IsTypeOf( int type ) {
	if ( type == TGANIMPOSITION_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

#endif /* !__TGANIMPOSITION_H__ */
