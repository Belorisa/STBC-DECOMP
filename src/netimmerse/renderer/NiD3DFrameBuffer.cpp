// NiD3DFrameBuffer.cpp -- the renderer's frame buffers

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiD3DFrameBuffer.h"
#include "NiD3DDirectDraw.h"
#include "NiD3DRender.h"
#include "NiDDImage.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007cb210 _$E30
// FUNCTION: STBC 0x007cb220 _$E68
// FUNCTION: STBC 0x007cb230 _$E44
// FUNCTION: STBC 0x007cb240 _$E57
// FUNCTION: STBC 0x007cb250 _$E72
// FUNCTION: STBC 0x007cb260 _$E76
// FUNCTION: STBC 0x007cb270 _$E80
// FUNCTION: STBC 0x007cb280 _$E84
// FUNCTION: STBC 0x007cb290 _$E188
// FUNCTION: STBC 0x007cb2a0 _$E193
// FUNCTION: STBC 0x007cb2b0 _$E198

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007cb1f0 _$E219
// FUNCTION: STBC 0x007caee0 _$E17
// FUNCTION: STBC 0x007caf00 _$E15
// FUNCTION: STBC 0x007caf10 _$E23
// FUNCTION: STBC 0x007caf30 _$E21

/*
================
NiD3DFrameBuffer::CreateFlippingSurfaces

Creates the fullscreen flipping chain, with two back buffers when asked and
the driver can, else with one, and keeps its back buffer.
================
*/
// FUNCTION: STBC 0x7C96C0
bool NiD3DFrameBuffer::CreateFlippingSurfaces( bool tripleBuffer ) {
	assert( !directDraw->GetPrimarySurface() );
	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
	IDirectDrawSurface7 *surface = 0;
	if ( tripleBuffer ) {
		desc.dwBackBufferCount = 2;
		if ( directDraw->GetDirectDraw()->CreateSurface( &desc, &surface, 0 ) != DD_OK ) {
			surface = 0;
		}
	}
	if ( !surface ) {
		desc.dwBackBufferCount = 1;
		if ( directDraw->GetDirectDraw()->CreateSurface( &desc, &surface, 0 ) != DD_OK ) {
			NiD3DRender::Error( "Creation failed: Error creating fullscreen frontbuffer" );
			return false;
		}
	}
	frontSurface = surface;
	directDraw->SetPrimarySurface( surface, 0 );

	GetSurfaceDesc( frontSurface, &desc );
	if ( desc.dwBackBufferCount > 1 ) {
		NiD3DRender::Message( "(Creation): Using Triple Buffering" );
	}

	DDSCAPS2 surfaceCaps;	// (name invented; fits the /Od frame order)
	memset( &surfaceCaps, 0, sizeof( surfaceCaps ) );
	surfaceCaps.dwCaps = DDSCAPS_BACKBUFFER;
	if ( frontSurface->GetAttachedSurface( &surfaceCaps, &backSurface ) != DD_OK ) {
		NiD3DRender::Error( "Creation failed: Error creating fullscreen backbuffer" );
		return false;
	}
	return true;
}

/*
================
NiD3DFrameBuffer::CreateWindowedFrontSurface

A window renders into the display's primary surface, shared through the
DirectDraw object. The debug copy's if/else with one return (AnimOnly.exe
0x51D424, DDFramebuffer.cpp).
================
*/
// FUNCTION: STBC 0x7C9820
bool NiD3DFrameBuffer::CreateWindowedFrontSurface() {
	if ( directDraw->GetPrimarySurface() ) {
		frontSurface = directDraw->GetPrimarySurface();
		frontSurface->AddRef();
	} else {
		DDSURFACEDESC2 desc;
		memset( &desc, 0, sizeof( desc ) );
		desc.dwSize = sizeof( desc );
		desc.dwFlags = DDSD_CAPS;
		desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		IDirectDrawSurface7 *surface;
		if ( directDraw->GetDirectDraw()->CreateSurface( &desc, &surface, 0 ) != DD_OK ) {
			NiD3DRender::Error( "Creation failed: Onscreen Front Buffer Surface Creation failed" );
			return false;
		}
		directDraw->SetPrimarySurface( surface, 0 );
		frontSurface = surface;
	}
	return true;
}

