/*
===============================================================================

	SortedRegionMenu.h

	The members of SortedRegionMenu that the SWIG wrappers SortedRegionMenu_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __SORTEDREGIONMENU_H__
#define __SORTEDREGIONMENU_H__

#include "STCharacterMenu.h"

class STWarpButton;
class TGIcon;
class TGUIObject;
class TGEvent;
class TGObjPtrEvent;
class SetClass;
class TGObject;

// GetObjType's value for a SortedRegionMenu (the type its Cast checks).
#define SORTEDREGIONMENU_TYPE		0x80FC

// The event that the activation sends to the Helm's menu (invented name; the bridge's Helm character is looked up by name).
#define ET_SORTED_REGION_ACTIVATED	0x80002F

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TGString;
// FUNCTION: STBC 0x00516fa0 ??_GSortedRegionMenu@@UAEPAXI@Z
class SortedRegionMenu : public STCharacterMenu {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005581f0 ??0SortedRegionMenu@@QAE@J@Z @game/InterfaceModule
					SortedRegionMenu( long id ) : STCharacterMenu( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	virtual			~SortedRegionMenu( void );

	// TGObject
	// FUNCTION: STBC 0x00516ed0 ?GetObjType@SortedRegionMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return SORTEDREGIONMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00516f60 ?GetClassName@SortedRegionMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "SortedRegionMenu"; }
	// FUNCTION: STBC 0x00516f70 ?GetSwigTypeName@SortedRegionMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_SortedRegionMenu"; }
	// FUNCTION: STBC 0x00516f80 ?GetPythonClassName@SortedRegionMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "SortedRegionMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00516f90 ?GetClassHandlers@SortedRegionMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGPane
	virtual void	InsertChild( int index, TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true );

	// STMenu
	virtual void	SendActivationEvent( void );
	virtual void	Open( bool layout = true );
	virtual void	VirtualSlot115( void );				// 0x517710: clears the menu's info, then closes the ancestors that are SortedRegionMenus

	static SortedRegionMenu *	Cast( TGObject * );
	char *	GetRegionName( void ) { return regionName; }
	char *	GetMissionName( void ) { return missionName; }
	char *	GetEpisodeName( void ) { return episodeName; }
	char *	GetPlacementName( void ) { return placementName; }
	static void	SetWarpButton( STWarpButton * );
	// The warp button and the set course menu in a saved game, as their IDs
	// until PostFixUpMenus (invented names).
	static bool	WriteMenus( TGStream *stream );
	static bool	ReadMenus( TGStream *stream );
	static bool	FixUpMenus( void );
	static void	PostFixUpMenus( void );
	static STWarpButton *	GetWarpButton( void ) { return warpButton; }
	static SortedRegionMenu *	GetRoot( void ) { return setCourseMenu; }
	static SortedRegionMenu *	CreateNull( void ) { return NULL; }
	void	SetUseLargeLoadingScreen( bool use ) { useLargeLoadingScreen = use; }
	static void	SetPauseSorting( bool pause ) {}
	static bool	IsSortingPaused( void ) { return true; }
	void	SetWarpToHere( void );
	void	Unknown517A00( void );		// invented name: puts the children in order (see its banner)
	void	ClearInfo( void );
	static void	ClearSetCourseMenu( void );
	SetClass *	InitializeAllSets( void );
	static void	RegisterHandlerNames( void );		// invented name

protected:
	void	SetPlayerHandler( TGEvent *event );		// registered under its name (0x517DF0)
	void	PlayerShipMovedHandler( TGEvent *event );		// registered under its name (0x517DF0)
	bool	Unknown517E80( char *setName );		// invented name: moves the region that holds the set to the front
	void	Unknown517C60( void );		// takes the two separator icons out of the menu
	int		Compare( SortedRegionMenu *other );		// invented name: the order InsertChild keeps
	// Three owned strings the menu keeps, set to a copy of the string (NULL for none); ClearInfo clears them on each menu
	// (invented names). The first two then put the menu back in its parent's parent, to be sorted again.

	char *			regionName;				// 0x5C owned
	char *			episodeName;				// 0x60 owned
	char *			missionName;				// 0x64 owned
	char *			placementName;				// 0x68 owned
	bool			useLargeLoadingScreen;	// 0x6C
	bool			unknown6D;				// 0x6D cleared by the constructor; not modelled yet
	TGIcon *		unknown70;				// 0x70 a separator icon, one of the two children ClearSetCourseMenu removes
	TGIcon *		unknown74;				// 0x74 the other separator icon

	// The menu ClearSetCourseMenu empties (identity unproven: it is read only through the members above).
	static TGEventHandlerTable	classHandlers;	// invented name
	static SortedRegionMenu *	setCourseMenu;		// invented name
	// The warp button SetWarpButton is given.
	static STWarpButton *	warpButton;				// invented name

	// Declared for the SWIG wrappers that call them.
public:
	// The four setters below: the SWIG wrappers SortedRegionMenu_Set<Region|Episode|Mission|Placement>Name call them.
	void	SetEpisodeName( char *string );
	void	SetMissionName( char *string );
	void	SetPlacementName( char *string );
	void	SetRegionName( char *string );		// sets the string at 0x5C, with no re-sorting
					SortedRegionMenu( char *name, char *region, char *placement );
	static SortedRegionMenu *	Create( char *name, char *region, char *placement ) { return new SortedRegionMenu( name, region, placement ); }	// the SWIG wrapper SortedRegionMenu_Create expands it
					SortedRegionMenu( TGString *name, char *region, char *placement );
	static SortedRegionMenu *	CreateW( TGString *name, char *region, char *placement ) { return new SortedRegionMenu( name, region, placement ); }	// the SWIG wrapper SortedRegionMenu_CreateW expands it
};

/*
================
SortedRegionMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x00516ee0 ?IsTypeOf@SortedRegionMenu@@UAE_NH@Z
inline bool SortedRegionMenu::IsTypeOf( int type ) {
	if ( type == SORTEDREGIONMENU_TYPE ) {
		return true;
	}
	return STCharacterMenu::IsTypeOf( type );
}

#endif /* !__SORTEDREGIONMENU_H__ */
