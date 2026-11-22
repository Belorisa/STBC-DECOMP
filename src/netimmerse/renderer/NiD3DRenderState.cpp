// NiD3DRenderState.cpp -- the renderer's render state manager

#include "NiRTLib.h"
#include <assert.h>
#include "NiD3DRenderState.h"
#include "NiD3DRender.h"
#include "NiColor.h"
#include "NiPropertyState.h"
#include "NiTextureModeProperty.h"
#include "NiAlphaProperty.h"
#include "NiStencilProperty.h"
#include "NiFogProperty.h"
#include "NiCorrectionProperty.h"
#include "NiDitherProperty.h"
#include "NiShadeProperty.h"
#include "NiSpecularProperty.h"
#include "NiWireframeProperty.h"
#include "NiTransparentProperty.h"
#include "NiZBufferProperty.h"
#include "NiAmbientLight.h"
#include "NiDirectionalLight.h"
#include "NiPointLight.h"
#include "NiSpotLight.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d3130 _$E235
// FUNCTION: STBC 0x007d30b0 _$E17
// FUNCTION: STBC 0x007d30d0 _$E15
// FUNCTION: STBC 0x007d30e0 _$E23
// FUNCTION: STBC 0x007d3100 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d3110 _$E30
// FUNCTION: STBC 0x007d3120 _$E65
// FUNCTION: STBC 0x007d3150 _$E69
// FUNCTION: STBC 0x007d3160 _$E73
// FUNCTION: STBC 0x007d3170 _$E77
// FUNCTION: STBC 0x007d3180 _$E101
// FUNCTION: STBC 0x007d3190 _$E136
// FUNCTION: STBC 0x007d31a0 _$E34
// FUNCTION: STBC 0x007d31b0 _$E48
// FUNCTION: STBC 0x007d31c0 _$E61
// FUNCTION: STBC 0x007d31d0 _$E181
// FUNCTION: STBC 0x007d31e0 _$E186
// FUNCTION: STBC 0x007d31f0 _$E191
// FUNCTION: STBC 0x007d3200 _$E131
// FUNCTION: STBC 0x007d3210 _$E120
// FUNCTION: STBC 0x007d3220 _$E111
// FUNCTION: STBC 0x007d3230 _$E161
// FUNCTION: STBC 0x007d3240 _$E106
// FUNCTION: STBC 0x007d3250 _$E146
// FUNCTION: STBC 0x007d3260 _$E151
// FUNCTION: STBC 0x007d3270 _$E171
// FUNCTION: STBC 0x007d3280 _$E176

