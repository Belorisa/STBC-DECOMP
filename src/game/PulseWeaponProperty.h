/*
===============================================================================

	PulseWeaponProperty.h

	0xD0 bytes: the SWIG wrapper new_PulseWeaponProperty allocates that much.

	Evidence: the SWIG wrappers PulseWeaponProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __PULSEWEAPONPROPERTY_H__
#define __PULSEWEAPONPROPERTY_H__

#include "EnergyWeaponProperty.h"
#include "TGPoint3.h"

class TGModelProperty;

// The type PulseWeaponProperty::Cast (0x68C0F0) checks.
#define PULSEWEAPONPROPERTY_TYPE		0x8135

// FUNCTION: STBC 0x0068c460 ??_GPulseWeaponProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PulseWeaponProperty : public EnergyWeaponProperty {
public:
					PulseWeaponProperty( long id = 0 );
	virtual			~PulseWeaponProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0068c2d0 ?GetObjType@PulseWeaponProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return PULSEWEAPONPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0068c330 ?GetClassName@PulseWeaponProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "PulseWeaponProperty"; }
	// FUNCTION: STBC 0x0068c340 ?GetSwigTypeName@PulseWeaponProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_PulseWeaponProperty"; }
	// FUNCTION: STBC 0x0068c350 ?GetPythonClassName@PulseWeaponProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "PulseWeaponPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static PulseWeaponProperty *	Cast( TGModelProperty * );
	static PulseWeaponProperty *	Create( char * );
	void	SetModuleName( char * );

	// Sets the forward and up axes; right is their cross product.
	void			SetOrientation( const NiPoint3 &forward, const NiPoint3 &up );
	void			SetOrientation( const TGPoint3 &forward, const TGPoint3 &up ) { SetOrientation( (NiPoint3)forward, (NiPoint3)up ); }
	void			SetArcWidthAngles( float min, float max ) { arcWidthAngleMin = min; arcWidthAngleMax = max; }
	void			SetArcHeightAngles( float min, float max ) { arcHeightAngleMin = min; arcHeightAngleMax = max; }
	void			GetArcWidthAngles( float *min, float *max ) { *min = arcWidthAngleMin; *max = arcWidthAngleMax; }
	void			GetArcHeightAngles( float *min, float *max ) { *min = arcHeightAngleMin; *max = arcHeightAngleMax; }
	float			GetArcWidthAngleMin( void ) { return arcWidthAngleMin; }
	float			GetArcWidthAngleMax( void ) { return arcWidthAngleMax; }
	float			GetArcHeightAngleMin( void ) { return arcHeightAngleMin; }
	float			GetArcHeightAngleMax( void ) { return arcHeightAngleMax; }
	float			GetCooldownTime( void ) { return cooldownTime; }
	void			SetCooldownTime( float cooldownTime_ ) { cooldownTime = cooldownTime_; }
	char *			GetModuleName( void ) { return moduleName; }
	TGPoint3		GetOrientationForward( void );
	TGPoint3		GetOrientationUp( void );
	TGPoint3		GetOrientationRight( void );

protected:
	// Builds the display model's geometry under the root node (unidentified).
	void			Unknown68C980( NiNode *root );
	// The display model's pieces and their updates (unidentified).
	NiAVObject *	Unknown68CBE0( float radius );
	NiAVObject *	Unknown68D040( float size );
	void			Unknown68CDF0( float radius );
	void			Unknown68D460( float size );

	NiNode *		debugNode;				// 0x90 built by slot 15, as SubsystemProperty's; NULL until then; saved as its address and resolved on load (invented name)
	float			arcWidthAngleMin;		// 0x94
	float			arcWidthAngleMax;		// 0x98
	float			arcHeightAngleMin;		// 0x9C
	float			arcHeightAngleMax;		// 0xA0
	NiPoint3		orientationForward;		// 0xA4
	NiPoint3		orientationUp;			// 0xB0
	NiPoint3		orientationRight;		// 0xBC
	float			cooldownTime;			// 0xC8
	char *			moduleName;				// 0xCC SetModuleName sets it
};

/*
================
PulseWeaponProperty::GetOrientationForward
================
*/
// FUNCTION: STBC 0x00484a20 @game/AppWrap
inline TGPoint3 PulseWeaponProperty::GetOrientationForward( void ) {
	return orientationForward;
}

/*
================
PulseWeaponProperty::GetOrientationUp
================
*/
// FUNCTION: STBC 0x00575d50 @game/AppWrap
inline TGPoint3 PulseWeaponProperty::GetOrientationUp( void ) {
	return orientationUp;
}

/*
================
PulseWeaponProperty::GetOrientationRight
================
*/
// FUNCTION: STBC 0x00575d80 @game/AppWrap
inline TGPoint3 PulseWeaponProperty::GetOrientationRight( void ) {
	return orientationRight;
}

/*
================
PulseWeaponProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0068c2e0
inline bool PulseWeaponProperty::IsTypeOf( int type ) {
	if ( type == PULSEWEAPONPROPERTY_TYPE ) {
		return true;
	}
	return EnergyWeaponProperty::IsTypeOf( type );
}

#endif /* !__PULSEWEAPONPROPERTY_H__ */
