/*
===============================================================================

	NiD3DRender.h

	The Direct3D 7 implementation of NiRender.

	Evidence: the static initializer at 0x7BFCB0 registers "NiD3DRender"
	with NiRender's NiRTTI as its base; the constructor (0x7BFE40) stores the
	vtable at 0x898984 (NiRender's 55 slots, no new ones) and gives the
	layout (0x2AC bytes, the size both creation functions allocate); the
	destructor (0x7C0EA0) logs each step of the shutdown ("(Shutdown):
	Destroying texture manager", "Releasing D3D device", "Destroying
	framebuffers", ...), which names the helper objects it owns.

	The map at 0xCC instantiates NiTMap<unsigned int, NiD3DGeometryData *>: this unit
	emits its ~NiTMap (0x7C49A0), RemoveAll (0x7C4A10, out of line in the
	map's destructor, inline in the renderer's), KeyToHashIndex (0x7C4A70),
	IsKeysEqual (0x7C4A80), SetValue (0x7C4AA0), ClearValue (0x7C4AC0) and
	scalar deleting destructor (0x7C4B40), all byte-identical to retail;
	their annotations belong above the template definitions in NiTMap.h.

	The helper classes have no strings of their own; their names here are
	invented from the log messages that create and destroy them.

===============================================================================
*/

#ifndef __NID3DRENDER_H__
#define __NID3DRENDER_H__

#include "NiRender.h"
#include "NiStream.h"
#include "NiPoint3.h"
#include "NiBound.h"
#include "NiTMap.h"
#include "NiSystem.h"
#include "NiD3D7.h"
#include "NiD3DFrameBuffer.h"
#include "NiD3DDirectDraw.h"
#include "NiTextureManager.h"
#include "NiD3DTextureManager.h"
#include "NiD3DRenderState.h"
#include "NiD3DTexturePipeline.h"

#include "NiD3DLightManager.h"
#include "NiD3DVertexBufferManager.h"

// an entry of the cached geometry map (name invented): the destructor
// releases the vertex buffer at 0x00 and frees the entry
class NiD3DGeometryData {
public:
	IDirect3DVertexBuffer7 *	vertexBuffer;	// "(Shutdown): Vertex buffer has extra reference"
	unsigned int			revision;		// the data's revision ID (GetRevisionID) the buffer was made for
	bool					optimized;		// the buffer was optimized for the device
};

// the corners of the particle billboards slot 49 draws, with normals
// (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) and without
// (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) (names invented)
struct NiD3DParticleNormalVertex {
	float					x;				// the position
	float					y;
	float					z;
	float					nx;				// the particle's normal
	float					ny;
	float					nz;
	D3DCOLOR				color;
	float					u;
	float					v;
};

struct NiD3DParticleVertex {
	float					x;				// the position
	float					y;
	float					z;
	D3DCOLOR				color;
	float					u;
	float					v;
};

// NiD3DRender::Create modes (names invented)
#define RENDER_WINDOWED		0
#define RENDER_FULLSCREEN	1

// the creation flags (names invented)
#define RENDER_Z_BUFFER				0x01	// a z-buffer
#define RENDER_Z_BUFFER_DEEP		0x02	// a z-buffer, deeper than 16 bits first
#define RENDER_Z_BUFFER_STENCIL		0x04	// with stencil bits first
#define RENDER_NO_MULTITEXTURE		0x40	// leaves multitexturing uninitialized

// NiD3DRender::unknown2A4
#define RENDER_CAPS_STENCIL			0x80	// the z-buffer format has stencil bits

/*
================
NiD3DRenderLoadRecord

The record CreateObject hands the stream (vtable 0x898A78). The class name
is invented.
================
*/
// FUNCTION: STBC 0x7C47D0 ??_GNiD3DRenderLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7C47F0 ??1NiD3DRenderLoadRecord@@UAE@XZ
class NiD3DRenderLoadRecord : public NiObjectLoadRecord {
};

class NiD3DRender : public NiRender {
public:
	// FUNCTION: STBC 0x7C0120 ?GetRTTI@NiD3DRender@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiD3DRender();

