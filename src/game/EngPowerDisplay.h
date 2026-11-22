/*
===============================================================================

	EngPowerDisplay.h

	The engineering power display: a pane of 0x4C bytes the "Bridge.PowerDisplay"
	script fills with gauges. The vtable 0x8926AC is TGPane's 82 slots with
	the destructor, the type, name and class handler slots and ReadFromStream
	overridden. The one display that exists is kept in a static (0x98B044):
	the constructor and ReadFromStream set it, the destructor clears it, and
	the SWIG wrapper EngPowerDisplay_GetPowerDisplay returns it.

===============================================================================
*/

#ifndef __ENGPOWERDISPLAY_H__
#define __ENGPOWERDISPLAY_H__

#include "TGPane.h"

class STFillGauge;
class TGObject;
class TGStream;

// GetObjType's value for an EngPowerDisplay (the type its Cast checks).
#define ENGPOWERDISPLAY_TYPE		0x8108

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0054fac0 ??_GEngPowerDisplay@@UAEPAXI@Z
class EngPowerDisplay : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558ef0 ??0EngPowerDisplay@@QAE@J@Z @game/InterfaceModule
					EngPowerDisplay( long id ) : TGPane( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					EngPowerDisplay( float width, float height );
	virtual			~EngPowerDisplay( void );

	// TGObject
	// FUNCTION: STBC 0x0054fa10 ?GetObjType@EngPowerDisplay@@UAEHXZ
	virtual int		GetObjType( void ) { return ENGPOWERDISPLAY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0054fa80 ?GetClassName@EngPowerDisplay@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "EngPowerDisplay"; }
	// FUNCTION: STBC 0x0054fa90 ?GetSwigTypeName@EngPowerDisplay@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_EngPowerDisplay"; }
	// FUNCTION: STBC 0x0054faa0 ?GetPythonClassName@EngPowerDisplay@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EngPowerDisplayPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0054fab0 ?GetClassHandlers@EngPowerDisplay@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// The display; NULL until one is made (the SWIG wrapper EngPowerDisplay_GetPowerDisplay).
	static EngPowerDisplay *	GetPowerDisplay( void ) { return instance; }

	static EngPowerDisplay *	Cast( TGObject * );
	STFillGauge *	CreateBatteryGauge( int );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

protected:
	void	CallInitScript( void );						// invented name

	static EngPowerDisplay *	instance;				// 0x98B044 the one display (invented name)
	static TGEventHandlerTable	classHandlers;			// invented name

	// Declared for the SWIG wrappers that call them.
public:
	static EngPowerDisplay *	Create( float width, float height ) { return new EngPowerDisplay( width, height ); }	// the SWIG wrapper EngPowerDisplay_Create expands it
};

/*
================
EngPowerDisplay::IsTypeOf
================
*/
// FUNCTION: STBC 0x0054fa20 ?IsTypeOf@EngPowerDisplay@@UAE_NH@Z
inline bool EngPowerDisplay::IsTypeOf( int type ) {
	if ( type == ENGPOWERDISPLAY_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__ENGPOWERDISPLAY_H__ */
