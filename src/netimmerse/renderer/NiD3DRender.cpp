// NiD3DRender.cpp -- the Direct3D 7 renderer

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiD3DRender.h"
#include "NiCamera.h"
#include "NiColor.h"
#include "NiDDImage.h"
#include "NiRawImageData.h"
#include "NiLight.h"
#include "NiTrianglesData.h"
#include "NiTriShapeData.h"
#include "NiTriStripData.h"
#include "NiTriStripsData.h"
#include "NiLinesData.h"
#include "NiParticlesData.h"
#include "NiTextureProperty.h"
#include "NiPropertyState.h"
#include "NiTextureEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007bfad0 _$E30
// FUNCTION: STBC 0x007bfae0 _$E65
// FUNCTION: STBC 0x007bfaf0 _$E69
// FUNCTION: STBC 0x007bfb00 _$E73
// FUNCTION: STBC 0x007bfb10 _$E77
// FUNCTION: STBC 0x007bfb20 _$E181
// FUNCTION: STBC 0x007bfb30 _$E34
// FUNCTION: STBC 0x007bfb40 _$E48
// FUNCTION: STBC 0x007bfb50 _$E61
// FUNCTION: STBC 0x007bfb60 _$E186
// FUNCTION: STBC 0x007bfb70 _$E191
// FUNCTION: STBC 0x007bfb80 _$E216
// FUNCTION: STBC 0x007bfb90 _$E220
// FUNCTION: STBC 0x007bfba0 _$E227
// FUNCTION: STBC 0x007bfbb0 _$E210
// FUNCTION: STBC 0x007bfbc0 _$E235
// FUNCTION: STBC 0x007bfbd0 _$E259
// FUNCTION: STBC 0x007bfbe0 _$E239
// FUNCTION: STBC 0x007bfbf0 _$E263
// FUNCTION: STBC 0x007bfc00 _$E156
// FUNCTION: STBC 0x007bfc10 _$E243
// FUNCTION: STBC 0x007bfc20 _$E247
// FUNCTION: STBC 0x007bfc30 _$E251
// FUNCTION: STBC 0x007bfc40 _$E255
// FUNCTION: STBC 0x007bfc50 _$E131
// FUNCTION: STBC 0x007bfc60 _$E166
// FUNCTION: STBC 0x007bfc70 _$E101
// FUNCTION: STBC 0x007bfc80 _$E136
// FUNCTION: STBC 0x007bfc90 _$E141
// FUNCTION: STBC 0x007bfca0 _$E267

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007c4ca0 _$E280
// FUNCTION: STBC 0x007bfa70 _$E17
// FUNCTION: STBC 0x007bfa90 _$E15
// FUNCTION: STBC 0x007bfaa0 _$E23
// FUNCTION: STBC 0x007bfac0 _$E21

// FUNCTION: STBC 0x007bfcb0 _$E273
// FUNCTION: STBC 0x007bfcd0 _$E270
// DATA: STBC 0x9A1160 ?rtti@NiD3DRender@@2VNiRTTI@@B
const NiRTTI NiD3DRender::rtti( "NiD3DRender", &NiRender::rtti );

// DATA: STBC 0x9A1740 ?errorString@NiD3DRender@@2PADA
char NiD3DRender::errorString[256];

/*
================
NiD3DRender::Register

Nothing calls it: the stream registers no renderer.
================
*/
// FUNCTION: STBC 0x7BFCF0 ?Register@NiD3DRender@@SAHXZ
NiImplementRegister( NiD3DRender )

/*
================
NiD3DRender::NiD3DRender

Nothing is created here: the creation functions build the frame buffer, the
device and the managers.
================
*/
// FUNCTION: STBC 0x7BFE40
NiD3DRender::NiD3DRender() {
	frameBuffer = 0;
	direct3D = 0;
	device = 0;
	renderState = 0;
	texturePipeline = 0;
	lightManager = 0;
	textureManager = 0;
	backgroundColor = 0;
	clearStencil = 0;
	unknown2A4 = 0;
	unknown134 = 0.1f;
	unknown138 = 100.0f;

	unknown1BC._11 = 2.0f;
	unknown1BC._21 = 0.0f;
	unknown1BC._31 = -1.0f;
	unknown1BC._41 = 0.0f;
	unknown1BC._12 = 0.0f;
	unknown1BC._22 = 2.0f;
	unknown1BC._32 = -1.0f;
	unknown1BC._42 = 0.0f;
	unknown1BC._13 = 0.0f;
	unknown1BC._23 = 0.0f;
	unknown1BC._33 = 1.0f;
	unknown1BC._43 = 0.0f;
	unknown1BC._14 = 0.0f;
	unknown1BC._24 = 0.0f;
	unknown1BC._34 = 1.0f;
	unknown1BC._44 = 0.0f;

	unknown23C._11 = 2.0f;
	unknown23C._21 = 0.0f;
	unknown23C._31 = -1.0f;
	unknown23C._41 = 0.0f;
	unknown23C._12 = 0.0f;
	unknown23C._22 = 2.0f;
	unknown23C._32 = -1.0f;
	unknown23C._42 = 0.0f;
	unknown23C._13 = 0.0f;
	unknown23C._23 = 0.0f;
	unknown23C._33 = 1.0f;
	unknown23C._43 = 0.0f;
	unknown23C._14 = 0.0f;
	unknown23C._24 = 0.0f;
	unknown23C._34 = 0.0f;
	unknown23C._44 = 1.0f;

	memset( &unknown1FC, 0, sizeof( unknown1FC ) );
	unknown1FC._11 = unknown1FC._22 = unknown1FC._33 = unknown1FC._44 = 1.0f;

	unknownDC = NiPoint3::UNIT_X;
	unknownE8 = NiPoint3::UNIT_Y;
	vertexBufferManager = 0;

	memset( &zBufferFormat16, 0, sizeof( zBufferFormat16 ) );
	memset( &zBufferFormatDeep, 0, sizeof( zBufferFormatDeep ) );
	memset( &zBufferFormatStencil, 0, sizeof( zBufferFormatStencil ) );
	memset( &zBufferFormat, 0, sizeof( zBufferFormat ) );

	memset( &viewport, 0, sizeof( viewport ) );
	viewport.dwX = 0;
	viewport.dwY = 0;
	viewport.dwWidth = 1;
	viewport.dwHeight = 1;
	viewport.dvMinZ = 0.0f;
	viewport.dvMaxZ = 1.0f;

	unknown294 = true;
	unknown2A0 = false;
	unknown2A1 = false;
	unknown2A8 = false;
}

/*
================
NiD3DRender::SelectDevice

Copies the device of the given kind on the application's DirectDraw object
into device, and checks that it can render in the mode and at the bit depth
of the front surface. Returns 0 on success, otherwise the reason: 1 the bit
depth, 5 no fullscreen, 6 no window, 7 and 8 no device of the kind (7 for
kind 0, 8 for kinds 1 and 2), 9 kind 3, 10 a surface is missing.

Kinds 0 and 1 take the software device, kind 2 the transform and lighting
device or else the hardware one, kind 3 the transform and lighting device.
Kind 3 fails with 9 even when it copied the device (an original quirk).
================
*/
// FUNCTION: STBC 0x7C0160
// PARKED: frame: the debug NetImmerse 3.1's statements (AnimOnly 0x50DB40); same instructions, retail's frame is 0x10 larger (two more 4-byte slots after the device list: framemap)
int NiD3DRender::SelectDevice( IDirectDraw7 *directDraw, int mode, IDirectDrawSurface7 *front, IDirectDrawSurface7 *back,
								int kind, Ni3DDeviceDesc &device ) {
	if ( !back || !front ) {
		return 10;
	}

	NiTList<Ni3DDeviceDesc *> devices( 2 );
	NiD3DDirectDraw::EnumerateDevices( directDraw, 0, &devices );

	Ni3DDeviceDesc *rgbDevice = 0;
	Ni3DDeviceDesc *halDevice = 0;
	Ni3DDeviceDesc *tnlHalDevice = 0;
	Ni3DDeviceDesc *tnlOnlyDevice = 0;	// (name invented)
	NiTListIterator pos = devices.GetHeadPos();
	while ( pos ) {
		Ni3DDeviceDesc *candidate = devices.GetNext( pos );
		if ( candidate->GetDeviceGuid() == &IID_IDirect3DHALDevice ) {
			halDevice = candidate;
		} else if ( candidate->GetDeviceGuid() == &IID_IDirect3DTnLHalDevice ) {
			tnlHalDevice = candidate;
			tnlOnlyDevice = candidate;
		} else if ( candidate->GetDeviceGuid() == &IID_IDirect3DRGBDevice ) {
			rgbDevice = candidate;
		}
	}

	int error = 0;
	switch ( kind ) {
	case 0:
		if ( rgbDevice ) {
			device = *rgbDevice;
		} else {
			error = 7;
		}
		break;
	case 1:
		if ( rgbDevice ) {
			device = *rgbDevice;
		} else {
			error = 8;
		}
		break;
	case 2:
		if ( tnlHalDevice ) {
			device = *tnlHalDevice;
		} else if ( halDevice ) {
			device = *halDevice;
		} else {
			error = 8;
		}
		break;
	case 3:
		if ( tnlOnlyDevice ) {
			device = *tnlOnlyDevice;
		}
		error = 9;
		break;
	}

	while ( devices.GetSize() ) {
		delete devices.RemoveHead();
	}
	if ( error ) {
		return error;
	}

	if ( mode == RENDER_FULLSCREEN ) {
		if ( !device.IsFullscreen() ) {
			return 5;
		}
	} else if ( !device.IsWindowed() ) {
		return 6;
	}

	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	front->GetSurfaceDesc( &desc );
	unsigned int bitDepth = desc.ddpfPixelFormat.dwRGBBitCount;
	if ( bitDepth == 16 || ( bitDepth == 32 && ( device.GetFlags() & DEVICE_DESC_RENDERS_32_BIT ) ) ) {
		return 0;
	}
	return 1;
}

