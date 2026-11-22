/*
===============================================================================

	SubtitleWindow.h

	The main window that shows the subtitles: each line is a paragraph
	stacked in a pane over a background icon, placed and sized for the
	current view (SetPositionForMode). A line is known by its object ID:
	AddLine and AddParagraph return it, RemoveParagraph takes it.

	Evidence: the class strings "SubtitleWindow", "_p_SubtitleWindow" and
	"SubtitleWindowPtr", and the SWIG wrappers SubtitleWindow_*. The
	constructor (0x507900) calls MainWindow's with MWT_SUBTITLE, builds the
	paragraph array at 0x68 (vtable 0x88E9F0), stores the vtable 0x88E8A0
	(84 slots; SetVisible and Layout overridden), makes the pane at 0x80
	and the icon at 0x84 and places itself for SM_BRIDGE and SM_NONFELIX.
	The layout statics are the script's SUBTITLE_* values, read from the
	LCARS module of the current resolution. SubtitleAction's Play, Skip
	and Completed add and remove its lines.

===============================================================================
*/

#ifndef __SUBTITLEWINDOW_H__
#define __SUBTITLEWINDOW_H__

#include "MainWindow.h"
#include "NiTArray.h"

class TGEventHandlerTable;
class TGObject;
class TGIcon;
class TGStream;
class TGParagraph;
class TGString;

// GetObjType's value for a SubtitleWindow (the type its Cast checks).
#define SUBTITLEWINDOW_TYPE		0x8115

// The subtitle positions (the SWIG constants SubtitleWindow.SM_*).
#define SM_BRIDGE				0
#define SM_TACTICAL				1
#define SM_FELIX				2
#define SM_NONFELIX				3
#define SM_MAP					4
#define SM_CINEMATIC			5
#define SM_END_CINEMATIC		6
#define SM_SPECIAL_FELIX		7

// The icon of the LCARS group behind the lines (invented name).
#define BACKGROUND_ICON			120

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00507b30 ??_GSubtitleWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x00507b60 ??1SubtitleWindow@@UAE@XZ
class SubtitleWindow : public MainWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558690 ??0SubtitleWindow@@QAE@J@Z @game/InterfaceModule
					SubtitleWindow( long id ) : MainWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	void	SetOn( void ) { on = true; }
	bool	IsOn( void ) { return on; }
	static SubtitleWindow *	Cast( TGObject * );
	void	SetPositionForMode( int, bool );
	void	SetOff( void );

	// Declared for the SWIG wrappers that call them.
public:
					SubtitleWindow( void );
	static SubtitleWindow *	Create( void ) { return new SubtitleWindow; }	// the SWIG wrapper SubtitleWindow_Create expands it

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// TGUIObject
	virtual void	SetVisible( bool layout = true );
	virtual void	Layout( void );

	// The interface module's handlers of ET_UI_RESIZE and ET_UI_REPOSITION
	// call them (invented names, as the displays' SWIG names).
	void			ResizeUI( void );
	void			RepositionUI( void );

	// Adds a line of the text and returns its ID; 0 while the window is off
	// (invented name).
	long			AddLine( TGString *text );
	// Adds the paragraph under the other lines and returns its ID; 0 while
	// the window is off (invented name).
	long			AddParagraph( TGParagraph *paragraph );
	// Takes out and deletes the line with the ID, moving the lines under it
	// up (invented name).
	void			RemoveParagraph( long id );

	// The width the subtitles wrap to (invented name; CharacterAction::Play
	// reads it from the subtitle window).
	float			GetSubtitleWidth( void ) { return width; }
	// The bridge window's subtitle width, and the margin the paragraphs
	// leave in a width (invented names; CharacterAction::Play reads them).
	static float	GetBridgeSubtitleWidth( void ) { return bridgeSubtitleWidth; }
	static float	GetWidthMinusTextWidth( void ) { return widthMinusTextWidth; }

protected:
	// Moves the lines first to last down by dy (invented name).
	void			MoveLines( float dy, unsigned int first, unsigned int last );
	// Reads the layout statics from the LCARS module (invented name).
	void			LoadLayout( void );

	bool			on;						// 0x50 (SubtitleWindow_IsOn, SetOn)
	bool			felix;					// 0x51 placed for Felix's view
	bool			cinematic;				// 0x52 placed for a cinematic; other modes wait for SM_END_CINEMATIC
	float			left;					// 0x54
	float			centerY;				// 0x58 the lines are centered on it
	float			maxHeight;				// 0x5C
	float			width;					// 0x60
	int				mode;					// 0x64 SM_*
	NiTArray<TGParagraph *>	lines;			// 0x68 top to bottom
	TGPane *		linePane;				// 0x80 holds the lines
	TGIcon *		background;				// 0x84 behind the lines

	// The script's SUBTITLE_* values; NI_INFINITY until LoadLayout reads them.
	static float	bridgeXPos;
	static float	tacticalXPos;
	static float	felixXPos;
	static float	mapXPos;
	static float	cinematicXPos;
	static float	bridgeCenterYPos;
	static float	tacticalCenterYPos;
	static float	specialFelixCenterYPos;
	static float	felixCenterYPos;
	static float	mapCenterYPos;
	static float	cinematicCenterYPos;
	static float	bridgeSubtitleWidth;
	static float	tacticalSubtitleWidth;
	static float	felixSubtitleWidth;
	static float	mapSubtitleWidth;
	static float	cinematicSubtitleWidth;
	static float	widthMinusTextWidth;	// the margin the paragraphs leave in the width
	static float	maxSubtitleHeight;		// SUBTITLE_MAX_HEIGHT
	static float	felixMaxHeight;

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__SUBTITLEWINDOW_H__ */
