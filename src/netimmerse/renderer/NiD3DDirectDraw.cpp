// NiD3DDirectDraw.cpp -- the renderer's DirectDraw encapsulation object

#include "NiRTLib.h"
#include <stdio.h>

#include "NiD3DDirectDraw.h"
#include "NiD3DRender.h"
#include "NiDDImage.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007c9500 _$E230
// FUNCTION: STBC 0x007c92d0 _$E17
// FUNCTION: STBC 0x007c92f0 _$E15
// FUNCTION: STBC 0x007c9300 _$E23
// FUNCTION: STBC 0x007c9320 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007c9520 _$E30
// FUNCTION: STBC 0x007c9580 _$E67
// FUNCTION: STBC 0x007c9630 _$E44
// FUNCTION: STBC 0x007c9640 _$E57
// FUNCTION: STBC 0x007c9650 _$E71
// FUNCTION: STBC 0x007c9660 _$E75
// FUNCTION: STBC 0x007c9670 _$E79
// FUNCTION: STBC 0x007c9680 _$E83
// FUNCTION: STBC 0x007c9690 _$E187
// FUNCTION: STBC 0x007c96a0 _$E192
// FUNCTION: STBC 0x007c96b0 _$E197

// FUNCTION: STBC 0x007c7bc0 _$E218
// FUNCTION: STBC 0x007c7c00 _$E215
// DATA: STBC 0x9A1268 ?directDraws@NiD3DDirectDraw@@2V?$NiTList@PAVNiD3DDirectDraw@@@@A
NiTList<NiD3DDirectDraw *> NiD3DDirectDraw::directDraws( 2 );

// DATA: STBC 0x9A1294 ?displayDevices@NiD3DDirectDraw@@2PAV?$NiTList@PAVNi3DDeviceDesc@@@@A
NiTList<Ni3DDeviceDesc *> *NiD3DDirectDraw::displayDevices;
// DATA: STBC 0x9A1298 ?fullscreenDevice@NiD3DDirectDraw@@2PAVNi3DDeviceDesc@@A
Ni3DDeviceDesc *NiD3DDirectDraw::fullscreenDevice;
// DATA: STBC 0x9A129C ?windowedDevice@NiD3DDirectDraw@@2PAVNi3DDeviceDesc@@A
Ni3DDeviceDesc *NiD3DDirectDraw::windowedDevice;
// DATA: STBC 0x9A12A0 ?softwareDevice@NiD3DDirectDraw@@2PAVNi3DDeviceDesc@@A
Ni3DDeviceDesc *NiD3DDirectDraw::softwareDevice;
// DATA: STBC 0x9A12A4 ?directDrawCreateEx@NiD3DDirectDraw@@2P6GJPAU_GUID@@PAPAXABU2@PAUIUnknown@@@ZA
DirectDrawCreateExFunction NiD3DDirectDraw::directDrawCreateEx;

/*
================
Ni3DDeviceDescCleanup

Deletes the enumerated display devices at exit (the class name is
invented). The software device keeps its dangling pointer (an original
quirk).
================
*/
class Ni3DDeviceDescCleanup {
public:
	~Ni3DDeviceDescCleanup() {
		if ( NiD3DDirectDraw::displayDevices ) {
			while ( NiD3DDirectDraw::displayDevices->GetSize() ) {
				delete NiD3DDirectDraw::displayDevices->RemoveHead();
			}
			delete NiD3DDirectDraw::displayDevices;
			NiD3DDirectDraw::displayDevices = 0;
		}
		NiD3DDirectDraw::fullscreenDevice = 0;
		NiD3DDirectDraw::windowedDevice = 0;
	}
};

// FUNCTION: STBC 0x007c7c70 _$E223
// FUNCTION: STBC 0x007c7c80 _$E221
static Ni3DDeviceDescCleanup displayDeviceCleanup;

/*
================
NiD3DDirectDraw::NiD3DDirectDraw
================
*/
// FUNCTION: STBC 0x7C7D40
NiD3DDirectDraw::NiD3DDirectDraw() {
	primarySurface = 0;
	fullscreen = false;
	directDraw = 0;
	hardware = false;
	memset( &caps, 0, sizeof( caps ) );
	displayDevice = 0;
	bitDepth = 0;
	window = 0;
	displayModeSet = false;
	refCount = 0;
}

