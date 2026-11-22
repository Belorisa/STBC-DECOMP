/*
===============================================================================

	STTargetMenu.h

	The tactical target list: a top level menu holding an STSubsystemMenu
	per target, in the order of a table of the targets' IDs. A time slice
	process refreshes the list every second: it adds the ships of the
	player's set the player can target, hides the ones the sensors do not
	see, and takes down the menus of objects that are gone. The menu also
	keeps the persistent target, which it gives back to the player when the
	player has no target.

	0xB0 bytes (its scalar deleting destructor frees 0xB0).

	Evidence: the constructor (0x537BE0) builds the top level menu on an
	STTargetMenuSubPane, builds the process (0x78) on the callback
	0x537F40 and the menu itself and sets its delay to 1, stores the vtable
	0x89113C (STTopLevelMenu's 116 slots; the destructor, the type, name,
	class handler and stream slots and SetFocus overridden), makes the
	table at 0x6C for 8 entries (the size at 0x64, the count at 0x68),
	clears 0x70 and 0x74 and adds the broadcast handlers "STTargetMenu::*".
	The stream slots (0x5390E0, 0x539170) save the size, the count, each
	entry's ID and flag, and the IDs at 0x70 and 0x74. ClearPersistentTarget
	clears 0x70 and 0x74; 0x538600 stores a target's ID at 0x70 and the
	targeted subsystem handler a subsystem's ID at 0x74. 0x538FC0 sends
	ET_RESTORE_PERSISTENT_TARGET and targets 0x70's object and 0x74's
	subsystem when the player has no target. GetObjType returns 0x80F2; the
	class strings are "STTargetMenu", "_p_STTargetMenu" and
	"STTargetMenuPtr".

===============================================================================
*/

#ifndef __STTARGETMENU_H__
#define __STTARGETMENU_H__

#include "STTopLevelMenu.h"
#include "STSubPane.h"
#include "STTargetMenuSubPane.h"
#include "TimeSliceProcess.h"

class ObjectClass;
class STMenu;
class TGEvent;
class TGIntEvent;
class TGObject;
class TGObjPtrEvent;
class TGStream;
class TGStringEvent;
class TGString;
class TGUIObject;

// GetObjType's value for an STTargetMenu.
#define STTARGETMENU_TYPE		0x80F2

// The target list's events (the SWIG constants of the same names).
#define ET_RESTORE_PERSISTENT_TARGET	0x80005B
#define ET_TARGET_LIST_CLICKED			0x8000A0
#define ET_TARGET_LIST_OBJECT_ADDED		0x8000A2
#define ET_TARGET_LIST_OBJECT_REMOVED	0x8000A3

// A target of the menu (invented name).
struct STTargetEntry {
	long			objectID;				// 0x00 the target's TGObject ID
	bool			targetable;				// 0x04 the player can target it, as the last refresh found (invented name)
};

/*
================
STTargetMenuProcess

The time slice process of a target menu: after the base's update it calls
its function with the time and its data (invented names).

Evidence: 0x30 bytes of TimeSliceProcess, then the function (0x30) and
the data (0x34); the vtable 0x89130C has Update (0x537E10) and the scalar
deleting destructor (0x537E40), which frees with operator delete.
================
*/
// FUNCTION: STBC 0x00537e40 ??_GSTTargetMenuProcess@@UAEPAXI@Z
// FUNCTION: STBC 0x00537e60 ??1STTargetMenuProcess@@UAE@XZ
class STTargetMenuProcess : public TimeSliceProcess {
public:
	typedef void	( *Function )( double time, void *data );

					STTargetMenuProcess( Function function_, void *data_ ) { function = function_; data = data_; }

