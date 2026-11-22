/*
===============================================================================

	TGOverlayManager.h

	A character's named animation controllers: each TGOverlayController is
	filed under a name, and an overlay (a named texture or morph target) is
	shown on a controller while some action holds it. Only the calls the
	overlay action makes are declared yet. The class and its names are
	invented.

	0x30 bytes, a NetImmerse object (reference counted, held through
	NiPointer). Evidence: CharacterClass's setup (0x668430) allocates 0x30
	bytes, runs the constructor 0x7058F0 (NiObject's, two string maps at 8
	and 0x18 with the vtables 0x896888 and 0x896874, then the vtable
	0x896844, whose only override is the destructor 0x7059D0) and keeps it
	at 0x194. 0x706220 looks a controller up by name, checks it against
	TGOverlayController's NiRTTI (0x980B20, "TGOverlayController"), and
	either adds the overlay at once (0x706410) or starts a TGOverlayAction
	for the duration given; the action's destructor removes it (0x706560).

===============================================================================
*/

#ifndef __TGOVERLAYMANAGER_H__
#define __TGOVERLAYMANAGER_H__

#include "NiObject.h"
#include "NiTMap.h"
#include "NiTStringMap.h"
#include "NiTList.h"
#include "NiPointer.h"
#include "TGOverlayController.h"

class TGPhoneme;

// What the manager keeps of one named controller (0x34 bytes; invented name).
// FUNCTION: STBC 0x00705ec0 ??1TGOverlayEntry@@QAE@XZ @game/TGOverlayManager
struct TGOverlayEntry {
	NiPointer<NiObject>	controller;				// 0x00 a TGOverlayController or a TGAnimNode
	NiTList<char *>	families;					// 0x04 overlay names that stand for a group of overlays (invented name)
	NiTList<char *>	shown;						// 0x1C copies of the overlay names shown on the controller (invented name)
					TGOverlayEntry( void ) : families( 2 ), shown( 2 ) {}
};
class TGSequence;
class TGStream;

/*
================
PhonemeCode

A lip sync code of a sound and the overlay manager's phoneme that shows it
(8 bytes; CharacterAction's table at 0x9574B0 holds 0x95 of them, "0x0000"
on). A .LIP file holds indices into such a table. The name is invented.
================
*/
struct PhonemeCode {
	char *			code;			// as the .LWV file's markers name it
	int				phoneme;		// the phoneme's ID in the overlay manager
};

// FUNCTION: STBC 0x007059d0 ??_GTGOverlayManager@@UAEPAXI@Z @game/TGOverlayManager
class TGOverlayManager : public NiObject {
public:
					TGOverlayManager( void );
	virtual			~TGOverlayManager( void );

	// Shows the overlay on the named controller, once.
	void			AddOverlay( char *controller, char *overlay );
	// Takes back what AddOverlay showed.
	void			RemoveOverlay( char *controller, char *overlay );
	// Shows the overlay on the named controller for the duration (invented
	// name): at once and for good without one, else through a
	// TGOverlayAction.
	void			ShowOverlay( char *controller, char *overlay, float duration );
	// Files a one-entry phoneme that shows the overlay on the controller for
	// the duration, and returns nothing (invented name).
	void			AddOverlayPhoneme( char *controller, char *overlay, float duration );
	// Builds a sequence of random phonemes whose IDs lie from firstID to
	// lastID (the last filed without one; the first is 1 without one) until
	// the duration is filled, and saves the picks to the file when one is
	// given; NULL for a duration of 0 or less (invented name).
	TGSequence *	BuildRandomSequence( float duration, char *saveFile, int firstID, int lastID );
	// Builds a sequence from a saved lip sync: the stream holds a phoneme's
	// ID, time and duration per record up to the position given (the end of
	// the stream without one); the IDs go through the phoneme code table when
	// there is one (its entries' phonemes), and neighbouring records of
	// one phoneme are joined (invented name; the third argument is unused).
	TGSequence *	BuildSequenceFromStream( TGStream *stream, PhonemeCode *codes, int unused, int endPos );
	// Writes a .LIP file of the .LWV file's sound markers ("PMRK" on): per
	// marker the index of its code in the table, its start and its length
	// in seconds (invented name; 0x706970, which CharacterAction's
	// ConvertFolderLWVs calls).
	static void		ConvertLWVToLIP( char *lwvFile, char *lipFile, PhonemeCode *codes, int numCodes );
	// Files the controller (a TGOverlayController or a TGAnimNode) under the
	// name, unless the name has one already (invented name).
	void			AddController( NiObject *controller, char *name );
	// Adds the family name to the controller's families, unless it has it
	// already (invented name).
	void			AddFamily( char *controller, char *family );
	// The controller filed under the name, when someone else still holds it
	// (invented name).
	NiObject *		GetController( char *name );
	// Drops the controller filed under the name (invented name; nothing in
	// retail calls it).
	void			RemoveController( char *name );
	// Takes the family name out of the controller's families (invented
	// name; nothing in retail calls it).
	void			RemoveFamily( char *controller, char *family );
	// Plays a copy of the phoneme at its own duration (invented name).
	void			ShowPhoneme( TGPhoneme *phoneme );
	// Files the phoneme under the next ID and returns the ID; 0 for no
	// phoneme.
	long			AddPhoneme( TGPhoneme *phoneme );
	// The phoneme filed under the ID; NULL when there is none.
	TGPhoneme *		GetPhoneme( long id );

	// Whether building a phoneme sequence (0x706C60) prints its steps to the
	// debugger (invented name; CharacterClass::ShowPhonemeDebugging sets it).
	static bool		showPhonemeDebugging;

	// Shows what the controllers have been given since (invented name).
	void			ApplyOverlays( void );

	// Sets unknown2C; the overlays are shown anew when it comes on (placeholder name:
	// CharacterClass::Update turns it on while the character speaks).
	void			Unknown707240( bool value );

protected:
	// The overlay name of the controller's families that the overlay
	// contains; NULL when none (invented name).
	char *			FindFamily( char *controller, char *overlay );
	// Takes back the controller's shown overlay that contains the family's
	// name (invented name).
	void			RemoveFamilyOverlay( char *controller, char *family );
	NiTStringMap<TGOverlayEntry *>	controllers;	// 0x08 by controller name (vtable 0x896888; invented name)
	NiTMap<long, TGPhoneme *>	phonemes;		// 0x18 by ID (vtable 0x896874; invented name)
	long			nextPhonemeID;				// 0x28 the ID AddPhoneme gives next (invented name)
	bool			unknown2C;					// 0x2C set by the constructor
};

#endif /* !__TGOVERLAYMANAGER_H__ */