/*
================
NiD3DDirectDraw::~NiD3DDirectDraw
================
*/
// FUNCTION: STBC 0x7C7D90
NiD3DDirectDraw::~NiD3DDirectDraw() {
	NiD3DRender::Message( "(Shutdown): Releasing DirectDraw encapsulation object" );
	if ( directDraw ) {
		RestoreDisplayMode();
		delete displayDevice;
		primaryImage = 0;
		if ( primarySurface ) {
			ULONG references = primarySurface->Release();
			if ( references ) {
				char text[256];
				sprintf( text, "(Shutdown): Release of DirectDraw Primary Surface returned a refcount of %d", references );
				NiD3DRender::Warning( text );
			}
		}
		NiDDImage::ReleaseAllSurfaces( directDraw );
		directDraw->Release();
	}
	NiD3DRender::Message( "(Shutdown): DirectDraw encapsulation object released" );
	directDraws.Remove( this );
}

/*
================
NiD3DDirectDraw::CreateDefault

Every choice is the windowed device (an original quirk): AnimOnly.exe
0x51A6D8 (NiDirect3D) reads the windowed device in all three arms, and
release merges them, keeping only the argument's test.
================
*/
// FUNCTION: STBC 0x7C7F60
NiD3DDirectDraw *NiD3DDirectDraw::CreateDefault( bool unknown ) {
	Ni3DDeviceDesc *device = 0;
	if ( unknown ) {
		device = windowedDevice;
	} else if ( fullscreenDevice ) {
		device = windowedDevice;	// BUG: the fullscreen arm takes the windowed device
	} else {
		device = windowedDevice;
	}
	return Create( device );
}

/*
================
NiD3DDirectDraw::Create
================
*/
// FUNCTION: STBC 0x7C7F80
NiD3DDirectDraw *NiD3DDirectDraw::Create( Ni3DDeviceDesc *device ) {
	NiD3DRender::Message( "(Creation): Attempting to initialize DirectDraw device" );

	NiD3DDirectDraw *object = new NiD3DDirectDraw;
	object->hardware = false;
	object->directDraw = 0;
	object->primarySurface = 0;
	object->primaryImage = 0;
	object->displayDevice = new Ni3DDeviceDesc( *device );

	if ( !LoadDirectDrawCreateEx()
		|| directDrawCreateEx( object->displayDevice ? object->displayDevice->driverGuid : 0,
			(LPVOID *)&object->directDraw, IID_IDirectDraw7, 0 ) != DD_OK ) {
		NiD3DRender::Error( "Creation failed: DirectDrawCreate for HEL failed" );
		return 0;
	}

	DDCAPS_DX7 helCaps;
	memset( &object->caps, 0, sizeof( object->caps ) );
	memset( &helCaps, 0, sizeof( helCaps ) );
	object->caps.dwSize = sizeof( object->caps );
	helCaps.dwSize = sizeof( helCaps );
	if ( object->directDraw->GetCaps( &object->caps, &helCaps ) != DD_OK ) {
		NiD3DRender::Error( "Creation failed: Direct Draw Caps query failed" );
		delete object;
		return 0;
	}

	if ( !( object->caps.dwCaps & DDCAPS_3D ) ) {
		NiD3DRender::Warning( "(Creation): Hardware DirectDraw has no 3D capabilities" );
		NiD3DRender::Warning( "(Creation): Using DirectDraw software" );
		if ( !( helCaps.dwCaps & DDCAPS_3D ) ) {
			NiD3DRender::Error( "Creation failed: Software DirectDraw has no 3D capabilities" );
			delete object;
			return 0;
		}
		object->hardware = false;
		object->caps = helCaps;
	} else {
		object->hardware = true;
	}

	object->refCount = 1;
	directDraws.AddTail( object );
	NiD3DRender::Message( "(Creation): DirectDraw device initialization succeeded" );
	return object;
}

