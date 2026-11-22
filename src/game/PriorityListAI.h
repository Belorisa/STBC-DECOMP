/*
===============================================================================

	PriorityListAI.h

	An AI that runs the first of its AIs, in order of priority, that is
	not done and not dormant: the AIs before the current one may take over
	from it when it can be interrupted, and the AIs after an active one
	are deactivated.

	0x38 bytes: the create function (0x490850) allocates that much.

	Evidence: the constructor (0x48FCB0) and the destructor (0x48FD30)
	store the vtable 0x88C188 (21 slots, ArtificialIntelligence's); the
	log strings "PriorityListAI(%s)::SetActive", "::SetInactive",
	"::GotFocus", "::LostFocus", "::Update, contains %d" and "::IsDormant
	(no easy check)" name the overrides. The entries are 0xC bytes, kept
	in ascending order of priority by AddAI (0x48FDD0, the SWIG wrapper
	PriorityListAI_AddAI); the array grows by one entry at a time.

===============================================================================
*/

#ifndef __PRIORITYLISTAI_H__
#define __PRIORITYLISTAI_H__

#include "ArtificialIntelligence.h"

// An AI of the list and its priority (invented name).
struct PriorityListEntry {
	int							priority;	// lower runs first
	bool						done;		// its Update returned US_DONE (until a Reset)
	ArtificialIntelligence *	ai;			// owned
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PriorityListAI : public ArtificialIntelligence {
public:
					PriorityListAI( PhysicsObjectClass *object, char *name_ );
					PriorityListAI( TGStream *stream, bool *ok );
	virtual			~PriorityListAI( void );

	// ArtificialIntelligence
	virtual bool	IsInterruptable( void );
	virtual void	Reset( void );
	virtual void	GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais );
	virtual void	GetFocusAIs( NiTList<ArtificialIntelligence *> &ais );
	virtual void	SetActive( void );
	virtual void	SetInactive( void );
	virtual void	GotFocus( void );
	virtual void	LostFocus( void );
	virtual int		Update( double time );
	virtual bool	IsDormant( void );
	virtual PyObject *	VirtualSlot15( void );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual int		GetAIType( void );
	virtual bool	VirtualSlot20( int type );

	// Inserts the AI after the entries of the same or a lower priority;
	// the list owns it.
	void			AddAI( ArtificialIntelligence *ai, int priority );
	// Removes and deletes the first AI of the priority, or the AI.
	void			RemoveAIByPriority( int priority );
	void			RemoveAI( ArtificialIntelligence *ai );

	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );
	// The script-side constructor (the SWIG wrapper PriorityListAI_Create calls it).
	// FUNCTION: STBC 0x00604620 ?Create@PriorityListAI@@SAPAV1@PAVPhysicsObjectClass@@PAD@Z
	static PriorityListAI *	Create( PhysicsObjectClass *object, char *name ) { return new PriorityListAI( object, name ); }

protected:
	ArtificialIntelligence *	currentAI;	// 0x28 the AI that has the focus (invented name)
	int				count;					// 0x2C entries in use
	int				capacity;				// 0x30 entries allocated
	PriorityListEntry *	entries;			// 0x34
};

#endif /* !__PRIORITYLISTAI_H__ */