/*
================
NiD3DRenderState::NiD3DRenderState

Takes a reference to the device and keeps a copy of its description; the
tables translate the alpha property's modes into Direct3D's.
================
*/
// FUNCTION: STBC 0x7D1FF0
NiD3DRenderState::NiD3DRenderState( IDirect3DDevice7 *device_, const D3DDEVICEDESC7 *deviceDesc_ ) {
	refCount = 1;
	device = device_;
	device->AddRef();

	sourceBlendModes = 0;
	destBlendModes = 0;

	blendModes[NiAlphaProperty::ALPHA_ONE] = D3DBLEND_ONE;
	blendModes[NiAlphaProperty::ALPHA_ZERO] = D3DBLEND_ZERO;
	blendModes[NiAlphaProperty::ALPHA_SRCCOLOR] = D3DBLEND_SRCCOLOR;
	blendModes[NiAlphaProperty::ALPHA_INVSRCCOLOR] = D3DBLEND_INVSRCCOLOR;
	blendModes[NiAlphaProperty::ALPHA_DESTCOLOR] = D3DBLEND_DESTCOLOR;
	blendModes[NiAlphaProperty::ALPHA_INVDESTCOLOR] = D3DBLEND_INVDESTCOLOR;
	blendModes[NiAlphaProperty::ALPHA_SRCALPHA] = D3DBLEND_SRCALPHA;
	blendModes[NiAlphaProperty::ALPHA_INVSRCALPHA] = D3DBLEND_INVSRCALPHA;
	blendModes[NiAlphaProperty::ALPHA_DESTALPHA] = D3DBLEND_DESTALPHA;
	blendModes[NiAlphaProperty::ALPHA_INVDESTALPHA] = D3DBLEND_INVDESTALPHA;
	blendModes[NiAlphaProperty::ALPHA_SRCALPHASAT] = D3DBLEND_SRCALPHASAT;

	alphaProperty = 0;

	alphaTestFunctions[0] = D3DCMP_ALWAYS;
	alphaTestFunctions[1] = D3DCMP_LESS;
	alphaTestFunctions[2] = D3DCMP_EQUAL;
	alphaTestFunctions[3] = D3DCMP_LESSEQUAL;
	alphaTestFunctions[4] = D3DCMP_GREATER;
	alphaTestFunctions[5] = D3DCMP_NOTEQUAL;
	alphaTestFunctions[6] = D3DCMP_GREATEREQUAL;
	alphaTestFunctions[7] = D3DCMP_NEVER;

	// the alpha test table's lines, in its order, under the stencil
	// property's test functions (which run from NEVER to ALWAYS)
	stencilTestFunctions[NiStencilProperty::TEST_ALWAYS] = D3DCMP_ALWAYS;
	stencilTestFunctions[NiStencilProperty::TEST_LESS] = D3DCMP_LESS;
	stencilTestFunctions[NiStencilProperty::TEST_EQUAL] = D3DCMP_EQUAL;
	stencilTestFunctions[NiStencilProperty::TEST_LESSEQUAL] = D3DCMP_LESSEQUAL;
	stencilTestFunctions[NiStencilProperty::TEST_GREATER] = D3DCMP_GREATER;
	stencilTestFunctions[NiStencilProperty::TEST_NOTEQUAL] = D3DCMP_NOTEQUAL;
	stencilTestFunctions[NiStencilProperty::TEST_GREATEREQUAL] = D3DCMP_GREATEREQUAL;
	stencilTestFunctions[NiStencilProperty::TEST_NEVER] = D3DCMP_NEVER;

	stencilActions[NiStencilProperty::ACTION_KEEP] = D3DSTENCILOP_KEEP;
	stencilActions[NiStencilProperty::ACTION_ZERO] = D3DSTENCILOP_ZERO;
	stencilActions[NiStencilProperty::ACTION_REPLACE] = D3DSTENCILOP_REPLACE;
	stencilActions[NiStencilProperty::ACTION_INCREMENT] = D3DSTENCILOP_INCRSAT;
	stencilActions[NiStencilProperty::ACTION_DECREMENT] = D3DSTENCILOP_DECRSAT;
	stencilActions[NiStencilProperty::ACTION_INVERT] = D3DSTENCILOP_INVERT;

	fogColor = 0;
	unknown164 = -1.0f;
	unknown168 = -1.0f;
	nearDistance = 0.0f;
	farDistance = 0.0f;
	depthRange = 0.0f;
	unknown178 = 0.0f;
	unknown17C = 1.0f;
	fogProperty = 0;

	deviceDesc = *deviceDesc_;
	flags = 0;
	normalizeNormals = false;
}

/*
================
NiD3DRenderState::~NiD3DRenderState
================
*/
// FUNCTION: STBC 0x7D21D0
NiD3DRenderState::~NiD3DRenderState() {
	if ( device ) {
		device->Release();
		device = 0;
	}
}

/*
================
NiD3DRenderState::Create
================
*/
// FUNCTION: STBC 0x7D2230
NiD3DRenderState *NiD3DRenderState::Create( IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc, bool unknown1, bool unknown2, bool unknown3 ) {
	NiD3DRender::Message( "(Creation): Attempting to create render state manager" );

	NiD3DRenderState *renderState = new NiD3DRenderState( device, deviceDesc );
	assert( renderState );
	if ( unknown1 ) {
		renderState->SetFlag( UNKNOWN_FLAG_02 );
	}
	renderState->CheckCapabilities();
	if ( unknown3 ) {
		renderState->SetFlag( STENCIL );
	}
	if ( unknown2 ) {
		renderState->SetFlag( UNKNOWN_FLAG_08 );
	}
	renderState->Reset();

	NiD3DRender::Message( "(Creation): Render state manager creation succeeded" );
	return renderState;
}