/*
================
NiD3DFrameBuffer::CreateBackSurface

An offscreen back surface the size of the frame buffer, in local video
memory or in system memory, optionally usable as a texture. A texture in
video memory that the driver put in nonlocal (AGP) memory fails.
================
*/
// FUNCTION: STBC 0x7C98C0
bool NiD3DFrameBuffer::CreateBackSurface( bool videoMemory, bool texture, IDirect3DDevice7 *device ) {
	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	desc.dwWidth = width;
	desc.dwHeight = height;
	desc.ddsCaps.dwCaps = videoMemory ? DDSCAPS_3DDEVICE | DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM
		: DDSCAPS_3DDEVICE | DDSCAPS_SYSTEMMEMORY;
	if ( texture ) {
		desc.ddsCaps.dwCaps |= DDSCAPS_TEXTURE;
	}
	IDirectDrawSurface7 *surface;
	if ( directDraw->directDraw->CreateSurface( &desc, &surface, 0 ) != DD_OK ) {
		NiD3DRender::Error( "Creation failed: Back Buffer Surface Creation failed" );
		return false;
	}

	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	surface->GetSurfaceDesc( &desc );
	if ( videoMemory && texture && ( desc.ddsCaps.dwCaps & DDSCAPS_NONLOCALVIDMEM ) ) {
		NiD3DRender::Error( "Creation failed: Offscreen Video RAM Back Buffer Surface Creation failed" );
		surface->Release();
		return false;
	}
	backSurface = surface;
	return true;
}

/*
================
NiD3DFrameBuffer::CreateZBuffer

Takes the z-buffer already attached to the back surface, else creates one
in the given format and attaches it.
================
*/
// FUNCTION: STBC 0x7C99E0
bool NiD3DFrameBuffer::CreateZBuffer( bool videoMemory, const DDPIXELFORMAT *format ) {
	HRESULT result;
	IDirectDrawSurface7 *surface = 0;
	if ( !format->dwSize ) {
		NiD3DRender::Error( "Creation failed: 3D Hardware does not support a Z-buffer format" );
		return false;
	}
	if ( backSurface ) {
		DDSCAPS2 caps;
		memset( &caps, 0, sizeof( caps ) );
		caps.dwCaps = DDSCAPS_ZBUFFER;
		result = backSurface->GetAttachedSurface( &caps, &surface );
		if ( result != DD_OK && result != DDERR_NOTFOUND ) {
			NiD3DRender::Error( "Creation failed: Error searching back buffer for Z-buffer" );
			return false;
		}
		if ( result != DDERR_NOTFOUND ) {
			NiD3DRender::Message( "Render create info: Found attached Z-Buffer" );
			unknown04 |= 0x20;
		} else {
			DDSURFACEDESC2 desc;
			memset( &desc, 0, sizeof( desc ) );
			desc.dwSize = sizeof( desc );
			desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
			desc.dwWidth = width;
			desc.dwHeight = height;
			memcpy( &desc.ddpfPixelFormat, format, sizeof( DDPIXELFORMAT ) );
			desc.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | ( videoMemory ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY );
			if ( directDraw->GetDirectDraw()->CreateSurface( &desc, &surface, 0 ) != DD_OK ) {
				NiD3DRender::Error( "Creation failed: ZBuffer Surface Creation failed" );
				return false;
			}
			unknown04 &= ~0x20;
			if ( backSurface->AddAttachedSurface( surface ) != DD_OK ) {
				NiD3DRender::Error( "Creation failed: ZBuffer Surface Attachment failed" );
				return false;
			}
		}
	}
	zBuffer = surface;
	return true;
}

/*
================
NiD3DFrameBuffer::ReleaseZBuffer

Fails without a back surface, or when the z-buffer was never attached.
================
*/
// FUNCTION: STBC 0x7C9B70
bool NiD3DFrameBuffer::ReleaseZBuffer() {
	if ( !backSurface || ( unknown04 & 0x20 ) ) {
		return false;
	}
	if ( !zBuffer ) {
		return true;
	}
	if ( backSurface->DeleteAttachedSurface( 0, zBuffer ) != DD_OK ) {
		return false;
	}
	zBuffer->Release();
	zBuffer = 0;
	return true;
}

