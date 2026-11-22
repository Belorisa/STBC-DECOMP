/*
===============================================================================

	ViewScreenObject.h

	A bridge view screen: an object whose node is a ViewScreenNode. It shows
	the camera of a remote set (SetRemoteCam, ToggleRemoteCam) or its own
	model, and may carry a menu (the character menu MenuUp and MenuDown
	bring up and take down).

	0x78 bytes: the constructors (0x678540 by name, 0x678680 by ID) call
	ObjectClass's, store the vtable 0x894BB4 (67 slots, the 0x434F00-0x435030
	family's own overrides of slots 0-2, 4-7, 9-11, 19, 46 and 47) and call
	the builder 0x6787A0. GetObjType returns 0x801A.

===============================================================================
*/

#ifndef __VIEWSCREENOBJECT_H__
#define __VIEWSCREENOBJECT_H__

#include "ObjectClass.h"

class CameraObjectClass;
class NiCamera;
class ViewScreenNode;
class TGPane;
class SetClass;
class TGEvent;
class TGEventHandlerTable;
class TGObject;
class TGStream;

// GetObjType's value for a ViewScreenObject (the type its Cast checks).
#define VIEWSCREENOBJECT_TYPE		0x801A

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00678650 ??_GViewScreenObject@@UAEPAXI@Z
class ViewScreenObject : public ObjectClass {
public:
					ViewScreenObject( char *name );
					ViewScreenObject( long id );
	virtual			~ViewScreenObject( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	static ViewScreenObject **	GetClassObjects( SetClass *set, int *count );		// invented name
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	bool	RenderingRemoteCam( void ) { return useRemoteCam; }
	static ViewScreenObject *	Cast( TGObject * );
	static ViewScreenObject *	Create( char * );
	void	SetModel( char * );
	void	SetRemoteCam( CameraObjectClass * );
	CameraObjectClass *	GetRemoteCam( void );
	void	ToggleRemoteCam( bool );
	void	SetIsOn( bool );
	bool	IsOn( void );
	void	SetStaticIsOn( bool );
	bool	IsStaticOn( void );
	void	SetStaticVariation( float, float );
	void	SetOffTexture( char * );
	void	SetStaticTextureIconGroup( char * );
	void	SetMenu( TGPane *menu_ ) { menu = menu_; }
	void	ClearMenu( void ) { menu = NULL; }
	TGPane *	GetMenu( void ) { return menu; }
	void	MenuUp( void );
	void	MenuDown( void );
	int	IsMenuUp( void );
	void	LookTowardsSpace( void );
	static ViewScreenObject *	GetObject( SetClass *set, char *name );
	static ViewScreenObject *	GetObjectByID( SetClass *set, long id );		// invented name
	static int	GetNumClassObjects( SetClass *set );

	// The object's node as the view screen node it is (invented name).
	ViewScreenNode *	GetViewScreenNode( void );
	// Whether the screen shows the camera (BridgeSet::VirtualSlot26 renders
	// the set in the view screen's camera mode then; the body is open).
	bool	Unknown678A80( CameraObjectClass *camera );
	// A camera that sees what the remote camera sees, drawing with the
	// camera's renderer into the screen's rect of its view; NULL while the
	// screen is off or out of view (invented name).
	NiCamera *	CreateScreenCamera( CameraObjectClass *camera );

protected:
	void			Init( char *name );					// invented name: the node and the model
	void			MenuEventHandler( TGEvent *event );
	void			CameraDeletedHandler( TGEvent *event );

	CameraObjectClass *	remoteCam;		// 0x68 SetRemoteCam stores it (identity unproven)
	bool			useRemoteCam;		// 0x6C ToggleRemoteCam sets it (invented name)
	char *			modelName;			// 0x70 owned; SetModel's argument
	TGPane *		menu;				// 0x74 the menu MenuUp shows (identity unproven; invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__VIEWSCREENOBJECT_H__ */
