/*
===============================================================================

	TGCreditAction.h

	A timed action that shows a line of the credits: Play puts a paragraph
	of the text into the pane, and the movie module's per-frame Update
	fades the paragraph in over fadeIn seconds and out over fadeOut
	seconds after the duration. The action runs on real time.

	0x6C bytes, allocated from the pools.

	Evidence: the class name strings "TGCreditAction" and
	"TGCreditActionPtr" (0x95A9F0) and the SWIG wrappers TGCreditAction_*.
	The constructor (0x6B1BA0) calls TGTimedAction's, constructs the string
	at 0x28 and the color at 0x5C, stores the vtable 0x895710 (TGAction's
	24 slots; 4, 5 and 16 overridden) and adds the action to the static
	list at 0x995C80; its ten arguments are those of the scripts'
	TGCreditAction_Create (MainMenu/mainmenu.py: the string, the pane, the
	left and top, the duration, the fades, the font size and the two
	JUSTIFY_ constants). GetObjType (0x6B1D50) returns 0x902; the load's
	creator (0x6AE250) calls the ID constructor 0x6B1E00. Play (0x6B20B0)
	makes the paragraph in the "Crillee" font and places it by the
	justification; Update (0x6B2210) is called each frame by the movie
	module with the application's clock. The SWIG wrapper SetColor stores
	the color at 0x5C, SetDefaultColor the static one at 0x995C40.

===============================================================================
*/

#ifndef __TGCREDITACTION_H__
#define __TGCREDITACTION_H__

#include "TGTimedAction.h"
#include "TGString.h"
#include "NiColor.h"
#include "NiTList.h"

class TGPane;

// GetObjType's value for a TGCreditAction.
#define TGCREDITACTION_TYPE		0x902

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006b1dd0 ??_GTGCreditAction@@UAEPAXI@Z @game/TGCreditAction
class TGCreditAction : public TGTimedAction {
public:
	// Where the paragraph goes against its left and top (the SWIG
	// constants TGCreditAction_JUSTIFY_*). Left and top place the
	// paragraph's corner there, right and bottom its far edge; center
	// centers it in the pane and ignores the position.
	enum justify_t {
		JUSTIFY_LEFT,
		JUSTIFY_RIGHT,
		JUSTIFY_TOP,
		JUSTIFY_BOTTOM,
		JUSTIFY_CENTER
	};

					TGCreditAction( long id = 0 );
					TGCreditAction( TGString *string, TGPane *pane, float left, float top, float duration,
									float fadeIn, float fadeOut, int fontSize, int horizontalJustify,
									int verticalJustify );
	virtual			~TGCreditAction( void );

	static TGCreditAction *	Create( TGString *string, TGPane *pane, float left, float top, float duration,
									float fadeIn, float fadeOut, int fontSize, int horizontalJustify,
									int verticalJustify ) {
						return new TGCreditAction( string, pane, left, top, duration, fadeIn, fadeOut,
							fontSize, horizontalJustify, verticalJustify );
					}
	// FUNCTION: STBC 0x005e80c0 ?Create@TGCreditAction@@SAPAV1@PADPAVTGPane@@MMMMMHHH@Z @game/AppWrap
	static TGCreditAction *	Create( char *string, TGPane *pane, float left, float top, float duration,
									float fadeIn, float fadeOut, int fontSize, int horizontalJustify,
									int verticalJustify ) {
						TGString text( string );
						return new TGCreditAction( &text, pane, left, top, duration, fadeIn, fadeOut,
							fontSize, horizontalJustify, verticalJustify );
					}

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGAction
	virtual void	Play( void );

	void			SetColor( float r, float g, float b, float a = 1.0f ) { color = NiColorA( r, g, b, a ); }
	// The color the credits made after this take.
	static void		SetDefaultColor( float r, float g, float b, float a = 1.0f ) { defaultColor = NiColorA( r, g, b, a ); }

	// Fades the playing credits for the clock (seconds), and forgets those
	// whose paragraph is gone (name invented: the movie module's per-frame
	// slot is its one caller).
	static void		UpdateFades( float time );

protected:
	TGString		text;				// 0x28
	int				fontSize;			// 0x38
	long			paragraphID;		// 0x3C the paragraph Play made; 0 before it
	long			paneID;				// 0x40 the pane the paragraph goes in; 0 for none
	float			left;				// 0x44 in the pane's coordinates
	float			top;				// 0x48
	float			fadeIn;				// 0x4C seconds
	float			fadeOut;			// 0x50 seconds
	int				horizontalJustify;	// 0x54 justify_t
	int				verticalJustify;	// 0x58 justify_t
	NiColorA		color;				// 0x5C the alpha is the fade's

	static NiColorA	defaultColor;		// the color new credits take; white
	static NiTList<TGCreditAction *>	credits;	// every credit action (invented name)
};

#endif /* !__TGCREDITACTION_H__ */
