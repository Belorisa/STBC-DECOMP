// NiRendererDialog.cpp -- the display-settings dialog

#include "NiRTLib.h"
#include <stdio.h>

#include "NiRendererDialog.h"
#include "NiSystem.h"
#include "renderer/NiD3DRender.h"
#include "renderer/NiD3DDirectDraw.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007ba4c0 _$E219
// FUNCTION: STBC 0x007ba460 _$E17
// FUNCTION: STBC 0x007ba480 _$E15
// FUNCTION: STBC 0x007ba490 _$E23
// FUNCTION: STBC 0x007ba4b0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007ba4e0 _$E30
// FUNCTION: STBC 0x007ba4f0 _$E34
// FUNCTION: STBC 0x007ba500 _$E48
// FUNCTION: STBC 0x007ba510 _$E61
// FUNCTION: STBC 0x007ba520 _$E65
// FUNCTION: STBC 0x007ba530 _$E69
// FUNCTION: STBC 0x007ba540 _$E73
// FUNCTION: STBC 0x007ba550 _$E77
// FUNCTION: STBC 0x007ba560 _$E181
// FUNCTION: STBC 0x007ba570 _$E186
// FUNCTION: STBC 0x007ba580 _$E191

// DATA: STBC 0x9A0B34
static Ni3DDeviceDesc *selectedDevice;

// DATA: STBC 0x9A0B38
static NiD3DDisplayMode *selectedMode;

// DATA: STBC 0x9A0B3C
static bool fullscreenSelected;

// DATA: STBC 0x9A0B3D
static bool deepZBufferSelected;

/*
================
FillDeviceList

Lists the display devices that can render in the chosen mode and selects
the first.
================
*/
// FUNCTION: STBC 0x7B9880
void FillDeviceList( HWND dialog ) {
	SendDlgItemMessage( dialog, IDC_DEVICE, CB_RESETCONTENT, 0, 0 );

	NiTList<Ni3DDeviceDesc *> *devices = NiD3DRender::GetDisplayDevices();
	bool listed = false;
	if ( devices ) {
		NiTListIterator pos = devices->GetHeadPos();
		while ( pos ) {
			Ni3DDeviceDesc *device = devices->GetNext( pos );
			if ( fullscreenSelected || device->windowed ) {
				char *deviceName = device->name;
				int index = SendDlgItemMessage( dialog, IDC_DEVICE, CB_ADDSTRING, 0, (LPARAM)deviceName );
				SendDlgItemMessage( dialog, IDC_DEVICE, CB_SETITEMDATA, index, (LPARAM)device );
				listed = true;
			}
		}
	}

	SendDlgItemMessage( dialog, IDC_DEVICE, CB_SETCURSEL, 0, 0 );
	if ( listed ) {
		selectedDevice = (Ni3DDeviceDesc *)SendDlgItemMessage( dialog, IDC_DEVICE, CB_GETITEMDATA, 0, 0 );
	} else {
		selectedDevice = 0;
	}
}

/*
================
FillModeList

Lists the selected device's fullscreen modes, selecting 640 x 480 x 16
when the device has it. The selected mode is read from the first entry,
whichever one the list shows (a bug of the original).
================
*/
// FUNCTION: STBC 0x7B9940
void FillModeList( HWND dialog ) {
	SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_RESETCONTENT, 0, 0 );
	if ( !fullscreenSelected ) {
		return;
	}

	NiTList<NiD3DDisplayMode *> *modes = selectedDevice ? selectedDevice->modes : 0;
	int defaultIndex = 0;
	bool listed = false;
	if ( modes ) {
		NiTListIterator pos = modes->GetHeadPos();
		while ( pos ) {
			NiD3DDisplayMode *mode = modes->GetNext( pos );
			char text[256];
			sprintf( text, "%u x %u x %u bpp", mode->width, mode->height, mode->bitDepth );
			int index = SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_ADDSTRING, 0, (LPARAM)text );
			SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_SETITEMDATA, index, (LPARAM)mode );
			if ( mode->width == 640 && mode->height == 480 && mode->bitDepth == 16 ) {
				defaultIndex = index;
			}
			listed = true;
		}
	}

	SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_SETCURSEL, defaultIndex, 0 );
	if ( listed ) {
		selectedMode = (NiD3DDisplayMode *)SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_GETITEMDATA, 0, 0 );
	} else {
		selectedMode = 0;
	}
}

