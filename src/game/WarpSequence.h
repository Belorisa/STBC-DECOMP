/*
===============================================================================

	WarpSequence.h

	The sequence that warps a ship to another set: four sub-sequences
	(before, during, after the jump into the warp set, and after the warp)
	and the script actions of the warp's stages, which the script
	WarpSequence.SetupSequence puts together when the sequence plays.

	0x90 bytes. Evidence: the constructor (0x4D5B00) runs TGSequence's,
	stores the vtable 0x88B154 (TGAction's 24 slots: the destructor,
	GetObjType, IsTypeOf, the stream functions, the class strings, Play,
	Skip and Completed overridden, and a slot 24 of its own, SetEventDestination), keeps the ship's ID and set, copies the
	destination and placement names and makes the sub-sequences and
	actions; the stream loader's creator (UtopiaModule's unit, 0x4418E0)
	allocates 0x90 bytes and expands the loader constructor (0x441920).
	The class strings are "WarpSequence", "_p_WarpSequence" and
	"WarpSequencePtr"; GetObjType returns 0x80FE. Their copies, and the
	deleting destructor's, are in UtopiaModule's unit.

===============================================================================
*/

#ifndef __WARPSEQUENCE_H__
#define __WARPSEQUENCE_H__

#include "TGSequence.h"
#include "ShipClass.h"
#include "TGPoint3.h"

class SetClass;
class TGAction;
class TGObject;

// GetObjType's value for a WarpSequence (the type its Cast checks).
#define WARPSEQUENCE_TYPE		0x80FE

// The event that deletes its destination (the SWIG constant of the App module).
#define ET_DELETE_OBJECT_PUBLIC	1

// The pieces GetWarpSequencePiece answers (the SWIG constants WarpSequence_*).
enum warpSequencePiece_t {
	PRE_WARP_SEQUENCE,
	DURING_WARP_SEQUENCE,
	POST_DURING_WARP_SEQUENCE,
	POST_WARP_SEQUENCE,
	WARP_BEGIN_ACTION,
	WARP_END_ACTION,
	DEWARP_BEGIN_ACTION,
	DEWARP_END_ACTION,
	WARP_ENTER_ACTION,
	DEWARP_FINISH_ACTION,
	MOVE_ACTION_1,
	MOVE_ACTION_2
};

// The SWIG inline accessors read GetOrigin 0x38, GetDestination 0x3C,
// GetPlacementName 0x40, GetTimeToWarp 0x44, GetDestinationEpisode 0x48 and
// GetDestinationMission 0x4C, and GetShip looks up the ID at 0x34. The four
// AddAction* functions append to the sub-sequences whose IDs are at
// 0x50-0x5C; GetDestinationSet asks the destination module for its set.
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ShipClass;
// FUNCTION: STBC 0x004419c0 ??_GWarpSequence@@UAEPAXI@Z
class WarpSequence : public TGSequence {
public:
					WarpSequence( ShipClass *ship, char *destination_, float timeToWarp_, char *placementName_ );	// parameter names invented
	// FUNCTION: STBC 0x00441920 ??0WarpSequence@@QAE@J@Z @game/UtopiaModule
					WarpSequence( long id = 0 ) : TGSequence( id ) {}
	virtual			~WarpSequence( void );

	// TGObject
	// FUNCTION: STBC 0x00441940 ?GetObjType@WarpSequence@@UAEHXZ
	virtual int		GetObjType( void ) { return WARPSEQUENCE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00441990 ?GetClassName@WarpSequence@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "WarpSequence"; }
	// FUNCTION: STBC 0x004419a0 ?GetSwigTypeName@WarpSequence@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_WarpSequence"; }
	// FUNCTION: STBC 0x004419b0 ?GetPythonClassName@WarpSequence@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "WarpSequencePtr"; }

	// TGAction
	virtual void	Play( void );
	virtual void	Skip( void );
	virtual void	Completed( void );

	// The object sent ET_EXITED_WARP when the sequence completes (vtable slot 24, 0x4D65B0).
	virtual void	SetEventDestination( TGObject *destination );

	static WarpSequence *	Cast( TGObject * );
	void	AddActionBeforeWarp( TGAction *, float );
	void	AddActionDuringWarp( TGAction *, float );
	void	AddActionPostDuringWarp( TGAction *, float );
	void	AddActionAfterWarp( TGAction *, float );
	SetClass *	GetDestinationSet( void );
	static SetClass *	GetWarpSet( void );
	void	SetPlacement( char * );
	TGAction *	GetWarpSequencePiece( int );
	void	SetDestinationEpisode( char *episode );		// invented name
	void	SetDestinationMission( char *mission );		// invented name

	SetClass *		GetOrigin( void ) { return origin; }
	char *			GetDestination( void ) { return destination; }
	char *			GetPlacementName( void ) { return placementName; }
	float			GetTimeToWarp( void ) { return timeToWarp; }
	char *			GetDestinationEpisode( void ) { return destinationEpisode; }
	char *			GetDestinationMission( void ) { return destinationMission; }
	void			SetExitPoint( TGPoint3 *point ) { exitPoint = *point; }
	// FUNCTION: STBC 0x0061f9c0 ?GetExitPoint@WarpSequence@@QAE?AVTGPoint3@@XZ @game/AppWrap
	TGPoint3		GetExitPoint( void ) { return exitPoint; }

protected:
	// Lets the script WarpSequence.SetupSequence put the pieces together
	// (invented name).
	void			SetupSequence( void );

	long			shipID;					// 0x34 the ship that warps (invented name)
	SetClass *		origin;					// 0x38
	char *			destination;			// 0x3C the destination's module name, a heap copy
	char *			placementName;			// 0x40 a heap copy
	float			timeToWarp;				// 0x44
	char *			destinationEpisode;		// 0x48 a heap copy
	char *			destinationMission;		// 0x4C a heap copy
	long			beforeWarpID;			// 0x50 the sub-sequences (invented names)
	long			duringWarpID;			// 0x54
	long			postDuringWarpID;		// 0x58
	long			afterWarpID;			// 0x5C
	long			warpBeginActionID;		// 0x60 the actions of the warp, by piece (invented names)
	long			warpEndActionID;		// 0x64
	long			dewarpBeginActionID;	// 0x68
	long			dewarpEndActionID;		// 0x6C
	long			warpEnterActionID;		// 0x70
	long			dewarpFinishActionID;	// 0x74
	long			moveAction1ID;			// 0x78
	long			moveAction2ID;			// 0x7C
	long			eventDestinationID;		// 0x80 SetEventDestination's object; 0 at first (invented name)
	NiPoint3		exitPoint;				// 0x84 SetExitPoint; passed on in the start warp event

	// Declared for the SWIG wrappers that call them.
public:
	static WarpSequence *	Create( ShipClass *, char *, float, char * );	// retail 0x4D6350: the SWIG wrapper WarpSequence_Create calls it
	static ShipClass *	GetShip( WarpSequence *sequence ) { return ShipClass::GetObjectByID( NULL, sequence->shipID ); }	// the SWIG wrapper WarpSequence_GetShip expands it
};

/*
================
WarpSequence::IsTypeOf
================
*/
// FUNCTION: STBC 0x00441950
inline bool WarpSequence::IsTypeOf( int type ) {
	if ( type == WARPSEQUENCE_TYPE ) {
		return true;
	}
	return TGSequence::IsTypeOf( type );
}

#endif /* !__WARPSEQUENCE_H__ */
