/*
===============================================================================

	RadarDisplay.h

	The radar of the tactical window: a stylized window of 0xAC bytes whose
	first child is the RadarScope. The vtable 0x891C70 is
	STStylizedWindow's with the destructor, the type, name and class handler
	slots, SetHighlighted and SetNotHighlighted overridden.

===============================================================================
*/

#ifndef __RADARDISPLAY_H__
#define __RADARDISPLAY_H__

#include "STStylizedWindow.h"

class TGEvent;
class TGMouseEvent;
class TGObject;

// GetObjType's value for a RadarDisplay (the type its Cast checks).
#define RADARDISPLAY_TYPE		0x8103

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00542480 ??_GRadarDisplay@@UAEPAXI@Z
class RadarDisplay : public STStylizedWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558bf0 ??0RadarDisplay@@QAE@J@Z @game/InterfaceModule
					RadarDisplay( long id ) : STStylizedWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// FUNCTION: STBC 0x005424b0 ??1RadarDisplay@@UAE@XZ
	virtual			~RadarDisplay( void ) {}

	// TGObject
	// FUNCTION: STBC 0x005423c0 ?GetObjType@RadarDisplay@@UAEHXZ
	virtual int		GetObjType( void ) { return RADARDISPLAY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00542440 ?GetClassName@RadarDisplay@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "RadarDisplay"; }
	// FUNCTION: STBC 0x00542450 ?GetSwigTypeName@RadarDisplay@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_RadarDisplay"; }
	// FUNCTION: STBC 0x00542460 ?GetPythonClassName@RadarDisplay@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "RadarDisplayPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00542470 ?GetClassHandlers@RadarDisplay@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	SetHighlighted( bool layout = true );
	virtual void	SetNotHighlighted( bool layout = true );

	void	SetColorBasedOnFlags( void );
	static RadarDisplay *	Cast( TGObject * );
	void	ResizeUI( void );
	void	RepositionUI( void );
	void	Update( void );								// invented name: the scope redraws

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	MouseHandler( TGMouseEvent *event );		// handler "RadarDisplay::MouseHandler"

protected:
	static TGEventHandlerTable	classHandlers;			// invented name

	// Declared for the SWIG wrappers that call them.
public:
					RadarDisplay( float width, float height );
	static RadarDisplay *	Create( float width, float height ) { return new RadarDisplay( width, height ); }	// the SWIG wrapper RadarDisplay_Create expands it
};

/*
================
RadarDisplay::IsTypeOf
================
*/
// FUNCTION: STBC 0x005423d0 ?IsTypeOf@RadarDisplay@@UAE_NH@Z
inline bool RadarDisplay::IsTypeOf( int type ) {
	if ( type == RADARDISPLAY_TYPE ) {
		return true;
	}
	return STStylizedWindow::IsTypeOf( type );
}

#endif /* !__RADARDISPLAY_H__ */
