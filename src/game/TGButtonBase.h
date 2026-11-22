/*
===============================================================================

	TGButtonBase.h

	A pane that acts as a button: pressed with the left mouse button or
	Enter, it sends its activation event, and it sends its mouse down and
	mouse up events as it is pressed and released.

	0x58 bytes (its scalar deleting destructor frees 0x58).

	Evidence: the constructor (0x73F710) builds the pane at the given
	size, clears the three events (0x4C, 0x50, 0x54), stores the vtable
	0x897630 (90 slots; 82 to 89 are new) and disables itself through
	TGUIObject::SetEnabled. The SWIG wrappers TGButtonBase_* name slots 82
	to 89 (ClearActivationEvent to SendMouseUpEvent) and ClearMouseDown;
	each Set*Event takes a reference to the new event and releases the
	old one, and each Send*Event posts a duplicate of the event to
	g_kEventManager. The destructor (0x73F770) releases the activation and
	mouse down events only. The handlers are named by the strings
	"TGButtonBase::MouseHandler" and "TGButtonBase::KeyboardHandler". The
	inline functions the vtable points at were kept from the UI module's
	unit (0x7267B0 to 0x726860) and from 0x51B3E0.

===============================================================================
*/

#ifndef __TGBUTTONBASE_H__
#define __TGBUTTONBASE_H__

#include "TGPane.h"

class TGEvent;
class TGKeyboardEvent;
class TGMouseEvent;
class TGObject;

// GetObjType's value for a TGButtonBase.
#define TGBUTTONBASE_TYPE		0x206

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00726860 ??_GTGButtonBase@@UAEPAXI@Z
class TGButtonBase : public TGPane {
public:
	// FUNCTION: STBC 0x00726790 ??0TGButtonBase@@QAE@J@Z
					TGButtonBase( long id ) : TGPane( id ) {}
					TGButtonBase( float width = 0.0f, float height = 0.0f );
	virtual			~TGButtonBase( void );

	// TGObject
	// FUNCTION: STBC 0x007267b0 ?GetObjType@TGButtonBase@@UAEHXZ
	virtual int		GetObjType( void ) { return TGBUTTONBASE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00726820 ?GetClassName@TGButtonBase@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGButtonBase"; }
	// FUNCTION: STBC 0x00726830 ?GetSwigTypeName@TGButtonBase@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGButtonBase"; }
	// FUNCTION: STBC 0x00726840 ?GetPythonClassName@TGButtonBase@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGButtonBasePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00726850 ?GetClassHandlers@TGButtonBase@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	SetDisabled( bool layout = true );

	virtual void	ClearActivationEvent( void );
	virtual void	SetActivationEvent( TGEvent *event );
	virtual void	SendActivationEvent( void );
	// FUNCTION: STBC 0x0051b3e0 ?GetActivationEvent@TGButtonBase@@UAEPAVTGEvent@@XZ
	virtual TGEvent *	GetActivationEvent( void ) { return activationEvent; }
	virtual void	SetMouseDownEvent( TGEvent *event );
	virtual void	SendMouseDownEvent( void );
	virtual void	SetMouseUpEvent( TGEvent *event );
	virtual void	SendMouseUpEvent( void );

	static TGButtonBase *	Cast( TGObject *object );
	void	ClearMouseDown( bool sendActivation );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	TGEvent *		activationEvent;		// 0x4C a reference held; an ID between ReadFromStream and PostFixUp
	TGEvent *		mouseDownEvent;			// 0x50 likewise
	TGEvent *		mouseUpEvent;			// 0x54 likewise

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	static TGButtonBase *	Create( float width, float height ) { return new TGButtonBase( width, height ); }	// the SWIG wrapper TGButtonBase_Create expands it
};

/*
================
TGButtonBase::IsTypeOf
================
*/
// FUNCTION: STBC 0x007267c0
inline bool TGButtonBase::IsTypeOf( int type ) {
	if ( type == TGBUTTONBASE_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__TGBUTTONBASE_H__ */
