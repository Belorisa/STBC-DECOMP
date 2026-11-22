/*
===============================================================================

	TractorBeamProperty.h

	0x140 bytes: the SWIG wrapper new_TractorBeamProperty allocates that much.

	Evidence: the SWIG wrappers TractorBeamProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __TRACTORBEAMPROPERTY_H__
#define __TRACTORBEAMPROPERTY_H__

#include "EnergyWeaponProperty.h"
#include "NiColor.h"
#include "TGPoint3.h"

class TGModelProperty;

// The type TractorBeamProperty::Cast (0x6966B0) checks.
#define TRACTORBEAMPROPERTY_TYPE		0x8136

// FUNCTION: STBC 0x00696b20 ??_GTractorBeamProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TractorBeamProperty : public EnergyWeaponProperty {
public:
					TractorBeamProperty( long id = 0 );
	virtual			~TractorBeamProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00696990 ?GetObjType@TractorBeamProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return TRACTORBEAMPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006969f0 ?GetClassName@TractorBeamProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TractorBeamProperty"; }
	// FUNCTION: STBC 0x00696a00 ?GetSwigTypeName@TractorBeamProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TractorBeamProperty"; }
	// FUNCTION: STBC 0x00696a10 ?GetPythonClassName@TractorBeamProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TractorBeamPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static TractorBeamProperty *	Cast( TGModelProperty * );
	static TractorBeamProperty *	Create( char * );
	void	SetTextureName( char * );

	TGPoint3		GetOrientationForward( void );
	TGPoint3		GetOrientationUp( void );
	TGPoint3		GetOrientationRight( void );
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
	int				GetTextureStart( void ) { return textureStart; }
	void			SetTextureStart( int textureStart_ ) { textureStart = textureStart_; }
	int				GetTextureEnd( void ) { return textureEnd; }
	void			SetTextureEnd( int textureEnd_ ) { textureEnd = textureEnd_; }
	char *			GetTextureName( void ) { return textureName; }
	NiColorA		GetOuterShellColor( void ) { return outerShellColor; }
	NiColorA		GetInnerShellColor( void ) { return innerShellColor; }
	NiColorA		GetOuterCoreColor( void ) { return outerCoreColor; }
	NiColorA		GetInnerCoreColor( void ) { return innerCoreColor; }
	// The copies kept out of line are the SWIG wrappers' (Copy calls them).
	// FUNCTION: STBC 0x00640b20 ?SetOuterShellColor@TractorBeamProperty@@QAEXVNiColorA@@@Z
	void			SetOuterShellColor( NiColorA color ) { outerShellColor = color; }
	// FUNCTION: STBC 0x00640c10 ?SetInnerShellColor@TractorBeamProperty@@QAEXVNiColorA@@@Z
	void			SetInnerShellColor( NiColorA color ) { innerShellColor = color; }
	// FUNCTION: STBC 0x00640d00 ?SetOuterCoreColor@TractorBeamProperty@@QAEXVNiColorA@@@Z
	void			SetOuterCoreColor( NiColorA color ) { outerCoreColor = color; }
	// FUNCTION: STBC 0x00640df0 ?SetInnerCoreColor@TractorBeamProperty@@QAEXVNiColorA@@@Z
	void			SetInnerCoreColor( NiColorA color ) { innerCoreColor = color; }
	int				GetNumSides( void ) { return numSides; }
	void			SetNumSides( int numSides_ ) { numSides = numSides_; }
	float			GetMainRadius( void ) { return mainRadius; }
	void			SetMainRadius( float mainRadius_ ) { mainRadius = mainRadius_; }
	float			GetTaperRadius( void ) { return taperRadius; }
	void			SetTaperRadius( float taperRadius_ ) { taperRadius = taperRadius_; }
	float			GetCoreScale( void ) { return coreScale; }
	void			SetCoreScale( float coreScale_ ) { coreScale = coreScale_; }
	float			GetTaperRatio( void ) { return taperRatio; }
	void			SetTaperRatio( float taperRatio_ ) { taperRatio = taperRatio_; }
	float			GetTaperMinLength( void ) { return taperMinLength; }
	void			SetTaperMinLength( float taperMinLength_ ) { taperMinLength = taperMinLength_; }
	float			GetTaperMaxLength( void ) { return taperMaxLength; }
	void			SetTaperMaxLength( float taperMaxLength_ ) { taperMaxLength = taperMaxLength_; }
	float			GetLengthTextureTilePerUnit( void ) { return lengthTextureTilePerUnit; }
	void			SetLengthTextureTilePerUnit( float lengthTextureTilePerUnit_ ) { lengthTextureTilePerUnit = lengthTextureTilePerUnit_; }
	float			GetPerimeterTile( void ) { return perimeterTile; }
	void			SetPerimeterTile( float perimeterTile_ ) { perimeterTile = perimeterTile_; }
	float			GetTextureSpeed( void ) { return textureSpeed; }
	void			SetTextureSpeed( float textureSpeed_ ) { textureSpeed = textureSpeed_; }
	float			GetTractorBeamWidth( void ) { return tractorBeamWidth; }
	void			SetTractorBeamWidth( float tractorBeamWidth_ ) { tractorBeamWidth = tractorBeamWidth_; }

protected:
	// Builds the display model's geometry under the root node (unidentified).
	void			Unknown697500( NiNode *root );
	// The display model's pieces and their updates (unidentified).
	NiAVObject *	Unknown697760( float radius );
	NiAVObject *	Unknown697BC0( float size );
	void			Unknown697970( float radius );
	void			Unknown697FE0( float size );

	NiNode *		debugNode;				// 0x90 built by slot 15, as SubsystemProperty's; NULL until then; saved as its address and resolved on load (invented name)
	float			arcWidthAngleMin;		// 0x94
	float			arcWidthAngleMax;		// 0x98
	float			arcHeightAngleMin;		// 0x9C
	float			arcHeightAngleMax;		// 0xA0
	NiPoint3		orientationForward;		// 0xA4 the projector's facing, in the ship's frame
	NiPoint3		orientationUp;			// 0xB0
	NiPoint3		orientationRight;		// 0xBC (the name follows TractorBeamProperty_GetOrientationRight)
	int				textureStart;			// 0xC8
	int				textureEnd;				// 0xCC
	char *			textureName;			// 0xD0 SetTextureName sets it
	NiColorA		outerShellColor;		// 0xD4
	NiColorA		innerShellColor;		// 0xE4
	NiColorA		outerCoreColor;			// 0xF4
	NiColorA		innerCoreColor;			// 0x104
	int				numSides;				// 0x114
	float			mainRadius;				// 0x118
	float			taperRadius;			// 0x11C
	float			coreScale;				// 0x120
	float			taperRatio;				// 0x124
	float			taperMinLength;			// 0x128
	float			taperMaxLength;			// 0x12C
	float			lengthTextureTilePerUnit; // 0x130
	float			perimeterTile;			// 0x134
	float			textureSpeed;			// 0x138
	float			tractorBeamWidth;		// 0x13C
};

/*
================
TractorBeamProperty::GetOrientationForward
================
*/
// FUNCTION: STBC 0x0057ead0
inline TGPoint3 TractorBeamProperty::GetOrientationForward( void ) {
	return orientationForward;
}

/*
================
TractorBeamProperty::GetOrientationUp
================
*/
// FUNCTION: STBC 0x0057eb30
inline TGPoint3 TractorBeamProperty::GetOrientationUp( void ) {
	return orientationUp;
}

/*
================
TractorBeamProperty::GetOrientationRight
================
*/
// FUNCTION: STBC 0x0057f530
inline TGPoint3 TractorBeamProperty::GetOrientationRight( void ) {
	return orientationRight;
}

/*
================
TractorBeamProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x006969a0
inline bool TractorBeamProperty::IsTypeOf( int type ) {
	if ( type == TRACTORBEAMPROPERTY_TYPE ) {
		return true;
	}
	return EnergyWeaponProperty::IsTypeOf( type );
}

#endif /* !__TRACTORBEAMPROPERTY_H__ */
