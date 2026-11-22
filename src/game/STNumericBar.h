/*
===============================================================================

	STNumericBar.h

	A horizontal bar for a value between a minimum and a maximum, with a
	marker, a percentage caption and an arrow button at each end. Clicking
	or dragging the bar sets the value; the arrows step it. Each change of
	a step sends the update event, a copy of which carries the new value.

	0x8C bytes (its scalar deleting destructor frees 0x8C).

	Evidence: the constructor (0x529F00) builds a pane and eight children
	and stores the vtable 0x889D9C (82 slots, none new); the stream
	constructor and the inline functions kept in the object factory's unit
	(0x43ECF0 to 0x43EDE0) store the same vtable. GetObjType returns 0x80F1.
	The SWIG wrappers STNumericBar_* name Cast, SetValue, SetMarkerValue,
	GetMarker, GetPositionFromAbsolutePercentage, SetNormalColor,
	SetEmptyColor, SetUseButtons, SetUpdateEvent and UpdateGauge. The handler
	names are the strings "STNumericBar::HandleKeyboard", "HandleSelectOption",
	"HandleMouse" and "HandleArrow". The caption is formatted with "%3d%%" and
	the step sound is "UINumericBarClick".

===============================================================================
*/

#ifndef __STNUMERICBAR_H__
#define __STNUMERICBAR_H__

#include "TGPane.h"
#include "TGParagraph.h"
#include "NiColor.h"

class TGIEvent;
class TGFloatEvent;
class TGIcon;
class TGIntEvent;
class TGKeyboardEvent;
class TGMouseEvent;
class TGObject;

// GetObjType's value for an STNumericBar.
#define STNUMERICBAR_TYPE				0x80F1

// The event the arrow buttons send (invented name; the SWIG constant is not known).
#define ET_ST_NUMERIC_BAR_ARROW			0x80008F

// The children, in the order the bar adds them (invented names).
#define STNUMERICBAR_LABEL				0		// the TGParagraph left of the bar (invented name)
#define STNUMERICBAR_CAPTION			1		// the TGParagraph with the percentage
#define STNUMERICBAR_FIRST_ARROW		2		// STRoundedButton
#define STNUMERICBAR_SECOND_ARROW		3		// STRoundedButton
#define STNUMERICBAR_BAR_END			4		// TGIcon
#define STNUMERICBAR_MARKER				5		// TGIcon
#define STNUMERICBAR_BAR_FRAME			6		// TGFrame behind the bar (invented name)
#define STNUMERICBAR_BAR_PANE			7		// TGPane holding the three bar icons

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043ede0 ??_GSTNumericBar@@UAEPAXI@Z
class STNumericBar : public TGPane {
public:
	// FUNCTION: STBC 0x0043ecf0 ??0STNumericBar@@QAE@J@Z @game/UtopiaModule
					STNumericBar( long id ) : TGPane( id ) {}	// the loader's: the stream fills the members
					STNumericBar( float width, float height );
	virtual			~STNumericBar( void );

	// TGObject
	// FUNCTION: STBC 0x0043ed30 ?GetObjType@STNumericBar@@UAEHXZ
	virtual int		GetObjType( void ) { return STNUMERICBAR_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0043eda0 ?GetClassName@STNumericBar@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STNumericBar"; }
	// FUNCTION: STBC 0x0043edb0 ?GetSwigTypeName@STNumericBar@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STNumericBar"; }
	// FUNCTION: STBC 0x0043edc0 ?GetPythonClassName@STNumericBar@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STNumericBarPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0043edd0 ?GetClassHandlers@STNumericBar@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	GotTrueFocusPath( bool layout = true );
	virtual void	LostTrueFocusPath( bool layout = true );
	virtual void	SetDisabled( bool layout = true );

	static STNumericBar *	Cast( TGObject *object );
	// The bar's text and its percentage: its first two children.
	TGParagraph *	GetText( void ) { return TGParagraph::Cast( GetNthChild( 0 ) ); }
	TGParagraph *	GetPercentageText( void ) { return TGParagraph::Cast( GetNthChild( 1 ) ); }

	void			SetValue( float value );
	// The value's bounds (the SWIG inline wrappers STNumericBar_SetRange and
	// STNumericBar_GetRange read and write 0x4C and 0x50).
	void			SetRange( float min, float max ) { minValue = min; maxValue = max; }
	void			GetRange( float *min, float *max ) { *min = minValue; *max = maxValue; }
	void			SetMarkerValue( float markerValue );
	TGIcon *		GetMarker( void );
	float			GetPositionFromAbsolutePercentage( float percentage );
	void			SetNormalColor( NiColorA *color );
	void			SetEmptyColor( NiColorA *color );
	void			SetUseButtons( bool use );
	void			SetUpdateEvent( TGFloatEvent *event );
	void			UpdateGauge( bool layout );
	void			RefreshIconGroups( void );		// invented name

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleKeyboard( TGKeyboardEvent *event );
	void			HandleSelectOption( TGIntEvent *event );
	void			HandleMouse( TGMouseEvent *event );
	void			HandleArrow( TGIEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	void	SetUseAlternateColor( bool useAlternateColor_ ) { useAlternateColor = useAlternateColor_; }
	void	SetUseMarker( bool useMarker_ ) { useMarker = useMarker_; }
	float	GetKeyInterval( void ) { return keyInterval; }
	float	GetValue( void ) { return value; }
	void	SetAlternateValue( float alternateValue_ ) { alternateValue = alternateValue_; }
	void	SetKeyInterval( float keyInterval_ ) { keyInterval = keyInterval_; }

protected:
	// Moves the value one step up or down and sends the update event (invented name).
	void			Step( bool up );

	float			minValue;				// 0x4C
	float			maxValue;				// 0x50
	float			value;					// 0x54
	bool			useMarker;		// 0x58 the caption shows the marker's value (invented name)
	bool			useAlternateColor;				// 0x59 (invented name)
	bool			useButtons;				// 0x5A the arrows are shown
	float			markerValue;			// 0x5C
	float			alternateValue;				// 0x60 the value the marker is drawn from
	float			keyInterval;				// 0x64 the value a click on an arrow changes
	NiColorA		normalColor;			// 0x68 the filled part
	NiColorA		emptyColor;				// 0x78 the rest
	TGFloatEvent *	updateEvent;			// 0x88 a copy held; an ID between ReadFromStream and PostFixUp

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	static STNumericBar *	Create( float width, float height ) { return new STNumericBar( width, height ); }	// the SWIG wrapper STNumericBar_Create expands it
};

/*
================
STNumericBar::IsTypeOf
================
*/
// FUNCTION: STBC 0x0043ed40
inline bool STNumericBar::IsTypeOf( int type ) {
	if ( type == STNUMERICBAR_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__STNUMERICBAR_H__ */
