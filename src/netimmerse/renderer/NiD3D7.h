/*
===============================================================================

	NiD3D7.h

	The DirectX 7 declarations the Direct3D renderer uses.

	The renderer targets DirectX 7: it loads DDRAW.DLL and looks up
	"DirectDrawCreateEx" (DirectX 7's entry point) at 0x7C81BB, and
	queries the DirectDraw object for IID_IDirect3D7
	({F5049E77-4861-11D2-A407-00A0C90629A8}, 0x89DCC0). Its device calls
	land on IDirect3DDevice7's slots: Clear (0x28), SetTransform (0x2C),
	SetViewport (0x34), BeginScene (0x14), EndScene (0x18).

	The compiler's own include directory holds the DirectX 5 headers, which
	supply the types DirectX 7 kept unchanged (D3DVALUE, D3DCOLOR, D3DRECT,
	D3DMATRIX, the transform states). The DirectX 7 SDK itself is not
	available here, so this header declares its additions as the SDK does,
	as far as the renderer's code proves them: each interface is declared up
	to the last method the renderer calls.

===============================================================================
*/

#ifndef __NID3D7_H__
#define __NID3D7_H__

#include <windows.h>
#include <d3d.h>
#include <d3dtypes.h>
#include <d3dcaps.h>

// ddraw.h and d3d.h (DirectX 7): the interfaces and device types added
// after DirectX 5 (the DirectX 5 d3d.h supplies IID_IDirect3DHALDevice and
// IID_IDirect3DRGBDevice)
DEFINE_GUID( IID_IDirectDraw7,				0x15e65ec0,0x3b9c,0x11d2,0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b );
DEFINE_GUID( IID_IDirect3D7,				0xf5049e77,0x4861,0x11d2,0xa4,0x07,0x00,0xa0,0xc9,0x06,0x29,0xa8 );
DEFINE_GUID( IID_IDirect3DTnLHalDevice,		0xf5049e78,0x4861,0x11d2,0xa4,0x07,0x00,0xa0,0xc9,0x06,0x29,0xa8 );

// d3dcaps.h (DirectX 6 and 7): device capabilities added after DirectX 5
#define D3DDEVCAPS_SEPARATETEXTUREMEMORIES	0x00004000L
#define D3DDEVCAPS_HWTRANSFORMANDLIGHT		0x00010000L
#define D3DDEVCAPS_HWRASTERIZATION			0x00080000L

// d3dtypes.h (DirectX 7)
typedef struct _D3DVIEWPORT7 {
	DWORD		dwX;
	DWORD		dwY;			// viewport top left
	DWORD		dwWidth;
	DWORD		dwHeight;		// viewport dimensions
	D3DVALUE	dvMinZ;			// min and max of clip volume
	D3DVALUE	dvMaxZ;
} D3DVIEWPORT7, *LPD3DVIEWPORT7;

// d3d.h (DirectX 7)
#define D3D_OK		DD_OK

// d3dtypes.h (DirectX 7)
#define D3DCLEAR_STENCIL	0x00000004l	// clear stencil planes

// d3dcaps.h (DirectX 6 and 7): rasterization capabilities added after
// DirectX 5
#define D3DPRASTERCAPS_WBUFFER		0x00040000L
#define D3DPRASTERCAPS_ZFOG			0x00100000L

// d3dcaps.h (DirectX 7): the device description GetCaps fills
typedef struct _D3DDeviceDesc7 {
	DWORD			dwDevCaps;					// capabilities of device
	D3DPRIMCAPS		dpcLineCaps;
	D3DPRIMCAPS		dpcTriCaps;
	DWORD			dwDeviceRenderBitDepth;		// one of DDBB_8, 16, etc..
	DWORD			dwDeviceZBufferBitDepth;	// one of DDBD_16, 32, etc..
	DWORD			dwMinTextureWidth, dwMinTextureHeight;
	DWORD			dwMaxTextureWidth, dwMaxTextureHeight;
	DWORD			dwMaxTextureRepeat;
	DWORD			dwMaxTextureAspectRatio;
	DWORD			dwMaxAnisotropy;
	D3DVALUE		dvGuardBandLeft;
	D3DVALUE		dvGuardBandTop;
	D3DVALUE		dvGuardBandRight;
	D3DVALUE		dvGuardBandBottom;
	D3DVALUE		dvExtentsAdjust;
	DWORD			dwStencilCaps;
	DWORD			dwFVFCaps;
	DWORD			dwTextureOpCaps;
	WORD			wMaxTextureBlendStages;
	WORD			wMaxSimultaneousTextures;
	DWORD			dwMaxActiveLights;
	D3DVALUE		dvMaxVertexW;
	GUID			deviceGUID;
	WORD			wMaxUserClipPlanes;
	WORD			wMaxVertexBlendMatrices;
	DWORD			dwVertexProcessingCaps;
	DWORD			dwReserved1;
	DWORD			dwReserved2;
	DWORD			dwReserved3;
	DWORD			dwReserved4;
} D3DDEVICEDESC7, *LPD3DDEVICEDESC7;

// d3dcaps.h (DirectX 6 and 7): texture capabilities added after DirectX 5
#define D3DPTEXTURECAPS_PROJECTED			0x00000400L

#define D3DPTFILTERCAPS_MINFPOINT			0x00000100L
#define D3DPTFILTERCAPS_MINFLINEAR			0x00000200L
#define D3DPTFILTERCAPS_MINFANISOTROPIC		0x00000400L
#define D3DPTFILTERCAPS_MIPFPOINT			0x00010000L
#define D3DPTFILTERCAPS_MIPFLINEAR			0x00020000L
#define D3DPTFILTERCAPS_MAGFPOINT			0x01000000L
#define D3DPTFILTERCAPS_MAGFLINEAR			0x02000000L
#define D3DPTFILTERCAPS_MAGFANISOTROPIC		0x04000000L
#define D3DPTFILTERCAPS_MAGFAFLATCUBIC		0x08000000L
#define D3DPTFILTERCAPS_MAGFGAUSSIANCUBIC	0x10000000L

