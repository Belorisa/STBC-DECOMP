/*
===============================================================================

	NiD3DPixelFormat.h

	A DirectDraw pixel format in the form the renderer works with.

	Evidence: 0x7C7830 fills it from a DDPIXELFORMAT and gives the members:
	the palette flag from DDPF_PALETTEINDEXED8, the bit count from
	dwRGBBitCount, then per channel (red, green, blue, alpha) the mask, the
	number of bits, the bits lost against 8 and the shift of the lowest bit;
	the frame buffers (at 0x20), the DirectDraw images and the texture
	format search call it. The constructor (0x7CAF40) and assignment
	(0x7CAF80) are inline: NiD3DFrameBuffer::CreateSurfaces expands both,
	and the frame buffer's unit keeps out-of-line copies for AttachSurfaces.
	The class name is DamageTool.exe's (NiPixelDesc: its constructor,
	copy, assignment and destructor, and NiRawImageData's conversions that
	take one by const reference); the member names are invented from that
	use, and the file keeps its invented name.

	NiDDPixelDesc is the DirectDraw side of a format, derived from
	NiPixelDesc: DamageTool.exe names its copy constructor (0x7C7600), its
	DDPIXELFORMAT reader (protected InitFromDDPixelFormat, 0x7C7830) and
	its mask reader (protected MaskToValues, 0x7D77F0), and NiDDImage's
	surface functions take it. The renderer keeps every distinct format it
	meets in NiDDPixelDescTable (DamageTool's protected static ms_descs, the
	NiTArray<const NiDDPixelDesc *> at 0x9A1430, built by the texture
	manager's unit), and a format carries its index in that table at 0x24:
	the frame buffer's constructor zeroes it after the base constructor's
	unwind state, the frame buffer creation functions copy it beside the
	inline assignment, and NiDDPixelDescTable::Register (0x7CAFD0) looks a
	format up by its masks, adding a copy when it is new. Index 0 means not
	registered; slot 0 of the table stays empty.

===============================================================================
*/

#ifndef __NID3DPIXELFORMAT_H__
#define __NID3DPIXELFORMAT_H__

#include <string.h>

#include "NiTArray.h"
#include "NiD3D7.h"

class NiPixelDesc {
public:
							NiPixelDesc();
	// a format from its fields; the losses follow from the bits (AnimOnly.exe
	// 0x40E460, which NiRawImageData's static formats call)
							NiPixelDesc( unsigned char bitsPerPixel_, bool palettized_, unsigned int redMask,
										 unsigned int greenMask, unsigned int blueMask, unsigned int alphaMask,
										 unsigned char redBits, unsigned char greenBits, unsigned char blueBits,
										 unsigned char alphaBits, unsigned char redShift, unsigned char greenShift,
										 unsigned char blueShift, unsigned char alphaShift );
	// declared, though empty: NiD3DFrameBuffer's destructor (0x7CABB0) keeps
	// an unwind state for the frame buffer's pixel format; the unwind
	// funclets call the out-of-line copy
	// FUNCTION: STBC 0x7C6C30 ??1NiPixelDesc@@QAE@XZ @netimmerse/renderer/NiDDImage
							~NiPixelDesc() {}
	// guards against self-copy as the assignment does, though it copies
	// unknown20 too
							NiPixelDesc( const NiPixelDesc &format );

	NiPixelDesc &		operator=( const NiPixelDesc &format );
	// palettized formats are all equal; others compare their masks
	bool					operator==( const NiPixelDesc &format ) const;

	// a channel's mask (name invented; AnimOnly.exe 0x40E550, which
	// NiRawImageData.cpp's conversions call)
	unsigned int			GetMask( unsigned int channel ) const { return masks[channel]; }

	bool					palettized;		// true at construction
	unsigned char			bitsPerPixel;
	unsigned int			masks[4];		// red, green, blue, alpha
	unsigned char			bits[4];
	unsigned char			losses[4];		// 8 - bits; all 8 at construction
	unsigned char			shifts[4];
	unsigned int			unknown20;		// not copied by operator=
};