/*
================
NiReadDeviceSelection
================
*/
// FUNCTION: STBC 0x7B9A80
void NiReadDeviceSelection( HWND dialog ) {
	int index = SendDlgItemMessage( dialog, IDC_DEVICE, CB_GETCURSEL, 0, 0 );
	if ( index >= 0 ) {
		selectedDevice = (Ni3DDeviceDesc *)SendDlgItemMessage( dialog, IDC_DEVICE, CB_GETITEMDATA, index, 0 );
	} else {
		selectedDevice = 0;
	}
}

/*
================
NiReadModeSelection
================
*/
// FUNCTION: STBC 0x7B9AD0
void NiReadModeSelection( HWND dialog ) {
	int index = SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_GETCURSEL, 0, 0 );
	if ( index >= 0 ) {
		selectedMode = (NiD3DDisplayMode *)SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_GETITEMDATA, index, 0 );
	} else {
		selectedMode = 0;
	}
}

/*
================
ReadWindowedSelection
================
*/
void ReadWindowedSelection( HWND dialog ) {
	int index = SendDlgItemMessage( dialog, IDC_WINDOWED, CB_GETCURSEL, 0, 0 );
	if ( index >= 0 ) {
		fullscreenSelected = SendDlgItemMessage( dialog, IDC_WINDOWED, CB_GETITEMDATA, index, 0 ) != 0;
	}
}

/*
================
DisplaySettingsProc

The dialog procedure. A double click on a device accepts the dialog.
================
*/
// FUNCTION: STBC 0x7B9B20
// PARKED: register/store forwarding: in the windowed-combo case retail keeps the new fullscreen flag in CL and compares it with the old one; this build compares against memory
BOOL CALLBACK DisplaySettingsProc( HWND dialog, UINT message, WPARAM wParam, LPARAM lParam ) {
	switch ( message ) {
		case WM_INITDIALOG: {
			SetWindowText( dialog, "Select Display Device" );
			SendDlgItemMessage( dialog, IDC_WINDOWED, CB_RESETCONTENT, 0, 0 );
			int index = SendDlgItemMessage( dialog, IDC_WINDOWED, CB_ADDSTRING, 0, (LPARAM)"Windowed" );
			SendDlgItemMessage( dialog, IDC_WINDOWED, CB_SETITEMDATA, index, FALSE );
			SendDlgItemMessage( dialog, IDC_WINDOWED, CB_SETCURSEL, 0, 0 );
			fullscreenSelected = false;
			index = SendDlgItemMessage( dialog, IDC_WINDOWED, CB_ADDSTRING, 0, (LPARAM)"Fullscreen" );
			SendDlgItemMessage( dialog, IDC_WINDOWED, CB_SETITEMDATA, index, TRUE );
			FillDeviceList( dialog );
			FillModeList( dialog );
			return TRUE;
		}

		case WM_COMMAND:
			switch ( LOWORD( wParam ) ) {
				case IDOK:
					NiReadDeviceSelection( dialog );
					NiReadModeSelection( dialog );
					deepZBufferSelected = ( SendDlgItemMessage( dialog, IDC_DEEP_Z_BUFFER, BM_GETSTATE, 0, 0 ) & BST_CHECKED ) != 0;
					SendDlgItemMessage( dialog, IDC_WINDOWED, CB_RESETCONTENT, 0, 0 );
					SendDlgItemMessage( dialog, IDC_DEVICE, CB_RESETCONTENT, 0, 0 );
					SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_RESETCONTENT, 0, 0 );
					EndDialog( dialog, TRUE );
					return TRUE;

				case IDCANCEL:
					SendDlgItemMessage( dialog, IDC_WINDOWED, CB_RESETCONTENT, 0, 0 );
					SendDlgItemMessage( dialog, IDC_DEVICE, CB_RESETCONTENT, 0, 0 );
					SendDlgItemMessage( dialog, IDC_DISPLAY_MODE, CB_RESETCONTENT, 0, 0 );
					EndDialog( dialog, FALSE );
					return TRUE;

				case IDC_DEVICE:
					if ( HIWORD( wParam ) == CBN_DBLCLK ) {
						SendMessage( dialog, WM_COMMAND, IDOK, 0 );
					}
					NiReadDeviceSelection( dialog );
					FillModeList( dialog );
					return TRUE;

				case IDC_WINDOWED: {
					bool wasFullscreen = fullscreenSelected;
					ReadWindowedSelection( dialog );
					if ( wasFullscreen != fullscreenSelected ) {
						FillDeviceList( dialog );
						FillModeList( dialog );
					}
					return TRUE;
				}
			}
			break;
	}
	return FALSE;
}

