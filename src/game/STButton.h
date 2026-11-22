/*
===============================================================================

	STButton.h

	The interface's menu button: a row of six children, laid out left to
	right. A pane holding the name paragraph sits over the body icon, with
	a rounded end cap at each end and a marker between the left cap and
	the body that shows where the focus is. A checkbox, shown in place of
	the left cap, marks a choosable button that is chosen. The button
	takes its colours from the depth of the sub-panes (menus) it sits in,
	and sends ET_ST_BUTTON_CLICKED up its parents when activated.

	0x114 bytes (its scalar deleting destructor frees 0x114).

	Evidence: the constructors (0x518BE0 with a C string, 0x518E70 with a
	TGString, called by STButton_Create and STButton_CreateW) build the
	button base at the given size, construct the eleven colours, store
	the vtable 0x88F810 (91 slots; 90 is new) and call slot 90, which
	builds the children. GetObjType returns 0x80EC. The SWIG wrappers
	STButton_* name the members: Set*Color copy the colours (0x58 to
	0x104), Is/SetAutoChoose, Is/SetChoosable and Is/SetChosen test bits
	28, 27 and 29 of the flags, Is/SetUseEndCapSpace bit 26 (set when
	unused), IsUsingEndCaps/SetUseEndCaps 0x10D, SetUseUIHeight 0x10C and
	SetJustification 0x110 (the SWIG constants STButton_LEFT, _CENTER and
	_RIGHT). The colour tables are the SWIG variables g_kSTButtonColors,
	g_kSTButtonMarker* and g_kSTMenuText*.

===============================================================================
*/

#ifndef __STBUTTON_H__
#define __STBUTTON_H__

#include "TGButtonBase.h"
#include "TGParagraph.h"
#include "NiColor.h"

class STStylizedWindow;
class TGObject;
class TGString;

// GetObjType's value for an STButton.
#define STBUTTON_TYPE				0x80EC

// The event an activated button sends up its parents (the SWIG constant
// ET_ST_BUTTON_CLICKED).
#define ET_ST_BUTTON_CLICKED		0x800095

// Where the name sits (the SWIG constants STButton_LEFT, _CENTER and _RIGHT).
#define STBUTTON_LEFT				0
#define STBUTTON_CENTER				1
#define STBUTTON_RIGHT				2

// The button's own state flags, above the TGUIO_* flags (invented names;
// the SWIG accessors name the states they test).
#define STBF_STRETCH_TEXT			0x02000000	// wrap the name to the parent's width
#define STBF_NO_END_CAP_SPACE		0x04000000	// IsUseEndCapSpace is false
#define STBF_CHOOSABLE				0x08000000	// IsChoosable
#define STBF_AUTO_CHOOSE			0x10000000	// IsAutoChoose: activation toggles the chosen state
#define STBF_CHOSEN					0x20000000	// IsChosen
#define STBF_SIZE_TO_TEXT			0x40000000	// the button is as big as its name
#define STBF_FILL_PARENT_WIDTH		0x80000000	// the button is as wide as its parent

// The four button colours of each depth in g_kSTButtonColors.
#define STBUTTON_NUM_COLORS			4
// The depths that have their own colours; deeper buttons use the last.
#define STBUTTON_NUM_UI_HEIGHTS		4

// The children, in the order the button adds them (invented names).
#define STBUTTON_TEXT_PANE			0
#define STBUTTON_LEFT_END_CAP		1
#define STBUTTON_CHECKBOX			2
#define STBUTTON_MARKER				3
#define STBUTTON_BODY				4
#define STBUTTON_RIGHT_END_CAP		5

// The icon numbers of the marker and the end caps in the LCARS icon group.
#define STBUTTON_ICON_MARKER		0xBB9
#define STBUTTON_ICON_LEFT_END_CAP	0xBBA
#define STBUTTON_ICON_RIGHT_END_CAP	0xBBB

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00518e30 ??_GSTButton@@UAEPAXI@Z
// FUNCTION: STBC 0x00518e60 ??1STButton@@UAE@XZ
class STButton : public TGButtonBase {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00557690 ??0STButton@@QAE@J@Z @game/InterfaceModule
					STButton( long id ) : TGButtonBase( id ) {}
					STButton( char *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum );
					STButton( TGString *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum );
	// BUG: Create and CreateW drop their iconGroupName and bodyIconNum: the button always gets
	// no icon group and body icon 200 (the SWIG defaults), whatever the script passes.
	static STButton *	Create( char *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum ) { return new STButton( name, event, buttonFlags, width, height, NULL, 200 ); }	// the SWIG wrapper STButton_Create expands it
	static STButton *	CreateW( TGString *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum ) { return new STButton( name, event, buttonFlags, width, height, NULL, 200 ); }	// the SWIG wrapper STButton_CreateW expands it

