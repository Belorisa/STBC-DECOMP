/*
===============================================================================

	Sun.h

	A planet that shines: its model is built from two textures. 0x84
	bytes, allocated from the pools.

	Evidence: the SWIG wrapper Sun_Create ("f|ffOO", the first optional
	float 125 by default) allocates 0x84 bytes and calls the constructor
	(0x5A4680), which calls Planet's with the first two floats and no model
	and stores the vtable 0x88A5D4: Planet's 69 slots, none added.
	GetObjType (0x440B20) returns 0x8005, the type Cast checks.
	UtopiaModule's loader creator (0x440AC0) expands the loading
	constructor (0x440B00), so its unit keeps the inline virtuals and both
	destructors.

===============================================================================
*/

#ifndef __SUN_H__
#define __SUN_H__

#include "Planet.h"

class TGObject;

// GetObjType's value for a Sun (the type its Cast checks).
#define SUN_TYPE		0x8005

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00440bd0 ??_GSun@@UAEPAXI@Z @game/UtopiaModule
class Sun : public Planet {
public:
	// unknown_ is not used.
					Sun( float radius_, float atmosphereRadius_, float unknown_, char *unknown7C_, char *unknown80_ );
	// The script-side constructor (the SWIG wrapper Sun_Create expands it).
	static Sun *	Create( float radius, float atmosphereRadius, float unknown, char *baseTexture, char *flareTexture ) { return new Sun( radius, atmosphereRadius, unknown, baseTexture, flareTexture ); }
	// FUNCTION: STBC 0x00440b00 ??0Sun@@QAE@J@Z @game/UtopiaModule
					Sun( long id ) : Planet( id ) {}	// the loader's: the stream sets the rest
	// FUNCTION: STBC 0x00440c00 ??1Sun@@UAE@XZ @game/UtopiaModule
	virtual			~Sun( void ) {
		delete [] unknown7C;
		delete [] unknown80;
	}

	// TGObject
	// FUNCTION: STBC 0x00440b20 ?GetObjType@Sun@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return SUN_TYPE; }
	// FUNCTION: STBC 0x00440b30 ?IsTypeOf@Sun@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == SUN_TYPE ) {
			return true;
		}
		return Planet::IsTypeOf( type );
	}
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00440b90 ?GetClassName@Sun@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "Sun"; }
	// FUNCTION: STBC 0x00440ba0 ?GetSwigTypeName@Sun@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_Sun"; }
	// FUNCTION: STBC 0x00440bb0 ?GetPythonClassName@Sun@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "SunPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00440bc0 ?GetClassHandlers@Sun@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	static Sun *	Cast( TGObject * );

protected:
	// Builds the sun's model from the two textures, at the radius and the
	// atmosphere (125 when it is 0), and makes it the object's (SunEffect::Create
	// builds it).
	void			Unknown5A4780( void );

	char *			unknown7C;			// 0x7C owned: a texture name (the base texture SunEffect::Create takes)
	char *			unknown80;			// 0x80 owned: a texture name
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__SUN_H__ */