/*
================
NiD3DFrameBuffer::CreateClipper
================
*/
// FUNCTION: STBC 0x7C9BB0
bool NiD3DFrameBuffer::CreateClipper() {
	if ( directDraw->GetDirectDraw()->CreateClipper( 0, &clipper, 0 ) != DD_OK ) {
		NiD3DRender::Warning( "Creation failed: Clipper Creation failed" );
		NiD3DRender::Error( "Creation failed: Clipper Creation failed" );
		return false;
	}
	if ( clipper->SetHWnd( 0, window ) != DD_OK ) {
		NiD3DRender::Warning( "Creation failed: Clipper Window Attach failed" );
		NiD3DRender::Error( "Creation failed: Clipper Creation failed" );
		return false;
	}
	if ( frontSurface->SetClipper( clipper ) != DD_OK ) {
		NiD3DRender::Warning( "Creation failed: Frontbuffer SetClipper failed" );
		NiD3DRender::Error( "Creation failed: Clipper Creation failed" );
		return false;
	}
	return true;
}

/*
================
NiD3DFrameBuffer::GetSurfaceSize
================
*/
// FUNCTION: STBC 0x7C9C50
bool NiD3DFrameBuffer::GetSurfaceSize( IDirectDrawSurface7 *surface, unsigned int *width_, unsigned int *height_ ) {
	DDSURFACEDESC2 desc;
	if ( !GetSurfaceDesc( surface, &desc ) ) {
		return false;
	}
	*width_ = desc.dwWidth;
	*height_ = desc.dwHeight;
	return true;
}

/*
================
NiD3DFrameBuffer::GetSurfaceDesc
================
*/
// FUNCTION: STBC 0x7C9CB0
bool NiD3DFrameBuffer::GetSurfaceDesc( IDirectDrawSurface7 *surface, DDSURFACEDESC2 *desc ) {
	memset( desc, 0, sizeof( *desc ) );
	desc->dwSize = sizeof( *desc );
	if ( surface->GetSurfaceDesc( desc ) != DD_OK ) {
		return false;
	}
	return true;
}

/*
================
NiD3DFrameBuffer::NiD3DFrameBuffer

Nothing is created here; the windowed blit waits for the vertical blank.

================
*/
// FUNCTION: STBC 0x7C9CE0
NiD3DFrameBuffer::NiD3DFrameBuffer() {
	window = 0;
	unknown04 = 0;
	width = 0;
	height = 0;
	frontSurface = 0;
	backSurface = 0;
	zBuffer = 0;
	clipper = 0;
	frontImage = backImage = 0;
	unknown50 = 0;
	unknown5C = false;
	memset( &bltFX, 0, sizeof( bltFX ) );
	bltFX.dwSize = sizeof( bltFX );
	bltFX.dwDDFX = DDBLTFX_NOTEARING;
	refCount = 1;
}

/*
================
NiD3DFrameBuffer::CreateDirectDraw
================
*/
// FUNCTION: STBC 0x7C9DF0 ?CreateDirectDraw@NiD3DFrameBuffer@@SAPAVNiD3DDirectDraw@@HPAVNi3DDeviceDesc@@@Z
NiD3DDirectDraw *NiD3DFrameBuffer::CreateDirectDraw( int unknown, Ni3DDeviceDesc *device ) {
	// the default device's flag: cleared for mode 1
	bool defaultFlag = true;
	if ( unknown == 1 ) {
		defaultFlag = false;
	}
	if ( device ) {
		return NiD3DDirectDraw::Create( device );
	}
	return NiD3DDirectDraw::CreateDefault( defaultFlag );
}

/*
================
NiD3DFrameBuffer::CreateDirectDraw
================
*/
// FUNCTION: STBC 0x7C9E30 ?CreateDirectDraw@NiD3DFrameBuffer@@SAPAVNiD3DDirectDraw@@PAUIDirectDraw7@@PAVNi3DDeviceDesc@@@Z
NiD3DDirectDraw *NiD3DFrameBuffer::CreateDirectDraw( IDirectDraw7 *directDraw, Ni3DDeviceDesc *device ) {
	return NiD3DDirectDraw::CreateFromDirectDraw( directDraw, device );
}

/*
================
NiD3DFrameBuffer::Create

The back surface goes in video memory when the display device rasterizes
in hardware.
================
*/

// FUNCTION: STBC 0x7C9E50
NiD3DFrameBuffer *NiD3DFrameBuffer::Create( NiD3DDirectDraw *directDraw_, int mode, unsigned int flags, unsigned int width_,
											unsigned int height_, HWND window_, HWND focusWindow, IDirect3DDevice7 *device ) {
	NiD3DRender::Message( "(Creation): Creating framebuffer object" );
	NiD3DFrameBuffer *frameBuffer = new NiD3DFrameBuffer;
	if ( !frameBuffer ) {
		return 0;
	}
	if ( !directDraw_ ) {
		goto failed;
	}
	frameBuffer->directDraw = directDraw_;
	directDraw_->IncRefCount();
	if ( !frameBuffer->CreateSurfaces( mode, flags, width_, height_, window_, focusWindow,
			directDraw_->GetDisplayDevice()->IsHardware(), device ) ) {
		goto failed;
	}
	if ( !frameBuffer->CreateImages() ) {
		goto failed;
	}
	NiD3DRender::Message( "(Creation): Framebuffer object creation succeeded" );
	return frameBuffer;

failed:
	delete frameBuffer;
	return 0;
}