/*
================
NiD3DRenderState::CheckCapabilities

Reads what the device can do: per-pixel fog, the blend modes it takes and
w-buffering.

The blend mode test masks the capability bits with the D3DBLEND value
itself rather than with its D3DPBLENDCAPS bit, so the masks name the wrong
modes; kept as shipped.
================
*/
// FUNCTION: STBC 0x7D2300
void NiD3DRenderState::CheckCapabilities() {
	if ( ( deviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE ) && ( deviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZFOG ) ) {
		NiD3DRender::Message( "(Creation): Device supports per-pixel z-based fog" );
		SetFlag( PIXEL_FOG );
	} else {
		ClearFlag( PIXEL_FOG );
	}

	destBlendModes = 0;
	sourceBlendModes = 0;
	for ( unsigned int i = 0; i < 11; i++ ) {
		if ( blendModes[i] & deviceDesc.dpcTriCaps.dwSrcBlendCaps ) {
			sourceBlendModes |= 1 << i;
		}
		if ( blendModes[i] & deviceDesc.dpcTriCaps.dwDestBlendCaps ) {
			destBlendModes |= 1 << i;
		}
	}

	if ( deviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER ) {
		SetFlag( W_BUFFER );
	} else {
		ClearFlag( W_BUFFER );
	}
}

/*
================
NiD3DRenderState::Unknown7D23C0

Collects the features of a property state the device can't render.
================
*/
// FUNCTION: STBC 0x7D23C0
unsigned int NiD3DRenderState::Unknown7D23C0( NiPropertyState *state ) {
	unsigned int unsupported = 0;

	NiAlphaProperty *alpha = state->GetAlpha();
	if ( alpha->GetAlphaBlending() ) {
		if ( !( sourceBlendModes & ( 1 << alpha->GetSrcBlendMode() ) ) || !( destBlendModes & ( 1 << alpha->GetDestBlendMode() ) ) ) {
			unsupported = 2;
		}
	}

	NiTextureModeProperty *textureMode = state->GetTextureMode();
	if ( textureMode->GetApplyMode() == NiTextureModeProperty::APPLY_MODULATE && !( deviceDesc.dpcTriCaps.dwTextureBlendCaps & D3DPTBLENDCAPS_MODULATEALPHA ) ) {
		unsupported |= 4;
	}
	if ( textureMode->GetApplyMode() == NiTextureModeProperty::APPLY_HILIGHT || textureMode->GetApplyMode() == NiTextureModeProperty::APPLY_HILIGHT2 ) {
		unsupported |= 4;
	}

	if ( state->GetStencil()->GetStencilOn() && !IsFlagSet( STENCIL ) ) {
		unsupported |= 0x200000;
	}
	return unsupported;
}

/*
================
NiD3DRenderState::Reset

Puts the cached states back to their defaults and sets every render state
the manager tracks, plus the ones it leaves fixed, on the device.
================
*/
// FUNCTION: STBC 0x7D2460
void NiD3DRenderState::Reset() {
	perspectiveCorrection = true;
	dither = false;
	gouraudShading = true;
	specular = false;
	stencil = false;
	colorKey = false;
	wireframe = false;
	zWrite = false;
	zTest = false;
	alphaBlend = false;
	sourceBlend = blendModes[6];
	destBlend = blendModes[7];
	alphaTest = false;
	alphaTestFunction = D3DCMP_ALWAYS;
	alphaTestReference = 0;
	fog = false;
	rangeFog = false;
	fogColor = 0;

	device->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CW );
	device->SetRenderState( D3DRENDERSTATE_MONOENABLE, FALSE );
	device->SetRenderState( D3DRENDERSTATE_LASTPIXEL, FALSE );
	device->SetRenderState( D3DRENDERSTATE_STIPPLEENABLE, FALSE );
	device->SetRenderState( D3DRENDERSTATE_STIPPLEDALPHA, FALSE );
	device->SetRenderState( D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE );
	device->SetRenderState( D3DRENDERSTATE_FILLMODE, wireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID );
	device->SetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, perspectiveCorrection );
	device->SetRenderState( D3DRENDERSTATE_SHADEMODE, gouraudShading ? D3DSHADE_GOURAUD : D3DSHADE_FLAT );
	device->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, colorKey );
	device->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	device->SetRenderState( D3DRENDERSTATE_SRCBLEND, sourceBlend );
	device->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlend );
	device->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	device->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, alphaTestFunction );
	device->SetRenderState( D3DRENDERSTATE_ALPHAREF, alphaTestReference );
	if ( zTest ) {
		device->SetRenderState( D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL );
	} else {
		device->SetRenderState( D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS );
	}
	device->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, zWrite );
	device->SetRenderState( D3DRENDERSTATE_DITHERENABLE, dither );
	device->SetRenderState( D3DRENDERSTATE_SPECULARENABLE, specular );
	device->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	if ( IsFlagSet( PIXEL_FOG ) ) {
		device->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_LINEAR );
	} else {
		device->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE );
	}
	device->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_LINEAR );
	device->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, FALSE );
	device->SetRenderState( D3DRENDERSTATE_FOGCOLOR, fogColor );
	device->SetRenderState( D3DRENDERSTATE_NORMALIZENORMALS, FALSE );
	if ( IsFlagSet( W_BUFFER ) ) {
		device->SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_USEW );
	} else {
		device->SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_TRUE );
	}
}

