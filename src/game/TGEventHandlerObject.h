/*
===============================================================================

	TGEventHandlerObject.h

	An attribute object that receives events: ProcessEvent hands an event to
	the handlers registered for its type, first the object's own (created on
	first use) and then those of its class.

	0x14 bytes. Evidence: the constructor (0x6D8F90) calls
	TGTemplatedAttrObject's, stores the vtable 0x896044 (21 slots) and clears
	the handler table at 0x10; GetObjType returns 0x102, the type
	TGEventHandlerObject::Cast checks. Slot 20 is ProcessEvent (the SWIG
	wrapper TGEventHandlerObject_ProcessEvent); slot 19 returns the class's
	static handler table (0x998558), which the tables of the object's own
	handlers take as their parent. DeleteMeStage1 and DeleteMeStage2 are
	named by the strings they are registered under. The SWIG variables
	ANY_TARGET and INVALID_DESTINATION are defined in this unit, and so is
	the array of the class tables (0x998728, built by the dynamic
	initialiser 0x6D9F90 with one element growing by one): each module, as
	it registers its classes, gives every class table its parent's table
	and appends it to the array.

===============================================================================
*/

#ifndef __TGEVENTHANDLEROBJECT_H__
#define __TGEVENTHANDLEROBJECT_H__

#include "TGTemplatedAttrObject.h"
#include "NiTArray.h"

class TGEvent;
class TGObject;
class TGHandler;
class TGEventHandlerTable;

// GetObjType's value for a TGEventHandlerObject.
#define TGEVENTHANDLEROBJECT_TYPE	0x102

// The event types of the two stages of deleting an event handler: the
// first posts the second, whose handler deletes the object.
#define ET_DELETE_ME_STAGE_1		1		// invented name
#define ET_DELETE_ME_STAGE_2		2		// invented name

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006d9030 ??_GTGEventHandlerObject@@UAEPAXI@Z
class TGEventHandlerObject : public TGTemplatedAttrObject {
public:
					TGEventHandlerObject( long id = 0 );
	virtual			~TGEventHandlerObject( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	virtual TGEventHandlerTable *	GetClassHandlers( void );		// invented name
	virtual bool	ProcessEvent( TGEvent *event );

	static TGEventHandlerObject *	Cast( TGObject *object );
	bool			CallNextHandler( TGEvent *event );

	// Handlers of this object alone: registered C++ ones and Python ones,
	// called as functions or as methods.
	bool			AddFuncHandlerForInstance( int eventType, char *function );		// invented name
	bool			AddMethodHandlerForInstance( int eventType, char *method );		// invented name
	bool			AddPythonFuncHandlerForInstance( int eventType, char *function );
	bool			AddPythonMethodHandlerForInstance( int eventType, char *method );
	bool			RemoveHandlerForInstance( int eventType, char *function );
	bool			RemoveAllInstanceHandlers( void );

	// Handlers of a class, in the class's static table.
	static bool		AddFuncHandlerForClass( TGEventHandlerTable *table, int eventType, char *function );		// invented name
	static bool		AddMethodHandlerForClass( TGEventHandlerTable *table, int eventType, char *method );		// invented name
	static bool		AddPythonFuncHandlerForClass( TGEventHandlerTable *table, int eventType, char *function );	// invented name
	static bool		AddPythonMethodHandlerForClass( TGEventHandlerTable *table, int eventType, char *method );	// invented name
	static bool		RemoveHandlerForClass( TGEventHandlerTable *table, int eventType, char *function );			// invented name

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			DeleteMeStage1( TGEvent *event );
	void			DeleteMeStage2( TGEvent *event );

	// The array of the class tables, which the modules append to as they
	// register their classes and TGEventManager::RemoveAllClassHandlers
	// empties (invented name).
	static NiTArray<TGEventHandlerTable *>	classTables;

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	bool			CreateInstanceHandlers( void );									// invented name
	TGHandler *		FindHandlers( int eventType, TGEventHandlerTable **owner );		// invented name

	TGEventHandlerTable *	handlers;		// 0x10 the object's own handlers; NULL until the first is added

	static TGEventHandlerTable	classHandlers;		// invented name
};

// A registered handler method (see TGHandlerFunction).
typedef void ( TGEventHandlerObject::*TGHandlerMethod )( TGEvent *event );	// invented name

// A handler target that matches every object.
extern TGEventHandlerObject *	ANY_TARGET;

/*
================
TGEventHandlerObject::IsTypeOf
================
*/
// FUNCTION: STBC 0x006d8fc0
inline bool TGEventHandlerObject::IsTypeOf( int type ) {
	if ( type == TGEVENTHANDLEROBJECT_TYPE ) {
		return true;
	}
	return TGTemplatedAttrObject::IsTypeOf( type );
}

#endif /* !__TGEVENTHANDLEROBJECT_H__ */
