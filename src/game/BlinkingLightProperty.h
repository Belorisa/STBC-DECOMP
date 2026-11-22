/*
===============================================================================

	BlinkingLightProperty.h

	0x70 bytes: BlinkingLightProperty::Create allocates that much. The SWIG wrappers
	BlinkingLightProperty_* read the members inline (offsets at the members); SetColor writes the
	colour at 0x50. 

===============================================================================
*/

#ifndef __BLINKINGLIGHTPROPERTY_H__
#define __BLINKINGLIGHTPROPERTY_H__

#include "EffectEmitterProperty.h"
#include "TGColorA.h"

class TGStream;

class TGModelProperty;

// The type BlinkingLightProperty::Cast (0x67B510) checks.
#define BLINKINGLIGHTPROPERTY_TYPE		0x8145

// FUNCTION: STBC 0x0067b7e0 ??_GBlinkingLightProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class BlinkingLightProperty : public EffectEmitterProperty {
public:
					BlinkingLightProperty( long id = 0 );
	virtual			~BlinkingLightProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0067b660 ?GetObjType@BlinkingLightProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return BLINKINGLIGHTPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0067b6b0 ?GetClassName@BlinkingLightProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "BlinkingLightProperty"; }
	// FUNCTION: STBC 0x0067b6c0 ?GetSwigTypeName@BlinkingLightProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_BlinkingLightProperty"; }
	// FUNCTION: STBC 0x0067b6d0 ?GetPythonClassName@BlinkingLightProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "BlinkingLightPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual bool	Copy( TGModelProperty *property );

	static BlinkingLightProperty *	Cast( TGModelProperty * );
	static BlinkingLightProperty *	Create( char * );
	void	SetTextureName( char * );

	// The copies kept out of line are the SWIG wrappers' (Copy calls them).
	// FUNCTION: STBC 0x005b5380 ?GetColor@BlinkingLightProperty@@QAE?AVTGColorA@@XZ
	TGColorA		GetColor( void ) { return color; }
	// FUNCTION: STBC 0x00646670 ?SetColor@BlinkingLightProperty@@QAEXVTGColorA@@@Z
	void			SetColor( TGColorA color_ ) { color = color_; }
	void			SetRadius( float radius_ ) { radius = radius_; }
	void			SetPeriod( float period_ ) { period = period_; }
	void			SetDuration( float duration_ ) { duration = duration_; }
	float			GetRadius( void ) { return radius; }
	float			GetPeriod( void ) { return period; }
	float			GetDuration( void ) { return duration; }
	char *			GetTextureName( void ) { return textureName; }

protected:
	TGColorA		color;					// 0x50
	float			radius;					// 0x60
	float			period;					// 0x64
	float			duration;				// 0x68
	char *			textureName;			// 0x6C SetTextureName sets it
};

/*
================
BlinkingLightProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0067b670
inline bool BlinkingLightProperty::IsTypeOf( int type ) {
	if ( type == BLINKINGLIGHTPROPERTY_TYPE ) {
		return true;
	}
	return EffectEmitterProperty::IsTypeOf( type );
}

#endif /* !__BLINKINGLIGHTPROPERTY_H__ */
