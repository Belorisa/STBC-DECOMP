/*
===============================================================================

	TorpedoTubeProperty.h

	0x90 bytes: the SWIG wrapper new_TorpedoTubeProperty allocates that much.

	Evidence: the SWIG wrappers TorpedoTubeProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __TORPEDOTUBEPROPERTY_H__
#define __TORPEDOTUBEPROPERTY_H__

#include "WeaponProperty.h"
#include "TGPoint3.h"

class TGModelProperty;

// The type TorpedoTubeProperty::Cast (0x694FE0) checks.
#define TORPEDOTUBEPROPERTY_TYPE		0x8134

// FUNCTION: STBC 0x00695290 ??_GTorpedoTubeProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TorpedoTubeProperty : public WeaponProperty {
public:
					TorpedoTubeProperty( long id = 0 );
	virtual			~TorpedoTubeProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00695110 ?GetObjType@TorpedoTubeProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return TORPEDOTUBEPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00695160 ?GetClassName@TorpedoTubeProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TorpedoTubeProperty"; }
	// FUNCTION: STBC 0x00695170 ?GetSwigTypeName@TorpedoTubeProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TorpedoTubeProperty"; }
	// FUNCTION: STBC 0x00695180 ?GetPythonClassName@TorpedoTubeProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TorpedoTubePropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static TorpedoTubeProperty *	Cast( TGModelProperty * );
	static TorpedoTubeProperty *	Create( char * );

	// The out-of-line copies are TorpedoTube's.
	// FUNCTION: STBC 0x0057c370 ?GetDirection@TorpedoTubeProperty@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetDirection( void ) { return direction; }
	void			SetDirection( TGPoint3 *direction_ ) { direction = *direction_; }
	// FUNCTION: STBC 0x0057c3d0 ?GetRight@TorpedoTubeProperty@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetRight( void ) { return right; }
	void			SetRight( TGPoint3 *right_ ) { right = *right_; }
	float			GetImmediateDelay( void ) { return immediateDelay; }
	void			SetImmediateDelay( float immediateDelay_ ) { immediateDelay = immediateDelay_; }
	float			GetReloadDelay( void ) { return reloadDelay; }
	void			SetReloadDelay( float reloadDelay_ ) { reloadDelay = reloadDelay_; }
	int				GetMaxReady( void ) { return maxReady; }
	void			SetMaxReady( int maxReady_ ) { maxReady = maxReady_; }

protected:
	// Builds the display model's geometry under the root node (unidentified).
	void			Unknown695560( NiNode *root );

	NiNode *		debugNode;				// 0x68 built by slot 15, as SubsystemProperty's; NULL until then (invented name)
	TGPoint3		direction;				// 0x6C the launch direction, in the ship's frame
	TGPoint3		right;					// 0x78
	float			immediateDelay;			// 0x84
	float			reloadDelay;			// 0x88
	int				maxReady;				// 0x8C
};

/*
================
TorpedoTubeProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00695120
inline bool TorpedoTubeProperty::IsTypeOf( int type ) {
	if ( type == TORPEDOTUBEPROPERTY_TYPE ) {
		return true;
	}
	return WeaponProperty::IsTypeOf( type );
}

#endif /* !__TORPEDOTUBEPROPERTY_H__ */
