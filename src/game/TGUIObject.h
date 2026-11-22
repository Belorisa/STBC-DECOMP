/*
===============================================================================

	TGUIObject.h

	The base of the user interface: an event handler with a rectangle on
	the screen, a parent pane and a word of state flags. The interface is
	laid out in fractions of the screen; the position and size setters
	round them to whole pixels.

	0x30 bytes, allocated from the pools (the scalar deleting destructor
	0x72FD40 frees through the pool of that size).

	Evidence: the constructor (0x72FC20) calls TGEventHandlerObject's,
	constructs the bounds (0x18), stores the vtable 0x897D5C (64 slots; 21
	to 63 are new), clears the bounds and the parent (0x14), sets the flags
	(0x28) to TGUIO_VISIBLE and clears the batches (0x2C). GetObjType returns
	0x201. The SWIG wrappers TGUIObject_* name the members: GetParent reads
	0x14, GetLeft and GetTop the bounds, the Is* accessors test the flags,
	and the virtual wrappers name slots 21 to 58 (SetParent to Layout).
	The handler names are the strings "TGUIObject::MouseHandler" and
	"TGUIObject::KeyboardHandler". A parent of (TGPane *)-1 is no pane
	(UI_NO_PARENT): the walks up the parents stop at it.

===============================================================================
*/

#ifndef __TGUIOBJECT_H__
#define __TGUIOBJECT_H__

#include "TGEventHandlerObject.h"
#include "TGRect.h"
#include "NiPoint2.h"
#include "TGIconManager.h"

class TGPane;
class TGMouseEvent;
class TGKeyboardEvent;
class TGUIBatch;

// GetObjType's value for a TGUIObject.
#define TGUIOBJECT_TYPE			0x201

// A parent that is no pane: the walks up the parents stop at it as at NULL.
#define UI_NO_PARENT			( (TGPane *)-1 )

// The state flags (0x28): the SWIG constants TGUIO_*.
#define TGUIO_ENABLED			0x0001
#define TGUIO_SELECTED			0x0002
#define TGUIO_HIGHLIGHTED		0x0004
#define TGUIO_VISIBLE			0x0008
#define TGUIO_EXCLUSIVE			0x0010
#define TGUIO_SKIP_PARENT		0x0020
#define TGUIO_ALWAYS_HANDLE_EVENTS	0x0040
#define TGUIO_USE_PARENT_BATCH	0x0080
#define TGUIO_NO_FOCUS			0x0100
#define TGUIO_BATCH_CHILD_POLYS	0x0200
#define TGUIO_COMPLETELY_VISIBLE	0x0400	// visible, and so is every parent

// The corner AlignTo lines up (the SWIG constants TGUIObject_ALIGN_*).
enum alignment_t {						// invented name
	ALIGN_UL,
	ALIGN_UR,
	ALIGN_BL,
	ALIGN_BR
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0072fd40 ??_GTGUIObject@@UAEPAXI@Z
class TGUIObject : public TGEventHandlerObject {
public:
					TGUIObject( long id = 0 );
	virtual			~TGUIObject( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual void	Print( int indent = 0, int depth = -1 );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	virtual void	SetParent( TGPane *parent, bool layout = true );
	virtual void	SetPosition( float x, float y, bool layout = true );
	virtual void	Move( float dx, float dy, bool layout = true );
	virtual void	Resize( float width, float height, bool layout = true );
	virtual TGUIObject *	GetFocusLeaf( void );
	virtual void	GotFocus( bool layout = true );
	virtual void	GotTrueFocus( bool layout = true );
	virtual void	GotTrueFocusPath( bool layout = true );
	virtual void	LostFocus( bool layout = true );
	virtual void	LostTrueFocus( bool layout = true );
	virtual void	LostTrueFocusPath( bool layout = true );
	virtual int		FocusHeight( void );
	virtual void	GetDesiredSize( NiPoint2 *size );
	virtual void	MouseEntered( TGUIObject *from );
	virtual void	MouseExited( TGUIObject *to );
	virtual bool	IsCompletelyEnabled( void );
	virtual void	SetEnabled( bool layout = true );
	virtual void	SetDisabled( bool layout = true );
	virtual void	SetSelected( bool layout = true );
	virtual void	SetNotSelected( bool layout = true );
	virtual void	SetHighlighted( bool layout = true );
	virtual void	SetNotHighlighted( bool layout = true );
	virtual bool	IsCompletelyVisible( void );
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );
	virtual void	SetExclusive( bool layout = true );
	virtual void	SetNotExclusive( bool layout = true );
	// FUNCTION: STBC 0x00442b10 ?SetSkipParent@TGUIObject@@UAEXXZ
	virtual void	SetSkipParent( void ) { flags |= TGUIO_SKIP_PARENT; }
	// FUNCTION: STBC 0x00442b20 ?SetNoSkipParent@TGUIObject@@UAEXXZ
	virtual void	SetNoSkipParent( void ) { flags &= ~TGUIO_SKIP_PARENT; }
	// FUNCTION: STBC 0x00442b30 ?SetAlwaysHandleEvents@TGUIObject@@UAEXXZ
	virtual void	SetAlwaysHandleEvents( void ) { flags |= TGUIO_ALWAYS_HANDLE_EVENTS; }
	// FUNCTION: STBC 0x00442b40 ?SetNotAlwaysHandleEvents@TGUIObject@@UAEXXZ
	virtual void	SetNotAlwaysHandleEvents( void ) { flags &= ~TGUIO_ALWAYS_HANDLE_EVENTS; }
	virtual void	SetUseParentBatch( bool layout = true );
	virtual void	SetNotUseParentBatch( bool layout = true );
	virtual void	SetBatchChildPolys( bool layout = true );
	virtual void	SetNotBatchChildPolys( bool layout = true );
	// FUNCTION: STBC 0x00442ba0 ?SetNoFocus@TGUIObject@@UAEXXZ
	virtual void	SetNoFocus( void ) { flags |= TGUIO_NO_FOCUS; }
	// FUNCTION: STBC 0x00442bb0 ?SetNotNoFocus@TGUIObject@@UAEXXZ
	virtual void	SetNotNoFocus( void ) { flags &= ~TGUIO_NO_FOCUS; }
	virtual void	Layout( void );
	virtual void	VirtualSlot59( void );						// sets TGUIO_COMPLETELY_VISIBLE from the flags and the parent's
	virtual void	VirtualSlot60( void );						// empty here; called when the object leaves its batch
	virtual void	VirtualSlot61( NiPoint2 *offset );			// empty here; called with the distance moved
	virtual void	VirtualSlot62( TGRect *clipRect );			// empty here; called with the new clip rectangle
	virtual void	VirtualSlot63( void *argument );			// fills the batches

