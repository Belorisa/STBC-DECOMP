/*
===============================================================================

	TGButton.h

	A button drawn as an icon of an icon group: its one child is a TGIcon
	that shows the group's first icon while the button is enabled and
	selected, the next one while it is enabled and not selected, and the
	one after that while it is disabled. The button takes the icon's size.

	0x64 bytes (its scalar deleting destructor frees 0x64).

	Evidence: the constructor (0x7438E0, called by new_TGButton with the
	group name, the first icon number and the colour) builds the button
	base at no size, stores the vtable 0x897798 (93 slots; 90 to 92 are
	new), clears the enabled and selected flags, clears the group name
	(0x58), copies the colour into a NiColorA of its own (0x60) and calls
	slot 90. The SWIG wrappers name slots 90 (TGButton_SetIcons) and 91
	(TGButton_SetIconColor), and TGButton_GetFirstIconNum reads 0x5C.
	GetObjType returns 0x207. The type's inline members are emitted with
	the UI module's class factories (0x7268E0 to 0x726990).

===============================================================================
*/

#ifndef __TGBUTTON_H__
#define __TGBUTTON_H__

#include "TGButtonBase.h"

class NiColorA;
class TGObject;

// GetObjType's value for a TGButton.
#define TGBUTTON_TYPE			0x207

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00726990 ??_GTGButton@@UAEPAXI@Z
class TGButton : public TGButtonBase {
public:
	// FUNCTION: STBC 0x007268c0 ??0TGButton@@QAE@J@Z
					TGButton( long id ) : TGButtonBase( id ) {}
					TGButton( char *groupName = NULL, int firstIconNum = 0, NiColorA *color = NULL );
	virtual			~TGButton( void );

	// TGObject
	// FUNCTION: STBC 0x007268e0 ?GetObjType@TGButton@@UAEHXZ
	virtual int		GetObjType( void ) { return TGBUTTON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00726950 ?GetClassName@TGButton@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGButton"; }
	// FUNCTION: STBC 0x00726960 ?GetSwigTypeName@TGButton@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGButton"; }
	// FUNCTION: STBC 0x00726970 ?GetPythonClassName@TGButton@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGButtonPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00726980 ?GetClassHandlers@TGButton@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	SetEnabled( bool layout = true );
	virtual void	SetDisabled( bool layout = true );
	virtual void	SetSelected( bool layout = true );
	virtual void	SetNotSelected( bool layout = true );

	virtual void	SetIcons( char *groupName, int firstIconNum );
	virtual void	SetIconColor( NiColorA *color = NULL );
	virtual void	UpdateIcon( bool layout = true );		// invented name

	static TGButton *	Cast( TGObject *object );
	static TGButton *	Create( char *groupName = NULL, int firstIconNum = 0, NiColorA *color = NULL ) { return new TGButton( groupName, firstIconNum, color ); }	// the SWIG wrapper TGButton_Create expands it

	int				GetFirstIconNum( void ) { return firstIconNum; }

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	char *			iconGroupName;			// 0x58 owned copy, or NULL
	int				firstIconNum;			// 0x5C the enabled and selected icon; the next two follow it
	NiColorA *		iconColor;				// 0x60 owned, or NULL for the icon's own white

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGButton::IsTypeOf
================
*/
// FUNCTION: STBC 0x007268f0
inline bool TGButton::IsTypeOf( int type ) {
	if ( type == TGBUTTON_TYPE ) {
		return true;
	}
	return TGButtonBase::IsTypeOf( type );
}

#endif /* !__TGBUTTON_H__ */
