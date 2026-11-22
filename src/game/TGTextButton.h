/*
===============================================================================

	TGTextButton.h

	A button that shows a paragraph of text inside a frame drawn from nine
	icons of an icon group: four corners, four edges and the middle. The
	group holds three sets of the nine icons, one after the other: for the
	enabled button, the enabled and selected button and the disabled one.
	The edges and the middle are stretched to fit, wrapped (their texture
	repeated across a stretched poly) or tiled with more polys.

	0xA0 bytes (its scalar deleting destructor frees 0xA0).

	Evidence: the constructors (0x73AB70 with a C string, 0x73AD00 with a
	TGString, called by new_TGTextButton and TGTextButton_CreateW with the
	text, the group name, the first icon number, a scale, the frame mode
	and two margins) build the button base at no size, the array of
	frame polys (0x64) and the inner rectangle (0x88), store the vtable
	0x89790C (96 slots; 90 to 95 are new), centre the text both ways
	(0x80, 0x84), copy the group name (0x58) and add a read-only
	TGParagraph at the margins. The SWIG wrappers name slots 90 to 95
	(SetText to AlignTextVertical), TGTextButton_GetInnerRect copies 0x88,
	and the SWIG constants TGTextButton_FRAME_* and TGTextButton_ALIGN_*
	give the modes. GetObjType returns 0x20B. The type's inline members
	are emitted with the UI module's class factories (0x726A90 to
	0x726B40).

===============================================================================
*/

#ifndef __TGTEXTBUTTON_H__
#define __TGTEXTBUTTON_H__

#include "TGButtonBase.h"
#include "TGFrame.h"
#include "TGRect.h"
#include "NiTArray.h"

class TGIconPoly;
class TGObject;
class TGParagraph;
class TGString;

// GetObjType's value for a TGTextButton.
#define TGTEXTBUTTON_TYPE		0x20B

// The frame modes FRAME_* (the SWIG constants TGTextButton_FRAME_*) are
// TGFrame's stretch types.

// Where the text sits (the SWIG constants TGTextButton_ALIGN_*).
#define ALIGN_LEFT				0
#define ALIGN_CENTER			1
#define ALIGN_RIGHT				2
#define ALIGN_TOP				0
#define ALIGN_MIDDLE			1
#define ALIGN_BOTTOM			2

// The nine icons of a frame, from the first icon of a state's set
// (invented names).
#define FRAME_TOP_LEFT			0
#define FRAME_TOP				1
#define FRAME_TOP_RIGHT			2
#define FRAME_LEFT				3
#define FRAME_MIDDLE			4
#define FRAME_RIGHT				5
#define FRAME_BOTTOM_LEFT		6
#define FRAME_BOTTOM			7
#define FRAME_BOTTOM_RIGHT		8
#define FRAME_NUM_ICONS			9

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00726b40 ??_GTGTextButton@@UAEPAXI@Z
class TGTextButton : public TGButtonBase {
public:
	// FUNCTION: STBC 0x00726a00 ??0TGTextButton@@QAE@J@Z
					TGTextButton( long id ) : TGButtonBase( id ) {}
					TGTextButton( char *text = NULL, char *iconGroupName = NULL, int firstIconNum = 0, float iconScale = 1.0f,
								int frameMode = FRAME_STRETCH_TO_FIT, float horizontalMargin = 0.01f, float verticalMargin = 0.005f );
					TGTextButton( TGString *text, char *iconGroupName = NULL, int firstIconNum = 0, float iconScale = 1.0f,
								int frameMode = FRAME_STRETCH_TO_FIT, float horizontalMargin = 0.01f, float verticalMargin = 0.005f );
	virtual			~TGTextButton( void );

	// TGObject
	// FUNCTION: STBC 0x00726a90 ?GetObjType@TGTextButton@@UAEHXZ
	virtual int		GetObjType( void ) { return TGTEXTBUTTON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00726b00 ?GetClassName@TGTextButton@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGTextButton"; }
	// FUNCTION: STBC 0x00726b10 ?GetSwigTypeName@TGTextButton@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGTextButton"; }
	// FUNCTION: STBC 0x00726b20 ?GetPythonClassName@TGTextButton@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGTextButtonPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00726b30 ?GetClassHandlers@TGTextButton@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	SetEnabled( bool layout = true );
	virtual void	SetDisabled( bool layout = true );
	virtual void	Layout( void );
	virtual void	VirtualSlot60( void );
	virtual void	VirtualSlot61( NiPoint2 *offset );
	virtual void	VirtualSlot62( TGRect *clipRect );

	virtual void	SetText( char *text );
	virtual void	SetTextW( TGString *text );
	virtual void	SetIcons( char *iconGroupName, int firstIconNum );
	virtual void	SetTextWrapWidth( float width );
	virtual void	AlignTextHorizontal( int align, bool layout = true );
	virtual void	AlignTextVertical( int align, bool layout = true );

	static TGTextButton *	Cast( TGObject *object );

	void			GetInnerRect( TGRect *rect ) { *rect = innerRect; }

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// The frame, laid out by the frame mode (invented names).
	void			StretchFrame( void );
	void			WrapFrame( void );
	void			TileFrame( void );
	TGIconPoly *	AddFramePoly( int iconNum, TGRect *rect, bool stretch );

	// The first of the nine icons for the button's state (invented name).
	int				GetFrameIconNum( void ) {
		int iconNum = firstIconNum;
		if ( flags & TGUIO_ENABLED ) {
			if ( flags & TGUIO_SELECTED ) {
				iconNum += FRAME_NUM_ICONS;
			}
		} else {
			iconNum += 2 * FRAME_NUM_ICONS;
		}
		return iconNum;
	}

	char *			iconGroupName;			// 0x58 owned copy, or NULL for no frame
	int				firstIconNum;			// 0x5C the first of the three sets of nine icons
	float			iconScale;				// 0x60 the frame icons' size, as a multiple of their own
	NiTArray<TGIconPoly *>	framePolys;		// 0x64 the polys drawn for the frame
	int				frameMode;				// 0x7C FRAME_*
	int				horizontalAlign;		// 0x80 ALIGN_LEFT, ALIGN_CENTER or ALIGN_RIGHT
	int				verticalAlign;			// 0x84 ALIGN_TOP, ALIGN_MIDDLE or ALIGN_BOTTOM
	TGRect			innerRect;				// 0x88 inside the frame
	float			horizontalMargin;		// 0x98 between the text and the left and right sides
	float			verticalMargin;			// 0x9C between the text and the top and bottom

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	static TGTextButton *	CreateW( TGString *text, char *iconGroupName, int firstIconNum, float iconScale, int frameMode, float horizontalMargin, float verticalMargin ) { return new TGTextButton( text, iconGroupName, firstIconNum, iconScale, frameMode, horizontalMargin, verticalMargin ); }	// the SWIG wrapper TGTextButton_CreateW expands it
	static TGTextButton *	Create( char *text, char *iconGroupName, int firstIconNum, float iconScale, int frameMode, float horizontalMargin, float verticalMargin ) { return new TGTextButton( text, iconGroupName, firstIconNum, iconScale, frameMode, horizontalMargin, verticalMargin ); }	// the SWIG wrapper TGTextButton_Create expands it
};

/*
================
TGTextButton::IsTypeOf
================
*/
// FUNCTION: STBC 0x00726aa0
inline bool TGTextButton::IsTypeOf( int type ) {
	if ( type == TGTEXTBUTTON_TYPE ) {
		return true;
	}
	return TGButtonBase::IsTypeOf( type );
}

#endif /* !__TGTEXTBUTTON_H__ */
