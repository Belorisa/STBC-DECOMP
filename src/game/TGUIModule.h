/*
===============================================================================

	TGUIModule.h

	The module of the TG interface: at startup it files the creators of
	the UI classes for the load (types 0x201 to 0x20F, and the keyboard and
	mouse events again), links each class's handler table to its parent's,
	registers the handler names and the Python method
	TGParagraph.GetCString, and points Python's output at the console; it
	then adds the classes' handlers and makes the cursor timer. As the game
	ends it empties the root window and the icon manager. The class and its
	names are invented.

	Evidence: the one instance at 0x99CF38 is built by 0x725620
	(TGModule's constructor with the priority 0x80000000, then the vtable
	0x896BF0) and destroyed at exit through TGModule's destructor. The
	vtable's slots 0, 1, 2, 4 and 5 are 0x725640, 0x725670, 0x725680,
	0x725690 and 0x7256B0; slots 3, 6 and 7 are TGModule's empty ones. The
	unit emits the load constructors and the inline members of the UI
	classes after their creators (0x725B30 to 0x726B40).

===============================================================================
*/

#ifndef __TGUIMODULE_H__
#define __TGUIMODULE_H__

#include "TGModule.h"
#include "TGIconManager.h"

class TGObject;

// FUNCTION: STBC 0x00725610 ??1TGUIModule@@QAE@XZ
class TGUIModule : public TGModule {
public:
	// FUNCTION: STBC 0x00725620 ??0TGUIModule@@QAE@XZ
					TGUIModule( void ) : TGModule( 0x80000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );		// the handlers, once every module has started
	virtual void	VirtualSlot02( void );
	virtual void	VirtualSlot04( void );
	virtual void	VirtualSlot05( void );		// the end

	// Files the creator of each UI class by its type, and links the class
	// handler tables.
	void			RegisterClasses( void );

	// The classes' handlers: their names, then the handlers.
	void			RegisterHandlerNames( void );
	void			RegisterHandlers( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGUIObject( long id );
	static TGObject *	CreateTGPane( long id );
	static TGObject *	CreateTGRootPane( long id );
	static TGObject *	CreateTGFrame( long id );
	static TGObject *	CreateTGWindow( long id );
	static TGObject *	CreateTGFrameWindow( long id );
	static TGObject *	CreateTGParagraph( long id );
	static TGObject *	CreateTGDialogWindow( long id );
	static TGObject *	CreateTGStringDialog( long id );
	static TGObject *	CreateTGConsole( long id );
	static TGObject *	CreateTGPrompt( long id );
	static TGObject *	CreateTGIcon( long id );
	static TGObject *	CreateTGButtonBase( long id );
	static TGObject *	CreateTGButton( long id );
	static TGObject *	CreateTGTextButton( long id );

	// A screen fraction moved to the nearest pixel edge across the screen's
	// width (horizontal) or height; 0 when it rounds to no pixel.
	// The pixel count goes through a float before the divide: retail loads it
	// with fild and divides with fdivrp (TGUIObject::Resize, 0x730510; the
	// wrapper, 0x5CB980), where dividing the int directly gives fidivr.
	static float	PixelAlignValue( float value, bool horizontal = true ) {
						if ( horizontal ) {
							float width = g_kIconManager.GetScreenWidth();
							int pixels = (int)( width * value + ( value < 0.0f ? -0.5f : 0.5f ) );
							if ( pixels == 0 ) {
								return 0.0f;
							}
							float rounded = (float)pixels;
							return rounded / width;
						}
						float height = g_kIconManager.GetScreenHeight();
						int pixels = (int)( height * value + ( value < 0.0f ? -0.5f : 0.5f ) );
						if ( pixels == 0 ) {
							return 0.0f;
						}
						float rounded = (float)pixels;
						return rounded / height;
					}

	// A point of screen fractions with both its coordinates aligned.
	// Each axis is PixelAlignValue's body, the pixel count through a float
	// before the divide. The bytes show it on x only (fild; fdivrp, where
	// dividing the int folds the conversion into fidivr because the width
	// sits on the x87 stack and dies there); on y the height is in memory and
	// both spellings give fild; fdiv m32 (TGUIModule::PixelAlignRect keeps
	// its width live and shows fild; fdiv st(1) either way).
	static NiPoint2	PixelAlignPoint( const NiPoint2 &point ) {
						NiPoint2 aligned;
						float width = g_kIconManager.GetScreenWidth();
						float height = g_kIconManager.GetScreenHeight();
						int pixels = (int)( point.x * width + ( point.x < 0.0f ? -0.5f : 0.5f ) );
						if ( pixels != 0 ) {
							float rounded = (float)pixels;
							aligned.x = rounded / width;
						} else {
							aligned.x = 0.0f;
						}
						pixels = (int)( point.y * height + ( point.y < 0.0f ? -0.5f : 0.5f ) );
						if ( pixels != 0 ) {
							float rounded = (float)pixels;
							aligned.y = rounded / height;
						} else {
							aligned.y = 0.0f;
						}
						return aligned;
					}

	// A rectangle of screen fractions with its four values aligned
	// (TGUIModule_PixelAlignRect). Out of line: 0x5CB6A0, the copy the
	// SWIG wrapper's unit kept, called by TGUIObject::AlignTo and TGParagraph's
	// character placement.
	static TGRect	PixelAlignRect( const TGRect &rect );
};

/*
================
TGUIModule::PixelAlignRect

The left and width are aligned across the screen's width, the top and
height across its height.
================
*/
// FUNCTION: STBC 0x005cb6a0 ?PixelAlignRect@TGUIModule@@SA?AVTGRect@@ABV2@@Z @game/AppWrap
// DRAFT: x87 schedule: retail loads both screen sizes onto the x87 stack first and stores the height from there; this build copies the height through eax and multiplies with the value on top
inline TGRect TGUIModule::PixelAlignRect( const TGRect &rect ) {
	TGRect aligned;
	float width = g_kIconManager.GetScreenWidth();
	float height = g_kIconManager.GetScreenHeight();
	int pixels = (int)( rect.left * width + ( rect.left < 0.0f ? -0.5f : 0.5f ) );
	if ( pixels != 0 ) {
		float rounded = (float)pixels;
		aligned.left = rounded / width;
	} else {
		aligned.left = 0.0f;
	}
	pixels = (int)( rect.top * height + ( rect.top < 0.0f ? -0.5f : 0.5f ) );
	if ( pixels != 0 ) {
		float rounded = (float)pixels;
		aligned.top = rounded / height;
	} else {
		aligned.top = 0.0f;
	}
	pixels = (int)( rect.width * width + ( rect.width < 0.0f ? -0.5f : 0.5f ) );
	if ( pixels != 0 ) {
		float rounded = (float)pixels;
		aligned.width = rounded / width;
	} else {
		aligned.width = 0.0f;
	}
	pixels = (int)( rect.height * height + ( rect.height < 0.0f ? -0.5f : 0.5f ) );
	if ( pixels != 0 ) {
		float rounded = (float)pixels;
		aligned.height = rounded / height;
	} else {
		aligned.height = 0.0f;
	}
	return aligned;
}

#endif /* !__TGUIMODULE_H__ */
