/*
===============================================================================

	SetManager.h

	The sets of the game, sorted by name. The members that the SWIG
	wrappers SetManager_* call, and the layout the constructor proves.

	0x18 bytes. Evidence: the dynamic initialiser of g_kSetManager (0x417E00)
	runs the constructor 0x417E30, which clears the members and stores the
	vtable 0x888B78 (three slots: the scalar deleting destructor 0x417E50,
	WriteToStream 0x4186D0 and ReadFromStream 0x418740); the
	destructor (0x417E70) terminates the manager, then deletes the sets
	still in the array at 0x08. AddSet (0x417F00) searches that array by
	name. The SWIG wrappers name the inline accessors: GetRenderedSet and
	ClearRenderedSet 0x04, GetNumSets 0x0C, GetSet (the search by name). The objects' GetObject functions (CharacterClass::GetObject
	0x666E50) walk it from the last set to the first.

===============================================================================
*/

#ifndef __SETMANAGER_H__
#define __SETMANAGER_H__

#include "SortedArray.h"
#include "SetClass.h"

class TGStream;

// FUNCTION: STBC 0x00417e50 ??_GSetManager@@UAEPAXI@Z
class SetManager {
public:
					SetManager( void );
	virtual			~SetManager( void );

	void	Terminate( void );
	void	AddSet( SetClass *, char * );
	void	RemoveSet( char * );
	void	DeleteSet( char * );
	void	MakeRenderedSet( char * );
	// Renders the set, when there is one (0x418510).
	void	MakeRenderedSet( SetClass *set );
	// Passes the time to every set's Unknown410460 (0x418630; the editor's
	// destructor passes the game time).
	void	Unknown418630( float time );
	// Calls the rendered set's VirtualSlot26, which renders it through its
	// active camera, when there is one (0x418660).
	void	Unknown418660( void );
	// Updates one set a frame, round robin, skipping the rendered set and
	// the player's (a single-player game only) (invented name, 0x418530).
	void	Update( float time );
	// The saved game (UtopiaModule::ProcessSave and ProcessInternalLoad,
	// invented names).
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	void	RestoreSets( void );
	void	Clear( void );

	SetClass *		GetRenderedSet( void ) { return renderedSet; }
	void			ClearRenderedSet( void ) { renderedSet = NULL; }
	// deletes every set and forgets the rendered one (the SWIG wrapper's copy
	// of it stores the NULL)
	void			DeleteAllSets( void ) { sets.DeleteAll(); renderedSet = NULL; }
	int				GetNumSets( void ) { return sets.GetCount(); }
	SetClass **		GetSets( void ) { return sets.GetItems(); }		// invented name
	SetClass *		GetSet( char *name ) { return sets.Get( name ); }

	SetClass *		renderedSet;		// 0x04 the set MakeRenderedSet chose (invented name)
	SortedArray<SetClass, char *>	sets;	// 0x08 owned, by name (invented name)
	int				unknown14;			// 0x14
};

extern SetManager	g_kSetManager;

struct _object;
typedef _object PyObject;

// The Python methods SetManager.GetAllSets (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *		SetManager_GetAllSets( PyObject *self, PyObject *args );

#endif /* !__SETMANAGER_H__ */
