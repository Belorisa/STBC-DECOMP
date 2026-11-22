/*
===============================================================================

	SubsystemProperty.h

	The property a ship subsystem is built from: its condition, position,
	radius and repair figures, as the hardpoint scripts set them.

	0x48 bytes. Evidence: the constructor (0x692470) calls
	TGModelProperty's, stores the vtable 0x895384 and sets max condition
	0x20 (200), critical 0x24, targetable 0x25, 0x26, the position 0x28
	(NiPoint3::ZERO), 0x34 (NiPoint2::ZERO), repair complexity 0x3C (1),
	disabled percentage 0x40 (0.5) and radius 0x44 (0.25). ShipSubsystem's
	accessors of the same names (GetMaxCondition, IsCritical,
	IsTargetable, GetRepairComplexity, GetDisabledPercentage, GetRadius)
	read them. Slot 15 builds a node at 0x1C that shows the subsystem on
	the model; the destructor detaches it. 

===============================================================================
*/

#ifndef __SUBSYSTEMPROPERTY_H__
#define __SUBSYSTEMPROPERTY_H__

#include "TGModelProperty.h"
#include "NiPoint2.h"
#include "TGPoint3.h"

class NiAVObject;
class NiNode;

// The type SubsystemProperty::Cast (0x6923F0) checks.
#define SUBSYSTEMPROPERTY_TYPE		0x812B

// FUNCTION: STBC 0x00692650 ??_GSubsystemProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class SubsystemProperty : public TGModelProperty {
public:
					SubsystemProperty( long id = 0 );
	virtual			~SubsystemProperty( void );

	// TGObject
	// FUNCTION: STBC 0x006924e0 ?GetObjType@SubsystemProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return SUBSYSTEMPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00692520 ?GetClassName@SubsystemProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "SubsystemProperty"; }
	// FUNCTION: STBC 0x00692530 ?GetSwigTypeName@SubsystemProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_SubsystemProperty"; }
	// FUNCTION: STBC 0x00692540 ?GetPythonClassName@SubsystemProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "SubsystemPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static SubsystemProperty *	Cast( TGModelProperty * );
	static SubsystemProperty *	Create( char * );

	float			GetMaxCondition( void ) { return maxCondition; }
	void			SetMaxCondition( float condition ) { maxCondition = condition; }
	bool			IsCritical( void ) { return critical; }
	void			SetCritical( bool critical_ ) { critical = critical_; }
	bool			IsTargetable( void ) { return targetable; }
	void			SetTargetable( bool targetable_ ) { targetable = targetable_; }
	bool			IsPrimary( void ) { return primary; }
	void			SetPrimary( bool primary_ ) { primary = primary_; }
	NiPoint3		GetPosition( void ) { return position; }
	// FUNCTION: STBC 0x0063ae20 ?GetPositionTG@SubsystemProperty@@QAE?AVTGPoint3@@XZ @game/AppWrap
	TGPoint3		GetPositionTG( void ) { return position; }
	// The copy kept out of line in the class's unit (Copy calls it).
	// FUNCTION: STBC 0x006927a0 ?SetPosition@SubsystemProperty@@QAEXVNiPoint3@@@Z
	void			SetPosition( NiPoint3 position_ ) { position = position_; }
	NiPoint2		GetPosition2D( void ) { return position2D; }
	void			GetPosition2D( float *x, float *y ) { *x = position2D.x; *y = position2D.y; }
	void			SetPosition( float x, float y, float z ) { position = NiPoint3( x, y, z ); }
	void			SetPosition2D( float x, float y ) { position2D = NiPoint2( x, y ); }
	// FUNCTION: STBC 0x006927c0 ?SetPosition2D@SubsystemProperty@@QAEXVNiPoint2@@@Z
	void			SetPosition2D( NiPoint2 position ) { position2D = position; }
	float			GetRepairComplexity( void ) { return repairComplexity; }
	void			SetRepairComplexity( float complexity ) { repairComplexity = complexity; }
	float			GetDisabledPercentage( void ) { return disabledPercentage; }
	void			SetDisabledPercentage( float percentage ) { disabledPercentage = percentage; }
	float			GetRadius( void ) { return radius; }
	void			SetRadius( float radius_ ) { radius = radius_; }

protected:
	// Builds the display model's geometry under the root node (unidentified).
	void			Unknown692B10( NiNode *root );
	// The display model's geometry for the radius (unidentified).
	NiAVObject *	Unknown692CC0( float radius );
	// Resizes the display model's geometry (unidentified).
	void			Unknown6930B0( float radius );

	NiNode *		debugNode;			// 0x1C built by slot 15; NULL until then (invented name)
	float			maxCondition;		// 0x20
	bool			critical;			// 0x24
	bool			targetable;			// 0x25
	bool			primary;			// 0x26 true at first
	NiPoint3		position;			// 0x28 in the model's frame
	NiPoint2		position2D;			// 0x34
	float			repairComplexity;	// 0x3C
	float			disabledPercentage;	// 0x40 the condition percentage below which the subsystem is disabled
	float			radius;				// 0x44
};

/*
================
SubsystemProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x006924f0
inline bool SubsystemProperty::IsTypeOf( int type ) {
	if ( type == SUBSYSTEMPROPERTY_TYPE ) {
		return true;
	}
	return TGModelProperty::IsTypeOf( type );
}

#endif /* !__SUBSYSTEMPROPERTY_H__ */
