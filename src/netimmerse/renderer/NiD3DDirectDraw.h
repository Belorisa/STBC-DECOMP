/*
===============================================================================

	NiD3DDirectDraw.h

	The renderer's DirectDraw encapsulation object: the DirectDraw 7 object
	of one display device, shared by the frame buffers that render on it;
	and the device descriptions the DirectDraw enumeration finds.

	Evidence: the creation functions (0x7C7F80 and 0x7C8360, "(Creation):
	Attempting to initialize DirectDraw device") allocate 0x1A4 bytes; the
	destructor (0x7C7D90) logs "(Shutdown): Releasing DirectDraw
	encapsulation object"; the owners release it through the reference count
	at 0x1A0 and delete it at zero. The class name is invented from the log.

	A device description (Ni3DDeviceDesc) is 0x1C bytes with a virtual
	destructor (vtable 0x88B9F8, one slot: the scalar deleting destructor,
	whose retail copy 0x462A90 a game unit emitted). The class name is the
	SWIG type of GraphicsModeInfo's description ("_p_Ni3DDeviceDesc",
	GraphicsModeInfo_GetDesc), and GraphicsModeInfo::operator= (0x462240)
	builds an object of this vtable for it; Gamebryo's later DirectX 8
	renderer has the same role under NiDX8DeviceDesc. The Direct3D device
	enumeration callback (0x7C8870) builds one for each Direct3D device of
	each DirectDraw driver and names it "%s on %s" (the Direct3D device on
	the driver); the DirectDraw object keeps its own copy (0x7C8055), made by
	the inline copy constructor. Its display modes are the 0x0C-byte width,
	height and bit depth that SetDisplayMode (0x7C9020) searches before a
	fullscreen mode change; the display mode class name is invented.

===============================================================================
*/

#ifndef __NID3DDIRECTDRAW_H__
#define __NID3DDIRECTDRAW_H__

#include <string.h>

#include "NiPointer.h"
#include "NiTList.h"
#include "NiImage.h"
#include "NiD3D7.h"

/*
================
NiD3DDisplayMode

A fullscreen mode a display device supports.
================
*/
class NiD3DDisplayMode {
public:
						NiD3DDisplayMode() { width = height = bitDepth = 0; }

	unsigned int		width;
	unsigned int		height;
	unsigned int		bitDepth;		// 16 or 32
};

// Ni3DDeviceDesc::flags
#define DEVICE_DESC_SEPARATE_TEXTURE_MEMORIES	0x01	// D3DDEVCAPS_SEPARATETEXTUREMEMORIES
#define DEVICE_DESC_RENDERS_32_BIT				0x02	// DDBD_32 among the render bit depths
#define DEVICE_DESC_TRANSFORM_AND_LIGHT			0x04	// the hardware transform and lighting device

/*
================
Ni3DDeviceDesc

The description of a Direct3D device on a DirectDraw driver.
================
*/
class Ni3DDeviceDesc {
public:
							// an empty device, to copy one into (the movie manager's)
							Ni3DDeviceDesc() : name( 0 ), windowed( false ), fullscreen( false ), hardware( false ),
												   driverGuid( 0 ), deviceGuid( 0 ), flags( 0 ),
												   modes( new NiTList<NiD3DDisplayMode *>( 2 ) ) {}
							Ni3DDeviceDesc( const char *name_, bool windowed_, bool fullscreen_, bool hardware_,
												const GUID *driverGuid_, const GUID *deviceGuid_, unsigned int flags_ );
							Ni3DDeviceDesc( const Ni3DDeviceDesc &device );
	virtual					~Ni3DDeviceDesc();

	Ni3DDeviceDesc &	operator=( const Ni3DDeviceDesc &device );

	// adds a fullscreen mode (name invented)
	void					AddMode( unsigned int width, unsigned int height, unsigned int bitDepth );
	// renders in hardware (name invented)
	bool					IsHardware() const { return hardware; }
	// the name (name invented; AnimOnly.exe 0x513C20)
	const char *			GetName() const { return name; }
	// (names invented; AnimOnly.exe 0x5130C0, 0x5130E0, 0x513100, 0x513120, called by
	// NiD3DRender::SelectDevice's debug copy)
	bool					IsWindowed() const { return windowed; }
	bool					IsFullscreen() const { return fullscreen; }
	const GUID *			GetDeviceGuid() const { return deviceGuid; }
	unsigned int			GetFlags() const { return flags; }

