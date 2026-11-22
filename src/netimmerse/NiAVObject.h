/*
===============================================================================

	NiAVObject.h

	The base of the scene graph: an NiObjectNET with a local and a world
	transform, a world bound, velocities, a parent and a list of rendering
	properties.

	Evidence: the vtable at 0x898CA8 (39 slots: 27 new, 12 of them inline
	bodies the linker took from game units); the constructor (0x7DC0C0) and
	destructor (0x7DC210); the viewer strings (0x7DDA10) name the members
	("m_bAppCulled", "m_localTranslate", "m_localRotate", "m_fLocalScale",
	"m_localVelocity", "m_worldTranslate", "m_worldRotate", "m_worldScale",
	"m_worldVelocity", "m_worldBound") and the propagation mode packed in the
	flags ("m_ePropagate": NONE, USE_TRIANGLES, USE_OBBS). Slot 21 is the
	getter the viewer strings print as m_bAppCulled. NiGeometry's
	overrides give slots 12, 14-19 and 27-30 their meaning: slot 12
	transforms the model vertices by a rotation and translation, 14-19
	allocate, transform and free the world vertex (0xD4) and normal (0xD8)
	copies, 27 and 28 store the property and effect states that 29 and 30
	return. Their names follow the Gamebryo lineage, which keeps these
	slots.

===============================================================================
*/

#ifndef __NIAVOBJECT_H__
#define __NIAVOBJECT_H__

#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiBound.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiQuaternion.h"
#include "NiBoundingVolume.h"
#include "NiDynamicEffectState.h"
#include "NiPropertyState.h"
#include "NiStreamMacros.h"

class NiCollisionRecord;
struct NiCollisionIntersect;
class NiPick;
class NiCamera;
class NiBitVector;
class NiNode;

/*
================
NiAVObjectLoadRecord

The link IDs of the attached properties, read by LoadBinary (0x7DD480)
and resolved by LinkObject (0x7DD5F0). The class name is invented.
================
*/
// FUNCTION: STBC 0x7E6810 ??1NiAVObjectLoadRecord@@UAE@XZ @netimmerse/NiNode
// FUNCTION: STBC 0x7E6830 ??_GNiAVObjectLoadRecord@@UAEPAXI@Z @netimmerse/NiNode
class NiAVObjectLoadRecord : public NiObjectNETLoadRecord {
public:
	NiTArray<unsigned int>	propertyLinkIDs;
};

