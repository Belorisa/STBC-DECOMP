/*
===============================================================================

	BaseObjectClass.h

	The base of every object that can be placed in a set: an event handler
	that owns a scene-graph node. The node carries the object's transform;
	the NetImmerse object the object shows (a model, a light, a camera) is
	attached under it, and the node itself is attached under the scene root
	of the set that contains the object.

	0x3C bytes, allocated from the pools (Create allocates 0x3C, the scalar
	deleting destructor 0x4309E0 frees through the pool of that size).

	Evidence: the constructors (0x4308E0 by ID, 0x430A10 from a NetImmerse
	object) call TGEventHandlerObject's, construct the smart pointers at
	0x18 and 0x1C and the string at 0x2C, store the vtable 0x889708
	(50 slots) and call InitObject (0x430830), which creates the node; the
	destructor (0x430A90) frees the name at 0x28. GetObjType returns 0x8002,
	the type Cast checks. The SWIG wrappers name the members they read
	(GetNode 0x18, GetNiObject 0x1C, GetContainingSet 0x20, SetDeleteMe
	0x24, GetName 0x28, GetDisplayName 0x2C) and the virtual slots they call
	(Update 21, UpdateNodeOnly 22, GetAnimNode 25, SetName 26, Rotate 28,
	SetTranslateXYZ 29, SetTranslate 31, SetAngleAxisRotation 32,
	SetMatrixRotation 34, SetScale 35, GetScale 36, GetWorldLocation 37,
	GetTranslate 38, GetRotation 39, GetWorldRotation 40). The inline
	virtuals at 0x419880-0x419900 (kept from another unit) fill slots 23,
	25, 28, 30, 31 and 34; 28 calls 27, 30 calls 29, 31 calls 30 and 34
	calls 33. ObjectClass's placement (0x435280) passes GetWorldLocation's
	point to slot 31 and GetWorldRotation's matrix to slot 33: slot 31 takes
	the TGPoint3 by reference, and slot 34 does not take a TGMatrix3 one.

===============================================================================
*/

#ifndef __BASEOBJECTCLASS_H__
#define __BASEOBJECTCLASS_H__

#include "TGEventHandlerObject.h"
#include "TGString.h"
#include "TGPoint3.h"
#include "TGMatrix3.h"
#include "TGAnimNode.h"
#include "NiNode.h"
#include "NiPointer.h"

class NiQuaternion;

class NiAVObject;
class SetClass;
class TGObject;

// Event types, as the SWIG constants of the App module give them.
#define ET_NAME_CHANGE			0x800109

// GetObjType's value for a BaseObjectClass.
#define BASEOBJECTCLASS_TYPE	0x8002

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004309e0 ??_GBaseObjectClass@@UAEPAXI@Z @game/BaseObjectClass
class BaseObjectClass : public TGEventHandlerObject {
public:
					BaseObjectClass( long id = 0 );
					BaseObjectClass( NiAVObject *object );
	virtual			~BaseObjectClass( void );

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