	// NiRender overrides
	virtual const char *	VirtualSlot12();
	virtual unsigned int	VirtualSlot13();
	virtual bool			VirtualSlot14( NiImage::texFormat_t format );
	virtual unsigned int	VirtualSlot15( NiPropertyState *state, NiDynamicEffectState *effects );
	virtual unsigned int	VirtualSlot16( NiPropertyState *state );
	virtual bool			VirtualSlot17();
	virtual void			ClearBuffer( const NiRect<float> *rect, unsigned int flags );
	virtual unsigned int	VirtualSlot19();
	virtual unsigned int	VirtualSlot20();
	virtual unsigned int	VirtualSlot21();
	virtual void *			VirtualSlot22( int which );
	virtual void			VirtualSlot23( NiImage *image, const NiRect<unsigned int> *rect, unsigned int x, unsigned int y, bool keyed );
	virtual bool			SetBackgroundColor( const NiColor &color );
	virtual void			VirtualSlot25( unsigned int unknown );
	virtual float			VirtualSlot26();
	virtual void			VirtualSlot27( float unknown );
	virtual bool			VirtualSlot28( NiObject *object, bool unknown1, bool keep );
	virtual void			VirtualSlot29( bool kept );
	virtual void			VirtualSlot30( NiImage *image );
	virtual bool			VirtualSlot31( unsigned int *unknown1, unsigned int *unknown2 );
	virtual bool			VirtualSlot32( unsigned int *unknown1, unsigned int *unknown2, unsigned int *unknown3, unsigned int *unknown4, unsigned int *unknown5 );
	virtual unsigned int	VirtualSlot33();
	virtual bool			VirtualSlot34( unsigned int unknown );
	virtual unsigned int	VirtualSlot35();
	virtual void			VirtualSlot36( unsigned int unknown );
	virtual void			VirtualSlot37( unsigned int *unknown1, unsigned int *unknown2 );
	virtual void			VirtualSlot38( void *unknown );
	virtual void			VirtualSlot39();
	virtual void			PurgeEffect( NiDynamicEffect *effect );
	virtual void			VirtualSlot41( const NiPoint3 &location, const NiPoint3 &direction, const NiPoint3 &up,
										   const NiPoint3 &right, const NiFrustum &frustum, const NiRect<float> &port );
	virtual void			VirtualSlot42( const NiPoint3 &translate, const NiMatrix3 &rotate, float scale );

	virtual void			VirtualSlot43( bool unknown );
	virtual void			VirtualSlot44();
	virtual void			VirtualSlot45( NiTriShapeData *data, const NiBound &bound );

	virtual void			VirtualSlot46( NiTriBasedGeomData *data, const NiBound &bound );
	virtual void			VirtualSlot47( NiTriBasedGeomData *data, const NiBound &bound );

	virtual void			VirtualSlot48( NiTriStripsData *data, const NiBound &bound );
	virtual void			VirtualSlot49( NiParticlesData *data, const NiBound &bound );
	virtual void			VirtualSlot50( NiLinesData *data, const NiBound &bound );
	virtual void			VirtualSlot51( unsigned int vertexCount, NiPoint3 *vertices, NiPoint2 *textures, NiColorA *colors );
	virtual void			VirtualSlot52( NiPlane **planes, unsigned int count, unsigned int sideCount, unsigned int planeMask );
	virtual bool			VirtualSlot53( NiObject *object );
	virtual void			PurgeGeometryData( NiGeometryData *data );

							NiD3DRender();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// a renderer on a display device, the preferred one of its kind when
	// none is given; NULL on failure, with the reason in errorString
	// (name invented)
	static NiD3DRender *	Create( int mode, unsigned int width, unsigned int height, unsigned int flags,
									HWND window, HWND focusWindow, Ni3DDeviceDesc *device );
	// a renderer on the application's DirectDraw object and surfaces; NULL
	// when one is missing or on failure (name invented)
	static NiD3DRender *	CreateFromSurfaces( int mode, unsigned int flags, HWND window, IDirectDraw7 *directDraw,
												IDirectDrawSurface7 *front, IDirectDrawSurface7 *back,
												Ni3DDeviceDesc *device );
	// copies the device of a kind on the application's DirectDraw object
	// and checks it against the mode and the front surface; 0 or an error
	// (name invented; DamageTool.exe names it GetCompatibleDeviceDesc, with
	// ModeType, HW3DClass and CreationError enums, and takes the device by
	// reference)
	static int				SelectDevice( IDirectDraw7 *directDraw, int mode, IDirectDrawSurface7 *front,
										  IDirectDrawSurface7 *back, int kind, Ni3DDeviceDesc &device );
	// queries the Direct3D interface (name invented)
	bool					QueryDirect3D( NiD3DDirectDraw *directDraw );
	// keeps the deepest z-buffer formats of each kind (name invented)
	static HRESULT CALLBACK	EnumZBufferFormatsCallback( LPDDPIXELFORMAT format, LPVOID context );
	// finds the device's z-buffer formats of each kind (name invented)
	void					FindZBufferFormats( const GUID *deviceGuid );
	// creates the frame buffer, on the application's surfaces when given,
	// then the device and the managers (name invented)
	bool					Initialize( NiD3DDirectDraw *directDraw, int mode, unsigned int flags, unsigned int width,
										unsigned int height, HWND window, HWND focusWindow,
										IDirectDrawSurface7 *front, IDirectDrawSurface7 *back );

	IDirect3DDevice7 *		GetD3DDevice();
	IDirectDraw7 *			Unknown7C1320();
	bool					Unknown7C45B0();		// "(Runtime): Recovered from Alt-Tab or minimization of application"
	// the cached vertex buffer of the geometry data, made or refreshed when
	// the data changed
	IDirect3DVertexBuffer7 *	Unknown7C3ED0( NiGeometryData *data, bool optimize );
	// draws the vertices, through the given vertex buffer or the data's
	// cached one, indexed when there are indices, in as many passes as the
	// texture pipeline needs (name invented)
	void					DrawPrimitive( D3DPRIMITIVETYPE type, unsigned int vertexCount, unsigned short *indices,
										   unsigned int indexCount, NiGeometryData *data, IDirect3DVertexBuffer7 *vertexBuffer,
										   unsigned int textureSets, NiPoint3 *normals );
	// draws the strips one after another from the data's cached vertex
	// buffer, in as many passes as the texture pipeline needs (name
	// invented)
	void					DrawStrips( D3DPRIMITIVETYPE type, unsigned int vertexCount, unsigned short stripCount,
										unsigned short *stripLists, unsigned short *stripLengths, NiGeometryData *data );

