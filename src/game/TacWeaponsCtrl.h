/*
===============================================================================

	TacWeaponsCtrl.h

	The tactical weapons control: a stylized window, 0xB0 bytes. The vtable
	0x8922AC is STStylizedWindow's with the destructor, the type, name and
	class handler slots, the stream slots, SetHighlighted and
	SetNotHighlighted overridden. The one control that exists is kept in
	a static (0x98AC54): the constructor and ReadFromStream set it, the
	destructor clears it, and the SWIG wrapper
	TacWeaponsCtrl_GetTacWeaponsCtrl returns it.

===============================================================================
*/

#ifndef __TACWEAPONSCTRL_H__
#define __TACWEAPONSCTRL_H__

#include "STStylizedWindow.h"

class STToggle;
class TGEvent;
class TGEventHandlerObject;
class TGObject;
class TGStream;
class TGUIObject;

// GetObjType's value for a TacWeaponsCtrl (the type its Cast checks).
#define TACWEAPONSCTRL_TYPE		0x8106

// The event type the toggles of the control send (invented name).
#define ET_TAC_TRACTOR_BEAM_STATE	0x8000AF
#define ET_TAC_CLOAK_STATE			0x8000B0		// invented name
#define ET_TAC_TORPEDO_SPREAD_STATE	0x8000AE		// invented name
#define ET_TAC_PHASER_INTENSITY		0x8000AC		// invented name
#define ET_TAC_TORPEDO_TYPE_STATE	0x8000AD		// invented name
#define ET_TAC_WEAPONS_REFRESH		0x8000B1		// invented name: the control's timer sends it

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00547db0 ??_GTacWeaponsCtrl@@UAEPAXI@Z
class TacWeaponsCtrl : public STStylizedWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558d80 ??0TacWeaponsCtrl@@QAE@J@Z @game/InterfaceModule
					TacWeaponsCtrl( long id ) : STStylizedWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					TacWeaponsCtrl( float width, float height );
	virtual			~TacWeaponsCtrl( void );

	// TGObject
	// FUNCTION: STBC 0x00547cf0 ?GetObjType@TacWeaponsCtrl@@UAEHXZ
	virtual int		GetObjType( void ) { return TACWEAPONSCTRL_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00547d70 ?GetClassName@TacWeaponsCtrl@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TacWeaponsCtrl"; }
	// FUNCTION: STBC 0x00547d80 ?GetSwigTypeName@TacWeaponsCtrl@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TacWeaponsCtrl"; }
	// FUNCTION: STBC 0x00547d90 ?GetPythonClassName@TacWeaponsCtrl@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TacWeaponsCtrlPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00547da0 ?GetClassHandlers@TacWeaponsCtrl@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	SetHighlighted( bool layout = true );
	virtual void	SetNotHighlighted( bool layout = true );

	// The control; NULL until one is made (the SWIG wrapper TacWeaponsCtrl_GetTacWeaponsCtrl).
	static TacWeaponsCtrl *	GetTacWeaponsCtrl( void ) { return instance; }

	static TacWeaponsCtrl *	Cast( TGObject * );
	static TacWeaponsCtrl *	Create( float width, float height ) { return new TacWeaponsCtrl( width, height ); }	// the SWIG wrapper TacWeaponsCtrl_Create expands it
	STToggle *	GetPhaserIntensityToggle( void );
	STToggle *	GetTorpTypeToggle( void );
	STToggle *	GetTorpSpreadToggle( void );
	STToggle *	GetBeamToggle( void );
	STToggle *	GetCloakToggle( void );
	void	RefreshPhaserSettings( void );
	void	RefreshTorpedoSettings( void );
	void	RefreshTractorToggle( void );
	void	RefreshCloakToggle( void );
	void	Init( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	PhaserSettingChange( TGEvent *event );
	void	TorpedoTypeToggle( TGEvent *event );
	void	TorpedoSpreadToggle( TGEvent *event );
	void	OtherBeamToggle( TGEvent *event );
	void	HandleCloakToggle( TGEvent *event );
	void	HandleCloakToggleRefresh( TGEvent *event );
	static void		KeyboardHandler( TGEventHandlerObject *object, TGEvent *event );
	void	HandleTorpedoCountChanged( TGEvent *event );
	void	HandleSelectOption( TGEvent *event );
	void	HandlePlayerChanged( TGEvent *event );
	void	HandleTargetChanged( TGEvent *event );
	void	HandleSubsystemDisabled( TGEvent *event );
	void	HandleSubsystemOperational( TGEvent *event );
	void	HandleTractorBeamFiring( TGEvent *event );

protected:
	// The toggles' actions (invented names): the next torpedo type with
	// torpedoes left, the spread, the tractor beam and the cloak.
	void	SelectTorpedoType( void );
	void	SetTorpedoSpread( void );
	void	SetTractorBeam( void );
	void	SetCloak( void );

	void	SetColorBasedOnFlags( void );				// empty (invented name)
	TGUIObject *	Unknown548120( char *lcars );		// Init builds the first pane with it (invented name)
	TGUIObject *	Unknown548A00( char *lcars );		// Init builds the second pane with it (invented name)
	TGUIObject *	Unknown548E60( char *lcars );		// Init builds the third pane with it (invented name)

	static TacWeaponsCtrl *	instance;				// 0x98AC54 the one control (invented name)
	static TGEventHandlerTable	classHandlers;		// invented name
	long			timerID;						// 0xAC the timer that refreshes the control (invented name)
};

/*
================
TacWeaponsCtrl::IsTypeOf
================
*/
// FUNCTION: STBC 0x00547d00 ?IsTypeOf@TacWeaponsCtrl@@UAE_NH@Z
inline bool TacWeaponsCtrl::IsTypeOf( int type ) {
	if ( type == TACWEAPONSCTRL_TYPE ) {
		return true;
	}
	return STStylizedWindow::IsTypeOf( type );
}

#endif /* !__TACWEAPONSCTRL_H__ */
