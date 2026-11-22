/*
===============================================================================

	TGUITheme.h

	A named set of frames for the UI: the frame (an icon group) and its
	size for plain frame windows, for buttons and for dialogs. The theme
	manager holds the themes; the "FromTheme" constructors take the
	current one's frames.

	0x20 bytes, with a vtable of its own (0x897E80, three slots).

	Evidence: the constructor (0x745C80, called by Create 0x745F30 and the
	manager's AddTheme) clears 0x04 to 0x1C and copies the theme's name;
	the setters SetThemeName, SetWindowFrameName, SetButtonFrameName and
	SetDialogFrameName (SWIG TGUITheme_*) replace the strings at 0x04,
	0x0C, 0x14 and 0x1C with heap copies. Copy (0x745E70) copies each
	frame name with its float at 0x08, 0x10 and 0x18. The floats are the
	frames' sizes: TGFrameWindow::CreateFromTheme (0x7452D0) passes 0x0C
	and 0x08 as the frame name and size, TGDialogWindow::CreateFromTheme
	(0x739280) passes 0x1C and 0x18 as the dialog frame and 0x14 and 0x10
	as the buttons', which become each TGTextButton's icon group and icon
	scale. The two other slots write and read the names and sizes
	(0x745F90, 0x7460C0).

===============================================================================
*/

#ifndef __TGUITHEME_H__
#define __TGUITHEME_H__

class TGStream;

// FUNCTION: STBC 0x00745cb0 ??_GTGUITheme@@UAEPAXI@Z
class TGUITheme {
public:
					TGUITheme( void ) {}		// for ReadFromStream, which sets every member
					TGUITheme( char *name );
	virtual			~TGUITheme( void );

	virtual bool	WriteToStream( TGStream *stream );		// invented name, as TGObject's
	virtual bool	ReadFromStream( TGStream *stream );		// invented name, as TGObject's

	static TGUITheme *	Create( char *name );

	void			SetThemeName( char *name );
	void			Copy( TGUITheme *other );
	void			SetWindowFrameName( char *name );
	void			SetButtonFrameName( char *name );
	void			SetDialogFrameName( char *name );

	char *			GetThemeName( void ) { return themeName; }
	float			GetWindowFrameScale( void ) { return windowFrameScale; }
	void			SetWindowFrameScale( float windowFrameScale_ ) { windowFrameScale = windowFrameScale_; }
	char *			GetWindowFrameName( void ) { return windowFrameName; }
	float			GetButtonFrameScale( void ) { return buttonFrameScale; }
	void			SetButtonFrameScale( float buttonFrameScale_ ) { buttonFrameScale = buttonFrameScale_; }
	char *			GetButtonFrameName( void ) { return buttonFrameName; }
	float			GetDialogFrameScale( void ) { return dialogFrameScale; }
	void			SetDialogFrameScale( float dialogFrameScale_ ) { dialogFrameScale = dialogFrameScale_; }
	char *			GetDialogFrameName( void ) { return dialogFrameName; }

	char *			themeName;				// 0x04 a heap copy, or NULL
	float			windowFrameScale;		// 0x08 invented name
	char *			windowFrameName;		// 0x0C a heap copy, or NULL
	float			buttonFrameScale;		// 0x10 invented name; the buttons' icon scale
	char *			buttonFrameName;		// 0x14 a heap copy, or NULL; the buttons' icon group
	float			dialogFrameScale;		// 0x18 invented name
	char *			dialogFrameName;		// 0x1C a heap copy, or NULL
};

#endif /* !__TGUITHEME_H__ */