/*
================
NiD3DRenderState::ApplyPropertyState

Sets the device's render states for the properties of a geometry object;
each applier sets only the states that change.
================
*/
// FUNCTION: STBC 0x7D26F0
void NiD3DRenderState::ApplyPropertyState( NiPropertyState *state ) {
	ApplyWireframe( state->GetWireframe() );
	ApplyCorrection( state->GetCorrection() );
	ApplyShade( state->GetShade() );
	ApplyTransparent( state->GetTransparent() );
	ApplyAlpha( state->GetAlpha() );
	ApplyZBuffer( state->GetZBuffer() );
	ApplyFog( state->GetFog() );
	SetFogColor( state->GetFog()->GetFogColor() );
	ApplyDither( state->GetDither() );
	ApplySpecular( state->GetSpecular() );
	ApplyStencil( state->GetStencil() );
	ApplyMaterial( state->GetMaterialAndRevise() );
}

/*
================
NiD3DRenderState::Unknown7D2940

Has the device renormalize lit normals while the scale is more than 1% away
from one; the other arguments are unused.
================
*/
// FUNCTION: STBC 0x7D2940
void NiD3DRenderState::Unknown7D2940( float scale, const NiMatrix3 &rotate, const NiPoint3 &translate ) {
	if ( scale < 0.99f || scale > 1.01f ) {
		if ( !normalizeNormals ) {
			device->SetRenderState( D3DRENDERSTATE_NORMALIZENORMALS, TRUE );
			normalizeNormals = true;
		}
	} else if ( normalizeNormals ) {
		device->SetRenderState( D3DRENDERSTATE_NORMALIZENORMALS, FALSE );
		normalizeNormals = false;
	}
}

/*
================
NiD3DRenderState::Unknown7D29B0

Forgets the alpha and fog properties applied last, so that the next ones
are applied in full; called at the start of each frame.
================
*/
// FUNCTION: STBC 0x7D29B0
void NiD3DRenderState::Unknown7D29B0() {
	fogProperty = 0;
	alphaProperty = 0;
}

/*
================
NiD3DRenderState::ApplyAlpha

Sets the blend and alpha test states of an alpha property, each only when
it changes; the same property twice in a row is skipped.
================
*/
// FUNCTION: STBC 0x7D29C0
void NiD3DRenderState::ApplyAlpha( NiAlphaProperty *property ) {
	if ( property == alphaProperty ) {
		return;
	}
	alphaProperty = property;

	if ( property->GetAlphaBlending() ) {
		if ( !alphaBlend ) {
			device->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
			alphaBlend = true;
		}
		D3DBLEND source = blendModes[property->GetSrcBlendMode()];
		D3DBLEND dest = blendModes[property->GetDestBlendMode()];
		if ( sourceBlend != source ) {
			device->SetRenderState( D3DRENDERSTATE_SRCBLEND, source );
			sourceBlend = source;
		}
		if ( destBlend != dest ) {
			device->SetRenderState( D3DRENDERSTATE_DESTBLEND, dest );
			destBlend = dest;
		}
	} else if ( alphaBlend ) {
		device->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
		alphaBlend = false;
	}

	if ( property->GetAlphaTesting() ) {
		if ( !alphaTest ) {
			device->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
			alphaTest = true;
		}
		D3DCMPFUNC function = alphaTestFunctions[property->GetTestMode()];
		if ( alphaTestFunction != function ) {
			device->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, function );
			alphaTestFunction = function;
		}
		DWORD reference = property->GetTestRef();
		if ( alphaTestReference != reference ) {
			device->SetRenderState( D3DRENDERSTATE_ALPHAREF, reference );
			alphaTestReference = reference;
		}
	} else if ( alphaTest ) {
		device->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, FALSE );
		alphaTest = false;
	}
}