	TGPane *		GetParent( void ) { return parent; }
	TGPane *		GetConceptualParent( void );
	void			GetBounds( TGRect *rect ) { *rect = bounds; }
	void			SetBounds( TGRect *rect, bool layout = true );
	void			GetPosition( NiPoint2 *point ) { point->x = bounds.left; point->y = bounds.top; }
	float			GetLeft( void ) { return bounds.GetLeft(); }
	// Whether a point in the object's own frame lies inside its size.
	bool			IsWithin( NiPoint2 *point ) { return point->x >= 0.0f && point->x <= bounds.GetWidth() && point->y >= 0.0f && point->y <= bounds.GetHeight(); }
	float			GetTop( void ) { return bounds.GetTop(); }
	float			GetWidth( void ) { return bounds.GetWidth(); }
	float			GetHeight( void ) { return bounds.GetHeight(); }
	float			GetRight( void ) { return bounds.GetRight(); }		// TGUIObject_GetRight
	float			GetBottom( void ) { return bounds.GetBottom(); }	// TGUIObject_GetBottom
	void			GetScreenOffset( NiPoint2 *offset );
	void			GetClipRect( TGRect *rect );
	TGUIObject *	GetCommonAncestor( TGUIObject *object );
	void			AlignTo( TGUIObject *object, alignment_t from, alignment_t to, bool layout = true );

	unsigned int	GetFlags( void ) { return flags; }
	int				IsEnabled( void ) { return flags & TGUIO_ENABLED; }
	int				IsSelected( void ) { return flags & TGUIO_SELECTED; }
	int				IsHighlighted( void ) { return flags & TGUIO_HIGHLIGHTED; }
	int				IsVisible( void ) { return flags & TGUIO_VISIBLE; }

	bool			HasFocus( void );
	bool			HasTrueFocus( void );
	bool			HasAncestor( TGUIObject *object );
	bool			IsInTrueFocusPath( void );

	TGUIBatch **	GetBatches( void );					// invented name

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	int				IsExclusive( void ) { return flags & TGUIO_EXCLUSIVE; }
	int				IsSkipParent( void ) { return flags & TGUIO_SKIP_PARENT; }
	int				IsAlwaysHandleEvents( void ) { return flags & TGUIO_ALWAYS_HANDLE_EVENTS; }
	int				IsUseParentBatch( void ) { return flags & TGUIO_USE_PARENT_BATCH; }
	int				IsBatchChildPolys( void ) { return flags & TGUIO_BATCH_CHILD_POLYS; }
	int				IsNoFocus( void ) { return flags & TGUIO_NO_FOCUS; }

	// GraphicsMenu::ChangeDisplayMode drops every object's batches.
	void			DeleteBatches( void );				// invented name

protected:
	// Panes keep their children's links and flags.
	friend class	TGPane;

	TGPane *		parent;					// 0x14 NULL, or UI_NO_PARENT
	TGRect			bounds;					// 0x18 relative to the parent
	unsigned int	flags;					// 0x28 TGUIO_*
	TGUIBatch *		batches;				// 0x2C the polygons drawn for the object and the children that use its batch

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGUIObject::IsTypeOf
================
*/
// FUNCTION: STBC 0x0072fcb0
inline bool TGUIObject::IsTypeOf( int type ) {
	if ( type == TGUIOBJECT_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

/*
================
TGUIObject::GetDesiredSize
================
*/
// FUNCTION: STBC 0x00442c60
inline void TGUIObject::GetDesiredSize( NiPoint2 *size ) {
	size->x = bounds.width;
	size->y = bounds.height;
}

/*
================
TGUIObject::SetNotUseParentBatch
================
*/
// FUNCTION: STBC 0x0053b4a0
inline void TGUIObject::SetNotUseParentBatch( bool layout ) {
	flags &= ~TGUIO_USE_PARENT_BATCH;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGUIObject::SetNotBatchChildPolys
================
*/
// FUNCTION: STBC 0x0053b4c0
inline void TGUIObject::SetNotBatchChildPolys( bool layout ) {
	flags &= ~TGUIO_BATCH_CHILD_POLYS;
	if ( layout == true ) {
		Layout();
	}
}

#endif /* !__TGUIOBJECT_H__ */
