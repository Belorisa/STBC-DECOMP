/*
===============================================================================

	NiD3DRenderState.h

	The renderer's render state manager: a cache of the Direct3D device's
	render states and their capability limits.

	Evidence: the creation function (0x7D2230, "(Creation): Attempting to
	create render state manager") allocates 0x1E8 bytes and runs the
	constructor (0x7D1FF0) with the device and its description; its
	functions set device render states through IDirect3DDevice7::
	SetRenderState (slot 0x50) on the device at 0x04; NiD3DRender releases
	it through the reference count at 0x00 and its scalar deleting
	destructor (0x7C1300). The class name is invented from the log.

	The blend table at 0xF8 is indexed by the alpha property's blend modes
	(ONE, ZERO, SRCCOLOR, INVSRCCOLOR, DESTCOLOR, INVDESTCOLOR, SRCALPHA,
	INVSRCALPHA, DESTALPHA, INVDESTALPHA, SRCALPHASAT, the Gamebryo order)
	and the test table at 0x124 by its test functions (ALWAYS, LESS, EQUAL,
	LESSEQUAL, GREATER, NOTEQUAL, GREATEREQUAL, NEVER). The reset (0x7D2460)
	sets the cached states, each followed by the render state it tracks.

===============================================================================
*/

#ifndef __NID3DRENDERSTATE_H__
#define __NID3DRENDERSTATE_H__

#include "NiD3D7.h"
#include "NiD3DMaterialManager.h"

class NiColor;
class NiMatrix3;
class NiPoint3;
class NiPropertyState;
class NiAlphaProperty;
class NiFogProperty;
class NiStencilProperty;
class NiMaterialProperty;
class NiCorrectionProperty;
class NiDitherProperty;
class NiShadeProperty;
class NiSpecularProperty;
class NiWireframeProperty;
class NiTransparentProperty;
class NiZBufferProperty;

// FUNCTION: STBC 0x7C1300 ??_GNiD3DRenderState@@QAEPAXI@Z
class NiD3DRenderState {
public:
							NiD3DRenderState( IDirect3DDevice7 *device_, const D3DDEVICEDESC7 *deviceDesc_ );
							~NiD3DRenderState();

	// creates the manager for a device; the flags enable optional features
	// (names invented)
	static NiD3DRenderState *	Create( IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc, bool unknown1, bool unknown2, bool unknown3 );

	void					IncRefCount() { refCount++; }

	// releases a reference; the last one deletes the manager
	void					DecRefCount() { if ( --refCount == 0 ) delete this; }

	// the flags set, cleared and tested by mask (names invented): AnimOnly.exe's
	// debug copies 0x520FB0, 0x520FE0 and 0x521040 (D3DRenderState.cpp), which
	// every flag change and test of the manager calls, as NiD3DTexturePipeline's
	void					SetFlag( unsigned int mask ) { flags |= mask; }
	void					ClearFlag( unsigned int mask ) { flags &= ~mask; }
	bool					IsFlagSet( unsigned int mask ) const { return ( mask & flags ) != 0; }
	// the alpha property blend modes the device takes, a bit per mode (names
	// invented; AnimOnly.exe 0x52BB80, 0x52BBA0, which
	// NiD3DTexturePipeline::GetBlendPasses calls)
	unsigned int			GetSourceBlendModes() const { return sourceBlendModes; }
	unsigned int			GetDestBlendModes() const { return destBlendModes; }
	// unknown17C, kept above zero, and unknown178 with it (names invented;
	// AnimOnly.exe 0x5138D0, 0x5138F0)
	float					GetUnknown17C() const { return unknown17C; }
	void					SetUnknown17C( float value ) {
								if ( value <= 0.0f ) {
									value = 1e-5f;
								}
								unknown17C = value;
								unknown178 = 1.0f / value - 1.0f;
							}

	void					CheckCapabilities();
	// the features of a property state the device can't render: 2 for an
	// alpha blend it can't do, 4 for a texture apply mode, 0x200000 for
	// stencilling (the bits as NiD3DRender's slots 15 and 16 return them)
	unsigned int			Unknown7D23C0( NiPropertyState *state );
	void					Reset();
	// sets the device's render states for a property state (name invented)
	void					ApplyPropertyState( NiPropertyState *state );
	// the camera's near and far distances (name invented)
	void					SetCameraRange( float nearDistance_, float farDistance_ ) {
								nearDistance = nearDistance_;
								farDistance = farDistance_;
								depthRange = farDistance_ - nearDistance_;
							}

