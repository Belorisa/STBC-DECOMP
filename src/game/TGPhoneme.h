/*
===============================================================================

	TGPhoneme.h

	A mouth shape of the lip sync: the overlays (named textures or morph
	targets) to show on a character's overlay controllers, each at a time
	within the phoneme. The phoneme lasts as long as its latest entry.
	The phoneme actions and sequences hold copies of them and show them
	through the character's overlay manager.

	0x1C bytes, allocated with operator new. Evidence: the constructor
	(0x707910) builds a list of two items per block at 4 and clears the
	duration at 0; the destructor (0x707930) frees each entry's two
	strings and the entry, then the list. An entry (0x10 bytes, built by
	0x7079B0) holds copies of the controller's and the overlay's names
	and two floats. The saved games write each entry after a true and end
	the list with a false ("Saving Phoneme %s on %s\n", "Loading Phoneme %s
	on %s\n"). The SWIG type string "_p_TGPhoneme" names the class; the
	entry's class and every member name are invented.

===============================================================================
*/

#ifndef __TGPHONEME_H__
#define __TGPHONEME_H__

#include "NiTList.h"

class TGStream;

// One overlay of a phoneme.
struct TGPhonemeEntry {
	char *			controller;		// 0x00 owned copy: the overlay controller's name
	char *			overlay;		// 0x04 owned copy: the overlay's name
	float			time;			// 0x08 seconds into the phoneme
	float			unknown0C;		// 0x0C
};

class TGPhoneme {
public:
					TGPhoneme( void );
					~TGPhoneme( void );

	// Adds a copy of the overlay; the phoneme lasts at least until its time.
	void			AddEntry( char *controller, char *overlay, float time, float unknown );
	// Adds the entries of each phoneme of the group.
	void			AddPhonemes( TGPhoneme *group );
	// A new phoneme with copies of the entries.
	TGPhoneme *		Clone( void );

	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );

	// The first entry's names and time; NULL or 0 without entries.
	char *			GetController( void );
	char *			GetOverlay( void );
	float			GetTime( void );

	// A new phoneme holding the entry at the index alone; NULL when there is none.
	TGPhoneme *		GetEntryPhoneme( int index );
	// The same, taking the entry out of the phoneme; the entry itself is not freed.
	TGPhoneme *		RemoveEntryPhoneme( int index );

	float			GetDuration( void ) { return duration; }
	void			SetDuration( float duration_ ) { duration = duration_; }

	float			duration;					// 0x00 seconds: the latest entry's time
	NiTList<TGPhonemeEntry *>	entries;		// 0x04
};

#endif /* !__TGPHONEME_H__ */