/*
================
NiPixelDesc::NiPixelDesc

Retail keeps an out-of-line copy at 0x7CAF40 for
NiD3DFrameBuffer::AttachSurfaces, which does not inline it.
================
*/
// FUNCTION: STBC 0x7CAF40 ??0NiPixelDesc@@QAE@XZ @netimmerse/renderer/NiD3DFrameBuffer
inline NiPixelDesc::NiPixelDesc() {
	palettized = true;
	bitsPerPixel = 0;
	unknown20 = 0;
	memset( masks, 0, sizeof( masks ) );
	memset( shifts, 0, sizeof( shifts ) );
	memset( bits, 0, sizeof( bits ) );
	losses[0] = losses[1] = losses[2] = losses[3] = 8;
}

/*
================
NiPixelDesc::NiPixelDesc

Written as the SDK's debug NetImmerse 3.1 compiles it (AnimOnly.exe
0x40E460): the fields in order, each loss as 8 less the channel's bits,
and unknown20 cleared. The parameter names are invented.
================
*/
inline NiPixelDesc::NiPixelDesc( unsigned char bitsPerPixel_, bool palettized_, unsigned int redMask,
								 unsigned int greenMask, unsigned int blueMask, unsigned int alphaMask,
								 unsigned char redBits, unsigned char greenBits, unsigned char blueBits,
								 unsigned char alphaBits, unsigned char redShift, unsigned char greenShift,
								 unsigned char blueShift, unsigned char alphaShift ) {
	palettized = palettized_;
	bitsPerPixel = bitsPerPixel_;
	masks[0] = redMask;
	masks[1] = greenMask;
	masks[2] = blueMask;
	masks[3] = alphaMask;
	bits[0] = redBits;
	bits[1] = greenBits;
	bits[2] = blueBits;
	bits[3] = alphaBits;
	losses[0] = 8 - redBits;
	losses[1] = 8 - greenBits;
	losses[2] = 8 - blueBits;
	losses[3] = 8 - alphaBits;
	shifts[0] = redShift;
	shifts[1] = greenShift;
	shifts[2] = blueShift;
	shifts[3] = alphaShift;
	unknown20 = 0;
}

/*
================
NiPixelDesc::NiPixelDesc
================
*/
// FUNCTION: STBC 0x7C7520 ??0NiPixelDesc@@QAE@ABV0@@Z @netimmerse/renderer/NiDDImage
inline NiPixelDesc::NiPixelDesc( const NiPixelDesc &format ) {
	if ( this != &format ) {
		palettized = format.palettized;
		bitsPerPixel = format.bitsPerPixel;
		masks[0] = format.masks[0];
		masks[1] = format.masks[1];
		masks[2] = format.masks[2];
		masks[3] = format.masks[3];
		bits[0] = format.bits[0];
		bits[1] = format.bits[1];
		bits[2] = format.bits[2];
		bits[3] = format.bits[3];
		losses[0] = format.losses[0];
		losses[1] = format.losses[1];
		losses[2] = format.losses[2];
		losses[3] = format.losses[3];
		shifts[0] = format.shifts[0];
		shifts[1] = format.shifts[1];
		shifts[2] = format.shifts[2];
		shifts[3] = format.shifts[3];
		unknown20 = format.unknown20;
	}
}

/*
================
NiPixelDesc::operator=

unknown20 is not copied. Retail keeps the out-of-line copy at 0x7CAF80 for
NiD3DFrameBuffer::AttachSurfaces, which does not inline it.
================
*/
// FUNCTION: STBC 0x7CAF80 ??4NiPixelDesc@@QAEAAV0@ABV0@@Z @netimmerse/renderer/NiD3DFrameBuffer
inline NiPixelDesc &NiPixelDesc::operator=( const NiPixelDesc &format ) {
	if ( this != &format ) {
		palettized = format.palettized;
		bitsPerPixel = format.bitsPerPixel;
		memcpy( masks, format.masks, sizeof( masks ) );
		memcpy( bits, format.bits, sizeof( bits ) );
		memcpy( shifts, format.shifts, sizeof( shifts ) );
		memcpy( losses, format.losses, sizeof( losses ) );
	}
	return *this;
}

