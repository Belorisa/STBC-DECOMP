/*
===============================================================================

	STSubPane.h

	The pane that holds a menu's items: buttons, toggles and sub-menus,
	stacked top to bottom with a vertical spacing between them. It sizes
	itself to its contents, keeps a containing stylized window scrolled
	to its focus, and moves the focus between its options and into the
	neighbouring menus on the keyboard's select events.

	0x54 bytes (its scalar deleting destructor frees 0x54).

	Evidence: the constructor (0x532F50, called by STSubPane_Create with
	DEFAULT_ST_INDENT_VERT) builds the pane at no size, stores the vtable
	0x890AC4 (88 slots; 82 to 87 are new) and keeps the spacing (0x4C) and
	the fill flag (0x50); GetObjType returns 0x80EA. The SWIG wrappers
	STSubPane_* name the members: GetVertSpacing reads 0x4C,
	SetExpandToFillParent writes 0x50, IsRadioGroup and SetRadioGroup the
	top bit of the flags, and GetSubmenu, GetSubmenuW, GetButton,
	GetButtonW, RemoveItem and RemoveItemW call slots 82 to 87. The
	handler names are the strings "STSubPane::KeyboardHandler" and
	"STSubPane::HandleSelectOption". The stream functions sit in a second
	unit (STSubPaneStream.cpp) that includes <iostream>.

===============================================================================
*/

#ifndef __STSUBPANE_H__
#define __STSUBPANE_H__

#include "TGPane.h"
#include "TGString.h"
#include "InterfaceModule.h"

class STButton;
class STMenu;
class STStylizedWindow;
class TGIntEvent;
class TGObject;

// GetObjType's value for an STSubPane.
#define STSUBPANE_TYPE			0x80EA

// The flag SetRadioGroup sets: choosing one button of the pane unchooses
// the others (invented name).
#define STSPF_RADIO_GROUP		0x80000000

// The input events of the menus (the SWIG constants): select the option
// numbered by the event's value, from 1; and move the selection by the
// event's value, or activate the selected option when it is 0.
#define ET_INPUT_SELECT_X		0x800490
#define ET_INPUT_SELECT_OPTION	0x800492

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00533170 ??_GSTSubPane@@UAEPAXI@Z
class STSubPane : public TGPane {
public:
					STSubPane( float vertSpacing, bool expandToFillParent );
					STSubPane( long id, bool expandToFillParent );
	virtual			~STSubPane( void );

	// TGObject
	// FUNCTION: STBC 0x00532f80 ?GetObjType@STSubPane@@UAEHXZ
	virtual int		GetObjType( void ) { return STSUBPANE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00532ff0 ?GetClassName@STSubPane@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STSubPane"; }
	// FUNCTION: STBC 0x00533000 ?GetSwigTypeName@STSubPane@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STSubPane"; }
	// FUNCTION: STBC 0x00533010 ?GetPythonClassName@STSubPane@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STSubPanePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00533020 ?GetClassHandlers@STSubPane@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	GetDesiredSize( NiPoint2 *size );
	virtual void	Layout( void );
	virtual void	VirtualSlot59( void );

	// TGPane
	virtual void	InsertChild( int index, TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true );
	virtual void	RemoveChild( TGUIObject *child, bool layout = true );
	virtual void	DeleteChild( TGUIObject *child, bool layout = true );
	virtual void	SetFocus( TGUIObject *child, bool layout = true );

	// The sub-menu, the button or toggle, and the item with the name;
	// NULL, or nothing removed, when there is none.
	// FUNCTION: STBC 0x00533030 ?GetSubmenu@STSubPane@@UAEPAVSTMenu@@PAD@Z
	virtual STMenu *	GetSubmenu( char *name ) {
		return GetSubmenuW( &TGString( name ) );
	}
	virtual STMenu *	GetSubmenuW( TGString *name );
	// FUNCTION: STBC 0x005330a0 ?GetButton@STSubPane@@UAEPAVSTButton@@PAD@Z
	virtual STButton *	GetButton( char *name ) {
		return GetButtonW( &TGString( name ) );
	}
	virtual STButton *	GetButtonW( TGString *name );
	// FUNCTION: STBC 0x00533110 ?RemoveItem@STSubPane@@UAEXPAD@Z
	virtual void	RemoveItem( char *name ) {
		RemoveItemW( &TGString( name ) );
	}
	virtual void	RemoveItemW( TGString *name );

	static STSubPane *	Cast( TGObject *object );
	// A new pane of the size, its children DEFAULT_ST_INDENT_VERT apart.
	static STSubPane *	Create( float width, float height, bool expandToFillParent ) {
		STSubPane *subPane = new STSubPane( DEFAULT_ST_INDENT_VERT, expandToFillParent );
		subPane->Resize( width, height, false );
		return subPane;
	}

	// Sizes the pane to its children and, with layout, resizes the
	// menus and sub-panes it sits in up to its window.
	void			ResizeToContents( bool layout = true );
	void			SetRadioGroup( bool radioGroup );
	int				IsRadioGroup( void ) { return flags & STSPF_RADIO_GROUP; }	// the flag bit as an int: the SWIG wrapper converts it with test/setne
	float			GetVertSpacing( void ) { return vertSpacing; }
	void			SetVertSpacing( float spacing ) { vertSpacing = spacing; }	// invented name
	void			SetExpandToFillParent( bool expand ) { expandToFillParent = expand; }
	// The heights of the visible children and their spacing, with a
	// margin of twice DEFAULT_ST_INDENT_VERT.
	float			GetTotalHeightOfChildren( void );
	STStylizedWindow *	GetContainingWindow( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			KeyboardHandler( TGKeyboardEvent *event );
	void			HandleSelectOption( TGIntEvent *event );

	// Moves the focus by offset options, into the neighbouring menus past
	// either end (invented name).
	bool			MoveSelection( int offset );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// Stacks the visible children top to bottom, vertSpacing apart
	// (invented name).
	void			StackChildren( void );
	// The number of menus, other than top level menus, the pane sits in
	// (invented name).
	int				GetMenuDepth( void );
	// The indent of the nearest menu, other than a top level menu, the
	// pane sits in; 0 outside menus (invented name).
	float			GetMenuIndent( void );
	// Focuses the visible option numbered index, from 0, and activates
	// it (invented name).
	bool			SelectOption( int index );
	// Moves the focus offset menus on from the pane's menu, to the last
	// option of a menu before it or the first of a menu after it
	// (invented name).
	bool			MoveToNeighbourMenu( int offset );
	// Activates the focused option (invented name).
	bool			ActivateSelection( void );

	float			vertSpacing;			// 0x4C between the children
	bool			expandToFillParent;		// 0x50 as wide as the containing window allows

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STSubPane::IsTypeOf
================
*/
// FUNCTION: STBC 0x00532f90
inline bool STSubPane::IsTypeOf( int type ) {
	if ( type == STSUBPANE_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__STSUBPANE_H__ */
