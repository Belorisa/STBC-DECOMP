/*
===============================================================================

	TGConditionHandler.h

	A base for what waits on conditions: it holds the conditions added to
	it, is one of their handlers, and hears through ConditionChanged when
	a condition's status changes. The handler saves its conditions by ID
	and turns them back into pointers after a load.

	0x10 bytes. Not a TGObject: it is a second base of ConditionalAI
	(at 0x28) and TGConditionAction (at 0x20).

	Evidence: the constructor (0x701580) stores the vtable 0x8967C0 (8
	slots) and makes room for one condition; the destructor (0x7015E0)
	removes the handler from its conditions and frees the array. The SWIG
	wrappers TGConditionHandler_ConditionChanged, AddCondition and
	RemoveCondition call slots 0, 1 and 2. Slots 3 to 6 stream the
	conditions and fix them up like TGObject's slots 4 to 7, whose names
	they take; slot 7 is the scalar deleting destructor.

===============================================================================
*/

#ifndef __TGCONDITIONHANDLER_H__
#define __TGCONDITIONHANDLER_H__

class TGCondition;
class TGStream;

class TGConditionHandler {
public:
					TGConditionHandler( void );

	// FUNCTION: STBC 0x007015B0 ?ConditionChanged@TGConditionHandler@@UAEXPAVTGCondition@@@Z
	virtual void	ConditionChanged( TGCondition * ) {}
	// The handler becomes one of the condition's handlers.
	virtual void	AddCondition( TGCondition *condition );
	// Every entry of the condition goes, and the handler leaves its
	// handlers each time (the last handler to leave deletes it).
	virtual void	RemoveCondition( TGCondition *condition );
	// The conditions by ID; the read leaves the IDs in the array until
	// PostFixUp (invented names).
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// Whether every condition read is loaded.
	virtual bool	FixUpPointers( void );
	// Adds the conditions read again, by pointer.
	virtual void	PostFixUp( void );
	virtual			~TGConditionHandler( void );

protected:
	int				numConditions;			// 0x04
	int				maxConditions;			// 0x08 the array's size; grows by two
	TGCondition **	conditions;				// 0x0C not owned
};

#endif /* !__TGCONDITIONHANDLER_H__ */