	virtual void	Update( float time );
	virtual void	UpdateNodeOnly( void );
	// FUNCTION: STBC 0x00419880 ?SetLastUpdateTime@BaseObjectClass@@UAEXM@Z
	virtual void	SetLastUpdateTime( float time ) { lastUpdateTime = time; }		// invented name
	virtual void	SetContainingSet( SetClass *set );								// invented name
	virtual TGAnimNode *	GetAnimNode( void );
	virtual void	SetName( char *name_ );
	// FUNCTION: STBC 0x004198d0 ?Rotate@BaseObjectClass@@UAEXPAVTGMatrix3@@@Z
	virtual void	Rotate( TGMatrix3 *matrix ) { Rotate( *matrix ); }
	virtual void	Rotate( const NiMatrix3 &matrix );
	virtual void	SetTranslateXYZ( float x, float y, float z );
	// FUNCTION: STBC 0x004198e0 ?SetTranslate@BaseObjectClass@@UAEXABVTGPoint3@@@Z
	virtual void	SetTranslate( const TGPoint3 &point ) { SetTranslate( &point ); }
	// FUNCTION: STBC 0x00419900 ?SetTranslate@BaseObjectClass@@UAEXPBVNiPoint3@@@Z
	virtual void	SetTranslate( const NiPoint3 *point ) { SetTranslateXYZ( point->x, point->y, point->z ); }
	virtual void	SetAngleAxisRotation( float angle, float x, float y, float z );
	// FUNCTION: STBC 0x004198f0 ?SetMatrixRotation@BaseObjectClass@@UAEXPAVTGMatrix3@@@Z
	virtual void	SetMatrixRotation( TGMatrix3 *matrix ) { SetMatrixRotation( *matrix ); }
	virtual void	SetMatrixRotation( const NiMatrix3 &matrix );
	virtual void	SetScale( float scale );
	virtual float	GetScale( void );
	virtual const TGPoint3 &	GetWorldLocation( void );
	virtual const TGPoint3 &	GetTranslate( void );
	virtual const TGMatrix3 &	GetRotation( void );
	virtual const TGMatrix3 &	GetWorldRotation( void );
	virtual TGPoint3	VirtualSlot41( char *nodeName );	// the translation of a named node of the animation
	virtual TGMatrix3	VirtualSlot42( char *nodeName );	// the rotation of a named node of the animation
	virtual NiPoint3	GetWorldForward( void );			// invented name
	virtual NiPoint3	GetWorldUp( void );					// invented name
	virtual NiPoint3	GetWorldRight( void );				// invented name
	virtual void	AddToSet( SetClass *set );				// invented name
	virtual void	RemoveFromSet( SetClass *set );			// invented name
	virtual void	SetNiObject( NiAVObject *object );		// invented name
	virtual void	UpdateNode( float time );				// invented name

	// The world directions as TGPoint3s, for the scripts (the SWIG wrappers
	// BaseObjectClass_GetWorld*TG); the out-of-line copies are the SWIG
	// unit's, except GetWorldForwardTG's (0x480690).
	// FUNCTION: STBC 0x00480690 ?GetWorldForwardTG@BaseObjectClass@@QAE?AVTGPoint3@@XZ @game/OptimizedAvoidObstacles
	TGPoint3		GetWorldForwardTG( void ) { return GetWorldForward(); }
	// FUNCTION: STBC 0x005f4900 ?GetWorldBackwardTG@BaseObjectClass@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetWorldBackwardTG( void ) { return -GetWorldForward(); }
	// FUNCTION: STBC 0x005f49e0 ?GetWorldUpTG@BaseObjectClass@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetWorldUpTG( void ) { return GetWorldUp(); }
	// FUNCTION: STBC 0x005f4ab0 ?GetWorldDownTG@BaseObjectClass@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetWorldDownTG( void ) { return -GetWorldUp(); }
	// FUNCTION: STBC 0x005f4b90 ?GetWorldRightTG@BaseObjectClass@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetWorldRightTG( void ) { return GetWorldRight(); }
	// FUNCTION: STBC 0x005f4c60 ?GetWorldLeftTG@BaseObjectClass@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetWorldLeftTG( void ) { return -GetWorldRight(); }

	static BaseObjectClass *	Create( NiAVObject *object );
	static BaseObjectClass *	Cast( TGObject *object );

	// FUNCTION: STBC 0x004e8870 ?GetNode@BaseObjectClass@@QAEPAVNiNode@@XZ @game/PulsingLight
	NiNode *		GetNode( void ) { return node; }
	NiAVObject *	GetNiObject( void ) { return niObject; }
	SetClass *		GetContainingSet( void ) { return containingSet; }
	void			SetDeleteMe( bool deleteMe_ ) { deleteMe = deleteMe_; }
	bool			IsDeleteMe( void ) { return deleteMe; }	// invented name
	// whether the set's Update has stepped the object this frame
	// (SetClass 0x40FFB0, 0x413C60; invented names)
	void			SetUnknown25( bool value ) { unknown25 = value; }
	bool			GetUnknown25( void ) { return unknown25; }
	char *			GetName( void ) { return name; }
	TGString &		GetDisplayName( void ) { return displayName; }
	void			SetHidden( bool hidden ) { if ( node != NULL ) { node->SetAppCulled( hidden ); } }
	bool			IsHidden( void ) { if ( node != NULL ) { return node->GetAppCulled(); } return false; }