/*
================
NiD3DRender::QueryDirect3D

The Direct3D interface of the DirectDraw object.
================
*/
inline bool NiD3DRender::QueryDirect3D( NiD3DDirectDraw *directDraw ) {
	if ( directDraw->directDraw->QueryInterface( IID_IDirect3D7, (void **)&direct3D ) != DD_OK ) {
		Error( "Creation failed: Could not query Direct3D from DirectDraw" );
		return false;
	}
	return true;
}

/*
================
NiD3DRender::Create

Fullscreen takes the preferred fullscreen device, a window the preferred
device on the primary display, and either falls back to software. The
focus window defaults to the rendering window.
================
*/
// FUNCTION: STBC 0x7C09C0
NiD3DRender *NiD3DRender::Create( int mode, unsigned int width, unsigned int height, unsigned int flags,
									HWND window, HWND focusWindow, Ni3DDeviceDesc *device ) {
	Message( "(Creation): Attempting to create Direct3D renderer" );
	NiImage::createFunction = NiDDImage::Create;
	if ( !focusWindow ) {
		focusWindow = window;
	}

	int frameBufferMode = FRAMEBUFFER_WINDOWED;
	if ( mode == RENDER_FULLSCREEN ) {
		frameBufferMode = FRAMEBUFFER_FLIPPING;
		if ( !device ) {
			device = NiD3DDirectDraw::GetFullscreenDevice();
		}
	} else if ( !device ) {
		device = NiD3DDirectDraw::GetWindowedDevice();
	}
	if ( !device ) {
		device = NiD3DDirectDraw::GetSoftwareDevice();
	}
	if ( !device ) {
		Error( "Creation failed: Could not find any Direct3D-compatible 3D hardware or software" );
		return 0;
	}

	NiD3DRender *render = new NiD3DRender;
	NiD3DDirectDraw *directDraw = NiD3DFrameBuffer::CreateDirectDraw( frameBufferMode, device );
	bool success = directDraw && render->QueryDirect3D( directDraw )
		&& render->Initialize( directDraw, mode, flags, width, height, window, focusWindow, 0, 0 );
	if ( !success ) {
		if ( render->direct3D ) {
			render->direct3D->Release();
			render->direct3D = 0;
		}
		if ( directDraw ) {
			directDraw->DecRefCount();
		}
		delete render;
		return 0;
	}
	directDraw->DecRefCount();
	Message( "(Creation): Renderer creation successful" );
	return render;
}

/*
================
NiD3DRender::CreateFromSurfaces

As Create, on the application's own DirectDraw object, display device and
front and back surfaces; the window is also the focus window.
================
*/
// FUNCTION: STBC 0x7C0C90
NiD3DRender *NiD3DRender::CreateFromSurfaces( int mode, unsigned int flags, HWND window, IDirectDraw7 *directDraw,
											  IDirectDrawSurface7 *front, IDirectDrawSurface7 *back,
											  Ni3DDeviceDesc *device ) {
	Message( "(Creation): Attempting to create Direct3D renderer from app-supplied buffers" );
	NiImage::createFunction = NiDDImage::Create;
	if ( !device || !back || !front ) {
		return 0;
	}

	NiD3DRender *render = new NiD3DRender;
	NiD3DDirectDraw *appDirectDraw = NiD3DFrameBuffer::CreateDirectDraw( directDraw, device );
	bool success = appDirectDraw && render->QueryDirect3D( appDirectDraw )
		&& render->Initialize( appDirectDraw, mode, flags, 0, 0, window, window, front, back );
	if ( !success ) {
		if ( render->direct3D ) {
			render->direct3D->Release();
			render->direct3D = 0;
		}
		if ( appDirectDraw ) {
			appDirectDraw->DecRefCount();
		}
		delete render;
		return 0;
	}
	appDirectDraw->DecRefCount();
	Message( "(Creation): Renderer creation successful" );
	return render;
}

/*
================
NiD3DRender::~NiD3DRender

Destroys the managers, the cached geometry, the device and the frame
buffers, logging each step.
================
*/
// FUNCTION: STBC 0x7C0EA0
// FUNCTION: STBC 0x7C0130 ??_GNiD3DRender@@UAEPAXI@Z
NiD3DRender::~NiD3DRender() {
	Message( "(Shutdown): Shutting down D3D Renderer" );

	delete lightManager;
	delete texturePipeline;

	Message( "(Shutdown): Destroying cached geometry data" );
	NiTMapIterator pos = unknownCC.GetFirstPos();
	NiD3DGeometryData *data = 0;
	while ( pos ) {
		unsigned int key;
		unknownCC.GetNext( pos, key, data );
		if ( data ) {
			if ( data->vertexBuffer && data->vertexBuffer->Release() ) {
				Warning( "(Shutdown): Vertex buffer has extra reference" );
			}
			delete data;
		}
	}
	unknownCC.RemoveAll();
	Message( "(Shutdown): Finished destroying cached geometry data" );

	Message( "(Shutdown): Destroying texture manager" );
	if ( textureManager ) {
		textureManager->Release();
	}
	Message( "(Shutdown): Finished destroying texture manager" );

	if ( renderState ) {
		renderState->DecRefCount();
	}

	if ( device ) {
		Message( "(Shutdown): Releasing D3D device" );
		device->Release();
		device = 0;
		Message( "(Shutdown): Released D3D device" );
	}
	if ( direct3D ) {
		direct3D->Release();
		direct3D = 0;
	}

	delete vertexBufferManager;
	frameBuffer = 0;

	Message( "(Shutdown): Destroying framebuffers" );
	for ( unsigned int i = 0; i < frameBuffers.GetSize(); i++ ) {
		if ( frameBuffers.GetAt( i ) ) {
			frameBuffers.GetAt( i )->DecRefCount();
			frameBuffers.SetAt( i, 0 );
		}
	}
	Message( "(Shutdown): Finished destroying framebuffers" );

	Message( "(Shutdown): Finished shutting down D3D Renderer" );
}

/*
================
NiD3DRender::Unknown7C1320
================
*/
// FUNCTION: STBC 0x7C1320
IDirectDraw7 *NiD3DRender::Unknown7C1320() {
	return frameBuffer->directDraw->directDraw;
}

/*
================
NiD3DRender::VirtualSlot38

Starts a frame: once the surfaces are usable, resets the per-frame state of
the managers and begins the scene.
================
*/
// FUNCTION: STBC 0x7C1330
void NiD3DRender::VirtualSlot38( void *unknown ) {
	NiRender::VirtualSlot38( unknown );
	if ( Unknown7C45B0() ) {
		textureManager->VirtualSlot00();
		lightManager->DisableLights();
		renderState->Unknown7D29B0();
		texturePipeline->ResetPasses();
		if ( device->BeginScene() == D3D_OK ) {
			unknown2A8 = true;
		}
	}
}

/*
================
NiD3DRender::VirtualSlot39

Ends the scene, and counts the presentation on the back buffer image.
================
*/
// FUNCTION: STBC 0x7C13A0
void NiD3DRender::VirtualSlot39() {
	NiRender::VirtualSlot39();
	if ( unknown2A8 ) {
		if ( device->EndScene() == D3D_OK ) {
			unknown2A8 = false;
			NiImage *image = frameBuffer->backImage;
			if ( image ) {
				image->unknown20++;
			}
		}
	}
}

/*
================
NiD3DRender::Unknown7C13E0
================
*/
// FUNCTION: STBC 0x7C13E0
void NiD3DRender::Unknown7C13E0( bool unknown ) {
	renderState->Unknown7D3040( unknown );
}

/*
================
NiD3DRender::Unknown7C1400
================
*/
// FUNCTION: STBC 0x7C1400
void NiD3DRender::Unknown7C1400( bool unknown ) {
	renderState->Unknown7D3070( unknown );
}

/*
================
NiD3DRender::VirtualSlot26
================
*/
// FUNCTION: STBC 0x7C1420
float NiD3DRender::VirtualSlot26() {
	return renderState->GetUnknown17C();
}

/*
================
NiD3DRender::VirtualSlot27

Sets the render state's value, kept above zero, and its reciprocal less one.
================
*/
// FUNCTION: STBC 0x7C1430
void NiD3DRender::VirtualSlot27( float unknown ) {
	renderState->SetUnknown17C( unknown );
}

/*
================
NiD3DRender::VirtualSlot17

Shows the back buffer of the current frame buffer.
================
*/
// FUNCTION: STBC 0x7C1480
bool NiD3DRender::VirtualSlot17() {
	return frameBuffer->SwapBuffers();
}

/*
================
NiD3DRender::ClearBuffer

Clears the current frame buffer: its back buffer (flag 1), the stencil
buffer (flag 2, when the z-buffer format has one) and the z-buffer (flag 4),
inside the rectangle given as fractions of the frame buffer (left, right,
top, bottom, from the bottom up), or all of it.
================
*/
// FUNCTION: STBC 0x7C1490
void NiD3DRender::ClearBuffer( const NiRect<float> *rect, unsigned int flags ) {
	if ( frameBuffer->Unknown7CAE70() && !Unknown7C45B0() ) {
		return;
	}

	DWORD stencil = ( ( flags & 2 ) && ( unknown2A4 & 0x80 ) ) ? D3DCLEAR_STENCIL : 0;
	DWORD clearFlags = ( flags & 1 ) | ( ( flags >> 1 ) & 2 ) | stencil;

	D3DRECT clearRect;
	if ( !rect ) {
		clearRect.x1 = clearRect.y1 = 0;
		clearRect.x2 = frameBuffer->width;
		clearRect.y2 = frameBuffer->height;
	} else {
		float width = (float)frameBuffer->width;
		float height = (float)frameBuffer->height;
		clearRect.x1 = (LONG)( width * rect->left );
		clearRect.y1 = (LONG)( ( 1.0f - rect->top ) * height );
		clearRect.x2 = (LONG)( width * rect->right );
		clearRect.y2 = (LONG)( ( 1.0f - rect->bottom ) * height );
	}

	viewport.dwWidth = frameBuffer->width;
	viewport.dwHeight = frameBuffer->height;
	device->SetViewport( &viewport );
	device->Clear( 1, &clearRect, clearFlags, backgroundColor, 1.0f, clearStencil );
}