/*
================
NiD3DRenderState::ApplyDither
================
*/
// FUNCTION: STBC 0x7D2B10
void NiD3DRenderState::ApplyDither( NiDitherProperty *property ) {
	bool on = property->GetDithering();
	if ( dither != on ) {
		device->SetRenderState( D3DRENDERSTATE_DITHERENABLE, on );
		dither = on;
	}
}

/*
================
NiD3DRenderState::ApplyFog

Sets the fog states of a fog property: the fog range from its depth, and
z-based (FOG_Z_LINEAR) or range-based (FOG_RANGE_SQ) fog. Any other function
turns fog off. The same property twice in a row is skipped.
================
*/
// FUNCTION: STBC 0x7D2B50
void NiD3DRenderState::ApplyFog( NiFogProperty *property ) {
	if ( property == fogProperty ) {
		return;
	}
	fogProperty = property;

	if ( property->GetFog() ) {
		if ( !fog ) {
			device->SetRenderState( D3DRENDERSTATE_FOGENABLE, TRUE );
			fog = true;
		}

		float depth = property->GetDepth() * depthRange;
		float start = farDistance - depth;
		float end = depth * unknown178 + farDistance;
		if ( unknown164 != start ) {
			unknown164 = start;
			device->SetRenderState( D3DRENDERSTATE_FOGTABLESTART, *(DWORD *)&start );
		}
		if ( unknown168 != end ) {
			unknown168 = end;
			device->SetRenderState( D3DRENDERSTATE_FOGTABLEEND, *(DWORD *)&end );
		}

		if ( property->GetFogFunction() == NiFogProperty::FOG_Z_LINEAR ) {
			// z-based
			if ( rangeFog ) {
				if ( IsFlagSet( PIXEL_FOG ) ) {
					device->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_LINEAR );
					device->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_NONE );
				} else {
					device->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_LINEAR );
				}
				device->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, FALSE );
				rangeFog = false;
			}
		} else if ( property->GetFogFunction() == NiFogProperty::FOG_RANGE_SQ ) {
			// range-based: per vertex only
			if ( !rangeFog ) {
				if ( IsFlagSet( PIXEL_FOG ) ) {
					device->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE );
					device->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_LINEAR );
				}
				device->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, TRUE );
				rangeFog = true;
			}
		} else if ( fog ) {
			device->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
			fog = false;
		}
	} else if ( fog ) {
		device->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		fog = false;
	}
}

/*
================
NiD3DRenderState::SetFogColor
================
*/
// FUNCTION: STBC 0x7D2D10
void NiD3DRenderState::SetFogColor( const NiColor &color ) {
	D3DCOLOR fog = RGB_MAKE( (unsigned char)( color.r * 255.0f ), (unsigned char)( color.g * 255.0f ), (unsigned char)( color.b * 255.0f ) );
	if ( fog != fogColor ) {
		device->SetRenderState( D3DRENDERSTATE_FOGCOLOR, fog );
		fogColor = fog;
	}
}

/*
================
NiD3DRenderState::ApplyMaterial
================
*/
// FUNCTION: STBC 0x7D2D80
void NiD3DRenderState::ApplyMaterial( NiMaterialProperty *property ) {
	materials.SetCurrent( property );
	materials.Apply( device );
}

/*
================
NiD3DRenderState::ApplyCorrection

Perspective correction follows the correction property.
================
*/
// FUNCTION: STBC 0x7D2DB0
void NiD3DRenderState::ApplyCorrection( NiCorrectionProperty *property ) {
	bool on = property->GetCorrect();
	if ( perspectiveCorrection != on ) {
		device->SetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, on );
		perspectiveCorrection = on;
	}
}

/*
================
NiD3DRenderState::ApplyShade
================
*/
// FUNCTION: STBC 0x7D2DF0
void NiD3DRenderState::ApplyShade( NiShadeProperty *property ) {
	bool smooth = property->GetSmooth();
	if ( gouraudShading != smooth ) {
		device->SetRenderState( D3DRENDERSTATE_SHADEMODE, smooth ? D3DSHADE_GOURAUD : D3DSHADE_FLAT );
		gouraudShading = smooth;
	}
}

