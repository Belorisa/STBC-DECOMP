/*
===============================================================================

	Mission.h

	The script object of a mission. The constructor (0x408070) calls
	ScriptObject's with the first event type 0x80C001, stores the vtable
	0x888864 (28 slots: the destructor, GetObjType, IsTypeOf, the stream
	functions, the class strings, GetClassHandlers, Initialize and
	Terminate overridden) and makes the four Python ObjectGroup instances
	(0x50-0x5C); the first three are the affiliation groups
	ObjectClass::GetAffiliationColor (0x436250) converts and asks, the
	fourth is the tractor group the scripts name (GetTractorGroup). 0x60
	bytes, allocated from the pools; GetObjType returns 0x80D0; the class
	strings are "Mission", "_p_Mission" and "MissionPtr". Episode::LoadMission
	makes the missions.

===============================================================================
*/

#ifndef __MISSION_H__
#define __MISSION_H__

#include <string.h>
#include "Python.h"
#include "ScriptObject.h"
#include "NiTList.h"

class ShipClass;
class TGEvent;
class TGStream;

/*
================
PrecreatedShip

A ship the mission's script asked for by name before it exists (invented name). AddPrecreatedShip
makes one per requested copy, with the name copied and no object yet.
================
*/
struct PrecreatedShip {
					PrecreatedShip( void ) {}	// the stream constructor: the stream fills the members
	char *			name;		// 0x00 a heap copy of the ship's script name
	long			id;			// 0x04 the ID of the created ship; 0 until there is one

					PrecreatedShip( char *shipName ) {
						name = new char[strlen( shipName ) + 1];
						strcpy( name, shipName );
						id = 0;
					}
};

// GetObjType's value for a Mission.
#define MISSION_TYPE			0x80D0

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004082d0 ??_GMission@@UAEPAXI@Z
// FUNCTION: STBC 0x00408300 ??1Mission@@UAE@XZ
class Mission : public ScriptObject {
public:
					Mission( char *script );
	// FUNCTION: STBC 0x0043d900 ??0Mission@@QAE@J@Z @game/UtopiaModule
					Mission( long id ) : ScriptObject( id ), precreatedShips( 2 ) {}	// the loader's: the stream fills the members
	virtual			~Mission( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ScriptObject
	virtual void	Initialize( TGEvent *event );
	virtual void	Terminate( TGEvent *event );

	static int	GetNextEventType( void );

	// Register the class's handler methods by name, and add them to its
	// table (invented names; Game's registration calls them).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );
	void	PlayerDied( TGEvent *event );			// 0x409230
	void	PlayerChanged( TGEvent *event );		// 0x409270
	void	PlayerEnteredSet( TGEvent *event );		// 0x4090D0
	void	PlayerExitedSet( TGEvent *event );		// 0x409170
	bool	AddPrecreatedShip( char *, int );
	ShipClass *	GetPrecreatedShip( char * );

	PyObject *		GetFriendlyGroup( void ) { return friendlyGroup; }	// invented name
	PyObject *		GetEnemyGroup( void ) { return enemyGroup; }		// invented name
	PyObject *		GetNeutralGroup( void ) { return neutralGroup; }	// invented name
	PyObject *		GetTractorGroup( void ) { return tractorGroup; }	// the scripts' name

protected:
	// Makes the ships the script asked for (invented name).
	bool			CreatePrecreatedShips( void );
	// Deletes the precreated ships still listed (invented name).
	void			DeletePrecreatedShips( void );
	void			AddPlayerSetHandlers( void );		// invented name
	void			AddPlayerDiedHandler( void );		// invented name
	void			WatchPlayerDie( void );				// invented name
	void			FilterBridgeSounds( void );			// invented name

	NiTList<PrecreatedShip *>	precreatedShips;	// 0x38 the ships asked for by name (invented name)
	PyObject *		friendlyGroup;		// 0x50 the ObjectGroup of the player's friends (invented name)
	PyObject *		enemyGroup;			// 0x54 (invented name)
	PyObject *		neutralGroup;		// 0x58 (invented name)
	PyObject *		tractorGroup;		// 0x5C the ObjectGroup of the ships the tractor beams hold (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

// The Python methods Mission.GetFriendlyGroup, Mission.GetEnemyGroup, Mission.GetNeutralGroup, Mission.GetTractorGroup (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *		Mission_GetFriendlyGroup( PyObject *self, PyObject *args );
PyObject *		Mission_GetEnemyGroup( PyObject *self, PyObject *args );
PyObject *		Mission_GetNeutralGroup( PyObject *self, PyObject *args );
PyObject *		Mission_GetTractorGroup( PyObject *self, PyObject *args );

#endif /* !__MISSION_H__ */