/*
================
NiPixelDesc::operator==
================
*/
inline bool NiPixelDesc::operator==( const NiPixelDesc &format ) const {
	return ( palettized && format.palettized ) || memcmp( masks, format.masks, sizeof( masks ) ) == 0;
}

// the implicit copy constructor; Register's new entry copies the format
// FUNCTION: STBC 0x7C7600 ??0NiDDPixelDesc@@QAE@ABV0@@Z @netimmerse/renderer/NiDDImage
class NiDDPixelDesc : public NiPixelDesc {
public:
							NiDDPixelDesc() { index = 0; }
							NiDDPixelDesc( DDPIXELFORMAT *format ) { index = 0; InitFromDDPixelFormat( format ); }
	// the format of a surface; palettized with no channels when the
	// surface is NULL or does not give its format
							NiDDPixelDesc( IDirectDrawSurface7 *surface );

	// the DirectDraw form of the format (name invented)
	void					GetDDPixelFormat( DDPIXELFORMAT *format ) const;

	// the index in the table (name invented; AnimOnly.exe 0x5185C0, which
	// NiDDImage.cpp's surface functions call)
	unsigned int			GetIndex() const { return index; }

	// in NiDDPixelDescTable; 0 until registered. Register takes the format
	// by const reference (DamageTool's decorated name) and stores it (0x7CAFD0)
	mutable unsigned int	index;

protected:
	// takes the format of a surface
	void					InitFromDDPixelFormat( DDPIXELFORMAT *format );
	// the shift and bit count of a channel mask; uses no member
	void					MaskToValues( unsigned long mask, unsigned char &shift, unsigned char &bitCount );
};

// every distinct format met; slot 0 unused
class NiDDPixelDescTable {
public:
	// the format's index, adding a copy of it when no entry is equal
	static unsigned int		Register( const NiDDPixelDesc &desc );

protected:
	// NiDDImage::Save reads an entry directly (retail 0x7C6650 indexes the
	// array inline), so the class grants it access (friend invented)
	friend class NiDDImage;
	// deletes the entries at exit (NiD3DPixelFormat.cpp; friend invented)
	friend class NiDDPixelDescTableCleanup;

	static NiTArray<const NiDDPixelDesc *>	descs;
};

/*
================
NiDDPixelDesc::GetDDPixelFormat
================
*/
inline void NiDDPixelDesc::GetDDPixelFormat( DDPIXELFORMAT *format ) const {
	if ( !format ) {
		return;
	}

	memset( format, 0, sizeof( *format ) );
	format->dwSize = sizeof( *format );
	format->dwFlags = DDPF_RGB;
	if ( palettized ) {
		format->dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
	}
	if ( masks[3] ) {
		format->dwFlags |= DDPF_ALPHAPIXELS;
	}
	format->dwRGBBitCount = bitsPerPixel;
	format->dwRBitMask = masks[0];
	format->dwGBitMask = masks[1];
	format->dwBBitMask = masks[2];
	format->dwRGBAlphaBitMask = masks[3];
}