	char *					name;			// "%s on %s": the Direct3D device on the driver
	bool					windowed;		// the primary driver, and it can render into a window
	bool					fullscreen;		// can render fullscreen (every enumerated device)
	bool					hardware;		// D3DDEVCAPS_HWRASTERIZATION
	GUID *					driverGuid;		// the DirectDraw driver; NULL for the primary display
	const GUID *			deviceGuid;		// the Direct3D device type: IID_IDirect3DTnLHalDevice, IID_IDirect3DHALDevice or IID_IDirect3DRGBDevice
	unsigned int			flags;			// DEVICE_DESC_*
	NiTList<NiD3DDisplayMode *> *	modes;	// the fullscreen modes
};

/*
================
Ni3DDeviceDesc::Ni3DDeviceDesc
================
*/
inline Ni3DDeviceDesc::Ni3DDeviceDesc( const char *name_, bool windowed_, bool fullscreen_, bool hardware_,
												const GUID *driverGuid_, const GUID *deviceGuid_, unsigned int flags_ )
	: modes( new NiTList<NiD3DDisplayMode *>( 2 ) ) {
	name = 0;
	unsigned int length = strlen( name_ ) + 1;
	name = new char[length];
	memcpy( name, name_, length );
	windowed = windowed_;
	fullscreen = fullscreen_;
	hardware = hardware_;
	if ( driverGuid_ ) {
		driverGuid = new GUID;
		*driverGuid = *driverGuid_;
	} else {
		driverGuid = 0;
	}
	deviceGuid = deviceGuid_;
	flags = flags_;
}

/*
================
Ni3DDeviceDesc::Ni3DDeviceDesc

A deep copy: the name, the driver GUID and every mode.
================
*/
inline Ni3DDeviceDesc::Ni3DDeviceDesc( const Ni3DDeviceDesc &device ) {
	if ( device.name ) {
		unsigned int length = strlen( device.name ) + 1;
		name = new char[length];
		memcpy( name, device.name, length );
	} else {
		name = 0;
	}
	windowed = device.windowed;
	fullscreen = device.fullscreen;
	hardware = device.hardware;
	if ( device.driverGuid ) {
		driverGuid = new GUID;
		*driverGuid = *device.driverGuid;
	} else {
		driverGuid = 0;
	}
	deviceGuid = device.deviceGuid;
	flags = device.flags;
	if ( device.modes ) {
		modes = new NiTList<NiD3DDisplayMode *>( 2 );
		NiTListIterator pos = device.modes->GetHeadPos();
		while ( pos ) {
			modes->AddTail( new NiD3DDisplayMode( *device.modes->GetNext( pos ) ) );
		}
	} else {
		modes = 0;
	}
}

/*
================
Ni3DDeviceDesc::operator=

A deep copy: frees the name, the driver GUID and the modes, then copies
the device's.
================
*/
// FUNCTION: STBC 0x7C0700 ??4Ni3DDeviceDesc@@QAEAAV0@ABV0@@Z
inline Ni3DDeviceDesc &Ni3DDeviceDesc::operator=( const Ni3DDeviceDesc &device ) {
	delete [] name;
	name = 0;
	if ( device.name ) {
		unsigned int length = strlen( device.name ) + 1;
		name = new char[length];
		memcpy( name, device.name, length );
	}
	hardware = device.hardware;
	windowed = device.windowed;
	fullscreen = device.fullscreen;
	if ( driverGuid ) {
		delete driverGuid;
		driverGuid = 0;
	}
	if ( device.driverGuid ) {
		driverGuid = new GUID;
		*driverGuid = *device.driverGuid;
	}
	deviceGuid = device.deviceGuid;
	flags = device.flags;
	if ( modes ) {
		while ( !modes->IsEmpty() ) {
			delete modes->RemoveHead();
		}
		delete modes;
		modes = 0;
	}
	if ( device.modes ) {
		modes = new NiTList<NiD3DDisplayMode *>( 2 );
		NiTListIterator pos = device.modes->GetHeadPos();
		while ( pos ) {
			modes->AddTail( new NiD3DDisplayMode( *device.modes->GetNext( pos ) ) );
		}
	}
	return *this;
}