	// more frame buffers, by index: the main one is 0; 0 also reports a
	// failed creation (names invented)
	unsigned int			CreateOffscreenFrameBuffer( unsigned int width, unsigned int height );
	unsigned int			CreateWindowedFrameBuffer( HWND window );
	void					RemoveFrameBuffer( unsigned int index );
	bool					SetFrameBuffer( unsigned int index );
	// a copy of the current back surface (name invented)
	NiRawImageData *		GrabBackBuffer();

	// picks the coordinate generation of the pipeline's reflection stages
	void					Unknown7C3380( bool unknown );
	void					Unknown7C13E0( bool unknown );

	void					Unknown7C1400( bool unknown );
	IDirectDrawSurface7 *	Unknown7C2B30();
	const NiPixelDesc **	GetTextureFormats();
	Ni3DDeviceDesc *	Unknown7C2EE0() const;

	// hands an image that goes to every texture manager (slot 5); cdecl
	static void				Unknown7C2EB0( NiDDImage *image );

	// NiD3DDirectDraw's display devices, for the application (names
	// invented)
	static NiTList<Ni3DDeviceDesc *> *	GetDisplayDevices();
	static Ni3DDeviceDesc *	GetWindowedDevice();
	static Ni3DDeviceDesc *	GetFullscreenDevice();
	static Ni3DDeviceDesc *	GetSoftwareDevice();

	// log lines on the debugger output: "NI D3D Renderer: %s", the same with
	// WARNING and ERROR; an error is also kept in errorString
	static void				Message( const char *text );
	static void				Warning( const char *text );
	static void				Error( const char *text );

	static const NiRTTI		rtti;
	static char				errorString[256];	// the last error, read by the game when creation fails

protected:
	unsigned int			AddFrameBuffer( NiD3DFrameBuffer *buffer, NiD3DDirectDraw *directDraw );

	IDirect3D7 *			direct3D;		// queried from the DirectDraw object
	IDirect3DDevice7 *		device;			// "(Shutdown): Releasing D3D device"
	NiTArray<NiD3DFrameBuffer *>	frameBuffers;	// "(Shutdown): Destroying framebuffers"
	NiD3DFrameBuffer *		frameBuffer;	// the one being rendered to
	// the device's z-buffer formats with the most depth bits: of 16 bits or
	// less, deeper without stencil, and deeper with stencil
	DDPIXELFORMAT			zBufferFormat16;
	DDPIXELFORMAT			zBufferFormatDeep;
	DDPIXELFORMAT			zBufferFormatStencil;
	DDPIXELFORMAT			zBufferFormat;	// of the z-buffer the device was created with
	NiD3DRenderState *		renderState;		// "render state manager"
	NiD3DTexturePipeline *	texturePipeline;	// "texture pipeline manager"
	NiD3DLightManager *		lightManager;		// the lights of the scene
	NiTextureManager *		textureManager;		// "texture manager"
	NiD3DVertexBufferManager *	vertexBufferManager;
	NiTMap<unsigned int, NiD3DGeometryData *>	unknownCC;	// "cached geometry data" (the value type as AnimOnly.exe 0x512BE5 reads it)
	NiPoint3				unknownDC;		// the camera's right vector; NiPoint3::UNIT_X at construction
	NiPoint3				unknownE8;		// the camera's up vector; NiPoint3::UNIT_Y at construction
	NiPoint3				unknownF4;		// unknownDC, then in the model space of the object drawn, scaled
	NiPoint3				unknown100;		// unknownE8, the same way
	NiBound					unknown10C;
	NiBound					unknown120;
	float					unknown134;		// the camera's near distance; 0.1 at construction
	float					unknown138;		// its far less its near distance; 100.0 at construction
	D3DMATRIX				unknown13C;		// the view transform of slot 44
	D3DMATRIX				unknown17C;		// the projection transform of slot 44
	D3DMATRIX				unknown1BC;		// a projection transform of slot 43
	D3DMATRIX				unknown1FC;		// identity: the view and world transforms of slot 43
	D3DMATRIX				unknown23C;		// a projection transform of slot 43
	D3DVIEWPORT7			viewport;		// set before each clear
	bool					unknown294;		// the camera has active culling planes (slot 52)

	D3DCOLOR				backgroundColor;	// the clear color
	DWORD					clearStencil;		// the stencil clear value
	bool					unknown2A0;		// the device transforms and lights in hardware
	bool					unknown2A1;
	unsigned int			unknown2A4;		// device capability flags
	bool					unknown2A8;		// set while between BeginScene and EndScene
};

#endif /* !__NID3DRENDER_H__ */
