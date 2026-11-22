/*
===============================================================================

	UtopiaApp.h

	The game's application: the one instance NiApplication::application
	points to. It reads the display options ("Options.cfg", "Graphics
	Options") and the command line, and keeps the display size and depth
	and the state of the test menu.

	0xBC bytes: the SWIG wrapper new_UtopiaApp allocates that much.

	Evidence: the constructor (0x437FB0) calls TGApplication's with the
	caption "Bridge Commander" and 800 by 600, stores the vtable 0x889A98
	(43 slots, none of its own) and reads "Fullscreen Mode", "Display
	Width", "Display Height" and "Display Depth", and the command-line
	options "ForceWindowed" and "ForceFullscreen". The SWIG wrappers
	UtopiaApp_* read the display width (0xA8), height (0xAC), the device
	description (0xB0), the direct start flag (0xB4) and the test menu
	state (0xB8) inline, and NiApplication's fullscreen flag and bit depth;
	UtopiaApp_SetFrameRate sets NiApplication's frame period, and
	UtopiaApp_GetApp returns NiApplication::application.

===============================================================================
*/

#ifndef __UTOPIAAPP_H__
#define __UTOPIAAPP_H__

#include "TGApplication.h"

// The destructor is the implicit one: its copy (0x438280) only jumps to
// TGApplication's.
// FUNCTION: STBC 0x00438260 ??_GUtopiaApp@@UAEPAXI@Z
// FUNCTION: STBC 0x00438280 ??1UtopiaApp@@UAE@XZ
class UtopiaApp : public TGApplication {
public:
							UtopiaApp( void );

	// NiApplication
	virtual bool			Initialize();
	virtual void			Terminate();
	virtual bool			OnWindowResize( int width, int height, unsigned int sizeType, HWND window );
	virtual bool			OnDefault( NiEventRecord *event );
	virtual void			OnIdle();
	virtual bool			CreateRenderer();
	// TGApplication
	virtual void			Pause( bool pause );
	virtual void			OnUnpause();

	// FUNCTION: STBC 0x005af370 ?GetApp@UtopiaApp@@SAPAV1@XZ @game/ShipClass
	static UtopiaApp *		GetApp( void ) { return static_cast<UtopiaApp *>( application ); }
	// The application's renderer; static, as its callers show (invented
	// name). The game units keep out-of-line copies (Game's at 0x407C50).
	// FUNCTION: STBC 0x00407c50 ?GetRenderer@UtopiaApp@@SA?AV?$NiPointer@VNiRender@@@@XZ @game/Game
	static NiPointer<NiRender>	GetRenderer( void ) { return GetApp()->renderer; }
	// Replaces it (invented name); taking the smart pointer by value keeps it
	// out of line, in UtopiaModule's unit.
	// FUNCTION: STBC 0x004460f0 ?SetRenderer@UtopiaApp@@SAXV?$NiPointer@VNiRender@@@@@Z @game/UtopiaModule
	static void				SetRenderer( NiPointer<NiRender> renderer_ ) { GetApp()->renderer = renderer_; }

	bool					IsFullscreen( void ) { return fullscreen; }
	unsigned int			GetWidth( void ) { return width; }
	unsigned int			GetHeight( void ) { return height; }
	unsigned int			GetBitDepth( void ) { return bitDepth; }
	char *					GetDeviceDesc( void ) { return deviceDesc; }
	bool					IsDirectStart( void ) { return directStart; }
	int						GetTestMenuState( void ) { return testMenuState; }
	void					SetTestMenuState( int testMenuState_ ) { testMenuState = testMenuState_; }
	// a rate too small to invert gives the shortest frame period instead
	void					SetFrameRate( float rate ) {
								if ( rate < 0.00001f ) {
									minFramePeriod = 0.00001f;
								} else {
									minFramePeriod = 1.0f / rate;
								}
							}

protected:
	// Rounds the width up to the next supported display mode (640x480 to
	// 1600x1200) and takes the mode's height; a width past them is kept
	// (invented name).
	void					SnapToMode( void ) {
								if ( width <= 640 ) {
									width = 640;
									height = 480;
								} else if ( width <= 800 ) {
									width = 800;
									height = 600;
								} else if ( width <= 1024 ) {
									width = 1024;
									height = 768;
								} else if ( width <= 1280 ) {
									width = 1280;
									height = 1024;
								} else if ( width <= 1600 ) {
									width = 1600;
									height = 1200;
								}
							}

	unsigned int			width;					// 0xA8 "Display Width"
	unsigned int			height;					// 0xAC "Display Height"
	char *					deviceDesc;				// 0xB0
	bool					directStart;			// 0xB4
	int						testMenuState;			// 0xB8
};

#endif /* !__UTOPIAAPP_H__ */