/*
================
Ni3DDeviceDesc::~Ni3DDeviceDesc

The modes are deleted twice over: the second pass finds the list empty
(an original quirk).
================
*/
// FUNCTION: STBC 0x462A90 ??_GNi3DDeviceDesc@@UAEPAXI@Z
inline Ni3DDeviceDesc::~Ni3DDeviceDesc() {
	delete [] name;
	name = 0;
	if ( driverGuid ) {
		delete driverGuid;
	}
	while ( !modes->IsEmpty() ) {
		delete modes->RemoveHead();
	}
	if ( modes ) {
		while ( !modes->IsEmpty() ) {
			delete modes->RemoveHead();
		}
	}
	delete modes;
	modes = 0;
}

// what EnumDevicesCallback learns about the driver it enumerates (name
// invented)
struct NiD3DDeviceEnumContext {
	NiTList<Ni3DDeviceDesc *> *	devices;	// the list to add to
	GUID *				driverGuid;		// NULL for the primary display
	DDCAPS_DX7 *		driverCaps;
	IDirectDraw7 *		directDraw;		// created on the driver
};

// what EnumModesCallback adds to (name invented)
struct NiD3DModeEnumContext {
	Ni3DDeviceDesc *	device;
	DWORD				renderBitDepths;	// the device's DDBD_* render depths
};

// DirectDrawCreateEx, looked up in DDRAW.DLL (the type name is invented;
// ddraw.h declares the function only)
typedef HRESULT (WINAPI *DirectDrawCreateExFunction)( GUID *guid, LPVOID *directDraw, REFIID iid, IUnknown *outer );

class NiD3DDirectDraw {
public:
							NiD3DDirectDraw();
							~NiD3DDirectDraw();

	// a frame buffer's reference; the last one released deletes the object
	void					IncRefCount() { refCount++; }
	void					DecRefCount() { if ( --refCount == 0 ) delete this; }

	// the total and free texture memory (name invented)
	bool					GetTextureMemory( unsigned int *total, unsigned int *free ) const;

	// creates the object on a display device (0x7C7F80, "(Creation):
	// Attempting to initialize DirectDraw device"), on the default device
	// (0x7C7F60, which tests its argument but takes the windowed device
	// either way), or around a DirectDraw object the application made
	// (0x7C8360); names invented
	static NiD3DDirectDraw *	Create( Ni3DDeviceDesc *device );
	static NiD3DDirectDraw *	CreateDefault( bool unknown );
	static NiD3DDirectDraw *	CreateFromDirectDraw( IDirectDraw7 *directDraw_, Ni3DDeviceDesc *device );

	// sets the cooperative level and, fullscreen, the display mode, once
	// (name invented)
	bool					SetDisplayMode( HWND window_, bool fullscreen_, unsigned int width, unsigned int height, unsigned int bitDepth_ );
	// the display mode's depth (name invented; AnimOnly.exe 0x513F80, which
	// NiD3DFrameBuffer's surface creation and NiD3DRender::Initialize call)
	unsigned int			GetBitDepth() const { return bitDepth; }
	// leaves fullscreen: restores the display mode and the normal
	// cooperative level (name invented)
	void					RestoreDisplayMode();
	// keeps the primary surface and an image of it (names invented)
	void					SetPrimarySurface( IDirectDrawSurface7 *surface, NiImage *image );
	NiImage *				GetPrimaryImage() const { return primaryImage; }
	// the primary surface and the DirectDraw object, as the frame buffer
	// and the renderer read them (AnimOnly.exe 0x51EDB0 and 0x513500;
	// names invented)
	IDirectDrawSurface7 *	GetPrimarySurface() const { return primarySurface; }
	IDirectDraw7 *			GetDirectDraw() const { return directDraw; }
	// the device this object was created on (name invented)
	Ni3DDeviceDesc *	GetDisplayDevice() const { return displayDevice; }