// FUNCTION: STBC 0x007ddfa0 ??_GNiAVObject@@UAEPAXI@Z @netimmerse/NiAVObject
class NiAVObject : public NiObjectNET {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7DBF70)
	static int				Register();
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// told of a collision; a nonzero return stops the search (Gamebryo
	// lineage: NiAVObject's collide callback)
	typedef int				( *CollideCallback )( NiCollisionIntersect &intersect );

	CollideCallback			GetCollideCallback() const { return collideCallback; }

	// FUNCTION: STBC 0x7DDF90 ?GetRTTI@NiAVObject@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );
	virtual void			VirtualSlot03();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiAVObject();

	// transforms the model data (NiNode passes it on to the children)
	// FUNCTION: STBC 0x4341B0 ?ApplyTransform@NiAVObject@@UAEXABVNiMatrix3@@ABVNiPoint3@@_N@Z
	virtual void			ApplyTransform( const NiMatrix3 &rotate, const NiPoint3 &translate, bool onLeft ) {}
	// FUNCTION: STBC 0x004341c0 ?GetWorldBound@NiAVObject@@UAEAAVNiBound@@XZ @game/TGAnimNode
	virtual NiBound &		GetWorldBound() { return worldBound; }

	// world-space copies of the model vertices and normals (NiGeometry
	// keeps them; NiNode passes the calls on to the children)
	// FUNCTION: STBC 0x434240 ?CreateWorldVertices@NiAVObject@@UAEXXZ
	virtual void			CreateWorldVertices() {}
	// FUNCTION: STBC 0x434250 ?UpdateWorldVertices@NiAVObject@@UAEXXZ
	virtual void			UpdateWorldVertices() {}
	// FUNCTION: STBC 0x434260 ?DestroyWorldVertices@NiAVObject@@UAEXXZ
	virtual void			DestroyWorldVertices() {}
	// FUNCTION: STBC 0x434270 ?CreateWorldNormals@NiAVObject@@UAEXXZ
	virtual void			CreateWorldNormals() {}
	// FUNCTION: STBC 0x434280 ?UpdateWorldNormals@NiAVObject@@UAEXXZ
	virtual void			UpdateWorldNormals() {}
	// FUNCTION: STBC 0x434290 ?DestroyWorldNormals@NiAVObject@@UAEXXZ
	virtual void			DestroyWorldNormals() {}
	virtual void			SetAppCulled( bool appCulled );
	// FUNCTION: STBC 0x004341a0 ?GetAppCulled@NiAVObject@@UBE_NXZ @game/TGAnimNode
	virtual bool			GetAppCulled() const { return flags & APP_CULLED_MASK; }
	virtual NiAVObject *	GetObjectByName( const char *name_ );
	// FUNCTION: STBC 0x434210 ?VirtualSlot23@NiAVObject@@UAEXH@Z
	virtual void			VirtualSlot23( int ) {}
	// FUNCTION: STBC 0x434220 ?VirtualSlot24@NiAVObject@@UAEXXZ
	virtual void			VirtualSlot24() {}
	virtual void			UpdateDownwardPass( float time );
	// FUNCTION: STBC 0x456E90 ?VirtualSlot26@NiAVObject@@UAE_NXZ
	virtual bool			VirtualSlot26() { return true; }
	// the property and effect states, gathered on the way down
	virtual void			UpdatePropertiesDownward( NiPropertyState *parentState );
	virtual void			UpdateEffectsDownward( NiDynamicEffectState *parentState );
	virtual NiPointer<NiPropertyState>	GetPropertyState();
	virtual NiPointer<NiDynamicEffectState>	GetEffectState();
	// draws the object for a camera that sees it (Gamebryo lineage name;
	// the geometry draws through the camera's renderer)
	// FUNCTION: STBC 0x4341E0 ?Display@NiAVObject@@UAEXPAVNiCamera@@@Z
	virtual void			Display( NiCamera *camera ) {}
	// FUNCTION: STBC 0x4341F0 ?TestCollisions@NiAVObject@@UAE_NMAAVNiCollisionRecord@@0@Z
	virtual bool			TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) { return false; }
	// FUNCTION: STBC 0x434200 ?FindCollisions@NiAVObject@@UAEHMAAVNiCollisionRecord@@0@Z
	virtual int				FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) { return 0; }
	// FUNCTION: STBC 0x434230 ?FindIntersections@NiAVObject@@UAE_NABVNiPoint3@@0AAVNiPick@@@Z
	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) { return false; }
	virtual void			VirtualSlot35( NiCamera *camera );
	virtual void			VirtualSlot36( NiCamera *camera );
	virtual void			UpdateWorldData();
	// FUNCTION: STBC 0x4341D0 ?UpdateWorldBound@NiAVObject@@UAEXXZ
	virtual void			UpdateWorldBound() {}

							NiAVObject();

	void					Update( float time );

	// rebuild the property and effect states of the subtree from the
	// parent's
	void					UpdateProperties();
	void					UpdateEffects();
	void					AttachParent( NiNode *parent_ );

	// TODO identity: unless app-culled, tests the world bound against the
	// camera's active culling planes and hands the visible object to
	// Display with the camera (0x7DCB80; NiCamera::Click's call)
	void					CullShow( NiCamera *camera );
	// TODO identity: the renderer's key for the object's property and effect
	// states (NiRender::VirtualSlot15), 0x800000 without a renderer; nothing
	// in retail calls it (0x7DCC80)
	unsigned int			Unknown7DCC80( NiRender *renderer );
	// TODO identity: CullShow's two halves on their own (ViewScreen's
	// 0x6763B0 calls them): disables the camera planes the world bound lies
	// wholly in front of, recording them in the mask, and is true when the
	// bound is wholly behind one (0x7DCA70); enables again the planes of the
	// mask (0x7DCB00)
	bool					Unknown7DCA70( NiCamera *camera, NiBitVector &disabled );
	void					Unknown7DCB00( NiCamera *camera, NiBitVector &disabled );
	// whether a ray from origin along dir can meet the world bound; the
	// pick's first test (name invented)
	bool					TestRayBound( const NiPoint3 &origin, const NiPoint3 &dir );
	NiNode *				GetParent() const;
	void					DetachParent();

	// the local transform, relative to the parent, and the world transform
	// the last update computed (Gamebryo lineage names)
	void					SetTranslate( const NiPoint3 &translate );
	void					SetTranslate( float x, float y, float z );
	const NiPoint3 &		GetTranslate() const;
	void					SetRotate( const NiMatrix3 &rotate );
	const NiMatrix3 &		GetRotate() const;
	// the rotation as a unit quaternion (Gamebryo lineage overloads)
	void					SetRotate( const NiQuaternion &quat );
	void					GetRotate( NiQuaternion &quat ) const;
	void					SetScale( float scale );
	float					GetScale() const;
	const NiPoint3 &		GetWorldTranslate() const;
	void					SetWorldTranslate( const NiPoint3 &translate );
	const NiMatrix3 &		GetWorldRotate() const;
	float					GetWorldScale() const;
	// the local velocity (Gamebryo lineage names); PhysicsObjectClass
	// (0x5A04C0, 0x5A05A0) inlines both
	void					SetVelocity( const NiPoint3 &velocity ) { localVelocity = velocity; }
	const NiPoint3 &		GetVelocity() const { return localVelocity; }

	// properties
	void					AttachProperty( NiProperty *property );
	void					DetachProperty( NiProperty *property );	// Gamebryo lineage
	NiPointer<NiProperty>	GetProperty( int type );
	NiPointer<NiProperty>	RemoveProperty( int type );

	// collision propagation, packed in the flags
	enum PropagationMode {
		PROPAGATE_NONE,
		PROPAGATE_USE_TRIANGLES,
		PROPAGATE_USE_OBBS
	};

	// flags
	enum {
		APP_CULLED_MASK		= 0x0001,
		PROPAGATE_MASK		= 0x0006,
		PROPAGATE_POS		= 1,
		UNKNOWN_08_MASK		= 0x0008,	// set by NiNode's constructor; NiNode's slot 26 reports it
		UNKNOWN_08_POS		= 3
	};

	void					SetPropagationMode( PropagationMode mode );

	// sets the field of width mask (unshifted) at pos in the flags (Gamebryo
	// lineage: NiDeclareFlags)
	// FUNCTION: STBC 0x4D4F90 ?SetField@NiAVObject@@QAEXGGG@Z @game/WarpFlash
	void					SetField( unsigned short value, unsigned short mask, unsigned short pos ) { flags = ( flags & ~( mask << pos ) ) | ( value << pos ); }
	// one bit of the flags (NiDeclareFlags, as NiProperty's; AnimOnly.exe
	// 0x417470, 0x4174F0)
	void					SetBit( bool value, unsigned short pos ) { if ( value ) { flags |= 1 << pos; } else { flags &= ~( 1 << pos ); } }
	bool					GetBit( unsigned short pos ) const { return ( flags >> pos ) & 1; }
	PropagationMode			GetPropagationMode() const { return (PropagationMode)( ( flags & PROPAGATE_MASK ) >> PROPAGATE_POS ); }

	// alternate bounding volumes (Gamebryo lineage: the model and world ABVs)
	void					SetModelABV( NiBoundingVolume *volume );
	NiBoundingVolume *		GetModelABV() const { return modelABV; }
	NiBoundingVolume *		GetWorldABV() const { return worldABV; }
	// the properties attached to this object (Gamebryo lineage name)
	NiPropertyList &		GetPropertyList();
	const NiPoint3 &		GetWorldVelocity() const { return worldVelocity; }

	// the collision tests of two moving objects by their world volumes,
	// the bound sphere standing in for a missing one: whether they meet
	// within deltaTime, and where and when they first do (names invented
	// after NiBoundingVolume's)
	bool					TestBoundIntersect( float deltaTime, NiAVObject *other );
	bool					FindBoundIntersect( float deltaTime, NiAVObject *other, float &time, NiPoint3 &point,
												bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 );

	// hands a collision to the collide callbacks of both roots, each from
	// its own side (the name follows NiOBBNode's twin, 0x828F10)
	int						ReportCollision( NiCollisionIntersect &intersect );

	// parentState with this object's properties in their type slots; a copy
	// when a property changes it and copyOnChange is set
	// (public: DamageTool.exe's ?PushLocalProperties@NiAVObject@@QAE...)
	NiPointer<NiPropertyState>	PushLocalProperties( NiPropertyState *parentState, bool copyOnChange );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAVObject *dest );

	// gives dest a clone of each property of the list, the last first
	// (this goes unused)
	void					CopyProperties( NiAVObject *dest, NiPropertyList *list );

	static char *			GetViewerString( const char *prefix, PropagationMode mode );

	unsigned short			flags;			// the flags above
	NiNode *				parent;
	NiBound					worldBound;
	NiMatrix3				localRotate;
	NiPoint3				localTranslate;
	float					localScale;
	NiMatrix3				worldRotate;
	NiPoint3				worldTranslate;
	float					worldScale;
	NiPoint3				localVelocity;
	NiBoundingVolume *		modelABV;		// owned: the model-space bounding volume
	NiPoint3				worldVelocity;
	NiBoundingVolume *		worldABV;		// owned: the world-space copy of modelABV
	CollideCallback			collideCallback;	// called with each collision the OBB trees find, or NULL
	void *					collideCallbackData;
	NiPropertyList			properties;
};

