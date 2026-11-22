/*
===============================================================================

	TGPhonemeAction.h

	A timed action that shows one phoneme on a character's overlay
	controllers for its duration (the phoneme's own unless one is given).

	0x34 bytes, allocated from the pools. Evidence: the constructor
	(0x7082A0) calls TGTimedAction's, keeps a copy of the phoneme
	(TGPhoneme::Clone) at 0x30 and a reference to the overlay manager at
	0x28, and takes the duration from the phoneme when it is given as -1;
	the destructor (0x708350) deletes the copy. The load's constructor
	(0x6FD5E0) and the creator (0x6FD5B0, type 0x70D) are the actions
	module's. The vtable 0x896594 has TGAction's 24 slots; GetObjType
	returns 0x70D and IsTypeOf accepts TGTimedAction's and TGAction's types
	too; slot 21 tells whether the action uses the manager. The class
	strings are "TGPhonemeAction", "_p_TGPhonemeAction",
	"TGPhonemeActionPtr" and "CT_TG_PHONEME_ACTION". The manager is saved
	by its address and found again through the pointer map after a load;
	the phoneme is saved after it, followed by the marker 1234.

===============================================================================
*/

#ifndef __TGPHONEMEACTION_H__
#define __TGPHONEMEACTION_H__

#include "TGTimedAction.h"
#include "TGOverlayManager.h"
#include "NiPointer.h"

class TGPhoneme;

// GetObjType's value for a TGPhonemeAction.
#define TGPHONEMEACTION_TYPE	0x70D

// The value the saved games write after a phoneme action (invented name).
#define PHONEME_ACTION_MARKER	1234

// FUNCTION: STBC 0x006fd6e0 ??_GTGPhonemeAction@@UAEPAXI@Z @game/TGActionModule
class TGPhonemeAction : public TGTimedAction {
public:
					TGPhonemeAction( TGOverlayManager *manager_, TGPhoneme *phoneme_, float duration_ );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006fd5e0 ??0TGPhonemeAction@@QAE@J@Z @game/TGActionModule
					TGPhonemeAction( long id ) : TGTimedAction( id ) { manager = NULL; }
	virtual			~TGPhonemeAction( void );

	// TGObject
	// FUNCTION: STBC 0x006fd660 ?GetObjType@TGPhonemeAction@@UAEHXZ @game/TGActionModule
	virtual int		GetObjType( void ) { return TGPHONEMEACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006fd6b0 ?GetClassName@TGPhonemeAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetClassName( void ) { return "TGPhonemeAction"; }
	// FUNCTION: STBC 0x006fd6c0 ?GetSwigTypeName@TGPhonemeAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGPhonemeAction"; }
	// FUNCTION: STBC 0x006fd6d0 ?GetPythonClassName@TGPhonemeAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetPythonClassName( void ) { return "TGPhonemeActionPtr"; }

	// TGAction
	virtual void	Play( void );
	virtual bool	PlaysOn( TGAnimNode *node );

protected:
	NiPointer<TGOverlayManager>	manager;	// 0x28
	long			savedManager;			// 0x2C the manager's address when saved, until PostFixUp (invented name)
	TGPhoneme *		phoneme;				// 0x30 owned copy
};

/*
================
TGPhonemeAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fd670 @game/TGActionModule
inline bool TGPhonemeAction::IsTypeOf( int type ) {
	if ( type == TGPHONEMEACTION_TYPE ) {
		return true;
	}
	return TGTimedAction::IsTypeOf( type );
}

#endif /* !__TGPHONEMEACTION_H__ */