	// the display devices, enumerated on the first call (names invented)
	static NiTList<Ni3DDeviceDesc *> *	GetDisplayDevices();
	// the preferred devices, enumerating first when needed (names invented)
	static Ni3DDeviceDesc *	GetFullscreenDevice();
	static Ni3DDeviceDesc *	GetWindowedDevice();
	static Ni3DDeviceDesc *	GetSoftwareDevice();
	static bool				LoadDirectDrawCreateEx();
	static void				EnumerateDevices( IDirectDraw7 *directDraw_, GUID *driverGuid, NiTList<Ni3DDeviceDesc *> *devices );

	// the enumeration callbacks: DirectDrawEnumerateEx's and
	// DirectDrawEnumerate's for the drivers, EnumDevices' for the Direct3D
	// devices of a driver and EnumDisplayModes' for its modes (names
	// invented)
	static BOOL WINAPI		EnumDriversCallbackEx( GUID *guid, LPSTR description, LPSTR name, LPVOID context, HMONITOR monitor );
	static BOOL WINAPI		EnumDriversCallback( GUID *guid, LPSTR description, LPSTR name, LPVOID context );
	static HRESULT CALLBACK	EnumDevicesCallback( LPSTR description, LPSTR name, LPD3DDEVICEDESC7 desc, LPVOID context );
	static HRESULT CALLBACK	EnumModesCallback( LPDDSURFACEDESC2 desc, LPVOID context );

	IDirectDrawSurface7 *	primarySurface;	// "(Shutdown): Release of DirectDraw Primary Surface returned a refcount of %d"
	NiPointer<NiImage>		primaryImage;	// the primary surface's image (an NiDDBufferImage)
	bool					fullscreen;		// the display mode and the exclusive level were set
	IDirectDraw7 *			directDraw;		// "Creation failed: Could not query Direct3D from DirectDraw"
	bool					hardware;		// the driver has 3D capabilities ("Using DirectDraw software" otherwise)
	DDCAPS_DX7				caps;			// of the driver, or of the software emulation
	Ni3DDeviceDesc *	displayDevice;	// a copy of the device this object was created on
	unsigned int			bitDepth;		// of the display mode
	bool					displayModeSet;	// SetDisplayMode ran
	HWND					window;			// the cooperative level's window
	int						refCount;		// deleted when the last frame buffer lets go

	static NiTList<NiD3DDirectDraw *>	directDraws;	// every live object
	static NiTList<Ni3DDeviceDesc *> *	displayDevices;
	static Ni3DDeviceDesc *	fullscreenDevice;	// the preferred fullscreen device
	static Ni3DDeviceDesc *	windowedDevice;		// the preferred device on the primary display
	static Ni3DDeviceDesc *	softwareDevice;		// the first device without hardware rasterization
	static DirectDrawCreateExFunction	directDrawCreateEx;
};

/*
================
NiD3DDirectDraw::GetTextureMemory
================
*/
inline bool NiD3DDirectDraw::GetTextureMemory( unsigned int *total, unsigned int *free ) const {
	DDSCAPS2 caps;
	memset( &caps, 0, sizeof( caps ) );
	caps.dwCaps = DDSCAPS_TEXTURE;
	if ( directDraw->GetAvailableVidMem( &caps, (LPDWORD)total, (LPDWORD)free ) != DD_OK ) {
		*free = 0;
		*total = 0;
		return false;
	}
	return true;
}

/*
================
NiD3DDirectDraw::GetFullscreenDevice
================
*/
inline Ni3DDeviceDesc *NiD3DDirectDraw::GetFullscreenDevice() {
	if ( !fullscreenDevice ) {
		GetDisplayDevices();
	}
	return fullscreenDevice;
}

/*
================
NiD3DDirectDraw::GetWindowedDevice
================
*/
inline Ni3DDeviceDesc *NiD3DDirectDraw::GetWindowedDevice() {
	if ( !windowedDevice ) {
		GetDisplayDevices();
	}
	return windowedDevice;
}

/*
================
NiD3DDirectDraw::GetSoftwareDevice
================
*/
inline Ni3DDeviceDesc *NiD3DDirectDraw::GetSoftwareDevice() {
	if ( !softwareDevice ) {
		GetDisplayDevices();
	}
	return softwareDevice;
}

#endif /* !__NID3DDIRECTDRAW_H__ */