/*
================
NiD3DFrameBuffer::CreateFromSurfaces
================
*/
// FUNCTION: STBC 0x7CA030
NiD3DFrameBuffer *NiD3DFrameBuffer::CreateFromSurfaces( NiD3DDirectDraw *directDraw_, int mode, HWND window_,
														IDirectDrawSurface7 *front, IDirectDrawSurface7 *back ) {
	NiD3DFrameBuffer *frameBuffer = new NiD3DFrameBuffer;
	if ( !frameBuffer ) {
		return 0;
	}
	if ( !directDraw_ ) {
		goto failed;
	}
	frameBuffer->directDraw = directDraw_;
	directDraw_->IncRefCount();
	if ( !frameBuffer->AttachSurfaces( mode, window_, front, back ) ) {
		goto failed;
	}
	return frameBuffer;

failed:
	delete frameBuffer;
	return 0;
}

/*
================
NiD3DFrameBuffer::CreateSurfaces

A window renders into the display's primary surface from an offscreen back
surface the size of its client area; fullscreen sets the display mode and
flips; an offscreen buffer is its own front surface. The frame buffer then
takes the back surface's pixel format. Cases in the debug copy's order, the
switch on the argument, the flipping depth and triple-buffer flag in locals
of their own (AnimOnly.exe 0x51DEEE), the depth read through GetBitDepth
(0x513F80), in the offscreen case too. The debug copy also clears one more
local it never reads.
================
*/
// FUNCTION: STBC 0x7CA1D0
bool NiD3DFrameBuffer::CreateSurfaces( int mode_, unsigned int flags, unsigned int width_, unsigned int height_,
										HWND window_, HWND focusWindow, bool videoMemory, IDirect3DDevice7 *device ) {
	unsigned int bitDepth;
	mode = mode_;
	switch ( mode_ ) {
	case FRAMEBUFFER_FLIPPING: {
		NiD3DRender::Message( "(Creation): Creating fullscreen flipping buffers" );
		window = 0;
		width = width_;
		height = height_;
		unsigned int depth;
		if ( flags & FRAMEBUFFER_32_BIT ) {
			depth = 32;
		} else {
			depth = 16;
		}
		if ( !directDraw->SetDisplayMode( focusWindow, true, width, height, depth ) ) {
			return false;
		}
		bool tripleBuffer = ( flags & FRAMEBUFFER_TRIPLE_BUFFER ) ? true : false;
		if ( !CreateFlippingSurfaces( tripleBuffer ) ) {
			return false;
		}
		break;
	}
	case FRAMEBUFFER_WINDOWED: {
		NiD3DRender::Message( "(Creation): Creating windowed rendering buffers" );
		if ( !window_ ) {
			NiD3DRender::Error( "(Creation): Windowed renderer requires a non-NULL window handle" );
			return false;
		}
		if ( !directDraw->SetDisplayMode( focusWindow, false, 0, 0, 0 ) ) {
			return false;
		}
		bitDepth = directDraw->GetBitDepth();
		if ( bitDepth != 16 && bitDepth != 32 ) {
			NiD3DRender::Error( "Creation failed: Windowed mode must be 16 or 32 bits" );
			return false;
		}
		window = window_;
		RECT rect;
		GetClientRect( window, &rect );
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		if ( !CreateWindowedFrontSurface() ) {
			return false;
		}
		if ( !CreateBackSurface( videoMemory, false, 0 ) ) {
			return false;
		}
		if ( !CreateClipper() ) {
			return false;
		}
		break;
	}
	case FRAMEBUFFER_OFFSCREEN:
		NiD3DRender::Message( "(Creation): Creating offscreen (texture) rendering buffer" );
		window = 0;
		width = width_;
		height = height_;
		bitDepth = directDraw->GetBitDepth();
		if ( !CreateBackSurface( videoMemory, true, device ) ) {
			return false;
		}
		frontSurface = backSurface;
		frontSurface->AddRef();
		break;
	}

	pixelFormat = NiDDPixelDesc( backSurface );
	NiDDPixelDescTable::Register( pixelFormat );
	NiD3DRender::Message( "(Creation): Rendering buffer creation successful" );
	return true;
}

