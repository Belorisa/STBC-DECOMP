/*
===============================================================================

	TGIconPoly.h

	One icon drawn as a quad of a UI batch (TGUIBatch): the rectangle it
	covers on the screen, the batch that holds its vertices and its place
	in the batch. TGIcon keeps the one it draws (TGIcon_GetIconPoly).

	Evidence: the SWIG type _p_TGIconPoly ("TGIconPoly *") is what
	TGIcon_GetIconPoly returns. The icon manager makes one in a batch
	(0x72A470, through 0x748890). 0x740A10 adds an offset to the rectangle
	(0x00, 0x04) and rewrites the quad's vertices in the batch (0x10) at the
	poly's index (0x14); 0x740AE0 writes a colour into the quad's four
	vertex colours; 0x740690 clips the quad's texture coordinates to a
	rectangle. The icon manager allocates the polys 64 at a time (0x18
	bytes each, constructor 0x72A830) and keeps the free ones on a list
	linked through 0x10, which holds the batch only while the poly is in
	one (TGIconManager::AllocIconPoly 0x72A6E0, FreeIconPoly 0x72A850).

===============================================================================
*/

#ifndef __TGICONPOLY_H__
#define __TGICONPOLY_H__

#include "TGRect.h"

class NiPoint2;
class NiPoint3;
class NiColorA;
class TGUIBatch;

// The index of a poly in no batch.
#define ICONPOLY_NO_INDEX	0xFFFF

// FUNCTION: STBC 0x00729d50 ??1TGIconPoly@@QAE@XZ @game/TGIconManager
// FUNCTION: STBC 0x00729d60 ??_ETGIconPoly@@QAEPAXI@Z @game/TGIconManager
class TGIconPoly {
public:
	// FUNCTION: STBC 0x0072a830 ??0TGIconPoly@@QAE@XZ @game/TGIconManager
					TGIconPoly( void ) : batch( NULL ), index( ICONPOLY_NO_INDEX ) {}

	void			SetClipRect( TGRect *clipRect );	// invented name
	void			Move( NiPoint2 *offset );			// invented name
	void			SetColor( NiColorA *color );		// invented name
	void			SetTextureCoords( NiPoint2 *coords );	// invented name; the quad's four texture coordinates
	NiPoint3 *		GetVertices( void );				// invented name; the quad's four vertices in the batch's shape data
	NiPoint2 *		GetTextureCoords( void );			// invented name; the quad's four texture coordinates in the batch's shape data
	// Tells the renderer the texture coordinates changed (the revision bump TGFrame::AddPoly's
	// retail copy inlines after GetTextureCoords; invented name).
	void			TexturesChanged( void );
	void			UpdateVertices( void );				// invented name; writes the rectangle into the quad's four vertices

	TGRect			rect;					// 0x00 on the screen
	union {
		TGUIBatch *		batch;				// 0x10 holds the quad's vertices, while the poly is in a batch
		TGIconPoly *	nextFree;			// 0x10 the next poly on the icon manager's free list, while it is free (invented name)
	};
	unsigned short	index;					// 0x14 the quad's place in the batch
};

#include "NiTriShape.h"
#include "TGUIBatch.h"

inline void TGIconPoly::TexturesChanged( void ) {
	batch->shape->modelData->IncRevisionID();
}

#endif /* !__TGICONPOLY_H__ */
