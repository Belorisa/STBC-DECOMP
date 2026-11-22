/*
===============================================================================

	NiD3DFrameBuffer.h

	A set of DirectDraw surfaces the renderer draws to: front and back
	buffers, a z-buffer and their images.

	Evidence: the creation functions (0x7C9E50 and 0x7CA030, "(Creation):
	Creating framebuffer object") allocate 0xC8 bytes and run the
	constructor (0x7C9CE0); the destructor (0x7CABB0) releases the two image
	pointers and the DirectDraw object; NiD3DRender's destructor deletes each
	frame buffer when the reference count at 0xC4 drops to zero ("(Shutdown):
	Destroying framebuffers"); the viewport set before a clear takes its
	width from 0x4C and its height from 0x48; SwapBuffers (0x7CAD30) flips
	the surface at 0x14, or in a window blits the surface at 0x18 into it
	with the DDBLTFX at 0x60 after mapping the client rectangle of the window
	at 0x58. The class names are invented from the log.

===============================================================================
*/

#ifndef __NID3DFRAMEBUFFER_H__
#define __NID3DFRAMEBUFFER_H__

#include "NiPointer.h"
#include "NiImage.h"
#include "NiD3D7.h"
#include "NiD3DPixelFormat.h"

class NiD3DDirectDraw;
class Ni3DDeviceDesc;

// NiD3DFrameBuffer::mode (names invented)
#define FRAMEBUFFER_WINDOWED		0	// blitted into a window
#define FRAMEBUFFER_FLIPPING		1	// a fullscreen flipping chain; the back surface goes with the front
#define FRAMEBUFFER_OFFSCREEN		2	// an offscreen texture surface

// the flags of NiD3DFrameBuffer::Create (names invented)
#define FRAMEBUFFER_32_BIT			0x10	// a 32-bit fullscreen display mode, else 16-bit
#define FRAMEBUFFER_TRIPLE_BUFFER	0x20	// two back buffers when the driver can

class NiD3DFrameBuffer {
public:
							NiD3DFrameBuffer();
							~NiD3DFrameBuffer();

	// releases a reference; the last one deletes the frame buffer
	void					DecRefCount() { if ( --refCount == 0 ) delete this; }

	// a frame buffer on the DirectDraw object: mode 0 renders into a window,
	// 1 into a fullscreen flipping chain and 2 into an offscreen texture
	// surface (names invented)
	static NiD3DFrameBuffer *	Create( NiD3DDirectDraw *directDraw_, int mode, unsigned int flags, unsigned int width_,
										unsigned int height_, HWND window_, HWND focusWindow, IDirect3DDevice7 *device );
	// a frame buffer around the application's own surfaces: mode 0 in a
	// window, 1 a flipping chain (names invented)
	static NiD3DFrameBuffer *	CreateFromSurfaces( NiD3DDirectDraw *directDraw_, int mode, HWND window_,
													IDirectDrawSurface7 *front, IDirectDrawSurface7 *back );

	// the DirectDraw object for a new frame buffer: on the display device
	// when there is one, else on the default device (name invented)
	static NiD3DDirectDraw *	CreateDirectDraw( int unknown, Ni3DDeviceDesc *device );
	// on the application's DirectDraw object
	static NiD3DDirectDraw *	CreateDirectDraw( IDirectDraw7 *directDraw, Ni3DDeviceDesc *device );

	// a surface's size; false when its description fails (name invented)
	static bool				GetSurfaceSize( IDirectDrawSurface7 *surface, unsigned int *width_, unsigned int *height_ );
	// a surface's description (name invented)
	static bool				GetSurfaceDesc( IDirectDrawSurface7 *surface, DDSURFACEDESC2 *desc );


	// the surfaces and the pixel format of the two creation functions, and
	// the images of the surfaces (names invented)
	bool					CreateSurfaces( int mode, unsigned int flags, unsigned int width_, unsigned int height_,
											HWND window_, HWND focusWindow, bool videoMemory, IDirect3DDevice7 *device );
	bool					AttachSurfaces( int mode, HWND window_, IDirectDrawSurface7 *front, IDirectDrawSurface7 *back );
	bool					CreateImages();

	// the front and back surfaces: a fullscreen flipping chain, or the
	// display's primary surface in a window (names invented)
	bool					CreateFlippingSurfaces( bool tripleBuffer );
	bool					CreateWindowedFrontSurface();
	// device: the renderer's device, when it adds a frame buffer; unused
	bool					CreateBackSurface( bool videoMemory, bool texture, IDirect3DDevice7 *device );
	bool					CreateZBuffer( bool videoMemory, const DDPIXELFORMAT *format );
	// detaches the z-buffer from the back surface and releases it (name
	// invented)
	bool					ReleaseZBuffer();
	// gives a windowed front surface a clipper on the window (name invented)
	bool					CreateClipper();
	bool					SwapBuffers();		// "(Swap buffers):Error flipping surfaces"
	void					Unknown7CACB0();	// releases the surfaces
	bool					Unknown7CAE70();
	bool					Unknown7CAE80();	// restores the surfaces
	// the surface the device renders to (AnimOnly.exe 0x513E80; name
	// invented)
	IDirectDrawSurface7 *	GetBackSurface() const { return backSurface; }
	// the DirectDraw object, the z-buffer, the width, the height and the
	// pixel format, as the renderer reads them (names invented; AnimOnly.exe
	// 0x513810, 0x513BA0, 0x513940, 0x513960, 0x513C00)
	NiD3DDirectDraw *		GetDirectDraw() const { return directDraw; }
	IDirectDrawSurface7 *	GetZBuffer() const { return zBuffer; }
	unsigned int			GetWidth() const { return width; }
	unsigned int			GetHeight() const { return height; }
	NiDDPixelDesc *	GetPixelFormat() { return &pixelFormat; }
	// the back (1) or front (2) image; a frame buffer without a front image
	// gives its back image (name invented; AnimOnly.exe 0x513980)
	bool					GetImage( int which, NiImage *&image ) {
								if ( which == 1 ) {
									if ( !backImage ) {
										return false;
									}
									image = backImage;
									return true;
								} else if ( which == 2 ) {
									if ( !frontImage ) {
										if ( !backImage ) {
											return false;
										}
										image = backImage;
									} else {
										image = frontImage;
									}
									return true;
								}
								return false;
							}

	NiD3DDirectDraw *		directDraw;		// shared, reference counted
	unsigned int			unknown04;		// flags; 0x20: the z-buffer was not attached
	int						mode;			// FRAMEBUFFER_*
	NiPointer<NiImage>		frontImage;		// "Front buffer surface image could not be created"
	NiPointer<NiImage>		backImage;		// "Back buffer surface image could not be created"
	IDirectDrawSurface7 *	frontSurface;	// flipped, or blitted into in a window
	IDirectDrawSurface7 *	backSurface;	// the surface the device renders to
	IDirectDrawSurface7 *	zBuffer;		// "Error detaching ZBuffer surface from backbuffer"
	NiDDPixelDesc	pixelFormat;
	unsigned int			height;
	unsigned int			width;
	IUnknown *				unknown50;
	LPDIRECTDRAWCLIPPER		clipper;		// set on the front surface before a blit
	HWND					window;			// the window presented into
	bool					unknown5C;		// set once a flip went through
	DDBLTFX					bltFX;			// for the windowed blit
	int						refCount;
};

#endif /* !__NID3DFRAMEBUFFER_H__ */
