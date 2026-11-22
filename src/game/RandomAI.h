/*
===============================================================================

	RandomAI.h

	An AI that runs its AIs one at a time in a random order: each update
	runs the current one, and when it is done (or dormant) before it was
	ever active, another one that is still available and not dormant is
	picked at once.

	0x40 bytes: the create function (0x491CB0) allocates that much.

	Evidence: the constructor (0x491370) and the destructor (0x491400)
	store the vtable 0x88C1DC (21 slots, ArtificialIntelligence's); the
	log strings "RandomAI(%s)::SetActive", "::SetInactive", "::GotFocus",
	"::LostFocus" and "::Update, contains %d" name the overrides. AddAI
	(0x491490, the SWIG wrapper RandomAI_AddAI) grows the AI array by one
	and remakes the availability flags. The picks use
	g_kSystemWrapper.GetRandomNumber.

===============================================================================
*/

#ifndef __RANDOMAI_H__
#define __RANDOMAI_H__

#include "ArtificialIntelligence.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class RandomAI : public ArtificialIntelligence {
public:
					RandomAI( PhysicsObjectClass *object, char *name_ );
					RandomAI( TGStream *stream, bool *ok );
	virtual			~RandomAI( void );

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

	// The AI joins the choice; the RandomAI owns it.
	void			AddAI( ArtificialIntelligence *ai );

	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );
	// The script-side constructor (the SWIG wrapper RandomAI_Create calls it).
	// FUNCTION: STBC 0x00604ef0 ?Create@RandomAI@@SAPAV1@PAVPhysicsObjectClass@@PAD@Z
	static RandomAI *	Create( PhysicsObjectClass *object, char *name ) { return new RandomAI( object, name ); }

protected:
	// Makes every AI available again (invented name).
	void			ResetAvailable( void );
	// Picks an available AI that is not dormant, at random; the dormant
	// ones tried on the way are no longer available. false when none is
	// left (invented name).
	bool			PickNextAI( void );
	// Deactivates the current AI and makes the AI at the index current,
	// activating it while the RandomAI is active; -1 for none (invented
	// name).
	void			SetCurrentAI( int index );

	bool			currentRan;			// 0x28 the current AI has been active (invented name)
	bool *			available;			// 0x2C per AI: not yet done or dormant
	int				count;				// 0x30
	int				current;			// 0x34 index of the current AI, -1 for none
	int				dormantCount;		// 0x38 AIs found dormant since the last reset
	ArtificialIntelligence **	ais;	// 0x3C owned
};

#endif /* !__RANDOMAI_H__ */
