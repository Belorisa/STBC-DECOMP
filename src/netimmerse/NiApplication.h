/*
===============================================================================

	NiApplication.h

	The NetImmerse 3.1 application framework: the one application object,
	which owns the main window, the renderer, the camera and the scene,
	runs the message loop, measures the frame time and shows the frame
	rate in the status bar. The game's application derives from it.

	Evidence: the constructor (0x7B7180) stores the vtable 0x8988D4 (39
	slots) and the object in the static instance pointer 0x9A09D0, which
	the window procedure (0x7BA5E0) and the message loop read. Initialize
	(slot 1, 0x7B7C70) calls slots 35, 36 and 34 in turn, the creators of
	the camera (an NiCamera, 0x7E7AF0), the renderer and the scene (an
	NiNode, 0x7E37C0); Terminate (slot 2) releases all three. The window
	procedure sends WM_CREATE to slot 11, WM_DESTROY to slot 14 (then
	PostQuitMessage), WM_MOVE to slot 15, WM_SIZE to slot 12 and WM_PAINT
	to slot 8. Slot 30 is one turn of the message loop (PeekMessage); slot
	32, called when there is no message, renders a frame when the frame
	period has passed; slot 38 shows "fps: %.1f" (0x976428) in the status
	bar. The command-line options "full", "direct3d", "opengl" and
	"software" (0x976358 down) select the renderer.

	The class name and the names of the lineage's functions (Initialize,
	Terminate, OnIdle, OnWindowResize, OnWindowDestroy, CreateScene,
	CreateCamera, CreateRenderer, MeasureTime, DrawFrameRate, Process,
	SetCommandLine, CommandSelectRenderer) follow the Gamebryo framework;
	no string in this binary names them. Slots 3 to 15 are Gamebryo's
	CreateMenuBar through OnWindowMove in its order, each taking as many
	arguments as its retail body pops; the window procedure's WM_COMMAND,
	WM_PAINT, WM_ERASEBKGND, WM_CREATE, WM_SIZE, WM_DESTROY and WM_MOVE
	cases call the slots of those names. Member names are invented from
	their use.

===============================================================================
*/

#ifndef __NIAPPLICATION_H__
#define __NIAPPLICATION_H__

#include <windows.h>
#include "NiPointer.h"
#include "NiColor.h"
#include "NiNode.h"
#include "NiCamera.h"
#include "renderer/NiRender.h"

// winbase.h's GetCurrentTime() macro (GetTickCount) would rename the
// accessor below
#undef GetCurrentTime

class NiAppWindow;
class NiCommand;
class NiMessageLoop;
class NiKeyboard;
class NiMouse;
class NiGamePad;
class NiFrameRate;

// a window message as the window procedure hands it to the application
// (Gamebryo lineage name and layout)
struct NiEventRecord {
	HWND					hWnd;
	UINT					uiMsg;
	WPARAM					wParam;
	LPARAM					lParam;
};

// the renderer the command line selects
enum rendererType_t {
	RENDERER_DIRECT3D,				// "direct3d"
	RENDERER_OPENGL,				// "opengl"
	RENDERER_SOFTWARE,				// "software"
	RENDERER_PSX2					// not built in this binary
};

class NiApplication {
public:
							NiApplication( const char *caption, unsigned int width, unsigned int height,
										   unsigned int menuID, unsigned int numStatusPanes, unsigned int bitDepth );
	virtual					~NiApplication();

	virtual bool			Initialize();
	virtual void			Terminate();
	virtual void			CreateMenuBar();
	virtual bool			OnHighLevelEvent( unsigned int unknown );
	virtual bool			OnOSEvent( unsigned int unknown );
	virtual bool			OnDiskEvent( unsigned int unknown );
	virtual bool			OnCommand( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3 );	// WM_COMMAND
	virtual bool			OnWindowDraw( HDC dc );						// WM_PAINT, the BeginPaint context
	virtual bool			OnWindowErase( HWND window );					// WM_ERASEBKGND
	virtual bool			OnWindowActivate( unsigned int unknown1, unsigned int unknown2 );
	virtual bool			OnWindowCreate( unsigned int unknown );			// WM_CREATE: false fails the creation
	virtual bool			OnWindowResize( int width, int height, unsigned int sizeType, HWND window );
	virtual bool			OnWindowZoom( unsigned int unknown1, unsigned int unknown2 );
	virtual bool			OnWindowDestroy( HWND window, bool unknown );
	virtual bool			OnWindowMove( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3 );	// WM_MOVE
	virtual short			VirtualSlot16( NiEventRecord *event );					// keyboard messages: the modifiers' high word
	virtual bool			VirtualSlot17( char key, unsigned int modifiers );	// WM_SYSCHAR
	virtual bool			VirtualSlot18( unsigned int key, unsigned int modifiers );	// WM_SYSKEYDOWN
	virtual bool			VirtualSlot19( unsigned int key, unsigned int modifiers );	// WM_SYSKEYUP
	virtual bool			VirtualSlot20( char key, unsigned int modifiers );	// WM_CHAR
	virtual bool			VirtualSlot21( unsigned int unknown1, unsigned int unknown2 );
	virtual bool			VirtualSlot22( unsigned int unknown1, unsigned int unknown2 );
	virtual short			VirtualSlot23( NiEventRecord *event );					// mouse messages: the modifiers' high word
	virtual bool			VirtualSlot24( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3, unsigned int unknown4 );
	virtual bool			VirtualSlot25( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3, unsigned int unknown4 );
	virtual bool			VirtualSlot26( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3 );
	virtual unsigned short	VirtualSlot27( unsigned int unknown );
	virtual bool			VirtualSlot28( unsigned int unknown1, unsigned int unknown2 );
	virtual bool			VirtualSlot29( unsigned int unknown1, unsigned int unknown2 );
	// one turn of the message loop: false on WM_QUIT
	virtual bool			Process( int &exitCode );
	// a window message the window procedure doesn't handle; true when it
	// is handled (Gamebryo 1.1.2: NiApplication::OnDefault)
	virtual bool			OnDefault( NiEventRecord *event );
	// renders a frame when the frame period has passed
	virtual void			OnIdle();
	// shows or hides the frame-rate overlay (Gamebryo lineage name)
	virtual void			EnableFrameRate( bool enable );
	virtual bool			CreateScene();
	virtual bool			CreateCamera();
	virtual bool			CreateRenderer();
	virtual void			MeasureTime();
	virtual void			DrawFrameRate();