/*
================
NiDDPixelDesc::MaskToValues

The shift of a mask's lowest set bit and the count of set bits from it.
The texture format callback 0x7D3960 calls the out-of-line copy 0x7D77F0
for its last two channels.
================
*/
// FUNCTION: STBC 0x7D77F0 ?MaskToValues@NiDDPixelDesc@@IAEXKAAE0@Z @netimmerse/renderer/NiTextureManager
inline void NiDDPixelDesc::MaskToValues( unsigned long mask, unsigned char &shift, unsigned char &bitCount ) {
	if ( !mask ) {
		shift = 0;
		bitCount = 0;
	}

	int bit;
	for ( bit = 0; bit < 32; bit++ ) {
		if ( mask & 1 ) {
			break;
		}
		mask >>= 1;
	}
	shift = bit;

	bitCount = 0;
	for ( ; bit <= 32; bit++ ) {
		if ( !( mask & 1 ) ) {
			break;
		}
		bitCount++;
		mask >>= 1;
	}
}

/*
================
NiDDPixelDesc::InitFromDDPixelFormat

A palettized format keeps no channel masks; an absent alpha channel has no
bits. Each channel written out, as the debug copy has it (AnimOnly.exe
0x518700).
================
*/
// FUNCTION: STBC 0x7C7830 ?InitFromDDPixelFormat@NiDDPixelDesc@@IAEXPAU_DDPIXELFORMAT@@@Z @netimmerse/renderer/NiDDImage
inline void NiDDPixelDesc::InitFromDDPixelFormat( DDPIXELFORMAT *format ) {
	if ( !format ) {
		return;
	}

	bitsPerPixel = (unsigned char)format->dwRGBBitCount;
	if ( format->dwFlags & DDPF_PALETTEINDEXED8 ) {
		palettized = true;
		memset( masks, 0, sizeof( masks ) );
		memset( bits, 0, sizeof( bits ) );
		memset( shifts, 0, sizeof( shifts ) );
		losses[0] = losses[1] = losses[2] = losses[3] = 8;
	} else {
		palettized = false;
		masks[0] = format->dwRBitMask;
		MaskToValues( masks[0], shifts[0], bits[0] );
		losses[0] = 8 - bits[0];
		masks[1] = format->dwGBitMask;
		MaskToValues( masks[1], shifts[1], bits[1] );
		losses[1] = 8 - bits[1];
		masks[2] = format->dwBBitMask;
		MaskToValues( masks[2], shifts[2], bits[2] );
		losses[2] = 8 - bits[2];
		if ( format->dwFlags & DDPF_ALPHAPIXELS ) {
			masks[3] = format->dwRGBAlphaBitMask;
			MaskToValues( masks[3], shifts[3], bits[3] );
			losses[3] = 8 - bits[3];
		} else {
			masks[3] = 0;
			shifts[3] = 0;
			bits[3] = 0;
			losses[3] = 8;
		}
	}
}

/*
================
NiDDPixelDesc::NiDDPixelDesc
================
*/
inline NiDDPixelDesc::NiDDPixelDesc( IDirectDrawSurface7 *surface ) {
	index = 0;
	if ( surface ) {
		DDSURFACEDESC2 desc;
		memset( &desc, 0, sizeof( desc ) );
		desc.dwSize = sizeof( desc );
		if ( surface->GetSurfaceDesc( &desc ) == DD_OK && ( desc.dwFlags & DDSD_PIXELFORMAT ) ) {
			InitFromDDPixelFormat( &desc.ddpfPixelFormat );
		}
	}
}

/*
================
NiDDPixelDescTable::Register
================
*/
// FUNCTION: STBC 0x7CAFD0 ?Register@NiDDPixelDescTable@@SAIABVNiDDPixelDesc@@@Z @netimmerse/renderer/NiD3DFrameBuffer
inline unsigned int NiDDPixelDescTable::Register( const NiDDPixelDesc &desc ) {
	if ( !desc.index ) {
		unsigned int i;
		for ( i = 1; i < descs.GetSize(); i++ ) {
			if ( *descs.GetAt( i ) == desc ) {
				desc.index = i;
				break;
			}
		}
		if ( !desc.index ) {
			desc.index = i;
			descs.SetAtGrow( i, new NiDDPixelDesc( desc ) );
		}
	}
	return desc.index;
}

#endif /* !__NID3DPIXELFORMAT_H__ */
