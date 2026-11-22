/*
===============================================================================

	ShieldsDisplay.h

	The members of ShieldsDisplay that the SWIG wrappers ShieldsDisplay_* call, and the
	shield range checks it keeps. No virtual functions declared yet.

	Evidence: the constructor (0x5457E0) fills the 7 x 21 ints at 0x4C with
	FloatRangeWatcher::RID_INVALID; 0x545970 adds, for each of the shield
	class's seven watchers (the six facings and the overall one), checks at
	0.001 and at each 0.05 up to 1.0 that post ET_SHIELD_LEVEL_CHANGE to the
	ship; RemoveEvents removes them. The scalar deleting destructor frees
	0x298 bytes.

===============================================================================
*/

#ifndef __SHIELDSDISPLAY_H__
#define __SHIELDSDISPLAY_H__

#include "TGPane.h"
#include "ShieldClass.h"

class TGEvent;
class TGObject;

// A shield's level changed (the SWIG constant of the App module).
#define ET_SHIELD_LEVEL_CHANGE		0x8000A7

// The range checks of a shield: at SHIELD_LEVEL_LOWEST, then at each of the
// SHIELD_LEVEL_STEPS multiples of SHIELD_LEVEL_STEP up to 1.0.
#define SHIELD_LEVEL_STEPS		20
#define SHIELD_LEVEL_CHECKS		( SHIELD_LEVEL_STEPS + 1 )
#define SHIELD_LEVEL_LOWEST		0.001f
#define SHIELD_LEVEL_STEP		0.05f

// GetObjType's value for a ShieldsDisplay (the type its Cast checks).
#define SHIELDSDISPLAY_TYPE		0x80FF

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00545930 ??_GShieldsDisplay@@UAEPAXI@Z
class ShieldsDisplay : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005589e0 ??0ShieldsDisplay@@QAE@J@Z @game/InterfaceModule
					ShieldsDisplay( long id ) : TGPane( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// TGObject
	// FUNCTION: STBC 0x00545960 ??1ShieldsDisplay@@UAE@XZ
	virtual			~ShieldsDisplay( void ) {}
	// FUNCTION: STBC 0x00545880 ?GetObjType@ShieldsDisplay@@UAEHXZ
	virtual int		GetObjType( void ) { return SHIELDSDISPLAY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x005458f0 ?GetClassName@ShieldsDisplay@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ShieldsDisplay"; }
	// FUNCTION: STBC 0x00545900 ?GetSwigTypeName@ShieldsDisplay@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ShieldsDisplay"; }
	// FUNCTION: STBC 0x00545910 ?GetPythonClassName@ShieldsDisplay@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ShieldsDisplayPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00545920 ?GetClassHandlers@ShieldsDisplay@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static ShieldsDisplay *	Cast( TGObject * );
	void	UpdateForNewShip( void );
	void	RemoveEvents( void );
	void	ResizeUI( void );
	void	RepositionUI( void );

	void	AddEvents( void );							// invented name
	void	SetShipIcon( void );						// the script function it calls
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	Update( TGEvent *event );					// handler "ShieldsDisplay::Update"
	void	ShipIdentified( TGEvent *event );			// handler "ShieldsDisplay::ShipIdentified"

protected:
	// Colours the facing's icon by the shield's level (invented name).
	void	UpdateShieldIcon( ShieldClass *shields, int shield );

protected:
	static TGEventHandlerTable	classHandlers;	// invented name
	int				shieldChecks[ShieldClass::NUM_SHIELDS + 1][SHIELD_LEVEL_CHECKS];	// 0x4C range check IDs per shield watcher; RID_INVALID without one (invented name)

	// Declared for the SWIG wrappers that call them.
public:
					ShieldsDisplay( float width, float height );
	static ShieldsDisplay *	Create( float width, float height ) { return new ShieldsDisplay( width, height ); }	// the SWIG wrapper ShieldsDisplay_Create expands it
};

/*
================
ShieldsDisplay::IsTypeOf
================
*/
// FUNCTION: STBC 0x00545890 ?IsTypeOf@ShieldsDisplay@@UAE_NH@Z
inline bool ShieldsDisplay::IsTypeOf( int type ) {
	if ( type == SHIELDSDISPLAY_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__SHIELDSDISPLAY_H__ */
