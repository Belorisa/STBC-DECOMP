/*
===============================================================================

	TGPane.h

	A UI object that holds other UI objects: its children, drawn and given
	events in list order, and the child that has the focus.

	0x4C bytes, allocated from the pools (the scalar deleting destructor
	0x72DDD0 frees through the pool of that size).

	Evidence: the constructors (0x72DCC0 with a size, 0x72DE00 with an ID)
	call TGUIObject's, build the child list at 0x30 (an NiTList with two
	items per block), store the vtable 0x897C14 (82 slots; 64 to 81 are
	new) and clear the focus (0x48); GetObjType returns 0x204. The SWIG
	wrappers TGPane_* name slots 64 to 81 (InsertChild to MoveFocus) and
	give their arguments; slot 80, GetFocus, returns 0x48.

===============================================================================
*/

#ifndef __TGPANE_H__
#define __TGPANE_H__

#include "TGUIObject.h"
#include "NiTList.h"

class TGObject;
class TGEvent;

// GetObjType's value for a TGPane.
#define TGPANE_TYPE				0x204

// The event types of the gamepad and of the control module (the SWIG
// constants ET_GAMEPAD and FIRST_TG_CONTROL_MODULE_EVENT_TYPE).
#define ET_GAMEPAD							0x30003
#define FIRST_TG_CONTROL_MODULE_EVENT_TYPE	0x40001

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0072ddd0 ??_GTGPane@@UAEPAXI@Z
class TGPane : public TGUIObject {
public:
					TGPane( long id = 0 );
					TGPane( float width, float height );
	virtual			~TGPane( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual void	Print( int indent = 0, int depth = -1 );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// TGUIObject
	virtual TGUIObject *	GetFocusLeaf( void );
	virtual int		FocusHeight( void );
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );
	virtual void	SetUseParentBatch( bool layout = true );
	virtual void	SetNotUseParentBatch( bool layout = true );
	virtual void	SetBatchChildPolys( bool layout = true );
	virtual void	SetNotBatchChildPolys( bool layout = true );
	virtual void	Layout( void );
	virtual void	VirtualSlot59( void );
	virtual void	VirtualSlot60( void );
	virtual void	VirtualSlot61( NiPoint2 *offset );
	virtual void	VirtualSlot62( TGRect *clipRect );
	virtual void	VirtualSlot63( void *argument );

	static TGPane *	Cast( TGObject * );

	virtual void	InsertChild( int index, TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true );
	// Appends a child, drawn last (TGPane_AddChild).
	void			AddChild( TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true ) { InsertChild( 0x7FFFFFFF, child, x, y, layout ); }
	// Inserts a child in front of the others, drawn first (TGPane_PrependChild).
	// FUNCTION: STBC 0x513C80 ?PrependChild@TGPane@@QAEXPAVTGUIObject@@MM_N@Z @game/PlayerReticleWindow
	void			PrependChild( TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true ) { InsertChild( 0, child, x, y, layout ); }
	virtual void	RemoveChild( TGUIObject *child, bool layout = true );
	virtual void	DeleteChild( TGUIObject *child, bool layout = true );
	virtual int		GetNumChildren( void );
	virtual int		FindPos( TGUIObject *child );
	virtual TGUIObject *	GetFirstChild( void );
	virtual TGUIObject *	GetLastChild( void );
	virtual TGUIObject *	GetNthChild( int index );
	virtual TGUIObject *	GetNextChild( TGUIObject *child );
	virtual TGUIObject *	GetPrevChild( TGUIObject *child );
	virtual void	KillChildren( void );
	virtual void	MoveToFront( TGUIObject *child, bool layout = true );
	virtual void	MoveTowardsFront( TGUIObject *child, bool layout = true );
	virtual void	MoveToBack( TGUIObject *child, bool layout = true );
	virtual void	MoveTowardsBack( TGUIObject *child, bool layout = true );
	virtual void	SetFocus( TGUIObject *child, bool layout = true );
	virtual TGUIObject *	GetFocus( void );
	virtual void	MoveFocus( int direction, bool wrap = false, bool layout = true );


	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			GamepadHandler( TGEvent *event );
	void			ControlHandler( TGEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	void			UpdateChildVisibility( bool visible );		// invented name
	TGUIObject *	GetFirstFocusableChild( void );				// invented name

	NiTList<TGUIObject *>	children;		// 0x30 drawn and given events from the head
	TGUIObject *	focus;					// 0x48 the child with the focus, or NULL

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	static TGPane *	Create( float width, float height ) { return new TGPane( width, height ); }	// the SWIG wrapper TGPane_Create expands it
};

/*
================
TGPane::IsTypeOf
================
*/
// FUNCTION: STBC 0x0072dd50 @game/TGPane
inline bool TGPane::IsTypeOf( int type ) {
	if ( type == TGPANE_TYPE ) {
		return true;
	}
	return TGUIObject::IsTypeOf( type );
}

/*
================
TGPane::GetClassHandlers

The linker kept a copy of this inline emitted outside the pane's unit.
================
*/
// FUNCTION: STBC 0x0051cfd0
inline TGEventHandlerTable *TGPane::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__TGPANE_H__ */
