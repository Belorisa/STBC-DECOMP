/*
===============================================================================

	TGConditionAction.h

	An action that waits on conditions: once played, it completes when its
	conditions say so. Without an evaluation function that is when every
	condition is true; with one, the function was asked once for every
	combination of the conditions' statuses, and the answers are kept as
	a table indexed by the statuses (condition i is bit i).

	0x3C bytes, allocated from the pools.

	Evidence: the constructor (0x704AD0) calls TGAction's and
	TGConditionHandler's, stores the vtables 0x8963F4 (TGAction's 24 slots)
	and 0x896454 (TGConditionHandler's 8) and clears the members; the
	destructor (0x704B30) frees the table. GetObjType returns 0x706. The
	SWIG wrappers TGConditionAction_Create and Play call the constructor and
	slot 16; the SWIG constants TGConditionAction_TGCA_WAIT and
	TGCA_COMPLETED are the evaluation function's answers. The actions module
	adds 0x704D10 to the shadow class "TGConditionAction" as
	"SetEvaluationFunction". The class strings are "TGConditionAction",
	"_p_TGConditionAction" and "TGConditionActionPtr".

===============================================================================
*/

#ifndef __TGCONDITIONACTION_H__
#define __TGCONDITIONACTION_H__

#include "Python.h"
#include "TGAction.h"
#include "TGConditionHandler.h"

// GetObjType's value for a TGConditionAction.
#define TGCONDITIONACTION_TYPE	0x706

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006fd210 ??_GTGConditionAction@@UAEPAXI@Z @game/TGActionModule
// FUNCTION: STBC 0x006fdb20 ?WriteToStream@TGConditionAction@@WCA@AE_NPAVTGStream@@@Z @game/TGActionModule
// FUNCTION: STBC 0x006fdb30 ?ReadFromStream@TGConditionAction@@WCA@AE_NPAVTGStream@@@Z @game/TGActionModule
// FUNCTION: STBC 0x006fdb40 ?FixUpPointers@TGConditionAction@@WCA@AE_NXZ @game/TGActionModule
// FUNCTION: STBC 0x006fdb50 ?PostFixUp@TGConditionAction@@WCA@AEXXZ @game/TGActionModule
class TGConditionAction : public TGAction, public TGConditionHandler {
public:
	// What the evaluation function answers for a combination of statuses
	// (the SWIG constants TGCA_*).
	enum evaluation_t {
		TGCA_WAIT,
		TGCA_COMPLETED
	};

					TGConditionAction( void );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006fd140 ??0TGConditionAction@@QAE@J@Z @game/TGActionModule
					TGConditionAction( long id ) : TGAction( id ) {}
	virtual			~TGConditionAction( void );

	// TGObject
	// FUNCTION: STBC 0x006fd1a0 ?GetObjType@TGConditionAction@@UAEHXZ @game/TGActionModule
	virtual int		GetObjType( void ) { return TGCONDITIONACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006fd1e0 ?GetClassName@TGConditionAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetClassName( void ) { return "TGConditionAction"; }
	// FUNCTION: STBC 0x006fd1f0 ?GetSwigTypeName@TGConditionAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGConditionAction"; }
	// FUNCTION: STBC 0x006fd200 ?GetPythonClassName@TGConditionAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetPythonClassName( void ) { return "TGConditionActionPtr"; }

	// TGAction
	virtual void	Play( void );

	// TGConditionHandler
	virtual void	ConditionChanged( TGCondition *condition );

	// Asks the function, with a tuple of the conditions' statuses, about
	// every combination of them; an answer other than TGCA_WAIT or
	// TGCA_COMPLETED counts as TGCA_WAIT.
	void			SetEvaluationFunction( PyObject *function );

protected:
	// Completes the action, if it is playing and its conditions say so
	// (invented name).
	void			CheckConditions( void );

	bool			playing;				// 0x30 Play was called
	int				numTableConditions;		// 0x34 the conditions the table was made for
	char *			table;					// 0x38 an evaluation_t per combination, NULL for none

	// Declared for the SWIG wrappers that call them.
public:
	static TGConditionAction *	Create( void ) { return new TGConditionAction; }	// the SWIG wrapper TGConditionAction_Create expands it
};

/*
================
TGConditionAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fd1b0 @game/TGActionModule
inline bool TGConditionAction::IsTypeOf( int type ) {
	if ( type == TGCONDITIONACTION_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

// The Python method SetEvaluationFunction( action, function ) of the
// shadow class.
PyObject *	TGConditionAction_SetEvaluationFunction( PyObject *self, PyObject *args );

#endif /* !__TGCONDITIONACTION_H__ */
