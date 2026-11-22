/*
===============================================================================

	TGPhonemeSequence.h

	The lip sync of a line of speech: a timed action that steps through
	its phonemes as the game time passes, showing each on the character's
	overlay controllers when its time comes. Every playing sequence is in
	a static list, which the actions module advances each frame.

	0x50 bytes, allocated from the pools. Evidence: the constructor
	(0x7087C0) calls TGTimedAction's, builds an array of 128 entries
	growing by one at 0x38, sets the elapsed time at 0x30 to -1 and keeps
	a reference to the overlay manager at 0x28. Play (0x708AA0) clears the
	elapsed time, appends the sequence to the list at 0x99B698 and looks
	the first entry's phoneme up in the manager; 0x708B60 advances the
	elapsed time and steps through the entries; 0x708D50 walks the list
	and calls it on each sequence with the frame's game time. The
	destructor (0x708880) takes the sequence out of the list and deletes
	the entries. An entry (0x0C bytes, built by 0x7089A0) holds the
	phoneme's ID in the manager (saved with WriteID), its time and its
	duration. The load's constructor (0x6FD740) and the creator (0x6FD710,
	type 0x70F) are the actions module's. The vtable 0x8965F4 has
	TGAction's 24 slots; GetObjType returns 0x70F and IsTypeOf accepts
	TGTimedAction's and TGAction's types too. The class strings are
	"TGPhonemeSequence", "_p_TGPhonemeSequence", "TGPhonemeSequencePtr"
	and "CT_TG_PHONEME_SEQUENCE". The entry's class and the member names
	are invented.

	TGPhonemeSequence.cpp holds TGPhonemeAction's members too: they come
	between the list's initialiser and the sequence's members.

===============================================================================
*/

#ifndef __TGPHONEMESEQUENCE_H__
#define __TGPHONEMESEQUENCE_H__

#include "TGTimedAction.h"
#include "TGOverlayManager.h"
#include "NiPointer.h"
#include "NiTArray.h"
#include "NiTList.h"

// GetObjType's value for a TGPhonemeSequence.
#define TGPHONEMESEQUENCE_TYPE	0x70F

// One phoneme of a sequence.
struct TGPhonemeSequenceEntry {
	long			phonemeID;		// 0x00 the phoneme's key in the overlay manager
	float			time;			// 0x04 seconds into the sequence
	float			duration;		// 0x08
};

// FUNCTION: STBC 0x006fd860 ??_GTGPhonemeSequence@@UAEPAXI@Z @game/TGActionModule
class TGPhonemeSequence : public TGTimedAction {
public:
					TGPhonemeSequence( TGOverlayManager *manager_ );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006fd740 ??0TGPhonemeSequence@@QAE@J@Z @game/TGActionModule
					TGPhonemeSequence( long id ) : TGTimedAction( id ) { manager = NULL; }
	virtual			~TGPhonemeSequence( void );

	// TGObject
	// FUNCTION: STBC 0x006fd7e0 ?GetObjType@TGPhonemeSequence@@UAEHXZ @game/TGActionModule
	virtual int		GetObjType( void ) { return TGPHONEMESEQUENCE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006fd830 ?GetClassName@TGPhonemeSequence@@UAEPADXZ @game/TGActionModule
	virtual char *	GetClassName( void ) { return "TGPhonemeSequence"; }
	// FUNCTION: STBC 0x006fd840 ?GetSwigTypeName@TGPhonemeSequence@@UAEPADXZ @game/TGActionModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGPhonemeSequence"; }
	// FUNCTION: STBC 0x006fd850 ?GetPythonClassName@TGPhonemeSequence@@UAEPADXZ @game/TGActionModule
	virtual char *	GetPythonClassName( void ) { return "TGPhonemeSequencePtr"; }

	// TGAction
	virtual void	Play( void );
	virtual bool	PlaysOn( TGAnimNode *node );

	// Appends the phoneme's entry (invented name).
	void			AddEntry( long phonemeID, float time, float duration );

	// Advances the sequence by the time.
	void			Update( float time );						// invented name

	// Advances every playing sequence by the time.
	static void		UpdateAll( float time );					// invented name

protected:
	NiPointer<TGOverlayManager>	manager;	// 0x28
	long			savedManager;			// 0x2C the manager's address when saved, until PostFixUp
	float			elapsed;				// 0x30 seconds since Play; -1 before
	float			nextTime;				// 0x34 the time of the next entry
	NiTArray<TGPhonemeSequenceEntry *>	entries;	// 0x38 owned

	// Every playing sequence.
	static NiTList<TGPhonemeSequence *>	sequences;
};

/*
================
TGPhonemeSequence::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fd7f0 @game/TGActionModule
inline bool TGPhonemeSequence::IsTypeOf( int type ) {
	if ( type == TGPHONEMESEQUENCE_TYPE ) {
		return true;
	}
	return TGTimedAction::IsTypeOf( type );
}

#endif /* !__TGPHONEMESEQUENCE_H__ */