/*
================
NiD3DRender::SetBackgroundColor

The color the frame buffer is cleared to.
================
*/
// FUNCTION: STBC 0x7C15D0
bool NiD3DRender::SetBackgroundColor( const NiColor &color ) {
	backgroundColor = RGB_MAKE( (unsigned char)( color.r * 255.0f ), (unsigned char)( color.g * 255.0f ), (unsigned char)( color.b * 255.0f ) );
	return true;
}

/*
================
NiD3DRender::VirtualSlot25

Sets the value the stencil buffer is cleared to.
================
*/
// FUNCTION: STBC 0x7C1620
void NiD3DRender::VirtualSlot25( unsigned int unknown ) {
	clearStencil = unknown;
}

/*
================
NiD3DRender::VirtualSlot22

The current frame buffer's back (1) or front (2) image; a frame buffer
without a front image shows its back image.
================
*/
// FUNCTION: STBC 0x7C1630
void *NiD3DRender::VirtualSlot22( int which ) {
	NiImage *image = 0;
	if ( frameBuffer->GetImage( which, image ) ) {
		return image;
	}
	return 0;
}

/*
================
NiD3DRender::VirtualSlot19

The width of the current frame buffer.
================
*/
// FUNCTION: STBC 0x7C1670
unsigned int NiD3DRender::VirtualSlot19() {
	return frameBuffer->GetWidth();
}

/*
================
NiD3DRender::VirtualSlot20

The height of the current frame buffer.
================
*/
// FUNCTION: STBC 0x7C1680
unsigned int NiD3DRender::VirtualSlot20() {
	return frameBuffer->GetHeight();
}

/*
================
NiD3DRender::VirtualSlot21
================
*/
// FUNCTION: STBC 0x7C1690
unsigned int NiD3DRender::VirtualSlot21() {
	return frameBuffer->pixelFormat.bitsPerPixel;
}

/*
================
NiD3DRender::PurgeEffect

Drops a light that is being deleted from the light manager; other effects
need nothing.
================
*/
// FUNCTION: STBC 0x7C16A0
void NiD3DRender::PurgeEffect( NiDynamicEffect *effect ) {
	NiLight *light = NiDynamicCast( NiLight, effect );
	if ( light ) {
		lightManager->RemoveLight( light );
	}
}

/*
================
NiD3DRender::VirtualSlot41

Sets the camera: the view transform from its frame, the projection from
its frustum and the viewport from the port, a fraction of the frame
buffer from the bottom up.
================
*/
// FUNCTION: STBC 0x7C16F0
void NiD3DRender::VirtualSlot41( const NiPoint3 &location, const NiPoint3 &direction, const NiPoint3 &up,
								 const NiPoint3 &right, const NiFrustum &frustum, const NiRect<float> &port ) {
	unknown13C._11 = right.x;
	unknown13C._12 = up.x;
	unknown13C._13 = direction.x;
	unknown13C._21 = right.y;
	unknown13C._22 = up.y;
	unknown13C._23 = direction.y;
	unknown13C._31 = right.z;
	unknown13C._32 = up.z;
	unknown13C._33 = direction.z;
	unknown13C._41 = -location.Dot( right );
	unknown13C._42 = -location.Dot( up );
	unknown13C._43 = -location.Dot( direction );
	unknown13C._14 = unknown13C._24 = unknown13C._34 = 0.0f;
	unknown13C._44 = 1.0f;
	device->SetTransform( D3DTRANSFORMSTATE_VIEW, &unknown13C );

	unknownF4 = unknownDC = right;
	unknown100 = unknownE8 = up;

	if ( texturePipeline ) {
		texturePipeline->Unknown7D0320( direction, up, right, location.x, location.y, location.z );
	}

	unknown134 = frustum.nearDistance;
	unknown138 = frustum.farDistance - frustum.nearDistance;
	float width = frustum.right - frustum.left;
	float centerX = frustum.left + frustum.right;
	float height = frustum.top - frustum.bottom;
	float centerY = frustum.bottom + frustum.top;
	float invDepth = 1.0f / unknown138;
	unknown17C._11 = 2.0f / width;
	unknown17C._21 = 0.0f;
	unknown17C._31 = -( centerX / width );
	unknown17C._41 = 0.0f;
	unknown17C._12 = 0.0f;
	unknown17C._22 = 2.0f / height;
	unknown17C._32 = -( centerY / height );
	unknown17C._42 = 0.0f;
	unknown17C._13 = 0.0f;
	unknown17C._23 = 0.0f;
	unknown17C._33 = invDepth * frustum.farDistance;
	unknown17C._43 = -( invDepth * frustum.nearDistance ) * frustum.farDistance;
	unknown1BC._33 = unknown17C._33;
	unknown1BC._43 = unknown17C._43;
	unknown17C._14 = 0.0f;
	unknown17C._24 = 0.0f;
	unknown17C._34 = 1.0f;
	unknown17C._44 = 0.0f;
	device->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &unknown17C );

	NiD3DFrameBuffer *buffer = frameBuffer;
	D3DVIEWPORT7 portViewport;
	memset( &portViewport, 0, sizeof( portViewport ) );
	portViewport.dwX = (DWORD)( buffer->width * port.left );
	portViewport.dwY = (DWORD)( ( 1.0f - port.top ) * buffer->height );
	portViewport.dwWidth = (DWORD)( ( port.right - port.left ) * buffer->width );
	portViewport.dwHeight = (DWORD)( ( port.top - port.bottom ) * buffer->height );
	portViewport.dvMinZ = 0.0f;
	portViewport.dvMaxZ = 1.0f;
	device->SetViewport( &portViewport );

	if ( renderState ) {
		renderState->SetCameraRange( frustum.nearDistance, frustum.farDistance );
	}

}

/*
================
NiD3DRender::VirtualSlot52

Only notes whether there are culling planes; the planes are not used.
================

*/
// FUNCTION: STBC 0x7C1A60
void NiD3DRender::VirtualSlot52( NiPlane **planes, unsigned int count, unsigned int sideCount, unsigned int planeMask ) {
	unknown294 = count != 0;

}

/*
================
NiD3DRender::VirtualSlot42

Sets the world transform of the object drawn next, and the camera's right
and up vectors in its model space.
================
*/
// FUNCTION: STBC 0x7C1A80
void NiD3DRender::VirtualSlot42( const NiPoint3 &translate, const NiMatrix3 &rotate, float scale ) {
	D3DMATRIX world;
	world._11 = rotate.GetEntry( 0, 0 ) * scale;
	world._12 = rotate.GetEntry( 1, 0 ) * scale;
	world._13 = rotate.GetEntry( 2, 0 ) * scale;
	world._21 = rotate.GetEntry( 0, 1 ) * scale;
	world._22 = rotate.GetEntry( 1, 1 ) * scale;
	world._23 = rotate.GetEntry( 2, 1 ) * scale;
	world._31 = rotate.GetEntry( 0, 2 ) * scale;
	world._32 = rotate.GetEntry( 1, 2 ) * scale;
	world._33 = rotate.GetEntry( 2, 2 ) * scale;
	world._41 = translate.x;
	world._42 = translate.y;
	world._43 = translate.z;
	world._14 = world._24 = world._34 = 0.0f;
	world._44 = 1.0f;

	device->SetTransform( D3DTRANSFORMSTATE_WORLD, &world );
	renderState->Unknown7D2940( scale, rotate, translate );

	unknownF4 = scale * ( unknownDC * rotate );
	unknown100 = scale * ( unknownE8 * rotate );
}

/*
================
NiD3DRender::VirtualSlot43

Inside a scene: restores the saved bound and puts identity view and world
transforms on the device, with one of the two fixed projections.
================
*/
// FUNCTION: STBC 0x7C1BF0
void NiD3DRender::VirtualSlot43( bool unknown ) {
	if ( unknown2A8 ) {
		NiRender::VirtualSlot43( unknown );
		unknown10C = unknown120;
		device->SetTransform( D3DTRANSFORMSTATE_VIEW, &unknown1FC );
		device->SetTransform( D3DTRANSFORMSTATE_WORLD, &unknown1FC );
		if ( unknown == true ) {
			device->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &unknown23C );
		} else {
			device->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &unknown1BC );
		}
	}
}

/*
================
NiD3DRender::VirtualSlot44

Puts the camera's view and projection transforms back on the device.
================
*/
// FUNCTION: STBC 0x7C1C70
void NiD3DRender::VirtualSlot44() {
	device->SetTransform( D3DTRANSFORMSTATE_VIEW, &unknown13C );
	device->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &unknown17C );
}

/*
================
NiD3DRender::VirtualSlot45

Draws an indexed triangle list (NiTriShapeData), with its world bound.
================
*/
// FUNCTION: STBC 0x7C1CA0
void NiD3DRender::VirtualSlot45( NiTriShapeData *data, const NiBound &bound ) {
	if ( unknown2A8 && data->GetTriList() && data->GetActiveTriangleCount() ) {
		unknown10C = bound;
		DrawPrimitive( D3DPT_TRIANGLELIST, data->GetActiveVertexCount(), data->GetTriList(),
			data->GetActiveTriangleCount() * 3, data, 0, 0, 0 );
	}
}

