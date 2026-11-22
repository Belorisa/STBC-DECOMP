/*
===============================================================================

	ArtificialIntelligence.h

	An AI driving a PhysicsObjectClass: the object's ID and the AI's own
	ID, under which the static AI map (0x9816A0) and the AI manager know it.

	Evidence: the constructor (0x470520) and the destructor (0x4707D0)
	store the vtable 0x88BB54 (21 slots; slot 19 is _purecall); the
	constructor takes the next ID (0x8DB134) and files the AI in the map at
	0x9816A0, the destructor removes it. The SWIG wrapper ArtificialIntelligence_
	GetObject calls 0x470A30, which looks up the object by the ID at 0x04.
	The wrappers IsInterruptable and Reset call slots 1 and 2; IsActive,
	HasFocus, IsPaused, Pause, Unpause, SetInterruptable, GetID and GetName
	read and write 0x20-0x24 and 0x0C inline. The strings
	"RegisterExternalFunction", "UnregisterExternalFunction" and
	"CallExternalFunction" name the Python functions of this unit that call
	slots 5, 6 and 7, and "GetAllAIsInTree" and "GetFocusAIs" those that
	call slots 3 and 4. The log strings of the subclasses ("PlainAI(%s)::
	SetActive", "::SetInactive", "::GotFocus", "::LostFocus", "::Update",
	"::IsDormant") name slots 8 to 12 and 14. Slot 16 writes the AI to a
	stream, and the stream constructor (0x4711F0) reads it back; slot 19
	gives the type the stream starts with, by which CreateFromStream
	(0x471470) finds the subclass's create function. The update statuses
	are the SWIG constants ArtificialIntelligence_US_ACTIVE to
	US_NUM_STATUSES; ConditionalAI's log names them "Active", "Done",
	"Dormant" and "Invalid" in that order.

===============================================================================
*/

#ifndef __ARTIFICIALINTELLIGENCE_H__
#define __ARTIFICIALINTELLIGENCE_H__

#include "Python.h"
#include "NiTList.h"
#include "NiTStringMap.h"

class PhysicsObjectClass;
class ShipClass;
class TGStream;

// The type an AI writes first to a stream (slot 19). The names are
// invented; 7, 8, 10 and 11 are the types of the other classes of the
// AI module that register a create function.
enum aiType_t {
	AI_TYPE_CONDITIONAL		= 1,	// ConditionalAI
	AI_TYPE_PLAIN			= 2,	// PlainAI
	AI_TYPE_PREPROCESSING	= 3,	// PreprocessingAI
	AI_TYPE_PRIORITY_LIST	= 4,	// PriorityListAI
	AI_TYPE_RANDOM			= 5,	// RandomAI
	AI_TYPE_SEQUENCE		= 6,	// SequenceAI
	AI_TYPE_NETWORK			= 9,	// NetworkAI
	AI_TYPE_BUILDER			= 12	// BuilderAI
};

class ArtificialIntelligence;

// Makes an AI from a stream: the subclass's stream constructor.
typedef ArtificialIntelligence *( *aiCreateFunction_t )( TGStream *stream );

// A create function filed under its type, in the list CreateFromStream
// searches. The name is invented.
struct AICreateFunction {
	long					type;
	aiCreateFunction_t		create;
	AICreateFunction *		next;
};

class ArtificialIntelligence {
public:
	// The result of an update (the SWIG constants US_*).
	enum updateStatus_t {
		US_ACTIVE,
		US_DONE,
		US_DORMANT,
		US_INVALID,
		US_NUM_STATUSES
	};

