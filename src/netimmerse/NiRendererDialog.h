/*
===============================================================================

	NiRendererDialog.h

	The application framework's display-settings dialog: the user picks
	windowed or fullscreen, the Direct3D device and the fullscreen mode, and
	whether to ask for a deeper z-buffer, and the dialog creates the
	renderer. The dialog template is built in memory, the way the DirectX
	SDK samples' device selection dialog builds its own. The function names
	are invented; the strings name only the dialog ("Select Display Device",
	"Select New DirectDraw Driver") and its controls.

	Evidence: NiApplication::CreateRenderer (0x7B77B0) calls 0x7BA2E0 with
	the application window, its render window, the flag it otherwise turns
	into creation flag 4, and the full-screen flag by address, and takes the
	renderer it returns. 0x7BA2E0 clears the flag, builds the template
	(0x7B9EF0), runs it with DialogBoxIndirectParamA and the dialog
	procedure 0x7B9B20, sets the flag from the choice and creates the
	renderer with NiD3DRender::Create (0x7C09C0); a failure restores the
	window, reports the renderer's error string and runs the dialog again.
	0x7BA250, which nothing calls, runs the same dialog and only reports
	the choice. The choice lives in the unit's statics 0x9A0B34 (the
	device), 0x9A0B38 (the mode), 0x9A0B3C (fullscreen) and 0x9A0B3D (the
	z-buffer box).

===============================================================================
*/

#ifndef __NIRENDERERDIALOG_H__
#define __NIRENDERERDIALOG_H__

#include <windows.h>

class NiD3DRender;
class Ni3DDeviceDesc;
class NiD3DDisplayMode;

// the dialog's controls
#define IDC_DEVICE					1000	// "Driver:"
#define IDC_WINDOWED				1001	// "Mode:", windowed or fullscreen
#define IDC_DISPLAY_MODE			1002	// "Resolution:"
#define IDC_DEEP_Z_BUFFER			1003	// "Use 24/32-bit ZBuffer if available"
#define IDC_STATIC					( -1 )	// the labels, as the resource headers define it

// what NiSelectDisplaySettings returns when the dialog fails or is
// cancelled
#define DISPLAY_SETTINGS_CANCELLED	2

// runs the dialog until it creates a renderer; NULL when the user cancels
NiD3DRender *				NiSelectRenderer( HWND window, HWND renderWindow, bool stencil, bool &fullscreen );

// runs the dialog once and reports the choice; 0, or
// DISPLAY_SETTINGS_CANCELLED
int							NiSelectDisplaySettings( HWND window, bool &fullscreen, bool &deepZBuffer,
													 Ni3DDeviceDesc *&device, NiD3DDisplayMode *&mode );

// the dialog template, in a heap block the caller deletes
DLGTEMPLATE *				NiBuildDisplaySettingsTemplate();

// the selection of the device and mode lists into the statics
void						NiReadDeviceSelection( HWND dialog );
void						NiReadModeSelection( HWND dialog );

#endif /* !__NIRENDERERDIALOG_H__ */