/*
================
NiD3DRender::VirtualSlot46

Draws a triangle list without indices: three vertices per triangle.
================
*/
// FUNCTION: STBC 0x7C1D10
void NiD3DRender::VirtualSlot46( NiTriBasedGeomData *data, const NiBound &bound ) {
	if ( unknown2A8 ) {
		unknown10C = bound;
		DrawPrimitive( D3DPT_TRIANGLELIST, data->GetActiveTriangleCount() * 3, 0, 0, data, 0, 0, 0 );
	}
}

/*
================
NiD3DRender::VirtualSlot47

Draws one triangle strip without indices.
================
*/
// FUNCTION: STBC 0x7C1D60
void NiD3DRender::VirtualSlot47( NiTriBasedGeomData *data, const NiBound &bound ) {
	if ( unknown2A8 ) {
		unknown10C = bound;
		DrawPrimitive( D3DPT_TRIANGLESTRIP, data->GetActiveVertexCount(), 0, 0, data, 0, 0, 0 );
	}
}

/*
================
NiD3DRender::VirtualSlot48

Draws the triangle strips of an NiTriStripsData, with its world bound.
================
*/
// FUNCTION: STBC 0x7C1DB0
void NiD3DRender::VirtualSlot48( NiTriStripsData *data, const NiBound &bound ) {
	if ( unknown2A8 && data->GetStripLists() && data->GetStripCount() ) {
		unknown10C = bound;
		DrawStrips( D3DPT_TRIANGLESTRIP, data->GetActiveVertexCount(), data->GetStripCount(),
			data->GetStripLists(), data->GetStripLengths(), data );
	}
}

/*
================
NiD3DRender::VirtualSlot49

Draws the active particles of an NiParticlesData as camera-facing squares:
each particle's four corners lie its size times the radius along the
camera's right and up vectors (unknownF4 and unknown100, in the model
space) from its vertex, as two triangles each.
================
*/
// FUNCTION: STBC 0x7C1E10
void NiD3DRender::VirtualSlot49( NiParticlesData *data, const NiBound &bound ) {
	if ( !unknown2A8 ) {
		return;
	}
	unsigned short particleCount = data->GetActiveParticleCount();
	if ( !particleCount ) {
		return;
	}
	unknown10C = bound;

	unsigned int vertexCount = particleCount * 4;
	renderState->ApplyPropertyState( propertyState );
	texturePipeline->Unknown7CD640( *propertyState, effectState, data->GetNormals() != 0, 1 );
	lightManager->Unknown7D1950( effectState, propertyState->GetTexture()->GetImage() != 0,
		propertyState->GetTextureMode(), propertyState->GetVertexColor() );
	texturePipeline->Unknown7CE040();
	unsigned short *indices = vertexBufferManager->Unknown7CBBE0( vertexCount );

	float radius = data->GetRadius();
	NiColorA *colors = data->GetColors();
	float *sizes = data->GetSizes();
	NiPoint3 *vertices = data->GetVertices();
	NiPoint3 *normals = data->GetNormals();
	// the corner offsets: down and right, down and left
	NiPoint3 downRight = ( unknown100 - unknownF4 ) * radius;
	NiPoint3 downLeft = ( unknown100 + unknownF4 ) * radius;
	unsigned int i;

	if ( normals ) {
		IDirect3DVertexBuffer7 *vertexBuffer = vertexBufferManager->Unknown7CB570(
			D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1, vertexCount );
		if ( !vertexBuffer ) {
			return;
		}
		NiD3DParticleNormalVertex *vertex = 0;
		DWORD size;
		if ( vertexBuffer->Lock( DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void **)&vertex, &size ) != D3D_OK ) {
			return;
		}
		if ( colors ) {
			for ( i = 0; i < particleCount; i++ ) {
				int a = FloatToInt( colors->a * 255.0f );
				int r = FloatToInt( colors->r * 255.0f );
				int g = FloatToInt( colors->g * 255.0f );
				int b = FloatToInt( colors->b * 255.0f );
				D3DCOLOR color = ( ( ( ( ( a << 8 ) | r ) << 8 ) | g ) << 8 ) | b;
				NiPoint3 left = *sizes * downLeft;
				NiPoint3 right = *sizes * downRight;
				NiPoint3 corner;
				NiPoint3 normal = *normals;

				corner = *vertices - left;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = color;
				vertex->u = 0.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = *vertices - right;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = color;
				vertex->u = 1.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = left + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = color;
				vertex->u = 1.0f;
				vertex->v = 1.0f;
				vertex++;
				corner = right + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = color;
				vertex->u = 0.0f;
				vertex->v = 1.0f;
				vertex++;

				vertices++;
				colors++;
				sizes++;
				normals++;
			}
		} else {
			for ( i = 0; i < particleCount; i++ ) {
				NiPoint3 left = *sizes * downLeft;
				NiPoint3 right = *sizes * downRight;
				NiPoint3 corner;
				NiPoint3 normal = *normals;

				corner = *vertices - left;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 0.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = *vertices - right;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 1.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = left + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 1.0f;
				vertex->v = 1.0f;
				vertex++;
				corner = right + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->nx = normal.x;
				vertex->ny = normal.y;
				vertex->nz = normal.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 0.0f;
				vertex->v = 1.0f;
				vertex++;

				vertices++;
				sizes++;
				normals++;
			}
		}
		vertexBuffer->Unlock();
		device->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, vertexBuffer, 0, vertexCount, indices, particleCount * 6, 0 );
	} else {
		IDirect3DVertexBuffer7 *vertexBuffer = vertexBufferManager->Unknown7CB570(
			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, vertexCount );
		if ( !vertexBuffer ) {
			return;
		}
		NiD3DParticleVertex *vertex = 0;
		DWORD size;
		if ( vertexBuffer->Lock( DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void **)&vertex, &size ) != D3D_OK ) {
			return;
		}
		if ( colors ) {
			for ( i = 0; i < particleCount; i++ ) {
				int a = FloatToInt( colors->a * 255.0f );
				int r = FloatToInt( colors->r * 255.0f );
				int g = FloatToInt( colors->g * 255.0f );
				int b = FloatToInt( colors->b * 255.0f );
				D3DCOLOR color = ( ( ( ( ( a << 8 ) | r ) << 8 ) | g ) << 8 ) | b;
				NiPoint3 left = *sizes * downLeft;
				NiPoint3 right = *sizes * downRight;
				NiPoint3 corner;

				corner = *vertices - left;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = color;
				vertex->u = 0.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = *vertices - right;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = color;
				vertex->u = 1.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = left + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = color;
				vertex->u = 1.0f;
				vertex->v = 1.0f;
				vertex++;
				corner = right + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = color;
				vertex->u = 0.0f;
				vertex->v = 1.0f;
				vertex++;

				vertices++;
				colors++;
				sizes++;
			}
		} else {
			for ( i = 0; i < particleCount; i++ ) {
				NiPoint3 left = *sizes * downLeft;
				NiPoint3 right = *sizes * downRight;
				NiPoint3 corner;

				corner = *vertices - left;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 0.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = *vertices - right;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 1.0f;
				vertex->v = 0.0f;
				vertex++;
				corner = left + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 1.0f;
				vertex->v = 1.0f;
				vertex++;
				corner = right + *vertices;
				vertex->x = corner.x;
				vertex->y = corner.y;
				vertex->z = corner.z;
				vertex->color = 0xFFFFFFFF;
				vertex->u = 0.0f;
				vertex->v = 1.0f;
				vertex++;

				vertices++;
				sizes++;
			}
		}
		vertexBuffer->Unlock();
		device->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, vertexBuffer, 0, vertexCount, indices, particleCount * 6, 0 );
	}
}

/*
================
NiD3DRender::VirtualSlot50

Draws the lines of an NiLinesData: each flagged vertex is joined to the
next one, and a flagged last vertex to the first.
================
*/
// FUNCTION: STBC 0x7C28A0
void NiD3DRender::VirtualSlot50( NiLinesData *data, const NiBound &bound ) {
	if ( unknown2A8 ) {
		unknown10C = bound;
		unsigned short vertexCount = data->GetVertexCount();
		bool *flags = data->GetFlags();
		unsigned short *indices = vertexBufferManager->Unknown7CBBA0( vertexCount * 2 );
		unsigned short *index = indices;
		unsigned int indexCount = 0;
		unsigned int last = vertexCount - 1;
		for ( unsigned int i = 0; i < last; i++ ) {
			if ( *flags ) {
				index[0] = i;
				index[1] = i + 1;
				index += 2;
				indexCount += 2;
			}
			flags++;
		}
		if ( *flags ) {
			index[0] = last;
			index[1] = 0;
			indexCount += 2;
		}
		DrawPrimitive( D3DPT_LINELIST, vertexCount, indices, indexCount, data, 0, 0, 0 );
	}
}

/*
================
NiD3DRender::VirtualSlot51

Draws a triangle fan from a vertex buffer the vertex buffer manager fills.
================
*/
// FUNCTION: STBC 0x7C2960
void NiD3DRender::VirtualSlot51( unsigned int vertexCount, NiPoint3 *vertices, NiPoint2 *textures, NiColorA *colors ) {
	IDirect3DVertexBuffer7 *vertexBuffer = vertexBufferManager->Unknown7CB770( vertexCount, vertices, textures, colors, unknown134, unknown138 );
	if ( vertexBuffer ) {
		DrawPrimitive( D3DPT_TRIANGLEFAN, vertexCount, 0, 0, 0, vertexBuffer, textures != 0, 0 );
	}
}

/*
================
NiD3DRender::VirtualSlot15
================
*/
// FUNCTION: STBC 0x7C29C0
unsigned int NiD3DRender::VirtualSlot15( NiPropertyState *state, NiDynamicEffectState *effects ) {
	return texturePipeline->Unknown7D0A00( *state, effects ) | renderState->Unknown7D23C0( state );
}

