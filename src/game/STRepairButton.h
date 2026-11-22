/*
===============================================================================

	STRepairButton.h

	A button of the engineering repair pane: an STButton named for a
	subsystem, with the subsystem's damage icon (its seventh child) after
	the right end cap, coloured by the subsystem's state.

	Evidence: the handler name "STRepairButton::HandleSubsystemStateChanged"
	(0x8E32F0), registered by 0x52C9C0, which InterfaceModule's
	RegisterHandlerNames calls with the other UI classes' (between STMenu's and
	STTopLevelMenu's); the constructors 0x52C7C0 and 0x52C940 call STButton's
	and store the vtable 0x890554. Cast (0x52C770) checks the type 0x80EE; 0x52D190 reads
	"g_fRepairButtonFontSize" into 0x989608, which InterfaceModule's
	HandleUISwitchIconGroups gives the buttons' names. The class has no SWIG
	wrappers. The vtable has STButton's 91 slots; it overrides GetObjType,
	IsTypeOf, the stream slots, the three class strings, GetClassHandlers,
	Resize, GetDesiredSize and Layout. The destructor is implicit (0x52C930
	jumps to TGButtonBase's).

===============================================================================
*/

#ifndef __STREPAIRBUTTON_H__
#define __STREPAIRBUTTON_H__

#include "STButton.h"

class TGEvent;
class ShipSubsystem;
class TGObject;

// GetObjType's value for an STRepairButton (the type its Cast checks).
#define STREPAIRBUTTON_TYPE		0x80EE

// The damage icon's place among the button's children, after STButton's (invented name).
#define STREPAIRBUTTON_ICON		6

// FUNCTION: STBC 0x0052c900 ??_GSTRepairButton@@UAEPAXI@Z
// FUNCTION: STBC 0x0052c930 ??1STRepairButton@@UAE@XZ
class STRepairButton : public STButton {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005578e0 ??0STRepairButton@@QAE@J@Z @game/InterfaceModule
					STRepairButton( long id ) : STButton( id ) {}
	// A button named for the subsystem, with its damage icon and its
	// name in the repair font; the rest goes to STButton's.
					STRepairButton( ShipSubsystem *subsystem, char *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum );
					STRepairButton( ShipSubsystem *subsystem, TGString *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum );
	static STRepairButton *	Cast( TGObject *object );

	// TGObject
	// FUNCTION: STBC 0x0052c840 ?GetObjType@STRepairButton@@UAEHXZ
	virtual int		GetObjType( void ) { return STREPAIRBUTTON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0052c8c0 ?GetClassName@STRepairButton@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STRepairButton"; }
	// FUNCTION: STBC 0x0052c8d0 ?GetSwigTypeName@STRepairButton@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STRepairButton"; }
	// FUNCTION: STBC 0x0052c8e0 ?GetPythonClassName@STRepairButton@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STRepairButtonPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0052c8f0 ?GetClassHandlers@STRepairButton@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	GetDesiredSize( NiPoint2 *size );
	virtual void	Layout( void );

	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// Reads the name's font size, g_fRepairButtonFontSize, from the
	// resolution's LCARS module (invented name).
	static void		LoadFontSize( void );

	// The name's font size; NI_INFINITY until LoadFontSize reads it (invented name).
	static float	fontSize;

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleSubsystemStateChanged( TGEvent *event );	// handler "STRepairButton::HandleSubsystemStateChanged"

protected:
	// Adds the subsystem's damage icon and listens for its ship's
	// subsystem state changes (invented name).
	void			SetSubsystem( ShipSubsystem *subsystem );
	// Colours the damage icon: destroyed, disabled or damaged (invented name).
	void			UpdateIconColor( ShipSubsystem *subsystem );
	// Gives the name the repair font size (invented name).
	void			UpdateFontSize( void );

	static TGEventHandlerTable	classHandlers;		// invented name

	long			subsystemID;			// 0x114 the subsystem the button stands for (invented name)
};

#endif /* !__STREPAIRBUTTON_H__ */