/*
================
NiD3DDirectDraw::CreateFromDirectDraw

Wraps an application's DirectDraw object. Written as the SDK's debug
NetImmerse 3.1 compiles it (AnimOnly.exe 0x51AAEC, in its NiDirect3D
library): each query's result kept in a local, the object cleared after
each delete (a store the release build drops), AddRef through the member,
and the software caps copied with memcpy.
================
*/
// FUNCTION: STBC 0x7C8360
NiD3DDirectDraw *NiD3DDirectDraw::CreateFromDirectDraw( IDirectDraw7 *directDraw_, Ni3DDeviceDesc *device ) {
	NiD3DRender::Message( "(Creation): Attempting to initialize DirectDraw device from app-supplied DirectDraw object" );

	NiD3DDirectDraw *object = new NiD3DDirectDraw;
	object->hardware = false;
	object->directDraw = 0;
	object->window = 0;
	object->primarySurface = 0;
	object->primaryImage = 0;
	object->displayDevice = new Ni3DDeviceDesc( *device );

	object->directDraw = directDraw_;
	object->directDraw->AddRef();

	DDSURFACEDESC2 surfaceDesc;
	memset( &surfaceDesc, 0, sizeof( surfaceDesc ) );
	surfaceDesc.dwSize = sizeof( surfaceDesc );
	HRESULT result = object->directDraw->GetDisplayMode( &surfaceDesc );
	if ( result != DD_OK ) {
		NiD3DRender::Error( "Creation failed: Get Display Mode failed" );
		delete object;
		object = 0;
		return 0;
	}
	object->bitDepth = surfaceDesc.ddpfPixelFormat.dwRGBBitCount;

	DDCAPS_DX7 helCaps;
	memset( &object->caps, 0, sizeof( object->caps ) );
	memset( &helCaps, 0, sizeof( helCaps ) );
	object->caps.dwSize = sizeof( object->caps );
	helCaps.dwSize = sizeof( helCaps );
	result = object->directDraw->GetCaps( &object->caps, &helCaps );
	if ( result != DD_OK ) {
		NiD3DRender::Error( "Creation failed: Direct Draw Caps query failed" );
		delete object;
		object = 0;
		return 0;
	}

	if ( !( object->caps.dwCaps & DDCAPS_3D ) ) {
		NiD3DRender::Warning( "(Creation): Hardware DirectDraw has no 3D capabilities" );
		NiD3DRender::Warning( "(Creation): Using DirectDraw software" );
		if ( !( helCaps.dwCaps & DDCAPS_3D ) ) {
			NiD3DRender::Error( "Creation failed: Software DirectDraw has no 3D capabilities" );
			delete object;
			object = 0;
			return 0;
		}
		object->hardware = false;
		memcpy( &object->caps, &helCaps, sizeof( helCaps ) );
	} else {
		object->hardware = true;
	}

	object->refCount = 1;
	directDraws.AddTail( object );
	NiD3DRender::Message( "(Creation): DirectDraw device initialization succeeded" );
	return object;
}

/*
================
NiD3DDirectDraw::EnumModesCallback

Keeps the 16- and 32-bit modes the device renders at.
================
*/
// FUNCTION: STBC 0x7C8710
HRESULT CALLBACK NiD3DDirectDraw::EnumModesCallback( LPDDSURFACEDESC2 surfaceDesc, LPVOID context ) {
	if ( !surfaceDesc || !context ) {
		return DDENUMRET_CANCEL;
	}
	NiD3DModeEnumContext *modeContext = (NiD3DModeEnumContext *)context;
	DWORD renderBitDepths = modeContext->renderBitDepths;
	DWORD modeBitDepth = surfaceDesc->ddpfPixelFormat.dwRGBBitCount;
	if ( ( modeBitDepth == 32 && ( renderBitDepths & DDBD_32 ) )
		|| ( modeBitDepth == 16 && ( renderBitDepths & DDBD_16 ) ) ) {
		modeContext->device->AddMode( surfaceDesc->dwWidth, surfaceDesc->dwHeight, surfaceDesc->ddpfPixelFormat.dwRGBBitCount );
	}
	return DDENUMRET_OK;
}

/*
================
NiD3DDirectDraw::LoadDirectDrawCreateEx

Looks up DirectDrawCreateEx in DDRAW.DLL once.
================
*/
// FUNCTION: STBC 0x7C8770
bool NiD3DDirectDraw::LoadDirectDrawCreateEx() {
	if ( directDrawCreateEx ) {
		return true;
	}
	HMODULE module = GetModuleHandle( "DDRAW.DLL" );
	if ( module ) {
		directDrawCreateEx = (DirectDrawCreateExFunction)GetProcAddress( module, "DirectDrawCreateEx" );
		if ( directDrawCreateEx ) {
			return true;
		}
	}
	return false;
}