// d3dtypes.h (DirectX 6 and 7): the texture transform flags
#define D3DTTFF_DISABLE						0
#define D3DTTFF_COUNT2						2
#define D3DTTFF_COUNT3						3
#define D3DTTFF_PROJECTED					256

// d3dtypes.h (DirectX 7): the first texture stage's transform, an
// enumerator of D3DTRANSFORMSTATETYPE after the DirectX 5 ones
#define D3DTRANSFORMSTATE_TEXTURE0			( (D3DTRANSFORMSTATETYPE)16 )

// d3dtypes.h (DirectX 7): texture coordinate generation, ORed into
// D3DTSS_TEXCOORDINDEX
#define D3DTSS_TCI_CAMERASPACENORMAL				0x00010000
#define D3DTSS_TCI_CAMERASPACEPOSITION				0x00020000
#define D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR	0x00030000

// d3dtypes.h (DirectX 6 and 7): the texture stage filters
typedef enum _D3DTEXTUREMAGFILTER {
	D3DTFG_POINT		= 1,	// nearest
	D3DTFG_LINEAR		= 2,	// linear interpolation
	D3DTFG_FLATCUBIC	= 3,	// cubic
	D3DTFG_GAUSSIANCUBIC	= 4,	// different cubic kernel
	D3DTFG_ANISOTROPIC	= 5
} D3DTEXTUREMAGFILTER;

typedef enum _D3DTEXTUREMINFILTER {
	D3DTFN_POINT		= 1,	// nearest
	D3DTFN_LINEAR		= 2,	// linear interpolation
	D3DTFN_ANISOTROPIC	= 3
} D3DTEXTUREMINFILTER;

typedef enum _D3DTEXTUREMIPFILTER {
	D3DTFP_NONE			= 1,	// mipmapping disabled
	D3DTFP_POINT		= 2,	// nearest
	D3DTFP_LINEAR		= 3		// linear interpolation
} D3DTEXTUREMIPFILTER;

// d3dtypes.h (DirectX 7): a light as SetLight takes it
typedef struct _D3DLIGHT7 {
	D3DLIGHTTYPE	dltType;		// type of light source
	D3DCOLORVALUE	dcvDiffuse;		// diffuse color of light
	D3DCOLORVALUE	dcvSpecular;	// specular color of light
	D3DCOLORVALUE	dcvAmbient;		// ambient color of light
	D3DVECTOR		dvPosition;		// position in world space
	D3DVECTOR		dvDirection;	// direction in world space
	D3DVALUE		dvRange;		// cutoff range
	D3DVALUE		dvFalloff;		// falloff
	D3DVALUE		dvAttenuation0;	// constant attenuation
	D3DVALUE		dvAttenuation1;	// linear attenuation
	D3DVALUE		dvAttenuation2;	// quadratic attenuation
	D3DVALUE		dvTheta;		// inner angle of spotlight cone
	D3DVALUE		dvPhi;			// outer angle of spotlight cone
} D3DLIGHT7, *LPD3DLIGHT7;

// d3dtypes.h (DirectX 7): a material as SetMaterial takes it
typedef struct _D3DMATERIAL7 {
	D3DCOLORVALUE	dcvDiffuse;		// diffuse color RGBA
	D3DCOLORVALUE	dcvAmbient;		// ambient color RGB
	D3DCOLORVALUE	dcvSpecular;	// specular 'shininess'
	D3DCOLORVALUE	dcvEmissive;	// emissive color RGB
	D3DVALUE		dvPower;		// sharpness if specular highlight
} D3DMATERIAL7, *LPD3DMATERIAL7;

// d3dtypes.h (DirectX 7): render states added after DirectX 5. The DirectX
// 5 header's D3DRENDERSTATETYPE ends at D3DRENDERSTATE_STIPPLEPATTERN31 and
// cannot be extended, so these are its values.
#define D3DRENDERSTATE_LIGHTING					( (D3DRENDERSTATETYPE)137 )
#define D3DRENDERSTATE_AMBIENT					( (D3DRENDERSTATETYPE)139 )
#define D3DRENDERSTATE_FOGVERTEXMODE			( (D3DRENDERSTATETYPE)140 )
#define D3DRENDERSTATE_NORMALIZENORMALS			( (D3DRENDERSTATETYPE)143 )
#define D3DRENDERSTATE_DIFFUSEMATERIALSOURCE	( (D3DRENDERSTATETYPE)145 )
#define D3DRENDERSTATE_AMBIENTMATERIALSOURCE	( (D3DRENDERSTATETYPE)147 )
#define D3DRENDERSTATE_EMISSIVEMATERIALSOURCE	( (D3DRENDERSTATETYPE)148 )

// d3dtypes.h (DirectX 6): the stencil render states and operations
#define D3DRENDERSTATE_STENCILENABLE			( (D3DRENDERSTATETYPE)52 )
#define D3DRENDERSTATE_STENCILFAIL				( (D3DRENDERSTATETYPE)53 )
#define D3DRENDERSTATE_STENCILZFAIL				( (D3DRENDERSTATETYPE)54 )
#define D3DRENDERSTATE_STENCILPASS				( (D3DRENDERSTATETYPE)55 )
#define D3DRENDERSTATE_STENCILFUNC				( (D3DRENDERSTATETYPE)56 )
#define D3DRENDERSTATE_STENCILREF				( (D3DRENDERSTATETYPE)57 )
#define D3DRENDERSTATE_STENCILMASK				( (D3DRENDERSTATETYPE)58 )

