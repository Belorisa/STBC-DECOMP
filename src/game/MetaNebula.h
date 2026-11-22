/*
===============================================================================

	MetaNebula.h

	A nebula made of spheres: it fogs and damages only inside them. 0xD0
	bytes, allocated from the pools.

	Evidence: Create (0x59A250, the SWIG wrapper MetaNebula_Create)
	allocates 0xD0 bytes and calls the constructor (0x59AA80), which stores
	the vtable 0x88A394 after Nebula's constructor: Nebula's 75 slots, none
	added, up to the next vtable at 0x88A4C0. GetObjType (0x440890) returns
	0x800F, the type Cast (0x5973E0) checks. UtopiaModule's loader creator
	(0x4407D0) expands the loading constructor (0x440810), so its unit keeps
	the inline virtuals and both deleting destructors.

	The class, Create and AddNebulaSphere are named by the SWIG wrappers,
	GetNebulaSpheres by the method table of the App module (0x43B191); every
	other name is invented, after its use.

===============================================================================
*/

#ifndef __METANEBULA_H__
#define __METANEBULA_H__

#include "Nebula.h"

class NiAVObject;
class NiBound;
class SphereDistanceModifier;
class TGObject;
struct DistanceSphere;
struct _object;
typedef _object PyObject;

// GetObjType's value for a MetaNebula (the type its Cast checks).
#define METANEBULA_TYPE		0x800F

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00440930 ??_GMetaNebula@@UAEPAXI@Z @game/UtopiaModule
class MetaNebula : public Nebula {
public:
					MetaNebula( const NiColor &color_, float visibility_, float sensorDensity_, char *textureName_, char *externalTextureName_ );
	// FUNCTION: STBC 0x00440810 ??0MetaNebula@@QAE@J@Z @game/UtopiaModule
					MetaNebula( long id ) : Nebula( id ), spheres( 2 ) {}	// the loader's: the stream sets the rest
	virtual			~MetaNebula( void );

	// TGObject
	// FUNCTION: STBC 0x00440890 ?GetObjType@MetaNebula@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return METANEBULA_TYPE; }
	// FUNCTION: STBC 0x004408a0 ?IsTypeOf@MetaNebula@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == METANEBULA_TYPE ) {
			return true;
		}
		return Nebula::IsTypeOf( type );
	}
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00440900 ?GetClassName@MetaNebula@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "MetaNebula"; }
	// FUNCTION: STBC 0x00440910 ?GetSwigTypeName@MetaNebula@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_MetaNebula"; }
	// FUNCTION: STBC 0x00440920 ?GetPythonClassName@MetaNebula@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "MetaNebulaPtr"; }

	// Nebula
	// the fog distance for the camera, from the spheres it is in or looks
	// into; sets the fog of the set
	virtual float	VirtualSlot68( NiCamera *camera );
	// whether the way between the points runs through less nebula than one
	// can see through
	virtual int		VirtualSlot69( NiPoint3 *end, NiPoint3 *start );
	virtual float	VirtualSlot70( NiPoint3 *point );
	virtual float	VirtualSlot71( NiBound *bound );
	virtual bool	VirtualSlot72( NiPoint3 *start, NiPoint3 *end, NiPoint3 *entry );
	virtual void	AttachToSet( void );
	virtual bool	IsObjectInside( ObjectClass *object );

	static MetaNebula *	Cast( TGObject * );
	static MetaNebula *	Create( float r, float g, float b, float visibility, float sensorDensity, char *textureName, char *externalTextureName );
	void	AddNebulaSphere( float x, float y, float z, float radius );

	NiTList<DistanceSphere *> *	GetSpheres( void ) { return &spheres; }

protected:
	// Builds the sensors' distance modifier and what the nebula shows from
	// outside.
	void			CreateExterior( void );
	// Adds a sphere: its shape under the object's node, and a record of it
	// for the nebula and for the distance modifier.
	void			AddSphere( NiPoint3 *center, float radius );

	char *			externalTextureName;		// 0xAC the texture of the spheres seen from outside, owned
	SphereDistanceModifier *	modifier;		// 0xB0 given to the set
	NiPointer<NiAVObject>	exterior;			// 0xB4 attached under the set's backdrop root
	NiTList<DistanceSphere *>	spheres;			// 0xB8 owned
};

// The Python method MetaNebula.GetNebulaSpheres.
PyObject *		MetaNebula_GetNebulaSpheres( PyObject *self, PyObject *args );

#endif /* !__METANEBULA_H__ */
