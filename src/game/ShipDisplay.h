/*
===============================================================================

	ShipDisplay.h

	The members of ShipDisplay that the SWIG wrappers ShipDisplay_* call and
	the functions written so far use. 0xBC bytes; the vtable 0x892160 is
	STStylizedWindow's with the destructor, the type, name and class handler
	slots and the stream, fix-up and post fix-up slots overridden.

===============================================================================
*/

#ifndef __SHIPDISPLAY_H__
#define __SHIPDISPLAY_H__

#include "STStylizedWindow.h"

class ShipClass;
class TGEvent;
class TGStream;
class TGObject;
class ShieldsDisplay;
class DamageDisplay;
class STFillGauge;

// GetObjType's value for a ShipDisplay (the type its Cast checks).
#define SHIPDISPLAY_TYPE		0x811D

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00546bf0 ??_GShipDisplay@@UAEPAXI@Z
class ShipDisplay : public STStylizedWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558920 ??0ShipDisplay@@QAE@J@Z @game/InterfaceModule
					ShipDisplay( long id ) : STStylizedWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					ShipDisplay( void );
	// The script-side constructor (the SWIG wrapper ShipDisplay_Create expands it).
	static ShipDisplay *	Create( void ) { return new ShipDisplay; }
	// FUNCTION: STBC 0x00546c20 ??1ShipDisplay@@UAE@XZ
	virtual			~ShipDisplay( void ) {}

	// TGObject
	// FUNCTION: STBC 0x00546b30 ?GetObjType@ShipDisplay@@UAEHXZ
	virtual int		GetObjType( void ) { return SHIPDISPLAY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00546bb0 ?GetClassName@ShipDisplay@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ShipDisplay"; }
	// FUNCTION: STBC 0x00546bc0 ?GetSwigTypeName@ShipDisplay@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ShipDisplay"; }
	// FUNCTION: STBC 0x00546bd0 ?GetPythonClassName@ShipDisplay@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ShipDisplayPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00546be0 ?GetClassHandlers@ShipDisplay@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	HandleShipDestroyed( TGEvent *event );		// handler "ShipDisplay::HandleShipDestroyed"
	void	HandleShipExitedSet( TGEvent *event );		// handler "ShipDisplay::HandleShipExitedSet"
	void	HandleChangeTarget( TGEvent *event );		// handler "ShipDisplay::HandleChangeTarget"

	STFillGauge *	GetHealthGauge( void ) { return healthGauge; }
	DamageDisplay *	GetDamageDisplay( void ) { return damageDisplay; }
	ShieldsDisplay *	GetShieldsDisplay( void ) { return shieldsDisplay; }
	void	SetHealthGauge( STFillGauge * healthGauge_ ) { healthGauge = healthGauge_; }
	void	SetDamageDisplay( DamageDisplay * damageDisplay_ ) { damageDisplay = damageDisplay_; }
	void	SetShieldsDisplay( ShieldsDisplay * shieldsDisplay_ ) { shieldsDisplay = shieldsDisplay_; }
	static ShipDisplay *	Cast( TGObject * );
	void	SetShipID( long );
	void	AddTargetChangeEvent( void );
	void	ResizeUI( void );
	void	RepositionUI( void );

	long	GetShipID( void ) { return shipID; }		// invented name

	void	SetShipIDVar( long shipID_ ) { shipID = shipID_; }
protected:
	static TGEventHandlerTable	classHandlers;			// invented name
	long			shipID;					// 0xAC the ship shown, 0 for none (invented name; SetShipID reads it back after the script's SetShipID ran)
	ShieldsDisplay *	shieldsDisplay;		// 0xB0
	DamageDisplay *	damageDisplay;			// 0xB4
	STFillGauge *	healthGauge;			// 0xB8
};

/*
================
ShipDisplay::IsTypeOf
================
*/
// FUNCTION: STBC 0x00546b40 ?IsTypeOf@ShipDisplay@@UAE_NH@Z
inline bool ShipDisplay::IsTypeOf( int type ) {
	if ( type == SHIPDISPLAY_TYPE ) {
		return true;
	}
	return STStylizedWindow::IsTypeOf( type );
}

#endif /* !__SHIPDISPLAY_H__ */