					ArtificialIntelligence( PhysicsObjectClass *object, char *name_ );
	// Reads what WriteToStream wrote, after the type; ok is cleared on a
	// failure.
					ArtificialIntelligence( TGStream *stream, bool *ok );
	virtual			~ArtificialIntelligence( void );
	virtual bool	IsInterruptable( void );	// returns the flag at 0x23
	virtual void	Reset( void );
	// Adds the AIs of the tree under this one (this one included), and of
	// them the ones that have the focus.
	virtual void	GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais );
	virtual void	GetFocusAIs( NiTList<ArtificialIntelligence *> &ais );
	// The script functions the AI's Python side calls by name; the AI holds
	// a reference to each.
	virtual void	RegisterExternalFunction( char *functionName, PyObject *function );
	virtual void	UnregisterExternalFunction( char *functionName, PyObject *function );
	virtual bool	CallExternalFunction( char *functionName, PyObject *arguments );
	virtual void	SetActive( void );			// sets the flag at 0x20
	virtual void	SetInactive( void );		// clears the flag at 0x20
	virtual void	GotFocus( void );			// sets the flag at 0x21
	virtual void	LostFocus( void );			// clears the flag at 0x21
	virtual int		Update( double time );		// an updateStatus_t; the time is passed on to the script (invented name)
	virtual ArtificialIntelligence *	VirtualSlot13( void );	// the AI the manager files (this one, here)
	virtual bool	IsDormant( void );
	virtual PyObject *	VirtualSlot15( void );	// a new Python shadow instance of the AI
	// Writes the AI's state; the type comes first (GetAIType).
	virtual bool	WriteToStream( TGStream *stream );		// invented name
	// After a load (TGObject's names: ConditionalAI's overrides fill these
	// slots and TGConditionHandler's alike, through the thunks 0x47AD00 and
	// 0x47AD10).
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual int		GetAIType( void ) = 0;		// an aiType_t (invented name)
	virtual bool	VirtualSlot20( int );

	PhysicsObjectClass *	GetObject( void );
	ShipClass *	GetShip( void );
	static bool	LogAITree( char *, bool );
	// The AI with the ID, from the AI map; NULL when there is none (invented name).
	static ArtificialIntelligence *	GetAIByID( long id );
	// Reads the type and makes the AI through the type's create function;
	// NULL when no function is filed for the type (invented name).
	static ArtificialIntelligence *	CreateFromStream( TGStream *stream );
	// Files the create function for the type, unless the type has one
	// (invented name).
	static void		RegisterCreateFunction( long type, aiCreateFunction_t create );

	long			GetID( void ) { return id; }
	long			GetObjectID( void ) { return objectID; }		// invented name
	bool			IsActive( void ) { return active; }
	bool			HasFocus( void ) { return focus; }
	bool			IsPaused( void ) { return paused; }
	void			Pause( void ) { paused = true; }
	void			Unpause( void ) { paused = false; }
	void			SetInterruptable( bool interruptable_ ) { interruptable = interruptable_; }
	void			SetParentID( long parentID_ ) { parentID = parentID_; }		// invented name
	long			GetParentID( void ) { return parentID; }					// invented name

	char *	GetName( void ) { return name; }
protected:
	long			objectID;			// 0x04 the ID of the object the AI drives
	long			parentID;			// 0x08 the ID of the AI that contains this one (invented name)
	char *			name;				// 0x0C a heap copy (GetName)
	NiTStringMap<PyObject *>	externalFunctions;	// 0x10 by name; a reference held on each (invented name)
	bool			active;				// 0x20 (IsActive)
	bool			focus;				// 0x21 (HasFocus)
	bool			paused;				// 0x22 (IsPaused)
	bool			interruptable;		// 0x23 (IsInterruptable)
	long			id;					// 0x24 the key of the AI map at 0x9816A0 (GetID)

	// Every live AI, by ID.
	static NiTMap<long, ArtificialIntelligence *>	aiMap;		// invented name
	static long		nextID;				// invented name
	static AICreateFunction *	createFunctions;	// newest first (invented name)
};

/*
================
ArtificialIntelligence::RegisterCreateFunction

Evidence: each subclass's unit runs this body inline at startup with its
type and its create function.
================
*/
inline void ArtificialIntelligence::RegisterCreateFunction( long type, aiCreateFunction_t create ) {
	AICreateFunction *entry;
	for ( entry = createFunctions; entry != NULL; entry = entry->next ) {
		if ( entry->type == type ) {
			if ( entry->create != NULL ) {
				return;
			}
			break;
		}
	}
	entry = new AICreateFunction;
	entry->type = type;
	entry->create = create;
	entry->next = createFunctions;
	createFunctions = entry;
}

// Files a subclass's create function when the program starts (invented name).
class AICreateFunctionRegistration {
public:
					AICreateFunctionRegistration( long type, aiCreateFunction_t create ) {
						ArtificialIntelligence::RegisterCreateFunction( type, create );
					}
};

// The Python functions of the AI's shadow class, by the names they are
// registered under (ArtificialIntelligence_GetAIByID in App's table, the
// others as methods of the class).
PyObject *	ArtificialIntelligence_GetAllAIsInTree( PyObject *self, PyObject *args );
PyObject *	ArtificialIntelligence_GetFocusAIs( PyObject *self, PyObject *args );
PyObject *	ArtificialIntelligence_RegisterExternalFunction( PyObject *self, PyObject *args );
PyObject *	ArtificialIntelligence_UnregisterExternalFunction( PyObject *self, PyObject *args );
PyObject *	ArtificialIntelligence_CallExternalFunction( PyObject *self, PyObject *args );
extern "C" PyObject *	_wrap_ArtificialIntelligence_GetAIByID( PyObject *self, PyObject *args );
// The save's handlers for the shadow instances: the AI's ID goes into the
// instance's dictionary as "_AIID_", and the load points the instance's
// "this" at the AI with that ID. The names are invented.
bool		ArtificialIntelligence_SaveInstance( PyObject *instance, PyObject **state );
bool		ArtificialIntelligence_LoadInstance( PyObject *instance, PyObject *state );

#endif /* !__ARTIFICIALINTELLIGENCE_H__ */