/*
================
NiD3DRender::VirtualSlot16
================
*/
// FUNCTION: STBC 0x7C29F0
unsigned int NiD3DRender::VirtualSlot16( NiPropertyState *state ) {
	return renderState->Unknown7D23C0( state );
}

/*
================
NiD3DRender::VirtualSlot53

Hands the image to the texture manager; objects that are not NiDDImages go
as null.
================
*/
// FUNCTION: STBC 0x7C2A10
bool NiD3DRender::VirtualSlot53( NiObject *object ) {
	return textureManager->VirtualSlot02( NiDynamicCast( NiDDImage, object ) );
}

/*
================
NiD3DRender::VirtualSlot28
================
*/
// FUNCTION: STBC 0x7C2A70
bool NiD3DRender::VirtualSlot28( NiObject *object, bool unknown1, bool keep ) {
	return textureManager->VirtualSlot03( NiDynamicCast( NiDDImage, object ), unknown1, keep, -1 );
}

/*
================
NiD3DRender::VirtualSlot29
================
*/
// FUNCTION: STBC 0x7C2AD0
void NiD3DRender::VirtualSlot29( bool kept ) {
	textureManager->VirtualSlot05( 0, kept, -1 );
}

/*
================
NiD3DRender::VirtualSlot30
================
*/
// FUNCTION: STBC 0x7C2AF0
void NiD3DRender::VirtualSlot30( NiImage *image ) {
	textureManager->VirtualSlot05( image, true, -1 );
}

/*
================
NiD3DRender::VirtualSlot14

True when the device has a pixel format of the texture format
(AnimOnly.exe 0x5109D4, NiDirect3D.cpp).
================
*/
// FUNCTION: STBC 0x7C2B10
bool NiD3DRender::VirtualSlot14( NiImage::texFormat_t format ) {
	assert( format < NiImage::TEX_DEFAULT );
	return textureManager->GetUnknown08()[format] ? true : false;
}

/*
================
NiD3DRender::Unknown7C2B30

The current frame buffer's z-buffer, if there is a frame buffer.
================
*/
// FUNCTION: STBC 0x7C2B30
IDirectDrawSurface7 *NiD3DRender::Unknown7C2B30() {
	// AnimOnly.exe 0x510A27 (NiDirect3D.cpp)
	return frameBuffer ? frameBuffer->GetZBuffer() : 0;
}

/*
================
NiD3DRender::GetD3DDevice
================
*/
// FUNCTION: STBC 0x7C2B40
IDirect3DDevice7 *NiD3DRender::GetD3DDevice() {
	return device;
}

/*
================
NiD3DRender::GetTextureFormats

The texture manager's format of each kind. DamageTool.exe declares it
returning const NiPixelDesc ** while the manager keeps NiDDPixelDesc
pointers, so the array is handed out through the base type.
================
*/
// FUNCTION: STBC 0x7C2B50
const NiPixelDesc **NiD3DRender::GetTextureFormats() {
	return (const NiPixelDesc **)textureManager->GetUnknown08();
}

/*
================
NiD3DRender::VirtualSlot13

The device capability flags.
================
*/
// FUNCTION: STBC 0x7C2B60
unsigned int NiD3DRender::VirtualSlot13() {
	return unknown2A4;
}

/*
================
NiD3DRender::VirtualSlot23

Copies a rectangle of an image (all of it without one) into the current
frame buffer's back image at (x, y), clipped to the back image, through
the image's color key when keyed is set.
================
*/
// FUNCTION: STBC 0x7C2B70
void NiD3DRender::VirtualSlot23( NiImage *image, const NiRect<unsigned int> *rect, unsigned int x, unsigned int y, bool keyed ) {
	NiDDImage *ddImage = NiDynamicCast( NiDDImage, image );
	NiDDBufferImage *backImage = (NiDDBufferImage *)VirtualSlot22( 1 );

	IDirectDrawSurface7 *sourceSurface;
	IDirectDrawSurface7 *destSurface;
	if ( ddImage->Unknown7C6270( frameBuffer->GetDirectDraw()->GetDirectDraw(), frameBuffer->GetPixelFormat(), false, sourceSurface ) &&
		backImage->Unknown7C70C0( &destSurface ) ) {
		DDBLTFX fx;
		memset( &fx, 0, sizeof( fx ) );
		fx.dwSize = sizeof( fx );
		fx.ddckSrcColorkey.dwColorSpaceLowValue = fx.ddckSrcColorkey.dwColorSpaceHighValue = 0;

		RECT source;
		if ( rect ) {
			source.left = rect->left;
			source.right = rect->right;
			source.top = rect->top;
			source.bottom = rect->bottom;
			if ( source.left > image->GetWidth() ) {
				source.left = image->GetWidth();
			}
			if ( source.right > image->GetWidth() ) {
				source.right = image->GetWidth();
			}
			if ( source.top > image->GetHeight() ) {
				source.top = image->GetHeight();
			}
			if ( source.bottom > image->GetHeight() ) {
				source.bottom = image->GetHeight();
			}
		} else {
			source.left = source.top = 0;
			source.right = image->GetWidth();
			source.bottom = image->GetHeight();
		}

		RECT dest;
		dest.left = x;
		dest.top = y;
		dest.right = dest.left + ( source.right - source.left );
		dest.bottom = dest.top + ( source.bottom - source.top );
		int over = dest.right - backImage->GetWidth();
		if ( over > 0 ) {
			dest.right -= over;
			source.right -= over;
		}
		over = dest.bottom - backImage->GetHeight();
		if ( over > 0 ) {
			dest.bottom -= over;
			source.bottom -= over;
		}

		if ( keyed ) {
			destSurface->Blt( &dest, sourceSurface, &source, DDBLT_KEYSRCOVERRIDE | DDBLT_WAIT, &fx );
		} else {
			destSurface->Blt( &dest, sourceSurface, &source, DDBLT_WAIT, &fx );
		}
	}
}

/*
================
NiD3DRender::VirtualSlot12
================
*/
// FUNCTION: STBC 0x7C2D30
const char *NiD3DRender::VirtualSlot12() {
	return frameBuffers.GetAt( 0 )->GetDirectDraw()->GetDisplayDevice()->GetName();
}

/*
================
NiD3DRender::VirtualSlot31

The total and free texture memory of the display device.
================
*/
// FUNCTION: STBC 0x7C2D50
bool NiD3DRender::VirtualSlot31( unsigned int *unknown1, unsigned int *unknown2 ) {
	return frameBuffers.GetAt( 0 )->directDraw->GetTextureMemory( unknown1, unknown2 );
}

/*
================
NiD3DRender::VirtualSlot32
================
*/
// FUNCTION: STBC 0x7C2DC0
bool NiD3DRender::VirtualSlot32( unsigned int *unknown1, unsigned int *unknown2, unsigned int *unknown3, unsigned int *unknown4, unsigned int *unknown5 ) {
	return textureManager->GetUnknown8C( unknown1, unknown2, unknown4, unknown5 );
}

/*
================
NiD3DRender::VirtualSlot33
================
*/
// FUNCTION: STBC 0x7C2E00
unsigned int NiD3DRender::VirtualSlot33() {
	return textureManager->GetUnknownA0();
}

/*
================
NiD3DRender::VirtualSlot34
================
*/
// FUNCTION: STBC 0x7C2E10
bool NiD3DRender::VirtualSlot34( unsigned int unknown ) {
	textureManager->SetUnknown88( unknown );
	return true;
}

/*
================
NiD3DRender::VirtualSlot36

Sets both texture manager values that the new value does not exceed the
limit of.
================
*/
// FUNCTION: STBC 0x7C2E30
void NiD3DRender::VirtualSlot36( unsigned int unknown ) {
	// AnimOnly.exe 0x510EC5 (NiDirect3D.cpp; its vtable puts it two slots
	// earlier)
	textureManager->SetUnknown78And7C( unknown );
}

/*
================
NiD3DRender::VirtualSlot37

The two texture manager values, or their limits where they are unset.
================
*/
// FUNCTION: STBC 0x7C2E60
void NiD3DRender::VirtualSlot37( unsigned int *unknown1, unsigned int *unknown2 ) {
	NiTextureManager *manager = textureManager;
	if ( manager->unknown78 ) {
		*unknown1 = manager->unknown78;
	} else {
		*unknown1 = manager->unknown80;
	}
	if ( manager->unknown7C ) {
		*unknown2 = manager->unknown7C;
	} else {
		*unknown2 = manager->unknown84;
	}
}

/*
================
NiD3DRender::VirtualSlot35
================
*/
// FUNCTION: STBC 0x7C2EA0
unsigned int NiD3DRender::VirtualSlot35() {
	// AnimOnly.exe 0x510EF5 (NiDirect3D.cpp; its vtable puts it two slots
	// earlier)
	return textureManager->GetUnknown88();
}

/*
================
NiD3DRender::Unknown7C2EB0

Lets every texture manager drop what it holds for an image.
================
*/
// FUNCTION: STBC 0x7C2EB0
void NiD3DRender::Unknown7C2EB0( NiDDImage *image ) {
	for ( NiTextureManagerList *node = &NiTextureManager::managers; node; node = node->next ) {
		if ( node->item ) {
			node->item->VirtualSlot05( image, true, -1 );
		}
	}
}

/*
================
NiD3DRender::Unknown7C2EE0

The display device the first frame buffer was created on.
================
*/
// FUNCTION: STBC 0x7C2EE0
Ni3DDeviceDesc *NiD3DRender::Unknown7C2EE0() const {
	return frameBuffers.GetAt( 0 )->directDraw->displayDevice;
}

/*
================
NiD3DRender::GetDisplayDevices

Every display device, enumerated on the first call.
================
*/
// FUNCTION: STBC 0x7C2EF0
NiTList<Ni3DDeviceDesc *> *NiD3DRender::GetDisplayDevices() {
	return NiD3DDirectDraw::GetDisplayDevices();
}