/*
================
NiD3DRenderState::ApplySpecular
================
*/
// FUNCTION: STBC 0x7D2E30
void NiD3DRenderState::ApplySpecular( NiSpecularProperty *property ) {
	bool on = property->GetSpecular();
	if ( specular != on ) {
		device->SetRenderState( D3DRENDERSTATE_SPECULARENABLE, on );
		specular = on;
	}
}

/*
================
NiD3DRenderState::ApplyStencil

Sets the stencil states of a stencil property; turning the test off is
skipped when it is already off.
================
*/
// FUNCTION: STBC 0x7D2E70
void NiD3DRenderState::ApplyStencil( NiStencilProperty *property ) {
	if ( property->GetStencilOn() ) {
		stencil = true;
		device->SetRenderState( D3DRENDERSTATE_STENCILENABLE, TRUE );
		device->SetRenderState( D3DRENDERSTATE_STENCILFUNC, stencilTestFunctions[property->GetStencilFunction()] );
		device->SetRenderState( D3DRENDERSTATE_STENCILREF, property->GetStencilReference() );
		device->SetRenderState( D3DRENDERSTATE_STENCILMASK, property->GetStencilMask() );
		device->SetRenderState( D3DRENDERSTATE_STENCILFAIL, stencilActions[property->GetStencilFailAction()] );
		device->SetRenderState( D3DRENDERSTATE_STENCILZFAIL, stencilActions[property->GetStencilPassZFailAction()] );
		device->SetRenderState( D3DRENDERSTATE_STENCILPASS, stencilActions[property->GetStencilPassAction()] );
	} else if ( stencil ) {
		stencil = false;
		device->SetRenderState( D3DRENDERSTATE_STENCILENABLE, FALSE );
	}
}

/*
================
NiD3DRenderState::ApplyTransparent

Color keying follows the transparent property.
================
*/
// FUNCTION: STBC 0x7D2F40
void NiD3DRenderState::ApplyTransparent( NiTransparentProperty *property ) {
	bool on = property->GetTransparent();
	if ( colorKey != on ) {
		device->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, on );
		colorKey = on;
	}
}

/*
================
NiD3DRenderState::ApplyWireframe
================
*/
// FUNCTION: STBC 0x7D2F80
void NiD3DRenderState::ApplyWireframe( NiWireframeProperty *property ) {
	bool on = property->GetWireframe();
	if ( on != wireframe ) {
		device->SetRenderState( D3DRENDERSTATE_FILLMODE, on ? D3DFILL_WIREFRAME : D3DFILL_SOLID );
		wireframe = on;
	}
}

/*
================
NiD3DRenderState::ApplyZBuffer

The test compares less-or-equal when on, and always passes when off.
================
*/
// FUNCTION: STBC 0x7D2FC0
void NiD3DRenderState::ApplyZBuffer( NiZBufferProperty *property ) {
	bool test = property->GetZBufferTest();
	bool write = property->GetZBufferWrite();
	if ( zTest != test ) {
		if ( test ) {
			device->SetRenderState( D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL );
		} else {
			device->SetRenderState( D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS );
		}
		zTest = test;
	}
	if ( zWrite != write ) {
		device->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, write );
		zWrite = write;
	}
}

/*
================
NiD3DRenderState::Unknown7D3040

Turns antialiasing on, when the device has sort-independent antialiasing,
or off. The mode set is the sort-dependent one; kept as shipped.
================
*/
// FUNCTION: STBC 0x7D3040
void NiD3DRenderState::Unknown7D3040( bool unknown ) {
	if ( unknown && ( deviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT ) ) {
		device->SetRenderState( D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTDEPENDENT );
	} else {
		device->SetRenderState( D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE );
	}
}

/*
================
NiD3DRenderState::Unknown7D3070

Switches the depth buffer to w-buffering, when the device has it, or back
to z-buffering.
================
*/
// FUNCTION: STBC 0x7D3070
void NiD3DRenderState::Unknown7D3070( bool unknown ) {
	if ( unknown && IsFlagSet( W_BUFFER ) ) {
		device->SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_USEW );
	} else {
		device->SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_TRUE );
	}
}
