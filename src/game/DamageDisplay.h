/*
===============================================================================

	DamageDisplay.h

	The members of DamageDisplay that the SWIG wrappers DamageDisplay_* call and the functions
	written so far use. The vtable 0x891B28 has TGPane's 82 slots and no more.

===============================================================================
*/

#ifndef __DAMAGEDISPLAY_H__
#define __DAMAGEDISPLAY_H__

#include "NiTList.h"
#include "TGPane.h"

class DamageIcon;
class ShipClass;
class ShipSubsystem;
class TGEvent;
class TGObject;

// GetObjType's value for a DamageDisplay (the type its Cast checks).
#define DAMAGEDISPLAY_TYPE		0x8100

// The event the display's timer sends it (invented name); "DamageDisplay::HandleDamageDisplayRefresh" handles it.
#define ET_DAMAGE_DISPLAY_REFRESH	0x8000B4

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00540fd0 ??_GDamageDisplay@@UAEPAXI@Z
class DamageDisplay : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558a90 ??0DamageDisplay@@QAE@J@Z @game/InterfaceModule
					DamageDisplay( long id ) : TGPane( id ), damageIcons( 2 ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// TGObject
	virtual			~DamageDisplay( void );
	// FUNCTION: STBC 0x00540f20 ?GetObjType@DamageDisplay@@UAEHXZ
	virtual int		GetObjType( void ) { return DAMAGEDISPLAY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00540f90 ?GetClassName@DamageDisplay@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "DamageDisplay"; }
	// FUNCTION: STBC 0x00540fa0 ?GetSwigTypeName@DamageDisplay@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_DamageDisplay"; }
	// FUNCTION: STBC 0x00540fb0 ?GetPythonClassName@DamageDisplay@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "DamageDisplayPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00540fc0 ?GetClassHandlers@DamageDisplay@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static DamageDisplay *	Cast( TGObject * );
	// The subsystem's icon in the "DamageIcons" group; STRepairButton
	// takes it too (invented name).
	static int	GetIconNumber( ShipSubsystem *subsystem );
	void	UpdateForNewShip( void );
	void	RemoveEvents( void );
	void	ResizeUI( void );
	void	RepositionUI( void );
	ShipClass *	GetShip( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	HandleSubsystemEvents( TGEvent *event );
	void	HandleDamageDisplayRefresh( TGEvent *event );

protected:
	void	Unknown5414B0( void );		// UpdateForNewShip calls it before RemoveEvents (removes the icons, then rebuilds them)
	void	AddSubsystemIcons( ShipSubsystem *subsystem );	// invented name: an icon for the subsystem, then one for each of its children
	void	Unknown5410B0( void );		// UpdateForNewShip calls it last (listens to the ship's subsystem events)

	NiTList<DamageIcon *>	damageIcons;	// 0x4C the icons UpdateForNewShip removes (invented name; the element type is unproven)
	static TGEventHandlerTable	classHandlers;	// invented name
	long			timerID;				// 0x64 a timer RemoveEvents deletes, 0 for none (invented name)

	// Declared for the SWIG wrappers that call them.
public:
					DamageDisplay( float width, float height );
	static DamageDisplay *	Create( float width, float height ) { return new DamageDisplay( width, height ); }	// the SWIG wrapper DamageDisplay_Create expands it
};

/*
================
DamageDisplay::IsTypeOf
================
*/
// FUNCTION: STBC 0x00540f30 ?IsTypeOf@DamageDisplay@@UAE_NH@Z
inline bool DamageDisplay::IsTypeOf( int type ) {
	if ( type == DAMAGEDISPLAY_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__DAMAGEDISPLAY_H__ */