/*
================
NiD3DRender::GetWindowedDevice

The device for a window.
================
*/
// FUNCTION: STBC 0x7C2F00
Ni3DDeviceDesc *NiD3DRender::GetWindowedDevice() {
	return NiD3DDirectDraw::GetWindowedDevice();
}

/*
================
NiD3DRender::GetFullscreenDevice

The device for the full screen.
================
*/
// FUNCTION: STBC 0x7C2F20
Ni3DDeviceDesc *NiD3DRender::GetFullscreenDevice() {
	return NiD3DDirectDraw::GetFullscreenDevice();
}

/*
================
NiD3DRender::GetSoftwareDevice

The software device.
================
*/
// FUNCTION: STBC 0x7C2F40
Ni3DDeviceDesc *NiD3DRender::GetSoftwareDevice() {
	return NiD3DDirectDraw::GetSoftwareDevice();
}

/*
================
NiD3DRender::AddFrameBuffer

Gives a new frame buffer on the DirectDraw object a z-buffer like the main
one's, then keeps it at
the first free index past the main frame buffer's. Returns the index, or 0
when the z-buffer can't be made (the frame buffer is released).
================
*/
inline unsigned int NiD3DRender::AddFrameBuffer( NiD3DFrameBuffer *buffer, NiD3DDirectDraw *directDraw ) {
	if ( frameBuffers.GetAt( 0 )->zBuffer ) {
		if ( !buffer->CreateZBuffer( directDraw->GetDisplayDevice()->IsHardware(), &zBufferFormat ) ) {
			buffer->DecRefCount();
			return 0;
		}
	}
	unsigned int index;
	for ( index = 1; index < frameBuffers.GetSize(); index++ ) {
		if ( !frameBuffers.GetAt( index ) ) {
			break;
		}
	}
	frameBuffers.SetAtGrow( index, buffer );
	return index;
}

/*
================
NiD3DRender::CreateOffscreenFrameBuffer

A frame buffer on an offscreen texture surface of the given size, on the
main frame buffer's DirectDraw object. Returns its index, 0 on failure.
================
*/
// FUNCTION: STBC 0x7C2F60
unsigned int NiD3DRender::CreateOffscreenFrameBuffer( unsigned int width, unsigned int height ) {
	NiD3DDirectDraw *directDraw = frameBuffers.GetAt( 0 )->directDraw;
	NiD3DFrameBuffer *buffer = NiD3DFrameBuffer::Create( directDraw, FRAMEBUFFER_OFFSCREEN, 0, width, height, 0, 0, device );
	if ( !buffer ) {
		return 0;
	}
	return AddFrameBuffer( buffer, directDraw );
}

/*
================
NiD3DRender::CreateWindowedFrameBuffer

A frame buffer presented into a window, on the main frame buffer's
DirectDraw object. Returns its index, 0 on failure.
================
*/
// FUNCTION: STBC 0x7C30C0
unsigned int NiD3DRender::CreateWindowedFrameBuffer( HWND window ) {
	NiD3DDirectDraw *directDraw = frameBuffers.GetAt( 0 )->directDraw;
	NiD3DFrameBuffer *buffer = NiD3DFrameBuffer::Create( directDraw, FRAMEBUFFER_WINDOWED, 0, 0, 0, window, 0, 0 );
	if ( !buffer ) {
		return 0;
	}
	return AddFrameBuffer( buffer, directDraw );
}

/*
================
NiD3DRender::RemoveFrameBuffer

Releases an added frame buffer and frees its index; the main frame buffer
(index 0) stays.
================
*/
// FUNCTION: STBC 0x7C3210
void NiD3DRender::RemoveFrameBuffer( unsigned int index ) {
	if ( index && index < frameBuffers.GetSize() && frameBuffers.GetAt( index ) ) {
		frameBuffers.GetAt( index )->DecRefCount();
		frameBuffers.SetAt( index, 0 );
	}
}

/*
================
NiD3DRender::SetFrameBuffer

Renders into the frame buffer at the index from now on, with a camera at
the origin looking down the x axis. False when there is none, or when the
device can't take its back surface (the current one stays).
================
*/
// FUNCTION: STBC 0x7C3290
// PARKED: register choice only: retail loads the vtable into eax (the camera arguments into ecx and edx) and the back surface into the buffer's register; calls, stores and branches match
bool NiD3DRender::SetFrameBuffer( unsigned int index ) {
	if ( index < frameBuffers.GetSize() ) {
		NiD3DFrameBuffer *buffer = frameBuffers.GetAt( index );
		if ( buffer ) {
			NiD3DFrameBuffer *previous = frameBuffer;
			frameBuffer = buffer;
			if ( buffer->Unknown7CAE70() ) {
				Unknown7C45B0();
			} else if ( device->SetRenderTarget( buffer->backSurface, 0 ) != D3D_OK ) {
				frameBuffer = previous;
				return false;
			}
			NiFrustum frustum;
			frustum.left = frustum.bottom = -1.0f;
			frustum.right = frustum.top = 1.0f;
			frustum.nearDistance = 0.1f;
			frustum.farDistance = 1.0f;
			NiRect<float> port( 0.0f, 1.0f, 1.0f, 0.0f );
			VirtualSlot41( NiPoint3::ZERO, NiPoint3::UNIT_X, NiPoint3::UNIT_Y, NiPoint3::UNIT_Z, frustum, port );
			return true;
		}
	}
	return false;
}

/*
================
NiD3DRender::Unknown7C3380

Has the texture pipeline generate reflection coordinates through its
camera frame (Unknown7D0730) or with the fixed sphere map (Unknown7D08A0).
================
*/
// FUNCTION: STBC 0x7C3380
void NiD3DRender::Unknown7C3380( bool unknown ) {
	NiD3DTexturePipeline *pipeline = texturePipeline;
	pipeline->coordGenFunctions[2] = unknown ? &NiD3DTexturePipeline::Unknown7D0730 : &NiD3DTexturePipeline::Unknown7D08A0;

}
/*
================
NiD3DRender::GrabBackBuffer

A new raw image of the current frame buffer's back surface, in the
surface's pixel format; NULL when there is no frame buffer or the surface
can't be read (the image made for it is not freed: an original leak).
================
*/
// FUNCTION: STBC 0x7C33B0
NiRawImageData *NiD3DRender::GrabBackBuffer() {
	if ( frameBuffer ) {
		NiPointer<NiImage> image = frameBuffer->backImage;
		if ( image ) {
			NiRawImageData *raw = new NiRawImageData;
			if ( static_cast<NiDDBufferImage *>( (NiImage *)image )->Unknown7C7160( raw ) ) {
				return raw;
			}
			return 0;
		}
	}
	return 0;
}

