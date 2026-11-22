/*
===============================================================================

	AIManager.h

	The AIs in use: an entry for each AI set on an object, telling whether
	it is still building. The class and global names are invented: no
	string or SWIG type names them.

	Evidence: one instance at 0x9817A8, built by 0x4720E0 (an array with
	the vtable 0x88BBAC, NiTArray's, of one element growing by one; 0x18
	and 0x1C cleared, 0x20 the frame time) and destroyed at exit by
	0x472120, which deletes the entries and their AIs before the array.
	The game's load fixes it up after printing "Fixing up AI". AddAI
	(0x4726E0) files a 0xC-byte entry: the AI (its slot 13), true, its bool
	and -1. PhysicsObjectClass's SetAI, ClearAI, GetAI and HasBuildingAIs
	(0x5A15A0-0x5A1730) call the members below. RemoveAI (0x472810) defers
	deleting the AI the manager is updating (0x18) by setting 0x1C.

===============================================================================
*/

#ifndef __AIMANAGER_H__
#define __AIMANAGER_H__

#include "NiTArray.h"

// The frame time an AI update is given, in seconds (invented name).
#define AI_UPDATE_TIME			0.01f

class ArtificialIntelligence;
class PhysicsObjectClass;
class TGStream;

// invented name
struct AIManagerEntry {
					AIManagerEntry( ArtificialIntelligence *ai_, bool unknown05_ );

	// (invented) AddAI's call to it is the inline site retail's budget needs
	ArtificialIntelligence *	GetAI( void ) const { return ai; }

	ArtificialIntelligence *	ai;		// 0x00
	bool			building;			// 0x04 true until the AI is done building (invented name)
	bool			unknown05;			// 0x05 AddAI's bool
	bool			unknown06;			// 0x06 set while the AI waits its turn in a frame
	float			unknown08;			// 0x08 -1 at first
};

// invented name
class AIManager {
public:
					AIManager( void );
					~AIManager( void );

	// The frame's AI updates (invented name).
	void			Update( void );
	// Updates the AI of the entry, or the BuilderAIs of its tree while it
	// is building, until the frame time passes time; acts on the update
	// status (invented name).
	static void		UpdateAI( double time, AIManagerEntry *entry );
	int				Unknown472660( void );

	// Files the AI and returns its ID.
	long			AddAI( ArtificialIntelligence *ai, bool unknown );
	// Drops the AI's entry, tells the object's handlers, and deletes the AI
	// when deleteAI is set; false when the AI has no entry.
	bool			RemoveAI( ArtificialIntelligence *ai, bool deleteAI );
	// Removes the object's AIs that are still building.
	void			RemoveBuildingAIs( PhysicsObjectClass *object );
	// Removes and deletes every AI.
	bool			RemoveAllAIs( void );					// invented name
	// The object's first AI that is done building, else its last one.
	ArtificialIntelligence *	GetAI( PhysicsObjectClass *object );
	bool			HasBuildingAIs( PhysicsObjectClass *object );
	// Empty in the release build.
	void			Unknown472A00( ArtificialIntelligence *ai );

	// Saved games: every AI and its entry's state (invented names, after
	// TGObject's).
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );
	void			PostFixUp( void );
	// Forgets every entry; the entries and their AIs are not deleted
	// (invented name).
	void			Clear( void );

protected:
	NiTArray<AIManagerEntry *>	entries;	// 0x00
	ArtificialIntelligence *	updatingAI;	// 0x18 the AI being updated (invented name)
	bool			deleteUpdatingAI;		// 0x1C RemoveAI deferred deleting it (invented name)
	float			unknown20;				// 0x20 a frame time
};

extern AIManager	aiManager;		// invented name

/*
================
AIManagerEntry::AIManagerEntry
================
*/
inline AIManagerEntry::AIManagerEntry( ArtificialIntelligence *ai_, bool unknown05_ ) {
	ai = ai_;
	building = true;
	unknown05 = unknown05_;
	unknown08 = -1.0f;
}

#endif /* !__AIMANAGER_H__ */
