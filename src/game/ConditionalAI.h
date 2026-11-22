/*
===============================================================================

	ConditionalAI.h

	An AI that runs the AI it contains while its conditions allow: the
	condition statuses select the AI's update status from a table the
	evaluation function filled (condition i is bit i of the index). The
	status is updated whenever a condition changes. The AI's external
	functions are kept by name as lists, and are passed on to the
	condition scripts.

	0x58 bytes.

	Evidence: the constructor (0x478A50) calls ArtificialIntelligence's and
	TGConditionHandler's (at 0x28), sets the status to Invalid, clears the
	table and the contained AI, builds the function map at 0x48 (37
	buckets), and stores the vtables 0x88BC84 (ArtificialIntelligence's 21
	slots) and 0x88BC64 (TGConditionHandler's 8); the destructor (0x478B60)
	removes the conditions, frees the table, deletes the contained AI and
	empties the map. The log strings "ConditionalAI(%s)::SetActive",
	"::SetInactive", "::GotFocus", "::LostFocus", "::Update, condition %s"
	and "::IsDormant, (%s)" name the overrides; the update statuses are
	logged as "Active", "Done", "Dormant" and "Invalid". The SWIG wrappers
	ConditionalAI_SetContainedAI (0x4791E0) and GetContainedAI (0x44) name
	the contained AI; the Python method "SetEvaluationFunction" of the
	shadow class is 0x479C00.

===============================================================================
*/

#ifndef __CONDITIONALAI_H__
#define __CONDITIONALAI_H__

#include "ArtificialIntelligence.h"
#include "TGConditionHandler.h"
#include "NiTList.h"
#include "NiTStringMap.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00478b40 ??_GConditionalAI@@UAEPAXI@Z
// FUNCTION: STBC 0x0047acf0 ?WriteToStream@ConditionalAI@@WCI@AE_NPAVTGStream@@@Z
// FUNCTION: STBC 0x0047ad00 ?FixUpPointers@ConditionalAI@@WCI@AE_NXZ
// FUNCTION: STBC 0x0047ad10 ?PostFixUp@ConditionalAI@@WCI@AEXXZ
class ConditionalAI : public ArtificialIntelligence, public TGConditionHandler {
public:
					ConditionalAI( PhysicsObjectClass *object, char *name_ );
	// Reads what WriteToStream wrote, after the AI's part; ok is cleared
	// on a failure.
					ConditionalAI( TGStream *stream, bool *ok );
	virtual			~ConditionalAI( void );

	// ArtificialIntelligence
	virtual bool	IsInterruptable( void );
	virtual void	Reset( void );
	virtual void	GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais );
	virtual void	GetFocusAIs( NiTList<ArtificialIntelligence *> &ais );
	// The functions are kept in a list per name; each reference is held.
	virtual void	RegisterExternalFunction( char *functionName, PyObject *function );
	// Calls every function registered under the name on the condition
	// scripts that registered it.
	virtual bool	CallExternalFunction( char *functionName, PyObject *arguments );
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

	// TGConditionHandler
	virtual void	ConditionChanged( TGCondition *condition );
	virtual void	AddCondition( TGCondition *condition );
	virtual void	RemoveCondition( TGCondition *condition );

	// The AI it contains, which it owns; the contained AI's parent becomes
	// this one.
	void			SetContainedAI( ArtificialIntelligence *ai );
	ArtificialIntelligence *	GetContainedAI( void ) { return containedAI; }
	// Asks the function, with a tuple of the conditions' statuses, for the
	// update status of every combination of them; an answer that is not an
	// update status, or a failed call, counts as US_INVALID.
	void			SetEvaluationFunction( PyObject *function );

	// The create function of the type, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );
	// The script-side constructor (the SWIG wrapper ConditionalAI_Create calls it).
	// FUNCTION: STBC 0x00605640 ?Create@ConditionalAI@@SAPAV1@PAVPhysicsObjectClass@@PAD@Z
	static ConditionalAI *	Create( PhysicsObjectClass *object, char *name ) { return new ConditionalAI( object, name ); }

protected:
	// The update status the table gives the conditions' statuses;
	// US_INVALID when the conditions are not the ones the table was made
	// for (invented name).
	int				EvaluateConditions( void );
	// Releases the external functions of the condition script of the ID
	// (invented name).
	void			RemoveConditionFunctions( long scriptID );
	// Calls one registered function entry; false when no condition calls
	// it (invented name).
	bool			CallConditionFunction( char *functionName, PyObject *function, PyObject *arguments );

	int				status;					// 0x38 an updateStatus_t, from the table
	int				numTableConditions;		// 0x3C the conditions the table was made for
	char *			table;					// 0x40 an updateStatus_t per combination
	ArtificialIntelligence *	containedAI;	// 0x44 owned (GetContainedAI)
	NiTStringMap<NiTList<PyObject *> *>	functionLists;	// 0x48 the external functions by name (invented name)
};

// The Python method SetEvaluationFunction( ai, function ) of the shadow
// class.
PyObject *	ConditionalAI_SetEvaluationFunction( PyObject *self, PyObject *args );

#endif /* !__CONDITIONALAI_H__ */