/*
================
NiD3DRender::Initialize

Creates the frame buffer and the device, checks the device's capabilities
and creates the managers. With a z-buffer, the device is tried on the
z-buffer formats in the order the flags prefer until one takes.
================
*/
// FUNCTION: STBC 0x7C3480
// PARKED: each early return false gets its own xor al,al in retail; this compiler merges them (also in 0x7C09C0 and the frame buffer creation functions); register choices follow (directDraw and the frame buffer take ebx/edi the other way round)
bool NiD3DRender::Initialize( NiD3DDirectDraw *directDraw, int mode, unsigned int flags, unsigned int width,
								unsigned int height, HWND window, HWND focusWindow,
								IDirectDrawSurface7 *front, IDirectDrawSurface7 *back ) {
	bool multitexture = !( flags & RENDER_NO_MULTITEXTURE );
	int frameBufferMode = FRAMEBUFFER_WINDOWED;
	if ( mode == RENDER_FULLSCREEN ) {
		frameBufferMode = FRAMEBUFFER_FLIPPING;
	}
	if ( front || back ) {
		frameBuffer = NiD3DFrameBuffer::CreateFromSurfaces( directDraw, frameBufferMode, window, front, back );
	} else {
		frameBuffer = NiD3DFrameBuffer::Create( directDraw, frameBufferMode, flags, width, height, window, focusWindow, 0 );
	}
	if ( !frameBuffer ) {
		return false;
	}

	bool hardware = directDraw->displayDevice->hardware;
	frameBuffers.SetAtGrow( 0, frameBuffer );

	const GUID *deviceGuid = directDraw->displayDevice->deviceGuid;
	FindZBufferFormats( deviceGuid );
	DDPIXELFORMAT *zBufferFormats[3];

	if ( flags & ( RENDER_Z_BUFFER | RENDER_Z_BUFFER_DEEP ) ) {
		Message( "(Creation): Attempting to create z-buffer" );
		if ( flags & RENDER_Z_BUFFER_STENCIL ) {
			if ( flags & RENDER_Z_BUFFER_DEEP ) {
				zBufferFormats[0] = &zBufferFormatStencil;
				zBufferFormats[1] = &zBufferFormatDeep;
				zBufferFormats[2] = &zBufferFormat16;
			} else {
				zBufferFormats[0] = &zBufferFormatStencil;
				zBufferFormats[1] = &zBufferFormat16;
				zBufferFormats[2] = &zBufferFormatDeep;
			}
		} else {
			if ( flags & RENDER_Z_BUFFER_DEEP ) {
				zBufferFormats[0] = &zBufferFormatDeep;
				zBufferFormats[1] = &zBufferFormatStencil;
				zBufferFormats[2] = &zBufferFormat16;
			} else {
				zBufferFormats[0] = &zBufferFormat16;
				zBufferFormats[1] = &zBufferFormatDeep;
				zBufferFormats[2] = &zBufferFormatStencil;
			}
		}

		for ( unsigned int i = 0; i < 3; i++ ) {
			device = 0;
			if ( frameBuffer->CreateZBuffer( hardware, zBufferFormats[i] ) ) {
				Message( "(Creation): Attempting to create Direct3D device" );
				if ( direct3D->CreateDevice( *deviceGuid, frameBuffer->GetBackSurface(), &device ) != D3D_OK ) {
					Warning( "(Creation): D3D device creation with attached z-buffer failed" );
					device = 0;
				}
			}
			if ( device ) {
				if ( frameBuffer->zBuffer ) {
					Message( "(Creation): Z-buffer creation successful" );
					DDSURFACEDESC2 desc;
					memset( &desc, 0, sizeof( desc ) );
					desc.dwSize = sizeof( desc );
					frameBuffer->zBuffer->GetSurfaceDesc( &desc );
					zBufferFormat = desc.ddpfPixelFormat;
				}
				break;
			}
			Message( "(Creation): Trying different Z-buffer format" );
			if ( !frameBuffer->ReleaseZBuffer() ) {
				Warning( "Creation failed: Error detaching ZBuffer surface from backbuffer" );
				break;
			}
		}
	} else {
		device = 0;
		Message( "(Creation): Attempting to create Direct3D device" );
		if ( direct3D->CreateDevice( *deviceGuid, frameBuffer->GetBackSurface(), &device ) != D3D_OK ) {
			device = 0;
		}
	}
	if ( !device ) {
		Error( "Creation failed: Cannot create D3D device" );
		return false;
	}
	Message( "(Creation): Direct3D device creation successful" );

	if ( zBufferFormat.dwFlags && ( zBufferFormat.dwFlags & DDPF_STENCILBUFFER ) ) {
		unknown2A4 |= RENDER_CAPS_STENCIL;
		Message( "(Creation): Z-buffer format includes a Stencil buffer" );
	} else {
		unknown2A4 &= ~RENDER_CAPS_STENCIL;
		Message( "(Creation): Z-buffer format does NOT include a Stencil buffer" );
	}

	D3DDEVICEDESC7 deviceDesc;
	memset( &deviceDesc, 0, sizeof( deviceDesc ) );
	device->GetCaps( &deviceDesc );
	Message( "(Creation): Checking Direct3D device for required capabilities" );
	if ( !( deviceDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_PERSPECTIVE )
		|| !( deviceDesc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_COLORGOURAUDRGB ) ) {
		Error( "Creation failed: D3D device cannot support required capabilities" );
		return false;
	}
	Message( "(Creation): Direct3D device has all required capabilities" );

	if ( directDraw->displayDevice->flags & DEVICE_DESC_TRANSFORM_AND_LIGHT ) {
		unknown2A0 = true;
		vertexBufferManager = new NiD3DVertexBufferManager( direct3D, device, false );
	} else {
		unknown2A0 = false;
		vertexBufferManager = new NiD3DVertexBufferManager( direct3D, device, true );
	}
	unknown2A1 = false;
	if ( deviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER ) {
		unknown2A4 |= 0x200;
		Message( "(Creation): Direct3D device supports w-buffering" );
	}

	// the default camera: at the origin, looking down x
	NiFrustum frustum;
	frustum.left = frustum.bottom = -1.0f;
	frustum.right = frustum.top = 1.0f;
	frustum.nearDistance = 0.1f;
	frustum.farDistance = 1.0f;
	VirtualSlot41( NiPoint3::ZERO, NiPoint3::UNIT_X, NiPoint3::UNIT_Y, NiPoint3::UNIT_Z,
		frustum, NiRect<float>( 0.0f, 1.0f, 1.0f, 0.0f ) );

	bool trueColor = frameBuffer->directDraw->GetBitDepth() > 16;
	textureManager = NiD3DTextureManager::Create( multitexture, trueColor, frameBuffer->directDraw->GetDirectDraw(), device,
		&deviceDesc, hardware );
	if ( !textureManager ) {
		return false;
	}
	renderState = NiD3DRenderState::Create( device, &deviceDesc, hardware, frameBuffer->zBuffer != 0,
		( unknown2A4 & RENDER_CAPS_STENCIL ) != 0 );
	if ( !renderState ) {
		return false;
	}
	texturePipeline = NiD3DTexturePipeline::Create( device, &deviceDesc, renderState, textureManager, hardware );
	if ( !texturePipeline ) {
		return false;
	}
	if ( multitexture ) {
		Message( "(Creation): Attempting to initialize multitexturing" );
		texturePipeline->Unknown7CCE50();
	}

	lightManager = new NiD3DLightManager( device );
	unknown2A4 |= 0x04;
	if ( textureManager->TestFlags( TEXTURE_MANAGER_SQUARE ) ) {
		Message( "(Creation): D3D device requires square textures" );
		unknown2A4 |= 0x02;
	}
	if ( textureManager->unknown40[4] || textureManager->unknown40[3] || textureManager->unknown40[5] ) {
		Message( "(Creation): D3D device supports alpha blending" );
		unknown2A4 |= 0x40;
	}
	unknown2A4 |= 0x01;
	if ( trueColor ) {
		Message( "(Creation): D3D device is true color (24-32 bits)" );
		unknown2A4 |= 0x100;
	}
	if ( hardware ) {
		unknown2A4 |= 0x08;
	}
	assert( device );
	return true;
}

/*
================
NiD3DRender::EnumZBufferFormatsCallback

Sorts each z-buffer format by its depth bits, less its stencil bits; a
stencil format wins only with more than one stencil bit. The depth is
compared against the whole bit depth of the format kept (an original
quirk).
================
*/
// FUNCTION: STBC 0x7C3DA0
HRESULT CALLBACK NiD3DRender::EnumZBufferFormatsCallback( LPDDPIXELFORMAT format, LPVOID context ) {
	if ( !( format->dwFlags & DDPF_ZBUFFER ) ) {
		return D3DENUMRET_OK;
	}

	DDPIXELFORMAT **formats = (DDPIXELFORMAT **)context;
	bool stencil = ( format->dwFlags & DDPF_STENCILBUFFER ) != 0;
	DWORD stencilBits = stencil ? format->dwStencilBitDepth : 0;
	DWORD depthBits = format->dwZBufferBitDepth - stencilBits;
	if ( depthBits > 16 ) {
		if ( stencil ) {
			DDPIXELFORMAT *best = formats[2];
			if ( best->dwStencilBitDepth <= 1 ) {
				if ( stencilBits > 1 ) {
					*best = *format;
				}
			} else if ( stencilBits > 1 && depthBits > best->dwZBufferBitDepth ) {
				*best = *format;
			}
		} else if ( depthBits > formats[1]->dwZBufferBitDepth ) {
			*formats[1] = *format;
		}
	} else if ( depthBits > formats[0]->dwZBufferBitDepth ) {
		*formats[0] = *format;
	}
	return D3DENUMRET_OK;
}

/*
================
NiD3DRender::FindZBufferFormats

Keeps the deepest z-buffer format of 16 bits or less, the deepest without
stencil and the deepest with stencil; a kind the device lacks stays
zeroed.
================
*/
// FUNCTION: STBC 0x7C3E50
void NiD3DRender::FindZBufferFormats( const GUID *deviceGuid ) {
	memset( &zBufferFormat16, 0, sizeof( zBufferFormat16 ) );
	memset( &zBufferFormatDeep, 0, sizeof( zBufferFormatDeep ) );
	memset( &zBufferFormatStencil, 0, sizeof( zBufferFormatStencil ) );
	DDPIXELFORMAT *formats[3];
	formats[0] = &zBufferFormat16;
	formats[1] = &zBufferFormatDeep;
	formats[2] = &zBufferFormatStencil;
	direct3D->EnumZBufferFormats( *deviceGuid, EnumZBufferFormatsCallback, formats );
}

/*
================
NiD3DRender::Unknown7C3ED0

The data's cached vertex buffer, refilled when the data's revision
changed since it was made. Data that never changes gets its buffer
optimized for the device when asked; a buffer that was optimized is
remade when it is asked for unoptimized. Shaped as the debug copy
(AnimOnly.exe 0x5121C6, NiDirect3D.cpp: one return of the buffer local),
which also stores the revision of a remade buffer that is NULL; retail's
bytes test the buffer first.
================
*/
// FUNCTION: STBC 0x7C3ED0
IDirect3DVertexBuffer7 *NiD3DRender::Unknown7C3ED0( NiGeometryData *data, bool optimize ) {
	unsigned int id = data->GetID();
	NiD3DGeometryData *entry = 0;
	IDirect3DVertexBuffer7 *vertexBuffer = 0;
	if ( unknownCC.GetAt( id, entry ) ) {
		if ( entry->revision == data->GetRevisionID() ) {
			if ( !optimize && entry->optimized ) {
				if ( entry->vertexBuffer ) {
					entry->vertexBuffer->Release();
				}
				entry->vertexBuffer = vertexBufferManager->Unknown7CB8E0( data, NULL );
				entry->optimized = false;
			}
			vertexBuffer = entry->vertexBuffer;
		} else {
			vertexBuffer = vertexBufferManager->Unknown7CB8E0( data, entry->vertexBuffer );
			if ( entry->vertexBuffer != vertexBuffer ) {
				if ( entry->vertexBuffer ) {
					entry->vertexBuffer->Release();
				}
				entry->vertexBuffer = vertexBuffer;
				if ( optimize && data->IsStatic() && vertexBuffer ) {
					vertexBuffer->Optimize( device, 0 );
					entry->optimized = true;
				} else {
					entry->optimized = false;
				}
			}
			if ( vertexBuffer ) {
				entry->revision = data->GetRevisionID();
			}
		}
	} else {
		vertexBuffer = vertexBufferManager->Unknown7CB8E0( data, NULL );
		entry = new NiD3DGeometryData;
		entry->vertexBuffer = vertexBuffer;
		entry->revision = data->GetRevisionID();
		if ( optimize && data->IsStatic() && vertexBuffer ) {
			vertexBuffer->Optimize( device, 0 );
			entry->optimized = true;
		} else {
			entry->optimized = false;
		}
		unknownCC.SetAt( id, entry );
	}
	return vertexBuffer;
}

