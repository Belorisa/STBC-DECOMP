/*
===============================================================================

	TGMovieAction.h

	An action that plays a movie through g_kMovieManager: Play opens it
	(switching to movie mode first if asked), Skip and Completed stop it
	and switch out of movie mode unless asked not to; a repeating movie
	plays again when it completes.

	0x4C bytes, allocated from the pools.

	Evidence: the SWIG wrappers TGMovieAction_* name the methods, and
	TGMovieAction_Repeats reads the bool at 0x30. The load's creator
	(0x6AE150, type 0x901) calls the ID constructor 0x6AE180, which calls
	TGAction's and stores the vtable 0x8956AC; the script constructor
	(0x6B0620) copies the movie's file name into 0x20, sets both offsets to
	-1 and keeps its four flags at 0x25, 0x24, 0x30 and 0x32. Play
	(0x6B0820) switches to movie mode when 0x25 is set and passes 0x32 to
	the manager's AddMovie; Skip (0x6B0860) and Completed (0x6B08A0) do
	nothing once 0x31 is set and switch out of movie mode unless 0x24 is
	set. The vtable's slots 0, 1, 2, 4, 5, 9, 10, 11, 16, 18 and 20 are its
	own.

===============================================================================
*/

#ifndef __TGMOVIEACTION_H__
#define __TGMOVIEACTION_H__

#include "TGAction.h"
#include "SortedArray.h"

// An action to run when a movie reaches a frame (invented name). The movie's
// list of them is sorted by frame; deleting an entry skips its action.
class MovieFrameAction {
public:
					MovieFrameAction( void ) { actionID = 0; frame = 0; skipWhenStopped = true; }
					// Skips the action, which is done by then if it already ran.
					~MovieFrameAction( void ) {
						TGAction *action = (TGAction *)TGObject::GetTGObjectPtr( actionID );
						if ( action != NULL ) {
							action->Skip();
						}
					}

	// Positive when the frame sorts after the entry's.
	int				CompareKey( int frame_ ) {
						if ( frame_ == frame ) {
							return 0;
						}
						if ( frame_ < frame ) {
							return -1;
						}
						return 1;
					}

	// Positive when the other entry's frame sorts after this one's.
	int				Compare( MovieFrameAction *other ) { return CompareKey( other->frame ); }

	long			actionID;			// 0x00 the TGAction, by its ID
	int				frame;				// 0x04
	bool			skipWhenStopped;	// 0x08 AddFrameAction's flag: a played action is skipped when the movie stops (name invented, from ClearFrameActions)
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// GetObjType's value for a TGMovieAction.
#define TGMOVIEACTION_TYPE		0x901

// FUNCTION: STBC 0x006ae220 ??_GTGMovieAction@@UAEPAXI@Z @game/TGMovieModule
class TGMovieAction : public TGAction {
public:
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006ae180 ??0TGMovieAction@@QAE@J@Z @game/TGMovieModule
					TGMovieAction( long id ) : TGAction( id ) {}
	virtual			~TGMovieAction( void );

	// TGObject
	// FUNCTION: STBC 0x006ae1b0 ?GetObjType@TGMovieAction@@UAEHXZ @game/TGMovieModule
	virtual int		GetObjType( void ) { return TGMOVIEACTION_TYPE; }
	// FUNCTION: STBC 0x006ae1c0 ?IsTypeOf@TGMovieAction@@UAE_NH@Z @game/TGMovieModule
	virtual bool	IsTypeOf( int type ) {
						if ( type == TGMOVIEACTION_TYPE ) {
							return true;
						}
						return TGAction::IsTypeOf( type );
					}
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x006ae1f0 ?GetClassName@TGMovieAction@@UAEPADXZ @game/TGMovieModule
	virtual char *	GetClassName( void ) { return "TGMovieAction"; }
	// FUNCTION: STBC 0x006ae200 ?GetSwigTypeName@TGMovieAction@@UAEPADXZ @game/TGMovieModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGMovieAction"; }
	// FUNCTION: STBC 0x006ae210 ?GetPythonClassName@TGMovieAction@@UAEPADXZ @game/TGMovieModule
	virtual char *	GetPythonClassName( void ) { return "TGMovieActionPtr"; }

	// TGAction
	virtual void	Play( void );
	virtual void	Skip( void );
	virtual void	Completed( void );

	char *	GetMovie( void ) { return movie; }		// invented name
	void	SetOffset( float, float );
	float	GetOffsetX( void ) { return offsetX; }
	float	GetOffsetY( void ) { return offsetY; }
	bool	Repeats( void ) { return repeats; }
	// Plays the action when the movie reaches the frame.
	void	AddFrameAction( TGAction *action, int frame, bool skipWhenStopped );
	void	DeleteFrameAction( int );
	// Plays the actions of the frames up to this one (invented name).
	void	PlayFrameActions( int frame );
	// Deletes the actions not played yet and skips the played ones (invented name).
	void	ClearFrameActions( void );

protected:
	char *			movie;				// 0x20 the movie's file name, a copy
	bool			stayInMovieMode;	// 0x24 Skip and Completed switch out of movie mode unless set (name invented)
	bool			switchToMovieMode;	// 0x25 Play calls SwitchToMovieMode first (name invented)
	float			offsetX;			// 0x28 SetOffset (invented name); -1 centres the movie
	float			offsetY;			// 0x2C
	bool			repeats;			// 0x30 Completed plays it again
	bool			skipped;			// 0x31 set by Skip; Play, Skip and Completed then do nothing (name invented)
	bool			skipFrames;			// 0x32 Play's AddMovie argument: BinkOpen without BINKNOSKIP, late frames skipped (name invented)
	SortedArray<MovieFrameAction, int>	frameActions;	// 0x34 (invented name)
	SortedArray<MovieFrameAction, int>	playedFrameActions;	// 0x40 the entries PlayFrameActions moved out of frameActions (invented name)

	// Declared for the SWIG wrappers that call them.
public:
					TGMovieAction( char *movie, bool switchToMovieMode, bool stayInMovieMode, bool repeats, bool skipFrames );
	static TGMovieAction *	Create( char *movie, bool switchToMovieMode, bool stayInMovieMode, bool repeats, bool skipFrames ) { return new TGMovieAction( movie, switchToMovieMode, stayInMovieMode, repeats, skipFrames ); }	// the SWIG wrapper TGMovieAction_Create expands it
};

#endif /* !__TGMOVIEACTION_H__ */