NiRegisterStream( NiAVObject );

/*
================
NiAVObject::GetParent
================
*/
inline NiNode *NiAVObject::GetParent() const {
	return parent;
}

/*
================
NiAVObject::DetachParent

Clears the parent without leaving it: the parent's own detach calls it
(Gamebryo 1.2.2's NiAVObject.inl; AnimOnly.exe's NiNode::DetachChildAt,
DetachChild and SetAt call it).
================
*/
inline void NiAVObject::DetachParent() {
	parent = 0;
}

/*
================
NiAVObject::SetTranslate
================
*/
inline void NiAVObject::SetTranslate( const NiPoint3 &translate ) {
	localTranslate = translate;
}

/*
================
NiAVObject::SetTranslate
================
*/
inline void NiAVObject::SetTranslate( float x, float y, float z ) {
	localTranslate = NiPoint3( x, y, z );
}

/*
================
NiAVObject::GetTranslate
================
*/
inline const NiPoint3 &NiAVObject::GetTranslate() const {
	return localTranslate;
}

/*
================
NiAVObject::SetRotate
================
*/
inline void NiAVObject::SetRotate( const NiMatrix3 &rotate ) {
	localRotate = rotate;
}

/*
================
NiAVObject::GetRotate
================
*/
inline const NiMatrix3 &NiAVObject::GetRotate() const {
	return localRotate;
}