typedef enum _D3DSTENCILOP {
	D3DSTENCILOP_KEEP = 1,
	D3DSTENCILOP_ZERO = 2,
	D3DSTENCILOP_REPLACE = 3,
	D3DSTENCILOP_INCRSAT = 4,
	D3DSTENCILOP_DECRSAT = 5,
	D3DSTENCILOP_INVERT = 6,
	D3DSTENCILOP_INCR = 7,
	D3DSTENCILOP_DECR = 8,
	D3DSTENCILOP_FORCE_DWORD = 0x7fffffff
} D3DSTENCILOP;

// d3dtypes.h (DirectX 6): the values of D3DRENDERSTATE_ZENABLE
typedef enum _D3DZBUFFERTYPE {
	D3DZB_FALSE = 0,
	D3DZB_TRUE = 1,					// z buffering
	D3DZB_USEW = 2,					// w buffering
	D3DZB_FORCE_DWORD = 0x7fffffff
} D3DZBUFFERTYPE;

// d3dtypes.h (DirectX 7)
typedef enum _D3DMATERIALCOLORSOURCE {
	D3DMCS_MATERIAL = 0,			// color from material is used
	D3DMCS_COLOR1 = 1,				// diffuse vertex color is used
	D3DMCS_COLOR2 = 2,				// specular vertex color is used
	D3DMCS_FORCE_DWORD = 0x7fffffff
} D3DMATERIALCOLORSOURCE;

// d3dtypes.h (DirectX 6): the wrapping of the first texture coordinate sets
#define D3DRENDERSTATE_WRAP0					( (D3DRENDERSTATETYPE)128 )
#define D3DRENDERSTATE_WRAP1					( (D3DRENDERSTATETYPE)129 )

// d3dtypes.h (DirectX 6 and 7): the texture stage states; members follow
// as the code proves them
typedef enum _D3DTEXTURESTAGESTATETYPE {
	D3DTSS_COLOROP			= 1,	// D3DTEXTUREOP
	D3DTSS_COLORARG1		= 2,	// D3DTA_*
	D3DTSS_COLORARG2		= 3,	// D3DTA_*
	D3DTSS_ALPHAOP			= 4,	// D3DTEXTUREOP
	D3DTSS_ALPHAARG1		= 5,	// D3DTA_*
	D3DTSS_ALPHAARG2		= 6,	// D3DTA_*
	D3DTSS_TEXCOORDINDEX	= 11,	// the texture coordinate set
	D3DTSS_ADDRESS			= 12,	// D3DTEXTUREADDRESS for both coordinates
	D3DTSS_ADDRESSU			= 13,	// D3DTEXTUREADDRESS for u
	D3DTSS_ADDRESSV			= 14,	// D3DTEXTUREADDRESS for v
	D3DTSS_MAGFILTER		= 16,	// D3DTEXTUREMAGFILTER
	D3DTSS_MINFILTER		= 17,	// D3DTEXTUREMINFILTER
	D3DTSS_MIPFILTER		= 18,	// D3DTEXTUREMIPFILTER
	D3DTSS_TEXTURETRANSFORMFLAGS	= 24,	// D3DTTFF_*
	D3DTSS_FORCE_DWORD = 0x7fffffff
} D3DTEXTURESTAGESTATETYPE;

// d3dtypes.h (DirectX 6): the texture stage operations; members follow as
// the code proves them
typedef enum _D3DTEXTUREOP {
	D3DTOP_DISABLE			= 1,	// disables the stage and every stage after it
	D3DTOP_SELECTARG1		= 2,	// the default
	D3DTOP_SELECTARG2		= 3,
	D3DTOP_MODULATE			= 4,	// multiply the arguments together
	D3DTOP_MODULATE2X		= 5,	// multiply and shift left by 1
	D3DTOP_ADD				= 7,	// add the arguments together
	D3DTOP_BLENDTEXTUREALPHA	= 13,	// blend by the texture's alpha
	D3DTOP_FORCE_DWORD = 0x7fffffff
} D3DTEXTUREOP;

// d3dcaps.h (DirectX 6): the texture operations a device has
#define D3DTEXOPCAPS_SELECTARG2		0x00000004L

// d3dtypes.h (DirectX 6): flexible vertex formats
#define D3DFVF_XYZ				0x002
#define D3DFVF_XYZRHW			0x004
#define D3DFVF_NORMAL			0x010
#define D3DFVF_DIFFUSE			0x040
#define D3DFVF_SPECULAR			0x080
#define D3DFVF_TEX1				0x100
#define D3DFVF_TEXCOUNT_SHIFT	8
#define D3DFVF_TLVERTEX			( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1 )

// d3dtypes.h (DirectX 6): the texture stage arguments
#define D3DTA_DIFFUSE			0x00000000	// the diffuse color
#define D3DTA_CURRENT			0x00000001	// the previous stage's result
#define D3DTA_TEXTURE			0x00000002	// the stage's texture color

typedef enum _D3DSTATEBLOCKTYPE {
	D3DSBT_FORCE_DWORD = 0x7fffffff
} D3DSTATEBLOCKTYPE;

// d3dtypes.h (DirectX 7): vertex buffer capabilities
#define D3DVBCAPS_SYSTEMMEMORY		0x00000800l
#define D3DVBCAPS_WRITEONLY			0x00010000l
#define D3DVBCAPS_OPTIMIZED			0x80000000l

// ddraw.h (DirectX 7): Lock's flag that the old contents may be dropped
#define DDLOCK_DISCARDCONTENTS		0x00002000L

