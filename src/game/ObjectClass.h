/*
===============================================================================

	ObjectClass.h

	An object of a set that has a model: it can be lit, targeted, hailed
	and scanned, and collides with the other objects of its set.

	0x68 bytes, allocated from the pools (Create allocates 0x68, the scalar
	deleting destructor 0x435000 frees through the pool of that size).

	Evidence: the constructors (0x434F00 from a NetImmerse object, 0x435030
	by ID) call BaseObjectClass's, construct the smart pointer at 0x58,
	store the vtable 0x889950 (67 slots) and call InitObject (0x435090);
	the destructor (0x4350E0) frees the array at 0x64. GetObjType returns
	0x8003, the type Cast checks. The SWIG wrappers name the members they
	read (GetCollisionFlags 0x3C; the hailable and scannable flags 0x5C
	and 0x5D through IsHailable and IsScannable) and the virtual slots
	they call (IsTargetable 50, CanTargetObject 51, IsHailable 52,
	IsScannable 53, GetRadius through 57, GetRandomPointOnModel 60,
	GetLocalRandomPointAndNormalOnModel 61). The inline virtuals of slots
	50-53, 57 and 66 are kept from another unit (0x419920-0x419970).
	Slots 54-56 attach the node to a light, detach it, and report whether
	it is attached.

===============================================================================
*/

#ifndef __OBJECTCLASS_H__
#define __OBJECTCLASS_H__

#include "BaseObjectClass.h"
#include "NiBound.h"
#include "OBBModel.h"

class NiColorA;
class NiDynamicEffect;
class NiGeometry;
class NiObject;
class NiPick;
class PlacementObject;
class SetClass;
class TGEvent;
class TGObject;

// Event types, as the SWIG constants of the App module give them.
#define ET_HAILABLE_CHANGE		0x800108
#define ET_SCANNABLE_CHANGE		0x80010A

// GetObjType's value for an ObjectClass.
#define OBJECTCLASS_TYPE		0x8003

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00435000 ??_GObjectClass@@UAEPAXI@Z @game/ObjectClass
class ObjectClass : public BaseObjectClass {
public:
	// The collision flag bits (the SWIG constants ObjectClass_CFB_*; the
	// constant table's stores at 0x6567E4-0x6569A9 give the values).
	enum collisionFlags_t {
		CFB_NO_COLLISIONS				= 0x00,
		CFB_IN_PROXIMITY_MANAGER		= 0x01,
		CFB_MEMBER_GROUP_1				= 0x02,
		CFB_COLLIDES_WITH_GROUP_1		= 0x04,
		CFB_MEMBER_GROUP_2				= 0x08,
		CFB_COLLIDES_WITH_GROUP_2		= 0x10,
		CFB_MEMBER_GROUP_3				= 0x20,
		CFB_COLLIDES_WITH_GROUP_3		= 0x40,
		CFB_MEMBER_MASK					= 0x2A,
		CFB_COLLISION_MASK				= 0x54,
		CFB_DEFAULTS					= 0x7F
	};