/*
================
NiD3DFrameBuffer::AttachSurfaces

Takes the application's surfaces, holding a reference to each, with their
dimensions and the back surface's pixel format.
================
*/
// FUNCTION: STBC 0x7CA640
bool NiD3DFrameBuffer::AttachSurfaces( int mode_, HWND window_, IDirectDrawSurface7 *front, IDirectDrawSurface7 *back ) {
	IDirectDraw7 *ddraw = directDraw->directDraw;
	backSurface = back;
	frontSurface = front;
	mode = mode_;
	switch ( mode ) {
	case FRAMEBUFFER_FLIPPING:
		window = 0;
		if ( !front ) {
			NiD3DRender::Error( "Creation failed: App-supplied front buffer is NULL" );
			return false;
		}
		front->AddRef();
		if ( !backSurface ) {
			NiD3DRender::Error( "Creation failed: App-supplied back buffer is NULL" );
			return false;
		}
		frontImage = NiDDBufferImage::Create( front, ddraw );
		directDraw->SetPrimarySurface( frontSurface, frontImage );
		// the dimensions come from the front surface, though the error names
		// the back buffer (an original quirk)
		if ( !GetSurfaceSize( frontSurface, &width, &height ) ) {
			NiD3DRender::Error( "Creation failed: Cannot get dimensions from app-supplied back buffer" );
			return false;
		}
		backImage = NiDDBufferImage::Create( back, ddraw );
		break;
	case FRAMEBUFFER_WINDOWED: {
		unsigned int bitDepth = directDraw->GetBitDepth();
		if ( bitDepth != 16 && bitDepth != 32 ) {
			NiD3DRender::Error( "Creation failed: Windowed mode must be 16 or 32 bits" );
			return false;
		}
		window = window_;
		RECT rect;
		GetClientRect( window, &rect );
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		if ( !frontSurface ) {
			NiD3DRender::Error( "Creation failed: Supplied front buffer is invalid" );
			return false;
		}
		frontSurface->AddRef();
		DDSURFACEDESC2 frontDesc;
		GetSurfaceDesc( frontSurface, &frontDesc );
		frontImage = NiDDBufferImage::Create( front, ddraw );
		if ( frontDesc.ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE ) {
			directDraw->SetPrimarySurface( frontSurface, frontImage );
		}
		if ( !backSurface ) {
			NiD3DRender::Error( "Creation failed: App-supplied back buffer is NULL" );
			return false;
		}
		backImage = NiDDBufferImage::Create( back, ddraw );
		backSurface->AddRef();
		if ( !GetSurfaceSize( backSurface, &width, &height ) ) {
			NiD3DRender::Error( "Creation failed: Cannot get dimensions from app-supplied back buffer" );
			return false;
		}
		if ( !CreateClipper() ) {
			return false;
		}
		break;
	}
	default:
		return false;
	}

	pixelFormat = NiDDPixelDesc( backSurface );
	NiDDPixelDescTable::Register( pixelFormat );
	return true;
}

/*
================
NiD3DFrameBuffer::CreateImages

The images of the surfaces; a windowed front surface shares the DirectDraw
object's primary image, and an offscreen buffer's front image is its back
image. The DirectDraw object's surface and interface are read through its
accessors, as the SDK's debug NetImmerse 3.1 does (AnimOnly.exe 0x51E66E,
DDFramebuffer.cpp).
================
*/
// FUNCTION: STBC 0x7CAA60
bool NiD3DFrameBuffer::CreateImages() {
	assert( !frontImage );
	assert( !backImage );
	if ( frontSurface ) {
		if ( frontSurface == directDraw->GetPrimarySurface() ) {
			frontImage = directDraw->GetPrimaryImage();
		} else {
			frontImage = NiDDBufferImage::Create( frontSurface, directDraw->GetDirectDraw() );
		}
		if ( !frontImage ) {
			NiD3DRender::Error( "Creation failed: Front buffer surface image could not be created" );
			return false;
		}
	}
	if ( backSurface ) {
		backImage = NiDDBufferImage::Create( backSurface, directDraw->GetDirectDraw() );
		if ( !backImage ) {
			NiD3DRender::Error( "Creation failed: Back buffer surface image could not be created" );
			return false;
		}
	} else {
		backImage = 0;
	}
	if ( backImage && !frontImage ) {
		frontImage = backImage;
	}
	return true;
}

