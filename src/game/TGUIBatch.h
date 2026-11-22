/*
===============================================================================

	TGUIBatch.h

	One batch of the polygons a UI object draws for itself and for the
	children that use its batch (TGUIO_USE_PARENT_BATCH); a UI object keeps a
	singly linked list of them (TGUIObject, 0x2C). The name is invented.

	Evidence: TGUIObject's DeleteBatches (0x72FE00) walks the list through
	the first member, destroying each batch (0x7487B0) and freeing it with
	the global operator delete. The destructor releases two NetImmerse
	objects (0x04, and the shape 0x08) and frees a block (0x0C); VirtualSlot63 (0x730A90)
	skips batches whose count (0x10) is zero. The rest is not recovered.

===============================================================================
*/

#ifndef __TGUIBATCH_H__
#define __TGUIBATCH_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiTriShape.h"
class TGIconPoly;
class NiTextureProperty;
class TGRect;
class NiPoint2;
class NiColorA;

class TGUIBatch {								// invented name
public:
					~TGUIBatch( void );

	void			RemoveIconPoly( TGIconPoly *poly );		// invented name; 0x748950 takes the poly's quad out of the batch
	// A poly for the rectangle, its texture coordinates and colour, in a
	// new quad at the end of the batch (invented name).
	TGIconPoly *	AddIconPoly( TGRect *rect, NiPoint2 *textureCoords, NiColorA *color );
	// Makes room for more quads (invented name).
	void			Grow( void );

	// A batch drawing from the texture, its quads clamped by the clamp mode
	// (0x748480: the shape, an alpha, a z-buffer and a material property, and
	// a texturing property's clamp for a non-zero mode; invented name).
					TGUIBatch( NiTextureProperty *texture_, int clamp );
	TGUIBatch *		next;					// 0x00
	NiPointer<NiTextureProperty>	texture;		// 0x04 the texture the quads draw from (invented name); released by the destructor
	NiPointer<NiTriShape>	shape;					// 0x08 holds the quads' vertices and texture coordinates (invented name); released by the destructor
	TGIconPoly **	unknown0C;				// 0x0C the polys by their quad's index (RemoveIconPoly); freed by the destructor
	unsigned short	unknown10;				// 0x10 a count; empty batches are skipped
	unsigned short	unknown12;				// 0x12 the quads there is room for (AddIconPoly grows the batch when the count reaches it)
};

#endif /* !__TGUIBATCH_H__ */