/*
================
CopyToWideChar

Appends a string to a dialog template as a terminated wide string.
================
*/
// FUNCTION: STBC 0x7B9DF0
void CopyToWideChar( WCHAR **out, const char *string ) {
	int length = lstrlen( string );
	WCHAR *wide = *out;
	if ( length ) {
		length = MultiByteToWideChar( CP_ACP, 0, string, length, wide, length );
	}
	wide[length] = L'\0';
	*out += length + 1;
}

/*
================
AddDialogControl

Appends a visible child control to a dialog template, DWORD aligned.
================
*/
// FUNCTION: STBC 0x7B9E30
void AddDialogControl( WORD **out, DWORD style, short x, short y, short cx, short cy, WORD id,
							  const char *className, const char *title ) {
	DLGITEMTEMPLATE *item = (DLGITEMTEMPLATE *)( ( (ULONG)*out + 3 ) & ~3 );
	item->style = style | WS_CHILD | WS_VISIBLE;
	item->dwExtendedStyle = 0;
	item->x = x;
	item->y = y;
	item->cx = cx;
	item->cy = cy;
	item->id = id;
	*out = (WORD *)( item + 1 );

	CopyToWideChar( (WCHAR **)out, className );
	CopyToWideChar( (WCHAR **)out, title );

	// no creation data
	( *out )++;
}

/*
================
NiBuildDisplaySettingsTemplate

The dialog, built in a heap block the caller deletes.
================
*/
// FUNCTION: STBC 0x7B9EF0
DLGTEMPLATE *NiBuildDisplaySettingsTemplate() {
	DLGTEMPLATE *dlgTemplate = new DLGTEMPLATE[50];
	if ( !dlgTemplate ) {
		return 0;
	}
	ZeroMemory( dlgTemplate, 50 * sizeof( DLGTEMPLATE ) );

	DLGTEMPLATE *header = dlgTemplate;
	header->style = DS_MODALFRAME | DS_NOIDLEMSG | DS_SETFOREGROUND | DS_3DLOOK | DS_CENTER | WS_POPUP | WS_VISIBLE
					| WS_CAPTION | WS_SYSMENU | DS_SETFONT;
	header->dwExtendedStyle = 0;
	header->cdit = 9;
	header->x = 0;
	header->y = 0;
	header->cx = 200;
	header->cy = 120;

	// no menu, the default class, the title, then the font
	WORD *out = (WORD *)( header + 1 );
	*out++ = 0;
	*out++ = 0;
	CopyToWideChar( (WCHAR **)&out, "Select New DirectDraw Driver" );
	*out++ = 8;
	CopyToWideChar( (WCHAR **)&out, "Arial" );

	AddDialogControl( &out, CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 50, 5, 138, 45, IDC_WINDOWED, "COMBOBOX", "" );
	AddDialogControl( &out, SS_LEFT, 5, 5, 36, 13, (WORD)IDC_STATIC, "STATIC", "Mode:" );
	AddDialogControl( &out, CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 50, 26, 138, 45, IDC_DEVICE, "COMBOBOX", "" );
	AddDialogControl( &out, SS_LEFT, 5, 26, 36, 13, (WORD)IDC_STATIC, "STATIC", "Driver:" );
	AddDialogControl( &out, CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 50, 48, 138, 45, IDC_DISPLAY_MODE, "COMBOBOX", "" );
	AddDialogControl( &out, SS_LEFT, 5, 48, 36, 13, (WORD)IDC_STATIC, "STATIC", "Resolution:" );
	AddDialogControl( &out, BS_AUTOCHECKBOX | WS_TABSTOP, 40, 65, 120, 14, IDC_DEEP_Z_BUFFER, "BUTTON",
					  "Use 24/32-bit ZBuffer if available" );
	AddDialogControl( &out, BS_PUSHBUTTON | WS_TABSTOP, 123, 90, 51, 14, IDCANCEL, "BUTTON", "CANCEL" );
	AddDialogControl( &out, BS_PUSHBUTTON | WS_TABSTOP, 40, 90, 51, 14, IDOK, "BUTTON", "OK" );

	return dlgTemplate;
}