					ObjectClass( NiAVObject *object );
					ObjectClass( long id = 0 );
	virtual			~ObjectClass( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// FUNCTION: STBC 0x00419920 ?IsTargetable@ObjectClass@@UAE_NXZ
	virtual bool	IsTargetable( void ) { return false; }
	// FUNCTION: STBC 0x00419930 ?CanTargetObject@ObjectClass@@UAE_NPAV1@@Z
	virtual bool	CanTargetObject( ObjectClass *object ) { return false; }
	// FUNCTION: STBC 0x00419940 ?IsHailable@ObjectClass@@UAE_NXZ
	virtual bool	IsHailable( void ) { return hailable; }
	// FUNCTION: STBC 0x00419950 ?IsScannable@ObjectClass@@UAE_NXZ
	virtual bool	IsScannable( void ) { return scannable; }
	virtual void	AddLight( NiDynamicEffect *light );			// invented name
	virtual void	RemoveLight( NiDynamicEffect *light );		// invented name
	virtual bool	IsLitBy( NiDynamicEffect *light );			// invented name
	// FUNCTION: STBC 0x00419960 ?GetWorldBound@ObjectClass@@UAEAAVNiBound@@XZ
	virtual NiBound &	GetWorldBound( void ) { return niObject->GetWorldBound(); }	// invented name
	virtual void	VirtualSlot58( NiPoint3 *point1, NiPoint3 *point2 );		// fills the points at 0x40 and 0x4C on Update
	// a random vertex of the model: its position, its normal (when normal
	// is not NULL and the data has normals) and its geometry; false when the
	// model has no vertices
	virtual bool	VirtualSlot59( NiPoint3 **vertex, NiPoint3 **normal, NiGeometry **geometry );
	virtual NiAVObject *	GetRandomPointOnModel( void );
	virtual void	GetLocalRandomPointAndNormalOnModel( NiPoint3 *point, NiPoint3 *normal );	// types from the SWIG wrapper
	virtual bool	VirtualSlot62( NiPoint3 *origin, const NiPoint3 &direction, NiPick *pick );	// whether the ray hits the object
	virtual bool	VirtualSlot63( const NiPoint3 *origin, const NiPoint3 &direction, NiPick *pick );	// picks along the ray through the object's model (0x435E40)
	virtual bool	VirtualSlot64( TGStream *stream );	// writes the replaced textures
	virtual bool	VirtualSlot65( TGStream *stream );	// reads them back
	// FUNCTION: STBC 0x00419970 ?VirtualSlot66@ObjectClass@@UAEXXZ
	virtual void	VirtualSlot66( void ) { UpdateNodeOnly(); }

	static ObjectClass *	Cast( TGObject *object );
	static ObjectClass *	Create( TGAnimNode *animNode );

	float			GetRadius( void ) { return GetWorldBound().GetRadius(); }
	// The corners of the object's box in the world: slot 58's points, widened by the box at 0x40 and 0x4C when unknown3D is set (0x436130; invented name).
	void			GetExtents( NiPoint3 *minimum, NiPoint3 *maximum );
	unsigned char	GetCollisionFlags( void ) { return collisionFlags; }
	void			SetCollisionFlags( unsigned char flags ) { collisionFlags = flags; }

	void			PlaceObjectByName( char *placementName );
	// Moves the object to the world location of the placement object and turns it to its rotation (invented name).
	void			PlaceObject( PlacementObject *placement );
	void			RandomOrientation( void );
	void			AlignToObject( ObjectClass *object );
	void			SetHailable( bool hailable_ );
	void			SetScannable( bool scannable_ );
	bool			LineCollides( TGPoint3 *start, TGPoint3 *end );
	// Sets the collision model the ray tests use (unknown58); nothing in
	// retail calls it.
	void			Unknown435F50( OBBModel *model );
	// The point relative to the object for Python: (offset, distance,
	// direction, angle off the forward axis) (0x4353F0).
	PyObject *		GetRelativePositionInfo( TGPoint3 *point );
	void			RefreshReplacedTextures( void );
	void			ReplaceTexture( char *newTexture, char *oldTexture, bool record = true );	// parameter names invented, from the scripts' calls
	// The radar colour of the object's affiliation in the current mission.
	NiColorA *		GetAffiliationColor( void );		// invented name

	static ObjectClass *	GetObject( SetClass *set, char *name );
	static ObjectClass *	GetObjectByID( SetClass *set, long id );
	static int		GetNumClassObjects( SetClass *set );
	static ObjectClass **	GetClassObjects( SetClass *set, int *count );		// invented name

	// The most lights that light one object.
	static void		SetMaximumNumberOfLights( int count ) { maximumNumberOfLights = count; }
	static int		GetMaximumNumberOfLights( void ) { return maximumNumberOfLights; }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			EnteredSetHandler( TGEvent *event );

protected:
	void			InitObject( void );			// invented name

	unsigned char	collisionFlags;			// 0x3C 0x7F at first
	bool			unknown3D;				// 0x3D
	NiPoint3		unknown40;				// 0x40 filled by slot 58 on Update
	NiPoint3		unknown4C;				// 0x4C filled by slot 58 on Update
	NiPointer<OBBModel>	unknown58;			// 0x58 the collision model the ray tests (slots 62 and 63) use instead of the model; DamageableObject sets it to its LOD level's
	bool			hailable;				// 0x5C
	bool			scannable;				// 0x5D
	int				numReplacedTextures;	// 0x60 the number of blocks at replacedTextures (invented name)
	char **			replacedTextures;		// 0x64 blocks from the pools, freed with the array; each holds the new texture file and the old name, one after the other (invented name)

	static int		maximumNumberOfLights;
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
ObjectClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x00434f70
inline bool ObjectClass::IsTypeOf( int type ) {
	if ( type == OBJECTCLASS_TYPE ) {
		return true;
	}
	return BaseObjectClass::IsTypeOf( type );
}


struct _object;
typedef _object PyObject;

// The Python methods ObjectClass.GetRelativePositionInfo (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *		ObjectClass_GetRelativePositionInfo( PyObject *self, PyObject *args );

#endif /* !__OBJECTCLASS_H__ */