/*
================
NiD3DDirectDraw::EnumerateDevices

Adds the Direct3D devices of a driver with hardware capabilities.
================
*/
// FUNCTION: STBC 0x7C87B0
void NiD3DDirectDraw::EnumerateDevices( IDirectDraw7 *directDraw_, GUID *driverGuid, NiTList<Ni3DDeviceDesc *> *devices ) {
	DDCAPS_DX7 driverCaps;
	DDCAPS_DX7 helCaps;
	memset( &driverCaps, 0, sizeof( driverCaps ) );
	memset( &helCaps, 0, sizeof( helCaps ) );
	driverCaps.dwSize = sizeof( driverCaps );
	helCaps.dwSize = sizeof( helCaps );
	directDraw_->GetCaps( &driverCaps, &helCaps );
	if ( !driverCaps.dwCaps ) {
		return;
	}
	IDirect3D7 *direct3D;
	if ( directDraw_->QueryInterface( IID_IDirect3D7, (LPVOID *)&direct3D ) != DD_OK ) {
		return;
	}
	NiD3DDeviceEnumContext context;
	context.driverGuid = driverGuid;
	context.devices = devices;
	context.driverCaps = &driverCaps;
	context.directDraw = directDraw_;
	direct3D->EnumDevices( EnumDevicesCallback, &context );
	direct3D->Release();
}

/*
================
NiD3DDirectDraw::EnumDevicesCallback

Makes a display device of each Direct3D device: the hardware devices of
every driver, the software one only on the primary display. The primary
display's hardware devices go first.
================
*/
// FUNCTION: STBC 0x7C8870
HRESULT CALLBACK NiD3DDirectDraw::EnumDevicesCallback( LPSTR description, LPSTR name, LPD3DDEVICEDESC7 desc, LPVOID context ) {
	NiD3DDeviceEnumContext *enumContext = (NiD3DDeviceEnumContext *)context;
	if ( !desc || !context ) {
		return D3DENUMRET_CANCEL;
	}
	if ( ( desc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION ) || !enumContext->driverGuid ) {
		bool hardware = false;
		unsigned int flags = 0;
		DWORD renderBitDepths = 0;
		const GUID *deviceGuid = NULL;
		if ( desc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION ) {
			if ( desc->dwDevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) {
				deviceGuid = &IID_IDirect3DTnLHalDevice;
				flags |= DEVICE_DESC_TRANSFORM_AND_LIGHT;
			} else {
				deviceGuid = &IID_IDirect3DHALDevice;
			}
			flags |= ( desc->dwDevCaps & D3DDEVCAPS_SEPARATETEXTUREMEMORIES ) ? DEVICE_DESC_SEPARATE_TEXTURE_MEMORIES : 0;
			flags |= ( desc->dwDeviceRenderBitDepth & DDBD_32 ) ? DEVICE_DESC_RENDERS_32_BIT : 0;
			renderBitDepths = desc->dwDeviceRenderBitDepth;
			hardware = true;
		} else {
			if ( !desc->dwDevCaps ) {
				return D3DENUMRET_OK;
			}
			deviceGuid = &IID_IDirect3DRGBDevice;
			flags |= ( desc->dwDevCaps & D3DDEVCAPS_SEPARATETEXTUREMEMORIES ) ? DEVICE_DESC_SEPARATE_TEXTURE_MEMORIES : 0;
			flags |= ( desc->dwDeviceRenderBitDepth & DDBD_32 ) ? DEVICE_DESC_RENDERS_32_BIT : 0;
			renderBitDepths = desc->dwDeviceRenderBitDepth;
			hardware = false;
		}

		bool windowed = !enumContext->driverGuid && ( enumContext->driverCaps->dwCaps2 & DDCAPS2_CANRENDERWINDOWED );

		DDDEVICEIDENTIFIER2 identifier;
		enumContext->directDraw->GetDeviceIdentifier( &identifier, 0 );
		char *deviceName = new char[strlen( name ) + strlen( identifier.szDescription ) + 16];
		sprintf( deviceName, "%s on %s", name, identifier.szDescription );
		Ni3DDeviceDesc *device = new Ni3DDeviceDesc( deviceName, windowed, true, hardware,
			enumContext->driverGuid, deviceGuid, flags );
		delete [] deviceName;

		NiD3DModeEnumContext modeContext;
		modeContext.device = device;
		modeContext.renderBitDepths = renderBitDepths;
		enumContext->directDraw->EnumDisplayModes( 0, 0, &modeContext, EnumModesCallback );

		if ( !enumContext->driverGuid && hardware ) {
			enumContext->devices->AddHead( device );
		} else {
			enumContext->devices->AddTail( device );
		}
	}
	return D3DENUMRET_OK;
}