/*
================
NiSelectDisplaySettings
================
*/
// FUNCTION: STBC 0x7BA250
int NiSelectDisplaySettings( HWND window, bool &fullscreen, bool &deepZBuffer, Ni3DDeviceDesc *&device,
							 NiD3DDisplayMode *&mode ) {
	fullscreen = false;
	HINSTANCE instance = (HINSTANCE)GetWindowLong( window, GWL_HINSTANCE );
	DLGTEMPLATE *dlgTemplate = NiBuildDisplaySettingsTemplate();
	if ( dlgTemplate ) {
		int result = DialogBoxIndirectParam( instance, dlgTemplate, window, DisplaySettingsProc, 0 );
		delete [] dlgTemplate;
		if ( result ) {
			device = selectedDevice;
			fullscreen = fullscreenSelected;
			mode = fullscreenSelected ? selectedMode : 0;
			deepZBuffer = deepZBufferSelected;
			return 0;
		}
	}
	return DISPLAY_SETTINGS_CANCELLED;
}

/*
================
NiSelectRenderer

Runs the dialog until the chosen renderer can be created. A failure puts
the window back where it was and reports the renderer's error; the retry
loses the stencil flag (a quirk of the original).
================
*/
// FUNCTION: STBC 0x7BA2E0
// PARKED: store forwarding: retail re-reads the fullscreen flag through the reference after the inlined NiSelectDisplaySettings; this build tests the global it just reloaded
NiD3DRender *NiSelectRenderer( HWND window, HWND renderWindow, bool stencil, bool &fullscreen ) {
	RECT rect;
	GetWindowRect( window, &rect );

	unsigned int flags = 0;
	if ( stencil ) {
		flags = RENDER_Z_BUFFER_STENCIL;
	}
	for ( ;; ) {
		Ni3DDeviceDesc *device;
		NiD3DDisplayMode *mode;
		bool deepZBuffer;
		if ( NiSelectDisplaySettings( window, fullscreen, deepZBuffer, device, mode ) != 0 ) {
			return 0;
		}

		if ( deepZBuffer ) {
			flags |= RENDER_Z_BUFFER_DEEP;
		} else {
			flags |= RENDER_Z_BUFFER;
		}

		NiD3DRender *renderer;
		if ( fullscreen ) {
			if ( mode ) {
				switch ( mode->bitDepth ) {
					case 16:
						flags |= 8;
						break;
					case 32:
						flags |= 0x10;
						break;
				}
				renderer = NiD3DRender::Create( RENDER_FULLSCREEN, mode->width, mode->height, flags, window, window, device );
			} else {
				flags |= 8;
				renderer = NiD3DRender::Create( RENDER_FULLSCREEN, 640, 480, flags, window, window, device );
			}
		} else {
			renderer = NiD3DRender::Create( RENDER_WINDOWED, 0, 0, flags, renderWindow, window, device );
		}
		if ( renderer ) {
			return renderer;
		}

		MoveWindow( window, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE );
		char text[512];
		sprintf( text, "D3D Render Creation Error\nError string from renderer was:\n\"%s\"\n", NiD3DRender::errorString );
		NiMessageBox( text, "Renderer Creation Failure" );
		flags = 0;
	}
}
