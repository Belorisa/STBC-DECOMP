/*
===============================================================================

	ShipProperty.h

	The members of ShipProperty that the SWIG wrappers ShipProperty_* call. Only what the
	wrappers prove is declared, and no virtual functions yet. The wrappers
	ShipProperty_GetGenus and ShipProperty_SetGenus (0x643FC0, 0x644470)
	read and write the genus (0x1C) inline.

===============================================================================
*/

#ifndef __SHIPPROPERTY_H__
#define __SHIPPROPERTY_H__

#include "TGModelProperty.h"

class TGStream;

// What kind of object a ship property describes (the SWIG constants
// GENUS_*, 0x904C28).
enum genus_t {
	GENUS_UNKNOWN,
	GENUS_SHIP,
	GENUS_STATION,
	GENUS_ASTEROID
};

// The type ShipProperty::Cast (0x691190) checks.
#define SHIPPROPERTY_TYPE		0x812E

// FUNCTION: STBC 0x00691470 ??_GShipProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ShipProperty : public TGModelProperty {
public:
					ShipProperty( long id = 0 );
	virtual			~ShipProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00691300 ?GetObjType@ShipProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return SHIPPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00691340 ?GetClassName@ShipProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ShipProperty"; }
	// FUNCTION: STBC 0x00691350 ?GetSwigTypeName@ShipProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ShipProperty"; }
	// FUNCTION: STBC 0x00691360 ?GetPythonClassName@ShipProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ShipPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static ShipProperty *	Cast( TGModelProperty * );
	static ShipProperty *	Create( char * );
	void	SetShipName( char * );
	void	SetModelFilename( char * );
	void	SetAIString( char * );
	void	SetDeathExplosionSound( char * );

	genus_t			GetGenus( void ) { return genus; }
	void			SetGenus( genus_t genus_ ) { genus = genus_; }
	// The species is also the number of the ship's icon in the ShipIcons
	// group (RadarScope::CreateShipIcon).
	int				GetSpecies( void ) { return species; }
	void			SetSpecies( int species_ ) { species = species_; }
	float			GetMass( void ) { return mass; }
	void			SetMass( float mass_ ) { mass = mass_; }
	float			GetRotationalInertia( void ) { return rotationalInertia; }
	void			SetRotationalInertia( float inertia ) { rotationalInertia = inertia; }
	char *			GetShipName( void ) { return shipName; }
	char *			GetModelFilename( void ) { return modelFilename; }
	float			GetDamageResolution( void ) { return damageResolution; }
	void			SetDamageResolution( float resolution ) { damageResolution = resolution; }
	int				GetAffiliation( void ) { return affiliation; }
	void			SetAffiliation( int affiliation_ ) { affiliation = affiliation_; }
	bool			IsStationary( void ) { return stationary; }
	void			SetStationary( bool stationary_ ) { stationary = stationary_; }
	char *			GetAIString( void ) { return aiString; }
	char *			GetDeathExplosionSound( void ) { return deathExplosionSound; }

protected:
	genus_t			genus;			// 0x1C
	int				species;		// 0x20 0 at first
	float			mass;			// 0x24 1 at first
	float			rotationalInertia;	// 0x28 1 at first
	char *			shipName;		// 0x2C owned (ShipProperty_SetShipName)
	char *			modelFilename;	// 0x30 owned (ShipProperty_SetModelFilename)
	float			damageResolution;	// 0x34 15 at first
	int				affiliation;	// 0x38 0 at first
	bool			stationary;		// 0x3C false at first
	char *			aiString;		// 0x40 owned, "" at first (ShipProperty_SetAIString)
	char *			deathExplosionSound;	// 0x44 owned, "" at first (ShipProperty_SetDeathExplosionSound)
};

/*
================
ShipProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00691310
inline bool ShipProperty::IsTypeOf( int type ) {
	if ( type == SHIPPROPERTY_TYPE ) {
		return true;
	}
	return TGModelProperty::IsTypeOf( type );
}

#endif /* !__SHIPPROPERTY_H__ */