	void					Unknown7D2940( float scale, const NiMatrix3 &rotate, const NiPoint3 &translate );
	void					SetFogColor( const NiColor &color );	// name invented
	void					ApplyAlpha( NiAlphaProperty *property );		// name invented
	void					ApplyStencil( NiStencilProperty *property );	// name invented
	void					ApplyFog( NiFogProperty *property );			// name invented
	void					ApplyMaterial( NiMaterialProperty *property );	// name invented
	void					ApplyCorrection( NiCorrectionProperty *property );	// name invented
	void					ApplyDither( NiDitherProperty *property );		// name invented
	void					ApplyShade( NiShadeProperty *property );		// name invented
	void					ApplySpecular( NiSpecularProperty *property );	// name invented
	void					ApplyTransparent( NiTransparentProperty *property );	// name invented
	void					ApplyWireframe( NiWireframeProperty *property );	// name invented
	void					ApplyZBuffer( NiZBufferProperty *property );	// name invented
	void					Unknown7D29B0();						// forgets the properties applied, at the start of each frame
	void					Unknown7D3040( bool unknown );		// toggles D3DRENDERSTATE_ANTIALIAS
	void					Unknown7D3070( bool unknown );		// toggles w-buffering (D3DRENDERSTATE_ZENABLE)

	// flags
	enum {
		PIXEL_FOG			= 0x01,		// the device fogs per pixel by z
		UNKNOWN_FLAG_02		= 0x02,		// Create's unknown1
		STENCIL				= 0x04,		// the z-buffer has stencil bits

		UNKNOWN_FLAG_08		= 0x08,		// Create's unknown2
		W_BUFFER			= 0x10		// the device supports w-buffering
	};

	int						refCount;
	IDirect3DDevice7 *		device;
	D3DDEVICEDESC7			deviceDesc;
	unsigned int			flags;
	D3DBLEND				blendModes[11];			// by alpha property blend mode
	D3DCMPFUNC				alphaTestFunctions[8];	// by alpha property test function
	unsigned int			sourceBlendModes;		// a bit per blendModes entry the device takes as a source
	unsigned int			destBlendModes;			// a bit per blendModes entry the device takes as a destination
	D3DBLEND				sourceBlend;			// D3DRENDERSTATE_SRCBLEND as set on the device
	D3DBLEND				destBlend;				// D3DRENDERSTATE_DESTBLEND as set on the device
	D3DCMPFUNC				alphaTestFunction;		// D3DRENDERSTATE_ALPHAFUNC as set on the device
	DWORD					alphaTestReference;		// D3DRENDERSTATE_ALPHAREF as set on the device
	NiAlphaProperty *		alphaProperty;	// the alpha property last applied (0x7D29C0)
	D3DCOLOR				fogColor;		// D3DRENDERSTATE_FOGCOLOR as set on the device
	float					unknown164;		// -1 at construction
	float					unknown168;		// -1 at construction
	float					nearDistance;	// the camera's: fog ends at the far plane
	float					farDistance;
	float					depthRange;		// far less near: scales the fog property's depth

	float					unknown178;		// 1 / unknown17C - 1
	float					unknown17C;		// kept above zero
	bool					fog;			// D3DRENDERSTATE_FOGENABLE as set on the device
	bool					rangeFog;		// D3DRENDERSTATE_RANGEFOGENABLE as set on the device
	NiFogProperty *			fogProperty;	// the fog property last applied (0x7D2B50)
	bool					perspectiveCorrection;	// D3DRENDERSTATE_TEXTUREPERSPECTIVE
	bool					dither;					// D3DRENDERSTATE_DITHERENABLE
	bool					gouraudShading;			// D3DRENDERSTATE_SHADEMODE
	bool					specular;				// D3DRENDERSTATE_SPECULARENABLE
	bool					stencil;				// D3DRENDERSTATE_STENCILENABLE
	bool					colorKey;				// D3DRENDERSTATE_COLORKEYENABLE
	bool					wireframe;				// D3DRENDERSTATE_FILLMODE
	bool					zWrite;					// D3DRENDERSTATE_ZWRITEENABLE
	bool					zTest;					// D3DRENDERSTATE_ZFUNC: LESSEQUAL, else ALWAYS
	bool					alphaBlend;				// D3DRENDERSTATE_ALPHABLENDENABLE
	bool					alphaTest;				// D3DRENDERSTATE_ALPHATESTENABLE
	D3DCMPFUNC				stencilTestFunctions[8];	// by stencil property test function
	D3DSTENCILOP			stencilActions[6];		// by stencil property action
	NiD3DMaterialManager	materials;
	bool					normalizeNormals;	// D3DRENDERSTATE_NORMALIZENORMALS as set on the device
};

#endif /* !__NID3DRENDERSTATE_H__ */
