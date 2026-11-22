/*
===============================================================================

	PositionOrientationProperty.h

	0x50 bytes: PositionOrientationProperty::Create allocates that much. The out-of-line getters the
	SWIG wrappers PositionOrientationProperty_GetForward, GetUp, GetRight and GetPosition call
	(0x63A210, 0x63A2D0, 0x63A390, 0x582430) read the points at 0x20, 0x2C, 0x38 and 0x44, and
	SetPosition writes 0x44. 

===============================================================================
*/

#ifndef __POSITIONORIENTATIONPROPERTY_H__
#define __POSITIONORIENTATIONPROPERTY_H__

#include "NiPoint3.h"
#include "TGPoint3.h"
#include "TGModelProperty.h"

class TGStream;

class NiAVObject;
class NiNode;

// The type PositionOrientationProperty::Cast (0x688D00) checks.
#define POSITIONORIENTATIONPROPERTY_TYPE		0x812D

// FUNCTION: STBC 0x00688f80 ??_GPositionOrientationProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PositionOrientationProperty : public TGModelProperty {
public:
					PositionOrientationProperty( long id = 0 );
	virtual			~PositionOrientationProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00688e10 ?GetObjType@PositionOrientationProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return POSITIONORIENTATIONPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00688e50 ?GetClassName@PositionOrientationProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "PositionOrientationProperty"; }
	// FUNCTION: STBC 0x00688e60 ?GetSwigTypeName@PositionOrientationProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_PositionOrientationProperty"; }
	// FUNCTION: STBC 0x00688e70 ?GetPythonClassName@PositionOrientationProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "PositionOrientationPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static PositionOrientationProperty *	Cast( TGModelProperty * );
	static PositionOrientationProperty *	Create( char * );

	// The out-of-line copies are the ones the SWIG wrappers of the same
	// names call; Copy calls them too.
	// FUNCTION: STBC 0x00639fd0 ?SetOrientation@PositionOrientationProperty@@QAEXVTGPoint3@@00@Z
	void			SetOrientation( TGPoint3 forward_, TGPoint3 up_, TGPoint3 right_ ) { Unknown63A050( forward_, up_, right_ ); }
	// FUNCTION: STBC 0x0063a160 ?SetPosition@PositionOrientationProperty@@QAEXVTGPoint3@@@Z
	void			SetPosition( TGPoint3 position_ ) { position = position_; }
	// FUNCTION: STBC 0x0063a210 ?GetForward@PositionOrientationProperty@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetForward( void ) { return forward; }
	// FUNCTION: STBC 0x0063a2d0 ?GetUp@PositionOrientationProperty@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetUp( void ) { return up; }
	// FUNCTION: STBC 0x0063a390 ?GetRight@PositionOrientationProperty@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetRight( void ) { return right; }
	// FUNCTION: STBC 0x00582430 ?GetPosition@PositionOrientationProperty@@QAE?AVTGPoint3@@XZ
	TGPoint3		GetPosition( void ) { return position; }

protected:
	// Stores the three axes; only SetOrientation calls it.
	// FUNCTION: STBC 0x0063a050 ?Unknown63A050@PositionOrientationProperty@@IAEXVNiPoint3@@00@Z
	void			Unknown63A050( NiPoint3 forward_, NiPoint3 up_, NiPoint3 right_ ) { forward = forward_; up = up_; right = right_; }
	// Builds the display model's axes under the root node (unidentified).
	void			Unknown689410( NiNode *root );
	// The display model's geometry for the size (unidentified).
	NiAVObject *	Unknown689530( float size );
	// Resizes the display model's geometry (unidentified).
	void			Unknown689830( float size );

	NiNode *		debugNode;				// 0x1C built by slot 15, as SubsystemProperty's; NULL until then (invented name)
	NiPoint3		forward;				// 0x20
	NiPoint3		up;						// 0x2C
	NiPoint3		right;					// 0x38
	NiPoint3		position;				// 0x44
};

/*
================
PositionOrientationProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00688e20
inline bool PositionOrientationProperty::IsTypeOf( int type ) {
	if ( type == POSITIONORIENTATIONPROPERTY_TYPE ) {
		return true;
	}
	return TGModelProperty::IsTypeOf( type );
}

#endif /* !__POSITIONORIENTATIONPROPERTY_H__ */