	// the command line, parsed for the options
	// the camera the application draws with (name invented; out of line,
	// as a class returned by value under /GX)
	// FUNCTION: STBC 0x0042e330 ?GetCamera@NiApplication@@QBE?AV?$NiPointer@VNiCamera@@@@XZ
	NiPointer<NiCamera>		GetCamera() const { return camera; }
	NiAppWindow *			GetAppWindow() const { return appWindow; }
	NiKeyboard *			GetKeyboard() const { return keyboard; }
	NiMouse *				GetMouse() const { return mouse; }
	NiMessageLoop *			GetMessageLoop() const { return messageLoop; }
	unsigned int			GetMenuID() const { return menuID; }
	bool					GetFullscreen() const { return fullscreen; }
	rendererType_t			GetRendererType() const { return rendererType; }
	// the time of the last measurement, the seconds the last frame took,
	// and the seconds since the first measurement (Gamebryo lineage names)
	float					GetCurrentTime() const { return currentTime; }
	float					GetFrameTime() const { return frameTime; }
	float					GetAccumTime() const { return accumTime; }

	// the module handle the windows are created with (Gamebryo lineage name)
	static HINSTANCE		GetInstanceReference() { return instance; }
	static void				SetInstanceReference( HINSTANCE instance_ ) { instance = instance_; }

	static void				SetCommandLine( const char *commandLine );
	// creates the application: the program defines it (Gamebryo lineage
	// name; the game's is in UtopiaApp's unit)
	static NiApplication *	Create();
	static void				CommandSelectRenderer();

	static NiApplication *	application;		// the one instance
	static HINSTANCE		instance;
	static NiCommand *		command;
	static HACCEL			accelerator;		// the message loop's accelerator table
	static bool				unknown9A09CC;		// TODO identity: set by CreateRenderer for Direct3D (and by the game's own renderer setup, 0x438290); never read

protected:
	NiPointer<NiNode>		scene;					// 0x04
	NiPointer<NiCamera>		camera;					// 0x08
	NiPointer<NiRender>		renderer;				// 0x0C
	NiAppWindow *			appWindow;				// 0x10
	NiMessageLoop *			messageLoop;			// 0x14
	unsigned int			menuID;					// 0x18
	NiKeyboard *			keyboard;				// 0x1C
	NiMouse *				mouse;					// 0x20
	NiGamePad *				gamePad;				// 0x24 never created by the framework
	rendererType_t			rendererType;			// 0x28
	bool					fullscreen;				// 0x2C a bit depth was given, or the "full" option
	bool					unknown2D;				// 0x2D
	bool					unknown2E;				// 0x2E
	bool					unknown2F;				// 0x2F
	bool					unknown30;				// 0x30
	bool					unknown31;				// 0x31
	unsigned int			bitDepth;				// 0x34
	NiColor					backgroundColor;		// 0x38 half grey (Gamebryo lineage name)
	NiFrameRate *			frameRateDisplay;		// 0x44 the overlay Initialize creates (0x1F0 bytes, constructor 0x7BB440)
	bool					frameRateEnabled;		// 0x48
	int						maxTimer;				// 0x4C frames between frame-rate updates
	int						timer;					// 0x50 frames to the next update
	float					currentTime;			// 0x54 seconds, NiGetCurrentTimeInSec
	float					lastTime;				// 0x58 -1 until the first measurement
	float					frameTime;				// 0x5C
	float					accumTime;				// 0x60 seconds since the first measurement
	int						clicks;					// 0x64 frames rendered
	float					frameRate;				// 0x68
	float					lastFrameRateTime;		// 0x6C accumTime at the last update
	int						lastFrameRateClicks;	// 0x70 clicks at the last update
	float					minFramePeriod;			// 0x74 seconds
	float					lastFrame;				// 0x78 accumTime of the last frame rendered
};

#endif /* !__NIAPPLICATION_H__ */
