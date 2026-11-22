/*
===============================================================================

	Nebula.h

	A nebula: a region of a set that fogs the view, shortens the sensors and
	damages the objects inside it. 0xAC bytes, allocated from the pools.

	Evidence: Create (0x597650, the SWIG wrapper Nebula_Create) allocates
	0xAC bytes and calls the constructor (0x598180), which stores the vtable
	0x88A268 after ObjectClass's constructor: ObjectClass's 67 slots and eight
	new ones (67-74), up to MetaNebula's vtable at 0x88A394. GetObjType
	(0x440660) returns 0x800E, the type Cast (0x5973B0) checks. The inline
	virtuals and the deleting destructor (0x4407A0) sit in UtopiaModule's unit,
	which expands the loading constructor inside MetaNebula's (0x440810).

	The class, Create, SetupDamage, IsObjectInNebula and the static lookups
	are named by the SWIG wrappers; every other name is invented, after its
	use.

===============================================================================
*/

#ifndef __NEBULA_H__
#define __NEBULA_H__

#include "ObjectClass.h"
#include "NiColor.h"
#include "NiTList.h"

class NiCamera;
class NiNode;
class NiTriShape;
class NiTriShapeData;
class SetClass;
class TGFuzzyClusterInnerGeom;
class TGObject;
class TGStream;

// GetObjType's value for a Nebula (the vtable 0x88A268's slot 1, 0x440660).
#define NEBULA_TYPE		0x800E

// An object came into a nebula, or left one (the App constants); the nebula
// is the source and the object the destination.
#define ET_ENTERED_NEBULA	0x800060
#define ET_EXITED_NEBULA	0x800061

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004407a0 ??_GNebula@@UAEPAXI@Z @game/UtopiaModule
class Nebula : public ObjectClass {
public:
					Nebula( const NiColor &color_, float visibility_, float sensorDensity_, char *textureName_ );
	// FUNCTION: STBC 0x00440610 ??0Nebula@@QAE@J@Z @game/UtopiaModule
					Nebula( long id ) : ObjectClass( id ), objectIDs( 2 ) {}	// the loader's: the stream sets the rest
	virtual			~Nebula( void );

	// TGObject
	// FUNCTION: STBC 0x00440660 ?GetObjType@Nebula@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return NEBULA_TYPE; }
	// FUNCTION: STBC 0x00440670 ?IsTypeOf@Nebula@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == NEBULA_TYPE ) {
			return true;
		}
		return ObjectClass::IsTypeOf( type );
	}
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x004406d0 ?GetClassName@Nebula@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "Nebula"; }
	// FUNCTION: STBC 0x004406e0 ?GetSwigTypeName@Nebula@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_Nebula"; }
	// FUNCTION: STBC 0x004406f0 ?GetPythonClassName@Nebula@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "NebulaPtr"; }

	// BaseObjectClass and ObjectClass
	virtual void	Update( float time );
	virtual void	SetContainingSet( SetClass *set );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// TODO identity: slots 67 to 72. A plain nebula fills its whole set, so
	// its answers are constants; MetaNebula overrides 68 to 72 with tests
	// against its spheres.
	// FUNCTION: STBC 0x00440700 ?VirtualSlot67@Nebula@@UAE_NXZ @game/UtopiaModule
	virtual bool	VirtualSlot67( void ) { return false; }
	// how far the camera sees; MetaNebula sets the fog of the set from it
	virtual float	VirtualSlot68( NiCamera *camera );
	// whether the points are within sight of each other
	// FUNCTION: STBC 0x00440710 ?VirtualSlot69@Nebula@@UAEHPAVNiPoint3@@0@Z @game/UtopiaModule
	virtual int		VirtualSlot69( NiPoint3 *end, NiPoint3 *start ) {
		return ( *end - *start ).Length() < visibility;
	}
	// the distance from the point (70) or the sphere (71) to the nebula
	// FUNCTION: STBC 0x00440770 ?VirtualSlot70@Nebula@@UAEMPAVNiPoint3@@@Z @game/UtopiaModule
	virtual float	VirtualSlot70( NiPoint3 *point ) { return 0.0f; }
	// FUNCTION: STBC 0x00440760 ?VirtualSlot71@Nebula@@UAEMPAVNiBound@@@Z @game/UtopiaModule
	virtual float	VirtualSlot71( NiBound *bound ) { return 0.0f; }
	// where a line from start to end comes into the nebula
	// FUNCTION: STBC 0x00440780 ?VirtualSlot72@Nebula@@UAE_NPAVNiPoint3@@00@Z @game/UtopiaModule
	virtual bool	VirtualSlot72( NiPoint3 *start, NiPoint3 *end, NiPoint3 *entry ) {
		*entry = *start;
		return true;
	}
	// Gives the set the nebula's fog, sensor range and overlay.
	virtual void	AttachToSet( void );
	virtual bool	IsObjectInside( ObjectClass *object );

	static Nebula *	Cast( TGObject * );
	static Nebula *	Create( float r, float g, float b, float visibility, char *textureName, float sensorDensity );
	void	SetupDamage( float, float );
	static Nebula *	GetObject( SetClass *, char * );
	static Nebula *	GetObjectByID( SetClass *, long );
	static int	GetNumClassObjects( SetClass * );
	static Nebula **	GetClassObjects( SetClass *set, int *count );
	bool	IsObjectInNebula( ObjectClass * );
	float	GetDamage( void ) { return damage; }
	float	GetHullDamage( void ) { return hullDamage; }
	float	GetVisibility( void ) { return visibility; }	// invented name

protected:
	// A geodesic sphere of the colour: an icosahedron subdivided the number
	// of times, its triangles facing out or in (invented name; the nebulae's
	// overlay and spheres are built from it).
	static NiTriShape *	CreateSphere( float radius, unsigned int subdivisions, bool facingOut, const NiColor &color );
	// Builds the overlay the camera sees from inside.
	void			CreateOverlay( void );
	// Sends ET_ENTERED_NEBULA and ET_EXITED_NEBULA to the objects that came
	// in or went out since the last update.
	void			UpdateObjectsInside( void );
	// Sends ET_ENVIRONMENT_DAMAGE to the objects inside.
	void			DamageObjectsInside( void );

	float			visibility;				// 0x68 how far one sees inside
	float			sensorDensity;			// 0x6C the scale of the sensor range inside: 1 the normal range, 0 none
	NiPointer<NiNode>	overlayNode;		// 0x70 attached under the set's foreground root
	NiColor			color;					// 0x74 the fog's
	TGFuzzyClusterInnerGeom *	innerGeom;	// 0x80 the overlay's shape, owned by the overlay node
	int				unknown84;				// 0x84
	char *			textureName;			// 0x88 the overlay's texture, owned
	NiTList<long>	objectIDs;				// 0x8C the IDs of the objects inside
	float			damage;					// 0xA4 SetupDamage's first value
	float			hullDamage;				// 0xA8 SetupDamage's second value, the first without one
};

#endif /* !__NEBULA_H__ */
