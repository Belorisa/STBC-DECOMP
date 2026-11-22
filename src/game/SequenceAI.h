/*
===============================================================================

	SequenceAI.h

	An AI that runs its AIs one after another, the next when the current
	one is done (or dormant, when dormant AIs are skipped), and the whole
	sequence a number of times.

	0x40 bytes: the create function (0x4931B0) allocates that much.

	Evidence: the constructor (0x4927D0) and the destructor (0x492870)
	store the vtable 0x88C230 (21 slots, ArtificialIntelligence's); the
	log strings "SequenceAI(%s)::SetActive", "::SetInactive",
	"::GotFocus", "::LostFocus", "::Update, contains %d" and "::IsDormant
	(no easy check)" name the overrides. The SWIG wrappers SequenceAI_
	GetLoopCount (0x34), SetLoopCount (0x38), SetResetIfInterrupted
	(0x3C), SetDoubleCheckAllDone (0x3D) and SetSkipDormant (0x3E) are
	inline; LOOP_INFINITE is a SWIG constant of the class.

===============================================================================
*/

#ifndef __SEQUENCEAI_H__
#define __SEQUENCEAI_H__

#include "ArtificialIntelligence.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class SequenceAI : public ArtificialIntelligence {
public:
	enum {
		LOOP_INFINITE = -1				// the sequence repeats for ever
	};

					SequenceAI( PhysicsObjectClass *object, char *name_ );
					SequenceAI( TGStream *stream, bool *ok );
	virtual			~SequenceAI( void );

	// ArtificialIntelligence
	virtual bool	IsInterruptable( void );
	virtual void	Reset( void );
	virtual void	GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais_ );
	virtual void	GetFocusAIs( NiTList<ArtificialIntelligence *> &ais_ );
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

	// Appends the AI; the sequence owns it.
	void			AddAI( ArtificialIntelligence *ai );
	// Removes and deletes the AI.
	void			RemoveAI( ArtificialIntelligence *ai );
	void			RemoveAIByIndex( int index );
	// NULL when the index is out of range.
	ArtificialIntelligence *	GetAI( int index );

	int				GetLoopCount( void ) { return loopsLeft; }
	void			SetLoopCount( int loopCount_ ) { loopCount = loopCount_; loopsLeft = loopCount_; }
	void			SetResetIfInterrupted( bool resetIfInterrupted_ ) { resetIfInterrupted = resetIfInterrupted_; }
	void			SetDoubleCheckAllDone( bool doubleCheckAllDone_ ) { doubleCheckAllDone = doubleCheckAllDone_; }
	void			SetSkipDormant( bool skipDormant_ ) { skipDormant = skipDormant_; }

	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );
	// The script-side constructor (the SWIG wrapper SequenceAI_Create calls it).
	// FUNCTION: STBC 0x00604920 ?Create@SequenceAI@@SAPAV1@PAVPhysicsObjectClass@@PAD@Z
	static SequenceAI *	Create( PhysicsObjectClass *object, char *name ) { return new SequenceAI( object, name ); }

protected:
	// Takes the focus and the activity from the current AI and makes the
	// AI at the index current, reset and active; returns it, NULL for
	// -1 (invented name).
	ArtificialIntelligence *	SetCurrentAI( int index );

	int				count;				// 0x28
	int				current;			// 0x2C index of the current AI, -1 for none
	ArtificialIntelligence **	ais;	// 0x30 owned
	int				loopsLeft;			// 0x34 runs of the sequence still to go (GetLoopCount)
	int				loopCount;			// 0x38 runs of the sequence (SetLoopCount); LOOP_INFINITE
	bool			resetIfInterrupted;	// 0x3C SetInactive resets the sequence
	bool			doubleCheckAllDone;	// 0x3D a finished sequence updates once more
	bool			skipDormant;		// 0x3E a dormant AI counts as done
};

#endif /* !__SEQUENCEAI_H__ */
