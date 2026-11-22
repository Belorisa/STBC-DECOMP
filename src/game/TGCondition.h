/*
===============================================================================

	TGCondition.h

	Something that is true or false, watched by condition handlers: when
	the status changes, every handler's ConditionChanged is called. A
	condition belongs to its handlers: removing the last one deletes it.

	0x18 bytes, allocated from the pools.

	Evidence: the constructor (0x701020) calls TGObject's, stores the
	vtable 0x89677C (15 slots: TGObject's 12 and 3 of its own), clears
	the status, sets the active flag and makes room for one handler; the
	destructor (0x701120) frees the handler array. GetObjType returns 0x705,
	the type TGCondition::Cast (0x700FF0) checks. The SWIG wrappers
	TGCondition_* call slots 12 to 14 (SetStatus, SetActive, SetInactive),
	AddHandler (0x701180) and RemoveHandler (0x7011F0), and read GetStatus
	(0x08) and IsActive (0x09) inline. The class strings are "TGCondition",
	"_p_TGCondition" and "TGConditionPtr".

===============================================================================
*/

#ifndef __TGCONDITION_H__
#define __TGCONDITION_H__

#include "TGObject.h"

class TGConditionHandler;

// GetObjType's value for a TGCondition.
#define TGCONDITION_TYPE		0x705

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TGCondition : public TGObject {
public:
					TGCondition( long id = 0 );
	virtual			~TGCondition( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00442f90 ?FixUpPointers@TGCondition@@UAE_NXZ
	virtual bool	FixUpPointers( void ) { return true; }
	// FUNCTION: STBC 0x00442fa0 ?PostFixUp@TGCondition@@UAEXXZ
	virtual void	PostFixUp( void ) {}
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// Tells every handler when the status changes.
	virtual void	SetStatus( bool status_ );
	// FUNCTION: STBC 0x48CBC0 ?SetActive@TGCondition@@UAEXXZ
	virtual void	SetActive( void ) { active = true; }
	// FUNCTION: STBC 0x48CBD0 ?SetInactive@TGCondition@@UAEXXZ
	virtual void	SetInactive( void ) { active = false; }

	static TGCondition *	Cast( TGObject * );
	void			AddHandler( TGConditionHandler *handler );
	// Deletes the condition when no handler is left.
	void			RemoveHandler( TGConditionHandler *handler );
	bool			GetStatus( void ) { return status; }
	bool			IsActive( void ) { return active; }

	// Saved games: every live condition, after the count; false when one
	// of them failed (invented names).
	static bool		SaveConditions( TGStream *stream );
	static bool		LoadConditions( TGStream *stream );

protected:
	bool			status;					// 0x08 (GetStatus)
	bool			active;					// 0x09 (IsActive)
	int				numHandlers;			// 0x0C
	int				maxHandlers;			// 0x10 the array's size; doubles when full
	TGConditionHandler **	handlers;		// 0x14 not owned; the handlers own the condition
};

/*
================
TGCondition::IsTypeOf

Derived classes inline it into their own IsTypeOf (ConditionScript's,
0x442010).
================
*/
// FUNCTION: STBC 0x007010a0 @game/TGCondition
inline bool TGCondition::IsTypeOf( int type ) {
	if ( type == TGCONDITION_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

#endif /* !__TGCONDITION_H__ */
