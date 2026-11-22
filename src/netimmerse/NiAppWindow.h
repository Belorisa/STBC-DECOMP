/*
===============================================================================

	NiAppWindow.h

	The application framework's main window: its caption, size and style,
	and the optional status bar with its panes.

	Evidence: NiApplication's constructor (0x7B7180) allocates it (0x30
	bytes) and calls the constructor (0x7BE270) with the caption, width,
	height, status pane count and the window style 0xCA0000; the
	constructor copies the caption, or "NetImmerse Application" (0x9766AC)
	when there is none. The vtable at 0x898980 has a single slot (0x7BE6F0),
	which sizes the window with AdjustWindowRect and creates it. The
	destructor (0x7BE320) destroys the status bar and frees the caption.
	0x7BE4A0 creates the status bar ("msctls_statusbar32") and 0x7BE5B0
	sets a pane's text (SB_SETTEXT). The class name follows the Gamebryo
	lineage; the method names are invented.

===============================================================================
*/

#ifndef __NIAPPWINDOW_H__
#define __NIAPPWINDOW_H__

#include <windows.h>

class NiAppWindow {
public:
	// creates the window and its child, and shows it (Gamebryo lineage name)
	virtual HWND			CreateMainWindow( int showCommand );

							NiAppWindow( const char *caption, unsigned int width, unsigned int height,
										 unsigned int numStatusPanes, unsigned long style );
							~NiAppWindow();

	// create the status bar and grow the window by its height, and take it
	// down again (Gamebryo lineage names)
	void					StatusWindowCreate();
	void					StatusWindowDestroy();
	void					SetStatusPaneText( unsigned int pane, const char *text );
	// copies the caption into the title bar (Gamebryo lineage name)
	void					SetWindowCaption( char *caption_ );

	HWND					GetWindow() const { return window; }
	HWND					GetRenderWindow() const { return renderWindow; }
	unsigned int			GetWidth() const { return width; }
	unsigned int			GetHeight() const { return height; }
	void					SetWidth( unsigned int width_ ) { width = width_; }
	void					SetHeight( unsigned int height_ ) { height = height_; }
	void					SetParentWidth( unsigned int width_ ) { parentWidth = width_; }
	void					SetParentHeight( unsigned int height_ ) { parentHeight = height_; }
	// the window style the main window is created with (Gamebryo lineage name)
	void					SetWindowStyle( unsigned long style_ ) { style = style_; }
	// the status bar's window; the panes all live in it
	HWND					GetStatusWindow() const { return numStatusPanes > 0 && statusWindows ? statusWindows[0] : 0; }
	unsigned int			GetNumStatusPanes() const { return numStatusPanes; }
	bool					GetStatusBarCreated() const { return statusBarCreated; }
	// the pane array is allocated on first use
	void					SetStatusWindow( HWND statusWindow ) {
								if ( numStatusPanes > 0 ) {
									if ( !statusWindows ) {
										statusWindows = new HWND[numStatusPanes];
									}
									statusWindows[0] = statusWindow;
								}
							}

protected:
	HWND					window;					// 0x04
	HWND					renderWindow;			// 0x08 a child window covering the client size, drawn into when windowed (Gamebryo lineage name)
	HWND *					statusWindows;			// 0x0C allocated when the status bar is created
	char *					caption;				// 0x10
	unsigned int			width;					// 0x14 the client size
	unsigned int			parentWidth;			// 0x18 the main window's client size: the size asked for, then the resize handler's (Gamebryo lineage name)
	unsigned int			height;					// 0x1C
	unsigned int			parentHeight;			// 0x20
	unsigned int			numStatusPanes;			// 0x24
	bool					statusBarCreated;		// 0x28
	unsigned long			style;					// 0x2C
};

#endif /* !__NIAPPWINDOW_H__ */