// d3dtypes.h (DirectX 7): CreateVertexBuffer's description
typedef struct _D3DVERTEXBUFFERDESC {
	DWORD			dwSize;
	DWORD			dwCaps;			// D3DVBCAPS_*
	DWORD			dwFVF;
	DWORD			dwNumVertices;
} D3DVERTEXBUFFERDESC, *LPD3DVERTEXBUFFERDESC;

// ddraw.h (DirectX 6): a z-buffer format with stencil bits, whose count
// shares the union of dwRBitMask
#define DDPF_STENCILBUFFER		0x00004000l
#define dwStencilBitDepth		dwRBitMask

interface IDirect3D7;
interface IDirect3DDevice7;
interface IDirect3DVertexBuffer7;
interface IDirectDraw7;
interface IDirectDrawSurface7;

typedef HRESULT (CALLBACK *LPD3DENUMPIXELFORMATSCALLBACK)( LPDDPIXELFORMAT, LPVOID );
typedef HRESULT (CALLBACK *LPD3DENUMDEVICESCALLBACK7)( LPSTR deviceDescription, LPSTR deviceName, LPD3DDEVICEDESC7 desc, LPVOID context );

#undef INTERFACE
#define INTERFACE IDirect3DDevice7

DECLARE_INTERFACE_( IDirect3DDevice7, IUnknown )
{
	/*** IUnknown methods ***/
	STDMETHOD( QueryInterface )( THIS_ REFIID riid, LPVOID *ppvObj ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	/*** IDirect3DDevice7 methods ***/
	STDMETHOD( GetCaps )( THIS_ LPD3DDEVICEDESC7 desc ) PURE;
	STDMETHOD( EnumTextureFormats )( THIS_ LPD3DENUMPIXELFORMATSCALLBACK callback, LPVOID arg ) PURE;
	STDMETHOD( BeginScene )( THIS ) PURE;
	STDMETHOD( EndScene )( THIS ) PURE;
	STDMETHOD( GetDirect3D )( THIS_ IDirect3D7 **d3d ) PURE;
	STDMETHOD( SetRenderTarget )( THIS_ IDirectDrawSurface7 *surface, DWORD flags ) PURE;
	STDMETHOD( GetRenderTarget )( THIS_ IDirectDrawSurface7 **surface ) PURE;
	STDMETHOD( Clear )( THIS_ DWORD count, LPD3DRECT rects, DWORD flags, D3DCOLOR color, D3DVALUE z, DWORD stencil ) PURE;
	STDMETHOD( SetTransform )( THIS_ D3DTRANSFORMSTATETYPE state, LPD3DMATRIX matrix ) PURE;
	STDMETHOD( GetTransform )( THIS_ D3DTRANSFORMSTATETYPE state, LPD3DMATRIX matrix ) PURE;
	STDMETHOD( SetViewport )( THIS_ LPD3DVIEWPORT7 viewport ) PURE;
	STDMETHOD( MultiplyTransform )( THIS_ D3DTRANSFORMSTATETYPE state, LPD3DMATRIX matrix ) PURE;
	STDMETHOD( GetViewport )( THIS_ LPD3DVIEWPORT7 viewport ) PURE;
	STDMETHOD( SetMaterial )( THIS_ LPD3DMATERIAL7 material ) PURE;
	STDMETHOD( GetMaterial )( THIS_ LPD3DMATERIAL7 material ) PURE;
	STDMETHOD( SetLight )( THIS_ DWORD index, LPD3DLIGHT7 light ) PURE;
	STDMETHOD( GetLight )( THIS_ DWORD index, LPD3DLIGHT7 light ) PURE;
	STDMETHOD( SetRenderState )( THIS_ D3DRENDERSTATETYPE state, DWORD value ) PURE;
	STDMETHOD( GetRenderState )( THIS_ D3DRENDERSTATETYPE state, LPDWORD value ) PURE;
	STDMETHOD( BeginStateBlock )( THIS ) PURE;
	STDMETHOD( EndStateBlock )( THIS_ LPDWORD handle ) PURE;
	STDMETHOD( PreLoad )( THIS_ IDirectDrawSurface7 *texture ) PURE;
	STDMETHOD( DrawPrimitive )( THIS_ D3DPRIMITIVETYPE type, DWORD vertexFormat, LPVOID vertices, DWORD vertexCount, DWORD flags ) PURE;
	STDMETHOD( DrawIndexedPrimitive )( THIS_ D3DPRIMITIVETYPE type, DWORD vertexFormat, LPVOID vertices, DWORD vertexCount, LPWORD indices, DWORD indexCount, DWORD flags ) PURE;
	STDMETHOD( SetClipStatus )( THIS_ LPD3DCLIPSTATUS status ) PURE;
	STDMETHOD( GetClipStatus )( THIS_ LPD3DCLIPSTATUS status ) PURE;
	STDMETHOD( DrawPrimitiveStrided )( THIS_ D3DPRIMITIVETYPE type, DWORD vertexFormat, struct _D3DDRAWPRIMITIVESTRIDEDDATA *data, DWORD vertexCount, DWORD flags ) PURE;
	STDMETHOD( DrawIndexedPrimitiveStrided )( THIS_ D3DPRIMITIVETYPE type, DWORD vertexFormat, struct _D3DDRAWPRIMITIVESTRIDEDDATA *data, DWORD vertexCount, LPWORD indices, DWORD indexCount, DWORD flags ) PURE;
	STDMETHOD( DrawPrimitiveVB )( THIS_ D3DPRIMITIVETYPE type, IDirect3DVertexBuffer7 *vertexBuffer, DWORD startVertex, DWORD vertexCount, DWORD flags ) PURE;
	STDMETHOD( DrawIndexedPrimitiveVB )( THIS_ D3DPRIMITIVETYPE type, IDirect3DVertexBuffer7 *vertexBuffer, DWORD startVertex, DWORD vertexCount, LPWORD indices, DWORD indexCount, DWORD flags ) PURE;
	STDMETHOD( ComputeSphereVisibility )( THIS_ LPD3DVECTOR centers, LPD3DVALUE radii, DWORD sphereCount, DWORD flags, LPDWORD results ) PURE;
	STDMETHOD( GetTexture )( THIS_ DWORD stage, IDirectDrawSurface7 **texture ) PURE;
	STDMETHOD( SetTexture )( THIS_ DWORD stage, IDirectDrawSurface7 *texture ) PURE;
	STDMETHOD( GetTextureStageState )( THIS_ DWORD stage, D3DTEXTURESTAGESTATETYPE state, LPDWORD value ) PURE;
	STDMETHOD( SetTextureStageState )( THIS_ DWORD stage, D3DTEXTURESTAGESTATETYPE state, DWORD value ) PURE;
	STDMETHOD( ValidateDevice )( THIS_ LPDWORD passes ) PURE;
	STDMETHOD( ApplyStateBlock )( THIS_ DWORD handle ) PURE;
	STDMETHOD( CaptureStateBlock )( THIS_ DWORD handle ) PURE;
	STDMETHOD( DeleteStateBlock )( THIS_ DWORD handle ) PURE;
	STDMETHOD( CreateStateBlock )( THIS_ D3DSTATEBLOCKTYPE type, LPDWORD handle ) PURE;
	STDMETHOD( Load )( THIS_ IDirectDrawSurface7 *destTexture, LPPOINT destPoint, IDirectDrawSurface7 *sourceTexture, LPRECT sourceRect, DWORD flags ) PURE;
	STDMETHOD( LightEnable )( THIS_ DWORD index, BOOL enable ) PURE;
};

typedef struct IDirect3DDevice7 *LPDIRECT3DDEVICE7;

#undef INTERFACE
#define INTERFACE IDirect3D7

DECLARE_INTERFACE_( IDirect3D7, IUnknown )
{
	/*** IUnknown methods ***/
	STDMETHOD( QueryInterface )( THIS_ REFIID riid, LPVOID *ppvObj ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	/*** IDirect3D7 methods ***/
	STDMETHOD( EnumDevices )( THIS_ LPD3DENUMDEVICESCALLBACK7 callback, LPVOID context ) PURE;
	STDMETHOD( CreateDevice )( THIS_ REFCLSID deviceGuid, IDirectDrawSurface7 *surface, IDirect3DDevice7 **device ) PURE;
	STDMETHOD( CreateVertexBuffer )( THIS_ LPD3DVERTEXBUFFERDESC desc, IDirect3DVertexBuffer7 **vertexBuffer, DWORD flags ) PURE;
	STDMETHOD( EnumZBufferFormats )( THIS_ REFCLSID deviceGuid, LPD3DENUMPIXELFORMATSCALLBACK callback, LPVOID context ) PURE;
};

#undef INTERFACE
#define INTERFACE IDirect3DVertexBuffer7

DECLARE_INTERFACE_( IDirect3DVertexBuffer7, IUnknown )
{
	/*** IUnknown methods ***/
	STDMETHOD( QueryInterface )( THIS_ REFIID riid, LPVOID *ppvObj ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	/*** IDirect3DVertexBuffer7 methods ***/
	STDMETHOD( Lock )( THIS_ DWORD flags, LPVOID *data, LPDWORD size ) PURE;
	STDMETHOD( Unlock )( THIS ) PURE;
	STDMETHOD( ProcessVertices )( THIS_ DWORD vertexOp, DWORD destIndex, DWORD count, IDirect3DVertexBuffer7 *srcBuffer, DWORD srcIndex,
								 IDirect3DDevice7 *device, DWORD flags ) PURE;
	STDMETHOD( GetVertexBufferDesc )( THIS_ LPD3DVERTEXBUFFERDESC desc ) PURE;
	STDMETHOD( Optimize )( THIS_ IDirect3DDevice7 *device, DWORD flags ) PURE;
};

// d3dtypes.h (DirectX 7): ProcessVertices' operations
#define D3DVOP_LIGHT		( 1 << 10 )
#define D3DVOP_TRANSFORM	( 1 << 0 )
#define D3DVOP_CLIP			( 1 << 2 )

// ddraw.h (DirectX 6): SetCooperativeLevel's flag for a multithreaded caller
#define DDSCL_MULTITHREADED		0x00000400l

// ddraw.h (DirectX 7)
#undef INTERFACE
#define INTERFACE IDirectDrawSurface7

DECLARE_INTERFACE_( IDirectDrawSurface7, IUnknown )
{
	/*** IUnknown methods ***/
	STDMETHOD( QueryInterface )( THIS_ REFIID riid, LPVOID *ppvObj ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	/*** IDirectDrawSurface methods ***/
	STDMETHOD( AddAttachedSurface )( THIS_ IDirectDrawSurface7 *surface ) PURE;
	STDMETHOD( AddOverlayDirtyRect )( THIS_ LPRECT rect ) PURE;
	STDMETHOD( Blt )( THIS_ LPRECT destRect, IDirectDrawSurface7 *source, LPRECT sourceRect, DWORD flags, LPDDBLTFX fx ) PURE;
	STDMETHOD( BltBatch )( THIS_ LPDDBLTBATCH batch, DWORD count, DWORD flags ) PURE;
	STDMETHOD( BltFast )( THIS_ DWORD x, DWORD y, IDirectDrawSurface7 *source, LPRECT sourceRect, DWORD trans ) PURE;
	STDMETHOD( DeleteAttachedSurface )( THIS_ DWORD flags, IDirectDrawSurface7 *surface ) PURE;
	STDMETHOD( EnumAttachedSurfaces )( THIS_ LPVOID context, LPVOID callback ) PURE;
	STDMETHOD( EnumOverlayZOrders )( THIS_ DWORD flags, LPVOID context, LPVOID callback ) PURE;
	STDMETHOD( Flip )( THIS_ IDirectDrawSurface7 *target, DWORD flags ) PURE;
	STDMETHOD( GetAttachedSurface )( THIS_ struct _DDSCAPS2 *caps, IDirectDrawSurface7 **surface ) PURE;
	STDMETHOD( GetBltStatus )( THIS_ DWORD flags ) PURE;
	STDMETHOD( GetCaps )( THIS_ struct _DDSCAPS2 *caps ) PURE;
	STDMETHOD( GetClipper )( THIS_ LPDIRECTDRAWCLIPPER *clipper ) PURE;
	STDMETHOD( GetColorKey )( THIS_ DWORD flags, LPDDCOLORKEY colorKey ) PURE;
	STDMETHOD( GetDC )( THIS_ HDC *dc ) PURE;
	STDMETHOD( GetFlipStatus )( THIS_ DWORD flags ) PURE;
	STDMETHOD( GetOverlayPosition )( THIS_ LPLONG x, LPLONG y ) PURE;
	STDMETHOD( GetPalette )( THIS_ LPDIRECTDRAWPALETTE *palette ) PURE;
	STDMETHOD( GetPixelFormat )( THIS_ LPDDPIXELFORMAT format ) PURE;
	STDMETHOD( GetSurfaceDesc )( THIS_ struct _DDSURFACEDESC2 *desc ) PURE;
	STDMETHOD( Initialize )( THIS_ LPDIRECTDRAW directDraw, struct _DDSURFACEDESC2 *desc ) PURE;
	STDMETHOD( IsLost )( THIS ) PURE;
	STDMETHOD( Lock )( THIS_ LPRECT rect, struct _DDSURFACEDESC2 *desc, DWORD flags, HANDLE event ) PURE;
	STDMETHOD( ReleaseDC )( THIS_ HDC dc ) PURE;
	STDMETHOD( Restore )( THIS ) PURE;
	STDMETHOD( SetClipper )( THIS_ LPDIRECTDRAWCLIPPER clipper ) PURE;
	STDMETHOD( SetColorKey )( THIS_ DWORD flags, LPDDCOLORKEY colorKey ) PURE;
	STDMETHOD( SetOverlayPosition )( THIS_ LONG x, LONG y ) PURE;
	STDMETHOD( SetPalette )( THIS_ LPDIRECTDRAWPALETTE palette ) PURE;
	STDMETHOD( Unlock )( THIS_ LPRECT rect ) PURE;
	STDMETHOD( UpdateOverlay )( THIS_ LPRECT sourceRect, IDirectDrawSurface7 *dest, LPRECT destRect, DWORD flags, LPDDOVERLAYFX fx ) PURE;
	STDMETHOD( UpdateOverlayDisplay )( THIS_ DWORD flags ) PURE;
	STDMETHOD( UpdateOverlayZOrder )( THIS_ DWORD flags, IDirectDrawSurface7 *reference ) PURE;

	/*** Added in the v2 interface ***/
	STDMETHOD( GetDDInterface )( THIS_ LPVOID *directDraw ) PURE;
	STDMETHOD( PageLock )( THIS_ DWORD flags ) PURE;
	STDMETHOD( PageUnlock )( THIS_ DWORD flags ) PURE;

	/*** Added in the v3 interface ***/
	STDMETHOD( SetSurfaceDesc )( THIS_ struct _DDSURFACEDESC2 *desc, DWORD flags ) PURE;
};

typedef struct _DDSCAPS2 {
	DWORD		dwCaps;			// capabilities of surface wanted
	DWORD		dwCaps2;
	DWORD		dwCaps3;
	DWORD		dwCaps4;
} DDSCAPS2, *LPDDSCAPS2;

// ddraw.h (DirectX 7): a surface description. The DirectX 5 header's
// DDSURFACEDESC ends with a DDSCAPS; this one carries a DDSCAPS2 and the
// texture stage.
typedef struct _DDSURFACEDESC2 {
	DWORD			dwSize;				// size of the DDSURFACEDESC structure
	DWORD			dwFlags;			// determines what fields are valid
	DWORD			dwHeight;			// height of surface to be created
	DWORD			dwWidth;			// width of input surface
	union {
		LONG		lPitch;				// distance to start of next line (return value only)
		DWORD		dwLinearSize;		// formless late-allocated optimized surface size
	};
	union {
		DWORD		dwBackBufferCount;	// number of back buffers requested
		DWORD		dwDepth;			// the depth if this is a volume texture
	};
	union {
		DWORD		dwMipMapCount;		// number of mip-map levels requested
		DWORD		dwRefreshRate;		// refresh rate (used when display mode is described)
		DWORD		dwSrcVBHandle;		// the source used in VB::Optimize
	};
	DWORD			dwAlphaBitDepth;	// depth of alpha buffer requested
	DWORD			dwReserved;			// reserved
	LPVOID			lpSurface;			// pointer to the associated surface memory
	union {
		DDCOLORKEY	ddckCKDestOverlay;	// color key for destination overlay use
		DWORD		dwEmptyFaceColor;	// physical color for empty cubemap faces
	};
	DDCOLORKEY		ddckCKDestBlt;		// color key for destination blt use
	DDCOLORKEY		ddckCKSrcOverlay;	// color key for source overlay use
	DDCOLORKEY		ddckCKSrcBlt;		// color key for source blt use
	union {
		DDPIXELFORMAT	ddpfPixelFormat;	// pixel format description of the surface
		DWORD		dwFVF;				// vertex format description of vertex buffers
	};
	DDSCAPS2		ddsCaps;			// direct draw surface capabilities
	DWORD			dwTextureStage;		// stage in multitexture cascade
} DDSURFACEDESC2, *LPDDSURFACEDESC2;

// ddraw.h (DirectX 7): dwTextureStage is valid
#define DDSD_TEXTURESTAGE		0x00100000l

typedef HRESULT (FAR PASCAL *LPDDENUMMODESCALLBACK2)( LPDDSURFACEDESC2, LPVOID );

// ddraw.h (DirectX 7): the capabilities GetCaps fills. The DirectX 5
// header's DDCAPS stops at dwNLVBRops; DirectX 6 renamed its DDSCAPS and
// added a DDSCAPS2 at the end.
typedef struct _DDCAPS_DX7 {
	DWORD		dwSize;						// size of the DDDRIVERCAPS structure
	DWORD		dwCaps;						// driver specific capabilities
	DWORD		dwCaps2;					// more driver specific capabilites
	DWORD		dwCKeyCaps;					// color key capabilities of the surface
	DWORD		dwFXCaps;					// driver specific stretching and effects capabilites
	DWORD		dwFXAlphaCaps;				// alpha driver specific capabilities
	DWORD		dwPalCaps;					// palette capabilities
	DWORD		dwSVCaps;					// stereo vision capabilities
	DWORD		dwAlphaBltConstBitDepths;	// DDBD_2,4,8
	DWORD		dwAlphaBltPixelBitDepths;	// DDBD_1,2,4,8
	DWORD		dwAlphaBltSurfaceBitDepths;	// DDBD_1,2,4,8
	DWORD		dwAlphaOverlayConstBitDepths;	// DDBD_2,4,8
	DWORD		dwAlphaOverlayPixelBitDepths;	// DDBD_1,2,4,8
	DWORD		dwAlphaOverlaySurfaceBitDepths;	// DDBD_1,2,4,8
	DWORD		dwZBufferBitDepths;			// DDBD_8,16,24,32
	DWORD		dwVidMemTotal;				// total amount of video memory
	DWORD		dwVidMemFree;				// amount of free video memory
	DWORD		dwMaxVisibleOverlays;		// maximum number of visible overlays
	DWORD		dwCurrVisibleOverlays;		// current number of visible overlays
	DWORD		dwNumFourCCCodes;			// number of four cc codes
	DWORD		dwAlignBoundarySrc;			// source rectangle alignment
	DWORD		dwAlignSizeSrc;				// source rectangle byte size
	DWORD		dwAlignBoundaryDest;		// dest rectangle alignment
	DWORD		dwAlignSizeDest;			// dest rectangle byte size
	DWORD		dwAlignStrideAlign;			// stride alignment
	DWORD		dwRops[DD_ROP_SPACE];		// ROPS supported
	DDSCAPS		ddsOldCaps;					// was DDSCAPS ddsCaps; ddsCaps is of type DDSCAPS2 for DX6
	DWORD		dwMinOverlayStretch;		// minimum overlay stretch factor multiplied by 1000
	DWORD		dwMaxOverlayStretch;		// maximum overlay stretch factor multiplied by 1000
	DWORD		dwMinLiveVideoStretch;		// minimum live video stretch factor multiplied by 1000
	DWORD		dwMaxLiveVideoStretch;		// maximum live video stretch factor multiplied by 1000
	DWORD		dwMinHwCodecStretch;		// minimum hardware codec stretch factor multiplied by 1000
	DWORD		dwMaxHwCodecStretch;		// maximum hardware codec stretch factor multiplied by 1000
	DWORD		dwReserved1;				// reserved
	DWORD		dwReserved2;				// reserved
	DWORD		dwReserved3;				// reserved
	DWORD		dwSVBCaps;					// driver specific capabilities for System->Vmem blts
	DWORD		dwSVBCKeyCaps;				// driver color key capabilities for System->Vmem blts
	DWORD		dwSVBFXCaps;				// driver FX capabilities for System->Vmem blts
	DWORD		dwSVBRops[DD_ROP_SPACE];	// ROPS supported for System->Vmem blts
	DWORD		dwVSBCaps;					// driver specific capabilities for Vmem->System blts
	DWORD		dwVSBCKeyCaps;				// driver color key capabilities for Vmem->System blts
	DWORD		dwVSBFXCaps;				// driver FX capabilities for Vmem->System blts
	DWORD		dwVSBRops[DD_ROP_SPACE];	// ROPS supported for Vmem->System blts
	DWORD		dwSSBCaps;					// driver specific capabilities for System->System blts
	DWORD		dwSSBCKeyCaps;				// driver color key capabilities for System->System blts
	DWORD		dwSSBFXCaps;				// driver FX capabilities for System->System blts
	DWORD		dwSSBRops[DD_ROP_SPACE];	// ROPS supported for System->System blts
	DWORD		dwMaxVideoPorts;			// maximum number of usable video ports
	DWORD		dwCurrVideoPorts;			// current number of video ports used
	DWORD		dwSVBCaps2;					// more driver specific capabilities for System->Vmem blts
	DWORD		dwNLVBCaps;					// driver specific capabilities for non-local->local vidmem blts
	DWORD		dwNLVBCaps2;				// more driver specific capabilities non-local->local vidmem blts
	DWORD		dwNLVBCKeyCaps;				// driver color key capabilities for non-local->local vidmem blts
	DWORD		dwNLVBFXCaps;				// driver FX capabilities for non-local->local blts
	DWORD		dwNLVBRops[DD_ROP_SPACE];	// ROPS supported for non-local->local blts
	DDSCAPS2	ddsCaps;					// surface caps
} DDCAPS_DX7, *LPDDCAPS_DX7;

// ddraw.h (DirectX 6): the driver can render into a window
#define DDCAPS2_CANRENDERWINDOWED	0x00080000l

// ddraw.h (DirectX 7): the identity GetDeviceIdentifier fills
#define MAX_DDDEVICEID_STRING		512

typedef struct tagDDDEVICEIDENTIFIER2 {
	char			szDriver[MAX_DDDEVICEID_STRING];
	char			szDescription[MAX_DDDEVICEID_STRING];
	LARGE_INTEGER	liDriverVersion;	// version of the driver
	DWORD			dwVendorId;
	DWORD			dwDeviceId;
	DWORD			dwSubSysId;
	DWORD			dwRevision;
	GUID			guidDeviceIdentifier;
	DWORD			dwWHQLLevel;
} DDDEVICEIDENTIFIER2, *LPDDDEVICEIDENTIFIER2;

// ddraw.h (DirectX 6 and 7): DirectDrawEnumerateEx, which the DirectX 5
// header declares only with a multi-monitor windows.h
#if !defined( HMONITOR_DECLARED ) && ( WINVER < 0x0500 )
#define HMONITOR_DECLARED
DECLARE_HANDLE( HMONITOR );
#endif
#ifndef SM_CMONITORS
typedef BOOL (FAR PASCAL *LPDDENUMCALLBACKEXA)( GUID FAR *, LPSTR, LPSTR, LPVOID, HMONITOR );
typedef HRESULT (WINAPI *LPDIRECTDRAWENUMERATEEXA)( LPDDENUMCALLBACKEXA lpCallback, LPVOID lpContext, DWORD dwFlags );
#endif

// ddraw.h (DirectX 6 and 7): DirectDrawEnumerateEx's flags
#define DDENUM_ATTACHEDSECONDARYDEVICES		0x00000001L
#define DDENUM_DETACHEDSECONDARYDEVICES		0x00000002L
#define DDENUM_NONDISPLAYDEVICES			0x00000004L

#undef INTERFACE
#define INTERFACE IDirectDraw7

DECLARE_INTERFACE_( IDirectDraw7, IUnknown )
{
	/*** IUnknown methods ***/
	STDMETHOD( QueryInterface )( THIS_ REFIID riid, LPVOID *ppvObj ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	/*** IDirectDraw methods ***/
	STDMETHOD( Compact )( THIS ) PURE;
	STDMETHOD( CreateClipper )( THIS_ DWORD flags, LPDIRECTDRAWCLIPPER *clipper, IUnknown *outer ) PURE;
	STDMETHOD( CreatePalette )( THIS_ DWORD flags, LPPALETTEENTRY entries, LPDIRECTDRAWPALETTE *palette, IUnknown *outer ) PURE;
	STDMETHOD( CreateSurface )( THIS_ struct _DDSURFACEDESC2 *desc, IDirectDrawSurface7 **surface, IUnknown *outer ) PURE;
	STDMETHOD( DuplicateSurface )( THIS_ IDirectDrawSurface7 *surface, IDirectDrawSurface7 **duplicate ) PURE;
	STDMETHOD( EnumDisplayModes )( THIS_ DWORD flags, struct _DDSURFACEDESC2 *desc, LPVOID context, LPDDENUMMODESCALLBACK2 callback ) PURE;
	STDMETHOD( EnumSurfaces )( THIS_ DWORD flags, struct _DDSURFACEDESC2 *desc, LPVOID context, LPVOID callback ) PURE;
	STDMETHOD( FlipToGDISurface )( THIS ) PURE;
	STDMETHOD( GetCaps )( THIS_ LPDDCAPS_DX7 driverCaps, LPDDCAPS_DX7 helCaps ) PURE;
	STDMETHOD( GetDisplayMode )( THIS_ struct _DDSURFACEDESC2 *desc ) PURE;
	STDMETHOD( GetFourCCCodes )( THIS_ LPDWORD count, LPDWORD codes ) PURE;
	STDMETHOD( GetGDISurface )( THIS_ IDirectDrawSurface7 **surface ) PURE;
	STDMETHOD( GetMonitorFrequency )( THIS_ LPDWORD frequency ) PURE;
	STDMETHOD( GetScanLine )( THIS_ LPDWORD scanLine ) PURE;
	STDMETHOD( GetVerticalBlankStatus )( THIS_ LPBOOL inVerticalBlank ) PURE;
	STDMETHOD( Initialize )( THIS_ GUID *guid ) PURE;
	STDMETHOD( RestoreDisplayMode )( THIS ) PURE;
	STDMETHOD( SetCooperativeLevel )( THIS_ HWND window, DWORD flags ) PURE;
	STDMETHOD( SetDisplayMode )( THIS_ DWORD width, DWORD height, DWORD bpp, DWORD refreshRate, DWORD flags ) PURE;
	STDMETHOD( WaitForVerticalBlank )( THIS_ DWORD flags, HANDLE event ) PURE;

	/*** Added in the v2 interface ***/
	STDMETHOD( GetAvailableVidMem )( THIS_ LPDDSCAPS2 caps, LPDWORD total, LPDWORD free ) PURE;

	/*** Added in the V4 interface ***/
	STDMETHOD( GetSurfaceFromDC )( THIS_ HDC dc, IDirectDrawSurface7 **surface ) PURE;
	STDMETHOD( RestoreAllSurfaces )( THIS ) PURE;
	STDMETHOD( TestCooperativeLevel )( THIS ) PURE;

	/*** Added in the V7 interface ***/
	STDMETHOD( GetDeviceIdentifier )( THIS_ LPDDDEVICEIDENTIFIER2 identifier, DWORD flags ) PURE;
};

/*
================
FloatToInt

Rounds through the FPU's own conversion rather than the C cast's call to
_ftol (name invented).
================
*/
static inline int FloatToInt( float value ) {
	int result;
	__asm {
		fld value
		fistp result
	}
	return result;
}

#endif /* !__NID3D7_H__ */
