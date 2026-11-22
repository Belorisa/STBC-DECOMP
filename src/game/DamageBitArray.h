/*
===============================================================================

	DamageBitArray.h

	A counted block of bits, a bit per cell or triangle, used by the damage
	geometry: the polygonizer's members at 0xA4 and 0xC8, the cells of a
	DamageVoxelGrid and the six arrays of a VisibleDamagePiece (0x48-0x70).
	The name is invented.

	Evidence: each is built with both words cleared and destroyed by
	freeing the block; SetSize, inline everywhere, keeps the block when the
	byte count stays the same.

===============================================================================
*/

#ifndef __DAMAGEBITARRAY_H__
#define __DAMAGEBITARRAY_H__

#include <stddef.h>
#include <string.h>

struct DamageBitArray {
	unsigned int		count;				// 0x00 bits
	unsigned char *		bits;				// 0x04 owned heap block

						// a block of bitCount bits, not cleared (MotionBlurPass::Display, 0x4EE983);
						// with no count, an empty array. One constructor with a default
						// count, not a separate default one: MetaVolumePolygonizer's constructor
						// (0x4BD2D0) spends its inline budget as this body costs, so that
						// the grid list's constructor after its two arrays stays out of line
						DamageBitArray( unsigned int bitCount = 0 ) {
							count = bitCount;
							if ( bitCount > 0 ) {
								bits = new unsigned char[GetByteCount( bitCount )];
							} else {
								bits = NULL;
							}
						}
						// FUNCTION: STBC 0x004bb2a0 ??1DamageBitArray@@QAE@XZ @game/DamagePolygonizer
						~DamageBitArray() { delete [] bits; }

	unsigned int		GetByteCount( unsigned int bitCount ) const { return ( bitCount >> 3 ) + ( ( bitCount & 7 ) > 0 ); }
	unsigned int		GetByteCount() const { return GetByteCount( count ); }
	void				SetSize( unsigned int bitCount );
	// Each bit operation takes the byte index before the mask: retail copies
	// the index for the shift and consumes the original in the mask (the
	// inline copies in 0x4B48C0, 0x4B4660, 0x4B45E0, 0x4C4D70)
	// FUNCTION: STBC 0x004bb2f0 ?IsSet@DamageBitArray@@QBE_NI@Z @game/DamagePolygonizer
	bool				IsSet( unsigned int index ) const {
							unsigned int byteIndex = index >> 3;
							return ( bits[byteIndex] & (unsigned char)( 1 << ( index & 7 ) ) ) != 0;
						}
	void				Set( unsigned int index ) {
							unsigned int byteIndex = index >> 3;
							bits[byteIndex] |= 1 << ( index & 7 );
						}
	// clears every bit (invented name)
	void				ClearAll() { if ( bits != NULL ) { memset( bits, 0, GetByteCount() ); } }
	// sets the bit; true when it was set already
	bool				TestAndSet( unsigned int index ) {
							unsigned int byteIndex = index >> 3;
							unsigned char mask = 1 << ( index & 7 );
							bool wasSet = ( bits[byteIndex] & mask ) != 0;
							bits[byteIndex] |= mask;
							return wasSet;
						}
	// clears the bit; true when it was set
	// FUNCTION: STBC 0x004c4be0 ?Clear@DamageBitArray@@QAE_NI@Z @game/DamagePolygonizer
	bool				Clear( unsigned int index ) {
							unsigned int byteIndex = index >> 3;
							unsigned char mask = 1 << ( index & 7 );
							bool wasSet = ( bits[byteIndex] & mask ) != 0;
							bits[byteIndex] &= ~mask;
							return wasSet;
						}
};

inline void DamageBitArray::SetSize( unsigned int bitCount ) {
	if ( GetByteCount( bitCount ) != GetByteCount( count ) ) {
		delete [] bits;
		if ( bitCount > 0 ) {
			bits = new unsigned char[GetByteCount( bitCount )];
		} else {
			bits = NULL;
		}
	}
	count = bitCount;
}

#endif /* !__DAMAGEBITARRAY_H__ */