/*
================
NiAVObject::GetRotate
================
*/
inline void NiAVObject::GetRotate( NiQuaternion &quat ) const {
	quat.FromRotation( localRotate );
}

/*
================
NiAVObject::SetRotate
================
*/
inline void NiAVObject::SetRotate( const NiQuaternion &quat ) {
	quat.ToRotation( localRotate );
}

/*
================
NiAVObject::SetScale
================
*/
inline void NiAVObject::SetScale( float scale ) {
	assert( scale > 0.0f );
	localScale = scale;
}

/*
================
NiAVObject::SetWorldTranslate
================
*/
inline void NiAVObject::SetWorldTranslate( const NiPoint3 &translate ) {
	worldTranslate = translate;
}

/*
================
NiAVObject::GetScale
================
*/
inline float NiAVObject::GetScale() const {
	return localScale;
}

/*
================
NiAVObject::GetWorldTranslate
================
*/
// FUNCTION: STBC 0x8541E0 ?GetWorldTranslate@NiAVObject@@QBEABVNiPoint3@@XZ @netimmerse/NiBezierPatchEvaluate
inline const NiPoint3 &NiAVObject::GetWorldTranslate() const {
	return worldTranslate;
}

/*
================
NiAVObject::GetWorldRotate
================
*/
// FUNCTION: STBC 0x8541F0 ?GetWorldRotate@NiAVObject@@QBEABVNiMatrix3@@XZ @netimmerse/NiBezierPatchEvaluate
inline const NiMatrix3 &NiAVObject::GetWorldRotate() const {
	return worldRotate;
}

/*
================
NiAVObject::GetWorldScale
================
*/
// FUNCTION: STBC 0x854200 ?GetWorldScale@NiAVObject@@QBEMXZ @netimmerse/NiBezierPatchEvaluate
inline float NiAVObject::GetWorldScale() const {
	return worldScale;
}

/*
================
NiAVObject::GetPropertyList
================
*/
inline NiPropertyList &NiAVObject::GetPropertyList() {
	return properties;
}

/*
================
NiAVObject::SetAppCulled

The inline setter the linker took from a game unit (0x434180).
================
*/
// FUNCTION: STBC 0x00434180 ?SetAppCulled@NiAVObject@@UAEX_N@Z @game/TGAnimNode
inline void NiAVObject::SetAppCulled( bool appCulled ) {
	if ( appCulled ) {
		flags |= APP_CULLED_MASK;
	} else {
		flags &= ~APP_CULLED_MASK;
	}
}

/*
================
NiAVObject::SetPropagationMode
================
*/
inline void NiAVObject::SetPropagationMode( PropagationMode mode ) {
	flags = ( flags & ~PROPAGATE_MASK ) | ( mode << PROPAGATE_POS );
}

/*
================
NiAVObject::AttachProperty

The out-of-line copy (0x46C250) comes from a game unit.
================
*/
// FUNCTION: STBC 0x46C250
inline void NiAVObject::AttachProperty( NiProperty *property ) {
	properties.AddHead( property );
}

/*
================
NiAVObject::DetachProperty
================
*/
inline void NiAVObject::DetachProperty( NiProperty *property ) {
	properties.Remove( property );
}

#endif /* !__NIAVOBJECT_H__ */
