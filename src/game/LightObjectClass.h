/*
===============================================================================

	LightObjectClass.h

	The members of LightObjectClass that the SWIG wrappers LightObjectClass_* call. Only what the
	wrappers prove is declared: no data members yet.

	Evidence: the vtable 0x8897D0 adds five slots after BaseObjectClass's
	50: slot 50 (0x433E90, not decoded), then the four the SWIG wrappers
	LightObjectClass_AddIlluminatedObject, RemoveIlluminatedObject,
	IlluminateEntireSet and UnilluminateEntireSet call (0xCC, 0xD0, 0xD4,
	0xD8).

===============================================================================
*/

#ifndef __LIGHTOBJECTCLASS_H__
#define __LIGHTOBJECTCLASS_H__

#include "BaseObjectClass.h"
#include "NiTList.h"

// IsTypeOf's value for a light object.
#define LIGHTOBJECTCLASS_TYPE		0x80BC

class NiLight;
class TGStream;
class SetClass;
class TGObject;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00432ce0 ??_GLightObjectClass@@UAEPAXI@Z @game/LightObjectClass
class LightObjectClass : public BaseObjectClass {
public:
					LightObjectClass( long id = 0 );
	virtual			~LightObjectClass( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	SetContainingSet( SetClass *set );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// LightObjectClass
	virtual void	VirtualSlot50( void );
	virtual void	AddIlluminatedObject( BaseObjectClass *object );
	virtual void	RemoveIlluminatedObject( BaseObjectClass *object );
	virtual void	IlluminateEntireSet( void );
	virtual void	UnilluminateEntireSet( void );

	static LightObjectClass *	Cast( TGObject * );
	// The light object for the NetImmerse light (0x432D60; invented name).
	static LightObjectClass *	Create( NiLight *light );
	// The NetImmerse lights SetClass's CreateAmbientLight and
	// CreateDirectionalLight make, named (0x433220 and 0x4332F0; invented
	// names).
	static NiLight *	CreateNiAmbientLight( float red, float green, float blue, float dimmer, char *name );
	static NiLight *	CreateNiDirectionalLight( float red, float green, float blue, float dimmer, float directionX, float directionY,
											 float directionZ, char *name );
	// The point and spot lights ReadFromStream makes (0x432DC0 and 0x432F90; invented names).
	static NiLight *	CreateNiPointLight( float red, float green, float blue, float dimmer, float linearAttenuation, char *name );
	static NiLight *	CreateNiSpotLight( float red, float green, float blue, float dimmer, float directionX, float directionY,
										  float directionZ, float angle, char *name );
	void	SetIntensity( float );
	float	GetIntensity( void );
	static LightObjectClass *	GetObject( SetClass *, char * );
	// the light object with the ID in the set, or in the light table when
	// the set is NULL (0x432B00; invented name, after
	// ObjectClass::GetObjectByID)
	static LightObjectClass *	GetObjectByID( SetClass *set, long id );
	static int	GetNumClassObjects( SetClass * );
	// the set's light objects and their number (invented name, as
	// Backdrop::GetClassObjects)
	static LightObjectClass **	GetClassObjects( SetClass *set, int *count );

protected:
	// The IDs of the objects the light shines on alone; empty when it shines on the
	// whole set (invented name).
	NiTList<long>	illuminatedObjects;		// 0x3c

	// The light placements of the editor configure the NetImmerse light.
	friend class LightPlacement;

	static TGEventHandlerTable	classHandlers;		// invented name

public:
	// The NetImmerse light the object shows; NULL when it shows another kind
	// of object (0x433520; invented name). The light placements and the
	// placement editor's light dialog read it.
	NiLight *	GetNiLight( void );
};

/*
================
LightObjectClass::IsTypeOf

Inline: PulsingLight's IsTypeOf (0x4416B0) expands it.
================
*/
// FUNCTION: STBC 0x00432c50 @game/LightObjectClass
inline bool LightObjectClass::IsTypeOf( int type ) {
	if ( type == LIGHTOBJECTCLASS_TYPE ) {
		return true;
	}
	return BaseObjectClass::IsTypeOf( type );
}

#endif /* !__LIGHTOBJECTCLASS_H__ */