/*
================
NiD3DRender::DrawPrimitive

Applies the object's properties and lights, then draws, once per pass of
the texture pipeline.
================
*/
// FUNCTION: STBC 0x7C40D0
void NiD3DRender::DrawPrimitive( D3DPRIMITIVETYPE type, unsigned int vertexCount, unsigned short *indices,
								 unsigned int indexCount, NiGeometryData *data, IDirect3DVertexBuffer7 *vertexBuffer,
								 unsigned int textureSets, NiPoint3 *normals ) {
	if ( !vertexCount ) {
		return;
	}
	if ( !vertexBuffer ) {
		textureSets = data->GetTextureSets();
		normals = data->GetNormals();
	}
	if ( textureSets > 8 ) {
		textureSets = 8;
	}

	renderState->ApplyPropertyState( propertyState );
	bool passes = texturePipeline->Unknown7CD640( *propertyState, effectState, normals != 0, textureSets );
	lightManager->Unknown7D1950( effectState, propertyState->GetTexture()->GetImage() != 0,
		propertyState->GetTextureMode(), propertyState->GetVertexColor() );

	if ( passes && unknown2A1 ) {
		if ( !vertexBuffer ) {
			vertexBuffer = Unknown7C3ED0( data, false );
			if ( !vertexBuffer ) {
				return;
			}
		}
		vertexBuffer = texturePipeline->Unknown7D0170( lightManager->GetLighting(), unknown294, vertexCount, textureSets, vertexBuffer );
		if ( !vertexBuffer ) {
			return;
		}
		bool more = texturePipeline->Unknown7CE040();
		for ( ;; ) {
			if ( indices ) {
				device->DrawIndexedPrimitiveVB( type, vertexBuffer, 0, vertexCount, indices, indexCount, 0 );
			} else {
				device->DrawPrimitiveVB( type, vertexBuffer, 0, vertexCount, 0 );
			}
			if ( !more ) {
				break;
			}
			more = texturePipeline->Unknown7CE040();
		}
		texturePipeline->ResetPasses();
	} else {
		if ( !vertexBuffer ) {
			vertexBuffer = Unknown7C3ED0( data, true );
			if ( !vertexBuffer ) {
				return;
			}
		}
		bool more = texturePipeline->Unknown7CE040();
		for ( ;; ) {
			if ( indices ) {
				device->DrawIndexedPrimitiveVB( type, vertexBuffer, 0, vertexCount, indices, indexCount, 0 );
			} else {
				device->DrawPrimitiveVB( type, vertexBuffer, 0, vertexCount, 0 );
			}
			if ( !more ) {
				break;
			}
			more = texturePipeline->Unknown7CE040();
		}
	}
}

/*
================
NiD3DRender::DrawStrips

Applies the object's properties and lights, then draws each strip from
the data's cached vertex buffer, once per pass of the texture pipeline.
================
*/
// FUNCTION: STBC 0x7C42C0
void NiD3DRender::DrawStrips( D3DPRIMITIVETYPE type, unsigned int vertexCount, unsigned short stripCount,
							  unsigned short *stripLists, unsigned short *stripLengths, NiGeometryData *data ) {
	unsigned int textureSets = data->GetTextureSets();
	if ( textureSets > 8 ) {
		textureSets = 8;
	}
	if ( !vertexCount ) {
		return;
	}

	renderState->ApplyPropertyState( propertyState );
	bool passes = texturePipeline->Unknown7CD640( *propertyState, effectState, data->GetNormals() != 0, textureSets );
	lightManager->Unknown7D1950( effectState, propertyState->GetTexture()->GetImage() != 0,
		propertyState->GetTextureMode(), propertyState->GetVertexColor() );

	IDirect3DVertexBuffer7 *vertexBuffer = Unknown7C3ED0( data, true );
	if ( !vertexBuffer ) {
		return;
	}
	if ( unknown2A1 ) {
		IDirect3DVertexBuffer7 *passBuffer = texturePipeline->Unknown7D0170( lightManager->GetLighting(), unknown294, vertexCount, textureSets, vertexBuffer );
		if ( !passBuffer ) {
			return;
		}
		bool more = texturePipeline->Unknown7CE040();
		for ( ;; ) {
			unsigned short *indices = stripLists;
			for ( unsigned int i = 0; i < stripCount; i++ ) {
				device->DrawIndexedPrimitiveVB( type, passBuffer, 0, vertexCount, indices, stripLengths[i], 0 );
				indices += stripLengths[i];
			}
			if ( !more ) {
				break;
			}
			more = texturePipeline->Unknown7CE040();
		}
		texturePipeline->ResetPasses();
	} else {
		bool more = texturePipeline->Unknown7CE040();
		for ( ;; ) {
			unsigned short *indices = stripLists;
			for ( unsigned int i = 0; i < stripCount; i++ ) {
				device->DrawIndexedPrimitiveVB( type, vertexBuffer, 0, vertexCount, indices, stripLengths[i], 0 );
				indices += stripLengths[i];
			}
			if ( !more ) {
				break;
			}
			more = texturePipeline->Unknown7CE040();
		}
	}
}

/*
================
NiD3DRender::PurgeGeometryData

Drops the vertex buffer cached for geometry data that is being deleted.
================
*/
// FUNCTION: STBC 0x7C44C0
void NiD3DRender::PurgeGeometryData( NiGeometryData *data ) {
	unsigned int id = data->GetID();
	IDirect3DVertexBuffer7 *vertexBuffer;
	NiD3DGeometryData *geometry;
	if ( unknownCC.GetAt( id, geometry ) ) {
		if ( geometry->vertexBuffer ) {
			geometry->vertexBuffer->Release();
		}
		delete geometry;
		unknownCC.RemoveAt( id );
	}
}

/*
================
NiD3DRender::Unknown7C45B0

Once the frame buffer's surfaces were lost (Alt-Tab or minimization),
drops the cached vertex buffers, restores the surfaces and puts the back
surface back under the device. False while the surfaces cannot be used.
The back surface is read through the frame buffer's accessor, as the SDK's
debug NetImmerse 3.1 does (AnimOnly.exe 0x512BE5, NiDirect3D.cpp); BC's
version also checks SetRenderTarget, which 3.1 doesn't.
================
*/
// FUNCTION: STBC 0x7C45B0
bool NiD3DRender::Unknown7C45B0() {
	if ( frameBuffer->Unknown7CAE70() ) {
		NiTMapIterator pos = unknownCC.GetFirstPos();
		NiD3DGeometryData *data = 0;
		while ( pos ) {
			unsigned int key;
			unknownCC.GetNext( pos, key, data );
			if ( data ) {
				if ( data->vertexBuffer && data->vertexBuffer->Release() ) {
					Warning( "(Shutdown): Vertex buffer has extra reference" );
				}
				delete data;
			}
		}
		unknownCC.RemoveAll();

		if ( frameBuffer->Unknown7CAE80() ) {
			Message( "(Runtime): Recovered from Alt-Tab or minimization of application" );
			if ( device->SetRenderTarget( frameBuffer->GetBackSurface(), 0 ) != D3D_OK ) {
				return false;
			}
			textureManager->VirtualSlot07();
		} else {
			return false;
		}
	}
	return true;

}

/*
================
NiD3DRender::CreateObject

Creates a renderer and loads it, with a record of its own.
================
*/
// FUNCTION: STBC 0x7C4740
void NiD3DRender::CreateObject( NiStream &stream, NiObject *&object ) {
	NiD3DRender *render = new NiD3DRender;
	stream.SetLoadRecord( new NiD3DRenderLoadRecord );
	render->LoadBinary( stream );
	object = render;
}

/*
================
NiD3DRender::LoadBinary
================
*/
// FUNCTION: STBC 0x7C4800
void NiD3DRender::LoadBinary( NiStream &stream ) {
	NiRender::LoadBinary( stream );
}

/*
================
NiD3DRender::LinkObject
================
*/
// FUNCTION: STBC 0x7C4810
void NiD3DRender::LinkObject( NiStream &stream ) {
	NiRender::LinkObject( stream );
}

/*
================
NiD3DRender::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7C4820
bool NiD3DRender::RegisterStreamables( NiStream &stream ) {
	return NiRender::RegisterStreamables( stream );
}

/*
================
NiD3DRender::SaveBinary
================
*/
// FUNCTION: STBC 0x7C4830
void NiD3DRender::SaveBinary( NiStream &stream ) {
	NiRender::SaveBinary( stream );
}

/*
================
NiD3DRender::IsEqual
================
*/
// FUNCTION: STBC 0x7C4840
bool NiD3DRender::IsEqual( NiObject *object ) {
	return NiRender::IsEqual( object );
}

/*
================
NiD3DRender::Message
================
*/
// FUNCTION: STBC 0x7C4850
void NiD3DRender::Message( const char *text ) {
	char buffer[256];
	_snprintf( buffer, sizeof( buffer ) - 2, "NI D3D Renderer: %s\n", text );
	NiOutputDebugString( buffer );
}

/*
================
NiD3DRender::Warning
================
*/
// FUNCTION: STBC 0x7C4880
void NiD3DRender::Warning( const char *text ) {
	char buffer[256];
	_snprintf( buffer, sizeof( buffer ) - 2, "NI D3D Renderer WARNING: %s\n", text );
	NiOutputDebugString( buffer );
}

/*
================
NiD3DRender::Error

Logs the error and keeps it as the last error.
================
*/
// FUNCTION: STBC 0x7C48B0
void NiD3DRender::Error( const char *text ) {
	char buffer[256];
	_snprintf( buffer, sizeof( buffer ) - 2, "NI D3D Renderer ERROR: %s\n", text );
	NiOutputDebugString( buffer );
	if ( text ) {
		strncpy( errorString, text, sizeof( errorString ) - 1 );
	} else {
		errorString[0] = '\0';
	}
}