/*
================
NiD3DFrameBuffer::~NiD3DFrameBuffer
================
*/
// FUNCTION: STBC 0x7CABB0
NiD3DFrameBuffer::~NiD3DFrameBuffer() {
	frontImage = 0;
	backImage = 0;
	Unknown7CACB0();
	if ( directDraw ) {
		directDraw->DecRefCount();
		directDraw = 0;
	}
}

/*
================
NiD3DFrameBuffer::Unknown7CACB0

Releases the surfaces: the z-buffer is detached from the back surface first
unless it was never attached, and a flipping chain's back surface goes with
its front surface.
================
*/
// FUNCTION: STBC 0x7CACB0
void NiD3DFrameBuffer::Unknown7CACB0() {
	if ( unknown50 ) {
		unknown50->Release();
	}
	unknown50 = 0;
	if ( clipper ) {
		clipper->Release();
	}
	clipper = 0;
	if ( frontSurface ) {
		frontSurface->Release();
	}
	frontSurface = 0;
	if ( zBuffer ) {
		if ( !( unknown04 & 0x20 ) ) {
			int err;
			if ( backSurface ) {
				err = backSurface->DeleteAttachedSurface( 0, zBuffer );
			}
			assert( err == 0 );	// BUG: err is read uninitialised without a back surface (debug builds only)
		}
		zBuffer->Release();
		zBuffer = 0;
	}
	if ( mode != FRAMEBUFFER_FLIPPING && backSurface ) {
		backSurface->Release();
	}
	backSurface = 0;
}

/*
================
NiD3DFrameBuffer::SwapBuffers

Presents the back surface: a flipping chain flips, waiting while the
hardware is still drawing; a window or the desktop takes a blit. Fails
while a surface is lost.
================
*/
// FUNCTION: STBC 0x7CAD30
bool NiD3DFrameBuffer::SwapBuffers() {
	if ( backSurface->IsLost() != DD_OK
		|| ( frontSurface && frontSurface->IsLost() != DD_OK )
		|| ( zBuffer && zBuffer->IsLost() != DD_OK ) ) {
		return false;
	}

	if ( mode == FRAMEBUFFER_FLIPPING ) {
		for ( ;; ) {
			HRESULT result = frontSurface->Flip( 0, 0 );
			if ( result == DD_OK ) {
				unknown5C = true;
				break;
			}
			if ( result != DDERR_WASSTILLDRAWING ) {
				NiD3DRender::Warning( "(Swap buffers):Error flipping surfaces" );
				break;
			}
		}
	} else if ( mode != FRAMEBUFFER_OFFSCREEN ) {
		if ( clipper && frontSurface ) {
			frontSurface->SetClipper( clipper );
		}

		// the whole back surface, onto the client area in a window
		RECT source;
		RECT dest;
		dest.left = source.left = 0;
		dest.top = source.top = 0;
		dest.right = source.right = width;
		dest.bottom = source.bottom = height;
		if ( mode == FRAMEBUFFER_WINDOWED ) {
			ClientToScreen( window, (POINT *)&dest.left );
			ClientToScreen( window, (POINT *)&dest.right );
		}
		frontSurface->Blt( &dest, backSurface, &source, DDBLT_WAIT | DDBLT_DDFX, &bltFX );
	}

	if ( frontImage ) {
		frontImage->unknown20++;
	}
	if ( backImage ) {
		backImage->unknown20++;
	}
	return true;
}

/*
================
NiD3DFrameBuffer::Unknown7CAE70

True when the back surface's memory was lost.
================
*/
// FUNCTION: STBC 0x7CAE70
bool NiD3DFrameBuffer::Unknown7CAE70() {
	return backSurface->IsLost() != DD_OK ? true : false;
}

/*
================
NiD3DFrameBuffer::Unknown7CAE80

Restores the surfaces, then every surface of the DirectDraw object.
================
*/
// FUNCTION: STBC 0x7CAE80
bool NiD3DFrameBuffer::Unknown7CAE80() {
	if ( mode != FRAMEBUFFER_FLIPPING && backSurface && backSurface->Restore() != DD_OK ) {
		return false;
	}
	if ( frontSurface && frontSurface->Restore() != DD_OK ) {
		return false;
	}
	if ( zBuffer && zBuffer->Restore() != DD_OK ) {
		return false;
	}
	directDraw->directDraw->RestoreAllSurfaces();
	return true;
}