	void			AttachObject( BaseObjectClass *object );
	void			DetachObject( BaseObjectClass *object );
	bool			IsObjectAttached( BaseObjectClass *object );		// invented name
	// Whether the node is the object's own or lies under it (0x430D40 and 0x430D60; unreferenced; invented names).
	bool			ContainsNode( NiAVObject *target );
	bool			ContainsNode( NiAVObject *target, NiAVObject *root );

	// The order of the objects of a set's type lists (SortedArray): by ID.
	int				CompareKey( long id_ );								// invented name
	int				Compare( BaseObjectClass *other ) { return CompareKey( other->GetObjID() ); }	// invented name
	void			AlignToVectors( const NiPoint3 &forward, const NiPoint3 &up );
	void			AlignToVectors( const TGPoint3 &forward, const TGPoint3 &up ) { AlignToVectors( (NiPoint3)forward, (NiPoint3)up ); }
	// The turn that takes the object's own forward and up onto the given ones,
	// as a quaternion and as a matrix (invented names).
	NiQuaternion	Unknown431280( const NiPoint3 &forward, const NiPoint3 &up );
	NiMatrix3		Unknown431220( const NiPoint3 &forward, const NiPoint3 &up );
	// The same turn as an axis scaled by minus its angle (0x4312D0; unreferenced; invented name).
	NiPoint3		Unknown4312D0( const NiPoint3 &forward, const NiPoint3 &up );
	// Counts the geometry of the object, or of the object's own NetImmerse object, as changed
	// (invented name).
	void			MarkGeometryChanged( NiAVObject *object );
	void			SetDisplayName( TGString *displayName_ );

	static BaseObjectClass *	GetObject( SetClass *set, char *name );
	static BaseObjectClass *	GetObjectByID( SetClass *set, long id );
	static int		GetNumClassObjects( SetClass *set );
	static BaseObjectClass **	GetClassObjects( SetClass *set, int *count );		// invented name
	// Takes the property of the type off the object and off every object
	// below it (invented name).
	static void		RemovePropertyFromTree( NiAVObject *object, int type );

protected:
	void			InitObject( void );			// invented name

	float			lastUpdateTime;		// 0x14 the time Update was last given; -1 before the first (invented name)
	NiPointer<NiNode>	node;			// 0x18 the object's transform; attached under the set's scene root
	NiPointer<NiAVObject>	niObject;	// 0x1C what the object shows, attached under node
	SetClass *		containingSet;		// 0x20
	bool			deleteMe;			// 0x24
	bool			unknown25;			// 0x25
	bool			unknown26;			// 0x26 the node's hidden state as loaded, applied by PostFixUp
	char *			name;				// 0x28 owned
	TGString		displayName;		// 0x2C

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
BaseObjectClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x00430960
inline bool BaseObjectClass::IsTypeOf( int type ) {
	if ( type == BASEOBJECTCLASS_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

/*
================
BaseObjectClass::GetAnimNode

The object the object shows, when it is an animation node.
================
*/
// FUNCTION: STBC 0x00419890
inline TGAnimNode *BaseObjectClass::GetAnimNode( void ) {
	NiAVObject *object = niObject;		// NiDynamicCast evaluates its argument twice
	return NiDynamicCast( TGAnimNode, object );
}

// Helpers of the unit for turning one direction onto another (invented
// names).
// The angle and the unit axis of the turn that takes from onto to; a zero
// axis when they are parallel.
void			GetTurnBetween( float &angle, NiPoint3 &axis, const NiPoint3 &from, const NiPoint3 &to );
// The turn that takes fromForward onto toForward and then the turned
// fromUp onto toUp.
NiQuaternion	GetTurnBetweenFrames( const NiPoint3 &fromForward, const NiPoint3 &fromUp, const NiPoint3 &toForward, const NiPoint3 &toUp );

#endif /* !__BASEOBJECTCLASS_H__ */