					// FUNCTION: STBC 0x00537e10 ?Update@STTargetMenuProcess@@UAEXN@Z
	virtual void	Update( double time ) {
						TimeSliceProcess::Update( time );
						function( time, data );
					}

protected:
	Function		function;				// 0x30
	void *			data;					// 0x34
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00537f50 ??_GSTTargetMenu@@UAEPAXI@Z
class STTargetMenu : public STTopLevelMenu {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	virtual			~STTargetMenu( void );

	// TGObject
	// FUNCTION: STBC 0x00537e70 ?GetObjType@STTargetMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return STTARGETMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00537f00 ?GetClassName@STTargetMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STTargetMenu"; }
	// FUNCTION: STBC 0x00537f10 ?GetSwigTypeName@STTargetMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STTargetMenu"; }
	// FUNCTION: STBC 0x00537f20 ?GetPythonClassName@STTargetMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STTargetMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00537f30 ?GetClassHandlers@STTargetMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGPane
	virtual void	SetFocus( TGUIObject *child, bool layout = true );

	static STTargetMenu *	Cast( TGObject *object );
	static STTargetMenu *	GetTargetMenu( void );
	void			RebuildShipMenu( ObjectClass *object );
	void			RebuildShipMenus( void );
	void			ResetAffiliationColors( void );
	STMenu *		GetObjectEntry( ObjectClass *object );
	void			ClearPersistentTarget( void );
	void			ClearTargetList( void );

	// The index of the object's entry, or -1 (invented name).
	int				FindTarget( ObjectClass *object );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			ObjectEnteredSet( TGEvent *event );
	void			ObjectExitedSet( TGStringEvent *event );
	void			HandleObjectExploding( TGEvent *event );
	void			ObjectChangedGroup( TGEvent *event );
	void			ObjectCloaked( TGEvent *event );
	void			ObjectDecloaked( TGEvent *event );
	void			ObjectIdentified( TGObjPtrEvent *event );
	void			TargetedSubsystemChanged( TGEvent *event );
	void			TargetButtonClicked( TGIntEvent *event );
	void			TargetChanged( TGEvent *event );

protected:
	// The process's callback: refreshes the menu of data (invented name).
	static void		ProcessCallback( double time, void *data );
	// Calls UpdateTargets (invented name).
	void			Unknown538C80( void );
	// Adds the targets of the player's set, hides the ones the sensors do
	// not see and takes down the ones that are gone (invented name).
	void			UpdateTargets( void );
	// Makes the object the persistent target and highlights it (invented name).
	void			SetPersistentTarget( ObjectClass *target );
	// Unhighlights the menus, then highlights and focuses the target's
	// and closes the others (invented name).
	void			HighlightTarget( ObjectClass *target );
	// A menu for the ship, at the end of the list (invented name).
	void			AddTarget( ObjectClass *object );
	// Takes the target's menu down (invented name).
	void			RemoveTarget( ObjectClass *object );
	void			RemoveTargetAt( int index );		// invented name
	// Gives the player back the persistent target when it has none (invented name).
	void			RestorePersistentTarget( void );

	int				maxTargets;				// 0x64 entries allocated in targets (invented name)
	int				numTargets;				// 0x68 entries in use in targets
	STTargetEntry *	targets;				// 0x6C one per menu child, in the children's order
	long			persistentTargetID;		// 0x70 (invented name)
	long			persistentSubsystemID;	// 0x74 (invented name)
	STTargetMenuProcess	process;			// 0x78 (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
					STTargetMenu( TGString *name );
					STTargetMenu( long id );
	static STTargetMenu *	CreateW( TGString *name ) { return new STTargetMenu( name ); }	// the SWIG wrapper STTargetMenu_CreateW expands it
};

/*
================
STTargetMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x00537e80 ?IsTypeOf@STTargetMenu@@UAE_NH@Z
inline bool STTargetMenu::IsTypeOf( int type ) {
	if ( type == STTARGETMENU_TYPE ) {
		return true;
	}
	return STTopLevelMenu::IsTypeOf( type );
}

#endif /* !__STTARGETMENU_H__ */