/*
================
NiD3DDirectDraw::EnumDriversCallbackEx
================
*/
// FUNCTION: STBC 0x7C8C70
BOOL WINAPI NiD3DDirectDraw::EnumDriversCallbackEx( GUID *guid, LPSTR description, LPSTR name, LPVOID context, HMONITOR monitor ) {
	IDirectDraw7 *driver;
	if ( LoadDirectDrawCreateEx() && directDrawCreateEx( guid, (LPVOID *)&driver, IID_IDirectDraw7, 0 ) == DD_OK ) {
		EnumerateDevices( driver, guid, (NiTList<Ni3DDeviceDesc *> *)context );
		driver->Release();
	}
	return DDENUMRET_OK;
}

/*
================
NiD3DDirectDraw::EnumDriversCallback
================
*/
// FUNCTION: STBC 0x7C8D90
BOOL WINAPI NiD3DDirectDraw::EnumDriversCallback( GUID *guid, LPSTR description, LPSTR name, LPVOID context ) {
	IDirectDraw7 *driver;
	if ( LoadDirectDrawCreateEx() && directDrawCreateEx( guid, (LPVOID *)&driver, IID_IDirectDraw7, 0 ) == DD_OK ) {
		EnumerateDevices( driver, guid, (NiTList<Ni3DDeviceDesc *> *)context );
		driver->Release();
	}
	return DDENUMRET_OK;
}

/*
================
NiD3DDirectDraw::GetDisplayDevices

Enumerates the drivers and their devices on the first call, and picks the
software, windowed and fullscreen devices: the windowed device is on the
primary display, preferring hardware transform and lighting, then hardware
rasterization; the fullscreen device prefers the same, on any driver.
================
*/
// FUNCTION: STBC 0x7C8EB0
NiTList<Ni3DDeviceDesc *> *NiD3DDirectDraw::GetDisplayDevices() {
	if ( !displayDevices ) {
		displayDevices = new NiTList<Ni3DDeviceDesc *>( 2 );

		// every driver, through DirectDrawEnumerateEx when DDRAW.DLL has it
		HMODULE module = GetModuleHandle( "DDRAW.DLL" );
		if ( module ) {
			LPDIRECTDRAWENUMERATEEXA enumerateEx = (LPDIRECTDRAWENUMERATEEXA)GetProcAddress( module, "DirectDrawEnumerateExA" );
			if ( enumerateEx ) {
				enumerateEx( EnumDriversCallbackEx, displayDevices,
					DDENUM_ATTACHEDSECONDARYDEVICES | DDENUM_DETACHEDSECONDARYDEVICES | DDENUM_NONDISPLAYDEVICES );
			} else {
				DirectDrawEnumerateA( EnumDriversCallback, displayDevices );
			}
		} else {
			DirectDrawEnumerateA( EnumDriversCallback, displayDevices );
		}

		NiTListIterator pos = displayDevices->GetHeadPos();
		while ( pos ) {
			Ni3DDeviceDesc *device = displayDevices->GetNext( pos );
			if ( !device->IsHardware() ) {
				softwareDevice = device;
				break;
			}
		}

		pos = displayDevices->GetHeadPos();
		while ( pos ) {
			Ni3DDeviceDesc *device = displayDevices->GetNext( pos );
			if ( device->windowed && !device->driverGuid ) {
				if ( device->flags & DEVICE_DESC_TRANSFORM_AND_LIGHT ) {
					windowedDevice = device;
					break;
				}
				if ( device->hardware ) {
					windowedDevice = device;
				}
			}
		}

		if ( windowedDevice && windowedDevice->fullscreen ) {
			fullscreenDevice = windowedDevice;
		}

		pos = displayDevices->GetHeadPos();
		while ( pos ) {
			Ni3DDeviceDesc *device = displayDevices->GetNext( pos );
			if ( device->fullscreen ) {
				if ( !device->driverGuid ) {
					if ( device->flags & DEVICE_DESC_TRANSFORM_AND_LIGHT ) {
						fullscreenDevice = device;
						break;
					}
					if ( device->hardware && !( fullscreenDevice->flags & DEVICE_DESC_TRANSFORM_AND_LIGHT ) ) {
						fullscreenDevice = device;
					}
				} else if ( !device->windowed && !( fullscreenDevice->flags & DEVICE_DESC_TRANSFORM_AND_LIGHT ) ) {
					fullscreenDevice = device;
				}
			}
		}
	}
	return displayDevices;
}

