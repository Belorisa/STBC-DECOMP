/*
===============================================================================

	LightPlacement.h

	A placement that stands for a light of its set: the editor configures it
	as an ambient or a directional light, and the saved script recreates it
	with LightPlacement_Create and ConfigAmbientLight or
	ConfigDirectionalLight.

	0x74 bytes. Evidence: the constructor (0x49A180) runs PlacementObject's,
	clears 0x70 and stores the vtable 0x88A920 (71 slots, PlacementObject's
	with 13 replaced); the deleting destructor (0x441090) frees 0x74 bytes
	to the pool. 0x70 is the ID of the LightObjectClass the placement made:
	the Config functions store the new light's ID there and look the light
	up by it in the containing set, and the stream functions write and read
	it as an ID. The type functions (slots 1, 2, 9 to 11 and 19) are inline
	copies kept beside the class factories (0x440FE0 to 0x441090).

===============================================================================
*/

#ifndef __LIGHTPLACEMENT_H__
#define __LIGHTPLACEMENT_H__

#include "PlacementObject.h"

class LightObjectClass;
class TGObject;
class TGEventHandlerTable;

// GetObjType's value for a LightPlacement (the type Cast checks, 0x49A150).
#define LIGHTPLACEMENT_TYPE		0x80E1

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// The kinds of light GetLightType tells apart (invented names).
enum lightPlacementType_t {
	LIGHT_TYPE_NONE,
	LIGHT_TYPE_AMBIENT,
	LIGHT_TYPE_DIRECTIONAL,
	LIGHT_TYPE_POINT,
	LIGHT_TYPE_SPOT
};

// FUNCTION: STBC 0x00441090 ??_GLightPlacement@@UAEPAXI@Z
class LightPlacement : public PlacementObject {
public:
	// FUNCTION: STBC 0x00440fc0 ??0LightPlacement@@QAE@J@Z @game/UtopiaModule
					LightPlacement( long id ) : PlacementObject( id ) {}	// the loader's: the stream fills the members
					LightPlacement( char *name, char *setName, char *modelName );
	static LightPlacement *	Create( char *name, char *setName, char *modelName ) { return new LightPlacement( name, setName, modelName ); }	// the SWIG wrapper LightPlacement_Create expands it
	virtual			~LightPlacement( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ObjectClass
	virtual void	Update( float time );

	// PlacementObject
	virtual bool	VirtualSlot68( FILE *file, char *setName );
	virtual bool	VirtualSlot69( FILE *file );

	static LightPlacement *	Cast( TGObject * );
	void	ConfigAmbientLight( float, float, float, float );
	void	ConfigDirectionalLight( float, float, float, float );
	void	ConfigPointLight( float, float, float, float, float );
	void	ConfigSpotLight( float, float, float, float, float, float, float, float );

	// the kind of the light the placement made (invented name)
	int				GetLightType( void );
	// the light the placement made, from its containing set (invented name)
	LightObjectClass *	GetLight( void );

protected:

	long			lightID;			// 0x70 the LightObjectClass's ID (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
LightPlacement::GetObjType
================
*/
// FUNCTION: STBC 0x00440fe0
inline int LightPlacement::GetObjType( void ) {
	return LIGHTPLACEMENT_TYPE;
}

/*
================
LightPlacement::IsTypeOf
================
*/
// FUNCTION: STBC 0x00440ff0
inline bool LightPlacement::IsTypeOf( int type ) {
	if ( type == LIGHTPLACEMENT_TYPE ) {
		return true;
	}
	return PlacementObject::IsTypeOf( type );
}

/*
================
LightPlacement::GetClassName
================
*/
// FUNCTION: STBC 0x00441050
inline char *LightPlacement::GetClassName( void ) {
	return "LightPlacement";
}

/*
================
LightPlacement::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00441060
inline char *LightPlacement::GetSwigTypeName( void ) {
	return "_p_LightPlacement";
}

/*
================
LightPlacement::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00441070
inline char *LightPlacement::GetPythonClassName( void ) {
	return "LightPlacementPtr";
}

/*
================
LightPlacement::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00441080
inline TGEventHandlerTable *LightPlacement::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__LIGHTPLACEMENT_H__ */