	// TGObject
	// FUNCTION: STBC 0x00518d60 ?GetObjType@STButton@@UAEHXZ
	virtual int		GetObjType( void ) { return STBUTTON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00518dd0 ?GetClassName@STButton@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STButton"; }
	// FUNCTION: STBC 0x00518de0 ?GetSwigTypeName@STButton@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STButton"; }
	// FUNCTION: STBC 0x00518df0 ?GetPythonClassName@STButton@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STButtonPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00518e00 ?GetClassHandlers@STButton@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	GotTrueFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );
	virtual void	LostTrueFocus( bool layout = true );
	virtual void	LostTrueFocusPath( bool layout = true );
	virtual void	GetDesiredSize( NiPoint2 *size );
	virtual void	SetEnabled( bool layout = true );
	virtual void	SetDisabled( bool layout = true );
	virtual void	SetSelected( bool layout = true );
	virtual void	SetNotSelected( bool layout = true );
	virtual void	SetHighlighted( bool layout = true );
	virtual void	SetNotHighlighted( bool layout = true );
	virtual void	Layout( void );

	// TGPane: a button keeps no focus among its children.
	// FUNCTION: STBC 0x00518e20 ?SetFocus@STButton@@UAEXPAVTGUIObject@@_N@Z
	virtual void	SetFocus( TGUIObject *child, bool layout = true ) {}
	// FUNCTION: STBC 0x00518e10 ?GetFocus@STButton@@UAEPAVTGUIObject@@XZ
	virtual TGUIObject *	GetFocus( void ) { return NULL; }

	// TGButtonBase
	virtual void	SendActivationEvent( void );

	// Builds the children; the constructors' common part (invented name).
	virtual void	Init( TGString *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum );

	static STButton *	Cast( TGObject *object );
	// The label: the first child of the button's first child.
	TGParagraph *	GetText( void ) { return TGParagraph::Cast( TGPane::Cast( TGPane::GetNthChild( 0 ) )->GetFirstChild() ); }

	void			SetName( TGString *name, bool layout = true );
	void			GetName( TGString *name );

	void			SetNormalColor( NiColorA *color ) { normalColor = *color; }
	void			SetSelectedColor( NiColorA *color ) { selectedColor = *color; }
	void			SetHighlightedColor( NiColorA *color ) { highlightedColor = *color; }
	void			SetDisabledColor( NiColorA *color ) { disabledColor = *color; }
	void			SetTextColor( NiColorA *color ) { textColor = *color; }
	void			SetHighlightedTextColor( NiColorA *color ) { highlightedTextColor = *color; }
	void			SetSelectedTextColor( NiColorA *color ) { selectedTextColor = *color; }
	void			SetDisabledTextColor( NiColorA *color ) { disabledTextColor = *color; }
	void			SetMarkerNormalColor( NiColorA *color ) { markerNormalColor = *color; }
	void			SetMarkerHighlightedColor( NiColorA *color ) { markerHighlightedColor = *color; }
	void			SetMarkerSelectedColor( NiColorA *color ) { markerSelectedColor = *color; }

	void			SetJustification( int justification );
	STStylizedWindow *	GetContainingWindow( void );

	void			SetAutoChoose( bool autoChoose ) {
		if ( autoChoose ) {
			flags |= STBF_AUTO_CHOOSE;
		} else {
			flags &= ~STBF_AUTO_CHOOSE;
		}
	}
	bool			IsAutoChoose( void ) { return ( flags & STBF_AUTO_CHOOSE ) != 0; }
	void			SetChoosable( bool choosable, bool layout = true );
	bool			IsChoosable( void ) { return ( flags & STBF_CHOOSABLE ) != 0; }
	void			SetChosen( bool chosen, bool layout = true );
	bool			IsChosen( void ) { return ( flags & STBF_CHOSEN ) != 0; }

	void			SetColorBasedOnFlags( void );
	void			SetColor( NiColorA *color );

	bool			IsUsingEndCaps( void ) { return usingEndCaps; }
	void			SetUseEndCaps( bool use, bool layout = true ) {
		usingEndCaps = use;
		if ( layout ) {
			Layout();
		}
	}
	void			SetUseEndCapSpace( bool use, bool layout = true ) {
		if ( !use ) {
			flags |= STBF_NO_END_CAP_SPACE;
		} else {
			flags &= ~STBF_NO_END_CAP_SPACE;
		}
		if ( layout ) {
			Layout();
		}
	}
	bool			IsUseEndCapSpace( void ) { return ( flags & STBF_NO_END_CAP_SPACE ) == 0; }
	void			SetUseUIHeight( bool use ) {
		useUIHeight = use;
		Layout();
	}

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// The number of sub-panes the button sits in, less one for a chosen
	// button that cannot be unchosen, at most STBUTTON_NUM_UI_HEIGHTS - 1
	// (invented name).
	int				GetUIHeight( void );
	// Takes the button colours of the button's depth (invented name).
	void			UpdateUIHeightColors( void );
	// Places the name in the text pane by the justification (invented name).
	void			JustifyText( bool layout );

	NiColorA		normalColor;			// 0x58 the end caps, marker and body
	NiColorA		selectedColor;			// 0x68
	NiColorA		highlightedColor;		// 0x78
	NiColorA		disabledColor;			// 0x88
	NiColorA		textColor;				// 0x98 the name
	NiColorA		highlightedTextColor;	// 0xA8
	NiColorA		selectedTextColor;		// 0xB8
	NiColorA		disabledTextColor;		// 0xC8
	NiColorA		markerNormalColor;		// 0xD8 the marker while the button is in the true focus path
	NiColorA		markerHighlightedColor;	// 0xE8 saved, never shown
	NiColorA		markerSelectedColor;	// 0xF8 saved, never shown
	int				uiHeight;				// 0x108 the depth the colours were taken for
	bool			useUIHeight;			// 0x10C take the colours of the depth at each layout
	bool			usingEndCaps;			// 0x10D
	int				justification;			// 0x110 STBUTTON_LEFT, STBUTTON_CENTER or STBUTTON_RIGHT

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STButton::IsTypeOf
================
*/
// FUNCTION: STBC 0x00518d70
inline bool STButton::IsTypeOf( int type ) {
	if ( type == STBUTTON_TYPE ) {
		return true;
	}
	return TGButtonBase::IsTypeOf( type );
}

#endif /* !__STBUTTON_H__ */