/*
================
NiD3DDirectDraw::SetDisplayMode

Fullscreen, takes the exclusive level (the multi-monitor one when there
are several DirectDraw objects) and sets a mode the device lists; in a
window, takes the normal level and reads the display's bit depth.
================
*/
// FUNCTION: STBC 0x7C9020
bool NiD3DDirectDraw::SetDisplayMode( HWND window_, bool fullscreen_, unsigned int width, unsigned int height, unsigned int bitDepth_ ) {
	if ( displayModeSet ) {
		return true;
	}
	window = window_;

	if ( fullscreen_ ) {
		if ( !window_ ) {
			NiD3DRender::Error( "Creation failed: Fullscreen renderer requires a non-NULL top-level window handle" );
			return false;
		}
		if ( directDraws.GetSize() <= 1 ) {
			if ( directDraw->SetCooperativeLevel( window_, DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE
					| DDSCL_MULTITHREADED ) != DD_OK ) {
				NiD3DRender::Error( "Creation failed: Set Cooperative Level failed" );
				return false;
			}
		} else {
			// several displays: the multi-monitor exclusive level
			if ( directDraw->SetCooperativeLevel( window_, DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE
					| DDSCL_SETFOCUSWINDOW | DDSCL_CREATEDEVICEWINDOW | DDSCL_MULTITHREADED ) != DD_OK ) {
				NiD3DRender::Error( "Creation failed: Set Multimon Cooperative Level failed" );
				return false;
			}
		}
		displayModeSet = true;

		if ( displayDevice ) {
			bool supported = false;
			NiTListIterator pos = displayDevice->modes->GetHeadPos();
			while ( pos ) {
				NiD3DDisplayMode *mode = displayDevice->modes->GetNext( pos );
				if ( mode->height == height && mode->width == width && mode->bitDepth == bitDepth_ ) {
					supported = true;
					break;
				}
			}
			if ( !supported ) {
				NiD3DRender::Error( "Creation failed: Desired fullscreen display mode not supported" );
				goto failed;
			}
		}

		if ( directDraw->SetDisplayMode( width, height, bitDepth_, 0, 0 ) != DD_OK ) {
			NiD3DRender::Error( "Creation failed: Set Display Mode failed" );
			goto failed;
		}
		bitDepth = bitDepth_;
		fullscreen = true;
		return true;
	}

	if ( directDraw->SetCooperativeLevel( window_, DDSCL_NORMAL | DDSCL_MULTITHREADED ) != DD_OK ) {
		NiD3DRender::Error( "Creation failed: Set Cooperative Level failed" );
		goto failed;
	}
	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	displayModeSet = true;
	desc.dwSize = sizeof( desc );
	if ( directDraw->GetDisplayMode( &desc ) != DD_OK ) {
		NiD3DRender::Error( "Creation failed: Get Display Mode failed" );
		goto failed;
	}
	bitDepth = desc.ddpfPixelFormat.dwRGBBitCount;
	return true;

failed:
	// back to the normal level
	directDraw->SetCooperativeLevel( window, DDSCL_NORMAL | DDSCL_MULTITHREADED );
	return false;
}

/*
================
NiD3DDirectDraw::RestoreDisplayMode
================
*/
// FUNCTION: STBC 0x7C91E0
void NiD3DDirectDraw::RestoreDisplayMode() {
	if ( fullscreen ) {
		if ( directDraw->RestoreDisplayMode() != DD_OK ) {
			NiD3DRender::Warning( "(Shutdown): Restore Display Mode failed" );
		}
		if ( directDraw->SetCooperativeLevel( window, DDSCL_NORMAL | DDSCL_MULTITHREADED ) != DD_OK ) {
			NiD3DRender::Warning( "(Shutdown): Set Cooperative Level failed" );
		}
	}
	fullscreen = false;
}

/*
================
NiD3DDirectDraw::SetPrimarySurface

Only the first surface is kept; without an image, one is made of it.
================
*/
// FUNCTION: STBC 0x7C9230
void NiD3DDirectDraw::SetPrimarySurface( IDirectDrawSurface7 *surface, NiImage *image ) {
	if ( primarySurface || !surface ) {
		return;
	}
	primarySurface = surface;
	surface->AddRef();
	if ( image ) {
		primaryImage = image;
	} else {
		primaryImage = NiDDBufferImage::Create( primarySurface, directDraw );
	}
}

/*
================
Ni3DDeviceDesc::AddMode
================
*/
// FUNCTION: STBC 0x7C9330
void Ni3DDeviceDesc::AddMode( unsigned int width, unsigned int height, unsigned int bitDepth ) {
	NiD3DDisplayMode *mode = new NiD3DDisplayMode;
	mode->width = width;
	mode->height = height;
	mode->bitDepth = bitDepth;
	modes->AddTail( mode );
}
