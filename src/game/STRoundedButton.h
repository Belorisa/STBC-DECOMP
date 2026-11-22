/*
===============================================================================

	STRoundedButton.h

	A button drawn by the scripts: its first child is the name paragraph and
	its second a pane built by UIHelpers.CreateRoundedButton (or
	CreateRightRoundedButton) at the button's size, whose icons take the
	button's colour for its state. The name is kept centred over the pane.
	Unlike STButton it takes no event from its parents' depth and plays no
	click.

	0xE0 bytes (its scalar deleting destructor frees 0xE0).

	Evidence: the constructors (0x52DC30 with a C string, 0x52DE80 with a
	TGString) build a TGButtonBase at the given size, clear the eight colours
	and store the vtable 0x8906C0 (91 slots; 90 is new). Slot 90 (0x52DF90)
	takes the first depth's STButton colours, builds the paragraph and sizes
	the button. GetObjType returns 0x80ED. The SWIG wrappers
	STRoundedButton_* name Cast, SetName, GetName, SetColor and
	SetColorBasedOnFlags. The pane is created by the strings "UIHelpers",
	"CreateRoundedButton" and "CreateRightRoundedButton" (0x52E320).

===============================================================================
*/

#ifndef __STROUNDEDBUTTON_H__
#define __STROUNDEDBUTTON_H__

#include "TGButtonBase.h"
#include "NiColor.h"

class TGEvent;
class TGObject;
class TGString;

// GetObjType's value for an STRoundedButton.
#define STROUNDEDBUTTON_TYPE		0x80ED

// The children, in the order the button holds them (invented names).
#define STROUNDEDBUTTON_TEXT		0
#define STROUNDEDBUTTON_PANE		1

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0052de40 ??_GSTRoundedButton@@UAEPAXI@Z
// FUNCTION: STBC 0x0052de70 ??1STRoundedButton@@UAE@XZ
class STRoundedButton : public TGButtonBase {
	friend class STNumericBar;		// builds its arrows with the first depth's colours
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005577e0 ??0STRoundedButton@@QAE@J@Z @game/InterfaceModule
					STRoundedButton( long id ) : TGButtonBase( id ) {}
					STRoundedButton( char *name, TGEvent *event, float width, float height, bool rightRounded );
					STRoundedButton( TGString *name, TGEvent *event, float width, float height, bool rightRounded );
	static STRoundedButton *	Create( char *name, TGEvent *event, float width, float height, bool rightRounded ) { return new STRoundedButton( name, event, width, height, rightRounded ); }	// the SWIG wrapper STRoundedButton_Create expands it
	static STRoundedButton *	CreateW( TGString *name, TGEvent *event, float width, float height, bool rightRounded ) { return new STRoundedButton( name, event, width, height, rightRounded ); }	// the SWIG wrapper STRoundedButton_CreateW expands it

	// TGObject
	// FUNCTION: STBC 0x0052dd70 ?GetObjType@STRoundedButton@@UAEHXZ
	virtual int		GetObjType( void ) { return STROUNDEDBUTTON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0052dde0 ?GetClassName@STRoundedButton@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STRoundedButton"; }
	// FUNCTION: STBC 0x0052ddf0 ?GetSwigTypeName@STRoundedButton@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STRoundedButton"; }
	// FUNCTION: STBC 0x0052de00 ?GetPythonClassName@STRoundedButton@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STRoundedButtonPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0052de10 ?GetClassHandlers@STRoundedButton@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	GotTrueFocus( bool layout = true );
	virtual void	LostTrueFocus( bool layout = true );
	virtual void	GetDesiredSize( NiPoint2 *size );
	virtual void	SetEnabled( bool layout = true );
	virtual void	SetDisabled( bool layout = true );
	virtual void	SetSelected( bool layout = true );
	virtual void	SetNotSelected( bool layout = true );
	virtual void	SetHighlighted( bool layout = true );
	virtual void	SetNotHighlighted( bool layout = true );
	virtual void	Layout( void );

	// TGPane: a button keeps no focus among its children.
	// FUNCTION: STBC 0x0052de30 ?SetFocus@STRoundedButton@@UAEXPAVTGUIObject@@_N@Z
	virtual void	SetFocus( TGUIObject *child, bool layout = true ) {}
	// FUNCTION: STBC 0x0052de20 ?GetFocus@STRoundedButton@@UAEPAVTGUIObject@@XZ
	virtual TGUIObject *	GetFocus( void ) { return NULL; }

	// TGButtonBase
	virtual void	SendActivationEvent( void );

	// Builds the children; the constructors' common part (invented name).
	virtual void	Init( TGString *name, TGEvent *event, float width, float height );

	static STRoundedButton *	Cast( TGObject *object );

	void			SetName( TGString *name, bool layout = true );
	void			GetName( TGString *name );
	void			SetColorBasedOnFlags( void );
	void			SetColor( NiColorA *color );
	void			SetNormalColor( NiColorA *color ) { normalColor = *color; }
	void			SetSelectedColor( NiColorA *color ) { selectedColor = *color; }
	void			SetHighlightedColor( NiColorA *color ) { highlightedColor = *color; }
	void			SetDisabledColor( NiColorA *color ) { disabledColor = *color; }
	void			SetTextColor( NiColorA *color ) { textColor = *color; }
	void			SetHighlightedTextColor( NiColorA *color ) { highlightedTextColor = *color; }
	void			SetSelectedTextColor( NiColorA *color ) { selectedTextColor = *color; }
	void			SetDisabledTextColor( NiColorA *color ) { disabledTextColor = *color; }

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// The number of sub-panes the button sits in (invented name; nothing calls it).
	int				GetUIHeight( void );
	// Centres the name over the pane (invented name).
	void			CenterText( bool layout );

	NiColorA		normalColor;			// 0x58 the pane
	NiColorA		selectedColor;			// 0x68
	NiColorA		highlightedColor;		// 0x78
	NiColorA		disabledColor;			// 0x88
	NiColorA		textColor;				// 0x98 the name
	NiColorA		highlightedTextColor;	// 0xA8
	NiColorA		selectedTextColor;		// 0xB8
	NiColorA		disabledTextColor;		// 0xC8
	int				unknownD8;				// 0xD8 cleared, saved and loaded
	bool			rightRounded;			// 0xDC the pane is built by CreateRightRoundedButton (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STRoundedButton::IsTypeOf
================
*/
// FUNCTION: STBC 0x0052dd80
inline bool STRoundedButton::IsTypeOf( int type ) {
	if ( type == STROUNDEDBUTTON_TYPE ) {
		return true;
	}
	return TGButtonBase::IsTypeOf( type );
}

#endif /* !__STROUNDEDBUTTON_H__ */
